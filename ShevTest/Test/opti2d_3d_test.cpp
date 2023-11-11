
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/rand.h"
#include "../Shev/func2d.h"
#include "../Shev/func3d.h"
#include "../Shev/opti2d.h"
#include "../Shev/Mathem.h"
#include "../Shev/LinAlg.h"
#include "../Shev/approx3d.h"
#include "../Shev/Vector3d.h"
#include "../Shev/ShevArray.h"
#include "../Shev/ShevArray2.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/intersect3d.h"
#include "../Shev/WireModel.h"
#include "../Shev/opti2d_3d.h"
#include "../Shev/trans.h"
#include "../Shev/HeapAndTree.h"
#include "PolyhedronErrorRecipient.h"
#include "../draw.h"

#include "display.h"

double timeInSec();
void randPolyhedron ( nat32 n, Polyhedron & poly );

void check ( const Polyhedron & poly, const Vector3d * vert )
{
    double max = 0;
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        const Plane3d & plane = poly.facet[i].plane;
        for ( nat j = 0; j < 4; ++j )
        {
            double t = plane % vert[j];
            max = _max ( max, t );
        }
    }
display << max << NL;
}

static bool maxParallelogramArea ( const WireModel<7> & model, Double<7> & best )
{
    Show< Vertex<7> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Vertex<7> * p = show.cur();
        const Double<7> & pc = p->coor;
        const Vector2d a1 ( pc.d3, pc.d4 );
        const Vector2d a2 ( pc.d5, pc.d6 );
        const double t = fabs ( a1 % a2 );
        if ( max < t ) max = t, best = pc;
        for ( nat k = 0; k < 7; ++k )
        {
            const Vertex<7> * v = p->vertex[k];
            if ( v < p ) continue;
            const Double<7> & vc = v->coor;
            const Vector2d b1 = Vector2d ( vc.d3, vc.d4 ) - a1;
            const Vector2d b2 = Vector2d ( vc.d5, vc.d6 ) - a2;
            const double c = b1 % b2;
            if ( c == 0 ) continue;
            const double t = - 0.5 * ( a1 % b2 + b1 % a2 ) / c;
            if ( t > 0 && t < 1 )
            {
                const Vector2d e1 = a1 + b1 * t;
                const Vector2d e2 = a2 + b2 * t;
                const double q = fabs ( e1 % e2 );
                if ( q > max )
                {
                    max = q;
                    const double s = 1 - t;
                    best.d0 = pc.d0 * s + vc.d0 * t;
                    best.d1 = pc.d1 * s + vc.d1 * t;
                    best.d2 = pc.d2 * s + vc.d2 * t;
                    best.d3 = e1.x;
                    best.d4 = e1.y;
                    best.d5 = e2.x;
                    best.d6 = e2.y;
                }
            }
        }
    }
    while ( show.next() );
    return max > 0;
}

static
bool maxParallelogramInConvexPolyhedronFN ( CArrRef<Vector2d> poly, CArrRef<Plane3d> plane, 
                                          WireModel<7> & model, List< Vertex<7> > & stor,
                                          Parallelogram3d & res
                                        )
{
    for ( nat i = 0; i < 1000; ++i )
    {
// ѕоиск максимального решени€
        Double<7> best;
        if ( ! maxParallelogramArea ( model, best ) ) return false;
// ѕоиск максимального нарушени€ ограничений дл€ выбранного решени€
        nat km;
        Vector2d pm;
        double max = 0.;
        for ( nat j = 0; j < poly.size(); ++j )
        {
            const Vector2d & p = poly[j];
            const Vector3d v ( best.d0 + best.d3*p.x + best.d5*p.y,
                               best.d1 + best.d4*p.x + best.d6*p.y,
                               best.d2 );
            for ( nat k = 0; k < plane.size(); ++k )
            {
                const double t = plane[k] % v;
                if ( max < t ) max = t, pm = p, km = k;
            }
        }
// ≈сли нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            if ( best.d3 * best.d6 < best.d4 * best.d5 )
            {
                _swap ( best.d3, best.d5 );
                _swap ( best.d4, best.d6 );
            }
            res = Parallelogram3d ( Vector3d ( best.d0 + best.d3 + best.d5, best.d1 + best.d4 + best.d6, best.d2 ),
                                    Vector3d ( best.d0 - best.d3 + best.d5, best.d1 - best.d4 + best.d6, best.d2 ),
                                    Vector3d ( best.d0 - best.d3 - best.d5, best.d1 - best.d4 - best.d6, best.d2 ) );
            return true;
        }
// ѕрименение ограничени€ к области допустимых преобразований
        const Vector3d & n = plane[km].norm;
        Double<8> g;
        g.d0 = n.x;
        g.d1 = n.y;
        g.d2 = n.z;
        g.d3 = n.x * pm.x;
        g.d4 = n.y * pm.x;
        g.d5 = n.x * pm.y;
        g.d6 = n.y * pm.y;
        g.d7 = plane[km].dist;
        model.cut ( g, stor );
    }
    return false;
}

Def<Parallelogram3d> maxParallelogramInConvexPolyhedronAFN ( const Polyhedron & poly )
{
    Def<Parallelogram3d> res;
// ѕриведение многоугольника к стандартному положению
    DynArray<Plane3d> plane ( poly.facet.size() );
    nat i;
    for ( i = 0; i < poly.facet.size(); ++i ) plane[i] = poly.facet[i].plane;
    const Segment3d seg = dimensions ( poly.vertex );
    const double max = normU ( seg );
    if ( max == 0 ) return res;
    const double coef = 2. / max;
    const Conform3d conf ( -0.5 * coef * ( seg.a + seg.b ), coef );
    plane *= Similar3d ( conf );
    FixArray<Vector2d, 4> coor;
    coor[0] = Vector2d(-1, 1);
    coor[1] = Vector2d(-1,-1);
    coor[2] = Vector2d( 1,-1);
    coor[3] = Vector2d( 1, 1);
    WireModel<7> model;
    List< Vertex<7> > stor;
    model.simplex ( 2*7, stor );
    Double<7> dn;
    dn.fill ( 1. );
    model.vlist -= dn;
    Double<8> g;
    g.d3 = 1;
    g.d5 = -1;
    g.d0 = g.d1 = g.d2 = g.d4 = g.d5 = g.d7 = 0;
    model.cut ( g, stor );
    if ( maxParallelogramInConvexPolyhedronFN ( coor, plane, model, stor, res ) )
    {
        res.isDef = true;
        res *= ~conf;
    }
    return res;
}

void maxParallelogramFN_test ()
{
	nat i;
    static PRandPoint3d rand;
    static Polyhedron poly;
    randPolyhedron ( 19, poly );
    double t0 = timeInSec();
    Def<Parallelogram3d> fig1 = maxParallelogramInConvexPolyhedronAFN ( poly );
    double t1 = timeInSec();
    FixArray<Vector3d, 4> vert;
    fig1.getVerts ( vert() );
    Def<Plane3d> plane = getPlane2 ( vert );
    Suite<DynArray<Vector3d> > sect;
    intersection ( poly, plane, sect );
    Suite<Vector2d> vert2;
    OrthoFunc3to2 func ( plane.norm );
    vert2.inc() = func ( sect[0][0] );
    for ( i = 1; i < sect[0].size(); ++i )
    {
        Vector2d v = func ( sect[0][i] );
        if ( norm2 ( v - vert2.las() ) > 1e-9 ) vert2.inc() = v;
    }
    if ( norm2 ( vert2[0] - vert2.las() ) < 1e-9 ) vert2.dec();
display << sect[0].size() << vert2.size() << NL;
    Def<Parallelogram2d> para = maxParallelogramInConvexPolygonA ( vert2 );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    draw ( fig1, 1, 0.3f, 0.3f, 1, VM_FLAT );
    draw ( fig1, 1, 0.3f, 0.3f, 1, VM_WIRE );
    OrthoFunc2to3 back ( plane );
    Vector2d vpar[4];
    para.getVerts ( vpar );
    for ( i = 0; i < 4; ++i ) vert[i] = back ( vpar[i] );
    drawPolygon ( vert, 1, 0.7f, 0.3f );
display << "time =" << t1 - t0 << fig1.getArea() - para.getArea() << NL;
}

void maxParallelogramFN_test2 ()
{
	nat i;
    static PRandPoint3d rand;
    static Polyhedron poly;
    for ( nat k = 0; k < 750; ++k )
    {
        randPolyhedron ( 19, poly );
//if(k!=122) continue;
        double t0 = timeInSec();
        Def<Parallelogram3d> fig1 = maxParallelogramInConvexPolyhedronAFN ( poly );
        double t1 = timeInSec();
        FixArray<Vector3d, 4> vert;
        fig1.getVerts ( vert() );
//check ( poly, vert() );
        Def<Plane3d> plane = getPlane2 ( vert );
        Suite<DynArray<Vector3d> > sect;
        intersection ( poly, plane, sect );
        Suite<Vector2d> vert2;
        OrthoFunc3to2 func ( plane.norm );
        vert2.inc() = func ( sect[0][0] );
        for ( i = 1; i < sect[0].size(); ++i )
        {
            Vector2d v = func ( sect[0][i] );
            if ( norm2 ( v - vert2.las() ) > 1e-9 ) vert2.inc() = v;
        }
        if ( norm2 ( vert2[0] - vert2.las() ) < 1e-9 ) vert2.dec();
        Def<Parallelogram2d> para = maxParallelogramInConvexPolygonA ( vert2 );
OrthoFunc2to3 back ( plane );
Vector2d vpar[4];
para.getVerts ( vpar );
for ( i = 0; i < 4; ++i ) vert[i] = back ( vpar[i] );
//check ( poly, vert() );
        if ( fabs ( fig1.getArea() - para.getArea() ) > 1e-4 )
            display << k << "time =" << t1 - t0 << fig1.getArea() << para.getArea() << 
            para.getArea() / fig1.getArea() - 1 << NL;
    }
display << "end" << NL;
}

void maxRectangle_test ()
{
    static PRandPoint3d rand;
    static Polyhedron poly;
    randPolyhedron ( 19, poly );
    double t0 = timeInSec();
    Def<Rectangle3d> fig1 = maxRectangleInConvexPolyhedronANR ( poly );
    double t1 = timeInSec();
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    draw ( fig1, 1, 0.7f, 0.3f, 1, VM_FLAT );
    draw ( fig1, 1, 0.7f, 0.3f, 1, VM_WIRE );
display << "time =" << t1 - t0 << NL;
}

void maxRhombus_test ()
{
    static PRandPoint3d rand;
    static Polyhedron poly;
    randPolyhedron ( 19, poly );
    double t0 = timeInSec();
    Def<Rhombus3d> fig1 = maxRhombusInConvexPolyhedronANR ( poly );
    double t1 = timeInSec();
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    draw ( fig1, 1, 0.7f, 0.3f, 1, VM_FLAT );
    draw ( fig1, 1, 0.7f, 0.3f, 1, VM_WIRE );
display << "time =" << t1 - t0 << NL;
}

void maxParallelogram_test ()
{
	nat i;
    static PRandPoint3d rand;
    static Polyhedron poly;
    randPolyhedron ( 19, poly );
    double t0 = timeInSec();
    Def<Parallelogram3d> fig1 = maxParallelogramInConvexPolyhedronP ( poly );
    if ( ! fig1.isDef ) return;
    double t1 = timeInSec();
    FixArray<Vector3d, 4> vert;
    fig1.getVerts ( vert() );
    Def<Plane3d> plane = getPlane2 ( vert );
    Suite<DynArray<Vector3d> > sect;
    intersection ( poly, plane, sect );
    Suite<Vector2d> vert2;
    OrthoFunc3to2 func ( plane.norm );
    vert2.inc() = func ( sect[0][0] );
    for ( i = 1; i < sect[0].size(); ++i )
    {
        Vector2d v = func ( sect[0][i] );
        if ( norm2 ( v - vert2.las() ) > 1e-9 ) vert2.inc() = v;
    }
    if ( norm2 ( vert2[0] - vert2.las() ) < 1e-9 ) vert2.dec();
//display << sect[0].size() << vert2.size() << NL;
    Def<Parallelogram2d> para = maxParallelogramInConvexPolygonP ( vert2 );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    draw ( fig1, 1, 0.3f, 0.3f, 1, VM_FLAT );
    draw ( fig1, 1, 0.3f, 0.3f, 1, VM_WIRE );
    OrthoFunc2to3 back ( plane );
    Vector2d vpar[4];
    para.getVerts ( vpar );
    for ( i = 0; i < 4; ++i ) vert[i] = back ( vpar[i] );
    drawPolygon ( vert, 1, 0.7f, 0.3f );
//display << "time =" << t1 - t0 << fig1.getArea() << para.getArea() << NL;
display << "time =" << t1 - t0 << fig1.getPerimeter() << para.getPerimeter() << NL;
}

//**************************** 18.08.2020 *********************************//
//
//          ћаксимальный эллипс вписанный в выпуклый многогранник
//                      A - максимум площади 
//
//**************************** 18.08.2020 *********************************//
static
bool maxEllipseInConvexPolyhedron(CCArrRef<Plane3d>& plane,
    bool (*func) (const WireModel<9>& model, Double<9>& best),
    Double<9>& best)
{
    WireModel<9> model;
    List< Vertex<9> > stor;
    model.simplex(2 * 9, stor);
    Double<9> dn;
    dn.fill(1.);
    dn.d3 = dn.d6 = 0;
    model.vlist -= dn;
    Double<10> g;
    g.d3 = 1;
    g.d6 = -1;
    g.d0 = g.d1 = g.d2 = g.d4 = g.d5 = g.d7 = g.d8 = g.d9 = 0;
    model.cut(g, stor);
    for (nat i = 0; i < 1000; ++i)
    {
        // ѕоиск максимального решени€
        if (!func(model, best))
            return false;
        const Vector3d o(best.d0, best.d1, best.d2);
        const Vector3d a(best.d3, best.d4, best.d5);
        const Vector3d b(best.d6, best.d7, best.d8);
        // ѕоиск максимального нарушени€ ограничений дл€ выбранного решени€
        nat km;
        double max = 0.;
        for (nat k = 0; k < plane.size(); ++k)
        {
            const Plane3d& p = plane[k];
            const double t = p % o + sqrt(_pow2(a * p.norm) + _pow2(b * p.norm));
            if (max < t) max = t, km = k;
        }
        // ≈сли нарушение мало, то завершение программы
        if (max < 1e-5)
        {
            return true;
        }
        // ѕрименение ограничени€ к области допустимых преобразований
        const double s = max - plane[km] % o;
        const Vector3d& n = plane[km].norm;
        const double ca = (n * a) / s;
        const double cb = (n * b) / s;
        Double<10> g;
        g.d0 = n.x;
        g.d1 = n.y;
        g.d2 = n.z;
        g.d3 = n.x * ca;
        g.d4 = n.y * ca;
        g.d5 = n.z * ca;
        g.d6 = n.x * cb;
        g.d7 = n.y * cb;
        g.d8 = n.z * cb;
        g.d9 = plane[km].dist;
        model.cut(g, stor);
    }
    return false;
}

bool maxEllipseInConvexPolyhedron(const Polyhedron& plh,
    bool (*func) (const WireModel<9>& model, Double<9>& best), Vector3d& o, Vector3d& a, Vector3d& b)
{
    // ѕриведение многогранника к стандартному положению
    DynArray<Plane3d> plane(plh.facet.size());
    nat i;
    for (i = 0; i < plh.facet.size(); ++i) plane[i] = plh.facet[i].plane;
    const Segment3d seg = dimensions(plh.vertex);
    const double max = normU(seg);
    if (max == 0) return false;
    const double coef = 2. / max;
    const Conform3d conf(-0.5 * coef * (seg.a + seg.b), coef);
    plane *= Similar3d(conf);
    Double<9> best;
    if (maxEllipseInConvexPolyhedron(plane, func, best))
    {
        o = Vector3d(best.d0, best.d1, best.d2);
        a = Vector3d(best.d3, best.d4, best.d5);
        b = Vector3d(best.d6, best.d7, best.d8);
        o *= ~conf;
        a /= coef;
        b /= coef;
        return true;
    }
    return false;
}

bool maxRectangleArea(const WireModel<9>& model, Double<9>& best);

Def<Ellipse3d> maxEllipseInConvexPolyhedronA ( const Polyhedron & outer )
{
    Vector3d o, a, b;
    Def<Ellipse3d> res;
    if ( ! maxEllipseInConvexPolyhedron ( outer, maxRectangleArea, o, a, b ) ) return res;
    res.o = o;
    res.a = norm2 ( a );
    res.b = norm2 ( b );
    res.spin = Spin3d ( a, b, a % b );
    res.isDef = true;
    return res;
}

void maxPolygon_test()
{
    static PRandPoint3d rand;
    static Polyhedron poly;
    for ( nat i = 0; i < 1; ++i ) randPolyhedron ( 11, poly );
    double t0 = timeInSec();
//    Def<Parallelogram3d> fig1 = maxParallelogramInConvexPolyhedronA ( poly );
//    Def<Rectangle3d> fig1 = maxRectangleInConvexPolyhedronA ( poly );
//    Def<Rhombus3d> fig1 = maxRhombusInConvexPolyhedronA ( poly );
    Def<Ellipse3d> fig1 = maxEllipseInConvexPolyhedronA ( poly );
//    Def<Ellipse3d> fig1 = maxEllipseInTetrahedronA ( poly.vertex[0], poly.vertex[1], poly.vertex[2], poly.vertex[3] );
    if (!fig1.isDef)
    {
        display << "error" << NL; return;
    }
    double t1 = timeInSec();
//    Def<Parallelogram3d> fig2 = maxParallelogramInConvexPolyhedronP2 ( poly );
//    if ( ! fig2.isDef ) return;
    double t2 = timeInSec();
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
//    draw ( fig1, 0.3f, 0.3f, 1, 1, VM_WIRE );
    draw ( fig1, 1, 0.3f, 0.3f, 1, VM_WIRE );
// drawPolygon(res, 1, 0.3f, 0.3f);
display << "time =" << t1 - t0 << _max ( fig1.a, fig1.b ) / _min ( fig1.a, fig1.b ) << NL;
}

Def<Parallelogram3d> maxParallelogramInConvexPolyhedronA2 ( const Polyhedron & poly );
Def<Parallelogram3d> maxParallelogramInConvexPolyhedronP2 ( const Polyhedron & poly );

void maxParallelogram_test2 ()
{
    static PRandPoint3d rand;
    static Polyhedron poly;
    randPolyhedron ( 19, poly );
    double t0 = timeInSec();
    Def<Parallelogram3d> fig1 = maxParallelogramInConvexPolyhedronP ( poly );
    if ( ! fig1.isDef ) return;
    double t1 = timeInSec();
    Def<Parallelogram3d> fig2 = maxParallelogramInConvexPolyhedronP2 ( poly );
    if ( ! fig2.isDef ) return;
    double t2 = timeInSec();
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    draw ( fig1, 0.3f, 0.3f, 1, 1, VM_WIRE );
    draw ( fig2, 1, 0.3f, 0.3f, 1, VM_WIRE );
display << "A2 =" << t2 - t1 << fig2.getArea() - fig1.getArea() << NL;
display << "A =" << t1 - t0 << fig1.getPerimeter()  - fig2.getPerimeter() << NL;
display << (t2 - t1) / (t1 - t0) << NL;
}

void opti2d_3d_test ()
{
    drawNewList2d();
    maxPolygon_test();
    endNewList();
}