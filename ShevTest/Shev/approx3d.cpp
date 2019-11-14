
#include "math.h"

#include "tune.h"
#include "Mathem.h"
#include "Matrix.h"
#include "approx.h"
#include "func1t.h"
#include "func3d.h"
#include "opti1d.h"
#include "opti3d.h"
#include "moment3d.h"
#include "approx2d.h"
#include "approx3d.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "ShevArray.h"
#include "WireModel.h"
#include "Polyhedron.h"
#include "intersect3d.h"

//************************* 19.01.2006 ******************************//
//
//      Аппроксимация прямой множеств точек и отрезков.
//      Минимум суммы квадратов расстояний.
//      Вычисляются направление вдоль прямой ( единичный вектор dir )
//      и точка point через которую проходит прямая 
//      ( она является центром масс ).
//
//************************* 28.09.2010 ******************************//

inline Def<Line3d> getLine2 ( const Def<Mom3d> & mom )
{
    Def<Line3d> res;
    if ( mom.isDef )
    {
        res.dir = mom.maxNorm;
        res.point = mom.o;
        res.isDef = true;
    }
    return res;
}

Def<Line3d> getLine2 ( CArrRef<Vector3d> point )
{
    return getLine2 ( momentum2pnt ( point ) );
}

Def<Line3d> getLine2 ( CArrRef<Vector3d> point, CArrRef<double> mass )
{
    return getLine2 (  momentum2pnt ( point, mass ) );
}

Def<Line3d> getLine2 ( CArrRef<Segment3d> segm )
{
    return getLine2 ( momentum2sgm ( segm ) );
}

//************************* 02.05.2013 ******************************//
//
//          Аппроксимация прямой множеств точек
//          Минимум максимума расстояний ( r )
//
//************************* 14.01.2018 ******************************//

Line3d getLineU ( CCArrRef<Vector3d> & data, double & r )
{
    Line3d res;
    const nat n = data.size();
// Вырожденные случаи
    if ( n == 0 )
    {
        res.point = null3d;
        res.dir = Vector3d ( 0, 0, 1 );
        r = 0;
        return res;
    }
    if ( n == 1 )
    {
m1:     res.point = data[0];
        res.dir = Vector3d ( 0, 0, 1 );
        r = 0;
        return res;
    }
    if ( n == 2 )
    {
m2:     Vector3d v = data[1] - data[0];
        if ( ! v ) goto m1;
        res.point = 0.5 * ( data[0] + data[1] );
        res.dir = v.setNorm2();
        r = 0;
        return res;
    }
// Поиск первой начальной точки
    nat i, i0 = 0;
    double max = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[i] - data[0] );
        if ( max < t ) max = t, i0 = i;
    }
    if ( max == 0 ) goto m1;
    const Vector3d & p0 = data[i0];
// Поиск второй начальной точки
    nat i1 = i0;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( data[i] - p0 );
        if ( max < t ) max = t, i1 = i;
    }
// Поиск третьей начальной точки
    Vector3d v = data[i1] - p0;
    nat i2 = i0;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( ( data[i] - p0 ) % v );
        if ( max < t ) max = t, i2 = i;
    }
    if ( max == 0 || i2 == i1 || i2 == i0 ) goto m2;
// Найдём ответ для трёх найденных точек
    if ( qmod ( data[i2] - data[i1] ) > qmod ( data[i1] - data[i0] ) )
    {
        _swap ( i0, i2 );
    }
    res.point = 0.5 * ( data[i0] + data[i2] );
    v = data[i1] - data[i0];
    double d = norm2 ( v );
    res.dir = v / d;
    r = sqrt ( res.qdis ( data[i2] ) );
// Проверим остальные точки
    nat im = 0;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = res.qdis ( data[i] );
        if ( max < t ) max = t, im = i;
    }
    max = sqrt ( max );
    const double eps = 1e-6 * d;
    if ( max < r + eps || im == i2 || im == i1 || im == i0 ) return res;
// По простому не получилось, будем искать ответ по сложному
    const double coef = 1 / r;
    const Vector3d o = res.point;
    DynArray<Vector3d> temp ( data.size() );
    for ( i = 0; i < data.size(); ++i ) temp[i] = ( data[i] - o ) * coef;
// Инициализация области допустимых преобразований
    WireModel<6> model;
    List< Vertex<6> > stor;
    model.simplex ( 2*(6+1), stor );
    Double<6> dn;
    dn.fill ( 2 );
    dn.d0 = 0;
    model.vlist -= dn;
    for ( i = 0;; ++i )
    {
        model.vlist.top();
        do
        {
            Vertex<6> * p = model.vlist.cur();
            const Double<6> & pc = p->coor;
            const double t = pc.d0 * pc.d3 + pc.d1 * pc.d4 + pc.d2 * pc.d5;
            p->data = fabs ( t ) < 1e-14 ? 0 : t;
        }
        while ( model.vlist.next() );
        Double<6> best;
        max = 0;
        model.vlist.top();
        do
        {
            const Vertex<6> * p = model.vlist.cur();
            const Double<6> & pc = p->coor;
            if ( p->data > 0 ) continue;
            if ( ! p->data )
            {
                const double t = pc.d0 * pc.d0 + pc.d1 * pc.d1 + pc.d2 * pc.d2;
                if ( max < t ) max = t, best = pc;
            }
            else
            {
                const Vector3d & a1 = (const Vector3d &) pc.d0;
                const Vector3d & a2 = (const Vector3d &) pc.d3;
                for ( nat k = 0; k < 6; ++k )
                {
                    const Vertex<6> * v = p->vertex[k];
                    if ( v->data <= 0 ) continue;
                    const Double<6> & vc = v->coor;
                    const Vector3d & b1 = (const Vector3d &) vc.d0;
                    const Vector3d & b2 = (const Vector3d &) vc.d3;
                    const double C = a1 * a2;
                    const double ab = a1 * b2 + a2 * b1;
                    const double bb = b1 * b2;
                    const double A = C - ab + bb;
                    const double B = ab - C - C;
                    const double D = B * B - 4 * A * C;
                    const double t = 2 * C / ( B + ( D > 0 ? sqrt ( D ) : 0 ) );
                    const double t1 = t + 1;
                    const double e0 = a1.x * t1 - b1.x * t;
                    const double e1 = a1.y * t1 - b1.y * t;
                    const double e2 = a1.z * t1 - b1.z * t;
                    const double p = e0 * e0 + e1 * e1 + e2 * e2;
                    const double e3 = a2.x * t1 - b2.x * t;
                    const double e4 = a2.y * t1 - b2.y * t;
                    const double e5 = a2.z * t1 - b2.z * t;
                    if ( p > max )
                    {
                        max = p;
                        best.d0 = e0;
                        best.d1 = e1;
                        best.d2 = e2;
                        best.d3 = e3;
                        best.d4 = e4;
                        best.d5 = e5;
                    }
                }
            }
        }
        while ( model.vlist.next() );
        max = 0;
        nat jm = 0;
        const Vector3d & a = (const Vector3d &) best.d0;
        const Vector3d & b = (const Vector3d &) best.d3;
        for ( nat j = 0; j < n; ++j )
        {
            const double t = qmod ( a % temp[j] + b );
            if ( max < t ) max = t, jm = j;
        }
        max = sqrt ( max );
        if ( max < 1 + 1e-6 || i == 100 )
        {
            r = 1 / norm2 ( a );
            res.dir = a * r;
            res.point = ( res.dir % b ) * r;
            res.point = res.point / coef + o;
            r /= coef;
            break;
        }
        const Vector3d & p = temp[jm];
        const double qx = ( a.y * p.z - a.z * p.y + b.x );
        const double qy = ( a.z * p.x - a.x * p.z + b.y );
        const double qz = ( a.x * p.y - a.y * p.x + b.z );
        Double<7> cor;
        cor.d0 = qz * p.y - qy * p.z;
        cor.d1 = qx * p.z - qz * p.x;
        cor.d2 = qy * p.x - qx * p.y;
        cor.d3 = qx;
        cor.d4 = qy;
        cor.d5 = qz;
        cor.d6 = -max;
        double d = cor % best;
        model.cut ( cor, stor );
    }
    return res;
}

//*******************************************************************//
//
//      Аппроксимация плоскостью множеств точек и отрезков
//      Минимум суммы квадратов расстояний
//
//************************* 29.09.2010 ******************************//

inline Def<Plane3d> getPlane2 ( const Def<Mom3d> & mom )
{
    Def<Plane3d> res;
    if ( mom.isDef )
    {
        res.norm = mom.minNorm;
        res.dist = -(mom.minNorm*mom.o);
        res.isDef = true;
    }
    return res;
}

Def<Plane3d> getPlane2 ( CArrRef<Vector3d> point )
{
    return getPlane2 ( momentum2pnt ( point ) );
}

Def<Plane3d> getPlane2 ( CArrRef<Vector3d> point, CArrRef<double> mass )
{
    return getPlane2 ( momentum2pnt ( point, mass ) );
}

Def<Plane3d> getPlane2 ( CArrRef<Segment3d> segm )
{
    return getPlane2 ( momentum2sgm ( segm ) );
}

//************************ 05.02.2010 *******************************//
//
//          Аппроксимация плоскостью множества точек
//          Минимум максимума расстояний ( r )
//
//************************ 09.12.2017 *******************************//

inline double getMaxDev ( const Plane3d & plane, const Vector3d & v0, const Vector3d & v1, const Vector3d & v2, const Vector3d & v3 )
{
    double max = plane % v0;
    double t = plane % v1;
    if ( fabs ( max ) < fabs ( t ) ) max = t;
    t = plane % v2;
    if ( fabs ( max ) < fabs ( t ) ) max = t;
    t = plane % v3;
    if ( fabs ( max ) < fabs ( t ) ) max = t;
    return max;
}

inline const Double<5> & fillData ( const Plane3d & plane, const Vector3d & v, Double<5> & d5 )
{
    if ( plane % v < 0 )
    {
        d5.d0 = -v.x;
        d5.d1 = -v.y;
        d5.d2 = -v.z;
        d5.d3 = -1;
    }
    else
    {
        d5.d0 = v.x;
        d5.d1 = v.y;
        d5.d2 = v.z;
        d5.d3 = 1;
    }
    return d5;
}

inline double dist ( const Double<4> & d4, const Vector3d & v )
{
    return fabs ( d4.d0 * v.x + d4.d1 * v.y + d4.d2 * v.z + d4.d3 ) - 1;
}

Plane3d getPlaneU ( CCArrRef<Vector3d> & point, double & r )
{
    r = 0;
    const nat n = point.size();
    if ( ! n ) return Plane3d ( Vector3d ( 0, 0, 1 ), 0 );
// Находим первую начальную точку
    nat i, i0 = 0;
    double max = 0;
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[i] - point[0] );
        if ( max < q ) max = q, i0 = i;
    }
    if ( ! max )
    {
        return Plane3d ( Vector3d ( 0, 0, 1 ), 0 );
    }
// Находим вторую начальную точку
    nat i1 = 0;
    double max1 = qmod ( point[0] - point[i0] );
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[i] - point[i0] );
        if ( max1 < q ) max1 = q, i1 = i;
    }
    if ( ! max1 || i1 == i0 )
    {
        return Plane3d ( Vector3d ( 0, 0, 1 ), -point[0].z );
    }
    const double eps = 1e-9 * sqrt ( max );
// Находим третью начальную точку
    nat i2 = 0;
    Line3d line ( ( point[i1] - point[i0] ).setNorm2(), point[i0] );
    r = line.qdis ( point[0] );
    for ( i = 1; i < n; ++i )
    {
        const double t = line.qdis ( point[i] );
        if ( r < t ) r = t, i2 = i;
    }
    if ( max1 < qmod ( point[i2] - point[i1] ) )
    {
        _swap ( i0, i2 );
        line = Line3d ( ( point[i1] - point[i0] ).setNorm2(), point[i0] );
        r = line.qdis ( point[i2] );
    }
    r = sqrt ( r );
    if ( r < eps || i2 == i0 || i2 == i1 )
    {
        const Vector3d norm = line.dir.perpendicular();
        return Plane3d ( norm, - ( norm * line.point ) );
    }
// Находим четвёртую начальную точку
    nat i3 = 0;
    Plane3d plane ( point[i0], point[i1], point[i2] );
    r = plane % point[0];
    for ( i = 1; i < n; ++i )
    {
        const double t = plane % point[i];
        if ( fabs ( r ) < fabs ( t ) ) r = t, i3 = i;
    }
    if ( fabs ( r ) < eps || i3 == i0 || i3 == i1 || i3 == i2 )
    {
        r = fabs ( r );
        return plane;
    }
// Находим оптимальную плоскость для четырёх выбранных точек
    {
        const Plane3d temp ( point[i3], point[i1], point[i2] );
        const double t = temp % point[i0];
        if ( fabs ( r ) > fabs ( t ) ) r = t, plane = temp;
    }
    {
        const Plane3d temp ( point[i0], point[i3], point[i2] );
        const double t = temp % point[i1];
        if ( fabs ( r ) > fabs ( t ) ) r = t, plane = temp;
    }
    {
        const Plane3d temp ( point[i0], point[i1], point[i3] );
        const double t = temp % point[i2];
        if ( fabs ( r ) > fabs ( t ) ) r = t, plane = temp;
    }
    r *= 0.5;
    plane.dist -= r;
    const Vector3d o = -0.25 * ( point[i0] + point[i1] + point[i2] + point[i3] );
    {
        const Vector3d v1 = point[i1] - point[i0];
        const Vector3d v2 = point[i3] - point[i2];
        Plane3d temp;
        temp.norm = ( v1 % v2 ).setNorm2();
        temp.dist = temp.norm * o;
        const double t = getMaxDev ( temp, point[i0], point[i1], point[i2], point[i3] );
        if ( fabs ( r ) > fabs ( t ) ) r = t, plane = temp;
    }
    {
        const Vector3d v1 = point[i2] - point[i0];
        const Vector3d v2 = point[i3] - point[i1];
        Plane3d temp;
        temp.norm = ( v1 % v2 ).setNorm2();
        temp.dist = temp.norm * o;
        const double t = getMaxDev ( temp, point[i0], point[i1], point[i2], point[i3] );
        if ( fabs ( r ) > fabs ( t ) ) r = t, plane = temp;
    }
    {
        const Vector3d v1 = point[i3] - point[i0];
        const Vector3d v2 = point[i1] - point[i2];
        Plane3d temp;
        temp.norm = ( v1 % v2 ).setNorm2();
        temp.dist = temp.norm * o;
        const double t = getMaxDev ( temp, point[i0], point[i1], point[i2], point[i3] );
        if ( fabs ( r ) > fabs ( t ) ) r = t, plane = temp;
    }
    r = fabs ( r );
    nat im = 0;
    max = fabs ( plane % point[0] );
    for ( i = 1; i < n; ++i )
    {
        const double t = fabs ( plane % point[i] );
        if ( max < t ) max = t, im = i;
    }
// Если найденная плоскость оптимальна для всех точек, то выходим
    if ( max - r < eps || im == i0 || im == i1 || im == i2 || im == i3 )
    {
        return plane;
    }
// Инициализация области допустимых преобразований
    max = sqrt ( _max ( qmod ( point[i0] ), qmod ( point[i1] ), qmod ( point[i2] ), qmod ( point[i3] ) ) );
    const double a = 1 / r;
    WireModel<4> model;
    List< Vertex<4> > stor;
    model.simplex ( 8*a, stor );
    model.vlist.top();
    do
    {
        Double<4> & d = model.vlist.cur()->coor;
        d.d1 -= a;
        d.d2 -= a;
        d.d3 -= a;
        d.d3 *= max;
    }
    while ( model.vlist.next() );
    Double<5> d5;
    d5.d4 = -1;
    model.cut ( fillData ( plane, point[i0], d5 ), stor );
    model.cut ( fillData ( plane, point[i1], d5 ), stor );
    model.cut ( fillData ( plane, point[i2], d5 ), stor );
    model.cut ( fillData ( plane, point[i3], d5 ), stor );
    model.cut ( fillData ( plane, point[im], d5 ), stor );
    for ( nat j = 0; j < n+n; ++j )
    {
        // Поиск максимального решения
        Show< Vertex<4> > show ( model.vlist );
        const Vertex<4> * ptr = 0;
        max = -1.;
        if ( show.top() )
        do
        {
            const Vertex<4> * p = show.cur();
            const double t = p->coor.d0 * p->coor.d0 + p->coor.d1 * p->coor.d1 + p->coor.d2 * p->coor.d2;
            if ( max < t ) max = t, ptr = p;
        }
        while ( show.next() );
        if ( ! ptr ) break;
        const Double<4> & d4 = ptr->coor;
        r = 1 / sqrt ( d4.d0 * d4.d0 + d4.d1 * d4.d1 + d4.d2 * d4.d2 );
        plane.norm.x = d4.d0 * r;
        plane.norm.y = d4.d1 * r;
        plane.norm.z = d4.d2 * r;
        plane.dist   = d4.d3 * r;
        // Поиск максимального нарушения ограничений для выбранного решения
        im = 0;
        max = dist ( d4, point[0] );
        for ( i = 1; i < n; ++i )
        {
            const double t = dist ( d4, point[i] );
            if ( max < t ) max = t, im = i;
        }
        // Если нарушение мало, то завершение программы
        if ( max < a * eps )
            break;
        // Применение ограничения к области допустимых преобразований
        model.cut ( fillData ( plane, point[im], d5 ), stor );
    }
    return plane;
}

//************************ 23.06.2005 *******************************//
//
//              Аппроксимация точек сферой
//              Sum ( Ri^2 - R^2 )^2 -> min
//
//************************ 02.05.2010 *******************************//

Def<Sphere3d> getSpherePnt22 ( CArrRef<Vector3d> p )
{
    Def<Sphere3d> res;
    const nat n = p.size();
    if ( n < 4 ) return res;
    HMatrix<double> a ( n, 3 );
    CmbArray<double, 80> b ( n );
    double x = 0.;
    double y = 0.;
    double z = 0.;
    double q = 0.;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        const double & xi = p[i].x;
        const double & yi = p[i].y;
        const double & zi = p[i].z;
        x += xi;
        y += yi;
        z += zi;
        q += ( b[i] = xi * xi + yi * yi + zi * zi );
    }
    const double d = 1. / n;
    x *= d;
    y *= d;
    z *= d;
    q *= d;
    for ( i = 0; i < n; ++i )
    {
        a[i][0] = p[i].x - x;
        a[i][1] = p[i].y - y;
        a[i][2] = p[i].z - z;
        b[i] -= q;
    }
    FixArray<double, 3> c;
    if ( ortholin ( a, b, c ) )
    {
        res.o.x = 0.5 * c[0];
        res.o.y = 0.5 * c[1];
        res.o.z = 0.5 * c[2];
        res.r = sqrt ( res.o * res.o + q - 2. * ( res.o.x * x + res.o.y * y + res.o.z * z ) );
        res.isDef = true;
    }
    return res;
}

//************************ 15.05.2010 *******************************//
//
//              Аппроксимация точек сферой
//                Sum ( Ri - R )^2 -> min
//
//************************ 15.05.2010 *******************************//

Def<Sphere3d> getSpherePnt2 ( CArrRef<Vector3d> p )
{
    Def<Sphere3d> res = getSpherePnt22 ( p );
    if ( ! res.isDef || res.r == 0 ) return res;
    const nat n = p.size();
    HMatrix<double> a ( n, 4 );
    Suite<double> stor;
    CmbArray<double, 80> b ( n );
    for ( nat j = 0; j < 15; ++j )
    {
        const Vector3d o = res.o;
        for ( nat i = 0; i < n; ++i )
        {
            const Vector3d v = ( o - p[i] ).setNorm2();
            a[i][0] = v.x;
            a[i][1] = v.y;
            a[i][2] = v.z;
            a[i][3] = -1;
            b[i] = v * p[i];
        }
        FixArray<double, 4> c;
        if ( ! ortholin ( a, b, c, stor ) ) return Def<Sphere3d>();
        res.o.x = c[0];
        res.o.y = c[1];
        res.o.z = c[2];
        res.r   = c[3];
        if ( qmod ( res.o - o ) < 1e-16 * res.r * res.r ) break;
    }
    return res;
}

//************************ 15.05.2009 *******************************//
//
//          Аппроксимация многогранника сферой
//
//************************ 03.09.2010 *******************************//

Def<Sphere3d> getSpherePlg ( const Polyhedron & poly )
{
    Def<Sphere3d> res;
    const Def<Vector3d> o = centerOfMass ( poly );
    if ( ! o.isDef ) return res;
    res.r = root3 ( 0.75 * volume ( poly ) / M_PI );
    res.o = o;
    res.isDef = true;
    return res;
}

//************************ 28.11.2008 *******************************//
//
//          Аппроксимация многогранника эллипсоидом
//
//************************ 29.09.2010 *******************************//

Def<Ellipsoid3d> getEllipsoidPlg ( const Polyhedron & poly )
{
    return getEllipsoid ( momentum2plh ( poly ) );
}

//************************ 15.05.2009 *******************************//
//
//     Аппроксимация многогранника прямоугольным параллелепипедом
//
//************************ 28.09.2010 *******************************//

Def<Cuboid3d> getCuboid ( const Polyhedron & poly )
{
    return getCuboid ( momentum2plh ( poly ) );
}

//************************ 11.04.2008 *******************************//
//
//      Вычисление ближайшей точки к заданным плоскостям
//      Минимум суммы абсолютных расстояний ( 1-норма )
//
//************************ 05.09.2012 *******************************//

Def<Vector3d> getNearPoint1 ( CArrRef<Plane3d> plane, nat & ix1, nat & ix2, nat & ix3 )
{
    const nat n = plane.size();
    if ( n < 3 ) return Def<Vector3d>();
    CmbArray<bool, 80> used ( n );
    nat i, i1, i2, i3;
    double max = 0;
    for ( i = 0; i < n; ++i )
    {
        used[i] = false;
        const Vector3d & v = plane[i].norm;
        const double ax = fabs ( v.x );
        if ( max < ax ) max = ax, i1 = i;
        const double ay = fabs ( v.y );
        if ( max < ay ) max = ay, i1 = i;
        const double az = fabs ( v.z );
        if ( max < az ) max = az, i1 = i;
    }
    if ( max == 0 ) return Def<Vector3d>();
    CmbArray<Line2d, 40> line ( n );
    CmbArray<double, 80> darr (n+n);
    for(;;)
    {
        used[i1] = true;
        const Plane3d & p1 = plane[i1];
        const double ax = fabs ( p1.norm.x );
        const double ay = fabs ( p1.norm.y );
        const double az = fabs ( p1.norm.z );
        if ( ax >= ay && ax >= az )
        {
            for ( i = 0; i < n; ++i )
            {
                Line2d & li = line[i];
                if ( i == i1 )
                {
                    li.norm.fill();
                    li.dist = 0.;
                }
                else
                {
                    const Plane3d & pi = plane[i];
                    const double coef = pi.norm.x / p1.norm.x;
                    li.norm.x = pi.norm.y - p1.norm.y * coef;
                    li.norm.y = pi.norm.z - p1.norm.z * coef;
                    li.dist = pi.dist - p1.dist * coef;
                }
            }
        }
        else
        if ( ay >= ax && ay >= az )
        {
            for ( i = 0; i < n; ++i )
            {
                Line2d & li = line[i];
                if ( i == i1 )
                {
                    li.norm.fill();
                    li.dist = 0.;
                }
                else
                {
                    const Plane3d & pi = plane[i];
                    const double coef = pi.norm.y / p1.norm.y;
                    li.norm.x = pi.norm.x - p1.norm.x * coef;
                    li.norm.y = pi.norm.z - p1.norm.z * coef;
                    li.dist = pi.dist - p1.dist * coef;
                }
            }
        }
        else
        if ( az >= ax && az >= ay )
        {
            for ( i = 0; i < n; ++i )
            {
                Line2d & li = line[i];
                if ( i == i1 )
                {
                    li.norm.fill();
                    li.dist = 0.;
                }
                else
                {
                    const Plane3d & pi = plane[i];
                    const double coef = pi.norm.z / p1.norm.z;
                    li.norm.x = pi.norm.x - p1.norm.x * coef;
                    li.norm.y = pi.norm.y - p1.norm.y * coef;
                    li.dist = pi.dist - p1.dist * coef;
                }
            }
        }
        if ( ! getNearPoint1 ( line, i2, i3 ).isDef ) return Def<Vector3d>();
        const Def<Line3d> line3 = intersection ( plane[i2], plane[i3] );
        if ( ! line3.isDef ) return Def<Vector3d>();
        ArrRef<double> w ( darr, n, n );
        for ( i = 0; i < n; ++i )
        {
            const double s = line3.dir * plane[i].norm;
            w[i] = fabs ( s );
            if ( s != 0 )
            {
                darr[i] = ( plane[i] % line3.point ) / s;
            }
        }
        ArrRef<double> d ( darr, 0, n );
        Def<nat> im = selectR ( d, w, 0.5 );
        if ( ! im.isDef ) return Def<Vector3d>();
        if ( used[im] ) break;
        i1 = im.a;
    }
    ix1 = i1;
    ix2 = i2;
    ix3 = i3;
    return intersection ( plane[i1], plane[i2], plane[i3] );
}

Def<Vector3d> getNearPoint1 ( CArrRef<Plane3d> plane )
{
    nat ix1, ix2, ix3;
    return getNearPoint1 ( plane, ix1, ix2, ix3 );
}

//************************ 30.06.2013 *******************************//
//
//      Вычисление ближайшей точки к заданным плоскостям
//      Минимум p-нормы ( p = 4/3, 6/5, 8/7, 10/9 )
//
//************************ 30.06.2013 *******************************//

class NearPointFunc3d : public MathFunc1
{
    CArrRef<Plane3d> plane;
    const Vector3d o, g;
    const nat k;
public:
    NearPointFunc3d ( CArrRef<Plane3d> p1, const Vector3d & p2, const Vector3d & p3, nat p4 ) :
      plane(p1), o(p2), g(p3), k(p4) {}
    virtual double operator () ( double t ) const;
};

double NearPointFunc3d::operator () ( double t ) const
{
    double sum = 0;
    const Vector3d p = o + g * t;
    for ( nat i = 0; i < plane.size(); ++i )
    {
        const double t = plane[i] % p;
        switch ( k )
        {
        case 3:
            sum += t * root3 ( t ); break;
        case 5:
            sum += t * root5 ( t ); break;
        case 7:
            sum += t * root7 ( t ); break;
        default:
            sum += t * root9 ( t ); break;
        }
    }
    return sum;
}

class NearPointFunc5 : public MathFunc1
{
public:
    Vector2d v, a, b, c;
    nat k;

    virtual double operator () ( double t ) const;
};

double NearPointFunc5::operator () ( double t ) const
{
    Vector2d u ( v.x * ( 1 - fabs ( t ) ) + t * v.y, 
                 v.y * ( 1 - fabs ( t ) ) - t * v.x );
    switch ( k )
    {
    case 3:
        u /= root4 ( _pow4 ( u.x ) + _pow4 ( u.y ) + _pow4 ( a * u ) + _pow4 ( b * u ) + _pow4 ( c * u ) );
        break;
    case 5:
        u /= root6 ( _pow6 ( u.x ) + _pow6 ( u.y ) + _pow6 ( a * u ) + _pow6 ( b * u ) + _pow6 ( c * u ) );
        break;
    case 7:
        u /= root8 ( _pow8 ( u.x ) + _pow8 ( u.y ) + _pow8 ( a * u ) + _pow8 ( b * u ) + _pow8 ( c * u ) );
        break;
    default:
        u /= root10 ( _pow10 ( u.x ) + _pow10 ( u.y ) + _pow10 ( a * u ) + _pow10 ( b * u ) + _pow10 ( c * u ) );
    }
    return u * v;
}

Def<Vector3d> getNearPoint ( nat k, CArrRef<Plane3d> plane )
{
    Def<Vector3d> p;
    const nat n = plane.size();
    if ( n <  3  ) return p;
    if ( n == 3 ) return intersection ( plane[0], plane[1], plane[2] );
    if ( n == 4 )
    {
        FixArray2<double, 3, 4> mat, slu;
        FixArray<nat, 4> ind;
        nat i, j, m;
        for ( j = 0; j < 4; ++j )
        {
            const Vector3d & norm = plane[j].norm;
            mat[0][j] = norm.x;
            mat[1][j] = norm.y;
            mat[2][j] = norm.z;
            ind[j] = j;
        }
        for ( i = 0; i < 3; ++i )
        {
            nat im = i;
            ArrRef<double> & r = mat[i];
            double max = fabs ( r[ind[im]] );
            for ( j = i+1; j < 4; ++j )
            {
                const double t = fabs ( r[ind[j]] );
                if ( max < t ) max = t, im = j;
            }
            if ( ! max ) return p;
            if ( i != im ) _swap ( ind[i], ind[im] );
            max = r[ind[i]];
            for ( j = i+1; j < 4; ++j ) r[ind[j]] /= max;
            for ( m = 0; m < 3; ++m )
            {
                if ( i == m ) continue;
                ArrRef<double> & p = mat[m];
                max = p[ind[i]];
                for ( j = i+1; j < 4; ++j ) p[ind[j]] -= max * r[ind[j]];
            }
        }
        m = ind[3];
        switch ( k )
        {
        case 3:
            for ( i = 0; i < 3; ++i ) mat[i][m] = _pow3 ( mat[i][m] );
            break;
        case 5:
            for ( i = 0; i < 3; ++i ) mat[i][m] = _pow5 ( mat[i][m] );
            break;
        case 7:
            for ( i = 0; i < 3; ++i ) mat[i][m] = _pow7 ( mat[i][m] );
            break;
        default:
            for ( i = 0; i < 3; ++i ) mat[i][m] = _pow9 ( mat[i][m] );
        }
        const Plane3d & pm = plane[m];
        for ( i = 0; i < 3; ++i )
        {
            const Plane3d & pi = plane[ind[i]];
            ArrRef<double> & r = slu[i];
            const double & t = mat[i][m];
            r[0] = pi.norm.x + t * pm.norm.x;
            r[1] = pi.norm.y + t * pm.norm.y;
            r[2] = pi.norm.z + t * pm.norm.z;
            r[3] = -( pi.dist + t * pm.dist );
        }
        FixArray<double, 3> res;
        if ( slu_gauss ( slu, res ) )
        {
            p.isDef = true;
            p.x = res[0];
            p.y = res[1];
            p.z = res[2];
        }
        return p;
    }
    if ( n == 5 )
    {
        nat i, i1, i2, i3, i4, i5;
        SMatrix<double, 5, 4> A;
        double max = 0;
        for ( i = 0; i < 5; ++i )
        {
            const Plane3d & pi = plane[i];
            A[i][0] = pi.norm.x;
            A[i][1] = pi.norm.y;
            A[i][2] = pi.norm.z;
            A[i][3] = pi.dist;
            const double t = fabs ( pi.norm.x );
            if ( max < t ) max = t, i1 = i;
        }
        if ( max < 1e-9 ) return p;
        max = 0;
        for ( i = 0; i < 5; ++i )
        {
            if ( i == i1 ) continue;
            A[i][0] /= A[i1][0];
            A[i][1] -= A[i1][1] * A[i][0];
            A[i][2] -= A[i1][2] * A[i][0];
            A[i][3] -= A[i1][3] * A[i][0];
            const double t = fabs ( A[i][1] );
            if ( max < t ) max = t, i2 = i;
        }
        if ( max < 1e-9 ) return p;
        max = 0;
        for ( i = 0; i < 5; ++i )
        {
            if ( i == i1 ) continue;
            if ( i == i2 ) continue;
            A[i][1] /= A[i2][1];
            A[i][0] -= A[i2][0] * A[i][1];
            A[i][2] -= A[i2][2] * A[i][1];
            A[i][3] -= A[i2][3] * A[i][1];
            const double t = fabs ( A[i][2] );
            if ( max < t ) max = t, i3 = i;
        }
        if ( max < 1e-9 ) return p;
        max = 0;
        for ( i = 0; i < 5; ++i )
        {
            if ( i == i1 ) continue;
            if ( i == i2 ) continue;
            if ( i == i3 ) continue;
            A[i][2] /= A[i3][2];
            A[i][0] -= A[i3][0] * A[i][2];
            A[i][1] -= A[i3][1] * A[i][2];
            A[i][3] -= A[i3][3] * A[i][2];
            const double t = fabs ( A[i][3] );
            if ( max < t ) max = t, i4 = i;
        }
        if ( max < 1e-9 ) return intersection ( plane[i1], plane[i2], plane[i3] );
        i5 = 10 - i1 - i2 - i3 - i4;
        NearPointFunc5 func;
        func.k = k;
        func.a = Vector2d ( A[i4][0], A[i5][0] );
        func.b = Vector2d ( A[i4][1], A[i5][1] );
        func.c = Vector2d ( A[i4][2], A[i5][2] );
        func.v = Vector2d ( A[i4][3], A[i5][3] );
        double t = fmax ( -1, 1, func, 0 );
        Vector2d u ( func.v.x * ( 1 - fabs ( t ) ) + t * func.v.y, 
                     func.v.y * ( 1 - fabs ( t ) ) - t * func.v.x );
        switch ( k )
        {
        case 3:
            u /= root4 ( _pow4 ( u.x ) + _pow4 ( u.y ) + _pow4 ( func.a * u ) + _pow4 ( func.b * u ) + _pow4 ( func.c * u ) );
            break;
        case 5:
            u /= root6 ( _pow6 ( u.x ) + _pow6 ( u.y ) + _pow6 ( func.a * u ) + _pow6 ( func.b * u ) + _pow6 ( func.c * u ) );
            break;
        case 7:
            u /= root8 ( _pow8 ( u.x ) + _pow8 ( u.y ) + _pow8 ( func.a * u ) + _pow8 ( func.b * u ) + _pow8 ( func.c * u ) );
            break;
        default:
            u /= root10 ( _pow10 ( u.x ) + _pow10 ( u.y ) + _pow10 ( func.a * u ) + _pow10 ( func.b * u ) + _pow10 ( func.c * u ) );
        }
        double uv = u * func.v;
        double ua = u * func.a;
        double ub = u * func.b;
        double uc = u * func.c;
        switch ( k )
        {
        case 3:
            ua = _pow3 ( ua );
            ub = _pow3 ( ub );
            uc = _pow3 ( uc );
            break;
        case 5:
            ua = _pow5 ( ua );
            ub = _pow5 ( ub );
            uc = _pow5 ( uc );
            break;
        case 7:
            ua = _pow7 ( ua );
            ub = _pow7 ( ub );
            uc = _pow7 ( uc );
            break;
        default:
            ua = _pow9 ( ua );
            ub = _pow9 ( ub );
            uc = _pow9 ( uc );
        }
        return intersection ( Plane3d ( plane[i1].norm, plane[i1].dist + uv * ua ),
                              Plane3d ( plane[i2].norm, plane[i2].dist + uv * ub ),
                              Plane3d ( plane[i3].norm, plane[i3].dist + uv * uc ) );
    }
    // Метод "тяжёлого шарика
    p = getNearPoint2 ( plane );
    if ( ! p.isDef ) return p;
    nat i, j;
    double step = 0, d = 0;
    for ( i = 0; i < n; ++i )
    {
        const double s = fabs ( plane[i] % p );
        if ( step < s ) step = s;
        const double t = fabs ( plane[i].dist );
        if ( d < t ) d = t;
    }
    if ( step * 1e6 < d ) return p;
    const double eps = 1e-8 * step;
    Vector3d g0 ( 0, 0, 0 );
    for ( j = 0; j < 50; ++j )
    {
        Vector3d g ( 0, 0, 0 );
        for ( i = 0; i < n; ++i )
        {
            double t = plane[i] % p;
            switch ( k )
            {
            case 3:
                t = root3 ( t ); break;
            case 5:
                t = root5 ( t ); break;
            case 7:
                t = root7 ( t ); break;
            default:
                t = root9 ( t ); break;
            }
            g += t * plane[i].norm;
        }
        if ( ! g ) return p;
        Vector3d dir = 2 * g.setNorm2(-1) + g0;
        g0 = g;
        const NearPointFunc3d func ( plane, p, dir.setNorm2(), k );
        const double t = fmin ( 0, step, func, eps );
        p += dir * t;
        if ( t < eps ) break;
        step = 0.5 * ( step + t );
    }
    return p;
}

Def<Vector3d> getNearPoint4_3 ( CArrRef<Plane3d> plane )
{
    return getNearPoint ( 3, plane );
}

Def<Vector3d> getNearPoint6_5 ( CArrRef<Plane3d> plane )
{
    return getNearPoint ( 5, plane );
}

Def<Vector3d> getNearPoint8_7 ( CArrRef<Plane3d> plane )
{
    return getNearPoint ( 7, plane );
}

Def<Vector3d> getNearPoint10_9 ( CArrRef<Plane3d> plane )
{
    return getNearPoint ( 9, plane );
}

//************************ 29.01.2005 *******************************//
//
//      Вычисление ближайшей точки к заданным плоскостям
//      Минимум суммы квадратов расстояний ( 2-норма )
//
//************************ 05.09.2012 *******************************//

Def<Vector3d> getNearPoint2 ( CArrRef<Plane3d> plane )
{
    Def<Vector3d> res;
    const nat n = plane.size();
    if ( n < 3 ) return res;
    HMatrix<double> a ( n, 3 );
    CmbArray<double, 80> b ( n );
    for ( nat i = 0; i < n; ++i )
    {
        const Plane3d & pi = plane[i];
        b[i] = - pi.dist;
        double * row = a[i];
        row[0] = pi.norm.x;
        row[1] = pi.norm.y;
        row[2] = pi.norm.z;
    }
    FixArray<double, 3> c;
    if ( res.isDef = ortholin ( a, b, c ) )
    {
        res.x = c[0];
        res.y = c[1];
        res.z = c[2];
    }
    return res;
}

Def<Vector3d> getNearPoint2 ( CArrRef<Plane3d> plane, CArrRef<double> mass )
{
    Def<Vector3d> res;
    const nat n = plane.size();
    if ( n < 3 ) return res;
    HMatrix<double> a ( n, 3 );
    CmbArray<double, 80> b ( n );
    for ( nat i = 0; i < n; ++i )
    {
        const Plane3d & pi = plane[i];
        b[i] = - pi.dist * mass[i];
        double * row = a[i];
        row[0] = pi.norm.x * mass[i];
        row[1] = pi.norm.y * mass[i];
        row[2] = pi.norm.z * mass[i];
    }
    FixArray<double, 3> c;
    if ( res.isDef = ortholin ( a, b, c ) )
    {
        res.x = c[0];
        res.y = c[1];
        res.z = c[2];
    }
    return res;
}

//************************ 22.05.2010 *******************************//
//
//      Вычисление ближайшей точки к заданным плоскостям
//      Минимум максимального расстояния ( бесконечная норма )
//
//************************ 01.06.2011 *******************************//

Def<Vector3d> getNearPointU ( CArrRef<Plane3d> plane )
{
    const Def<Sphere3d> s = minSphere ( plane );
    return Def<Vector3d> ( s.o, s.isDef );
}

//************************* 03.12.2016 ******************************//
//
//    Робастный метод вычисления ближайшей точки к заданным плоскостям
//    Заполняет массив весов (mass) значениями от 0 до 1
//
//************************* 03.12.2016 ******************************//

template<> inline void approx2 ( CArrRef<Plane3d> arr, Def<Vector3d> & res )
{
    res = getNearPoint2 ( arr );
}

template<> inline void approx2 ( CArrRef<Plane3d> arr, ArrRef<double> mass, Def<Vector3d> & res )
{
    res = getNearPoint2 ( arr, mass );
}

Def<Vector3d> getNearPointR ( CArrRef<Plane3d> plane, ArrRef<double> mass )
{
    Def<Vector3d> res;
    approxR ( plane, mass, res );
    return res;
}

//************************ 03.02.2005 *******************************//
//
//      Вычисление ближайшей точки к заданным прямым
//      Минимум суммы квадратов расстояний ( 2-норма )
//
//************************ 05.09.2012 *******************************//

Def<Vector3d> getNearPoint2 ( CArrRef<Line3d> line )
{
    Def<Vector3d> res;
    const nat n = line.size();
    if ( n < 2 ) return res;
    HMatrix<double> a ( 3*n, 3 );
    CmbArray<double, 80> b ( 3*n );
    nat j = 0;
    for ( nat i = 0; i < n; ++i )
    {
        const Vector3d & d = line[i].dir;
        const Vector3d & p = line[i].point;
        double * s = a[j];
        s[0] =  d.y;
        s[1] = -d.x;
        s[2] = 0.;
        b[j] = p.x * d.y - p.y * d.x;
        ++j;
        s = a[j];
        s[0] =  d.z;
        s[1] = 0.;
        s[2] = -d.x;
        b[j] = p.x * d.z - p.z * d.x;
        ++j;
        s = a[j];
        s[0] = 0.;
        s[1] =  d.z;
        s[2] = -d.y;
        b[j] = p.y * d.z - p.z * d.y;
        ++j;
    }
    FixArray<double, 3> x;
    res.isDef = lss_h ( a, b, x );
    if ( res.isDef )
    {
        res.x = x[0];
        res.y = x[1];
        res.z = x[2];
    }
    return res;
}

//************************ 10.03.2012 *******************************//
//
//      Вычисление ближайшей точки к заданным прямым
//      Минимум максимального расстояния ( бесконечная норма )
//
//************************ 10.03.2012 *******************************//

Def<Vector3d> getNearPointU ( CArrRef<Line3d> line )
{
    const Def<Sphere3d> s = minSphere ( line );
    return Def<Vector3d> ( s.o, s.isDef );
}