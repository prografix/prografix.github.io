
#include <math.h>

#include "rand.h"
#include "func2d.h"
#include "func3d.h"
#include "opti3d.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "WireModel.h"
#include "opti2d_3d.h"
#include "Polyhedron.h"

//**************************** 29.11.2017 *********************************//
//
//      Максимальный круг вписанный в выпуклый многогранник
//
//**************************** 29.11.2017 *********************************//

class FarPoint
{
public:
    virtual Vector3d operator() ( const Vector3d & a, const Vector3d & v ) = 0;
};

bool maxSolidOfRevolutionInConvexPolyhedron ( const Polyhedron & poly, bool sym, List< Vertex<6> > & stor,
                                              Conform3d & res, FarPoint & func );

class CircleFarPoint : public FarPoint
{
public:
    virtual Vector3d operator() ( const Vector3d & a, const Vector3d & v )
    {
        return ( a - ( a * v ) * v ).setNorm2();
    }
};

Def<Circle3d> maxCircleInConvexPolyhedron ( const Polyhedron & poly )
{
    Conform3d conf;
    Def<Circle3d> res;
    List< Vertex<6> > stor;
    if ( ! maxSolidOfRevolutionInConvexPolyhedron ( poly, true, stor, conf, CircleFarPoint() ) )
        return res;
    res.isDef = true;
    res.spin = conf.spin;
    res.o = conf.trans;
    res.r = conf.magn;
    return res;
}

//**************************** 08.07.2018 *********************************//
//
//      Максимальный треугольник вписанный в выпуклый многогранник
//
//**************************** 08.07.2018 *********************************//

Def<Triangle3d> maxTriangleInConvexPolyhedronA ( const Polyhedron & poly )
{
    Def<Triangle3d> triangle;
    CCArrRef<Vector3d> & vert = poly.vertex;
    if ( vert.size() < 3 ) return triangle;
    nat m0(0), m1(1), m2(2);
    double max = qmod ( ( vert[m1] - vert[m0] ) % ( vert[m2] - vert[m1] ) );
    for ( nat i0 =    0; i0 < vert.size() - 2; ++i0 )
    for ( nat i1 = i0+1; i1 < vert.size() - 1; ++i1 )
    for ( nat i2 = i1+1; i2 < vert.size()    ; ++i2 )
    {
        const double d = qmod ( ( vert[i1] - vert[i0] ) % ( vert[i2] - vert[i1] ) );
        if ( max < d )
        {
            max = d;
            m0 = i0;
            m1 = i1;
            m2 = i2;
        }
    }
    triangle.a = vert[m0];
    triangle.b = vert[m1];
    triangle.c = vert[m2];
    triangle.isDef = true;
    return triangle;
}

//**************************** 08.10.2018 *********************************//
//
//       Максимальный прямоугольник вписанный в выпуклый многогранник
//                            без вращения
//
//**************************** 08.10.2018 *********************************//

bool maxStretchPolygonInConvexPolyhedronANR ( CCArrRef<Vector2d> & inner, const Polyhedron & outer, 
                                              double & a, double & b, Vector3d & o )
{
// Приведение многоугольника к стандартному положению
    DynArray<Plane3d> plane ( outer.facet.size() );
    nat i;
    for ( i = 0; i < outer.facet.size(); ++i ) plane[i] = outer.facet[i].plane;
    const Segment3d seg = dimensions ( outer.vertex );
    const double max = normU ( seg );
    if ( max == 0 ) return false;
    const double coef = 2. / max;
    const Conform3d conf ( -0.5 * coef * ( seg.a + seg.b ), coef );
    plane *= Similar3d ( conf );
// Инициализация области допустимых преобразований
    List< Vertex<5> > stor;
    WireModel<5> model;
    model.simplex ( 10., stor );
    Double<5> dn;
    dn.d0 = dn.d1 = 0.;
    dn.d2 = dn.d3 = dn.d4 = 1.;
    model.vlist -= dn;
    const double eps = 1e-7;
    for ( i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        Show< Vertex<5> > show ( model.vlist );
        Double<5> best;
        double max = -1.;
        if ( show.top() )
        do
        {
            const Vertex<5> * p = show.cur();
            const Double<5> & pc = p->coor;
            const double t = pc.d0 * pc.d1;
            if ( max < t ) max = t, best = pc;
            for ( nat k = 0; k < 5; ++k )
            {
                const Vertex<5> * v = p->vertex[k];
                if ( v < p ) continue;
                const Double<5> & vc = v->coor;
                const double a0 = vc.d0 - pc.d0;
                const double a1 = vc.d1 - pc.d1;
                const double a = a0 * a1;
                if ( a == 0 ) continue;
                const double t = -0.5 * ( a1 * pc.d0 + a0 * pc.d1 ) / a;
                if ( t > 0 && t < 1 )
                {
                    const double e0 = pc.d0 + a0 * t;
                    const double e1 = pc.d1 + a1 * t;
                    const double p = e0 * e1;
                    if ( p > max )
                    {
                        max = p;
                        const double s = 1 - t;
                        best.d0 = e0;
                        best.d1 = e1;
                        best.d2 = pc.d2 * s + vc.d2 * t;
                        best.d3 = pc.d3 * s + vc.d3 * t;
                        best.d4 = pc.d4 * s + vc.d4 * t;
                    }
                }
            }
        }
        while ( show.next() );
        if ( max <= 0 )
            return false;
// Поиск максимального нарушения ограничений для выбранного решения
        a = best.d0;
        b = best.d1;
        o = Vector3d ( best.d2, best.d3, best.d4 );
        Vector2d pm;
        nat k, km = 0;
        max = 0.;
        for ( k = 0; k < plane.size(); ++k )
        {
            const Plane3d & pk = plane[k];
            const Vector2d dir ( a * pk.norm.x, b * pk.norm.y );
            const Vector2d p = getFarthestPoint ( inner, dir );
            const double t = dir * p + pk % o;
            if ( max < t ) max = t, pm = p, km = k;
        }
// Если нарушение мало, то завершаем программу
        if ( max < eps )
        {
            const Conform3d back = ~conf;
            a *= back.magn;
            b *= back.magn;
            o *= back;
            return true;
        }
// Применение ограничения к области допустимых преобразований
        const double x = plane[km].norm.x;
        const double y = plane[km].norm.y;
        Double<6> cor;
        cor.d0 = x * pm.x;
        cor.d1 = y * pm.y;
        cor.d2 = x;
        cor.d3 = y;
        cor.d4 = plane[km].norm.z;
        cor.d5 = plane[km].dist;
        model.cut ( cor, stor );
    }
    return false;
}

Def<Rectangle3d> maxRectangleInConvexPolyhedronANR ( const Polyhedron & outer )
{
    Def<Rectangle3d> res;
    FixArray<Vector2d, 4> inner;
    inner[0] = Vector2d ( 1, 1 );
    inner[1] = Vector2d (-1, 1 );
    inner[2] = Vector2d (-1,-1 );
    inner[3] = Vector2d ( 1,-1 );
    if ( ! maxStretchPolygonInConvexPolyhedronANR ( inner, outer, res.a, res.b, res.o ) ) return res;
    res.isDef = true;
    return res;
}

//**************************** 08.10.2018 *********************************//
//
//         Максимальный ромб вписанный в выпуклый многогранник
//                            без вращения
//
//**************************** 08.10.2018 *********************************//

Def<Rhombus3d> maxRhombusInConvexPolyhedronANR ( const Polyhedron & outer )
{
    Def<Rhombus3d> res;
    FixArray<Vector2d, 4> inner;
    inner[0] = Vector2d ( 1, 0 );
    inner[1] = Vector2d ( 0, 1 );
    inner[2] = Vector2d (-1, 0 );
    inner[3] = Vector2d ( 0,-1 );
    if ( ! maxStretchPolygonInConvexPolyhedronANR ( inner, outer, res.a, res.b, res.o ) ) return res;
    res.isDef = true;
    return res;
}

//**************************** 08.04.2018 *********************************//
//
//      Максимальный параллелограмм вписанный в выпуклый многогранник
//
//**************************** 16.12.2018 *********************************//

static
bool maxParallelogramInConvexPolyhedron ( CCArrRef<Vector2d> & poly, CCArrRef<Plane3d> & plane, 
                                          bool (*func) ( const WireModel<9> & model, Double<9> & best ),
                                          Double<9> & best
                                        )
{
    WireModel<9> model;
    List< Vertex<9> > stor;
    model.simplex ( 2*9, stor );
    Double<9> dn;
    dn.fill ( 1. );
    dn.d3 = dn.d6 = 0;
    model.vlist -= dn;
    Double<10> g;
    g.d3 = 1;
    g.d6 = -1;
    g.d0 = g.d1 = g.d2 = g.d4 = g.d5 = g.d7 = g.d8 = g.d9 = 0;
    model.cut ( g, stor );
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        if ( ! func ( model, best ) ) return false;
// Поиск максимального нарушения ограничений для выбранного решения
        nat km;
        Vector2d pm;
        double max = 0.;
        for ( nat j = 0; j < poly.size(); ++j )
        {
            const Vector2d & p = poly[j];
            const Vector3d v ( best.d6*p.y + best.d3*p.x + best.d0,
                               best.d7*p.y + best.d4*p.x + best.d1,
                               best.d8*p.y + best.d5*p.x + best.d2 );
            for ( nat k = 0; k < plane.size(); ++k )
            {
                const double t = plane[k] % v;
                if ( max < t ) max = t, pm = p, km = k;
            }
        }
// Если нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            return true;
        }
// Применение ограничения к области допустимых преобразований
        const Vector3d & n = plane[km].norm;
        Double<10> g;
        g.d0 = n.x;
        g.d1 = n.y;
        g.d2 = n.z;
        g.d3 = n.x * pm.x;
        g.d4 = n.y * pm.x;
        g.d5 = n.z * pm.x;
        g.d6 = n.x * pm.y;
        g.d7 = n.y * pm.y;
        g.d8 = n.z * pm.y;
        g.d9 = plane[km].dist;
        model.cut ( g, stor );
    }
    return false;
}

static
Def<Parallelogram3d> maxParallelogramInConvexPolyhedron ( const Polyhedron & poly, bool (*func) ( const WireModel<9> & model, Double<9> & best ) )
{
    Def<Parallelogram3d> res;
// Приведение многогранника к стандартному положению
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
    Double<9> best;
    if ( maxParallelogramInConvexPolyhedron ( coor, plane, func, best ) )
    {
        const Vector3d a ( best.d0, best.d1, best.d2 ), 
                       b ( best.d3, best.d4, best.d5 ), 
                       c ( best.d6, best.d7, best.d8 );
        res = Parallelogram3d ( a - b - c, a + b - c, a + b + c );
        res.isDef = true;
        res *= ~conf;
    }
    return res;
}

static bool maxParallelogramPerimeter ( const WireModel<9> & model, Double<9> & best )
{
    Show< Vertex<9> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Double<9> & pc = show.cur()->coor;
        const double t = sqrt ( pc.d3 * pc.d3 + pc.d4 * pc.d4 + pc.d5 * pc.d5 ) +
                         sqrt ( pc.d6 * pc.d6 + pc.d7 * pc.d7 + pc.d8 * pc.d8 );
        if ( max < t ) max = t, best = pc;
    }
    while ( show.next() );
    return max > 0;
}

Def<Parallelogram3d> maxParallelogramInConvexPolyhedronP ( const Polyhedron & poly )
{
    return maxParallelogramInConvexPolyhedron ( poly, maxParallelogramPerimeter );
}

static bool maxParallelogramArea ( const WireModel<9> & model, Double<9> & best )
{
    Show< Vertex<9> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Vertex<9> * p = show.cur();
        const Double<9> & pc = p->coor;
        const Vector3d a1 ( pc.d3, pc.d4, pc.d5 );
        const Vector3d a2 ( pc.d6, pc.d7, pc.d8 );
        const double t = qmod ( a1 % a2 );
        if ( max < t ) max = t, best = pc;
    }
    while ( show.next() );
    return max > 0;
}

Def<Parallelogram3d> maxParallelogramInConvexPolyhedronA ( const Polyhedron & poly )
{
    return maxParallelogramInConvexPolyhedron ( poly, maxParallelogramArea );
}

//**************************** 08.04.2018 *********************************//
//
//      Максимальный прямоугольник вписанный в выпуклый многогранник
//
//**************************** 16.12.2018 *********************************//

static
bool maxPolygon1pInConvexPolyhedron ( CCArrRef<Vector2d> & poly, CCArrRef<Plane3d> & plane, 
                                          bool (*func) ( const WireModel<9> & model, Double<9> & best ),
                                          Double<9> & best
                                    )
{
    WireModel<9> model;
    List< Vertex<9> > stor;
    model.simplex ( 2*9, stor );
    Double<9> dn;
    dn.fill ( 1. );
    dn.d3 = dn.d6 = 0;
    model.vlist -= dn;
    Double<10> g;
    g.d3 = 1;
    g.d6 = -1;
    g.d0 = g.d1 = g.d2 = g.d4 = g.d5 = g.d7 = g.d8 = g.d9 = 0;
    model.cut ( g, stor );
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        if ( ! func ( model, best ) ) return false;
// Поиск максимального нарушения ограничений для выбранного решения
        nat km;
        Vector2d pm;
        double max = 0.;
        for ( nat j = 0; j < poly.size(); ++j )
        {
            const Vector2d & p = poly[j];
            const Vector3d v ( best.d6*p.y + best.d3*p.x + best.d0,
                               best.d7*p.y + best.d4*p.x + best.d1,
                               best.d8*p.y + best.d5*p.x + best.d2 );
            for ( nat k = 0; k < plane.size(); ++k )
            {
                const double t = plane[k] % v;
                if ( max < t ) max = t, pm = p, km = k;
            }
        }
// Если нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            return true;
        }
// Применение ограничения к области допустимых преобразований
        const Vector3d & n = plane[km].norm;
        Double<10> g;
        g.d0 = n.x;
        g.d1 = n.y;
        g.d2 = n.z;
        g.d3 = n.x * pm.x;
        g.d4 = n.y * pm.x;
        g.d5 = n.z * pm.x;
        g.d6 = n.x * pm.y;
        g.d7 = n.y * pm.y;
        g.d8 = n.z * pm.y;
        g.d9 = plane[km].dist;
        model.cut ( g, stor );
    }
    return false;
}

bool maxPolygon1pInConvexPolyhedron ( CCArrRef<Vector2d> & plg, const Polyhedron & plh, ArrRef<Vector3d> & res )
{
// Приведение многогранника к стандартному положению
    DynArray<Plane3d> plane ( plh.facet.size() );
    nat i;
    for ( i = 0; i < plh.facet.size(); ++i ) plane[i] = plh.facet[i].plane;
    const Segment3d seg = dimensions ( plh.vertex );
    const double max = normU ( seg );
    if ( max == 0 ) return false;
    const double coef = 2. / max;
    const Conform3d conf ( -0.5 * coef * ( seg.a + seg.b ), coef );
    plane *= Similar3d ( conf );
    Double<9> best;
    if ( maxPolygon1pInConvexPolyhedron ( plg, plane, maxParallelogramArea, best ) )
    {
        for ( i = 0; i < plg.size(); ++i )
        {
        const Vector3d a ( best.d0, best.d1, best.d2 ), 
                       b ( best.d3, best.d4, best.d5 ), 
                       c ( best.d6, best.d7, best.d8 );
        }
        //res *= ~conf;
    }
    return false;
}

//**************************** 01.10.2019 *********************************//
//
//      Максимальный свободный эллипс вписанный в треугольник
//                      A - максимум площади 
//
//**************************** 01.10.2019 *********************************//

Def<Ellipse3d> maxEllipseInTriangleA ( Vector3d p1, Vector3d p2, Vector3d p3 )
{
    Def<Ellipse3d> res;
// Вычисление параметров эллипса
    res.o = ( p1 + p2 + p3 ) / 3;
    p1 -= res.o;
    p2 -= res.o;
    const Vector3d n = p1 % p2;
    if ( !n ) return res;
    const Spin3d s1 ( n, Vector3d ( 0, 0, n.z < 0 ? -1 : 1 ) );
    const Ortho3d ortho ( s1 );
    const Vector2d v1 ( ortho.getX ( p1 ), ortho.getY ( p1 ) );
    const Vector2d v2 ( ortho.getX ( p2 ), ortho.getY ( p2 ) );
    const double D = v1 % v2;
    if ( D == 0 ) return res;
    const double a = ( v2.y - v1.y ) / D;
    const double b = ( v1.x - v2.x ) / D;
    const double c = ( v2.y + v1.y ) / D;
    const double d = ( v1.x + v2.x ) / D;
    const double A = a * a + 3 * c * c;
    const double B = a * b - 3 * c * d;
    const double C = b * b + 3 * d * d;
// Заполняем результат
    const double angle = 0.5 * atan2 ( B + B, A - C );
    const Spin2d s2 ( angle );
    Vector2d ax, ay;
    s2.getReper ( ax, ay );
    res.a = ay.x * ( C * ay.x + B * ay.y ) + ay.y * ( B * ay.x + A * ay.y );
    res.b = ax.x * ( C * ax.x + B * ax.y ) + ax.y * ( B * ax.x + A * ax.y );
    if ( res.a <= 0 || res.b <= 0 ) return res;
    res.a = 1 / sqrt ( res.a );
    res.b = 1 / sqrt ( res.b );
    res.spin = ~s1 * Spin3d ( Vector3d ( 0, 0, 1 ), angle );
    res.isDef = true;
    return res;
}

//**************************** 01.10.2019 *********************************//
//
//        Максимальный свободный эллипс вписанный в тетраэдр
//                      A - максимум площади 
//
//**************************** 01.10.2019 *********************************//

Def<Ellipse3d> maxEllipseInTetrahedronA ( Vector3d p1, Vector3d p2, Vector3d p3, Vector3d p4 )
{
    Def<Ellipse3d> res, e;
    double max = -1;
    e = maxEllipseInTriangleA ( p1, p2, p3 );
    if ( e.isDef )
    {
        res = e;
        max = e.a * e.b;
    }
    e = maxEllipseInTriangleA ( p1, p2, p4 );
    if ( e.isDef )
    {
        const double t = e.a * e.b;
        if ( max < t ) res = e, max = t;
    }
    e = maxEllipseInTriangleA ( p1, p3, p4 );
    if ( e.isDef )
    {
        const double t = e.a * e.b;
        if ( max < t ) res = e, max = t;
    }
    e = maxEllipseInTriangleA ( p2, p3, p4 );
    if ( e.isDef )
    {
        const double t = e.a * e.b;
        if ( max < t ) res = e, max = t;
    }
    return res;
}
