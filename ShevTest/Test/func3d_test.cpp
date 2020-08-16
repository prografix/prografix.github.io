
#include "math.h"

#include "../Shev/rand.h"
#include "../Shev/tune.h"
#include "../Shev/LinAlg.h"
#include "../Shev/func2d.h"
#include "../Shev/func3d.h"
#include "../Shev/opti3d.h"
#include "../Shev/mathem.h"
#include "../Shev/moment3d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/Vector3d.h"
#include "../Shev/ShevList.h"
#include "../Shev/ShevArray.h"
#include "PolyhedronErrorRecipient.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/intersect2d.h"
#include "../Shev/approx2d.h"
#include "../Shev/RealFile.h"
#include "../Shev/filePYH.h"
#include "../Shev/lists.h"
#include "../Shev/trans.h"
#include "../Shev/heap.h"
#include "../draw.h"

#include "display.h"

double timeInSec();

bool minConvexPolyhedronDiameter ( const Polyhedron & poly, Vector3d & dir, double & d1, double & d2 )
{
    if ( poly.facet.size() == 0 || poly.vertex.size() == 0 ) return false;
    double mind = 1e300;
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        const Vector3d & norm = poly.facet[i].plane.norm;
        double min, max;
        min = max = norm * poly.vertex[0];
        for ( nat j = 1; j < poly.vertex.size(); ++j )
        {
            const double t = norm * poly.vertex[j];
            if ( min > t ) min = t; else
            if ( max < t ) max = t;
        }
        const double d = max - min;
        if ( mind > d ) mind = d, d1 = min, d2 = max, dir = norm;
    }
    for ( nat i1 = poly.facet.size()-1; i1 > 1; --i1 )
    {
        const Facet & fa1 = poly.facet[i1];
        for ( nat j1 = 0; j1 < fa1.nv; ++j1 )
        {
            const nat n1 = fa1.index[j1+fa1.nv+1];
            if ( i1 < n1 ) continue;
            Vector3d v1 = fa1.plane.norm % poly.facet[n1].plane.norm;
            if ( norm1 ( v1 ) < 1e-8 ) continue;
            v1.setNorm2();
            for ( nat i2 = i1-1; i2 > 0; --i2 )
            {
                const Facet & fa2 = poly.facet[i2];
                for ( nat j2 = 0; j2 < fa2.nv; ++j2 )
                {
                    const nat n2 = fa2.index[j2+fa2.nv+1];
                    if ( i2 < n2 ) continue;
                    Vector3d v2 = fa2.plane.norm % poly.facet[n2].plane.norm;
                    if ( norm1 ( v2 ) < 1e-8 ) continue;
                    v2.setNorm2();
                    Vector3d norm = v1 % v2;
                    if ( norm1 ( norm ) < 1e-8 ) continue;
                    norm.setNorm2();
                    double min = norm * poly.vertex[fa1.index[j1]];
                    double max = norm * poly.vertex[fa2.index[j2]];
                    if ( min > max )
                    {
                        min = -min;
                        max = -max;;
                        norm = -norm;
                    }
                    if ( ( fa1.plane.norm % norm ) * v1 > 0 ) continue;
                    if ( ( norm % poly.facet[n1].plane.norm ) * v1 > 0 ) continue;
                    if ( ( fa2.plane.norm % norm ) * v2 < 0 ) continue;
                    if ( ( norm % poly.facet[n2].plane.norm ) * v2 < 0 ) continue;
                    const double d = max - min;
                    if ( mind > d ) mind = d, d1 = min, d2 = max, dir = norm;
                }
            }
        }
    }
    return true;
}

namespace
{

class TrianFacet
{
public:
    nat vertex[3], facet[3], edge[3], index;
    Plane3d plane;
    List1d list;
};

}

inline void _swap ( SortItem<double, TrianFacet *> & p1, SortItem<double, TrianFacet *> & p2 )
{
    const SortItem<double, TrianFacet *> p ( p1 );
    p1 = p2;
    p2 = p;
    _swap ( p1.tail->index, p2.tail->index );
}

void randPolyhedron(nat32 n, Polyhedron& poly)
{
    static PRandVector3d vrand;
    DynArray<Vector3d> point(n);
    point << vrand;
    convexHull(point, poly);
}

namespace
{

void randVector3d ( ArrRef<Vector3d> p )
{
    static PRandVector3d rand;
    p << rand;
}

void randSegment3d ( ArrRef<Segment3d> p )
{
    static PRandVector3d rand;
    for ( nat i = 0; i < p.size(); ++i )
    {
        p[i].a = rand();
        p[i].b = rand();
    }
}

void center_test ()
{
    static PRandVector3d rand;
    Vector3d shift = rand();
    Polyhedron poly;
    poly.makeCube ( 1. );
    poly.vertex += shift;
    poly.initPlanes();
    display << centerOfMass ( poly.vertex ) - shift << NL;

    FixArray<Segment3d,1> segm;
    segm[0].a = Vector3d ( 1.,1.,1. ) + shift;
    segm[0].b = Vector3d ( 1.,1.,0. ) + shift;
    display << centerOfMass ( segm ) - shift << NL;

    nat32 i;
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        display << centerOfMass ( i, poly ) - shift << NL;
    }

    display << centerOfMass ( poly ) - shift << NL;
}

void momentum2pnt_test()
{
    const nat n = 5;
    FixArray<Vector3d, n> point;
    FixArray<double, n> mass;
    static PRand rand(33);
    for ( nat j = 0; j < 1000; ++j )
    {
        randVector3d ( point );
        for ( nat i = 0; i < n; ++i )
        {
            mass[i] = rand();
        }/*
        point[0] = Vector3d ( 1., -1., 0. );
        point[1] = Vector3d ( 1., 1., 0. );
        point[2] = Vector3d ( -1., 1., 0. );
        point[3] = Vector3d ( -1., -1., 0. );*/
        Vector3d o, minNorm, midNorm, maxNorm;
        const Mom3d mom = momentum2pnt ( point, mass );
        double min = momentum2pnt ( point, mass, Plane3d ( mom.minNorm, -( mom.minNorm * mom.o ) ) );
        double mid = momentum2pnt ( point, mass, Plane3d ( mom.midNorm, -( mom.midNorm * mom.o ) ) );
        double max = momentum2pnt ( point, mass, Plane3d ( mom.maxNorm, -( mom.maxNorm * mom.o ) ) );
//        display << minMom << midMom << maxMom << NL;
//        display << min << mid << max << NL << NL;
        if ( fabs ( max - mom.maxMom ) > 3e-15 || fabs ( mid - mom.midMom ) > 2e-15 || fabs ( min - mom.minMom ) > 1e-15 )
            display << max - mom.maxMom << mid - mom.midMom << min - mom.minMom << NL;
    }
    display << "end" << NL;
}

void momentum2sgm_test()
{
    FixArray<Segment3d, 5> segm;
    for ( nat j = 0; j < 1000; ++j )
    {
        randSegment3d ( segm );
        const Mom3d mom = momentum2sgm ( segm );
        double min = momentum2sgm ( segm, Plane3d ( mom.minNorm, -( mom.minNorm * mom.o ) ) );
        double mid = momentum2sgm ( segm, Plane3d ( mom.midNorm, -( mom.midNorm * mom.o ) ) );
        double max = momentum2sgm ( segm, Plane3d ( mom.maxNorm, -( mom.maxNorm * mom.o ) ) );
//        display << minMom << midMom << maxMom << NL;
//        display << min << mid << max << NL;
        if ( fabs ( max - mom.maxMom ) > 3e-15 || fabs ( mid - mom.midMom ) > 2e-15 || fabs ( min - mom.minMom ) > 1e-15 )
            display << max - mom.maxMom << mid - mom.midMom << min - mom.minMom << NL;
    }
    display << "end" << NL;
}

void momentum2fct_test()
{
    static PRandVector3d rand;
    Vector3d shift = rand();
    Polyhedron poly;
    poly.makeCuboid ( 1., 2., 3. );
    poly.vertex += shift;
    poly.initPlanes();
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        const Mom3d mom = momentum2fct ( i, poly );
        double min = momentum2fct ( i, poly, Plane3d ( mom.minNorm, -( mom.minNorm * mom.o ) ) );
        double mid = momentum2fct ( i, poly, Plane3d ( mom.midNorm, -( mom.midNorm * mom.o ) ) );
        double max = momentum2fct ( i, poly, Plane3d ( mom.maxNorm, -( mom.maxNorm * mom.o ) ) );
        display << poly.facet[i].plane.norm << NL;
        display << mom.minNorm << NL;
        display << min << mid << max << NL;
        display << mom.minMom << mom.midMom << mom.maxMom << NL << NL;
    }
}

void momentum2plh_test()
{
    static PRandVector3d rand;
    Vector3d shift = rand();
    Polyhedron poly;
    randPolyhedron ( 9, poly );
//    makeCuboid ( 1., 2., 3., poly );
    poly.vertex += shift;
    poly.initPlanes();
    const Mom3d mom = momentum2plh ( poly );
    double min = momentum2plh ( poly, Plane3d ( mom.minNorm, -( mom.minNorm * mom.o ) ) );
    double mid = momentum2plh ( poly, Plane3d ( mom.midNorm, -( mom.midNorm * mom.o ) ) );
    double max = momentum2plh ( poly, Plane3d ( mom.maxNorm, -( mom.maxNorm * mom.o ) ) );
    display << min << mid << max << NL;
    display << mom.minMom << mom.midMom << mom.maxMom << NL << NL;
}

inline void check ( CArrRef<TrianFacet> facet, nat i, nat nv, const nat * iv, const Vector3d * vert,
                    const Vector3d & o )
{
    const TrianFacet & fi = facet[i];
    nat j;
    for ( j = 0; j < 3; ++j )
    {
        const TrianFacet & fj = facet[fi.facet[j]];
        const nat k = fi.edge[j];
        if ( fj.facet[k] != i )
            display << "err" << NL;
    }
    const double eps = 1e-12;
    double max = fabs ( fi.plane % vert[iv[fi.vertex[0]]] );
    for ( j = 1; j < 2; ++j )
    {
        const double t = fabs ( fi.plane % vert[iv[fi.vertex[j]]] );
        if ( max < t ) max = t;
    }
    if ( max > eps )
        display << i << "dev =" << max << NL;
    max = fi.plane % vert[iv[0]];
    for ( j = 1; j < nv; ++j )
    {
        const double t = fi.plane % vert[iv[j]];
        if ( max < t ) max = t;
    }
//    if ( fabs ( max ) > eps )
//        display << "max v =" << max << NL;
    const Vector3d & v0 = vert[iv[fi.vertex[0]]];
    const Vector3d & v1 = vert[iv[fi.vertex[1]]];
    const Vector3d & v2 = vert[iv[fi.vertex[2]]];
    const Vector3d norm = ( ( v1 - v0 ) % ( v2 - v0 ) ).setNorm2();
    const Vector3d v = fi.plane.norm + norm;
    if ( v * v < 1e-9 )
    {
        display << "err" << i << NL;
    }
    Plane3d plane1;
    plane1.norm = ( ( vert[iv[fi.vertex[0]]] - o ) % ( vert[iv[fi.vertex[1]]] - o ) ).setNorm2();
    plane1.dist = - ( plane1.norm * o );
    Plane3d plane2;
    plane2.norm = ( ( vert[iv[fi.vertex[1]]] - o ) % ( vert[iv[fi.vertex[2]]] - o ) ).setNorm2();
    plane2.dist = - ( plane2.norm * o );
    Plane3d plane3;
    plane3.norm = ( ( vert[iv[fi.vertex[2]]] - o ) % ( vert[iv[fi.vertex[0]]] - o ) ).setNorm2();
    plane3.dist = - ( plane3.norm * o );
    Show<Item1d> list ( fi.list );
    if ( list.top() )
    do
    {
        const Vector3d & v = vert[list.cur()->info];
        const double d1 = plane1 % v;
        const double d2 = plane2 % v;
        const double d3 = plane3 % v;
        const double eps = -1e-14;
        if ( d1 < eps || d2 < eps || d3 < eps )
        {
            i = i;
        }
    }
    while ( list.next() );
}

inline void check ( nat nf, CArrRef<TrianFacet> facet, nat nv, const nat * iv, const Vector3d * vert,
                    const Vector3d & o )
{
    for ( nat i = 0; i < nf; ++i ) check ( facet, i, nv, iv, vert, o );
}

inline bool checkPlane ( const TrianFacet & facet, CArrRef<nat> iv, CArrRef<Vector3d> vert )
{
    const Vector3d & v0 = vert[iv[facet.vertex[0]]];
    const Vector3d & v1 = vert[iv[facet.vertex[1]]];
    const Vector3d & v2 = vert[iv[facet.vertex[2]]];
    const Vector3d norm = ( ( v1 - v0 ) % ( v2 - v0 ) ).setNorm2();
    const Vector3d v = facet.plane.norm + norm;
    if ( v * v < 1e-9 )
    {
        double t = v * v;
    }
    double d0 = facet.plane % v0;
    double d1 = facet.plane % v1;
    double d2 = facet.plane % v2;
    const double eps = 1e-14;
    if ( fabs ( d0 ) > eps || fabs ( d1 ) > eps || fabs ( d2 ) > eps )
    {
        d0 = d0;
        return true;
    }
    return false;
}

static void recalcNorm ( const Vector3d & v1, const Vector3d & v2, double d1, double d2, Vector3d & norm )
{
    const double vv = v1 * v2;
    const double dd = d1 * d2;
    const double di = dd - vv * vv;
    if ( di > 1e-8 * dd )
    {
        const double n1 = v1 * norm;
        const double n2 = v2 * norm;
        const double t = ( n1 * d2 - vv * n2 ) / di;
        const double s = ( n2 * d1 - vv * n1 ) / di;
        norm -= t * v1 + s * v2;
    }
}

static void calcPlane ( TrianFacet & fa, CArrRef<nat> iv, CArrRef<Vector3d> vert,
                        const Plane3d & p1, const Plane3d & p2, double c1, const Vector3d & v )
{
    Vector3d & norm = fa.plane.norm;
    norm = norm1 ( p2.norm - p1.norm ) < 1e-14 ? p2.norm : c1 * p2.norm - ( p2 % v ) * p1.norm;
    const Vector3d & v0 = vert[iv[fa.vertex[0]]];
    const Vector3d & v1 = vert[iv[fa.vertex[1]]];
    const Vector3d & v2 = vert[iv[fa.vertex[2]]];
    const Vector3d v01 = v0 - v1;
    const Vector3d v12 = v1 - v2;
    const Vector3d v20 = v2 - v0;
    const double d01 = v01 * v01;
    const double d12 = v12 * v12;
    const double d20 = v20 * v20;
    if ( d01 <= d12 && d01 <= d20 )
        recalcNorm ( v12, v20, d12, d20, norm );
    else
    if ( d12 <= d01 && d12 <= d20 )
        recalcNorm ( v01, v20, d01, d20, norm );
    else
        recalcNorm ( v12, v01, d12, d01, norm );
    norm.setNorm2();
    const double d0 = norm * v0;
    const double d1 = norm * v1;
    const double d2 = norm * v2;
    fa.plane.dist = -0.5 * ( _min ( d0, d1, d2 ) + _max ( d0, d1, d2 ) );
}

inline void outLink ( ArrRef<TrianFacet> & facet, nat i, nat j )
{
    const TrianFacet & fi = facet[i];
    TrianFacet & fj = facet[fi.facet[j]];
    const nat k = fi.edge[j];
    fj.facet[k] = i;
    fj.edge[k] = j;
}

static void putFacet ( TrianFacet & fa, MaxHeap< SortItem<double, TrianFacet*> > & heap )
{
    if ( fa.list.size() == 0 ) return;
    Item1d * p = fa.list.top();
    while ( fa.list.next() )
    {
        if ( p->a < fa.list.cur()->a ) p = fa.list.cur();
    }
    fa.list.makeFir(p);
    fa.index = heap.size();
    heap << SortItem<double, TrianFacet*> ( p->a, & fa );
}

static void putFacet ( TrianFacet & fa, MaxHeap< SortItem<double, TrianFacet*> > & heap, 
                       CArrRef<Vector3d> vert, const Vector3d & o, double eps )
{
// Вычисление приоритета
    if ( fa.list.size() == 0 ) return;
    const double d = -1. / ( fa.plane % o );
    fa.list.top();
    for(;;)
    {
        Item1d * p = fa.list.cur();
        p->a = fa.plane % vert[p->info];
        if ( p->a < eps )
        {
            if ( fa.list.delCur_() ) break;
            continue;
        }
        p->a *= d;
        if ( ! fa.list.next() ) break;
    }
// Постановка в очередь
    Item1d * p = fa.list.top();
    if ( p == 0 ) return;
    while ( fa.list.next() )
    {
        if ( p->a < fa.list.cur()->a ) p = fa.list.cur();
    }
    fa.list.makeFir(p);
    fa.index = heap.size();
    heap << SortItem<double, TrianFacet*> ( p->a, & fa );
}

static void moveVert ( TrianFacet & fa, nat i, ArrRef<TrianFacet> facet, CArrRef<Vector3d> vert,
                       CArrRef<nat> iv, const Vector3d & o )
{
    if ( fa.list.top() == 0 ) return;
    List1d & dest = facet[fa.facet[i]].list;
    Plane3d plane;
    plane.norm = ( vert[iv[fa.vertex[i]]] - o ) % ( vert[iv[fa.vertex[i<2?i+1:0]]] - o );
    plane.dist = - ( plane.norm * o );
    for(;;)
    {
        Item1d * p = fa.list.cur();
        if ( plane % vert[p->info] < 0 )
        {
            if ( fa.list.movCurAftLas_ ( dest ) ) break;
            continue;
        }
        if ( ! fa.list.next() ) break;
    }
}

bool convex ( CArrRef<Vector3d> point, nat & nv, ArrRef<nat> & iv, nat & nf, ArrRef<TrianFacet> & facet )
{
    const nat n = point.size();
    if ( n < 4 ) return false;
    nat i;
    for ( i = 0; i < n; ++i ) iv[i] = i;
// Находим первую начальную точку
    nat im = 0;
    double max = qmod ( point[0] );
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[i] );
        if ( max < q ) max = q, im = i;
    }
    const double eps = 1e-12 * sqrt ( max );
    _swap ( iv[im], iv[0] );
// Находим вторую начальную точку
    im = 1;
    max = 0.;
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[iv[i]] - point[iv[0]] );
        if ( max < q ) max = q, im = i;
    }
    if ( !max ) return false;
    _swap ( iv[im], iv[1] );
// Находим третью начальную точку
    im = 2;
    max = 0.;
    const Vector3d dir = point[iv[1]] - point[iv[0]];
    const double dd = dir * dir;
    for ( i = 2; i < n; ++i )
    {
        const Vector3d v = point[iv[i]] - point[iv[0]];
        const double q = dd * qmod ( v ) - _pow2 ( dir * v );
        if ( max < q ) max = q, im = i;
    }
    if ( max < eps * eps ) return false;
    _swap ( iv[im], iv[2] );
// Находим четвёртую начальную точку
    Vector3d norm = dir % ( point[iv[2]] - point[iv[0]] );
    if ( !norm ) return false;
    norm.setNorm2();
    const Plane3d plane ( norm, - ( norm * point[iv[0]] ) );
    im = 3;
    max = 0.;
    for ( i = 3; i < n; ++i )
    {
        const double q = fabs ( plane % point[iv[i]] );
        if ( max < q ) max = q, im = i;
    }
    if ( max < eps ) return false;
    _swap ( iv[im], iv[3] );
    if ( plane % point[iv[3]] > 0 ) _swap ( iv[1], iv[2] );
// Строим модель тетраэдра
    nv = 4;
    nf = 4;
    const nat array[12] = { 3, 2, 1, 2, 3, 0, 1, 0, 3, 0, 1, 2 };
    for ( i = 0; i < 4; ++i )
    {
        const nat * ia = array + 3 * i;
        TrianFacet & fi = facet[i];
        nat * v = fi.vertex;
        v[0] = ia[1];
        v[1] = ia[2];
        v[2] = ia[0];
        nat * f = fi.facet;
        f[0] = ia[0];
        f[1] = ia[1];
        f[2] = ia[2];
        nat * e = fi.edge;
        e[0] = 0;
        e[1] = 1;
        e[2] = 2;
        fi.plane = Plane3d ( point[iv[v[0]]], point[iv[v[1]]], point[iv[v[2]]] );
    }
// Вычисляем центр вписанной сферы
    Vector3d o;
    {
        const Plane3d & plane0 = facet[0].plane;
        const Plane3d & plane1 = facet[1].plane;
        const Plane3d & plane2 = facet[2].plane;
        const Plane3d & plane3 = facet[3].plane;
        const Vector3d & v0 = plane0.norm;
        const Vector3d & v1 = plane1.norm;
        const Vector3d & v2 = plane2.norm;
        const Vector3d & v3 = plane3.norm;
        SLU3<double> slu;
        slu.aa = v0.x - v3.x; slu.ab = v0.y - v3.y; slu.ac = v0.z - v3.z; slu.ad = plane3.dist - plane0.dist;
        slu.ba = v1.x - v3.x; slu.bb = v1.y - v3.y; slu.bc = v1.z - v3.z; slu.bd = plane3.dist - plane1.dist;
        slu.ca = v2.x - v3.x; slu.cb = v2.y - v3.y; slu.cc = v2.z - v3.z; slu.cd = plane3.dist - plane2.dist;
        slu.gauss ( o.x, o.y, o.z );
    }
// Распределяем внешние точки по граням
    for ( i = 4; i < n; ++i )
    {
        const Vector3d & v = point[iv[i]];
        nat jm = 4;
        double max = 1e-12;
        for ( nat j = 0; j < 4; ++j )
        {
            const Plane3d & plane = facet[j].plane;
            const double d = - ( plane % v ) / ( plane % o );
            if ( max < d ) max = d, jm = j;
        }
        if ( jm == 4 ) continue;
        facet[jm].list.addAftLas ( new Item1d ( max, iv[i] ) );
    }
// Ставим в очередь грани с внешними точками
    MaxHeap< SortItem<double, TrianFacet*> > heap ( n - 4 );
    for ( i = 0; i < 4; ++i ) putFacet ( facet[i], heap );
// Добавляем оставшиеся точки по одной
    Suite<nat> buf1;
    Suite< Set2<nat> > buf2;
    while ( heap.size() > 0 )
    {
        SortItem<double, TrianFacet*> t;
        heap >> t;
        TrianFacet & f0 = * t.tail;
        const Vector3d & v = point[iv[nv] = f0.list.fir()->info];
        f0.list.delFir();
// Делаем вместо одной грани три
        const nat n0 = nat ( t.tail - &facet[0] );
        const nat n1 = nf;
        const nat n2 = nf + 1;
        nf += 2;
        const nat if0 = f0.facet[0];
        const nat ie0 = f0.edge[0];
        TrianFacet & f1 = facet[n1];
        f1.vertex[0] = f0.vertex[1];
        f1.vertex[1] = f0.vertex[2];
        f1.vertex[2] = nv;
        const nat if1 = f1.facet[0] = f0.facet[1];
        f1.facet[1] = n2;
        f1.facet[2] = n0;
        const nat ie1 = f1.edge[0] = f0.edge[1];
        f1.edge[1] = 2;
        f1.edge[2] = 1;
        facet[if1].facet[ie1] = n1;
        facet[if1].edge[ie1] = 0;
        TrianFacet & f2 = facet[n2];
        f2.vertex[0] = f0.vertex[2];
        f2.vertex[1] = f0.vertex[0];
        f2.vertex[2] = nv;
        const nat if2 = f2.facet[0] = f0.facet[2];
        f2.facet[1] = n0;
        f2.facet[2] = n1;
        const nat ie2 = f2.edge[0] = f0.edge[2];
        f2.edge[1] = 2;
        f2.edge[2] = 1;
        facet[if2].facet[ie2] = n2;
        facet[if2].edge[ie2] = 0;
        f0.vertex[2] = nv;
        f0.facet[1] = n1;
        f0.facet[2] = n2;
        f0.edge[1] = 2;
        f0.edge[2] = 1;
        const double h = f0.plane % v;
        calcPlane ( f2, iv, point, f0.plane, facet[if2].plane, h, v );
        calcPlane ( f1, iv, point, f0.plane, facet[if1].plane, h, v );
        calcPlane ( f0, iv, point, f0.plane, facet[if0].plane, h, v );
        buf1.inc() = n0;
        buf1.inc() = n1;
        buf1.inc() = n2;
        ++nv;
// Распределяем внешние точки по новым граням
        moveVert ( f0, 1, facet, point, iv, o );
        moveVert ( f0, 2, facet, point, iv, o );
        moveVert ( f1, 1, facet, point, iv, o );
// Устраняем невыпуклости
        buf2.inc() = Set2<nat> ( n0, 0 );
        buf2.inc() = Set2<nat> ( n1, 0 );
        buf2.inc() = Set2<nat> ( n2, 0 );
        do
        {
            buf2.resize ( buf2.size()-1 );
            const Set2<nat> p = buf2[buf2.size()];
            TrianFacet & fa = facet[p.a];
            TrianFacet & fb = facet[fa.facet[p.b]];
            const double h = fb.plane % v;
            if ( h > eps )
            {
                const nat f1 = p.a;
                const nat a1 = p.b;
                const nat b1 = a1 == 2 ? 0 : a1 + 1;
                const nat c1 = b1 == 2 ? 0 : b1 + 1;
                const nat f2 = fa.facet[p.b];
                const nat a2 = fa.edge[p.b];
                const nat b2 = a2 == 2 ? 0 : a2 + 1;
                const nat c2 = b2 == 2 ? 0 : b2 + 1;
                fa.vertex[b1] = fb.vertex[c2];
                fb.vertex[b2] = fa.vertex[c1];
                fa.facet[a1] = fb.facet[b2];
                fb.facet[a2] = fa.facet[b1];
                fa.facet[b1] = f2;
                fb.facet[b2] = f1;
                fa.edge[a1] = fb.edge[b2];
                fb.edge[a2] = fa.edge[b1];
                fa.edge[b1] = b2;
                fb.edge[b2] = b1;
                outLink ( facet, f1, a1 );
                outLink ( facet, f2, a2 );
                calcPlane ( fa, iv, point, fb.plane, facet[fa.facet[a1]].plane, h, v );
                calcPlane ( fb, iv, point, fb.plane, facet[fb.facet[c2]].plane, h, v );
                buf2.inc() = Set2<nat> ( f1, a1 );
                buf2.inc() = Set2<nat> ( f2, c2 );
                buf1.inc() = f2;
                if ( fb.list.size() > 0 )
                {
                    heap.del ( fb.index );
                    fa.list.addAllAftLas ( fb.list );
                }
                moveVert ( fa, b1, facet, point, iv, o );
            }
        }
        while ( buf2.size() > 0 );
// Ставим в очередь грани с внешними точками
        for ( i = 0; i < buf1.size(); ++i ) putFacet ( facet[buf1[i]], heap, point, o, eps );
        buf1.resize ( 0 );
    }
check ( nf, facet, nv, &iv[0], &point[0], o );
    return true;
}

Polyhedron & convex ( CArrRef<Vector3d> point, Polyhedron & poly )
{
    nat nf, nv;
    DynArray<nat> iv ( point.size() );
    DynArray<TrianFacet> facet ( 2*point.size() - 4 );
    if ( ! convex ( point, nv, iv, nf, facet ) ) return poly.makeVoid();
// Заполнение многогранника
    poly.vertex.resize ( nv );
    nat i;
    for ( i = 0; i < nv; ++i ) poly.vertex[i] = point[iv[i]];
    poly.facet.resize ( nf );
    for ( i = 0; i < nf; ++i )
    {
        const TrianFacet & cf = facet[i];
        Facet & fi = poly.facet[i];
        fi.plane = cf.plane;
        fi.resize ( 3 );
        fi.index[0] = fi.index[3] = cf.vertex[0];
        fi.index[1] = cf.vertex[1];
        fi.index[2] = cf.vertex[2];
        fi.index[4] = cf.facet[0];
        fi.index[5] = cf.facet[1];
        fi.index[6] = cf.facet[2];
        fi.index[7] = cf.edge[0];
        fi.index[8] = cf.edge[1];
        fi.index[9] = cf.edge[2];
    }
    return poly;
}

Polyhedron & convex2 ( CArrRef<Vector3d> point, Polyhedron & poly )
{
    nat nf, nv;
    DynArray<nat> iv ( point.size() );
    DynArray<TrianFacet> facet ( 2*point.size() - 4 );
    if ( ! convex ( point, nv, iv, nf, facet ) ) return poly.makeVoid();
// Заполнение многогранника
    poly.vertex.resize ( point.size() );
    nat i;
    for ( i = 0; i < point.size(); ++i ) poly.vertex[i] = point[i];
    poly.facet.resize ( nf );
    for ( i = 0; i < nf; ++i )
    {
        const TrianFacet & cf = facet[i];
        Facet & fi = poly.facet[i];
        fi.plane = cf.plane;
        fi.resize ( 3 );
        fi.index[0] = fi.index[3] = iv[cf.vertex[0]];
        fi.index[1] = iv[cf.vertex[1]];
        fi.index[2] = iv[cf.vertex[2]];
        fi.index[4] = cf.facet[0];
        fi.index[5] = cf.facet[1];
        fi.index[6] = cf.facet[2];
        fi.index[7] = cf.edge[0];
        fi.index[8] = cf.edge[1];
        fi.index[9] = cf.edge[2];
    }
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    ::check ( poly, rec );
    return poly;
}

void convex_test ()
{
    const nat np = 1000;
    DynArray<Vector3d> point ( np );
    static QRandPoint3d vrand;
    Polyhedron poly;
//    for ( nat i = 0; i < 3; ++i ) for012f ( point ) = vrand;
    for ( nat n = np; n <= np; n += 10 )
    {
        for ( nat j = 0; j < n; ++j ) point[j] = vrand().setNormU();
//        if ( n < 1990 ) continue;
        convex2 ( ArrRef<Vector3d>(point, 0, n), poly );
        break;
    }
    drawPoints ( point, 1, 0, 0 );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
//    display << "end" << NL;
}

void convex_test2 ()
{
    const nat np = 10000;
    DynArray<Vector3d> point ( np );
    static QRandPoint3d vrand;
    Polyhedron poly;
    for ( nat n = 100; n <= np; n += 100 )
//    for ( nat i = 0; i < 1000; ++i )
    {
        for ( nat j = 0; j < n; ++j ) point[j] = vrand().setNorm1();
        double t0 = timeInSec();
        convex ( ArrRef<Vector3d>(point, 0, n), poly );
        double t1 = timeInSec();
// display << n << t1-t0 << NL;
    }
    display << "end" << NL;
}

/********************************************************************************************************/

void getDistanceEld_test ()
{
    static PRand rand;
    static PNormalRand nrand;
    double time = 0;
    for ( nat i = 0; i < 1000000; ++i )
    {
        double a = rand();
        double b = rand();
        double c = 2 - a - b;
        if ( i % 5 == 0 ) a = b; else
        if ( i % 5 == 2 ) a = c; else 
        if ( i % 5 == 3 ) b = c;
        const double s = i & 1 ? 0 : 1e-16;
        const double x = i % 11 == 0 ? s : 0.25 * nrand();
        const double y = i % 13 == 0 ? s : 0.25 * nrand();
        const double z = i % 19 == 0 ? s : 0.25 * nrand();
        const Vector3d p ( x, y, z );
        Vector3d r;
        double d;
if(i==251628)
i=i;
        double t1 = timeInSec();
        if ( getDistanceEld ( a, b, c, p, d, r ) )
        {
            double t2 = timeInSec();
            time += t2 - t1;
            double t = _pow2 ( r.x / a ) + _pow2 ( r.y / b ) + _pow2 ( r.z / c ) - 1;
            Vector3d norm ( r.x / ( a * a ), r.y / ( b * b ), r.z / ( c * c ) );
            norm.setNorm2();
            Vector3d v = p - r - norm * ( ( p - r ) * norm );
            double dv = fabs ( v.x ) + fabs ( v.y ) + fabs ( v.z );
            if ( d > 0 )
            {
                if ( fabs ( t ) > 1e-15 )
                    display << i << t << NL;
                if ( dv > 1e-15 )
                    display << i << "dv =" << dv << NL;
            }
            else
            {
                if ( fabs ( t ) > 3e-15 )
                    display << i << t << NL;
                if ( dv > 3e-14 )
                    display << i << "dv =" << dv << NL;
            }
        }
        else
        {
//            display << "err" << NL;
        }
//return;
    }
    display << "time =" << time << NL;
}

bool minConvexPolyhedronDiameter1 ( const Polyhedron & poly, Vector3d & dir, double & d1, double & d2 )
{
    if ( poly.facet.size() == 0 || poly.vertex.size() == 0 ) return false;
    double mind = 1e300;
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        const Vector3d & norm = poly.facet[i].plane.norm;
        double min, max;
        min = max = norm * poly.vertex[0];
        for ( nat j = 1; j < poly.vertex.size(); ++j )
        {
            const double t = norm * poly.vertex[j];
            if ( min > t ) min = t; else
            if ( max < t ) max = t;
        }
        const double d = max - min;
        if ( mind > d ) mind = d, d1 = min, d2 = max, dir = norm;
    }
    for ( nat i1 = poly.facet.size()-1; i1 > 1; --i1 )
    {
        const Facet & fa1 = poly.facet[i1];
        for ( nat j1 = 0; j1 < fa1.nv; ++j1 )
        {
            const nat n1 = fa1.index[j1+fa1.nv+1];
            if ( i1 < n1 ) continue;
            Vector3d v1 = fa1.plane.norm % poly.facet[n1].plane.norm;
            if ( norm1 ( v1 ) < 1e-8 ) continue;
            v1.setNorm2();
            for ( nat i2 = i1-1; i2 > 0; --i2 )
            {
                const Facet & fa2 = poly.facet[i2];
                for ( nat j2 = 0; j2 < fa2.nv; ++j2 )
                {
                    const nat n2 = fa2.index[j2+fa2.nv+1];
                    if ( i2 < n2 ) continue;
                    Vector3d v2 = fa2.plane.norm % poly.facet[n2].plane.norm;
                    if ( norm1 ( v2 ) < 1e-8 ) continue;
                    v2.setNorm2();
                    Vector3d norm = v1 % v2;
                    if ( norm1 ( norm ) < 1e-8 ) continue;
                    norm.setNorm2();
                    double min = norm * poly.vertex[fa1.index[j1]];
                    double max = norm * poly.vertex[fa2.index[j2]];
                    if ( min > max )
                    {
                        min = -min;
                        max = -max;;
                        norm = -norm;
                    }
                    if ( ( fa1.plane.norm % norm ) * v1 > 0 ) continue;
                    if ( ( norm % poly.facet[n1].plane.norm ) * v1 > 0 ) continue;
                    if ( ( fa2.plane.norm % norm ) * v2 < 0 ) continue;
                    if ( ( norm % poly.facet[n2].plane.norm ) * v2 < 0 ) continue;
                    const double d = max - min;
                    if ( mind > d ) mind = d, d1 = min, d2 = max, dir = norm;
                }
            }
        }
    }
    return true;
}

void minConvexPolyhedronDiameter_test ()
{
     Polyhedron poly;
     Vector3d dir;
     double d1, d2;
//     poly.makeTetrahedron(0.5);
//     poly.makeOctahedron(1);
//     poly.makeCube(0.5);
     randPolyhedron ( 9, poly );
     minConvexPolyhedronDiameter ( poly, dir, d1, d2 );
     draw ( poly, 0, 1, 1, 1, VM_WIRE );
     FixArray<Vector3d, 4> quad;
     Vector3d a, b;
     reper ( dir, a, b );
     Vector3d o = d1 * dir;
     quad[0] = o + a + b;
     quad[1] = o - a + b;
     quad[2] = o - a - b;
     quad[3] = o + a - b;
     drawPolygon ( quad, 1, 0, 0 );
     o = d2 * dir;
     quad[0] = o + a + b;
     quad[1] = o - a + b;
     quad[2] = o - a - b;
     quad[3] = o + a - b;
     drawPolygon ( quad, 1, 1, 0 );
     double diam2 = diameterPnt ( poly.vertex, dir );
     display << fabs ( diam2  + d1 - d2 ) << NL;
}

void minConvexPolyhedronDiameter_test2 ()
{
    Polyhedron poly;
    Vector3d dir;
    double d1, d2;
    for ( nat i = 0; i < 1000; ++i )
    {
        randPolyhedron ( 19, poly );
        minConvexPolyhedronDiameter ( poly, dir, d1, d2 );
        double diam2 = diameterPnt ( poly.vertex );
        double d = fabs ( diam2 + d1 - d2 );
        if ( d > 1e-14 )
        {
            double t1 = 0, t2 = 0;
            for ( nat j = 0; j < poly.vertex.size(); ++j )
            {
                double t = dir * poly.vertex[j];
                if ( t1 > t ) t1 = t;
                if ( t2 < t ) t2 = t;
            }
            diameterPnt ( poly.vertex );
            display << i << d << NL;
        }
    }
    display << "end" << NL;
}

void spherePPPP_test()
{
    static PRand rand;
    static PRandVector3d vrand;
    Plane3d a, b, c, d;
    for ( nat i = 0; i < 10000; ++i )
    {
        a.norm = vrand();
        b.norm = vrand();
        c.norm = vrand();
        d.norm = vrand();
        a.dist = - rand();
        b.dist = - rand();
        c.dist = - rand();
        d.dist = - rand();
        const Def<Sphere3d> e = spherePPPP ( a, b, c, d );
        if ( e.isDef )
        {
            const double ra = fabs ( a % e.o + e.r );
            const double rb = fabs ( b % e.o + e.r );
            const double rc = fabs ( c % e.o + e.r );
            const double rd = fabs ( d % e.o + e.r );
            const double max = _max ( _max ( ra, rb ), _max ( rc, rd ) );
            if ( max > 1e-12 )
            {
                display << i << max << NL;
            }
        }
        else
        {
            //display << i << "err" << NL;
        }
    }
    display << "end" << NL;
}

void isConvex_test()
{
    Polyhedron poly;
    randPolyhedron ( 19, poly );
//    RealFile file ( "boart0858.pyh", "rb" );
//    RealFile file ( "boart1246.pyh", "rb" );
//    RealFile file ( "boart1467.pyh", "rb" );
/*    if ( ! loadPYH (  file, poly ) )
    {
        display << "Not load pyh-file1" << NL;
        return;
    }*/
    display << isConvex ( poly ) << NL;
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
}

void convexHull_test()
{
    Polyhedron inner, outer;
    static nat i = 0;
    Suite<const char *> name;
/*    name.inc() = "data/sample_helium_emerald.pyh";
    name.inc() = "data/sample_helium_princess.pyh";
    name.inc() = "data/sample_helium_brilliant.pyh";*
    name.inc() = "data/boart0858.pyh";
    name.inc() = "data/boart1246.pyh";
    name.inc() = "data/boart1467.pyh";
    name.inc() = "data/octo0684.pyh";*/
//    name.inc() = "data/new_large_5313_1.pyh";
    name.inc() = "data/new_large_9080_2.pyh";
    RealFile file ( name[i%name.size()], "rb" );
    ++i;
    if ( ! loadPYH (  file, inner ) )
    {
        display << "Not load pyh-file1" << NL;
        return;
    }
    inner -= inner.centerOfMass();
    inner *= 0.3;
//    inner.makeCube ( 0.5 );
    convexHull ( inner, outer );
    inner += Vector3d ( 0.7, 0, 0 );
    outer -= Vector3d ( 0.7, 0, 0 );
    draw ( inner, 0, 1, 1, 1, VM_WIRE );
    draw ( outer, 1, 0, 0, 0, VM_WIRE );
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    display << "inner" << inner.vertex.size() << inner.facet.size() << NL; ::check ( inner, rec );
    display << "outer" << outer.vertex.size() << outer.facet.size() << NL; ::check ( outer, rec );
}

void getDistance2test ()
{
    static PRandPoint3d rand;
    for ( nat i = 0; i < 999999; ++i )
    {
        Triangle3d t;
        t.a = rand();
        t.b = rand();
        t.c = rand();
        Vector3d p = rand();
        double d = getDistance2 ( t, p );
        double d1 = Segment3d ( t.a, t.b ).qdis ( p );
        double d2 = Segment3d ( t.b, t.c ).qdis ( p );
        double d3 = Segment3d ( t.c, t.a ).qdis ( p );
        double d4 = _min ( d1, d2, d3 );
        if ( d - d4 > 5e-15 )
        {
            display << i << d4 - d << NL;
            continue;
        }
        /*if ( i == 11 )
        {
            getDistance2 ( t, p );
            FixArray<Vector3d, 3> fig;
            fig[0] = t.a;
            fig[1] = t.b;
            fig[2] = t.c;
            drawPolygon ( fig, 0, 1, 1 );
            drawPoint ( p, 1, 1, 0 );
        }*/
        Plane3d plane ( t.a, t.b, t.c );
        OrthoFunc3to2 ortho ( plane.norm );
        FixArray<Vector2d, 3> poly;
        poly[0] = ortho ( t.a );
        poly[1] = ortho ( t.b );
        poly[2] = ortho ( t.c );
        if ( isIntersect ( ortho ( p ), poly ) )
        {
            double d5 = _pow2 ( plane % p );
            if ( fabs ( d5 - d ) > 5e-15 )
            {
                display << i << d5 - d << "plane" << NL;
                if ( 0 )
                {
                    getDistance2 ( t, p );
                    FixArray<Vector3d, 3> fig;
                    fig[0] = t.a;
                    fig[1] = t.b;
                    fig[2] = t.c;
                    drawPolygon ( fig, 0, 1, 1 );
                    drawPoint ( p, 1, 1, 0 );
                }
            }
        }
    }
    display << "end" << NL;
}

void makeSpin_test()
{
    static PRand drand;
    static PRandVector3d vrand;
	nat i;
    const nat n = 4;
    FixArray<Set2<Vector3d>, n> data;
    for ( i = 0; i < n; ++i )
    {
        data[i].a = vrand();
    }
    Set4<double> set;
    set.a = drand();
    set.b = 2*drand() - 1;
    set.c = 2*drand() - 1;
    set.d = 2*drand() - 1;
    double c = 1 / sqrt ( set.a * set.a + set.b * set.b + set.c * set.c + set.d * set.d );
    set.a *= c;
    set.b *= c;
    set.c *= c;
    set.d *= c;
    Ortho3d ortho ( (Spin3d&) set );
    for ( i = 0; i < n; ++i )
    {
//draw ( Segment3d ( null3d, data[i].a ), 0, 1, 1 );
        data[i].b = ortho ( data[i].a );
draw ( Segment3d ( null3d, data[i].b ), 1, 1, 0 );
        //data[i].a += 1e-1 * vrand();
    }
    Spin3d spin = makeSpin3d ( data );
    ortho = Ortho3d ( spin );
    for ( i = 0; i < n; ++i )
    {
        draw ( Segment3d ( null3d, ortho ( data[i].a ) ), 0, 1, 1 );
    }
    set = set;
}

void overlap_test()
{
    const nat n = 2000;
    DynArray<Vector3d> ortho ( n );
    nat i, j;
    QRand rand2(2), rand3(3), rand5(5);
    for ( i = 0; i < n; ++i ) ortho[i] = Vector3d ( rand2(), rand3(), rand5() );
    DynArray2<double> d ( n, n );
    for ( i = 0; i < n; ++i )
    {
        d[i][i] = 10;
        for ( j = i+1; j < n; ++j ) d[i][j] = d[j][i] = norm2 ( ortho[i] - ortho[j] );
    }
    double avg = 0;
    for ( i = 0; i < n; ++i )
    {
        double min = d[i][0];
        for ( j = 1; j < n; ++j ) min = _min ( min, d[i][j] );
        //display << min << NL;
        avg += min;
    }
    avg /= n;
    display << avg << NL;
//---------------------------------------//
    QRandVector3d vrand;
    const nat m = 25;
    Segment3d dim;
    dim.b = Vector3d (1,1,1);
    dim.a = - dim.b;
    DynArray<Plane3d> plane ( m );
    DynArray<const Plane3d *> ptr ( m );
    DynArray<Vector3d> point1 ( m ), point2 ( m );
    for ( i = 0; i < m; ++i )
    {
        plane[i].norm = vrand();
        //point1[i] = vrand();
        //point2[i] = vrand();
        plane[i].dist = 0.1; // plane[i].norm * ( point1[i] - point2[i] );
        ptr[i] = plane(i);
    }
    Def<Sphere3d> sph = maxSphereInConvexPolyhedron ( dim, ptr );
    display << "end" << NL;
}

} // namespace

void func3d_test ()
{
    drawNewList2d();
//    getDistanceEld_test();
//    center_test();
//    momentum2pnt_test();
//    momentum2sgm_test();
//    momentum2fct_test();
//    momentum2plh_test();
//    convex_test();
//    getSphere_test();
//    calcNearestPoint_test1();
//    makeCuboid_test();
//    minConvexPolyhedronDiameter_test2();
//    spherePPPP_test();
//    isConvex_test();
//    convexHull_test();
//    getDistance2test();
//    makeSpin_test();
    overlap_test();
    endNewList ();
}