
#include "tune.h"
#include "math.h"
#include "approx.h"
#include "LinAlg.h"
#include "Mathem.h"
#include "Matrix.h"
#include "func1t.h"
#include "func1d.h"
#include "func2d.h"
#include "opti1d.h"
#include "opti2d.h"
#include "moment2d.h"
#include "approx2d.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"
#include "ShevArray.h"
#include "intersect2d.h"
#include "WireModel.h"

//************************* 01.08.2006 ******************************//
//
//    Аппроксимация прямой набора из n двухмерных точек
//    Минимизируется абсолютный момент 1-го порядка относительно прямой
//
//************************* 01.02.2011 ******************************//

Def<Line2d> getLine1 ( CCArrRef<Vector2d> & point )
{
    Def<Line2d> res;
    const nat n = point.size();
    if ( n < 2 ) return res;
    double min = 1e300;
    nat j, i1 = 0, i2 = n;
    if ( n < 20 )
    {
        for( j = 0; j <= n; ++j )
        {
            const Vector2d & v1 = point[i1];
            nat i, im = n, k;
            for ( i = 1; i < n; ++i )
            {
                const Vector2d & vi = point[i];
                Vector2d d = vi - v1;
                if ( ! d ) continue;
                d.setNorm2();
                const Vector2d p = 0.5 * ( vi + v1 );
                const double s = d % p;
                double sum = 0.;
                for ( k = 0; k < n; ++k )
                {
                    sum += fabs ( d % point[k] - s );
                }
                if ( min > sum )
                {
                    min = sum;
                    im = i;
                    res.norm = d.leftPerpendicular();
                    res.dist = - ( res.norm * p );
                    res.isDef = true;
                }
            }
            if ( im == n || im == i2 ) break;
            i2 = i1;
            i1 = im;
        }
    }
    else
    {
        CmbArray<SortItem<double, nat>, 64> dp ( n );
        CmbArray<Vector2d, 64> p ( n );
        for ( j = 0; j <= n; ++j )
        {
            Vector2d sum ( 0., 0. );
            nat i;
            for ( i = 0; i < n; ++i )
            {
                dp[i].tail = i;
                if ( i == i1 )
                {
                    dp[i].head = 1.7e308;
                }
                else
                {
                    Vector2d & u = p[i] = point[i] - point[i1];
                    if ( u.y < 0 ) u = - u;
                    sum += u;
                    dp[i].head = u.y == 0 ? 1e308 : u.x / u.y;
                }
            }
            quickSort321 ( dp );
            nat k, im = n;
            for ( k = 1; k < n; ++k )
            {
                i = dp[k].tail;
                Vector2d & u = p[i];
                const double d = u * u;
                if ( ! d ) continue;
                sum -= u;
                sum -= u;
                double t = u % sum;
                t *= t;
                if ( min * d > t )
                {
                    min = t / d;
                    im = i;
                }
            }
            if ( im == n || im == i2 )
            {
                break;
            }
            res.norm = p[im].setNorm2().leftPerpendicular();
            res.dist = - 0.5 * ( res.norm * ( point[i1] + point[im] ) );
            res.isDef = true;
            i2 = i1;
            i1 = im;
        }
    }
    return res;
}

Def<Line2d> getLine1 ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass )
{
    Def<Line2d> res;
    const nat n = point.size();
    if ( n < 2 ) return res;
    double min = 1e300;
    nat j, i1 = 0, i2 = n;
    if ( n < 20 )
    {
        for( j = 0; j <= n; ++j )
        {
            const Vector2d & v1 = point[i1];
            nat i, im = n, k;
            for ( i = 1; i < n; ++i )
            {
                const Vector2d & vi = point[i];
                Vector2d d = vi - v1;
                if ( ! d ) continue;
                d.setNorm2();
                const Vector2d p = 0.5 * ( vi + v1 );
                const double s = d % p;
                double sum = 0.;
                for ( k = 0; k < n; ++k )
                {
                    sum += fabs ( d % point[k] - s ) * mass[k];
                }
                if ( min > sum )
                {
                    min = sum;
                    im = i;
                    res.norm = d.leftPerpendicular();
                    res.dist = - ( res.norm * p );
                    res.isDef = true;
                }
            }
            if ( im == n || im == i2 ) break;
            i2 = i1;
            i1 = im;
        }
    }
    else
    {
        CmbArray<SortItem<double, nat>, 64> dp ( n );
        CmbArray<Vector2d, 64> p ( n );
        for ( j = 0; j <= n; ++j )
        {
            Vector2d sum ( 0., 0. );
            nat i;
            for ( i = 0; i < n; ++i )
            {
                dp[i].tail = i;
                if ( i == i1 )
                {
                    dp[i].head = 1.7e308;
                }
                else
                {
                    Vector2d & u = p[i] = ( point[i] - point[i1] ) * mass[i];
                    if ( u.y < 0 ) u = - u;
                    sum += u;
                    dp[i].head = u.y == 0 ? 1e308 : u.x / u.y;
                }
            }
            quickSort321 ( dp );
            nat k, im = n;
            for ( k = 1; k < n; ++k )
            {
                i = dp[k].tail;
                Vector2d & u = p[i];
                const double d = u * u;
                if ( ! d ) continue;
                sum -= u;
                sum -= u;
                double t = u % sum;
                t *= t;
                if ( min * d > t )
                {
                    min = t / d;
                    im = i;
                }
            }
            if ( im == n || im == i2 )
            {
                break;
            }
            res.norm = p[im].setNorm2().leftPerpendicular();
            res.dist = - 0.5 * ( res.norm * ( point[i1] + point[im] ) );
            res.isDef = true;
            i2 = i1;
            i1 = im;
        }
    }
    return res;
}

//*******************************************************************//
//
//      Аппроксимация прямой набора из n двухмерных точек
//      Минимизируется момент 2-го порядка относительно прямой
//
//************************* 01.02.2011 ******************************//

Def<Line2d> getLine2 ( CCArrRef<Vector2d> & point )
{
    Def<Line2d> res;
    const Def<Mom2d> mom = momentum2pnt ( point );
    if ( ! mom.isDef ) return res;
    res.norm = mom.maxA;
    res.dist = - ( res.norm * mom.o );
    res.isDef = true;
    return res;
}

Def<Line2d> getLine2 ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass )
{
    Def<Line2d> res;
    const Def<Mom2d> mom = momentum2pnt ( point, mass );
    if ( ! mom.isDef ) return res;
    res.norm = mom.maxA;
    res.dist = - ( res.norm * mom.o );
    res.isDef = true;
    return res;
}

//************************* 30.12.2009 ******************************//
//
//      Аппроксимация прямой набора из n двухмерных точек.
//      Минимизируется максимальное расстояние от точек до прямой
//
//************************* 01.05.2019 ******************************//

Line2d getLineU ( CCArrRef<Vector2d> & point, double & r )
{
    r = 0;
    const nat n = point.size();
    if ( ! n ) return Line2d ( Vector2d ( 0, 1 ), 0 );
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
        return Line2d ( Vector2d ( 0, 1 ), -point[0].y );
    }
// Находим вторую начальную точку
    nat i1 = 0;
    max = qmod ( point[0] - point[i0] );
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[i] - point[i0] );
        if ( max < q ) max = q, i1 = i;
    }
    const double eps = 1e-9 * sqrt ( max );
// Находим третью начальную точку
    nat i2 = 0;
    Line2d line ( point[i1], point[i0] );
    r = fabs ( line % point[0] );
    for ( i = 1; i < n; ++i )
    {
        const double t = fabs ( line % point[i] );
        if ( r < t ) r = t, i2 = i;
    }
    if ( max < qmod ( point[i2] - point[i1] ) )
    {
        _swap ( i0, i2 );
        line = Line2d ( point[i1], point[i0] );
        r = fabs ( line % point[i2] );
    }
    if ( r < eps || i2 == i0 || i2 == i1 ) return line;
    r *= 0.5;
    const double t = line % point[i2];
    line.dist -= 0.5 * t;
// Помещаем найденные три точки в массив
    CmbSuite<Vector2d, 16> vert;
    vert.resize ( 3 );
    if ( t > 0 )
    {
        vert[0] = point[i0];
        vert[1] = point[i1];
        vert[2] = point[i2];
    }
    else
    {
        vert[0] = point[i1];
        vert[1] = point[i0];
        vert[2] = point[i2];
    }
    while ( vert.size() < n )
    {
// Находим самую удалённую точку от прямой
        nat im = 0;
        max = fabs ( line % point[0] );
        for ( i = 1; i < n; ++i )
        {
            const double t = fabs ( line % point[i] );
            if ( max < t ) max = t, im = i;
        }
        if ( max < r + eps ) break;
// Добавляем новую вершину в многоугольник
        const Vector2d & v = point[im];
        max = 0;
        Vector2d a = vert.las() - v;
        for ( i = 0; i < vert.size(); ++i )
        {
            const Vector2d b = vert[i] - v;
            const double t = b % a;
            if ( max < t ) max = t, im = i;
            a = b;
        }
        vert.inc();
        for ( i = vert.size() - 1; i > im; --i ) vert[i] = vert[i-1];
        vert[im] = v;
// Вычисляем оптимальную прямую для нового многоугольника
        r = 0.5 * minConvexPolygonDiameter ( vert, line.norm, i1, i2 );
        line.dist = -0.5 * ( line.norm * ( vert[i1] + vert[i2] ) );
    }
    return line;
}

//************************* 11.09.2015 ******************************//
//
//      Робастный метод аппроксимации набора точек прямой
//      Заполняет массив весов (mass) значениями от 0 до 1
//
//************************* 11.12.2019 ******************************//

template<> inline void approx2 ( CCArrRef<Vector2d> & arr, Def<Line2d> & res )
{
    res = getLine2 ( arr );
}

template<> inline void approx2 ( CCArrRef<Vector2d> & arr, ArrRef<double> & mass, Def<Line2d> & res )
{
    res = getLine2 ( arr, mass );
}

Def<Line2d> getLineR ( CCArrRef<Vector2d> & arr, ArrRef<double> & mass )
{
    Def<Line2d> res;
    approxR ( arr, mass, res );
    return res;
}

//************************* 06.12.2006 ******************************//
//
//           Аппроксимация прямой набора из n отрезков
//      Минимизируется момент 1-го порядка относительно прямой
//
//************************* 10.01.2011 ******************************//

Def<Line2d> getLine1 ( CCArrRef<Segment2d> & segm )
{
    Def<Line2d> res = getLine2 ( segm );
    if ( ! res.isDef ) return res;
    const Segment2d & s = segm[0];
    double ax = res.norm % s.a;
    double ay = res.norm * s.a;
    double bx = res.norm % s.b;
    double by = res.norm * s.b;
    double maxX, minX, maxY, minY;
    if ( ax < bx ) minX = ax, maxX = bx; else minX = bx, maxX = ax;
    if ( ay < by ) minY = ay, maxY = by; else minY = by, maxY = ay;
    nat i;
    for ( i = 1; i < segm.size(); ++i )
    {
        const Segment2d & s = segm[i];
        double ax = res.norm % s.a;
        double ay = res.norm * s.a;
        double bx = res.norm % s.b;
        double by = res.norm * s.b;
        _mina ( minX, _min ( ax, bx ) );
        _maxa ( maxX, _max ( ax, bx ) );
        _mina ( minY, _min ( ay, by ) );
        _maxa ( minY, _max ( ay, by ) );

    }
    const double eps = 1e-9 * ( maxX - minX );
    double stepD = ( maxY - minY ) / 8;
    if ( stepD < eps ) return res;
    double stepA = atan2 ( maxY - minY, maxX - minX ) / 8;
    for (;;)
    {
        double sum2 = 0.;
        Vector2d gra1 (0,0);
        for ( i = 0; i < segm.size(); ++i )
        {
            const Segment2d & s = segm[i];
        }
        if ( stepD < eps )
        {
            break;
        }
        stepD *= 0.5;
    }
    return res;
}

//************************* 06.12.2006 ******************************//
//
//           Аппроксимация прямой набора из n отрезков
//      Минимизируется момент 2-го порядка относительно прямой
//
//************************* 10.01.2011 ******************************//

Def<Line2d> getLine2 ( CCArrRef<Segment2d> & segm )
{
    Def<Line2d> res;
    const Def<Mom2d> mom = momentum2sgm ( segm );
    if ( ! mom.isDef ) return res;
    res.norm = mom.maxA;
    res.dist = - ( res.norm * mom.o );
    res.isDef = true;
    return res;
}

//*******************************************************************//
//
//            Аппроксимация точек окружностью
//              Sum ( Ri^2 - R^2 )^2 -> min
//
//************************ 05.09.2012 *******************************//

Def<Circle2d> getCirclePnt22 ( CArrRef<Vector2d> p )
{
    Def<Circle2d> res;
    const nat n = p.size();
    if ( n < 3 ) return res;
    HMatrix<double> a ( n, 2 );
    CmbArray<double, 80> b ( n );
    double x = 0.;
    double y = 0.;
    double q = 0.;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        const double & xi = p[i].x;
        const double & yi = p[i].y;
        x += xi;
        y += yi;
        q += ( b[i] = xi * xi + yi * yi );
    }
    const double d = 1. / n;
    x *= d;
    y *= d;
    q *= d;
    for ( i = 0; i < n; ++i )
    {
        a[i][0] = p[i].x - x;
        a[i][1] = p[i].y - y;
        b[i] -= q;
    }
    FixArray<double, 2> c;
    if ( ortholin ( a, b, c ) )
    {
        res.o.x = 0.5 * c[0];
        res.o.y = 0.5 * c[1];
        res.r = sqrt ( res.o * res.o + q - 2. * ( res.o.x * x + res.o.y * y ) );
        res.isDef = true;
    }
    return res;
}

//************************ 02.05.2010 *******************************//
//
//            Аппроксимация точек окружностью
//               Sum ( Ri - R )^2 -> min
//
//************************ 05.09.2012 *******************************//

Def<Circle2d> getCirclePnt2 ( CArrRef<Vector2d> p )
{
    Def<Circle2d> res = getCirclePnt22 ( p );
    if ( ! res.isDef || res.r == 0 ) return res;
    const nat n = p.size();
    HMatrix<double> a ( n, 3 );
    Suite<double> stor;
    CmbArray<double, 80> b ( n );
    for ( nat j = 0; j < 15; ++j )
    {
        const Vector2d o = res.o;
        for ( nat i = 0; i < n; ++i )
        {
            const Vector2d v = ( o - p[i] ).setNorm2();
            a[i][0] = v.x;
            a[i][1] = v.y;
            a[i][2] = -1;
            b[i] = v * p[i];
        }
        FixArray<double, 3> c;
        if ( ! ortholin ( a, b, c, stor ) ) return Circle2d();
        res.o.x = c[0];
        res.o.y = c[1];
        res.r   = c[2];
        if ( qmod ( res.o - o ) < 1e-16 * res.r * res.r ) break;
    }
    return res;
}

//************************ 28.11.2009 *******************************//
//
//            Аппроксимация точек окружностью
//                Sum | Ri - R | -> min
//      ix1, ix2, ix3 - индексы точек задающие окружность
//
//************************ 06.10.2011 *******************************//

Def<Circle2d> getCirclePnt1 ( CArrRef<Vector2d> point, nat & ix1, nat & ix2, nat & ix3 )
{
    Def<Circle2d> res;
    nat np = point.size();
    if ( np < 3 ) return res;
    double min = 1e300;
    nat n1 = np - 1;
    nat n2 = np - 2;
    nat i0, i1, i2, i3;
    for ( i2 = 0; i2 < n2; ++i2 )
    {
        for ( i1 = i2+1; i1 < n1; ++i1 )
        {
            for ( i0 = i1+1; i0 < np; ++i0 )
            {
                const Def<Circle2d> c = circlePPP ( point[i0], point[i1], point[i2] );
                if ( ! c.isDef ) continue;
                double sum = 0.;
                for ( i3 = 0; i3 < np; ++i3 ) sum += fabs ( norm2 ( point[i3] - c.o ) - c.r );
                if ( sum < min ) 
                    min = sum, res = c, ix1 = i2, ix2 = i1, ix3 = i0;
            }
        }
    }
    return res;
}

Def<Circle2d> getCirclePnt1 ( CArrRef<Vector2d> p )
{
    nat ix1, ix2, ix3;
    return getCirclePnt1 ( p, ix1, ix2, ix3 );
}

//************************ 20.09.2009 *******************************//
//
//             Аппроксимация многоугольника кругом
//
//************************ 25.09.2010 *******************************//

Def<Circle2d> getCirclePlg ( CArrRef<Vector2d> poly )
{
    Def<Circle2d> res;
    if ( poly.size() == 0 ) return res;
    res.r = sqrt ( fabs ( area ( poly ) ) / M_PI );
    res.o = centerPlg ( poly );
    res.isDef = true;
    return res;
}

//************************ 11.11.2008 *******************************//
//
//             Аппроксимация многоугольника эллипсом
//
//************************ 26.09.2010 *******************************//

Def<Ellipse2d> getEllipsePlg ( CArrRef<Vector2d> poly )
{
    return getEllipse ( momentum2plg ( poly ) );
}

//************************ 25.09.2009 *******************************//
//
//             Аппроксимация многоугольника прямоугольником
//
//************************ 26.09.2010 *******************************//

Def<Rectangle2d> getRectanglePlg ( CArrRef<Vector2d> poly )
{
    return getRectangle ( momentum2plg ( poly ) );
}

//************************ 25.02.2023 *******************************//
//
//        Совмещение группы точек с выпуклым многоугольником
//                  при помощи сдвига, без вращения
//
//************************ 25.02.2023 *******************************//

Def<Vector2d> minMaxPointsConvexPolygonNR ( CCArrRef<Vector2d> & point, CCArrRef<Vector2d> & vert )
{
    Def<Vector2d> err;
    if ( point.size() < 1 || vert.size() < 3 ) return err;
// Получение границ многоугольника в виде прямых линий
    DynArray<Line2d> line ( vert.size() );
    if ( ! points2lines ( vert, line ) ) return err;
// Найдём габариты множества точек
    const Segment2d & seg1 = dimensions ( point );
    const Segment2d & seg2 = dimensions ( vert );
    const double dx = _max ( seg1.b.x, seg2.b.x ) - _min ( seg1.a.x, seg2.a.x );
    const double dy = _max ( seg1.b.y, seg2.b.y ) - _min ( seg1.a.y, seg2.a.y );
// Поиск оптимального сдвига
    Vector3d arr[4];
    arr[0] = Vector3d ( dx, dy, dx + dy );
    arr[1] = Vector3d ( -1., 0., 0. );
    arr[2] = Vector3d ( 0., -1., 0. );
    arr[3] = Vector3d ( 0., 0., -1. );
    const double eps = 1e-6 * arr[0].z;
    for ( nat k = 0; k < 100; ++k )
    {
        Vector3d & a0 = arr[0];
        const Vector2d o ( a0.x, a0.y );
        double max = -1;
        nat i, im;
        for ( i = 0; i < line.size(); ++i )
        {
            const Line2d & li = line[i];
            double pmax = li.norm * point[0];
            for ( nat j = 1; j < point.size(); ++j )
            {
                _maxa ( pmax, li.norm * point[j] );
            }
            pmax += li % o;
            if ( _maxa ( max, pmax ) ) im = i;
        }
        const double dist = max + a0.z;
        if ( dist < eps ) return o;
        const Line2d & li = line[im];
        const Vector3d cor ( li.norm.x, li.norm.y, 1. );
        nat ib = 0;
        double sg;
        for ( i = 1; i < 4; ++i )
        {
            const Vector3d & v = arr[i];
            double t = cor * v;
            if ( t > -1e-8 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.z * t;
                ib = i;
                sg = t;
            }
            else
            {
                const double s = v.z * t;
                if ( s < max ) max = s, ib = i, sg = t;
            }
        }
        if ( ib == 0 )
            return err;
        const Vector3d & v = arr[ib];
        a0 -= v * ( dist * sg );
        for ( i = 1; i < 4; ++i )
        {
            if ( i == ib ) continue;
            Vector3d & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sg );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return err;
}

//************************ 22.04.2023 *******************************//
//
//             Совмещение двух выпуклых многоугольников
//                  при помощи сдвига, без вращения
//
//************************ 22.04.2023 *******************************//

Def<Vector2d> overlayConvexPolygonsNR ( CCArrRef<Vector2d> & vert1, CCArrRef<Vector2d> & vert2 )
{
    nat i;
    Def<Vector2d> err;
    const nat nv1 = vert1.size();
    const nat nv2 = vert2.size();
    if ( nv1 < 3 || nv2 < 3 ) return err;
// Вспомогательные данные
    DynArray<Line2d> line1 ( nv1 );
    if ( ! points2lines ( vert1, line1 ) ) return err;
    DynArray<Line2d> line2 ( nv2 );
    if ( ! points2lines ( vert2, line2 ) ) return err;
    DynArray<double> pd1 ( nv1 );
    for ( i = 0; i < nv1; ++i )
    {
        const Line2d & li = line1[i];
        double max = li.norm * vert2[0];
        for ( nat j = 1; j < nv2; ++j ) _maxa ( max, li.norm * vert2[j] );
        pd1[i] = max + li.dist;
    }
    DynArray<double> pd2 ( nv2 );
    for ( i = 0; i < nv2; ++i )
    {
        const Line2d & li = line2[i];
        double max = li.norm * vert1[0];
        for ( nat j = 1; j < nv1; ++j ) _maxa ( max, li.norm * vert1[j] );
        pd2[i] = max + li.dist;
    }
// Найдём габариты множества точек
    const Segment2d seg1 = dimensions ( vert1 );
    const Segment2d seg2 = dimensions ( vert2 );
    const double dx = _max ( seg1.b.x, seg2.b.x ) - _min ( seg1.a.x, seg2.a.x );
    const double dy = _max ( seg1.b.y, seg2.b.y ) - _min ( seg1.a.y, seg2.a.y );
    const double dd = dx + dy;
// Поиск оптимального сдвига
    Double<4> arr[5];
    arr[0].init ( dx, dy, dd, dd );
    arr[1].init ( -1., 0., 0., 0. );
    arr[2].init ( 0., -1., 0., 0. );
    arr[3].init ( 0., 0., -1., 0. );
    arr[4].init ( 0., 0., 0., -1. );
    const double eps = 1e-6 * dd;
    for ( nat k = 0; k < 100; ++k )
    {
        Double<4> & a0 = arr[0];
        const Vector2d o ( a0.d0, a0.d1 );
        double max1 = -1;
        nat im1, im2;
        for ( i = 0; i < nv2; ++i )
        {
            if ( _maxa ( max1, pd2[i] + line2[i].norm * o ) ) im1 = i;
        }
        max1 += a0.d2;
        double max2 = -1;
        for ( i = 0; i < nv1; ++i )
        {
            if ( _maxa ( max2, pd1[i] - line1[i].norm * o ) ) im2 = i;
        }
        max2 += a0.d3;
        double dist;
        Double<4> cor;
        if ( max1 > max2 )
        {
            dist = max1;
            const Vector2d & norm = line2[im1].norm;
            cor.init ( norm.x, norm.y, 1, 0 );
        }
        else
        {
            dist = max2;
            const Vector2d & norm = line1[im2].norm;
            cor.init ( -norm.x, -norm.y, 0, 1 );
        }
        if ( dist < eps ) return o;
        nat ib = 0;
        double sg, max;
        for ( i = 1; i < 5; ++i )
        {
            const Double<4> & v = arr[i];
            double t = cor * v;
            if ( t > -1e-8 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = ( v.d2 + v.d3 ) * t;
                ib = i;
                sg = t;
            }
            else
            {
                const double s = ( v.d2 + v.d3 ) * t;
                if ( s < max ) max = s, ib = i, sg = t;
            }
        }
        if ( ib == 0 )
            return err;
        const Double<4> & v = arr[ib];
        a0 -= v * ( dist * sg );
        for ( i = 1; i < 5; ++i )
        {
            if ( i == ib ) continue;
            Double<4> & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sg );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return err;
}

//************************ 23.04.2023 *******************************//
//
//        Совмещение группы точек с выпуклым многоугольником
//                  при помощи вращения и сдвига
//
//************************ 23.04.2023 *******************************//

Def<Vector4d> minMaxPointsConvexPolygon ( const double a, const double b, CCArrRef<Vector2d> & point, CCArrRef<Line2d> & line ) 
{
    Vector4d arr[5];
    arr[0] = Vector4d ( 1, 1, 1, 1 );
    arr[1] = Vector4d ( -1., 0., 0., 0. );
    arr[2] = Vector4d ( 0., -1., 0., 0. );
    arr[3] = Vector4d ( 0., 0., -1., 0. );
    arr[4] = Vector4d ( 0., 0., 0., -1. );
    for ( nat k = 0; k < 100; ++k )
    {
        Vector4d & a0 = arr[0];
        const Vector2d o ( a0.x1, a0.x2 );
        const Vector2d w ( a + b * a0.x3, b - a * a0.x3 );
        double max = -1;
        nat i, im, km;
        for ( i = 0; i < line.size(); ++i )
        {
            const Line2d & li = line[i];
            const Vector2d u ( w * li.norm, w % li.norm );
            nat jm = 0;
            double pmax = u * point[0];
            for ( nat j = 1; j < point.size(); ++j )
            {
                const double p = u * point[j];
                if ( pmax < p ) pmax = p, jm = j;
            }
            pmax += li % o;
            if ( max < pmax ) max = pmax, im = i, km = jm;
        }
        const double dist = max + a0.x4;
        if ( dist < 1e-4 )
            return a0;
        const Line2d & li = line[im];
        const Vector2d & vm = point[km];
        const Vector4d cor ( li.norm.x, li.norm.y, li.norm * vm * b + li.norm % vm * a, 1. );
        nat ib = 0;
        double sg;
        for ( i = 1; i <= 4; ++i )
        {
            const Vector4d & v = arr[i];
            double t = cor * v;
            if ( t > -1e-8 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.x4 * t;
                ib = i;
                sg = t;
            }
            else
            {
                const double s = v.x4 * t;
                if ( s < max ) max = s, ib = i, sg = t;
            }
        }
        if ( ib == 0 )
        {
            return Def<Vector4d>();
        }
        const Vector4d & v = arr[ib];
        a0 -= v * ( dist * sg );
        for ( i = 1; i <= 4; ++i )
        {
            if ( i == ib ) continue;
            Vector4d & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sg );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return Def<Vector4d>();
}

Def<Conform2d> minMaxPointsConvexPolygon ( CCArrRef<Vector2d> & point, CCArrRef<Vector2d> & vert )
{
    nat i;
    Def<Conform2d> res;
// Приведём многоугольники к стандартному виду
    const Def<Vector2d> o1 = centerPlg ( point );
    if ( ! o1.isDef ) return res;
    const Def<Vector2d> o2 = centerPlg ( vert );
    if ( ! o2.isDef ) return res;
    DynArray<Vector2d> poly1 ( point.size() ), poly2 ( vert.size() );
    for ( i = 0; i < point.size(); ++i ) poly1[i] = point[i] - o1;
    for ( i = 0; i < vert.size(); ++i ) poly2[i] = vert[i] - o2;
    double d = norm2 ( poly2[0] );
    for ( i = 1; i < poly2.size(); ++i )
    {
        double t = norm2 ( poly2[i] );
        if ( d < t ) d = t;
    }
    if ( ! d ) return res;
    const double c = 1 / d;
    const Conform2d magn ( Spin2d(), null2d, c );
    const Conform2d conf1 = magn * Conform2d ( Spin2d(), -o1 );
    const Conform2d conf2 = magn * Conform2d ( Spin2d(), -o2 );
    poly1 *= c;
    poly2 *= c;
// Получение границ многоугольника в виде прямых линий
    DynArray<Line2d> line ( poly2.size() );
    if ( ! points2lines ( poly2, line ) ) return res;
// Выбор лучшего решения из n вариантов
    const nat n = 40;
    const double step = M_2PI / n;
    double max = -1e9;
    nat im;
    for ( i = 0; i < n; ++i )
    {
        const double a = i * step;
        const double cosa = cos(a);
        const double sina = sin(a);
        Def<Vector4d> d = minMaxPointsConvexPolygon ( cosa, sina, poly1, line );
        if ( d.isDef && max < d.x4 )
        {
            max = d.x4;
            res.spin = Spin2d ( atan2 ( sina - cosa * d.x3, cosa + sina * d.x3 ) );
            res.trans.x = d.x1;
            res.trans.y = d.x2;
            im = i;
        }
    }
    if ( max > -1e9 ) res = ~conf2 * res * conf1;
    return res;
}

//************************ 27.09.2021 *******************************//
//
//             Совмещение двух выпуклых многоугольников
//                  при помощи сдвига и вращения
//
//************************ 27.09.2021 *******************************//

Def<Conform2d> overlayConvexPolygons ( CCArrRef<Vector2d> & vert1, CCArrRef<Vector2d> & vert2 )
{
    Def<Conform2d> res;
    if ( vert1.size() < 3 || vert2.size() < 3 ) return res;
// Сделаем так, чтобы вершины меньшего по площади многоугольника выходили
// за пределы большего многоугольника минимальным образом.
    const double area1 = area ( vert1 );
    const double area2 = area ( vert2 );
    if ( area1 < area2 )
    {
        if ( area2 <= 0 ) return res;
        res = minMaxPointsConvexPolygon ( vert1, vert2 );
    }
    else
    {
        if ( area1 <= 0 ) return res;
        res = minMaxPointsConvexPolygon ( vert2, vert1 );
        if ( res.isDef ) res = ~res;
    }
    return res;
}

//************************ 07.02.2022 *******************************//
//
//        Совмещение группы точек с выпуклым многоугольником
//          при помощи преобразования сохраняющего площадь
//
//************************ 07.02.2022 *******************************//

Def<Affin2d> overlayPointsOnConvexPolygon ( CCArrRef<Vector2d> & point, CCArrRef<Line2d> & line )
{
    Def<Affin2d> res;
    const nat nl = line.size();
    if ( point.size() < 3 || nl < 3 ) return res;
// Инициализация области допустимых преобразований
    List< Vertex<7> > stor;
    WireModel<7> model;
    model.simplex ( 4*(7+1), stor );
    Double<7> dn;
    dn.fill ( 2 );
    model.vlist -= dn;
// Поиск оптимального преобразования
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        Double<7> best;
        best.d6 = -1e9;
        Show< Vertex<7> > show ( model.vlist );
        if ( show.top() )
        do
        {
            const Vertex<7> * p = show.cur();
            const Double<7> & pc = p->coor;
            const double c = pc.d0 * pc.d4 - pc.d1 * pc.d3 - 1;
            for ( nat k = 0; k < 7; ++k )
            {
                const Vertex<7> * v = p->vertex[k];
                if ( v < p ) continue;
                const Double<7> & vc = v->coor;
                const double a0 = vc.d0 - pc.d0;
                const double a1 = vc.d1 - pc.d1;
                const double a3 = vc.d3 - pc.d3;
                const double a4 = vc.d4 - pc.d4;
                const double a = a0 * a4 - a1 * a3;
                const double b = a0 * pc.d4 + a4 * pc.d0 - a1 * pc.d3 - a3 * pc.d1;
                double r[2];
                const nat n = root2 ( a, b, c, r );
                for ( nat j = 0; j < n; ++j )
                {
                    const double t = r[j];
                    if ( t >= 0 && t <= 1 )
                    {
                        const double h = pc.d6 + t * ( vc.d6 - pc.d6 );
                        if ( best.d6 < h )
                        {
                            best.d6 = h;
                            best.d0 = pc.d0 + t * a0;
                            best.d1 = pc.d1 + t * a1;
                            best.d2 = pc.d2 + t * ( vc.d2 - pc.d2 );
                            best.d3 = pc.d3 + t * a3;
                            best.d4 = pc.d4 + t * a4;
                            best.d5 = pc.d5 + t * ( vc.d5 - pc.d5 );
                        }
                    }
                }
            }
        }
        while ( show.next() );
// Поиск максимального нарушения ограничений для выбранного решения
        nat km = nl;
        Vector2d pm;
        double max = 0.;
        for ( nat j = 0; j < point.size(); ++j )
        {
            const Vector2d & p = point[j];
            const Vector2d p1 ( best.d0*p.x + best.d1*p.y + best.d2,
                                best.d3*p.x + best.d4*p.y + best.d5 );
            for ( nat k = 0; k < nl; ++k )
            {
                const double t = line[k] % p1;
                if ( km == nl || max < t ) max = t, pm = p, km = k;
            }
        }
        max += best.d6;
// Если нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            const Affin2d at ( Vector2d ( best.d0, best.d1 ), 
                               Vector2d ( best.d3, best.d4 ),
                               Vector2d ( best.d2, best.d5 ) );
            return at;
        }
// Применение ограничения к области допустимых преобразований
        const double nx = line[km].norm.x;
        const double ny = line[km].norm.y;
        Double<8> plane;
        plane.d0 = nx * pm.x;
        plane.d1 = nx * pm.y;
        plane.d2 = nx;
        plane.d3 = ny * pm.x;
        plane.d4 = ny * pm.y;
        plane.d5 = ny;
        plane.d6 = 1;
        plane.d7 = line[km].dist;
        model.cut ( plane, stor );
    }
    return res;
}

//************************* 13.07.2005 ******************************//
//
//      Вычисление ближайшей точки к заданным прямым
//      Минимум суммы абсолютных расстояний ( 1-норма )
//
//************************ 05.09.2012 *******************************//

Def<Vector2d> getNearPoint1 ( CArrRef<Line2d> line, nat & ix1, nat & ix2 )
{
    Def<Vector2d> p;
    const nat n = line.size();
    if ( n < 2 ) return p;
    CmbArray<bool, 80> used ( n );
    double max = 0;
    nat i, ii;
    for ( i = 0; i < n; ++i )
    {
        used[i] = false;
        const Vector2d & v = line[i].norm;
        const double ax = fabs ( v.x );
        if ( max < ax ) max = ax, ii = i;
        const double ay = fabs ( v.y );
        if ( max < ay ) max = ay, ii = i;
    }
    if ( max == 0 ) return p;
    if ( n <= 18 )
    {
        double min = 1e300;
        for(;;)
        {
            used[ii] = true;
            const Line2d & l0 = line[ii];
            nat im = n;
            for ( nat i = 0; i < n; ++i )
            {
                const Line2d & li = line[i];
                const double d = li.norm % l0.norm;
                if ( d == 0 ) continue;
                const Vector2d x ( ( li.norm.y * l0.dist - l0.norm.y * li.dist ) / d, 
                                   ( l0.norm.x * li.dist - li.norm.x * l0.dist ) / d );
                double sum = 0.;
                for ( nat l = 0; l < n; ++l )
                {
                    sum += fabs ( line[l] % x );
                }
                if ( min > sum )
                {
                    min = sum;
                    im = i;
                    p = x;
                    ix1 = ii;
                    ix2 = i;
                }
            }
            if ( im == n || used[im] ) break;
            ii = im;
        }
        return p;
    }
    CmbArray<double, 80> buf ( n + n );
    ArrRef<double> d ( buf, 0, n );
    ArrRef<double> w ( buf, n, n );
    for(;;)
    {
        used[ii] = true;
        const Line2d & ll = line[ii];
        const bool isX = fabs ( ll.norm.x ) > fabs ( ll.norm.y );
        if ( isX )
        {
            const double cy = ll.norm.y / ll.norm.x;
            const double cd = ll.dist   / ll.norm.x;
            for ( nat i = 0; i < n; ++i )
            {
                if ( i == ii )
                {
                    w[i] = 0;
                    continue;
                }
                const Line2d & li = line[i];
                const double a = li.norm.y - li.norm.x * cy;
                w[i] = fabs ( a );
                if ( a != 0 )
                {
                    d[i] = ( li.norm.x * cd - li.dist ) / a;
                }
            }
        }
        else
        {
            const double cx = ll.norm.x / ll.norm.y;
            const double cd = ll.dist   / ll.norm.y;
            for ( nat i = 0; i < n; ++i )
            {
                if ( i == ii )
                {
                    w[i] = 0;
                    continue;
                }
                const Line2d & li = line[i];
                const double a = li.norm.x - li.norm.y * cx;
                w[i] = fabs ( a );
                if ( a != 0 )
                {
                    d[i] = ( li.norm.y * cd - li.dist ) / a;
                }
            }
        }
        Def<nat> im = selectR ( d, w, 0.5 );
        if ( ! im.isDef ) return p;
        if ( used[im] )
        {
            const double di = d[im];
            p = isX ? Vector2d ( - ( ll.norm.y * di + ll.dist ) / ll.norm.x, di ) :
                      Vector2d ( di, - ( ll.norm.x * di + ll.dist ) / ll.norm.y );
            ix1 = ii;
            ix2 = im.a;
            break;
        }
        ii = im.a;
    }
    return p;
}

Def<Vector2d> getNearPoint1 ( CArrRef<Line2d> line )
{
    nat ix1, ix2;
    return getNearPoint1 ( line, ix1, ix2 );
}

//************************ 30.06.2013 *******************************//
//
//      Вычисление ближайшей точки к заданным прямым
//      Минимум p-нормы ( p = 4/3, 6/5, 8/7, 10/9 )
//
//************************ 30.06.2013 *******************************//

class NearPointFunc2d : public MathFunc1
{
    CArrRef<Line2d> line;
    const Vector2d o, g;
    const nat k;
public:
    NearPointFunc2d ( CArrRef<Line2d> p1, const Vector2d & p2, const Vector2d & p3, nat p4 ) :
      line(p1), o(p2), g(p3), k(p4) {}
    virtual double operator () ( double t ) const;
};

double NearPointFunc2d::operator () ( double t ) const
{
    double sum = 0;
    const Vector2d p = o + g * t;
    for ( nat i = 0; i < line.size(); ++i )
    {
        const double t = line[i] % p;
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

class NearPointFunc4 : public MathFunc1
{
public:
    Vector2d v, a, b;
    nat k;

    virtual double operator () ( double t ) const;
};

double NearPointFunc4::operator () ( double t ) const
{
    Vector2d u ( v.x * ( 1 - fabs ( t ) ) + t * v.y, 
                 v.y * ( 1 - fabs ( t ) ) - t * v.x );
    switch ( k )
    {
    case 3:
        u /= root4 ( _pow4 ( u.x ) + _pow4 ( u.y ) + _pow4 ( a * u ) + _pow4 ( b * u ) );
        break;
    case 5:
        u /= root6 ( _pow6 ( u.x ) + _pow6 ( u.y ) + _pow6 ( a * u ) + _pow6 ( b * u ) );
        break;
    case 7:
        u /= root8 ( _pow8 ( u.x ) + _pow8 ( u.y ) + _pow8 ( a * u ) + _pow8 ( b * u ) );
        break;
    default:
        u /= root10 ( _pow10 ( u.x ) + _pow10 ( u.y ) + _pow10 ( a * u ) + _pow10 ( b * u ) );
    }
    return u * v;
}

Def<Vector2d> getNearPoint ( nat k, CArrRef<Line2d> line )
{
    Def<Vector2d> p;
    const nat n = line.size();
    if ( n < 2 ) return p;
    if ( n == 2 ) return intersection ( line[0], line[1] );
    if ( n == 3 )
    {
        SLU2<double> slu;
        const double & x0 = line[0].norm.x;
        const double & x1 = line[1].norm.x;
        const double & x2 = line[2].norm.x;
        const double & y0 = line[0].norm.y;
        const double & y1 = line[1].norm.y;
        const double & y2 = line[2].norm.y;
        const double & d0 = line[0].dist;
        const double & d1 = line[1].dist;
        const double & d2 = line[2].dist;
        double a0 = x0;
        double a1 = x1;
        double a2 = x2;
        double b0 = y0;
        double b1 = y1;
        double b2 = y2;
        if ( fabs ( a0 ) >= fabs ( a1 ) && fabs ( a0 ) >= fabs ( a2 ) )
        {
            if ( !a0 ) return p;
            b1 -= b0 * ( a1 /= a0 );
            b2 -= b0 * ( a2 /= a0 );
            if ( fabs ( b1 ) >= fabs ( b2 ) )
            {
                if ( !b1 ) return p;
                a2 -= a1 * ( b2 /= b1 );
                switch ( k )
                {
                case 3:
                    a2 = _pow3 ( a2 );
                    b2 = _pow3 ( b2 ); break;
                case 5:
                    a2 = _pow5 ( a2 );
                    b2 = _pow5 ( b2 ); break;
                case 7:
                    a2 = _pow7 ( a2 );
                    b2 = _pow7 ( b2 ); break;
                default:
                    a2 = _pow9 ( a2 );
                    b2 = _pow9 ( b2 );
                }
                slu.aa = x0 + a2 * x2;
                slu.ab = y0 + a2 * y2;
                slu.ac = - ( d0 + a2 * d2 );
                slu.ba = x1 + b2 * x2;
                slu.bb = y1 + b2 * y2;
                slu.bc = - ( d1 + b2 * d2 );
            }
            else
            {
                a1 -= a2 * ( b1 /= b2 );
                switch ( k )
                {
                case 3:
                    a1 = _pow3 ( a1 );
                    b1 = _pow3 ( b1 ); break;
                case 5:
                    a1 = _pow5 ( a1 );
                    b1 = _pow5 ( b1 ); break;
                case 7:
                    a1 = _pow7 ( a1 );
                    b1 = _pow7 ( b1 ); break;
                default:
                    a1 = _pow9 ( a1 );
                    b1 = _pow9 ( b1 );
                }
                slu.aa = x0 + a1 * x1;
                slu.ab = y0 + a1 * y1;
                slu.ac = - ( d0 + a1 * d1 );
                slu.ba = x2 + b1 * x1;
                slu.bb = y2 + b1 * y1;
                slu.bc = - ( d2 + b1 * d1 );
            }
        }
        else
        if ( fabs ( a1 ) >= fabs ( a2 ) )
        {
            b0 -= b1 * ( a0 /= a1 );
            b2 -= b1 * ( a2 /= a1 );
            if ( fabs ( b0 ) >= fabs ( b2 ) )
            {
                if ( !b0 ) return p;
                a2 -= a0 * ( b2 /= b0 );
                switch ( k )
                {
                case 3:
                    a2 = _pow3 ( a2 );
                    b2 = _pow3 ( b2 ); break;
                case 5:
                    a2 = _pow5 ( a2 );
                    b2 = _pow5 ( b2 ); break;
                case 7:
                    a2 = _pow7 ( a2 );
                    b2 = _pow7 ( b2 ); break;
                default:
                    a2 = _pow9 ( a2 );
                    b2 = _pow9 ( b2 );
                }
                slu.aa = x1 + a2 * x2;
                slu.ab = y1 + a2 * y2;
                slu.ac = - ( d1 + a2 * d2 );
                slu.ba = x0 + b2 * x2;
                slu.bb = y0 + b2 * y2;
                slu.bc = - ( d0 + b2 * d2 );
            }
            else
            {
                a0 -= a2 * ( b0 /= b2 );
                switch ( k )
                {
                case 3:
                    a0 = _pow3 ( a0 );
                    b0 = _pow3 ( b0 ); break;
                case 5:
                    a0 = _pow5 ( a0 );
                    b0 = _pow5 ( b0 ); break;
                case 7:
                    a0 = _pow7 ( a0 );
                    b0 = _pow7 ( b0 ); break;
                default:
                    a0 = _pow9 ( a0 );
                    b0 = _pow9 ( b0 );
                }
                slu.aa = x1 + a0 * x0;
                slu.ab = y1 + a0 * y0;
                slu.ac = - ( d1 + a0 * d0 );
                slu.ba = x2 + b0 * x0;
                slu.bb = y2 + b0 * y0;
                slu.bc = - ( d2 + b0 * d0 );
            }
        }
        else
        {
            b0 -= b2 * ( a0 /= a2 );
            b1 -= b2 * ( a1 /= a2 );
            if ( fabs ( b1 ) >= fabs ( b0 ) )
            {
                if ( !b1 ) return p;
                a0 -= a1 * ( b0 /= b1 );
                switch ( k )
                {
                case 3:
                    a0 = _pow3 ( a0 );
                    b0 = _pow3 ( b0 ); break;
                case 5:
                    a0 = _pow5 ( a0 );
                    b0 = _pow5 ( b0 ); break;
                case 7:
                    a0 = _pow7 ( a0 );
                    b0 = _pow7 ( b0 ); break;
                default:
                    a0 = _pow9 ( a0 );
                    b0 = _pow9 ( b0 );
                }
                slu.aa = x2 + a0 * x0;
                slu.ab = y2 + a0 * y0;
                slu.ac = - ( d2 + a0 * d0 );
                slu.ba = x1 + b0 * x0;
                slu.bb = y1 + b0 * y0;
                slu.bc = - ( d1 + b0 * d0 );
            }
            else
            {
                a1 -= a0 * ( b1 /= b0 );
                switch ( k )
                {
                case 3:
                    a1 = _pow3 ( a1 );
                    b1 = _pow3 ( b1 ); break;
                case 5:
                    a1 = _pow5 ( a1 );
                    b1 = _pow5 ( b1 ); break;
                case 7:
                    a1 = _pow7 ( a1 );
                    b1 = _pow7 ( b1 ); break;
                default:
                    a1 = _pow9 ( a1 );
                    b1 = _pow9 ( b1 );
                }
                slu.aa = x2 + a1 * x1;
                slu.ab = y2 + a1 * y1;
                slu.ac = - ( d2 + a1 * d1 );
                slu.ba = x0 + b1 * x1;
                slu.bb = y0 + b1 * y1;
                slu.bc = - ( d0 + b1 * d1 );
            }
        }
        p.isDef = slu.gauss ( p.x, p.y );
        return p;
    }
    if ( n == 4 )
    {
        nat i, i1, i2, i3, i4;
        SMatrix<double, 4, 3> A;
        double max = 0;
        for ( i = 0; i < 4; ++i )
        {
            const Line2d & li = line[i];
            A[i][0] = li.norm.x;
            A[i][1] = li.norm.y;
            A[i][2] = li.dist;
            const double t = fabs ( li.norm.x );
            if ( max < t ) max = t, i1 = i;
        }
        if ( max < 1e-9 ) return p;
        max = 0;
        for ( i = 0; i < 4; ++i )
        {
            if ( i == i1 ) continue;
            A[i][0] /= A[i1][0];
            A[i][1] -= A[i1][1] * A[i][0];
            A[i][2] -= A[i1][2] * A[i][0];
            const double t = fabs ( A[i][1] );
            if ( max < t ) max = t, i2 = i;
        }
        if ( max < 1e-9 ) return p;
        max = 0;
        for ( i = 0; i < 4; ++i )
        {
            if ( i == i1 ) continue;
            if ( i == i2 ) continue;
            A[i][1] /= A[i2][1];
            A[i][0] -= A[i2][0] * A[i][1];
            A[i][2] -= A[i2][2] * A[i][1];
            const double t = fabs ( A[i][2] );
            if ( max < t ) max = t, i3 = i;
        }
        if ( max < 1e-9 ) return intersection ( line[i1], line[i2] );
        i4 = 6 - i1 - i2 - i3;
        NearPointFunc4 func;
        func.k = k;
        func.a = Vector2d ( A[i3][0], A[i4][0] );
        func.b = Vector2d ( A[i3][1], A[i4][1] );
        func.v = Vector2d ( A[i3][2], A[i4][2] );
        double t = fmax ( -1, 1, func, 0 );
        Vector2d u ( func.v.x * ( 1 - fabs ( t ) ) + t * func.v.y, 
                     func.v.y * ( 1 - fabs ( t ) ) - t * func.v.x );
        switch ( k )
        {
        case 3:
            u /= root4 ( _pow4 ( u.x ) + _pow4 ( u.y ) + _pow4 ( func.a * u ) + _pow4 ( func.b * u ) );
            break;
        case 5:
            u /= root6 ( _pow6 ( u.x ) + _pow6 ( u.y ) + _pow6 ( func.a * u ) + _pow6 ( func.b * u ) );
            break;
        case 7:
            u /= root8 ( _pow8 ( u.x ) + _pow8 ( u.y ) + _pow8 ( func.a * u ) + _pow8 ( func.b * u ) );
            break;
        default:
            u /= root10 ( _pow10 ( u.x ) + _pow10 ( u.y ) + _pow10 ( func.a * u ) + _pow10 ( func.b * u ) );
        }
        double uv = u * func.v;
        double ua = u * func.a;
        double ub = u * func.b;
        switch ( k )
        {
        case 3:
            ua = _pow3 ( ua );
            ub = _pow3 ( ub );
            break;
        case 5:
            ua = _pow5 ( ua );
            ub = _pow5 ( ub );
            break;
        case 7:
            ua = _pow7 ( ua );
            ub = _pow7 ( ub );
            break;
        default:
            ua = _pow9 ( ua );
            ub = _pow9 ( ub );
        }
        return intersection ( Line2d ( line[i1].norm, line[i1].dist + uv * ua ),
                              Line2d ( line[i2].norm, line[i2].dist + uv * ub ) );
    }
    // Метод "тяжёлого шарика
    p = getNearPoint2 ( line );
    if ( ! p.isDef ) return p;
    nat i, j;
    double step = 0, d = 0;
    for ( i = 0; i < n; ++i )
    {
        const double s = fabs ( line[i] % p );
        if ( step < s ) step = s;
        const double t = fabs ( line[i].dist );
        if ( d < t ) d = t;
    }
    if ( step * 1e6 < d ) return p;
    const double eps = 1e-8 * step;
    Vector2d g0 ( 0, 0 );
    for ( j = 0; j < 50; ++j )
    {
        Vector2d g ( 0, 0 );
        for ( i = 0; i < n; ++i )
        {
            double t = line[i] % p;
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
            g += t * line[i].norm;
        }
        if ( ! g ) return p;
        Vector2d dir = 2 * g.setNorm2(-1) + g0;
        g0 = g;
        const NearPointFunc2d func ( line, p, dir.setNorm2(), k );
        const double t = fmin ( 0, step, func, eps );
        p += dir * t;
        if ( t < eps ) break;
        step = 0.5 * ( step + t );
    }
    return p;
}

Def<Vector2d> getNearPoint4_3 ( CArrRef<Line2d> line )
{
    return getNearPoint ( 3, line );
}

Def<Vector2d> getNearPoint6_5 ( CArrRef<Line2d> line )
{
    return getNearPoint ( 5, line );
}

Def<Vector2d> getNearPoint8_7 ( CArrRef<Line2d> line )
{
    return getNearPoint ( 7, line );
}

Def<Vector2d> getNearPoint10_9 ( CArrRef<Line2d> line )
{
    return getNearPoint ( 9, line );
}

//************************ 17.01.2005 *******************************//
//
//      Вычисление ближайшей точки к заданным прямым
//      Минимум суммы квадратов расстояний ( 2-норма )
//
//************************ 02.12.2016 *******************************//

Def<Vector2d> getNearPoint2 ( CArrRef<Line2d> line )
{
    Def<Vector2d> p;
    const nat n = line.size();
    if ( n < 2 ) return p;
    HMatrix<double> a ( n, 2 );
    CmbArray<double, 80> b ( n );
    for ( nat i = 0; i < n; ++i )
    {
        b[i] = - line[i].dist;
        a[i][0] = line[i].norm.x;
        a[i][1] = line[i].norm.y;
    }
    FixArray<double, 2> c;
    if ( p.isDef = ortholin ( a, b, c ) )
    {
        p.x = c[0];
        p.y = c[1];
    }
    return p;
}

Def<Vector2d> getNearPoint2 ( CArrRef<Line2d> line, CArrRef<double> mass )
{
    Def<Vector2d> p;
    const nat n = line.size();
    if ( n < 2 ) return p;
    HMatrix<double> a ( n, 2 );
    CmbArray<double, 80> b ( n );
    for ( nat i = 0; i < n; ++i )
    {
        b[i]  = - line[i].dist   * mass[i];
        a[i][0] = line[i].norm.x * mass[i];
        a[i][1] = line[i].norm.y * mass[i];
    }
    FixArray<double, 2> c;
    if ( p.isDef = ortholin ( a, b, c ) )
    {
        p.x = c[0];
        p.y = c[1];
    }
    return p;
}

//************************ 19.12.2009 *******************************//
//
//      Вычисление ближайшей точки к заданным прямым
//      Минимакс расстояний от точки до прямых ( бесконечная норма )
//
//************************ 01.06.2011 *******************************//

Def<Vector2d> getNearPointU ( CArrRef<Line2d> line )
{
    const Def<Circle2d> cir = minCircle ( line );
    return Def<Vector2d> ( cir.o, cir.isDef );
}

//************************* 03.12.2016 ******************************//
//
//    Робастный метод вычисления ближайшей точки к заданным прямым
//    Заполняет массив весов (mass) значениями от 0 до 1
//
//************************* 11.12.2019 ******************************//

template<> inline void approx2 ( CCArrRef<Line2d> & arr, Def<Vector2d> & res )
{
    res = getNearPoint2 ( arr );
}

template<> inline void approx2 ( CCArrRef<Line2d> & arr, ArrRef<double> & mass, Def<Vector2d> & res )
{
    res = getNearPoint2 ( arr, mass );
}

Def<Vector2d> getNearPointR ( CCArrRef<Line2d> & line, ArrRef<double> & mass )
{
    Def<Vector2d> res;
    approxR ( line, mass, res );
    return res;
}

//************************* 23.11.2015 ******************************//
//
//      Вычисление ближайшей точки к 3 заданным точкам
//      Минимум суммы абсолютных расстояний ( 1-норма )
//
//************************* 23.11.2015 ******************************//

Vector2d getNearPoint1N3 ( CArrRef<Vector2d> p )
{
    Vector2d v10 = p[1] - p[0];
    if ( ! v10 ) return p[0];
    Vector2d v21 = p[2] - p[1];
    if ( ! v21 ) return p[1];
    Vector2d v02 = p[0] - p[2];
    if ( ! v02 ) return p[2];
    const double d10 = v10 * v10;
    const double d21 = v21 * v21;
    const double d02 = v02 * v02;
    nat i0 = 0, i1 = 1, i2 = 2;
    if ( d21 > d10 && d21 > d02 )
    {
        i0 = 1;
        i1 = 2;
        i2 = 0;
        _swap ( v10, v21 );
        _swap ( v02, v21 );
    }
    else
    if ( d02 > d10 )
    {
        i0 = 2;
        i1 = 0;
        i2 = 1;
        _swap ( v10, v02 );
        _swap ( v02, v21 );
    }
    const double s = v02 * v21;
    const double t = v02 % v21;
    double c = sqrt ( 3. );
    if ( s > 0 && fabs ( t ) <= c * s ) return p[i2];
    if ( t < 0 ) c = - c;
    const Segment2d s1 ( p[i0], 0.5 * ( p[i2] + p[i1] + c * v21.leftPerpendicular() ) );
    const Segment2d s2 ( p[i1], 0.5 * ( p[i2] + p[i0] + c * v02.leftPerpendicular() ) );
    return intersection ( s1, s2 );
}

//************************* 23.11.2015 ******************************//
//
//      Вычисление ближайшей точки к 4 заданным точкам
//      Минимум суммы абсолютных расстояний ( 1-норма )
//
//************************* 23.11.2015 ******************************//

Vector2d getNearPoint1N4 ( CArrRef<Vector2d> p )
{
    nat i, i1 = 0, i2, i3;
    double max = p[0] * p[0];
    for ( i = 1; i < 4; ++i )
    {
        const double t = p[i] * p[i];
        if ( max < t ) max = t, i1 = i;
    }
    max = 0;
    for ( i = 0; i < 4; ++i )
    {
        if ( i == i1 ) continue;
        const double t = qmod ( p[i] - p[i1] );
        if ( ! t ) return p[i];
        if ( max < t ) max = t, i2 = i;
    }
    max = 0;
    Line2d line ( p[i2], p[i1] );
    for ( i = 0; i < 4; ++i )
    {
        if ( i == i1 || i == i2 ) continue;
        const double t = fabs ( line % p[i] );
        if ( ! t ) return p[i];
        if ( max < t ) max = t, i3 = i;
    }
    const nat i4 = 6 - ( i1 + i2 + i3 );
    double d = line % p[i4];
    if ( ! d ) return p[i4];
    if ( ( line % p[i3] ) * d < 0 )
    {
        const Def<Vector2d> o = intersection ( Segment2d ( p[i1], p[i2] ), Segment2d ( p[i3], p[i4] ) );
        return o.isDef ? o : p[i4];
    }
    line = Line2d ( p[i2], p[i3] );
    d = line % p[i4];
    if ( ! d ) return p[i4];
    if ( ( line % p[i1] ) * d < 0 )
    {
        const Def<Vector2d> o = intersection ( Segment2d ( p[i3], p[i2] ), Segment2d ( p[i1], p[i4] ) );
        return o.isDef ? o : p[i4];
    }
    line = Line2d ( p[i1], p[i3] );
    d = line % p[i4];
    if ( ! d ) return p[i4];
    if ( ( line % p[i2] ) * d < 0 )
    {
        const Def<Vector2d> o = intersection ( Segment2d ( p[i3], p[i1] ), Segment2d ( p[i2], p[i4] ) );
        return o.isDef ? o : p[i4];
    }
    return p[i4];
}

//************************* 20.09.2008 ******************************//
//
//      Вычисление ближайшей точки к заданным точкам
//      Минимум суммы абсолютных расстояний ( 1-норма )
//      Метод "тяжёлого шарика", eps - заданная точность
//
//************************* 23.11.2015 ******************************//

Def<Vector2d> getNearPoint1 ( CArrRef<Vector2d> point, double eps )
{
    nat np = point.size();
    switch ( np )
    {
    case 0: return Def<Vector2d>();
    case 1: return point[0];
    case 2: return 0.5 * ( point[0] + point[1] );
    case 3: return getNearPoint1N3 ( point );
    case 4: return getNearPoint1N4 ( point );
    }
    Vector2d v0, v1 = centerPnt ( point ), gra0(0,0), dir;
    double step = 0.;
    nat i;
    for ( i = 0; i < np; ++i ) step += qmod ( point[i] - v1 );
    if ( step == 0 ) return v1;
    step = sqrt ( step / np ) / 16.;
    if ( eps < 1e-9 * step ) eps = 1e-9 * step;
    double sum1 = 1e300;
    for (;;)
    {
        double sum2 = 0.;
        Vector2d gra1 (0,0);
        for ( i = 0; i < np; ++i )
        {
            const Vector2d dif ( point[i] - v1 );
            const double len = norm2 ( dif );
            if ( ! len ) continue;
            gra1 += dif / len;
            sum2 += len;
        }
        gra1.setNorm2();
        if ( sum2 < sum1 )
        {
            sum1 = sum2;
            v0 = v1;
            dir = ( 2. * gra1 + gra0 ).setNorm2 ( step );
            gra0 = gra1;
            v1 += dir;
            continue;
        }
        if ( step < eps )
        {
            break;
        }
        step *= 0.5;
        dir *= 0.5;
        v1 = v0 + dir;
    }
    return v0;
}

//************************* 01.12.2009 ******************************//
//
//      Вычисление ближайшей точки к заданным точкам
//      Минимум суммы квадратов расстояний ( 2-норма )
//
//************************* 01.12.2009 ******************************//

Def<Vector2d> getNearPoint2 ( CArrRef<Vector2d> p )
{
    return centerPnt ( p );
}

//************************* 01.12.2009 ******************************//
//
//      Вычисление ближайшей точки к заданным точкам
//      Минимум максимального расстояния ( бесконечная норма )
//
//************************* 01.06.2011 ******************************//

Def<Vector2d> getNearPointU ( CArrRef<Vector2d> p )
{
    const Def<Circle2d> cir = minCircleAroundPoints ( p );
    return Def<Vector2d> ( cir.o, cir.isDef );
}

//************************* 05.03.2017 ******************************//
//
//                     Сплайн второго порядка
//
//************************* 30.11.2021 ******************************//

Spline2d::Spline2d ( const Vector2d & p1, const Vector2d & p2, const Vector2d & n1, const Vector2d & n2 ) : c(p1), isDef(false)
{
    const Vector2d p21 = p2 - p1;
    SLU2<double> slu;
    slu.aa = n1.x;
    slu.ab = n1.y;
    slu.ac = 0;
    slu.ba = n2.x;
    slu.bb = n2.y;
    slu.bc = 2 * ( n2 * p21 );
    if ( slu.gauss ( b.x, b.y ) )
    {
        a = p21 - b;
    }
    else
    {
        a = null2d;
        b = p21;
    }
}

Def<Vector2d> Spline2d::getPoint ( const Vector2d & norm ) const
{
    Def<Vector2d> res;
    const double s = norm * a;
    if ( s != 0 ) res = getPoint ( -0.5 * ( norm * b ) / s );
    return res;
}

Def<double> Spline2d::getParFromX ( double x ) const
{
    double p[2];
    switch ( root2 ( a.x, b.x, c.x - x, p ) )
    {
    case 1: return p[0];
    case 2: return fabs ( p[0] - 0.5 ) < fabs ( p[1] - 0.5 ) ? p[0] : p[1];
    }
    return Def<double>();
}

Def<double> Spline2d::getParFromY ( double y ) const
{
    double p[2];
    switch ( root2 ( a.y, b.y, c.y - y, p ) )
    {
    case 1: return p[0];
    case 2: return fabs ( p[0] - 0.5 ) < fabs ( p[1] - 0.5 ) ? p[0] : p[1];
    }
    return Def<double>();
}

inline double funcX ( double a, double b, double c, double x )
{
    const double A = sqrt ( a * ( 4 * ( a*x + b ) * x + c ) );
    const double B = b + a * ( x + x );
    return ( a*c - b*b ) * log ( A + B ) + A * B;
}

inline double func0 ( double a, double b, double c )
{
    const double A = sqrt ( a * c );
    return ( a*c - b*b ) * log ( A + b ) + A * b;
}

double Spline2d::getLength ( double par ) const
{
    if ( !isDef )
    {
        aa = a * a;
        ab = a * b;
        bb = b * b;
        aaa = 4 * aa * sqrt ( aa );
        abc = func0 ( aa, ab, bb );
        isDef = true;
    }
    if ( aa * 1e12 < bb ) 
        return par * sqrt ( aa + 2*ab + bb );
    return ( funcX ( aa, ab, bb, par ) - abc ) / aaa;
}

double Spline2d::getCurvature ( double par ) const
{
    if ( ! curv.isDef ) curv = 2 * fabs ( a % b );
    const double q = qmod ( b + a * ( par + par ) );
    return q > 0 ? curv / ( q * sqrt ( q ) ) : 0;
}

double Spline2d::getCurvatureAvg () const
{
    if ( !isDef )
    {
        aa = a * a;
        ab = a * b;
        bb = b * b;
        aaa = 4 * aa * sqrt ( aa );
        abc = func0 ( aa, ab, bb );
        isDef = true;
    }
    if ( ! curv.isDef ) curv = 2 * fabs ( a % b );
    return curv > 0 ? ( ( 4 * aa + 2 * ab ) / sqrt ( 4 * aa + 4 * ab + bb ) - ( 2 * ab ) / sqrt ( bb ) ) / curv : 0;
}
