
#include <math.h>

#include "moment3d.h"
#include "approx3d.h"
#include "LinAlg.h"
#include "Mathem.h"
#include "func1t.h"
#include "func2d.h"
#include "func3d.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "ShevArray.h"
#include "Polyhedron.h"
#include "Lists.h"
#include "trans.h"
#include "heap.h"

//**************************** 17.11.2012 *********************************//
//
//                            Объём фигуры
//
//**************************** 17.11.2012 *********************************//

double volume ( const Polyhedron & poly ) // Объём многогранника
{
    return poly.volume();
}

//**************************** 17.11.2012 *********************************//
//
//                        Площади поверхностей
//
//**************************** 12.06.2017 *********************************//

double getArea ( const Facet & facet, CArrRef<Vector3d> vertex ) // Площадь грани многогранника
{
    return facet.getArea ( vertex );
}

double getArea ( const Polyhedron & fig ) // Площадь поверхности многогранника
{
    return fig.getArea();
}

double getArea ( const Triangle3d & fig ) // Площадь поверхности треугольника с двух сторон
{
    return fig.getArea();
}

double getArea ( const Sphere3d & fig )
{
    return fig.getArea();
}

double getArea ( const Ellipsoid3d & fig )
{
    return fig.getArea();
}

double getArea ( const Cuboid3d & fig )
{
    return fig.getArea();
}

double getArea ( const Cylinder3d & fig )
{
    return fig.getArea();
}

double getArea ( const Cone3d & fig )
{
    return fig.getArea();
}

//**************************** 10.10.2009 *********************************//
//
//            Определение габаритов множества точек
//
//**************************** 10.10.2009 *********************************//

Def<Segment3d> dimensions ( CArrRef<Vector3d> point )
{
    Def<Segment3d> seg;
    if ( point.size() == 0 ) return seg;
    seg.isDef = true;
    seg.a = seg.b = point[0];
    for ( nat i = 1; i < point.size(); ++i )
    {
        const Vector3d & v = point[i];
        if ( seg.b.x < v.x ) seg.b.x = v.x; else
        if ( seg.a.x > v.x ) seg.a.x = v.x;
        if ( seg.b.y < v.y ) seg.b.y = v.y; else
        if ( seg.a.y > v.y ) seg.a.y = v.y;
        if ( seg.b.z < v.z ) seg.b.z = v.z; else
        if ( seg.a.z > v.z ) seg.a.z = v.z;
    }
    return seg;
}

//**************************** 08.01.2009 *********************************//
//
//          Диаметр множества точек вдоль заданного направления
//
//**************************** 08.01.2009 *********************************//

double diameterPnt ( CArrRef<Vector3d> point, const Vector3d & dir, nat & imin, nat & imax )
{
    if ( point.size() == 0 ) return 0.;
    double min = dir * point[0];
    double max = min;
    imin = imax = 0;
    for ( nat i = 1; i < point.size(); ++i )
    {
        const double t = dir * point[i];
        if ( min > t ) min = t, imin = i; else
        if ( max < t ) max = t, imax = i;
    }
    return max - min;
}

double diameterPnt ( CArrRef<Vector3d> point, const Vector3d & dir )
{
    nat imin, imax;
    return diameterPnt ( point, dir, imin, imax );
}

//**************************** 09.12.2017 *********************************//
//
//               Минимальный диаметр множества точек
//
//**************************** 09.12.2017 *********************************//

double diameterPnt ( CArrRef<Vector3d> point )
{
    double r;
    getPlaneU ( point, r );
    return r + r;
}

//**************************** 03.06.2017 *********************************//
//
//      Вычисление квадрата расстояния от точки до треугольника
//
//**************************** 24.06.2017 *********************************//

// Функция getDistance2Edge вычисляет квадрат расстояния от точки до ребра треугольника

bool getDistance2Edge ( double a, double b, double c, double & d )
{
    // Проверим попадает ли проекция точки на ребро?
    const double t = fabs ( b - c );
    if ( t >= a ) return false;
    // Вычислим квадрат расстояния до ребра
    d = b + c - 0.5 * ( a + t * t / a );
    if ( d > 0 )
        d *= 0.5;
    else
        d = 0;
    return true;
}

double getDistance2Facet ( double a, double b, double c, double d, double e, double f, double q )
{
    // Вычислим скалярные произведения
    const double p1 = 0.5 * ( a + c - b );
    const double p2 = 0.5 * ( d + a - e );
    const double p3 = 0.5 * ( c + d - f );
    // Проверим попадает ли проекция точки внутрь треугольника?
    const double g = a * p3 - p1 * p2;
    if ( g <= 0 ) return q;
    // Если треугольник - вырожденный, то вернём квадрат расстояния до ребра
    const double h = a * c - p1 * p1;
    if ( h <= 0 ) return q;
    const double gg = g * g;
    const double ah = a * h;
    if ( 4 * gg * c >= ah * h ) return q;
    // Вычислим квадрат расстояния до плоскости треугольника
    q -= gg / ah;
    return q < 0 ? 0 : q;
}

double getDistance2Triangle ( double ab, double bc, double ca, double ap, double bp, double cp )
{
    double d = _min ( ap, bp, cp );
    if ( ! d ) return 0;
    // Вычислим квадраты расстояний до рёбер треугольника
    double d1, d2, d3;
    nat edge = 0; // номер ближайшего к точке ребра
    nat count = 0; // количество рёбер на которые есть проекция точки
    if ( getDistance2Edge ( ab, ap, bp, d1 ) )
    {
        if ( d > d1 ) d = d1, edge = 1;
        ++count;
    }
    if ( getDistance2Edge ( bc, bp, cp, d2 ) )
    {
        if ( d > d2 ) d = d2, edge = 2;
        ++count;
    }
    if ( getDistance2Edge ( ca, cp, ap, d3 ) )
    {
        if ( d > d3 ) d = d3, edge = 3;
        ++count;
    }
    if ( ! d ) return 0;
    // Если точка проецируется хотя бы на 2 ребра треугольника 
    if ( count > 1 )
    {
        switch ( edge )
        {
        case 1: return getDistance2Facet ( ab, bc, ca, ap, bp, cp, d );
        case 2: return getDistance2Facet ( bc, ca, ab, bp, cp, ap, d );
        case 3: return getDistance2Facet ( ca, ab, bc, cp, ap, bp, d );
        }
    }
    return d;
}

double getDistance2 ( const Triangle3d & t, const Vector3d & p )
{
    // Вычислим квадраты расстояний до вершин треугольника
    const double ap = qmod ( t.a - p );
    const double bp = qmod ( t.b - p );
    const double cp = qmod ( t.c - p );
    // Вычислим квадраты расстояний между вершинами треугольника
    const double ab = qmod ( t.a - t.b );
    const double bc = qmod ( t.b - t.c );
    const double ca = qmod ( t.c - t.a );
    // Основной алгоритм
    return getDistance2Triangle ( ab, bc, ca, ap, bp, cp );
}

//**************************** 27.01.2018 *********************************//
//
//          Вычисление расстояния от точки до прямоугольника
//
//**************************** 27.01.2018 *********************************//

double getDistance ( const Rectangle3d & fig, const Vector3d & p )
{
    Vector3d vert[4];
    fig.getVerts ( vert );
    const double d1 = getDistance2 ( Triangle3d ( vert[0], vert[1], vert[2] ), p );
    const double d2 = getDistance2 ( Triangle3d ( vert[0], vert[2], vert[3] ), p );
    return sqrt ( _min ( d1, d2 ) );
}

//**************************** 15.08.2009 *********************************//
//
//    Вычисление расстояния от точки до границы эллипсоида
//    a, b, c - это значения полуосей эллипсоида
//    d - это расстояние ( отрицательное, если точка внутри эллипсоида )
//    r - это ближайшая точка на границе эллипсоида
//    Возвращает false, если среди чисел a, b, c есть неположительные
//
//**************************** 15.08.2009 *********************************//

class EldDist : public MathFunc1
{
    double a, b, c, A, B, C;
public:
    EldDist ( double _a, double _b, double _c, double _A, double _B, double _C ) :
        a(_a), b(_b), c(_c), A(_A), B(_B), C(_C) {}
    double operator () ( double x ) const
    {
        double sum = 0;
        if ( A > 0 ) sum += A / _pow2 ( 1 + x * a );
        if ( B > 0 ) sum += B / _pow2 ( 1 + x * b );
        if ( C > 0 ) sum += C / _pow2 ( 1 + x * c );
        return sum - 1;
    }
};

bool getDistanceEld ( double a, double b, double c, const Vector3d & p, double & d, Vector3d & r )
{
    if ( a <= 0 || b <= 0 || c <= 0 ) return false;
    const double a2 = 1 / ( a * a );
    const double b2 = 1 / ( b * b );
    const double c2 = 1 / ( c * c );
    const double A = p.x * p.x * a2;
    const double B = p.y * p.y * b2;
    const double C = p.z * p.z * c2;
    const double D = A + B + C;
    const EldDist func ( a2, b2, c2, A, B, C );
    const double f = func ( 0 );
    if ( f > 0 )
    {
// Исходная точка вне эллипсоида
        double t = ( sqrt ( D ) - 1 ) / _min ( a2, b2, c2 );
        zeroin ( 0, t, func, 0, t ) ;
        r.x = p.x / ( 1 + t * a2 );
        r.y = p.y / ( 1 + t * b2 );
        r.z = p.z / ( 1 + t * c2 );
        d = norm2 ( p - r );
        return true;
    }
    if ( f < 0 )
    {
// Исходная точка внутри эллипсоида
        if ( a <= b && a <= c )
        {
            double t = a * ( fabs ( p.x ) - a );
            if ( p.x != 0 )
            {
                zeroin ( 0, t, func, 0, t );
                const double kx = 1 + t * a2;
                const double ky = 1 + t * b2;
                const double kz = 1 + t * c2;
                r.y = p.y / ky;
                r.z = p.z / kz;
                if ( ky > 1000 * kx || kz > 1000 * kx )
                {
                    r.x = a * sqrt ( 1 - r.y * r.y * b2 - r.z * r.z * c2 );
                    if ( p.x < 0 ) r.x = - r.x;
                }
                else
                    r.x = p.x / kx;
            }
            else
            {
                if ( a == b || a == c || func ( t ) > 0 )
                {
                    Vector2d v;
                    d = getDistanceElp ( b, c, Vector2d ( p.y, p.z ), v );
                    r.x = 0;
                    r.y = v.x;
                    r.z = v.y;
                    return true;
                }
                r.y = p.y / ( 1 + t * b2 );
                r.z = p.z / ( 1 + t * c2 );
                r.x = a * sqrt ( 1 - r.y * r.y * b2 - r.z * r.z * c2 );
            }
        }
        else
        if ( b <= a && b <= c )
        {
            double t = b * ( fabs ( p.y ) - b );
            if ( p.y != 0 )
            {
                zeroin ( 0, t, func, 0, t );
                const double kx = 1 + t * a2;
                const double ky = 1 + t * b2;
                const double kz = 1 + t * c2;
                r.x = p.x / kx;
                r.z = p.z / kz;
                if ( kx > 1000 * ky || kz > 1000 * ky )
                {
                    r.y = b * sqrt ( 1 - r.x * r.x * a2 - r.z * r.z * c2 );
                    if ( p.y < 0 ) r.y = - r.y;
                }
                else
                    r.y = p.y / ky;
            }
            else
            {
                if ( b == c || func ( t ) > 0 )
                {
                    Vector2d v;
                    d = getDistanceElp ( a, c, Vector2d ( p.x, p.z ), v );
                    r.x = v.x;
                    r.y = 0;
                    r.z = v.y;
                    return true;
                }
                r.x = p.x / ( 1 + t * a2 );
                r.z = p.z / ( 1 + t * c2 );
                r.y = b * sqrt ( 1 - r.x * r.x * a2 - r.z * r.z * c2 );
            }
        }
        else
        {
            double t = c * ( fabs ( p.z ) - c );
            if ( p.z != 0 )
            {
                zeroin ( 0, t, func, 0, t );
                const double kx = 1 + t * a2;
                const double ky = 1 + t * b2;
                const double kz = 1 + t * c2;
                r.x = p.x / kx;
                r.y = p.y / ky;
                if ( ky > 1000 * kz || kx > 1000 * kz )
                {
                    r.z = c * sqrt ( 1 - r.x * r.x * a2 - r.y * r.y * b2 );
                    if ( p.z < 0 ) r.z = - r.z;
                }
                else
                    r.z = p.z / kz;
            }
            else
            {
                if ( func ( t ) > 0 )
                {
                    Vector2d v;
                    d = getDistanceElp ( a, b, Vector2d ( p.x, p.y ), v );
                    r.x = v.x;
                    r.y = v.y;
                    r.z = 0;
                    return true;
                }
                r.x = p.x / ( 1 + t * a2 );
                r.y = p.y / ( 1 + t * b2 );
                r.z = c * sqrt ( 1 - r.x * r.x * a2 - r.y * r.y * b2 );
            }
        }
// Если найденная точка не лежит на границе эллипсоида, то сдвигаем её туда по нормали
        const double k0 = r.x * r.x * a2 + r.y * r.y * b2 + r.z * r.z * c2 - 1;
        if ( fabs ( k0 ) > 1e-15 )
        {
            const Vector3d n ( r.x * a2, r.y * b2, r.z * c2 );
            const double k1 = n.x * r.x * a2 + n.y * r.y * b2 + n.z * r.z * c2;
            const double k2 = n.x * n.x * a2 + n.y * n.y * b2 + n.z * n.z * c2;
            double D = k1 * k1 - k0 * k2;
            D = D > 0 ? sqrt ( D ) : 0;
            const double t = ( D - k1 ) / k2;
            r += n * t;
        }
        d = - norm2 ( p - r );
    }
    else
    {
// Исходная точка на границе эллипсоида
        r = p;
        d = 0;
    }
    return true;
}

//****************** 28.08.2003 *******************************//
//
//      Сфера построенная по трём точкам.
//      o - центр сферы, q - квадрат радиуса.
//
//*************************************************************//

bool getSphere ( Vector3d a, Vector3d b, const Vector3d & c, Vector3d & o, double & q )
{
    a -= c;
    b -= c;
    const double aa = a * a;
    const double ab = a * b;
    const double bb = b * b;
    double d = aa * bb - ab * ab;
    if ( d == 0 ) return false;
    d = 0.5 / d;
    const double at = bb * ( aa - ab ) * d;
    const double bt = aa * ( bb - ab ) * d;
    o = a * at + b * bt;
    q = o * o;
    o += c;
    return true;
}

//****************** 28.08.2003 *******************************//
//
//      Сфера построенная по четырём точкам.
//      o - центр сферы, q - квадрат радиуса.
//
//*************************************************************//

bool getSphere ( Vector3d a, Vector3d b, Vector3d c, const Vector3d & d, Vector3d & o, double & q )
{
    a -= d;
    b -= d;
    c -= d;
    SLU3<double> slu;
    slu.aa = a.x; slu.ab = a.y; slu.ac = a.z; slu.ad = a * a;
    slu.ba = b.x; slu.bb = b.y; slu.bc = b.z; slu.bd = b * b;
    slu.ca = c.x; slu.cb = c.y; slu.cc = c.z; slu.cd = c * c;
    if ( ! slu.gauss ( o.x, o.y, o.z ) ) return false;
    o *= 0.5;
    q = o * o;
    o += d;
    return true;
}

//****************** 11.04.2010 *******************************//
//
//      Сфера построенная по четырём плоскостям
//      Расстояния до центра сферы отрицательные
//
//****************** 11.04.2010 *******************************//

Def<Sphere3d> spherePPPP ( const Plane3d & a, const Plane3d & b, const Plane3d & c, const Plane3d & d )
{
    SLU3<double> slu;
    slu.aa = a.norm.x - d.norm.x;
    slu.ab = a.norm.y - d.norm.y;
    slu.ac = a.norm.z - d.norm.z;
    slu.ad = d.dist - a.dist;
    slu.ba = b.norm.x - d.norm.x;
    slu.bb = b.norm.y - d.norm.y;
    slu.bc = b.norm.z - d.norm.z;
    slu.bd = d.dist - b.dist;
    slu.ca = c.norm.x - d.norm.x;
    slu.cb = c.norm.y - d.norm.y;
    slu.cc = c.norm.z - d.norm.z;
    slu.cd = d.dist - c.dist;
    Def<Sphere3d> res;
    if ( slu.gauss ( res.o.x, res.o.y, res.o.z ) )
    {
        res.r = - ( d % res.o );
        res.isDef = res.r >= 0;
    }
    return res;
}

//****************** 23.01.2010 *******************************//
//
//      Выпуклая оболочка точек в пространстве.
//
//****************** 23.01.2010 *******************************//

class TrianFacet
{
public:
    nat vertex[3], facet[3], edge[3], index;
    Plane3d plane;
    List1d list;
};

inline void _swap ( SortItem<double, TrianFacet *> & p1, SortItem<double, TrianFacet *> & p2 )
{
    const SortItem<double, TrianFacet *> p ( p1 );
    p1 = p2;
    p2 = p;
    _swap ( p1.tail->index, p2.tail->index );
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

bool convexHull ( CArrRef<Vector3d> point, nat & nv, ArrRef<nat> & iv, nat & nf, ArrRef<TrianFacet> & facet )
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
            const Set2<nat> p = buf2.las();
            buf2.dec();
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
    return true;
}

Polyhedron & convexHull ( CArrRef<Vector3d> point, Polyhedron & poly )
{
    nat i, nf, nv;
    DynArray<nat> iv ( point.size() );
    DynArray<TrianFacet> facet ( 2*point.size() - 4 );
    if ( ! convexHull ( point, nv, iv, nf, facet ) ) return poly.makeVoid();
// Заполнение многогранника
    poly.vertex.resize ( nv );
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

//****************** 20.11.2003 *******************************//
//
//      Получение плоскостей выпуклой оболочки
//
//****************** 03.01.2011 *******************************//

DynArrRef<Plane3d> & convexHull ( CArrRef<Vector3d> point, DynArrRef<Plane3d> & plane )
{
    nat nf, nv;
    DynArray<nat> iv ( point.size() );
    DynArray<TrianFacet> facet ( 2*point.size() - 4 );
    if ( ! convexHull ( point, nv, iv, nf, facet ) )
    {
        return plane.resize(0);
    }
// Заполнение массива плоскостей
    plane.resize ( nf );
    for ( nat i = 0; i < nf; ++i ) plane[i] = facet[i].plane;
    return plane;
}

//****************** 04.11.2014 *******************************//
//
//      Выпуклая оболочка многогранника
//
//****************** 04.11.2014 *******************************//

struct BEdgeF3D
{
    nat next;
    BEdgeF3D * some;
};

struct IEdgeF3D : public BEdgeF3D
{
    const Facet * facet;
};

struct OEdgeF3D : public BEdgeF3D
{
    nat a, b, prev;
    bool isDel;
};

inline double qArea ( const TrianFacet & fa, CArrRef<Vector3d> vert )
{
    const Vector3d & v0 = vert[fa.vertex[0]];
    const Vector3d & v1 = vert[fa.vertex[1]];
    const Vector3d & v2 = vert[fa.vertex[2]];
    return qmod ( ( v0 - v1 ) % ( v1 - v2 ) );
}

Polyhedron & convexHull ( const Polyhedron & inner, Polyhedron & outer )
{
    nat i, j, nf, nv;
    DynArray<nat> iv ( inner.vertex.size() );
    DynArray<TrianFacet> facet ( 2*inner.vertex.size() - 4 );
    if ( ! convexHull ( inner.vertex, nv, iv, nf, facet ) ) return outer.makeVoid();
// Создание массивов рёбер
    Suite<SortItem<Set2<nat>, nat> > isi;
    Suite<IEdgeF3D> iedge;
    for ( i = 0; i < inner.facet.size(); ++i )
    {
        const Facet & fi = inner.facet[i];
        for ( j = 0; j < fi.nv; ++j )
        {
            SortItem<Set2<nat>, nat> & si = isi.inc();
            si.head.a = fi.index[j];
            si.head.b = fi.index[j+1];
            si.tail = iedge.size();
            IEdgeF3D & e = iedge.inc();
            e.some = 0;
            e.next = si.tail + 1;
            if ( j + 1 == fi.nv ) e.next -= fi.nv;
            e.facet = & fi;
        }
    }
    quickSort123 ( isi );
    DynArray<SortItem<Set2<nat>, nat> > osi ( 3 * nf );
    DynArray<OEdgeF3D> oedge ( 3 * nf );
    for ( i = 0; i < nf; ++i )
    {
        const nat i0 = 3 * i;
        const nat i1 = i0 + 1;
        const nat i2 = i0 + 2;
        const TrianFacet & fi = facet[i];
        {
            OEdgeF3D & e = oedge[i0];
            e.some = 0;
            e.next = i1;
            e.prev = i2;
            e.a = fi.vertex[0];
            e.b = fi.vertex[1];
            e.isDel = false;
            SortItem<Set2<nat>, nat> & si = osi[i0];
            si.head.a = iv[e.a];
            si.head.b = iv[e.b];
            si.tail = i0;
        }
        {
            OEdgeF3D & e = oedge[i1];
            e.some = 0;
            e.next = i2;
            e.prev = i0;
            e.a = fi.vertex[1];
            e.b = fi.vertex[2];
            e.isDel = false;
            SortItem<Set2<nat>, nat> & si = osi[i1];
            si.head.a = iv[e.a];
            si.head.b = iv[e.b];
            si.tail = i1;
        }
        {
            OEdgeF3D & e = oedge[i2];
            e.some = 0;
            e.next = i0;
            e.prev = i1;
            e.a = fi.vertex[2];
            e.b = fi.vertex[0];
            e.isDel = false;
            SortItem<Set2<nat>, nat> & si = osi[i2];
            si.head.a = iv[e.a];
            si.head.b = iv[e.b];
            si.tail = i2;
        }
    }
    quickSort123 ( osi );
// Найдём одинаковые рёбра у входного и выходного многогранников
    nat i1 = 0, i2 = 0, count = 0;
    for(;;)
    {
        if ( i1 == isi.size() || i2 == osi.size() ) break;
        SortItem<Set2<nat>, nat> & si1 = isi[i1];
        SortItem<Set2<nat>, nat> & si2 = osi[i2];
        if ( si1 < si2 )
        {
            ++i1;
            continue;
        }
        if ( si1 > si2 )
        {
            ++i2;
            continue;
        }
        BEdgeF3D & e1 = iedge[si1.tail];
        BEdgeF3D & e2 = oedge[si2.tail];
        e1.some = & e2;
        e2.some = & e1;
        ++count;
        ++i1;
        ++i2;
    }
    if ( count == iedge.size() )
    {
        return outer = inner;
    }
// Заполнение вершин выходного многогранника
    outer.vertex.resize ( nv );
    for ( i = 0; i < nv; ++i ) outer.vertex[i] = inner.vertex[iv[i]];
// Удаление лишних рёбер
    DynArray<Set2<Suite<nat>, const TrianFacet *> > arr ( oedge.size() );
    for ( i = 0; i < iedge.size(); ++i )
    {
        const IEdgeF3D & ei = iedge[i];
        if ( ! ei.some || ! iedge[ei.next].some ) continue;
m3:     nat ii = ei.some->next;
m2:     OEdgeF3D & e1 = oedge[ii];
        if ( !! e1.some ) continue;
        if ( e1.isDel ) goto m1; // проверка на случай зацикливания
        SortItem<Set2<nat>, nat> si;
        si.head.a = iv[e1.b];
        si.head.b = iv[e1.a];
        j = lasEqu123 ( osi, si );
        if ( j == osi.size() ) goto m1;
        OEdgeF3D & e2 = oedge[osi[j].tail];
        for ( nat k = 0; k < ei.facet->nv; ++k )
        {
            if ( ei.facet->index[k] != si.head.a ) continue;
            oedge[e1.prev].next = e2.next;
            oedge[e1.next].prev = e2.prev;
            oedge[e2.prev].next = e1.next;
            oedge[e2.next].prev = e1.prev;
            e1.isDel = e2.isDel = true;
            goto m3;
        }
        ii = e2.next;
        goto m2;
    }
// Заполнение граней выходного многогранника ( основной вариант )
    count = 0;
    for ( i = 0; i < oedge.size(); ++i )
    {
        if ( oedge[i].isDel ) continue;
        Set2<Suite<nat>, const TrianFacet *> & set = arr[i];
        set.b = facet(i/3);
        double max = qArea ( *set.b, outer.vertex );
        for ( j = i;; )
        {
            OEdgeF3D & t = oedge[j];
            set.a.inc() = t.a;
            if ( set.a.size() == oedge.size() ) goto m1; // проверка на случай зацикливания
            const TrianFacet * f = facet(j/3);
            if ( set.b != f )
            {
                const double t = qArea ( *f, outer.vertex );
                if ( max < t ) max = t, set.b = f;
            }
            t.isDel = true;
            j = t.next;
            if ( j == i ) break;
        }
        ++count;
    }
    outer.facet.resize ( count );
    count = 0;
    for ( i = 0; i < oedge.size(); ++i )
    {
        Set2<Suite<nat>, const TrianFacet *> & set = arr[i];
        if ( ! set.a.size() ) continue;
        Facet & fi = outer.facet[count];
        fi.plane = set.b->plane;
        fi.resize ( set.a.size() );
        for ( j = 0; j < set.a.size(); ++j ) fi.index[j] = set.a[j];
        ++count;
    }
    return outer.linkFacets();
// Заполнение граней выходного многогранника ( запасной вариант, если основной не получился )
m1: outer.facet.resize ( nf );
    for ( i = 0; i < nf; ++i )
    {
        const TrianFacet & cf = facet[i];
        Facet & fi = outer.facet[i];
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
    return outer;
}


//****************** 21.08.2012 *******************************//
//
//      Проверка многогранника на выпуклость
//
//****************** 21.08.2012 *******************************//

bool isConvex ( const Polyhedron & poly )
{
    const nat nf = poly.facet.size();
    for ( nat i = 0; i < nf; ++i )
    {
        const Facet & fi = poly.facet[i];
        for ( nat j = 0; j < fi.nv; ++j )
        {
            const nat k = fi.index[j+fi.nv+1];
            if ( k >= nf ) return false;
            if ( k < i ) continue;
            const Facet & fk = poly.facet[k];
            const Vector3d v = fi.plane.norm % fk.plane.norm;
            if ( v * v < 1e-20 ) continue;
            if ( v * ( poly.vertex[fi.index[j+1]] - poly.vertex[fi.index[j]] ) < 0 )
                return false;
        }
    }
    return true;
}


//****************** 22.04.2018 *******************************//
//
//      Получение поворота совмещающего пары векторов
//
//****************** 22.04.2018 *******************************//

Spin3d makeSpin3d ( CArrRef<Set2<Vector3d> > data )
{
    Spin3d spin;
    const nat n = data.size();
    if ( n < 2 )
    {
        if ( !n ) return spin;
        return Spin3d ( data[0].a, data[0].b );
    }
    double xx = 0, xy = 0, xz = 0, yx = 0, yy = 0, yz = 0, zx = 0, zy = 0, zz = 0;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        const Vector3d & a = data[i].a;
        const Vector3d & b = data[i].b;
        xx += a.x * b.x;
        xy += a.x * b.y;
        xz += a.x * b.z;
        yx += a.y * b.x;
        yy += a.y * b.y;
        yz += a.y * b.z;
        zx += a.z * b.x;
        zy += a.z * b.y;
        zz += a.z * b.z;
    }
    SMatrix<double, 4, 4> a, v;
    a[0][0] = xx - yy - zz;
    a[0][1] = xy + yx;
    a[0][2] = xz + zx;
    a[0][3] = yz - zy;
    a[1][1] = yy - zz - xx;
    a[1][2] = yz + zy;
    a[1][3] = zx - xz;
    a[2][2] = zz - yy - xx;
    a[2][3] = xy - yx;
    a[3][3] = xx + yy + zz;
    double d[4], max = 0;
    jacobi ( 4, a, d, v );
    for ( i = 0; i < 4; ++i )
    {
        if ( max >= d[i] ) continue;
        max = d[i];
        Set4<double> & r = (Set4<double> &) spin;
        r.a = v[3][i];
        r.b = v[0][i];
        r.c = v[1][i];
        r.d = v[2][i];
    }
    if ( !max )
    {
        Set4<double> & r = (Set4<double> &) spin;
        r.b = r.c = r.d = 0;
        r.a = 1;
    }
    return spin;
}