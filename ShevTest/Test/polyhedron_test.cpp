
#include "math.h"

#include "PolyhedronErrorRecipient.h"
#include "../Shev/ShevArray2.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/intersect3d.h"
#include "../Shev/RealFile.h"
#include "../Shev/filePYH.h"
#include "../Shev/LinAlg.h"
#include "../Shev/mathem.h"
#include "../Shev/func3d.h"
#include "../Shev/func1t.h"
#include "../Shev/opti2d.h"
#include "../Shev/opti3d.h"
#include "../Shev/trans.h"
#include "../Shev/rand.h"
#include "../draw.h"
#include "Display.h"

double timeInSec();

namespace
{

void centerOfMass_test()
{
    Polyhedron poly;
//    poly.makeCuboid ( 1, 2, 3 );
//    makeEllipsoid ( 20, 1, 2, 3, poly );
/*
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        poly.facet[i].index[3] = poly.facet[i].index[1];
        Vector3d c = centerOfMass ( poly.facet[i], poly.vertex );
        display << c << poly.facet[i].plane % c << NL;
    }
*/
    PRandVector3d rand;
    for ( nat i = 0; i < 9; ++i )
    {
        poly.makeCuboid ( 1, 2, 3 );
        Vector3d v = rand();
        poly += v;
        display << poly.centerOfMass() - v << NL;
    }
}

void makeTetrahedron_test()
{
    Polyhedron poly;
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    check ( poly.makeTetrahedron ( 0.5 ), rec );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    display << volume ( poly ) << NL;
}

void makeOctahedron_test()
{
    Polyhedron poly;
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    check ( poly.makeOctahedron ( 1 ), rec );
}

void makeModel_test()
{
    Polyhedron poly;
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    Cone3d fig;
    fig.h = fig.r = 1;
    fig.o = Vector3d(0,0,0);
    check ( poly.makeModel ( fig, 100 ), rec );
}

void initPolyhedron ( Polyhedron & poly, nat n )
{
    static PRand rand;
    static QRand2Vector3d vrand;
    Polyhedron temp;
    //poly.makeCube ( 1 );
    //poly.makeTetrahedron ( 1 );
    poly.makeOctahedron ( 1 );
    //poly.makePyramid ( 1 );
        cut ( poly, Plane3d ( Vector3d(0,0,1), -0.6), temp); _swap (temp, poly);
    for ( nat i = 0; i < n; ++i )
    {
        cut ( poly, Plane3d ( vrand(), -0.6), temp ); _swap ( temp, poly );
    }
    poly *= getRandOrtho3d ( rand(), rand(), rand() );
    poly += Vector3d ( 0.2*rand(), 0.2*rand(), 0.2*rand() );
    for ( nat i = 0; i < poly.vertex.size(); ++i ) poly.vertex[i] += 1e-2 * vrand();
}

double maxDif ( const Polyhedron & poly )
{
    double dif = 0;
    const nat nf = poly.facet.size();
    for ( nat i = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        if ( ! f.nv ) continue;
        double max = fabs ( f.plane % poly.vertex[f.index[0]] );
        for ( nat j = 1; j < f.nv; ++j )
        {
            const double t = fabs ( f.plane % poly.vertex[f.index[j]] );
            _maxa ( max, t );
        }
        _maxa ( dif, max );
    }
    return dif;
}

double stdDif ( CCArrRef<Vector3d> & vert1, CCArrRef<Vector3d> & vert2 )
{
    double sum = 0;
    for ( nat i = 0; i < vert1.size(); ++i )
    {
        sum += qmod ( vert1[i] - vert2[i] );
    }
    return sum / vert1.size();
}

void normalizePolyhedron_test()
{
    Polyhedron poly;
    initPolyhedron ( poly, 40 ); //poly.makeOctahedron(1);
    double d1 = maxDif ( poly );
    DynArray<Vector3d> vert1 ( *poly.vertex );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    normalizeV2 ( poly );
    draw ( poly, 1, 1, 0, 1, VM_WIRE );
    double s = stdDif ( vert1, poly.vertex );
    double d2 = maxDif ( poly );
    display << d1 << d2 << s << NL;
}

void normalizePolyhedron_test2()
{
    Polyhedron poly;
    for ( nat k = 10; k < 19; ++k )
    {
        initPolyhedron ( poly, 100*k );
        double d1 = maxDif ( poly );
double t1 = timeInSec();
        normalizeV2 ( poly );
double t2 = timeInSec();
        double d2 = maxDif ( poly );
    display << k << poly.facet.size() + poly.vertex.size() << d1 << d2 << t2-t1 << NL;
    }
    display << "end" << NL;
}

void sym_test ( CCArrRef<Suite<SortItem<nat, double> > > & a )
{
    const nat na = a.size();
    nat err1 = 0, err2 = 0, err3 = 0, ne = 0;
    for ( nat i = 0; i < na; ++i )
    {
        CCArrRef<SortItem<nat, double> > & r = a[i];
//display << i << "|";
        nat jm = 0, k;
        double max = 0;
        for ( nat j = 0; j < r.size(); ++j )
        {
            const SortItem<nat, double> & e = r[j];
            if ( _maxa ( max, e.tail ) ) jm = e.head;
display << e.head << "(" << e.tail << ")";
            if ( j && r[j-1].head > e.head )
                ++err1;
            CCArrRef<SortItem<nat, double> > & r2 = a[e.head];
            for ( k = 0; k < r2.size(); ++k )
            {
                const SortItem<nat, double> & e2 = r2[k];
                if ( e2.head == i )
                {
                    if ( e.tail != e2.tail )
                        ++err3;
                    break;
                }
            }
            if ( k == r2.size() )
                ++err2;
        }
        if ( i != jm ) display << i << jm << max << NL;
        ne += r.size();
display << NL;
    }
    nat nn = na * na;
    display << err1 << err2 << err3 << double ( ne ) / nn << NL;
}

bool changePolyhedronV2 ( const Polyhedron & poly, ArrRef<Suite<SortItem<nat, double> > > & a, double * b, double * x, Vector3d * vertex, Plane3d * plane )
{
    nat i, l;
    const double c = 9;
    const nat nf = poly.facet.size();
    const nat nv = poly.vertex.size();
    DynArray<Suite<Set2<nat> > > va ( nv ), na ( nf );
    nat k = 0;
    for ( i = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        for ( nat j = 0; j < f.nv; ++j )
        {
            const nat iv = f.index[j];
            b[k] = f.plane % vertex[iv] + plane[i].norm * ( poly.vertex[iv] - vertex[iv] );
            va[iv].inc() = Set2<nat> ( i, k );
            na[i].inc() = Set2<nat> ( iv, k );
            a[k].resize();
            ++k;
        }
    }
    k = 0;
    for ( i = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        const Vector3d & ni = plane[i].norm;
        for ( nat j = 0; j < f.nv; ++j )
        {
            Suite<SortItem<nat, double> > & r = a[k];
            const nat iv = f.index[j];
            CCArrRef<Set2<nat> > & varr = va[iv];
            for ( l = 0; l < varr.size(); ++l )
            {
                const Set2<nat> & s = varr[l];
                const double d = ni * plane[s.a].norm;
                const nat ii = firEqu ( r, SortItem<nat, double> ( s.b ) );
                if ( ii < r.size() )
                    r[ii].tail += d;
                else
                    r.inc() = SortItem<nat, double> ( s.b, d );
            }
            const Vector3d & vj = vertex[iv];
            CCArrRef<Set2<nat> > & narr = na[i];
            for ( l = 0; l < narr.size(); ++l )
            {
                const Set2<nat> & s = narr[l];
                const double d = 1 + ( vj * vertex[s.a] ) / c;
                const nat ii = firEqu ( r, SortItem<nat, double> ( s.b ) );
                if ( ii < r.size() )
                    r[ii].tail += d;
                else
                    r.inc() = SortItem<nat, double> ( s.b, d );
            }
            insertSort123 ( r );
            ++k;
        }
    }
//sym_test ( a );
    bool ok = slu_LDLt ( a.size(), a(), b, x );
    if ( ! ok )
    {
        bool oko = slu_LDLtO ( a.size(), a(), b, x );
        if ( ! oko ) return false;
    }
 //   double maxv = 0;
    for ( i = 0; i < nv; ++i )
    {
        Vector3d v = null3d;
        CCArrRef<Set2<nat> > & varr = va[i];
        for ( l = 0; l < varr.size(); ++l )
        {
            const Set2<nat> & s = varr[l];
            v -= plane[s.a].norm * x[s.b];
        }
 //       _maxa ( maxv, norm2 ( v ) );
        vertex[i] = poly.vertex[i] + v;
    }
//    double maxn = 0, maxd = 0;
    for ( i = 0; i < nf; ++i )
    {
        double d = 0;
        Vector3d v = null3d;
        CCArrRef<Set2<nat> > & varr = na[i];
        for ( l = 0; l < varr.size(); ++l )
        {
            const Set2<nat> & s = varr[l];
            v -= vertex[s.a] * x[s.b];
            d -= x[s.b];
        }
        v /= c;
 //       _maxa ( maxn, norm2 ( v ) );
//        _maxa ( maxd, fabs ( d ) );
        const Facet & f = poly.facet[i];
        plane[i].norm = f.plane.norm + v;
        plane[i].dist = f.plane.dist + d;
    }
//display << maxv << maxn << maxd << NL;
    return true;
}

double calcQ ( const Plane3d & plane1, const Plane3d & plane2 )
{
    double sum = 0;
    sum += qmod ( plane1.norm - plane2.norm );
    sum += _pow2 ( plane1.dist - plane2.dist );
    return sum;
}

bool simple ( const Polyhedron & poly, nat i, const Vector3d * vertex, Plane3d & plane )
{
    const double c = 9;
    SLU3<double> slu;
    Suite<Set2<nat> > narr;
    const Facet & f = poly.facet[i];
    slu.ad = f.plane % vertex[f.index[0]];
    narr.inc() = Set2<nat> ( f.index[0], 0 );
    slu.bd = f.plane % vertex[f.index[1]];
    narr.inc() = Set2<nat> ( f.index[1], 1 );
    slu.cd = f.plane % vertex[f.index[2]];
    narr.inc() = Set2<nat> ( f.index[2], 2 );
    const Vector3d & v0 = vertex[f.index[0]];
    slu.aa = 1 + ( v0 * vertex[narr[0].a] ) / c;
    slu.ab = 1 + ( v0 * vertex[narr[1].a] ) / c;
    slu.ac = 1 + ( v0 * vertex[narr[2].a] ) / c;
    const Vector3d & v1 = vertex[f.index[1]];
    slu.ba = 1 + ( v1 * vertex[narr[0].a] ) / c;
    slu.bb = 1 + ( v1 * vertex[narr[1].a] ) / c;
    slu.bc = 1 + ( v1 * vertex[narr[2].a] ) / c;
    const Vector3d & v2 = vertex[f.index[2]];
    slu.ca = 1 + ( v2 * vertex[narr[0].a] ) / c;
    slu.cb = 1 + ( v2 * vertex[narr[1].a] ) / c;
    slu.cc = 1 + ( v2 * vertex[narr[2].a] ) / c;
    double x[3];
    slu.gauss ( x[0], x[1], x[2] );
    double d = 0;
    Vector3d v = null3d;
    v -= vertex[f.index[0]] * x[0];
    v -= vertex[f.index[1]] * x[1];
    v -= vertex[f.index[2]] * x[2];
    d -= x[0] + x[1] + x[2];
    v /= c;
    Plane3d res = f.plane;
    res.norm += v;
    res.dist += d;
    double max = 0;
    _maxa ( max, fabs ( res % vertex[f.index[0]] ) );
    _maxa ( max, fabs ( res % vertex[f.index[1]] ) );
    _maxa ( max, fabs ( res % vertex[f.index[2]] ) );
    double q1 = calcQ ( f.plane, plane );
    double q2 = calcQ ( f.plane, res );
    return true;
}

void calcQ ( const Polyhedron & poly, CCArrRef<Vector3d> & vertex, CCArrRef<Plane3d> & plane )
{
    double sum = 0, max = 0;
    const nat nf = poly.facet.size();
    const nat nv = poly.vertex.size();
    nat i = 0;
    for ( i = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        sum += qmod ( f.plane.norm - plane[i].norm );
        for ( nat j = 0; j < f.nv; ++j ) _maxa ( max, fabs ( plane[i] % vertex[f.index[j]] ) );
    }
    sum *= 9;
    for ( i = 0; i < nf; ++i )
    {
        sum += _pow2 ( poly.facet[i].plane.dist - plane[i].dist );
    }
    for ( i = 0; i < nv; ++i )
    {
        sum += qmod ( poly.vertex[i] - vertex[i] );
    }
display << 1e9*sum << max << NL;
}

bool changePolyhedron ( const Polyhedron & poly, ArrRef<Vector3d> & vertex, ArrRef<Plane3d> & plane )
{
//    double max1 = maxDif ( vertex, facet );
    double timeInSec();
    double t1 = timeInSec();
    const nat nf = poly.facet.size();
//    const nat nv = poly.vertex.size();
    nat i, n = 0;
    for ( i = 0; i < nf; ++i ) n += poly.facet[i].nv;
    DynArray<Suite<SortItem<nat, double> > > a ( n );
    DynArray<double> b ( n ), x ( n );
    bool ok;
calcQ ( poly, vertex, plane );
    for ( i = 0; i < 9; ++i )
    {
        ok = changePolyhedronV2 ( poly, a, b(), x(), vertex(), plane() );
calcQ ( poly, vertex, plane );
    }
    if ( ! ok ) return ok;
    double t2 = timeInSec();
//display << t2-t1 << NL;
    if ( ! ok ) return ok;
    return ok;
}

void changePolyhedron_test ()
{
    Polyhedron poly;
//    poly.makeTetrahedron(1);
//    poly.makeOctahedron(1);
//    poly.makeCube(1);
    poly.makeSphere(1,9);
    static PRandVector3d rand;
    double angle = 01 * 3.1415926 / 180;
    nat i;
    const nat nf = poly.facet.size();
    const nat nv = poly.vertex.size();
    nat im = 0;
    double min = 1;
    for ( i = 0; i < nf; ++i )
        if ( _mina ( min, poly.facet[i].plane.norm.z ) ) im = i;
    const Facet & table = poly.facet[im];
    DynArray<Plane3d> plane ( nf );
    DynArray<Vector3d> vertex ( nv ), vertex2 ( nv );
    for ( i = 0; i < nf; ++i ) plane[i] = poly.facet[i].plane;
    for ( i = 0; i < nv; ++i ) vertex[i] = vertex2[i] = poly.vertex[i];
    /*for ( i = 0; i < nv; ++i ) vertex1[i] = poly.vertex[i] + 0.2 * rand();
    for ( i = 0; i < table.nv; ++i )
    {
        Vector3d & v = poly.vertex[table.index[i]];
        v = table.plane.project ( v );
    }*/
    for ( i = 0; i < nf; ++i )
    {
 //       if ( i == im ) continue;
        Spin3d spin ( rand(), angle );
        poly.facet[i].plane.norm *= Ortho3d ( spin );
    }
    /*simple ( poly, 0, vertex(), plane[0]);
    simple ( poly, 1, vertex(), plane[1]);
    simple ( poly, 2, vertex(), plane[2]);
    simple ( poly, 3, vertex(), plane[3]);*/
    if ( ! changePolyhedron ( poly, vertex, plane ) )
        return;
    im = 0;
    double max = 0;
    for ( i = 0; i < nv; ++i ) if ( _maxa ( max, norm2 ( poly.vertex[i] - vertex2[i] ) ) ) im = i;
    poly.vertex.swap ( vertex );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
}

} // end of namespace

void polyhedron_test()
{
    drawNewList2d ();
//    makeWindows_test();
//    makeTetrahedron_test();
//    makeOctahedron_test();
//    centerOfMass_test();
//    makeModel_test();
//    normalizePolyhedron_test();
    changePolyhedron_test ();
    endNewList ();
}