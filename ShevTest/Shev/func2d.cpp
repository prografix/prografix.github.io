
#include "math.h"

#include "rand.h"
#include "heap.h"
#include "lists.h"
#include "LinAlg.h"
#include "Mathem.h"
#include "func2d.h"
#include "func1t.h"
#include "func2t.h"
#include "opti1d.h"
#include "Vector2d.h"
#include "ShevArray.h"
#include "intersect2d.h"

const double M_PI = 3.14159265358979323846;

////////////////////////////////// Площади ////////////////////////////////////

double getArea ( const Circle2d        & fig ) { return fig.getArea(); }
double getArea ( const Ellipse2d       & fig ) { return fig.getArea(); }
double getArea ( const Triangle2d      & fig ) { return fig.getArea(); }
double getArea ( const Rectangle2d     & fig ) { return fig.getArea(); }
double getArea ( const Rhombus2d       & fig ) { return fig.getArea(); }
double getArea ( const Parallelogram2d & fig ) { return fig.getArea(); }


///////////////////////////////// Периметры ///////////////////////////////////

double getPerimeter ( const Circle2d        & fig ) { return fig.getPerimeter(); }
double getPerimeter ( const Ellipse2d       & fig ) { return fig.getPerimeter(); }
double getPerimeter ( const Triangle2d      & fig ) { return fig.getPerimeter(); }
double getPerimeter ( const Rectangle2d     & fig ) { return fig.getPerimeter(); }
double getPerimeter ( const Rhombus2d       & fig ) { return fig.getPerimeter(); }
double getPerimeter ( const Parallelogram2d & fig ) { return fig.getPerimeter(); }

//**************************** 17.10.2008 *********************************//
//
//           Построение прямых по вершинам многоугольника
//
//**************************** 21.08.2011 *********************************//

bool points2lines ( CCArrRef<Vector2d> & poly, ArrRef<Line2d> & line )
{
    for ( nat i = 0; i < poly.size(); ++i )
    {
        const Vector2d & v2 = poly[i+1<poly.size()?i+1:0];
        const Vector2d & v1 = poly[i];
        Line2d & linei = line[i];
        Vector2d & norm = linei.norm;
        norm.x = v2.y - v1.y;
        norm.y = v1.x - v2.x;
        const double q = norm * norm;
        if ( q == 0 ) return false;
        norm /= sqrt ( q );
        const double d1 = norm * v1;
        const double d2 = norm * v2;
        linei.dist = d1 > d2 ? - d1 : - d2;
    }
    return true;
}

//**************************** 17.10.2008 *********************************//
//
//            Определение габаритов множества точек
//
//**************************** 21.08.2011 *********************************//

Def<Segment2d> dimensions ( CCArrRef<Vector2d> & point )
{
    Def<Segment2d> seg;
    if ( point.size() == 0 ) return seg;
    seg.isDef = true;
    seg.a = seg.b = point[0];
    for ( nat i = 1; i < point.size(); ++i )
    {
        const Vector2d & v = point[i];
        if ( seg.b.x < v.x ) seg.b.x = v.x; else
        if ( seg.a.x > v.x ) seg.a.x = v.x;
        if ( seg.b.y < v.y ) seg.b.y = v.y; else
        if ( seg.a.y > v.y ) seg.a.y = v.y;
    }
    return seg;
}

//**************************** 25.02.2018 *********************************//
//
//            Поиск двух ближайших точек в одном множестве
//
//**************************** 25.02.2018 *********************************//

void split ( nat n, CCArrRef<SortItem<double, nat> > & a, nat m, ArrRef<SortItem<double, nat> > & b )
{
    nat i1 = 0, i2 = m;
    for ( nat i = 0; i < n; ++i )
    {
        const SortItem<double, nat> & ai = a[i];
        if ( ai.tail < m )
        {
            b[i1++] = ai;
        }
        else
        {
            SortItem<double, nat> & bi = b[i2++];
            bi.head = ai.head;
            bi.tail = ai.tail - m;
        }
    }
}

double checkStrip ( double eps, CCArrRef<SortItem<double, nat> > & x, CCArrRef<SortItem<double, nat> > & y, Set2<nat> & pp )
{
    nat i;
    const nat n = x.size();
    const double c = x[n/2].head;
    const double a = c - eps;
    const double b = c + eps;
    Suite<nat> strip;
    for ( i = 0; i < n; ++i )
    {
        const double t = x[y[i].tail].head;
        if ( a < t && t < b ) strip.inc() = i;
    }
    for ( i = 0; i < strip.size(); ++i )
    {
        const SortItem<double, nat> & si = y[strip[i]];
        const double yi = si.head;
        const double xi = x[si.tail].head;
        for ( nat j = i+1; j < strip.size(); ++j )
        {
            const SortItem<double, nat> & sj = y[strip[j]];
            const double yj = sj.head;
            if ( yj >= yi + eps ) break;
            const double xj = x[sj.tail].head;
            const double t = sqrt ( _pow2 ( xi - xj ) + _pow2 ( yi - yj ) );
            if ( eps > t )
            {
                eps = t;
                pp.a = x[si.tail].tail;
                pp.b = x[sj.tail].tail;
            }
        }
    }
    return eps;
}

double closestPoints ( CCArrRef<SortItem<double, nat> > & x, CCArrRef<SortItem<double, nat> > & y, Set2<nat> & pp )
{
    const nat n = x.size();
    if ( n < 20 )
    {
        double min = 1e300;
        for ( nat i = 0; i < n; ++i )
        {
            const SortItem<double, nat> & si = y[i];
            const double yi = si.head;
            const double xi = x[si.tail].head;
            for ( nat j = i+1; j < n; ++j )
            {
                const SortItem<double, nat> & sj = y[j];
                const double yj = sj.head;
                const double xj = x[sj.tail].head;
                double t = _pow2 ( xj - xi ) + _pow2 ( yj - yi );
                if ( min > t )
                {
                    min = t;
                    pp.a = x[si.tail].tail;
                    pp.b = x[sj.tail].tail;
                }
            }
        }
        return sqrt ( min );
    }
    const nat m = n / 2;
    DynArray<SortItem<double, nat> > t ( n );
    split ( n, y, m, t );
    Set2<nat> pp1, pp2;
    const double eps1 = closestPoints ( CArrRef<SortItem<double, nat> > ( x(), m ), 
                                       CArrRef<SortItem<double, nat> > ( t(), m ), pp1 );
    const double eps2 = closestPoints ( CArrRef<SortItem<double, nat> > ( x(m), n-m ), 
                                       CArrRef<SortItem<double, nat> > ( t(m), n-m ), pp2 );
    const double eps = eps1 < eps2 ? ( pp = pp1, eps1 ) : ( pp = pp2, eps2 );
    return checkStrip ( eps, x, y, pp );
}

double closestPoints ( CCArrRef<Vector2d> & point, Set2<nat> & pp )
{
    const nat n = point.size();
    if ( n < 250 )
    {
        double min = 1e300;
        for ( nat i = 0; i < n; ++i )
        {
            for ( nat j = i+1; j < n; ++j )
            {
                const double t = qmod ( point[i] - point[j] );
                if ( min > t )
                {
                    min = t;
                    pp.a = i;
                    pp.b = j;
                }
            }
        }
        return sqrt ( min );
    }
    nat i;
    DynArray<SortItem<double, nat> > x ( n ), y( n );
    for ( i = 0; i < n; ++i )
    {
        SortItem<double, nat> & xi = x[i];
        xi.head = point[i].x;
        xi.tail = i;
    }
    quickSort123 ( x );
    for ( i = 0; i < n; ++i )
    {
        SortItem<double, nat> & yi = y[i];
        yi.head = point[x[i].tail].y;
        yi.tail = i;
    }
    quickSort123 ( y );
    const double res = closestPoints ( x, y, pp );
    if ( pp.a > pp.b ) _swap ( pp.a, pp.b );
    return res;
}

//**************************** 17.10.2008 *********************************//
//
//        Преобразование прямоугольника к стандартному положению
//
//**************************** 21.08.2011 *********************************//

Def<Conform2d> standardPosition ( const Def<Segment2d> & seg )
{
    Def<Conform2d> res;
    if ( ! seg.isDef ) return res;
    const double u = normU ( seg );
    if ( u == 0 ) return res;
    res.magn =  1. / u;
    res.trans = ( seg.b + seg.a ) * ( -0.5 * res.magn );
    res.isDef = true;
    return res;
}

Def<Conform2d> setStandardPosition ( ArrRef<Vector2d> & points )
{
    const Def<Conform2d> res = standardPosition ( dimensions ( points ) );
// Преобразование множества точек к стандартному виду
    if ( res.isDef ) points *= Affin2d ( res );
    return res;
}

//**************************** 08.10.2018 *********************************//
//
//          Самая дальняя точка вдоль заданного направления
//
//**************************** 08.10.2018 *********************************//

Def<Vector2d> getFarthestPoint ( CCArrRef<Vector2d> & ar, const Vector2d & dir )
{
    if ( ! ar.size() ) return Def<Vector2d>();
    nat im = 0;
    double max = dir * ar[0];
    for ( nat i = 1; i < ar.size(); ++i )
    {
        double t = dir * ar[i];
        if ( max < t ) max = t, im = i;
    }
    return ar[im];
}

//**************************** 22.10.2008 *********************************//
//
//          Диаметр множества точек вдоль заданного направления
//
//**************************** 03.12.2008 *********************************//

double diameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, nat & imin, nat & imax )
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

double diameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir )
{
    nat imin, imax;
    return diameterPnt ( point, dir, imin, imax );
}

//**************************** 03.03.2009 *********************************//
//
//  Максимальный диаметр множества точек вдоль заданного сектора направлений.
//  Сектор задаётся средним направлением dir и половинным углом в градусах angle.
//  Ответ получаем в виде возвращаемого диаметра, минимального направления res
//  и пары индексов исходных точек imin и imax.
//
//**************************** 03.03.2009 *********************************//

class Diameter : public MathFunc1
{
    CArrRef<Vector2d> point;
    const Vector2d dir;
public:
    Diameter ( CArrRef<Vector2d> p, const Vector2d & d ) : point(p), dir(d) {}
    double operator () ( double x ) const
    {
        return diameterPnt ( point, Spin2d ( x, true ) ( dir ) );
    }
};

double maxDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps, Vector2d & res, nat & imin, nat & imax )
{
// Вычислим максимум методом золотого сечения
    res = Spin2d ( goldenRatioMax ( -angle, angle, Diameter ( point, dir ), eps ), true ) ( dir );
    return diameterPnt ( point, res, imin, imax );
}

double maxDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps, Vector2d & res )
{
    nat imin, imax;
    return maxDiameterPnt ( point, dir, angle, eps, res, imin, imax );
}

double maxDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps )
{
    Vector2d res;
    nat imin, imax;
    return maxDiameterPnt ( point, dir, angle, eps, res, imin, imax );
}

//*************************************************************************//
//
//                      Площадь многоугольника 
//
//**************************** 26.06.2007 *********************************//

double area ( CCArrRef<Vector2d> & vert )
{
    return 0.5 * tempArea2<double, Vector2d> ( vert );
}

//*************************************************************************//
//
//                      Периметр многоугольника
//
//**************************** 25.09.2010 *********************************//

double perimeter ( CCArrRef<Vector2d> & vert )
{
    if ( vert.size() < 2 ) return 0.;
    double p = norm2 ( vert.las() - vert[0] );
    for ( nat i = 1; i < vert.size(); ++i ) p += norm2 ( vert[i-1] - vert[i] );
    return p;
}

//**************************** 14.08.2006 *********************************//
//
//      Максимальный диаметр выпуклого многоугольника за время O ( n )
//
//**************************** 02.03.2012 *********************************//

double maxConvexPolygonDiameter ( CArrRef<Vector2d> vert, nat & ix1, nat & ix2 )
{
    ix1 = ix2 = 0;
    const nat n = vert.size();
    if ( n < 2 ) return 0.;
    const Vector2d v = vert[0] - vert[n-1];
    nat r = 1;
    double max = v % vert[r];
    for ( nat j = 2; j < n; ++j )
    {
        const double t = v % vert[j];
        if ( max < t ) max = t, r = j;
        else break;
    }
    double result = 0.;
    for ( nat i = 1; ; ++i )
    {
        const nat i1 = i - 1;
        const Vector2d v = vert[i] - vert[i1];
        nat l = r;
        double max = v % vert[r];
        nat j;
        for ( j = r+1; j < n; ++j )
        {
            const double t = v % vert[j];
            if ( max <= t ) max = t, l = j;
            else break;
        }
        for ( j = r; j <= l; ++j )
        {
            const Vector2d u = vert[j] - vert[i1];
            const double t = u * u;
            if ( result < t )
            {
                result = t;
                ix1 = i1;
                ix2 = j;
            }
        }
        if ( l == n - 1 && max <= v % vert[0] ) break;
        r = l;
    }
    return sqrt ( result );
}

double maxConvexPolygonDiameter ( CArrRef<Vector2d> vert )
{
    nat ix1, ix2;
    return maxConvexPolygonDiameter ( vert, ix1, ix2 );
}

//**************************** 29.03.2008 *********************************//
//
//      Минимальный диаметр выпуклого многоугольника за время O ( n )
//
//**************************** 02.03.2012 *********************************//

double minConvexPolygonDiameter ( CArrRef<Vector2d> vert, Vector2d & dir, nat & i1, nat & i2 )
{
    i1 = i2 = 0;
    const nat n = vert.size();
    if ( n < 2 )
    {
        dir.x = 1.; dir.y = 0;
        return 0.;
    }
    if ( n == 2 )
    {
        dir = ( vert[1] - vert[0] ).leftPerpendicular().setNorm2();
        return 0.;
    }
    nat k = 1;
    const Line2d line ( vert[0], vert[n-1] );
    double max = line % vert[k];
    nat i;
    for ( i = 2; i < n; ++i )
    {
        const double t = line % vert[i];
        if ( max < t ) max = t, k = i;
        else break;
    }
    i2 = k;
    double min = max;
    dir = line.norm;
    for ( i = 1; i < n; ++i )
    {
        const Line2d line ( vert[i], vert[i-1] );
        double max = line % vert[k];
        nat j;
        for ( j = k+1; ; ++j )
        {
            if ( j == n ) j = 0;
            const double t = line % vert[j];
            if ( max < t ) max = t, k = j;
            else break;
        }
        if ( min > max )
        {
            min = max;
            dir = line.norm;
            i1 = i;
            i2 = k;
        }
    }
    return min;
}

double minConvexPolygonDiameter ( CArrRef<Vector2d> vert, Vector2d & dir )
{
    nat i1, i2;
    return minConvexPolygonDiameter ( vert, dir, i1, i2 );
}

double minConvexPolygonDiameter ( CArrRef<Vector2d> vert )
{
    nat i1, i2;
    Vector2d dir;
    return minConvexPolygonDiameter ( vert, dir, i1, i2 );
}

//**************************** 11.02.2025 *********************************//
//
//  Минимальный диаметр выпуклого многоугольника вдоль заданного сектора направлений за время O ( n ).
//  Сектор задаётся средним направлением dir и половинным углом в градусах angle.
//  Ответ получаем в виде возвращаемого диаметра и минимального направления res.
//
//**************************** 11.02.2025 *********************************//

double minConvexPolygonDiameter ( CCArrRef<Vector2d> & vert, Vector2d dir, double angle, Vector2d & res )
{
    dir.setNorm2();
    const nat n = vert.size();
    if ( n < 2 )
    {
        res = dir;
        return 0.;
    }
    Vector2d va = Spin2d (-angle, true ) ( dir );
    Vector2d vb = Spin2d ( angle, true ) ( dir );
    if ( va % vb < 0 ) _swap ( va, vb );
    const double da = diameterPnt ( vert, va );
    const double db = diameterPnt ( vert, vb );
    double min = da < db ? ( res = va, da ) : ( res = vb, db );
    nat k = 1;
    for ( nat i = 0; i < n; ++i )
    {
        const Line2d line ( vert[i], i > 0 ? vert[i-1] : vert[n-1] );
        double max = line % vert[k];
        for ( nat j = k+1; ; ++j )
        {
            if ( j == n ) j = 0;
            const double t = line % vert[j];
            if ( max < t ) max = t, k = j;
            else break;
        }
        Vector2d v = line.norm;
        if ( v * dir < 0 ) v = - v;
        if ( va % v > 0 && v % vb > 0 && min > max ) min = max, res = v;
    }
    return min;
}

//**************************** 04.06.2008 *********************************//
//
//              Проверка многоугольника на выпуклость
//
//**************************** 25.09.2010 *********************************//

bool isConvex ( CArrRef<Vector2d> vert )
{
    const nat n = vert.size();
    if ( n < 3 ) return true;
    Vector2d v1;
    nat i = 0;
    nat i1 = i;
    do
    {
        ++i1;
        if ( i1 == n ) i1 = 0;
        if ( i1 == i ) return true;
        v1 = vert[i1] - vert[i];
    }
    while ( ! v1 );
    for ( ;; )
    {
        Vector2d v2;
        nat i2 = i1;
        do
        {
            ++i2;
            if ( i2 == n ) i2 = 0;
            if ( i2 == i ) return true;
            v2 = vert[i2] - vert[i1];
        }
        while ( ! v2 );
        if ( v1 % v2 < 0 )
        {
            return false;
        }
        if ( i > i1 ) break;
        i = i1;
        i1 = i2;
        v1 = v2;
    }
    return true;
}

//**************************** 03.11.2007 *********************************//
//
//              Количество оборотов при обходе контура
//
//**************************** 25.09.2010 *********************************//

int loopNumber ( CArrRef<Vector2d> vert )
{
    nat n = vert.size();
    Vector2d a;
    for(;;)
    {
        if ( n < 3 ) return 0;
        a = vert[0] - vert[n-1];
        if ( !! a ) break;
        --n;
    }
    double sum = 0.;
    for ( nat i = 0; i < n; ++i )
    {
        const Vector2d b = vert[i<n-1?i+1:0] - vert[i];
        if ( ! b ) continue;
        sum += atan2 ( a%b, a*b );
        a = b;
    }
    return int ( floor ( sum / ( M_PI + M_PI ) + 0.5 ) );
}

//**************************** 29.04.2017 *********************************//
//
//              Расстояние от точки p до окружности c
//          ( отрицательное, если точка внутри окружности )
//
//**************************** 29.04.2017 *********************************//

double getDistance ( const Circle2d & c, const Vector2d & p )
{
    return ( c.r < 0 ? - norm2 ( p - c.o ) : norm2 ( p - c.o ) ) - c.r;
}

//**************************** 31.10.2008 *********************************//
//
//    Расстояние от точки p до эллипса ( x/a )^2 + ( y/b )^2 = 1
//    ( отрицательное, если точка внутри эллипса )
//    r - это ближайшая точка на границе эллипса
//
//**************************** 29.04.2017 *********************************//

class ElpDist : public MathFunc1
{
    double a, b, A, B;
public:
    ElpDist ( double _a, double _b, double _A, double _B ) : a(_a), b(_b), A(_A), B(_B) {}
    double operator () ( double t ) const
    {
        return A / _pow2 ( 1 + t * a ) + B / _pow2 ( 1 + t * b ) - 1;
    }
};

double getDistanceElp ( double a, double b, const Vector2d & p, Vector2d & r )
{
    if ( ! a || ! b )
    {
        r = Segment2d ( Vector2d ( a, b ), Vector2d ( -a, -b ) ).nearPoint ( p );
        return norm2 ( p - r );
    }
    a = fabs ( a );
    b = fabs ( b );
    const double a2 = 1 / ( a * a );
    const double b2 = 1 / ( b * b );
    const double A = p.x * p.x * a2;
    const double B = p.y * p.y * b2;
    const double D = A + B;
    const ElpDist func ( a2, b2, A, B );
    const double f = func ( 0 );
    if ( f > 0 )
    {
// Исходная точка вне эллипса
        double t = ( sqrt ( D ) - 1 ) / _min ( a2, b2 );
        zeroin ( 0, t, func, 0, t ) ;
        r.x = p.x / ( 1 + t * a2 );
        r.y = p.y / ( 1 + t * b2 );
        return norm2 ( p - r );
    }
    if ( f < 0 )
    {
// Исходная точка внутри эллипса
        if ( a <= b )
        {
            if ( p.x != 0 )
            {
                double t = a * ( fabs ( p.x ) - a );
                zeroin ( 0, t, func, 0, t );
                const double kx = 1 + t * a2;
                const double ky = 1 + t * b2;
                r.y = p.y / ky;
                if ( ky > 100 * kx )
                {
                    r.x = a * sqrt ( 1 - r.y * r.y * b2 );
                    if ( p.x < 0 ) r.x = - r.x;
                }
                else
                    r.x = p.x / kx;
            }
            else
            {
                const double t = 1 - b2 / a2;
                if ( fabs ( p.y ) < b * t )
                {
                    r.y = p.y / t;
                    r.x = a * sqrt ( 1 - r.y * r.y * b2 );
                }
                else
                {
                    r.x = 0;
                    r.y = p.y < 0 ? -b : b;
                    return - fabs ( p.y - r.y );
                }
            }
        }
        else
        {
            if ( p.y != 0 )
            {
                double t = b * ( fabs ( p.y ) - b );
                zeroin ( 0, t, func, 0, t );
                const double kx = 1 + t * a2;
                const double ky = 1 + t * b2;
                r.x = p.x / kx;
                if ( kx > 100 * ky )
                {
                    r.y = b * sqrt ( 1 - r.x * r.x * a2 );
                    if ( p.y < 0 ) r.y = - r.y;
                }
                else
                    r.y = p.y / ky;
            }
            else
            {
                const double t = 1 - a2 / b2;
                if ( fabs ( p.x ) < a * t )
                {
                    r.x = p.x / t;
                    r.y = b * sqrt ( 1 - r.x * r.x * a2 );
                }
                else
                {
                    r.y = 0;
                    r.x = p.x < 0 ? -a : a;
                    return - fabs ( p.x - r.x );
                }
            }
        }
// Если найденная точка не лежит на границе эллипса, то сдвигаем её туда по нормали
        const double k0 = r.x * r.x * a2 + r.y * r.y * b2 - 1;
        if ( fabs ( k0 ) > 1e-15 )
        {
            const Vector2d n ( r.x * a2, r.y * b2 ); // нормаль
            const double k1 = n.x * r.x * a2 + n.y * r.y * b2;
            const double k2 = n.x * n.x * a2 + n.y * n.y * b2;
            double D = k1 * k1 - k0 * k2;
            D = D > 0 ? sqrt ( D ) : 0;
            const double t = ( D - k1 ) / k2;
            r += n * t;
        }
        return - norm2 ( p - r );
    }
// Исходная точка на границе эллипса
    r = p;
    return 0;
}

double getDistanceElp ( double a, double b, const Vector2d & p )
{
    Vector2d r;
    return getDistanceElp ( a, b, p, r );
}

//**************************** 18.08.2004 *********************************//
//
//              Расстояние от точки до многоугольника
//              index - это номер ближайшей вершины
//              или ребра в зависимости от isBone
//
//**************************** 07.02.2014 *********************************//

bool distance ( CArrRef<Vector2d> vert, const Vector2d & point,
                double & dist, nat & index, bool & isBone )
{
    if ( ! tempDist2 ( vert, point, dist, index, isBone ) ) return false;
    if ( dist > 0 )
    {
        dist = sqrt ( dist );
        if ( isIntersect ( point, vert ) ) dist = - dist;
    }
    return true;
}

bool distance ( CArrRef<Vector2d> vert, const Vector2d & point,
                double & dist, nat & index )
{
    bool isBone;
    return distance ( vert, point, dist, index, isBone );
}

bool distance ( CArrRef<Vector2d> vert, const Vector2d & point, double & dist )
{
    bool isBone;
    nat index;
    return distance ( vert, point, dist, index, isBone );
}

//**************************** 09.10.2004 *********************************//
//
//                  Упорядочивание точек вдоль оси Х
//
//**************************** 14.08.2006 *********************************//

void mergeSort123 ( nat n, const Vector2d ** a, const Vector2d ** b )
{
    if ( n < 2 ) return;
    if ( n == 2 )
    {
        const Vector2d * & a0 = a[0];
        const Vector2d * & a1 = a[1];
        if ( a0->x < a1->x || ( a0->x == a1->x && a0->y <= a1->y ) ) return;
        const Vector2d * t = a0;
        a0 = a1;
        a1 = t;
        return;
    }
    nat i;
    for ( i = 0; i < n; ++i ) b[i] = a[i];
    const nat n2 = n / 2;
    const nat n1 = n - n2;
    mergeSort123 ( n1, b, a );
    mergeSort123 ( n2, b+n1, a+n1 );
    nat i1 = 0;
    nat i2 = n1;
    for ( i = 0;; ++i )
    {
        const Vector2d * & b1 = b[i1];
        const Vector2d * & b2 = b[i2];
        if ( b1->x < b2->x || ( b1->x == b2->x && b1->y <= b2->y ) )
        {
            a[i] = b[i1++];
            if ( i1 == n1 )
            {
                while ( i2 < n )
                {
                    a[++i] = b[i2++];
                }
                break;
            }
        }
        else
        {
            a[i] = b[i2++];
            if ( i2 == n )
            {
                while ( i1 < n1 )
                {
                    a[++i] = b[i1++];
                }
                break;
            }
        }
    }
}

//**************************** 09.10.2004 *********************************//
//
//      Выпуклая оболочка точек на плоскости за время O ( n log n ).
//
//**************************** 21.08.2006 *********************************//

namespace {

void convexNlogN ( nat n, ArrRef<const Vector2d *> a, ArrRef<const Vector2d *> b, nat & k1, nat & k2 )
{
    k1 = k2 = 1;
    mergeSort123 ( n, a(), b() );
    const Vector2d & e = *a[n-1];
    const Vector2d dir = e - *a[0];
    if ( !dir ) return;
    b[0] = a[0];
    for ( nat i = 1; i < n; ++i )
    {
        const Vector2d & v = *a[i];
        const double t = dir % ( v - e );
        if ( t <= 0 )
        {
            while ( k1 > 1 )
            {
                if ( ( *a[k1-2] - v ) % ( *a[k1-1] - v ) > 0 ) break;
                --k1;
            }
            a[k1++] = a[i];
        }
        if ( t >= 0 )
        {
            while ( k2 > 1 )
            {
                if ( ( *b[k2-2] - v ) % ( *b[k2-1] - v ) < 0 ) break;
                --k2;
            }
            b[k2++] = a[i];
        }
    }
}

} // namespace

SuiteRef<nat> & convexNlogN ( CArrRef<Vector2d> vert, SuiteRef<nat> & res )
{
    res.resize ( 0 );
    const nat n = vert.size();
    if ( n == 0 ) return res;
    DynArray<const Vector2d *> a ( n + n );
    ArrRef<const Vector2d *> b ( a, n, n );
    nat i;
    for ( i = 0; i < n; ++i ) a[i] = vert(i);
    nat k1, k2;
    convexNlogN ( n, a, b, k1, k2 );
// Записываем результат в выходный массив
    const Vector2d * f = vert();
    for ( i = 0; i < k1; ++i ) res.inc() = a[i] - f;
    if ( k2 > 2 )
    {
        for ( i = k2-1; --i > 0; ) res.inc() = b[i] - f;
    }
    return res;
}

SuiteRef<Vector2d> & convexNlogN ( CArrRef<Vector2d> vert, SuiteRef<Vector2d> & res )
{
    res.resize ( 0 );
    const nat n = vert.size();
    if ( n == 0 ) return res;
    DynArray<const Vector2d *> a ( n + n );
    ArrRef<const Vector2d *> b ( a, n, n );
    nat i;
    for ( i = 0; i < n; ++i ) a[i] = vert(i);
    nat k1, k2;
    convexNlogN ( n, a, b, k1, k2 );
// Записываем результат в выходный массив
    for ( i = 0; i < k1; ++i ) res.inc() = *a[i];
    if ( k2 > 2 )
    {
        for ( i = k2-1; --i > 0; ) res.inc() = *b[i];
    }
    return res;
}

//**************************** 19.07.2003 *********************************//
//
//      Выпуклая оболочка точек на плоскости за время O ( m * n ).
//
//**************************** 17.12.2008 *********************************//

SuiteRef<Vector2d> & convexMN ( SuiteRef<Vector2d> & point )
{
    if ( point.size() <= 1 ) return point;
// Поиск первой точки ( минимум по Y )
    nat i, k = 0;
    for ( i = point.size(); --i > 0; )
    {
        if ( point[k].y > point[i].y ) k = i;
        else
        if ( point[k].y == point[i].y && point[k].x < point[i].x ) k = i;
    }
    if ( k != 0 ) _swap ( point[0], point[k] );
// Поиск остальных точек ( обход против часовой стрелки )
    Vector2d w ( 1., 0.);
    nat l = 0;
    double c = -2., q = 0.;
    for(;;)
    {
        nat k = 0;
        if ( l > 0 )
        {
            const Vector2d v = point[0] - point[l];
            q = v * v;
            c = w * v;
            c *= fabs(c) / q;
        }
        for ( i = l+1; i < point.size(); ++i )
        {
            const Vector2d v = point[i] - point[l];
            double tq = v * v;
            if ( tq > 0 )
            {
                double tc = w * v;
                tc *= fabs(tc) / tq;
                if ( c < tc ) c = tc, q = tq, k = i;
                else
                if ( c == tc && q < tq ) q = tq, k = i;
            }
        }
        if ( k == 0 ) break;
        ++l;
        if ( k != l ) _swap ( point[l], point[k] );
        w = point[l] - point[l-1];
    }
    point.resize ( l + 1 );
    return point;
}


//**************************** 14.08.2006 *********************************//
//
//      Выпуклая оболочка вершин многоугольника за время O ( n ).
//      Обход вершин против часовой стрелки, иначе вызывается convexNlogN.
//      vert и res могут совпадать.
//
//**************************** 12.11.2020 *********************************//

inline nat nextIndex ( nat i, nat n )
{
    return ++i == n ? 0 : i;
}

void convexPolygon ( nat a, nat b, CCArrRef<nat> & vi, CCArrRef<Vector2d> & vert, SuiteRef<nat> & index )
{
    const nat n = vi.size();
    index.inc() = a;
    a = nextIndex ( a, n );
    if ( a == b ) return;
    index.inc() = a;
    for(;;)
    {
        a = nextIndex ( a, n );
m1:     const Vector2d & qi = vert[vi[index.las(0)]]; // последняя выпуклая вершина
        const Vector2d & q1 = vert[vi[index.las(1)]]; // предпоследняя выпуклая вершина
        const Vector2d & v = vert[vi[a]]; // исходная вершина после qi
        if ( ( v - qi ) % ( v - q1 ) < 0 )
        {
            if ( a == b ) break;
            const Vector2d & u = vert[vi.cprev(index.las())]; // исходная вершина перед qi
            if ( ( v - qi ) % ( v - u ) >= 0 )
            {
                do
                {
                    a = nextIndex ( a, n );
                    if ( a == b ) goto m1;
                }
                while ( ( vert[vi[a]] - qi ) % ( qi - q1 ) <= 0 );
                goto m1;
            }
        }
        else
        {
            while ( index.size() > 1 )
            {
                const Vector2d & qi = vert[vi[index.las(0)]];
                const Vector2d & q1 = vert[vi[index.las(1)]];
                if ( ( v - qi ) % ( q1 - qi ) > 0 ) break;
                index.dec();
            }
            if ( a == b ) break;
        }
        index.inc() = a; 
    }
}

SuiteRef<nat> & convexPolygon ( CCArrRef<Vector2d> & vert, SuiteRef<nat> & index )
{
    index.resize ( 0 );
    const nat n = vert.size();
    switch ( n )
    {
    case 2:
        if ( vert[0] != vert[1] ) index.inc() = 1;
    case 1:
        index.inc() = 0;
    case 0:
        return index;
    }
    if ( area ( vert ) <= 0 ) return convexNlogN ( vert, index );
    if ( n == 3 )
    {
        index.resize ( n );
        for ( nat i = 0; i < n; ++i ) index[i] = i;
        return index;
    }
// Выбираем не повторяющиеся вершины
    Suite<nat> temp ( n );
    temp.inc() = 0;
    nat i;
    for ( i = 1; i < n; ++i )
    {
        if ( vert[temp.las()] != vert[i] ) temp.inc() = i;
    }
    if ( vert[temp.las()] == vert[0] ) temp.dec();
    const nat m = temp.size();
    if ( m == 3 )
    {
        index = temp;
        return index;
    }
// Поиск крайних точек по X
    nat a = 0, b = 0;
    for ( i = 1; i < m; ++i )
    {
        const Vector2d & va = vert[temp[a]];
        const Vector2d & vb = vert[temp[b]];
        const Vector2d & vi = vert[temp[i]];
        if ( va.x > vi.x ) a = i;
        else
        if ( vb.x < vi.x ) b = i;
        else
        if ( va.x == vi.x && va.y > vi.y ) a = i;
        else
        if ( vb.x == vi.x && vb.y < vi.y ) b = i;
    }
// Поиск остальных точек
    convexPolygon ( b, a, temp, vert, index );
    convexPolygon ( a, b, temp, vert, index );
    for ( i = 0; i < index.size(); ++i ) index[i] = temp[index[i]];
    return index;
}

SuiteRef<Vector2d> & convexPolygon ( CCArrRef<Vector2d> & vert, SuiteRef<Vector2d> & res )
{
    const nat n = vert.size();
    if ( ! n )
    {
        res.resize(); return res;
    }
    Suite<nat> index ( n );
    convexPolygon ( vert, index );
    const nat m = index.size();
    if ( vert() == res() )
    {
        if ( vert.size() == m ) return res;
        nat k = firMin ( index );
        for ( nat i = 0; i < m; ++i )
        {
            res[i] = vert[index[k]];
            if ( ++k == m ) k = 0;
        }
        res.resize ( m );
    }
    else
    {
        if ( res.size() != m ) res.resize ( m );
        if ( m < n )
        {
            nat k = firMin ( index );
            for ( nat i = 0; i < m; ++i )
            {
                res[i] = vert[index[k]];
                if ( ++k == m ) k = 0;
            }
        }
        else
        {
            res = vert;
        }
    }
    return res;
}

//**************************** 23.08.2003 *********************************//
//
//      Окружность построенная по трём точкам.
//      o - центр окружности, r2 - квадрат радиуса.
//
//**************************** 02.02.2008 *********************************//

bool circlePPP ( const Vector2d & a, const Vector2d & b, const Vector2d & c, 
                 Vector2d & o, double & r2 )
{
    const Vector2d ba = b - a;
    const Vector2d ca = c - a;
    double p = ba % ca;
    if ( p == 0 ) return false;
    p += p;
    const double a2 = a * a;
    const double b2 = b * b - a2;
    const double c2 = c * c - a2;
    o.x = ( b2 * ca.y - c2 * ba.y ) / p;
    o.y = ( c2 * ba.x - b2 * ca.x ) / p;
    r2 = qmod ( a - o );
    return true;
}

Def<Circle2d> circlePPP ( const Vector2d & a, const Vector2d & b, const Vector2d & c )
{
    Def<Circle2d> res;
    const Vector2d ba = b - a;
    const Vector2d ca = c - a;
    double p = ba % ca;
    if ( p == 0 ) return res;
    p += p;
    const double a2 = a * a;
    const double b2 = b * b - a2;
    const double c2 = c * c - a2;
    res.o.x = ( b2 * ca.y - c2 * ba.y ) / p;
    res.o.y = ( c2 * ba.x - b2 * ca.x ) / p;
    res.r = norm2 ( a - res.o );
    res.isDef = true;
    return res;
}

//**************************** 02.02.2008 *********************************//
//
//      Окружность построенная по двум точкам и одной прямой.
//      Возвращаемое значение - количество окружностей.
//      o[2] - центры окружности, r[2] - радиусы.
//
//**************************** 19.12.2013 *********************************//

nat circlePPL ( const Vector2d & point1, const Vector2d & point2, const Line2d & line,
                         Vector2d o[2], double r[2] )
{
    const double d1 = fabs ( line % point1 );
    const double d2 = fabs ( line % point2 );
    if ( ! d1 && ! d2 )
    {
        if ( point1 != point2 ) return 0;
        o[0] = point1;
        r[0] = 0;
        return 1;
    }
    if ( d1 < d2 * 1e-12 )
    {
        const Vector2d v = point2 - point1;
        const double s = line.norm * v;
        if ( s >= 0 ) return 0;
        const double t = ( v * v ) / ( s + s );
        o[0] = point1 + t * line.norm;
        r[0] = - t;
        return 1;
    }
    if ( d2 < d1 * 1e-12 )
    {
        const Vector2d v = point1 - point2;
        const double s = line.norm * v;
        if ( s >= 0 ) return 0;
        const double t = ( v * v ) / ( s + s );
        o[0] = point2 + t * line.norm;
        r[0] = - t;
        return 1;
    }
    const Vector2d m = 0.5 * ( point1 + point2 );
    const double lm = line % m;
    if ( lm > 0 ) return 0;
    const Vector2d p ( point1.y - point2.y, point2.x - point1.x );
    const double p2 = p * p;
    const double lp = line.norm * p;
    const double a = p2 - lp * lp;
    const double c = 0.25 * p2 - lm * lm;
    if ( a < 1e-12 * p2 )
    {
        const double s = lm * lp;
        if ( s == 0 ) return 0;
        const double t = c / ( s + s );
        o[0] = m + t * p;
        r[0] = - ( lm + t * lp );
        return 1;
    }
    const double b = lm * lp;
    const double d = b * b - a * c;
    if ( d < 0 )
    {
        if ( line % point1 > 0 || line % point2 > 0 ) return 0;
        goto m1;
    }
    if ( d == 0 )
    {
m1:     const double t = b / a;
        o[0] = m + t * p;
        r[0] = - ( lm + t * lp );
        return 1;
    }
    const double q = sqrt ( d );
    const double t1 = ( b - q ) / a;
    const double t2 = ( b + q ) / a;
    o[0] = m + t1 * p;
    r[0] = - ( lm + t1 * lp );
    o[1] = m + t2 * p;
    r[1] = - ( lm + t2 * lp );
    return 2;
}

//**************************** 02.02.2008 *********************************//
//
//      Окружность построенная по одной точке и двум прямым.
//      Возвращаемое значение - количество окружностей.
//      o[2] - центры окружности, r[2] - радиусы.
//
//**************************** 04.01.2012 *********************************//

nat circlePLL ( const Vector2d & point, const Line2d & line1, const Line2d & line2,
                         Vector2d o[2], double r[2] )
{
    const double d1 = line1 % point;
    if ( d1 > 0 ) return 0;
    const double d2 = line2 % point;
    if ( d2 > 0 ) return 0;
    const Vector2d & norm1 = line1.norm;
    const Vector2d & norm2 = line2.norm;
    const double s = norm1 * norm2;
    if ( d1 == 0 )
    {
        const double c = 1 - s;
        if ( c < 8e-16 ) return 0;
        r[0] = - d2 / c;
        o[0] = point - r[0] * norm1;
        return 1;
    }
    if ( d2 == 0 )
    {
        const double c = 1 - s;
        if ( c < 8e-16 ) return 0;
        r[0] = - d1 / c;
        o[0] = point - r[0] * norm2;
        return 1;
    }
    const double p = norm1 % norm2;
    if ( s > 0 )
    {
        const double a = 1. - s;
        if ( a < 1e-12 )
        {
            const Def<Vector2d> v = intersection ( line1, line2 );
            if ( ! v.isDef )
                return 0;
            const Vector2d u ( v - point );
            const Vector2d dir = ( norm1 + norm2 ).setNorm2();
            const double z = u * dir;
            if ( z < 1e-12 )
                return 0;
            r[0] = u * u / ( z + z );
            o[0] = v - r[0] * dir;
            double d1 = line1 % o[0];
            double d2 = line2 % o[0];
            double d3 = ::norm2 ( point - o[0] );
            return 1;
        }
        const double b = - ( d1 + d2 );
        const double t = 1. + s;
        const double c = t <= 0 ? 0. : sqrt ( 2. * d1 * d2 * t );
        r[0] = ( b - c ) / a;
        o[0].x = point.x - ( ( r[0] + d1 ) * norm2.y - ( r[0] + d2 ) * norm1.y ) / p;
        o[0].y = point.y - ( ( r[0] + d2 ) * norm1.x - ( r[0] + d1 ) * norm2.x ) / p;
        if ( c == 0 ) return 1;
        r[1] = ( b + c ) / a;
        o[1].x = point.x - ( ( r[1] + d1 ) * norm2.y - ( r[1] + d2 ) * norm1.y ) / p;
        o[1].y = point.y - ( ( r[1] + d2 ) * norm1.x - ( r[1] + d1 ) * norm2.x ) / p;
        return 2;
    }
    const double dx = norm2.x - norm1.x;
    const double dy = norm2.y - norm1.y;
    const double a = dx * dx + dy * dy - p * p;
    if ( fabs ( a ) < 1e-12 ) return 0;
    const double dd = d2 - d1;
    nat res = 2;
    if ( fabs ( dx ) < fabs ( dy ) )
    {
        const double g = norm1.y * d2 - norm2.y * d1;
        const double b = dx * dd + p * g;
        const double c = dd * dd - g * g;
        double d = b * b - a * c;
        if ( d <= 0 )
        {
            double x = - b / a;
            o[0].x = point.x + x;
            o[0].y = point.y - ( dx * x + dd ) / dy;
            res = 1;
        }
        else
        {
            d = sqrt ( d );
            double x = - ( b - d ) / a;
            o[0].x = point.x + x;
            o[0].y = point.y - ( dx * x + dd ) / dy;
            x = - ( b + d ) / a;
            o[1].x = point.x + x;
            o[1].y = point.y - ( dx * x + dd ) / dy;
        }
    }
    else
    {
        const double g = norm1.x * d2 - norm2.x * d1;
        const double b = dy * dd - p * g;
        const double c = dd * dd - g * g;
        double d = b * b - a * c;
        if ( d <= 0 )
        {
            double y = - b / a;
            o[0].y = point.y + y;
            o[0].x = point.x - ( dy * y + dd ) / dx;
            res = 1;
        }
        else
        {
            d = sqrt ( d );
            double y = - ( b - d ) / a;
            o[0].x = point.x - ( dy * y + dd ) / dx;
            o[0].y = point.y + y;
            y = - ( b + d ) / a;
            o[1].x = point.x - ( dy * y + dd ) / dx;
            o[1].y = point.y + y;
        }
    }
    r[0] = - ( line1 % o[0] );
    if ( res == 1 ) return 1;
    r[1] = - ( line1 % o[1] );
    return 2;
}

//**************************** 02.02.2008 *********************************//
//
//      Окружность построенная по трём прямым.
//      o - центр окружности, r - радиус.
//
//**************************** 02.02.2008 *********************************//

bool circleLLL ( const Line2d & line0, const Line2d & line1, const Line2d & line2,
                 Vector2d & o, double & r )
{
    const Vector2d v1 = line1.norm - line0.norm;
    const Vector2d v2 = line2.norm - line0.norm;
    const double p = v2 % v1;
    if ( p == 0 ) return false;
    const double d1 = line1.dist - line0.dist;
    const double d2 = line2.dist - line0.dist;
    Vector2d v;
    v.x = ( v2.y * d1 - v1.y * d2 ) / p;
    v.y = ( v1.x * d2 - v2.x * d1 ) / p;
    const double d = line0 % v;
    if ( d > 0 ) return false;
    r = - d;
    o = v;
    return true;
}

Def<Circle2d> circleLLL ( const Line2d & line0, const Line2d & line1, const Line2d & line2 )
{
    Def<Circle2d> res;
    const Vector2d v1 = line1.norm - line0.norm;
    const Vector2d v2 = line2.norm - line0.norm;
    const double p = v2 % v1;
    if ( p == 0 ) return res;
    const double d1 = line1.dist - line0.dist;
    const double d2 = line2.dist - line0.dist;
    Vector2d v;
    v.x = ( v2.y * d1 - v1.y * d2 ) / p;
    v.y = ( v1.x * d2 - v2.x * d1 ) / p;
    const double d = line0 % v;
    if ( d > 0 ) return res;
    res.r = - d;
    res.o = v;
    res.isDef = true;
    return res;
}

//**************************** 03.10.2009 *********************************//
//
//                  Вычисление вершин прямоугольника
//
//**************************** 26.09.2010 *********************************//

FixArrRef<Vector2d, 4> & makePolygon ( const Rectangle2d & fig, FixArrRef<Vector2d, 4> & vert )
{
    vert[0] = Vector2d ( fig.a, fig.b );
    vert[1] = Vector2d (-fig.a, fig.b );
    vert[2] = Vector2d (-fig.a,-fig.b );
    vert[3] = Vector2d ( fig.a,-fig.b );
    vert *= Affin2d ( LinTran2d ( fig.spin ), fig.o );
    return vert;
}

//**************************** 20.10.2015 *********************************//
//
//                      Вычисление вершин ромба
//
//**************************** 20.10.2015 *********************************//

FixArrRef<Vector2d, 4> & makePolygon ( const Rhombus2d & fig, FixArrRef<Vector2d, 4> & vert )
{
    vert[0] = Vector2d ( fig.a, 0 );
    vert[1] = Vector2d ( 0, fig.b );
    vert[2] = Vector2d (-fig.a, 0 );
    vert[3] = Vector2d ( 0,-fig.b );
    vert *= Affin2d ( LinTran2d ( fig.spin ), fig.o );
    return vert;
}

//**************************** 14.02.2018 *********************************//
//
//                   Вычисление вершин параллелограмма
//
//**************************** 214.02.2018 *********************************//

FixArrRef<Vector2d, 4> & makePolygon ( const Parallelogram2d & fig, FixArrRef<Vector2d, 4> & vert )
{
    fig.getVerts ( vert() );
    return vert;
}

//**************************** 17.12.2008 *********************************//
//
//              Случайный выпуклый многоугольник
//
//**************************** 16.11.2015 *********************************//

SuiteRef<Vector2d> & randConvexPolygon ( SuiteRef<Vector2d> & poly )
{
    static PRandPoint2d rand;
    return convexNlogN ( DynArray<Vector2d> ( poly.size() ) << rand, poly );
}

//**************************** 17.12.2008 *********************************//
//
//                      Случайный многоугольник
//
//**************************** 05.10.2009 *********************************//

static
bool isIntersect ( const Vector2d & a, const Vector2d & b, const Vector2d & c, const Vector2d & d )
{
    const Vector2d v = b - a;
    const Vector2d u = d - c;
    const double p = v % u;
    if ( p == 0 ) return false;
    const Vector2d w = a - c;
    const double s = ( v % w ) / p;
    const double t = ( u % w ) / p;
    return s >= 0. && s <= 1. && t >= 0. && t <= 1.;
}

ArrRef<Vector2d> randPolygon ( ArrRef<Vector2d> poly )
{
    static PRandPoint2d rand;
    nat i;
    if ( poly.size() < 3 )
    {
        for ( i = 0; i < poly.size(); ++i ) poly[i] = rand();
        return poly;
    }
    for(;;)
    {
        for ( i = 0; i < poly.size(); ++i )
        {
            poly[i] = rand();
            if ( i < 3 ) continue;
            for ( nat j = 0; j < i - 2; ++j )
            {
                if ( isIntersect ( poly[j], poly[j+1], poly[i-1], poly[i] ) ) goto m1;
            }
        }
        for ( i = 1; i < poly.size() - 2; ++i )
        {
            if ( isIntersect ( poly[i], poly[i+1], poly.las(), poly[0] ) ) goto m1;
        }
        break;
m1:;}
    if ( area ( poly ) < 0 )
    {
        for ( i = 0; i < poly.size(); ++i )
        {
            poly[i].x = - poly[i].x;
        }
    }
    return poly;
}

//**************************** 13.11.2011 *********************************//
//
//       Правильный многоугольник ( r - радиус описанной окружности )
//
//**************************** 13.11.2011 *********************************//

ArrRef<Vector2d> regularPolygon ( ArrRef<Vector2d> poly, double r )
{
    if ( poly.size() < 3 )
    {
        if ( poly.size() == 1 ) poly[0].fill();
        else
        if ( poly.size() == 2 )
        {
            poly[0] = Vector2d ( 0, 1 );
            poly[1] = Vector2d ( 0,-1 );
        }
        return poly;
    }
    const double a = 2 * M_PI / poly.size();
    for ( nat i = 0; i < poly.size(); ++i )
    {
        const double f = a * i;
        poly[i].x = -r * sin ( f );
        poly[i].y =  r * cos ( f );
    }
    return poly;
}

//**************************** 25.06.2013 *********************************//
//
//                      Упрощение многоугольника
//
//**************************** 06.07.2013 *********************************//

inline 
void _swap ( SortItem<double, Set4<nat>*> & p1, SortItem<double, Set4<nat>*> & p2 )
{
    ::_swap ( p1.tail->d, p2.tail->d );
    ::_swap ( p1.tail, p2.tail );
    ::_swap ( p1.head, p2.head );
}

static double recalc ( CCArrRef<Vector2d> & poly, const Set4<nat> & set )
{
    double max = 0;
    const Line2d line ( poly[set.a], poly[set.c] );
    for ( nat i = set.a;; )
    {
        if ( ++i == poly.size() ) i = 0;
        if ( i == set.c ) break;
        const double t = fabs ( line % poly[i] );
        if ( max < t ) max = t;
    }
    return max;
}

static void simplify ( CCArrRef<Vector2d> & poly, double eps, ArrRef< Set4<nat> > & arr, MinHeap< SortItem<double, Set4<nat>*> > & heap, bool closed )
{
    const nat n = poly.size();
    nat i, j = n - 1;
    for ( i = 0; i < n; ++i )
    {
        arr[i] = Set4<nat> ( j, i, i + 1, i );
        j = i;
    }
    arr.las().c = 0;
    for ( i = 0; i < n; ++i ) heap << SortItem<double, Set4<nat>*> ( recalc ( poly, arr[i] ), arr(i) );
    nat k = 3;
    if ( ! closed )
    {
        k = 1;
        heap.del ( arr[0].d );
        heap.del ( arr.las().d );
    }
    while ( heap.size() > k && heap[0]->head < eps )
    {
        SortItem<double, Set4<nat>*> si;
        heap >> si;
        Set4<nat> & s1 = *si.tail;
        Set4<nat> & s2 = arr[s1.a];
        Set4<nat> & s3 = arr[s1.c];
        s2.c = s1.c;
        s3.a = s1.a;
        if ( s2.d < heap.size() )
        {
            heap[s2.d]->head = recalc ( poly, s2 );
            heap.down ( s2.d );
        }
        if ( s3.d < heap.size() )
        {
            heap[s3.d]->head = recalc ( poly, s3 );
            heap.down ( s3.d );
        }
    }
}

DynArrRef<nat> & simplify ( CCArrRef<Vector2d> & poly, double eps, bool closed, DynArrRef<nat> & res )
{
    const nat n = poly.size();
    if ( n < 4 )
    {
        res.resize ( n);
        for ( nat i = 0; i < n; ++i ) res[i] = i;
        return res;
    }
    DynArray< Set4<nat> > arr ( n );
    MinHeap< SortItem<double, Set4<nat>*> > heap ( n );
    simplify ( poly, eps, arr, heap, closed );
    if ( closed )
    {
        res.resize ( heap.size() );
        nat j = heap[0]->tail->b;
        for ( nat i = 0; i < res.size(); ++i )
        {
            res[i] = j;
            j = arr[j].c;
        }
    }
    else
    {
        res.resize ( heap.size() + 2 );
        nat j = arr[0].b;
        for ( nat i = 0; i < res.size(); ++i )
        {
            res[i] = j;
            j = arr[j].c;
        }
    }
    return res;
}

DynArrRef<Vector2d> & simplify ( CCArrRef<Vector2d> & poly, double eps, bool closed, DynArrRef<Vector2d> & res )
{
    const nat n = poly.size();
    if ( n < 4 )
    {
        return res = poly;
    }
    DynArray< Set4<nat> > arr ( n );
    MinHeap< SortItem<double, Set4<nat>*> > heap ( n );
    simplify ( poly, eps, arr, heap, closed );
    if ( closed )
    {
        res.resize ( heap.size() );
        nat j = heap[0]->tail->b;
        for ( nat i = 0; i < res.size(); ++i )
        {
            res[i] = poly[j];
            j = arr[j].c;
        }
    }
    else
    {
        res.resize ( heap.size() + 2 );
        nat j = arr[0].b;
        for ( nat i = 0; i < res.size(); ++i )
        {
            res[i] = poly[j];
            j = arr[j].c;
        }
    }
    return res;
}

//**************************** 05.10.2020 *********************************//
//
//           Упрощение многоугольника до заданного к-ва вершин
//
//**************************** 05.10.2020 *********************************//

inline double area ( CCArrRef<Vector2d> & poly, const Set4<nat> & set )
{
    return fabs ( ( poly[set.a] - poly[set.b] ) % ( poly[set.c] - poly[set.b] ) );
}

static void simplifyNV ( CCArrRef<Vector2d> & poly, nat nv, ArrRef< Set4<nat> > & arr, MinHeap< SortItem<double, Set4<nat>*> > & heap, bool closed )
{
    const nat n = poly.size();
    nat i, j = n - 1;
    for ( i = 0; i < n; ++i )
    {
        arr[i] = Set4<nat> ( j, i, i + 1, i );
        j = i;
    }
    arr.las().c = 0;
    for ( i = 0; i < n; ++i ) heap << SortItem<double, Set4<nat>*> ( area ( poly, arr[i] ), arr(i) );
    nat k = nv;
    if ( ! closed )
    {
        k -= 2;
        heap.del ( arr[0].d );
        heap.del ( arr.las().d );
    }
    while ( heap.size() > k )
    {
        SortItem<double, Set4<nat>*> si;
        heap >> si;
        Set4<nat> & s1 = *si.tail;
        Set4<nat> & s2 = arr[s1.a];
        Set4<nat> & s3 = arr[s1.c];
        s2.c = s1.c;
        s3.a = s1.a;
        if ( s2.d < heap.size() )
        {
            heap[s2.d]->head = area ( poly, s2 );
            heap.down ( s2.d );
        }
        if ( s3.d < heap.size() )
        {
            heap[s3.d]->head = area ( poly, s3 );
            heap.down ( s3.d );
        }
    }
}

DynArrRef<nat> & simplifyNV ( CCArrRef<Vector2d> & poly, nat nv, bool closed, DynArrRef<nat> & res )
{
    const nat n = poly.size();
    if ( n <= nv )
    {
        res.resize ( n );
        for ( nat i = 0; i < n; ++i ) res[i] = i;
        return res;
    }
    DynArray< Set4<nat> > arr ( n );
    MinHeap< SortItem<double, Set4<nat>*> > heap ( n );
    simplifyNV ( poly, nv, arr, heap, closed );
    if ( closed )
    {
        res.resize ( heap.size() );
        nat j = heap[0]->tail->b;
        for ( nat i = 0; i < res.size(); ++i )
        {
            res[i] = j;
            j = arr[j].c;
        }
    }
    else
    {
        res.resize ( heap.size() + 2 );
        nat j = arr[0].b;
        for ( nat i = 0; i < res.size(); ++i )
        {
            res[i] = j;
            j = arr[j].c;
        }
    }
    return res;
}

DynArrRef<Vector2d> & simplifyNV ( CCArrRef<Vector2d> & poly, nat nv, bool closed, DynArrRef<Vector2d> & res )
{
    const nat n = poly.size();
    if ( n <= nv )
    {
        return res = poly;
    }
    DynArray< Set4<nat> > arr ( n );
    MinHeap< SortItem<double, Set4<nat>*> > heap ( n );
    simplifyNV ( poly, nv, arr, heap, closed );
    if ( closed )
    {
        res.resize ( heap.size() );
        nat j = heap[0]->tail->b;
        for ( nat i = 0; i < res.size(); ++i )
        {
            res[i] = poly[j];
            j = arr[j].c;
        }
    }
    else
    {
        res.resize ( heap.size() + 2 );
        nat j = arr[0].b;
        for ( nat i = 0; i < res.size(); ++i )
        {
            res[i] = poly[j];
            j = arr[j].c;
        }
    }
    return res;
}

//**************************** 07.04.2023 *********************************//
//
//           Построение многоугольника по набору касательных
//
//**************************** 07.04.2023 *********************************//

namespace {

void plus ( SLU2<double> & slu, const Line2d & li )
{
    const double aa = li.norm.x * li.norm.x;
    const double ab = li.norm.x * li.norm.y;
    const double bb = li.norm.y * li.norm.y;
    slu.aa += aa;
    slu.ab += ab;
    slu.ac -= li.norm.x * li.dist;
    slu.bb += bb;
    slu.ba += ab;
    slu.bc -= li.norm.y * li.dist;
}

void minus ( SLU2<double> & slu, const Line2d & li )
{
    const double aa = li.norm.x * li.norm.x;
    const double ab = li.norm.x * li.norm.y;
    const double bb = li.norm.y * li.norm.y;
    slu.aa -= aa;
    slu.ab -= ab;
    slu.ac += li.norm.x * li.dist;
    slu.bb -= bb;
    slu.ba -= ab;
    slu.bc += li.norm.y * li.dist;
}

class Point2d
{
public:
    Vector2d v;
    nat i1, i2;
    SLU2<double> slu;
};

class Point2dItem : public ListItem<Point2d> {};
class Point2dList : public List<Point2dItem> {};

double maxDifc ( CCArrRef<Line2d> & line, const Point2dItem * p, const Point2dItem * c )
{
    SLU2<double> slu ( p->slu );
    slu += c->slu;
    minus ( slu, line[c->i1] );
    Vector2d v;
    slu.gauss ( v.x, v.y );
    nat i, i1 = p->i1, i2 = c->i2;
    double max = 0;
    if ( i1 < i2 )
    {
        for ( i = i1; i <= i2; ++i ) _maxa ( max, fabs ( line[i] % v ) );
    }
    else
    {
        const nat nl = line.size();
        for ( i = i1; i < nl; ++i ) _maxa ( max, fabs ( line[i] % v ) );
        for ( i = 0; i <= i2; ++i ) _maxa ( max, fabs ( line[i] % v ) );
    }
    return max;
}

} // namespace

inline void _swap ( SortItem<double, Point2dItem*> & p1, SortItem<double, Point2dItem*> & p2 )
{
    const SortItem<double, Point2dItem*> p ( p1 );
    p1 = p2;
    p2 = p;
    ::_swap ( p1.tail->info, p2.tail->info );
}

DynArrRef<Vector2d> & makePolygon ( CCArrRef<Line2d> & line, const double eps, DynArrRef<Vector2d> & poly )
{
    const nat nl = line.size();
    if ( nl < 3 )
    {
        return poly.resize();
    }
    nat i, j = nl - 1;
    Point2dList plist;
    for ( i = 0; i < nl; ++i )
    {
        const Line2d & l1 = line[j];
        const Line2d & l2 = line[i];
        Point2dItem * p = new Point2dItem;
        plist.addAftLas ( p );
        p->i1 = j;
        p->i2 = i;
        p->slu.aa = l1.norm.x * l1.norm.x + l2.norm.x * l2.norm.x;
        p->slu.ab = l1.norm.x * l1.norm.y + l2.norm.x * l2.norm.y;
        p->slu.ac = - l1.norm.x * l1.dist - l2.norm.x * l2.dist;
        p->slu.bb = l1.norm.y * l1.norm.y + l2.norm.y * l2.norm.y;
        p->slu.ba = p->slu.ab;
        p->slu.bc = - l1.norm.y * l1.dist - l2.norm.y * l2.dist;
        p->slu.gauss ( p->v.x, p->v.y );
        j = i;
    }
    SortItem<double, Point2dItem*> item;
    MinHeap< SortItem<double, Point2dItem*> > heap ( nl );
    Point2dItem * pi = plist.end();
    plist.top();
    do
    {
        pi->info = heap.size();
        Point2dItem * t = plist.cur();
        item.head = maxDifc ( line, pi, t );
        item.tail = pi;
        heap << item;
        pi = t;
    }
    while ( plist.next() );
    while ( heap.size() > 3 && heap[0]->head < eps )
    {
        heap >> item;
        Point2dItem * t = plist.jump ( item.tail ).cnext();
        t->slu += item.tail->slu;
        minus ( t->slu, line[t->i1] );
        t->slu.gauss ( t->v.x, t->v.y );
        t->i1 = item.tail->i1;
        plist.jump ( item.tail ).delCur();

        Point2dItem * p = plist.jump ( t ).cprev();
        SortItem<double, Point2dItem*> & ip = *heap[p->info];
        const double hp = ip.head;
        ip.head = maxDifc ( line, p, t );
        if ( hp < ip.head )
            heap.down ( p->info );
        else
            heap.raise ( p->info );

        p = plist.jump ( t ).cnext();
        SortItem<double, Point2dItem*> & it = *heap[t->info];
        const double ht = it.head;
        it.head = maxDifc ( line, t, p );
        if ( ht < it.head )
            heap.down ( t->info );
        else
            heap.raise ( t->info );
    }
    const nat np = plist.size();
    poly.resize(np);
    plist.top();
    for ( i = 0; i < np; ++i )
    {
        poly[i] = plist.cur()->v;
        plist.next();
    }
    return poly;
}