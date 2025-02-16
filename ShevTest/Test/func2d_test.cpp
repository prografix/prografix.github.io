
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/rand.h"
#include "../Shev/mathem.h"
#include "../Shev/func1t.h"
#include "../Shev/func2d.h"
#include "../Shev/func2t.h"
#include "../Shev/lists.h"
#include "../Shev/opti1d.h"
#include "../Shev/approx.h"
#include "../Shev/RealFile.h"
#include "../Shev/ShevArray.h"
#include "../Shev/intersect2d.h"

#include "../draw.h"
#include "display.h"

double timeInSec();

namespace
{

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

//**************************** 07.02.2009 *********************************//
//
//  Минимальный диаметр множества точек вдоль заданного сектора направлений.
//  Сектор задаётся средним направлением dir и половинным углом в градусах angle.
//  Ответ получаем в виде возвращаемого диаметра, минимального направления res
//  и пары индексов исходных точек imin и imax.
//
//**************************** 07.02.2009 *********************************//

double minDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps, Vector2d & res, nat & imin, nat & imax )
{
// Вычислим минимум методом золотого сечения
    res = Spin2d ( goldenRatioMin ( -angle, angle, Diameter ( point, dir ), eps ), true ) ( dir );
    return diameterPnt ( point, res, imin, imax );
}

double minDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps, Vector2d & res )
{
    nat imin, imax;
    return minDiameterPnt ( point, dir, angle, eps, res, imin, imax );
}

double minDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps )
{
    Vector2d res;
    nat imin, imax;
    return minDiameterPnt ( point, dir, angle, eps, res, imin, imax );
}

void convex_test()
{
    FixArray<Vector2d,19> p;
    Suite<Vector2d> t, r;
    static PRand rand;
    for ( nat j = 0; j < 1000; ++j )
    {
        t.resize ( p.size() );
        r.resize ( p.size() );
        nat i;
        for ( i = 0; i < t.size(); ++i )
        {
            double d = rand();
//            p[i].x = d * cos ( i * 2. * M_PI / n);
//            p[i].y = d * sin ( i * 2. * M_PI / n);
            p[i].x = d * 1e2;
            p[i].y = rand();
            t[i] = p[i];
        }
        convexNlogN ( p, r );
        for ( i = 0; i < t.size(); ++i )
        {
            double dist;
            distance ( r, t[i], dist );
            if ( dist > 0 )
            display << j << i << dist << NL;
        }
        convexMN ( t );
        if ( r.size() != t.size() )
            display << j << r.size() << t.size() << NL;
    }
    display << "end" << NL;
}

void convexPolygon_test()
{
    FixArray<Vector2d,12> buf;
    Suite<Vector2d> res;
    double time = 0;
    for ( nat n = 0; n <= 1000; ++n )
    {
        ArrRef<Vector2d> point ( buf, 0, n % 6 + 7 );
        randPolygon ( point );
        Suite<Vector2d> temp;
        for ( nat i = 0; i < point.size(); ++i )
        {
            temp.inc() = point[i];
            temp.inc() = point[i];
            temp.inc() = point[i];
        }
        temp <<= 1;
        double t0 = timeInSec();
        convexPolygon ( temp, res );
        double t1 = timeInSec();
        time += t1 - t0;
        if ( ! isConvex ( res ) )
        {
            display << n << NL;
            convexPolygon ( point, res );
            drawPolygon ( point, 0, 1, 1 );
            drawPolygon ( res, 1, 1, 0 );
            return;
        }
        for ( nat j = 0; j < point.size(); ++j )
        {
            if ( ! isIntersect ( point[j], res ) )
            {
                display << n << j << NL;
                convexPolygon ( point, res );
                drawPolygon ( point, 0, 1, 1 );
                drawPolygon ( res, 1, 1, 0 );
                return;
            }
        }
    }
    display << "end" << time << NL;
}

void convexPolygon_test2()
{
//    const char * filename = "data/cont 99.txt";
    const char * filename = "data/cont 707.txt";
    RealFile file ( filename, "r" );
    if ( ! file.isValid() )
    {
        display << "Not open file" << filename << NL;
        return;
    }
    nat32 nv;
    Suite<Vector2d> vert, res;
    while ( readIntDec ( file, nv ) )
    {
        display << nv << NL;
        vert.resize ( nv );
        int32 x, y;
        nat32 i;
        for ( i = 0; i < nv; ++i )
        {
            if ( ! readIntDec ( file, x ) || ! readIntDec ( file, y ) ) break;
            vert[i].x = x;
            vert[i].y = y;
        }
        convexPolygon ( vert, res );
        if ( ! isConvex ( res ) )
        {
            display << "not convex" << NL;
            convexPolygon ( vert, res );
            drawPolygon ( vert, 0, 1, 1 );
            drawPolygon ( res, 1, 1, 0 );
            return;
        }
        for ( nat j = 0; j < vert.size(); ++j )
        {
            if ( ! isIntersect ( vert[j], res ) )
            {
                display << "no intersect" << NL;
                convexPolygon ( vert, res );
                drawPolygon ( vert, 0, 1, 1 );
                drawPolygon ( res, 1, 1, 0 );
                return;
            }
        }
    }
    display << "end" << NL;
}

class ElpDist : public MathFunc1
{
    double a, b;
    const Vector2d p;
public:
    ElpDist ( double pa, double pb, const Vector2d & pp ) : a(pa), b(pb), p(fabs(pp.x),fabs(pp.y)) {}
    double operator () ( double x ) const
    {
        return qmod ( p - Vector2d ( a*x, b*sqrt(1-x*x) ) );
    }
};

bool distanceElp2 ( double a, double b, const Vector2d & p, double & d, Vector2d & r )
{
    if ( a <= 0 || b <= 0 ) return false;
    if ( ! p )
    {
        if ( a < b )
        {
            r.x = a;
            r.y = 0.;
            d = -a;
        }
        else
        {
            r.x = 0.;
            r.y = b;
            d = -b;
        }
        return true;
    }
    const Vector2d w ( fabs(p.x), fabs(p.y) );
    const double a2 = a * a;
    const double b2 = b * b;
    const double ab = a2 - b2;
    const double ax = a * w.x;
    const double by = b * w.y;
    double min = -1.;
    if ( ax > by )
    {
        const double g = by / ax;
        const double h = ab / ax;
        const double A = -2. * g;
        const double D = g * g;
        const double B = D + 1. - h * h;
        double u[4];
        const nat n = root4 ( A, B, A, D, u );
        for ( nat i = 0; i < n; ++i )
        {
            const double q = 1. / sqrt ( 1. + u[i] * u[i] );
            const double x = a * q;
            const double y = b * q * u[i];
            const Vector2d v ( x, y );
            const double t = qmod ( w - v );
            if ( min > t || min < 0 ) min = t, r = v;
        }
    }
    else
    {
        const double g = ax / by;
        const double h = ab / by;
        const double A = -2. * g;
        const double D = g * g;
        const double B = D + 1. - h * h;
        double u[4];
        const nat n = root4 ( A, B, A, D, u );
        for ( nat i = 0; i < n; ++i )
        {
            const double q = 1. / sqrt ( 1. + u[i] * u[i] );
            const double x = a * q * u[i];
            const double y = b * q;
            const Vector2d v ( x, y );
            const double t = qmod ( w - v );
            if ( min > t || min < 0 ) min = t, r = v;
        }
    }
    if ( min < 0 )
    {
        const ElpDist func ( a, b, p );
        const double t = goldenRatioMin ( 0., 1., func, 0. );
        r.x = a * t;
        r.y = b * sqrt ( 1. - t * t );
        min = qmod ( w - r );
    }
    d = sqrt ( min );
    if ( _pow2 ( p.x / a ) + _pow2 ( p.y / b ) < 1. ) d = -d;
    if ( p.x < 0 ) r.x = - r.x;
    if ( p.y < 0 ) r.y = - r.y;
    return true;
}

void distanceElp_test()
{
    static PRand rand;
    static PNormalRand nrand;
    double time1 = 0., time2 = 0.;
    for ( nat j = 0; j < 100000; ++j )
    {
        const double a = rand();
        const double b = 1 - a;
        const double x = j % 111 == 0 ? 0 : 0.25 * nrand();
        const double y = j % 113 == 0 ? 0 : 0.25 * nrand();
        const Vector2d p ( x, y );
        Vector2d r1;
        double d1;
        double t1 = timeInSec();
if(j==4875)
j=j;
        d1 = getDistanceElp ( a, b, p, r1 );
        {
            time1 += timeInSec() - t1 ;
            double t = _pow2 ( r1.x / a ) + _pow2 ( r1.y / b ) - 1;
            Vector2d norm ( r1.x / ( a * a ), r1.y / ( b * b ) );
            norm.setNorm2();
            Vector2d v = p - r1 - norm * ( ( p - r1 ) * norm );
            double dv = fabs ( v.x ) + fabs ( v.y );
            if ( d1 > 0 )
            {
                if ( fabs ( t ) > 3e-15 )
                    display << "1+" << j << "t =" << t << NL;
                if ( dv > 3e-15 )
                    display << "1+" << j << "dv =" << dv << NL;
            }
            else
            {
                if ( fabs ( t ) > 3e-15 )
                    display << "1-" << j << "t =" << t << NL;
                if ( dv > 3e-15 )
                    display << "1-" << j << "dv =" << dv << NL;
            }
        }
        Vector2d r2;
        double d2;
        t1 = timeInSec();
        if ( distanceElp2 ( a, b, p, d2, r2 ) )
        {
            time2 += timeInSec() - t1 ;
            double t = _pow2 ( r2.x / a ) + _pow2 ( r2.y / b ) - 1;
            Vector2d norm ( r2.x / ( a * a ), r2.y / ( b * b ) );
            norm.setNorm2();
            Vector2d v = p - r2 - norm * ( ( p - r2 ) * norm );
            double dv = fabs ( v.x ) + fabs ( v.y );
            if ( d2 > 0 )
            {
                if ( fabs ( t ) > 1e-15 )
                    display << "2+" << j << "t =" << t << NL;
                if ( dv > 1e-7 )
                    display << "2+" << j << "dv =" << dv << NL;
            }
            else
            {
                if ( fabs ( t ) > 1e-15 )
                    display << "2-" << j << "t =" << t << NL;
                if ( dv > 1e-7 )
                    display << "2-" << j << "dv =" << dv << NL;
            }
        }
        else
        {
            display << "error 2" << NL;
        }
        if ( 1.000001 * fabs ( d1 ) < fabs ( d2 ) || 
             1.000001 * fabs ( d2 ) < fabs ( d1 ) )
            display << "dif" << j << NL;
    }
    display << "time1" << time1 << NL << "time2" << time2 << NL;
}

void circle_test ()
{
    static PRand dr;
    Vector2d point[3];
    Line2d line[3];
	nat i;
    for ( i = 0; i < 3; ++i )
    {
        point[i].x = 2. * dr() - 1.;
        point[i].y = 2. * dr() - 1.;
        const double a = 2. * M_PI * dr();
        line[i].norm.x = cos ( a );
        line[i].norm.y = sin ( a );
        line[i].dist = dr();
    }
    {
        Vector2d o;
        double r;/*
        point[0] = Vector2d(1,1);
        point[1] = Vector2d(2,2);
        point[2] = Vector2d(3,3);*/
        if ( circlePPP ( point[0], point[1], point[2], o, r ) )
        {
            double t0 = fabs ( qmod ( point[0] - o ) - r ) / r;
            if ( t0 > 1e-15 )
            {
                display << "ppp" << 0 << t0 << NL;
            }
            double t1 = fabs ( qmod ( point[1] - o ) - r ) / r;
            if ( t1 > 1e-15 )
            {
                display << "ppp" << 1 << t1 << NL;
            }
            double t2 = fabs ( qmod ( point[2] - o ) - r ) / r;
            if ( t2 > 1e-15 )
            {
                display << "ppp" << 2 << t2 << NL;
            }
        }
        else
        {
                display << "ppp err" << NL;
        }
    }
    {
        if ( line[2] % point[1] > 0 && line[2] % point[0] > 0 ) line[2] = -line[2];
        Vector2d o[2];
        double r[2];/*
        point[0] = Vector2d(1,line[0].dist);
        point[1] = Vector2d(3,line[0].dist);
        line[2] = Line2d(Vector2d(0,1),0);*/
        nat n = circlePPL ( point[0], point[1], line[2], o, r );
        if ( n == 0 )
        {
            double d1 = line[2] % point[0];
            double d2 = line[2] % point[1];
            if ( d1 * d2 > 0 )
            {
                circlePPL ( point[0], point[1], line[2], o, r );
                display << "ppl err" << NL;
            }
        }
        for ( i = 0; i < n; ++i )
        {
            if ( r[i] < 0 )
            {
                display << "ppl r < 0" << NL;
            }
            double eps = 1e-14;
            double t0 = fabs ( ( norm2 ( point[0] - o[i] ) - r[i] ) / r[i] );
            if ( t0 > eps )
            {
                display << "ppl" << 0 << t0 << NL;
            }
            double t1 = fabs ( ( norm2 ( point[1] - o[i] ) - r[i] ) / r[i] );
            if ( t1 > eps )
            {
                display << "ppl" << 1 << t1 << NL;
            }
            double t2 = fabs ( ( line[2] % o[i] + r[i] ) / r[i] );
            if ( t2 > eps )
            {
                display << "ppl" << 2 << t2 << NL;
            }
        }
    }
    {
        Vector2d o[2];
        double r[2];
        if ( line[1] % point[0] > 0 ) line[1] = -line[1];
        if ( line[2] % point[0] > 0 ) line[2] = -line[2];
//        point[0] = Vector2d(1,1);
//        line[1] = Line2d(Vector2d(0, 1),-1);
//        line[2] = Line2d(Vector2d(0,-1),-1);
        nat n = circlePLL ( point[0], line[1], line[2], o, r );
        if ( n == 0 )
        {
            circlePLL ( point[0], line[1], line[2], o, r );
            display << "pll err" << NL;
        }
        for ( i = 0; i < n; ++i )
        {
            if ( r[i] < 0 )
            {
                display << "pll r < 0" << NL;
            }
            double eps = 1e-14;
            double t0 = fabs ( ( norm2 ( point[0] - o[i] ) - r[i] ) / r[i] );
            if ( t0 > eps )
            {
                display << "pll" << 0 << t0 << NL;
            }
            double t1 = fabs ( ( line[2] % o[i] + r[i] ) / r[i] );
            if ( t1 > eps )
            {
                display << "pll" << 1 << t1 << NL;
            }
            double t2 = fabs ( ( line[2] % o[i] + r[i] ) / r[i] );
            if ( t2 > eps )
            {
                display << "pll" << 2 << t2 << NL;
            }
        }
    }
    {
        Vector2d o;
        double r;
//        point[0] = Vector2d(1,1);
//        line[1] = Line2d(Vector2d(0, 1),-1);
//        line[2] = Line2d(Vector2d(0,-1),-1);
        if ( ! circleLLL ( line[0], line[1], line[2], o, r ) )
        {
            line[0] = -line[0];
            line[1] = -line[1];
            line[2] = -line[2];
        }
        if ( ! circleLLL ( line[0], line[1], line[2], o, r ) )
        {
            circleLLL ( line[0], line[1], line[2], o, r );
            display << "lll err" << NL;
        }
        else
        {
            if ( r < 0 )
            {
                display << "ppl r < 0" << NL;
            }
            double eps = 1e-14;
            double t0 = fabs ( ( line[0] % o + r ) / r );
            if ( t0 > eps )
            {
                display << "lll" << 0 << t0 << NL;
            }
            double t1 = fabs ( ( line[1] % o + r ) / r );
            if ( t1 > eps )
            {
                display << "lll" << 1 << t1 << NL;
            }
            double t2 = fabs ( ( line[2] % o + r ) / r );
            if ( t2 > eps )
            {
                display << "lll" << 2 << t2 << NL;
            }
        }
    }
    display << "end" << NL;
}

double minConvexPolygonDiameter2 ( CArrRef<Vector2d> vert, Vector2d & dir )
{
    double min = 1e300;
    for ( nat i = 0; i < vert.size(); ++i )
    {
        const nat i1 = i > 0 ? i - 1 : vert.size() - 1;
        const Line2d line ( vert[i], vert[i1] );
        double max = 0.;
        for ( nat j = 0; j < vert.size(); ++j )
        {
            const double t = line % vert[j];
            if ( max <= t ) max = t;
        }
        if ( min > max )
        {
            min = max;
            dir = line.norm;
        }
    }
    return min;
}

void minConvexPolygonDiameter_test1()
{
    const nat nn = 9;
    Suite<Vector2d> point;
    for ( nat32 i = 0; i < 10000; ++i )
    {
        point.resize ( nn );
        randConvexPolygon ( point );
        Vector2d dir1, dir2;
        double d1 = minConvexPolygonDiameter ( point, dir1 );
        double d2 = minConvexPolygonDiameter2( point, dir2 );
        if ( d1 != d2 )
        {
            display << "!=" << NL;
            minConvexPolygonDiameter ( point, dir1 );
            minConvexPolygonDiameter2( point, dir2 );
        }
    }
    display << "end" << NL;
}

void minConvexPolygonDiameter_test2()
{
    const nat nn = 299;
    Suite<Vector2d> point;
    for ( nat32 n = 0; n <= nn; ++n )
    {
        point.resize ( nn );
        randConvexPolygon ( point );
        Vector2d dir1, dir2;
        double t0 = timeInSec();
        double d1 = minConvexPolygonDiameter ( point, dir1 );
        double t1 = timeInSec();
        double d2 = minConvexPolygonDiameter2( point, dir2 );
        double t2 = timeInSec();
        display << point.size() << t1-t0 << t2-t1 << NL;
    }
    display << "end" << NL;
}

void minConvexPolygonDiameter_test3()
{
    const nat nn = 7;
    Suite<Vector2d> point;
    PRandVector2d vrand;
    double time1 = 0, time2 = 0;
    for ( nat32 i = 0; i < 20000; ++i )
    {
        point.resize ( nn );
        randConvexPolygon ( point );
        Vector2d v1, v2;
        Vector2d dir = vrand();
if ( i == 15500 ) drawPolygon ( point, 0, 1, 1 );
double t0 = timeInSec();
        double d1 = maxConvexPolygonDiameter ( point, dir, 5, v1 );
double t1 = timeInSec();
        double d2 = maxDiameterPnt ( point, dir, 5, 1e-4, v2 );
double t2 = timeInSec();
time1 += t1-t0;
time2 += t2-t1;
if ( i == 15500 )
{
    Vector2d va = Spin2d (-5, true ) ( dir );
    Vector2d vb = Spin2d ( 5, true ) ( dir );
    draw ( Segment2d ( null2d, va ), 1, 1, 1 );
    draw ( Segment2d ( null2d, vb ), 1, 1, 1 );
    draw ( Segment2d ( null2d, v1 ), 1, 1, 0 );
    draw ( Segment2d ( null2d, v2 ), 1, 0, 0 );
    display << va << vb << NL;
    display << v1 << NL << v2 << NL;
    display << "d" << diameterPnt ( point, v1 ) << diameterPnt ( point, v2 ) << NL;
}
        if ( fabs ( d1 - d2 ) > 0.1 )
        {
            display << i << d1 - d2 << NL;
        }
    }
    display << time1 << time2 << NL;
    display << "end" << NL;
}

void isConvex_test()
{
    const nat nn = 4;
    FixArray<Vector2d, nn> point;
    FixArray<Vector2d, nn+nn> point2;
    for ( nat32 n = nn; n <= nn; ++n )
    {
        nat np = n;//randConvexPolygon ( n, point );
        randPolygon ( ArrRef<Vector2d> ( point, 0, np ) );
//        drawPolygon ( np, point );
        for ( nat32 i = 0; i < np; ++i )
        {
            point2[i+i] = point2[i+i+1] = point[i];
        }
//        if ( ! isConvex ( np+np, point2 ) )
//        display << np << "err" << NL;
        display << n << isConvex ( CArrRef<Vector2d> ( point2, 0, n+n ) ) << NL;
    }
    display << "end" << NL;
}

void simplify_test()
{
    DynArray<Vector2d> poly ( 32 );
    regularPolygon ( poly );
    DynArray<  nat   > res1;
    DynArray<Vector2d> res2;
    bool closed = 0;
    simplify ( poly, 0.13, closed, res1 );
    simplify ( poly, 0.13, closed, res2 );
    if ( closed )
    {
        drawPolygon ( poly, 0, 1, 1 );
        drawPolygon ( res2, 1, 1, 1 );
    }
    else
    {
        drawLine ( poly, 0, 1, 1 );
        drawLine ( res2, 1, 1, 1 );
    }
    display << poly.size() << res2.size() << NL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
void split ( nat n, CArrRef<SortItem<double, nat> > a, nat m, ArrRef<SortItem<double, nat> > b )
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

double checkStrip ( double eps, CArrRef<SortItem<double, nat> > x, CArrRef<SortItem<double, nat> > y, Set2<nat> & pp )
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

double closestPoints ( CArrRef<SortItem<double, nat> > x, CArrRef<SortItem<double, nat> > y, Set2<nat> & pp )
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

double closestPoints1 ( CArrRef<Vector2d> point, Set2<nat> & pp )
{
    nat i;
    const nat n = point.size();
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
    return closestPoints ( x, y, pp );
}
*/
double closestPoints2 ( CArrRef<Vector2d> point, Set2<nat> & pp )
{
    double min = 1e9;
    const nat n = point.size();
    for ( nat i = 0; i < n; ++i )
    {
        for ( nat j = i+1; j < n; ++j )
        {
            double t = qmod ( point[i] - point[j] );
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

void closestPoints_test()
{
    FixArray<Vector2d, 16> point;
    static PRandPoint2d rand;
    point << rand;
    drawPoints ( point, 0, 1, 1 );
    Set2<nat> pp1, pp2;
    double d1 = closestPoints  ( point, pp1 );
    double d2 = closestPoints2 ( point, pp2 );
    drawCross ( point[pp2.a], 1, 1, 0 );
    drawCross ( point[pp2.b], 1, 1, 0 );
    drawCross ( point[pp1.a], 1, 0, 0 );
    drawCross ( point[pp1.b], 1, 0, 0 );
    if ( pp1 != pp2 ) display << d1 << d2 << NL;
}

void closestPoints_test2()
{
    Suite<Vector2d> point;
    static PRandPoint2d rand;
    Set2<nat> pp1, pp2;
    for ( nat n = 100; n <= 400; ++n )
    {
        point.resize ( n );
        point << rand;
        double t0 = timeInSec();
        double d1 = closestPoints  ( point, pp1 );
        double t1 = timeInSec();
        double d2 = closestPoints2 ( point, pp2 );
        double t2 = timeInSec();
        display << n << t1-t0 << t2-t1 << NL;
        if ( pp1 != pp2 ) display << d1 << d2 << NL;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

nat split ( nat n, CArrRef<SortItem<double, nat> > a, nat m, ArrRef<SortItem<double, nat> > b )
{
    nat i, i1 = 0;
    for ( i = 0; i < n; ++i )
    {
        const SortItem<double, nat> & ai = a[i];
        if ( ai.tail < m ) b[i1++] = ai;
    }
    nat i2 = i1;
    for ( i = 0; i < n; ++i )
    {
        const SortItem<double, nat> & ai = a[i];
        if ( ai.tail >= m )
        {
            SortItem<double, nat> & bi = b[i2++];
            bi.head = ai.head;
            bi.tail = ai.tail - m;
        }
    }
    return i1;
}

double minAngle ( const Vector2d & a, const Vector2d & b, const Vector2d & c )
{
    const Vector2d ab = b - a;
    const Vector2d bc = c - b;
    const Vector2d ca = a - c;
    const double aa = atan2 ( ca % ab, -(ca * ab) );
    const double bb = atan2 ( ab % bc, -(ab * bc) );
    const double cc = atan2 ( bc % ca, -(bc * ca) );
    return _min ( aa, bb, cc );
}

void checkStrip ( CArrRef<SortItem<double, nat> > x, 
                  CArrRef<SortItem<double, nat> > y1, CArrRef<SortItem<double, nat> > y2, 
                  double & eps, Set2<nat> & pp )
{
    nat n1 = y1.size();
    nat n2 = y2.size();
    if ( ! n1 || ! n2 ) return;
    const double c = x[x.size()/2].head;
    const double a = c - eps;
    const double b = c + eps;
    Suite<nat> strip1, strip2;
    nat i;
    for ( i = 0; i < n1; ++i )
    {
        if ( a < x[y1[i].tail].head ) strip1.inc() = i;
    }
    for ( i = 0; i < n2; ++i )
    {
        if ( x[y2[i].tail].head < b ) strip2.inc() = i;
    }
    n1 = strip1.size();
    n2 = strip2.size();
    if ( ! n1 || ! n2 ) return;
    if ( n1 == 1 )
    {
        const SortItem<double, nat> & si = y1[strip1[0]];
        const double yi = si.head;
        const double xi = x[si.tail].head;
        for ( nat j = 0; j < n2; ++j )
        {
            const SortItem<double, nat> & sj = y2[strip2[j]];
            const double yj = sj.head;
            const double xj = x[sj.tail].head;
            const double t = sqrt ( _pow2 ( xi - xj ) + _pow2 ( yi - yj ) );
            if ( eps > t )
            {
                eps = t;
                pp.a = x[si.tail].tail;
                pp.b = x[sj.tail].tail;
            }
        }
        return;
    }
    if ( n2 == 1 )
    {
        const SortItem<double, nat> & si = y2[strip2[0]];
        const double yi = si.head;
        const double xi = x[si.tail].head;
        for ( nat j = 0; j < n1; ++j )
        {
            const SortItem<double, nat> & sj = y1[strip1[j]];
            const double yj = sj.head;
            const double xj = x[sj.tail].head;
            const double t = sqrt ( _pow2 ( xi - xj ) + _pow2 ( yi - yj ) );
            if ( eps > t )
            {
                eps = t;
                pp.a = x[sj.tail].tail;
                pp.b = x[si.tail].tail;
            }
        }
        return;
    }
    nat i1 = 0, i2 = 0;
    for(;;)
    {
        const nat i3 = i1 + 1;
        const nat i4 = i2 + 1;
        const SortItem<double, nat> & s1 = y1[strip1[i1]];
        const SortItem<double, nat> & s2 = y2[strip2[i2]];
        const SortItem<double, nat> & s3 = y1[strip1[i3]];
        const SortItem<double, nat> & s4 = y2[strip2[i4]];
        const Vector2d v1 ( x[s1.tail].head, s1.head );
        const Vector2d v2 ( x[s2.tail].head, s2.head );
        const Vector2d v3 ( x[s3.tail].head, s3.head );
        const Vector2d v4 ( x[s4.tail].head, s4.head );
        if ( v1.y < v2.y )
        {
            if ( ( v3 - v1 ) % ( v2 - v1 ) > 0 )
            {
                i1 = i3;
                continue;
            }
        }
        else
        {
            if ( ( v4 - v2 ) % ( v1 - v2 ) < 0 )
            {
                i2 = i4;
                continue;
            }
        }
    }
}

double closestPoints ( CArrRef<SortItem<double, nat> > x, CArrRef<SortItem<double, nat> > y1, 
                       CArrRef<SortItem<double, nat> > y2, Set2<nat> & pp )
{
    const nat n = x.size();
    const nat n1 = y1.size();
    const nat n2 = y2.size();
    if ( n1 < 2 || n2 < 2 )
    {
        double min = 1e300;
        if ( ! n1 || ! n2 ) return min;
        for ( nat i = 0; i < n1; ++i )
        {
            const SortItem<double, nat> & si = y1[i];
            const double yi = si.head;
            const double xi = x[si.tail].head;
            for ( nat j = 0; j < n2; ++j )
            {
                const SortItem<double, nat> & sj = y2[j];
                const double yj = sj.head;
                const double xj = x[sj.tail].head;
                const double t = _pow2 ( xj - xi ) + _pow2 ( yj - yi );
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
    DynArray<SortItem<double, nat> > buf ( n );
    ArrRef<SortItem<double, nat> > t1 ( buf, 0, n1 ), t2 ( buf, n1, n2 );
    const nat m1 = split ( n1, y1, m, t1 );
    const nat m2 = split ( n2, y2, m, t2 );
    typedef CArrRef<SortItem<double, nat> > T;
    T a1 ( t1, 0, m1 ), a2 ( t2, 0, m2 ), b1 ( t1, m1, n1-m1 ), b2 ( t2, m2, n2-m2 );
    Set2<nat> pp1, pp2;
    const double eps1 = closestPoints ( T ( x, 0,  m  ), a1, a2, pp1 );
    const double eps2 = closestPoints ( T ( x, m, n-m ), b1, b2, pp2 );
    double eps = eps1 < eps2 ? ( pp = pp1, eps1 ) : ( pp = pp2, eps2 );
    for ( nat i = 0; i < n2; ++i ) t2[i].tail += m;
    checkStrip ( x, a1, b2, eps, pp );
    checkStrip ( x, a2, b1, eps, pp );
    return eps;
}

double closestPoints1 ( CArrRef<Vector2d> point1, CArrRef<Vector2d> point2, Set2<nat> & pp )
{
    nat i;
    const nat n1 = point1.size();
    const nat n2 = point2.size();
    const nat n = n1 + n2;
    DynArray<SortItem<double, nat> > x ( n ), y1 ( n1 ), y2 ( n2 );
    for ( i = 0; i < n1; ++i )
    {
        SortItem<double, nat> & xi = x[i];
        xi.head = point1[i].x;
        xi.tail = i;
    }
    for ( i = 0; i < n2; ++i )
    {
        SortItem<double, nat> & xi = x[i+n1];
        xi.head = point2[i].x;
        xi.tail = i + n1;
    }
    quickSort123 ( x );
    nat i1 = 0, i2 = 0;
    for ( i = 0; i < n; ++i )
    {
        SortItem<double, nat> & xi = x[i];
        if ( xi.tail < n1 )
        {
            SortItem<double, nat> & yi = y1[i1++];
            yi.head = point1[xi.tail].y;
            yi.tail = i;
        }
        else
        {
            xi.tail -= n1;
            SortItem<double, nat> & yi = y2[i2++];
            yi.head = point2[xi.tail].y;
            yi.tail = i;
        }
    }
    quickSort123 ( y1 );
    quickSort123 ( y2 );
    return closestPoints ( x, y1, y2, pp );
}

double closestPoints2 ( CArrRef<Vector2d> point1, CArrRef<Vector2d> point2, Set2<nat> & pp )
{
    double min = 1e9;
    const nat n1 = point1.size();
    const nat n2 = point2.size();
    for ( nat i = 0; i < n1; ++i )
    {
        for ( nat j = 0; j < n2; ++j )
        {
            double t = qmod ( point1[i] - point2[j] );
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

void closestPoints2_test()
{
    Vector2d a(0,0), b(1,0), c(0,2);
    double d = minAngle ( a, c, b ); return;
    FixArray<Vector2d, 6> point1, point2;
    static PRandPoint2d rand;
    point1 << rand;
    point2 << rand;
    drawPoints ( point1, 0, 1, 1 );
    drawPoints ( point2, 1, 1, 0 );
    Set2<nat> pp1, pp2;
    double d1 = closestPoints1 ( point1, point2, pp1 );
    double d2 = closestPoints2 ( point1, point2, pp2 );
    drawCross ( point1[pp2.a], 0, 1, 1 );
    drawCross ( point2[pp2.b], 1, 1, 0 );
    drawCross ( point1[pp1.a], 0, 1, 1 );
    drawCross ( point2[pp1.b], 1, 1, 0 );
    if ( pp1 != pp2 )
    {
        display << pp1.a << pp1.b << NL;
        display << pp2.a << pp2.b << NL;
        display << d1 << d2 << NL;
    }
}

class Otrezok : public ShevItem, public Segment2d
{
public:
	Otrezok () {}
	Otrezok ( const Vector2d & x, const Vector2d & y, int i = -123456789 ) : Segment2d(x,y), ShevItem(i) {}
};

class OList : public ListItem<List<Otrezok> >
{
    OList ( OList & );
public:
    OList () {}
    OList ( const OList & list, OList & stor );
};

class ZOList : public OList
{
public:
    ZOList () {}
	ZOList ( CArrRef<Vector2d> vert, OList & stor );
	ZOList & cut ( const Line2d & line, OList & stor, int mark = 0 );
    double square () const;
};

ZOList::ZOList ( CArrRef<Vector2d> vert, OList & stor )
{
    Vector2d a = vert.las();
    for ( nat i = 0; i < vert.size(); ++i )
    {
        const Vector2d & b = vert[i];
        if ( a == b ) continue;
        Otrezok * o = addNewAftLas ( stor );
        o->a = a;
        o->b = b;
        a = b;
    }
}

double ZOList::square () const
{
    double s = 0.;
    Show<Otrezok> show ( *this );
    if ( show.top() )
    do
    {
        s += show.cur()->a % show.cur()->b;
    }
    while ( show.next() );
    return s;
}

ZOList & ZOList::cut ( const Line2d & line, OList & stor, int mark )
{
    if ( top() )
    {
        double b = line % cur()->a;
        for (;;)
        {
            double a = b;
            Otrezok * t = cur ();
            b = line % t->b;
            if ( a >= 0 && b >= 0 )
            {
                if ( curIsLas() )
                {
                    movCurAftCur ( stor );
                    break;
                }
                else
                {
                    movCurAftCur ( stor );
                    continue;
                }
            }
            if ( a < 0 && b > 0 )
            {
                Vector2d v = t->b - t->a;
                v *= b / ( a - b );
                t->b += v;
            }
            else
            if ( a > 0 && b < 0 )
            {
                Vector2d v = t->b - t->a;
                v *= a / ( a - b );
                t->a += v;
            }
            if ( ! next() ) break;
        }
        if ( ! top() ) return *this;
        Otrezok * t = cur();
        while ( next() )
        {
            if ( t->b != cur()->a )
            {
                if ( stor.cur() == 0 )
                {
                    stor.addBefCur ( new Otrezok );
                }
                Otrezok * s = stor.cur();
                s->a = t->b;
                s->b = cur()->a;
                s->info = mark;
                stor.movCurBefCur ( *this );
                next ();
            }
            t = cur();
        }
        top();
        if ( t->b != cur()->a )
        {
            if ( stor.cur() == 0 )
            {
                stor.addBefCur ( new Otrezok );
            }
            Otrezok * s = stor.cur();
            s->a = t->b;
            s->b = cur()->a;
            s->info = mark;
            stor.movCurBefCur ( *this );
        }
    }
    return *this;
}

void getSym ( nat n )
{
    nat i, j;
    DynArray<Vector2d> poly ( n );
    regularPolygon ( poly );
    DynArray<Line2d> line ( poly.size() );
    for ( i = 0; i < poly.size(); ++i ) line[i] = Line2d ( poly[i], poly.cnext(i) );
    FixArray<double, 59> score;
    OList stor;
    const Spin2d spin ( M_PI / 380 );
    for ( i = 0; i < 2; ++i )
    {
        ZOList zol ( poly, stor );
        for ( j = 0; j < line.size(); ++j ) zol.cut ( line[j], stor );
        score[i] = zol.square();
if(i) display << n << 1-0.5*score[i] / area ( poly ) << NL;
        zol.movAllAftLas ( stor );
        poly *= spin;
    }
}

void getSym()
{
    for ( nat i = 5; i <= 20; ++i ) getSym ( i );
}

void convexPolygon_test3()
{
    Suite<Vector2d> poly;
    poly.inc() = Vector2d ( 801.086,	755.336 );
    poly.inc() = Vector2d ( 774.739,	740.372 );
    poly.inc() = Vector2d ( 752.073,	724.262 );
    poly.inc() = Vector2d ( 746.41,	713.994 );
    poly.inc() = Vector2d ( 743.192,	700.834 );
    poly.inc() = Vector2d ( 744.725,	696.024 );
    poly.inc() = Vector2d ( 879.09,	399.702 );
    poly.inc() = Vector2d ( 917.446,	347.393 );
    poly.inc() = Vector2d ( 1037.77,	371.827 );
    poly.inc() = Vector2d ( 1164.13,	718.558 );
    poly.inc() = Vector2d ( 1164.73,	722.026 );
    poly.inc() = Vector2d ( 1157.9,	731.725 );
    poly.inc() = Vector2d ( 1147.17,	739.308 );
    poly.inc() = Vector2d ( 1109.59,	759.557 );
    poly.inc() = Vector2d ( 1084.69,	770.499 );
    poly.inc() = Vector2d ( 1058.51,	779.255 );
    poly.inc() = Vector2d ( 1010.72,	790.155 );
    poly.inc() = Vector2d ( 985.561,	793.076 );
    poly.inc() = Vector2d ( 944.486,	793.772 );
    poly.inc() = Vector2d ( 911.589,	790.772 );
    poly.inc() = Vector2d ( 887.756,	786.559 );
    poly.inc() = Vector2d ( 856.107,	778.111 );
    poly.inc() = Vector2d ( 817.739,	763.351 );
    poly.inc() = Vector2d ( 801.086,	755.336 );
    Suite<Vector2d> conv;
    bool b = isConvex ( poly );
    convexPolygon ( poly, conv );
    b = b;
}

} // end of namespace

void func2d_test()
{
    drawNewList2d ();
//    distanceElp_test();
//    convex_test();
//    convexPolygon_test();
//    isConvex_test();
//    circle_test ();
//    nearPoint_test1();
    minConvexPolygonDiameter_test3();
//    simplify_test();
//    closestPoints2_test();
//    getSym();
    endNewList();
}
