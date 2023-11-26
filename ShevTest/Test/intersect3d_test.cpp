
#include <math.h>
#include <stdio.h>

#include "../draw.h"

#include "../Extra/ex_intersect3d.h"
#include "../Shev/rand.h"
#include "../Shev/typedef.h"
#include "../Shev/template.h"
#include "../Shev/Vector3d.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/intersect2d.h"
#include "../Shev/intersect3d.h"
#include "../Shev/func3d.h"
#include "../Shev/RealFile.h"
#include "../Shev/filePYH.h"
#include "../Shev/trans.h"
#include "../Shev/func1t.h"
#include "../Shev/func2d.h"
#include "PolyhedronErrorRecipient.h"

#include "display.h"

double timeInSec();
bool loadSTL ( const char * filename, Polyhedron & poly );
bool cut ( const Polyhedron & poly, int n, const Plane3d * plane, int mark, Polyhedron & res );

namespace
{

void intersection_test ()
{
    static PRand rand1;
    static PRandVector3d rand3;
    const double eps = 1e-12;
    nat i;
    for ( i = 0; i < 10000; ++i )
    {
        Plane3d plane1, plane2, plane3;
        plane1.dist = rand1();
        plane2.dist = rand1();
        plane3.dist = rand1();
        plane1.norm = rand3();
        plane2.norm = rand3();
        plane3.norm = rand3();
        const Def<Vector3d> point1 = intersection ( plane1, plane2, plane3 );
        if ( ! point1.isDef || fabs ( plane1 % point1 ) > eps || 
            fabs ( plane2 % point1 ) > eps || fabs ( plane3 % point1 ) > eps )
        {
            display << i << "ppp";
            if ( point1.isDef )
            {
                const double d1 = fabs ( plane1 % point1 );
                const double d2 = fabs ( plane2 % point1 );
                const double d3 = fabs ( plane3 % point1 );
                display << d1 << d2 << d3;
            }
            else
            {
                intersection ( plane1, plane2, plane3 );
                display << "err";
            }
            display << NL;
        }
        const Def<Line3d> line = intersection ( plane1, plane2 );
        if ( ! line.isDef || fabs ( plane1 % line.point ) > eps || fabs ( plane2 % line.point ) > eps || 
            fabs ( plane1 % ( line.dir + line.point ) ) > eps || fabs ( plane2 % ( line.dir + line.point ) ) > eps )
            display << i << "pp" << NL;
        const Def<Vector3d> point2 = intersection ( plane3, line );
        if ( ! point2.isDef || fabs ( plane1 % point2 ) > eps || 
            fabs ( plane2 % point2 ) > eps || fabs ( plane3 % point2 ) > eps )
        {
//            const double d = fabs ( plane % point );
            display << i << "pl" << NL;
            if ( point1.isDef )
            {
                const double d1 = fabs ( plane1 % point2 );
                const double d2 = fabs ( plane2 % point2 );
                const double d3 = fabs ( plane3 % point2 );
            }
        }
    }
    display << "end" << NL;
}

void intersectPlaneSegment()
{
    static PRandPoint3d rand;
    nat nn = 0, n0 = 0;
    for ( nat i = 0; i < 10000; ++i )
    {
        const Segment3d seg ( rand(), rand() );
        const Plane3d plane ( rand().setNorm2(), rand().x );
        const Def<Vector3d> p = intersection ( plane, seg );
        if ( p.isDef )
        {
            ++nn;
            const double d = fabs ( plane % p );
            if ( d == 0 ) ++n0;
            if ( d > 2e-16 )
                display << d << NL;
        }
    }
    display << nn << n0 << NL;
}

void cutPlaneSegment()
{
    static PRandPoint3d rand;
    nat nn = 0;
    for ( nat i = 0; i < 1000; ++i )
    {
        const Segment3d seg ( rand(), rand() );
        const Plane3d plane ( rand().setNorm2(), rand().x );
        const Def<Segment3d> res = cut ( plane, seg );
        if ( res.isDef )
        {
            const double da = plane % res.a;
            const double db = plane % res.b;
            const double max = _max ( da, db );
            if ( max > 1e-16 )
                display << max << NL;
            ++nn;
        }
    }
    display << nn << NL;
}

void intersectLineSphere()
{
    static PRand rand;
    static PRandPoint3d prand;
    nat nn = 0, n0 = 0;
    for ( nat i = 0; i < 10000; ++i )
    {
        const Sphere3d cir ( rand(), prand() );
        const Line3d line ( prand(), prand() );
        const Def<Segment3d> seg = intersection ( line, cir );
        if ( seg.isDef )
        {
            const double da = normU ( line.dir % ( line.point - seg.a ) );
            const double db = normU ( line.dir % ( line.point - seg.b ) );
            const double ca = fabs ( norm2 ( seg.a - cir.o ) - cir.r );
            const double cb = fabs ( norm2 ( seg.b - cir.o ) - cir.r );
            const double e = 5e-16;
            if ( ca > e || cb > e || da > e || db > e )
                display << ca << cb << da << db << NL;
            ++n0;
        }
        ++nn;
    }
    display << nn << n0 << NL;
}

void intersectSegmentSphere()
{
    static PRand rand;
    static PRandPoint3d prand;
    const Sphere3d cir ( rand(), prand() );
    const Segment3d seg ( 1.5*prand(), 1.5*prand() );
    draw ( seg, 1, 0, 0 );
    draw ( cir, 0, 1, 1, 1, VM_WIRE );
    const Def<Segment3d> res = intersection ( seg, cir );
    if ( res.isDef )
    {
        draw ( res, 1, 1, 0 );
        const double ca = norm2 ( res.a - cir.o ) - cir.r;
        const double cb = norm2 ( res.b - cir.o ) - cir.r;
        const double e = 5e-16;
        if ( ca > e || cb > e )
            display << ca << cb << NL;
    }
}

void intersectLineEllipsoid()
{
    static PRand rand;
    static PRandPoint3d prand;
    nat nn = 0, n0 = 0;
    for ( nat i = 0; i < 1000; ++i )
    {
        Ellipsoid3d elli;
        elli.a = 0.2 + rand();
        elli.b = 0.2 + rand();
        elli.c = 0.2 + rand();
        elli.spin = Spin3d ( prand(), rand() );
        elli.o = prand();
        const Line3d line ( prand(), prand() );
        const Def<Segment3d> seg = intersection ( line, elli );
        if ( seg.isDef )
        {
            const double da = normU ( line.dir % ( line.point - seg.a ) );
            const double db = normU ( line.dir % ( line.point - seg.b ) );
            const double e = 5e-16;
            if ( da > e || db > e )
                display << da << db << NL;
            ++n0;
        }
        ++nn;
    }
    display << nn << n0 << NL;
}

void intersectSegmentEllipsoid()
{
    static PRand rand;
    static PRandPoint3d prand;
    Ellipsoid3d elli;
    elli.a = 0.2 + rand();
    elli.b = 0.2 + rand();
    elli.c = 0.2 + rand();
    elli.spin = Spin3d ( prand(), rand() );
    elli.o = prand();
    const Segment3d seg ( 1.5*prand(), 1.5*prand() );
    draw ( seg, 1, 0, 0 );
    draw ( elli, 0, 1, 1, 1, VM_WIRE );
    const Def<Segment3d> res = intersection ( seg, elli );
    if ( res.isDef )
    {
        draw ( res, 1, 1, 0 );
    }
}

void intersectPlanePolyhedron()
{
//    RealFile file ( "data/boart0858.pyh", "rb" );
//    RealFile file ( "data/boart1246.pyh", "rb" );
    RealFile file ( "data/boart1467.pyh", "rb" );
    Polyhedron poly;
    if ( ! loadPYH (  file, poly ) )
    {
        display << "Not load pyh-file" << NL;
        return;
    }
    poly -= poly.centerOfMass();
    poly *= 0.3;
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    static PRand rand;
    static PRandVector3d vrand;
//    const Plane3d plane ( vrand(), 0.3*rand() );
    const Plane3d plane ( Vector3d(0,0,1), -poly.vertex[rand.number(poly.vertex.size())].z );
    display << plane.dist << NL;
    double bx = plane.norm.x;
    double by = plane.norm.y;
    double bz = plane.norm.z;
    double bd = plane.dist;
    const double ax = fabs ( bx );
    const double ay = fabs ( by );
    const double az = fabs ( bz );
    const nat way = ay > az ? ( ay > ax ? 1 : 0 ) : ( az > ax ? 2 : 0 );
    switch ( way )
    {
        case 0: bx /= ax; by /= ax; bz /= ax; bd /= ax; break;
        case 1: bx /= ay; by /= ay; bz /= ay; bd /= ay; break;
        case 2: bx /= az; by /= az; bz /= az; bd /= az; break;
    }
    DynArray<DynArray<Vector3d> > res;
    double t0 = timeInSec();
    if ( ! intersection ( poly, plane, res ) )
        display << "error" << NL;
    double t1 = timeInSec();
    for ( nat i = 0; i < res.size(); ++i )
    {
        for ( nat j = 0; j < res[i].size(); ++j )
        {
            double d, e;
            const Vector3d & v = res[i][j];
            switch ( way )
            {
                case 0: e = bx * v.x;
                        d = by * v.y + bz * v.z + bd; break;
                case 1: e = by * v.y;
                        d = bz * v.z + bx * v.x + bd; break;
                case 2: e = bz * v.z;
                        d = bx * v.x + by * v.y + bd; break;
            }
            d += e; 
            if ( d != 0 ) display << i << j << d << NL;
        }
        drawPolygon ( res[i], 1, 0, 0 );
        display << i << res[i].size() << NL;
    }
    display << "end" << NL;
}

void intersectPlanePolyhedron2()
{
//    RealFile file ( "data/boart0858.pyh", "rb" );
//    RealFile file ( "data/boart1246.pyh", "rb" );
    RealFile file ( "data/boart1467.pyh", "rb" );
    Polyhedron poly;
    if ( ! loadPYH (  file, poly ) )
    {
        display << "Not load pyh-file" << NL;
        return;
    }
    poly -= poly.centerOfMass();
    poly *= 0.3;
    static PRand rand;
    static PRandVector3d vrand;
    DynArray<DynArray<Vector3d> > res;
    for ( nat i = 0; i < 1000; ++i )
    {
        const Plane3d plane ( vrand(), 0.5*rand() );
        if ( ! intersection ( poly, plane, res ) )
            display << i << "error" << NL;
        if ( res.size() > 0 )
        {
            if ( ! res[0].size() ) display << i << res.size() << res[0].size() << NL;
            /*draw ( poly, 0, 1, 1, 1, VM_WIRE );
            for ( nat j = 0; j < res.size(); ++j ) 
                drawPolygon ( res[j], 1, 0, 0 );
            break;*/
        }
        else
        {
            intersection ( poly, plane, res );
            display << i << 0 << NL;
        }
    }
    display << "end" << NL;
}

void cutPlanePolyhedron()
{
    Polyhedron poly;
    if(0)
    {
    //    RealFile file ( "data/boart0858.pyh", "rb" );
    //    RealFile file ( "data/boart1246.pyh", "rb" );
        RealFile file ( "data/boart1467.pyh", "rb" );
        if ( ! loadPYH (  file, poly ) )
        {
            display << "Not load pyh-file" << NL;
            return;
        }
    }
    else
    {
        if ( ! loadSTL ( "data/1.stl", poly ) ) return;
    }
    {
        for ( nat j = 0; j < poly.facet.size(); ++j )
        {
            if ( poly.facet[j].nv != 3 ) 
                j=j;
        }
    }
    poly -= poly.centerOfMass();
    poly *= 0.02;
//    poly.makeCube ( 1 );
//    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    static PRand rand;
    static PRandVector3d vrand;
    const Plane3d plane ( vrand(), 0.3*rand() );
//    const Plane3d plane ( Vector3d(1,1,0), -2 );
    static CutStor3d stor;
    Polyhedron res;
    double t0 = timeInSec();
    if ( ! cut ( poly, plane, res, stor ) )
        display << "error" << NL;
    else
    {
        PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
        check ( res, rec );
        draw ( res, 0, 1, 1, 1, VM_WIRE );
    }
    {
        DynArray<DynArray<Vector3d> > res;
        intersection ( poly, plane, res );
        for ( nat j = 0; j < res.size(); ++j ) drawPolygon ( res[j], 1, 0, 0 );
    }
//double t1 = timeInSec();
    display << "end" << NL;
}

void randPolyhedron ( nat32 n, Polyhedron & poly )
{
    static PRandVector3d vrand;
    DynArray<Vector3d> point ( n );
    convexHull ( point << vrand, poly );
}

void intersect1c_test()
{
    Polyhedron poly1, poly2, poly3;
    randPolyhedron ( 9, poly1 );
    randPolyhedron ( 9, poly2 );
    draw ( poly1, 0, 1, 1, 1, VM_WIRE );
    draw ( poly2, 0, 0, 1, 1, VM_WIRE );
    if ( intersect1c ( poly1, poly2, poly3 ) )
        draw ( poly3, 1, 0, 0, 1, VM_WIRE );
}

void drawList ( List2n & list, CArrRef<Vector3d> vert, float r, float g, float b )
{
    if ( list.top() )
    do
    {
        Item2n * it = list.cur();
        draw ( Segment3d ( 0.5*vert[it->a], 0.5*vert[it->b] ), r, g, b );
    }
    while ( list.next() );
}

void checkList ( List2n & list )
{
    for ( nat i = 0; i < list.size(); ++i )
    {
        Item2n * i1 = list.cur();
        Item2n * i2 = list.cnext();
        if ( i1->b != i2->a ) display << i1->b << i2->a << NL;
    }
    while ( list.next() );
}

void link_test()
{
    FixArray<Vector3d, 12> vert;
    vert[0] = Vector3d (  2, -2, 0 );
    vert[1] = Vector3d (  2,  2, 0 );
    vert[2] = Vector3d ( -2,  2, 0 );
    vert[3] = Vector3d ( -2, -2, 0 );
    vert[4] = Vector3d (  0, -1.5, 0 );
    vert[5] = Vector3d (  1, -1.85, 0 );
    vert[6] = Vector3d ( -1, -1, 0 );
    vert[7] = Vector3d ( -1,  1, 0 );
    vert[8] = Vector3d (  1,  1, 0 );
    vert[9] = Vector3d (  1, -1, 0 );
    vert[10] = Vector3d (  0, -1.2, 0 );
    vert[11] = Vector3d (  -0.5, -1, 0 );
    List2n plus;
    plus.addAftCur ( new Item2n ( Set2<nat> ( 5, 4 ) ) );
    plus.addAftCur ( new Item2n ( Set2<nat> ( 4, 0 ) ) );
    plus.addAftCur ( new Item2n ( Set2<nat> ( 0, 1 ) ) );
    plus.addAftCur ( new Item2n ( Set2<nat> ( 1, 2 ) ) );
    plus.addAftCur ( new Item2n ( Set2<nat> ( 2, 3 ) ) );
    plus.addAftCur ( new Item2n ( Set2<nat> ( 3, 5 ) ) );
//    drawList ( plus, vert, 0, 1, 1 );
    List<ListItem<List2n> > minus;
    minus.addAftCur ( new ListItem<List2n> );
    List2n & list = * minus.cur();
    list.addAftCur ( new Item2n ( Set2<nat> ( 6, 7 ) ) );
    list.addAftCur ( new Item2n ( Set2<nat> ( 7, 8 ) ) );
    list.addAftCur ( new Item2n ( Set2<nat> ( 8, 9 ) ) );
    list.addAftCur ( new Item2n ( Set2<nat> ( 9, 11 ) ) );
    list.addAftCur ( new Item2n ( Set2<nat> ( 11,10 ) ) );
    list.addAftCur ( new Item2n ( Set2<nat> ( 10, 6 ) ) );
//    drawList ( list, vert, 1, 0, 0 );
//    link ( plus, minus, vert, Vector3d(0,0,1), CutStor3d() );
    checkList ( list );
    checkList ( plus );
    drawList ( list, vert, 1, 1, 0 );
    drawList ( plus, vert, 0, 1, 1 );
}

void intersect_2_Polyhedron()
{
#if 1
    Polyhedron poly1, poly2, sect;
    //*
    RealFile file1 ( "data/rough_full.pyh", "rb" );
//    RealFile file1 ( "data/new_large_5313_1.pyh", "rb" );
//    RealFile file1 ( "data/new_large_90_1.pyh", "rb" );
//    RealFile file1 ( "data/boart0858.pyh", "rb" );
//    RealFile file1 ( "data/boart1467.pyh", "rb" );
    if ( ! loadPYH (  file1, poly1 ) )
    {
        display << "Not load pyh-file1" << NL;
        return;
    }
//    draw ( poly1, 0, 1, 1, 1, VM_WIRE );
    draw ( poly1, 0, 1, 1, 1, VM_FLAT );
    return;
    RealFile file2 ( "data/large_poly2.pyh", "rb" );
//    RealFile file2 ( "data/new_large_9080_2.pyh", "rb" );
//    RealFile file2 ( "data/new_large_90_2.pyh", "rb" );
//    RealFile file2 ( "data/boart0858.pyh", "rb" );
//    RealFile file2 ( "data/boart1246.pyh", "rb" );
//    RealFile file2 ( "data/boart1467.pyh", "rb" );
    //RealFile file2 ( "data/octo0684.pyh", "rb" );
    if ( ! loadPYH (  file2, poly2 ) )
    {
        display << "Not load pyh-file2" << NL;
        return;
    }/*/
    //for ( nat i = 0; i < 5; ++i )
    {
        randPolyhedron ( 6, poly1 );
        randPolyhedron ( 6, poly2 );
    }//*/
    intersect ( poly1, poly2, 1e-9, sect );
 //   for ( i = 0; i < sect.vertex.size(); ++i ) display << sect.vertex[i] << NL;
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    check ( sect, rec );
    nat nb1, nb2;
    sect.countEdges ( nb1, nb2 );
    display << "Eiler" << sect.facet.size() << nb1 + nb2 << sect.vertex.size() <<
        int ( sect.facet.size() + sect.vertex.size() ) - int ( nb1 + nb2 + 2 ) << NL;
    const Vector3d o = sect.centerOfMass();
    poly1 -= o;
    poly1 *= 0.5;
//    draw ( poly1, 1, 0, 0, 1, VM_WIRE );
    poly2 -= o;
    poly2 *= 0.5;
//    draw ( poly2, 1, 1, 0, 1, VM_WIRE );
    sect -= o;
    sect *= 0.5;
//    draw ( sect, 0, 1, 1, 1, VM_WIRE );
//    draw ( sect, 0, 1, 1, 1, VM_FLAT );
#else
    char buf[80];
    Polyhedron poly1, poly2, sect;
    //for ( nat i = 1; i <= 11; ++i )
    for ( nat i = 1; i <= 1; ++i )
    {
        sprintf ( buf, "data/large%d_poly1.pyh", i );
        RealFile file1 ( buf, "rb" );
        if ( ! loadPYH (  file1, poly1 ) )
        {
            display << "Not load" << buf << NL;
            return;
        }
        sprintf ( buf, "data/large%d_poly2.pyh", i );
        RealFile file2 ( buf, "rb" );
        if ( ! loadPYH (  file2, poly2 ) )
        {
            display << "Not load" << buf << NL;
            return;
        }
        display << buf << NL;
    draw ( poly2, 1, 1, 0, 1, VM_WIRE );
        intersect ( poly1, poly2, 1e-9, sect );
    }
#endif
    display << "end" << NL;
}

bool _cut ( const Polyhedron & poly, unsigned np, const Plane3d * plane, Polyhedron & res)
{
    CutStor3d stor;
    Polyhedron tmp1, tmp2;
    tmp1 = poly;
//PolyhedronErrorRecipient rec ( 1e-4, 1e-6 );
    for ( unsigned i = 0; i < np; ++i )
    {
		if ( ! cut ( tmp1, plane[i], tmp2, stor ) ) return false;
//display << i << NL;
//check ( tmp2, rec );
        _swap ( tmp1, tmp2 );
    }
    _swap ( tmp1, res );
    return true;
}

bool intersectPolyhedrons ( const Polyhedron & pl1, const Polyhedron & pl2, Polyhedron & pl3 )
{
    const unsigned np = 100;
    Plane3d plane[np];
    nat i;
	for (i = 0; i < np; ++i)
        plane[i] = (Plane3d&) pl1.facet[i].plane;
    double timeInSec();
    double t1 = timeInSec();
//    bool ok = _cut ( pl2, np, plane, pl3 );
    bool ok = cut ( pl2, np, plane, 1, pl3 );
    double t2 = timeInSec();
    display << t2-t1 << NL;
//PolyhedronErrorRecipient rec ( 1e-4, 1e-6 );
//check ( pl3, rec );
    return ok;
}

void proba ()
{
    Polyhedron poly1, poly2, poly3;
    if ( loadSTL ( "F:/TEST/model.stl", poly1 ) )
    {
        poly1 -= poly1.centerOfMass ();
        poly1 *= 0.99;
    }
    else
    {
        display << "no load file 1" << NL;
        return;
    }
    if ( loadSTL ( "F:/TEST/diam9min_rec0000_60_mm_fixed_MeshlabSimplified_Faces60k.stl", poly2 ) )
    {
        poly2 -= poly2.centerOfMass ();
    }
    else
    {
        display << "no load file 2" << NL;
        return;
    }
    if ( intersectPolyhedrons ( poly1, poly2, poly3 ) )
    {/*
        POLYHEDRON * poly = makePOLYHEDRON ( poly3 );
        GEOMETRIC_FIGURE * f = new GEOMETRIC_FIGURE ( *poly, BASIS_1 );
        new POLYHEDRON_ENTRY ( f );*/
    }
    display << "end" << NL;
}

void intersectHalfSpaces()
{
    Polyhedron poly1, poly2;
    poly1.makeCube(1);
    poly1.makeTetrahedron(1);
    nat i;
    Suite<const Plane3d *> plane;
    for ( i = 0; i < poly1.facet.size(); ++i ) plane.inc() = & poly1.facet[i].plane;
    Polyhedron poly;
    if ( ! intersectHalfSpaces ( plane, poly ) )
    {
        display << "! intersectHalfSpaces" << NL;
        return;
    }
    draw ( poly, 1, 1, 0, 1, VM_WIRE );
    for ( i = 0; i < poly1.facet.size(); ++i )
    {
        for ( nat j = 0; j < 3; ++j ) display << poly1.facet[i].index[j];
        for ( nat j = 0; j < 3; ++j ) display << poly.facet[i].index[j];
        display << NL;
    }
}

} // namespace

void intersect3d_test ()
{
    drawNewList2d ();
//    intersection_test();
//    cutPlaneSegment();
//    intersectPlaneSegment();
//    intersectLineSphere();
//    intersectSegmentSphere();
//    intersectLineEllipsoid();
//    intersectSegmentEllipsoid();
//    intersectPlanePolyhedron2();
//    cutPlanePolyhedron();
//    intersect1c_test();
//    link_test();
//    intersect_2_Polyhedron();
//    proba ();
    intersectHalfSpaces();
    endNewList ();
}