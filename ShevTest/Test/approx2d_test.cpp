
#include "math.h"

#include "../draw.h"

#include "../Shev/tune.h"
#include "../Shev/rand.h"
#include "../Shev/heap.h"
#include "../Shev/LinAlg.h"
#include "../Shev/Mathem.h"
#include "../Shev/approx.h"
#include "../Shev/moment2d.h"
#include "../Shev/approx2d.h"
#include "../Shev/opti1d.h"
#include "../Shev/func1d.h"
#include "../Shev/func2d.h"
#include "../Shev/func3d.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/Vector2d.h"
#include "../Shev/Vector4d.h"
#include "../Shev/ShevArray.h"
#include "../Shev/intersect2d.h"
#include "../Shev/WireModel.h"

#include "display.h"

double timeInSec ();

namespace
{

bool getLine1g ( CArrRef<Vector2d> point, CArrRef<double> mass, Vector2d & dir, Vector2d & o )
{
    const nat n = point.size();
    if ( n < 2 ) return false;
    double min = 1e300;
    nat j, i1 = 0, i2 = n;
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
                    dir = d;
                    o = p;
                }
            }
            if ( im == n || im == i2 ) break;
            i2 = i1;
            i1 = im;
        }
    return min < 1e300 && j <= n;
}

void getLine1a_test ()
{
    static PRand randX;
    static PRand randY(333);
    Suite<Vector2d> vert;
    FixArray<double, 100> mass;
    for ( nat n = 10; n <= 40; n += 1 )
    {
        double t1 = 0, t2 = 0;
        vert.resize ( n );
        for ( int j = 0; j < 30; ++j )
        {
            for ( nat i = 0; i < n; ++i )
            {
                double x = 2. * randX() - 1.;
                double y = randY() - 0.5;
                y *= 0.5 * fabs ( y );
                vert[i].x = x;
                vert[i].y = y;
                mass[i] = 1.;//floor ( 3. * randY() );
            }
            Vector2d dir1, point1;
            double d = timeInSec ();
            getLine1g ( vert, mass, dir1, point1 );
            t1 += timeInSec () - d;
            Vector2d dir2, point2;
            d = timeInSec ();
            Def<Line2d> line = getLine1 ( vert, mass );
            t2 += timeInSec () - d;
            const double mom1 = momentum1pnt ( vert, mass, Line2d ( dir1.rightPerpendicular(), dir1 % point1 ) );
            const double mom2 = momentum1pnt ( vert, mass, line );
            if ( fabs ( mom1 - mom2 ) > 2e-16 )
                 display << fabs ( mom1 - mom2 ) << NL;
        }
        display << n << t1 << t2 << NL;
    }
    display << "end" << NL;
}

Def<Vector2d> getNearPointUg ( CArrRef<Line2d> line )
{
    nat nn = line.size();
    if ( nn < 2 ) return Def<Vector2d>();
    if ( nn == 2 ) return intersection ( line[0], line[1] );
    nat n1 = nn - 1;
    nat n2 = n1 - 1;
    nat i0, i1, i2, i3;
    for ( i2 = 0; i2 < n2; ++i2 )
    {
        Line2d line2 = line[i2];
        for ( i1 = i2+1; i1 < n1; ++i1 )
        {
            Line2d line1 = line[i1];
            const Vector2d p0 = intersection ( line1, line2 );
            for ( i0 = i1+1; i0 < nn; ++i0 )
            {
                Line2d line0 = line[i0];
                const Vector2d p1 = intersection ( line2, line0 );
                const Vector2d p2 = intersection ( line0, line1 );
                if ( line0 % p0 > 0 ) line0 = - line0;
                if ( line1 % p1 > 0 ) line1 = - line1;
                if ( line2 % p2 > 0 ) line2 = - line2;
                Vector2d o;
                double r;
                if ( ! circleLLL ( line0, line1, line2, o, r ) )
                    continue;
                double max = 0;
                for ( i3 = 0; i3 < nn; ++i3 )
                {
                    if ( i3 == i0 || i3 == i1 || i3 == i2 ) continue;
                    const double t = fabs ( line[i3] % o );
                    if ( max < t ) max = t;
                }
                if ( max < r * ( 1. + 1e-6 ) ) return o;
            }
        }
    }
    return Def<Vector2d>();
}

double _norm1 ( CArrRef<Line2d> arr, const Vector2d & v )
{
    double s = 0;
    for ( nat i = 0; i < arr.size(); ++i ) s += fabs ( arr[i] % v );
    return s;
}

double _norm2 ( CArrRef<Line2d> arr, const Vector2d & v )
{
    double s = 0;
    for ( nat i = 0; i < arr.size(); ++i ) s += _pow2 ( arr[i] % v );
    return sqrt ( s );
}

double _normU ( CArrRef<Line2d> arr, const Vector2d & v )
{
    double s = 0;
    for ( nat i = 0; i < arr.size(); ++i )
    {
        const double t = fabs ( arr[i] % v );
        if ( s < t ) s = t;
    }
    return s;
}
/*
inline double dot ( CArrRef<double> a, CArrRef<double> b )
{
    double sum = 0;
    for ( nat j = 0; j < a.size(); ++j ) sum += a[j] * b[j];
    return sum;
}

    DynArray2 & swap ( DynArray2 & a )
    {
        _swap ( _size0, a._size0 );
        _swap ( _size1, a._size1 );
        _swap ( _size, a._size );
        _swap ( _data, a._data );
        return *this;
    }

    friend inline void _swap ( DynArray2 & a1, DynArray2 & a2 )
    {
        a1.swap ( a2 );
    }

bool cut ( DynArray2<double> & simplex, CArrRef<double> plane, CArrRef<double> func, DynArray2<double> & temp )
{
    const nat n = plane.size();
    CArrRef<double> vn = simplex[n];
    const double dn = dot ( plane, vn ) - 1;
    double max = 0;
    nat im = 0;
    for ( nat i = 0; i < n; ++i )
    {
        ArrRef<double> ui = temp[i];
        CArrRef<double> vi = simplex[i];
        const double di = dot ( plane, vi ) - 1;
        double t = dn - di;
        if ( t <= 0 )
        {
            t = di / dot ( plane, plane );
            for ( nat j = 0; j < n; ++j )
            {
                ui[j] = vi[j] - plane[j] * t;
            }
        }
        else
        {
            t = dn / t;
            for ( nat j = 0; j < n; ++j )
            {
                ui[j] = vn[j] + ( vi[j] - vn[j] ) * t;
            }
            const double fi = dot ( func, ui );
            if ( max < fi ) max = fi, im = i;
        }
//display << dot ( plane, ui ) - 1 << NL;
    }
    temp[n] = temp[im];
    temp[im] = simplex[im];
    temp.swap ( simplex );
    return true;
}
*/

void getNearPoint2_test1()
{
    static PRand rand;
    static PRandVector2d vrand;
    const nat n = 5;
    FixArray<Line2d, n> line;
    nat i, j;
    for ( j = 0; j < 13; ++j )
    {
        for ( i = 0; i < n; ++i )
        {
            double t = rand();
            line[i].dist = 0.25 * t * t;
            Vector2d & v = line[i].norm;
            v = vrand();
        }
    }
    for ( i = 0; i < n; ++i )
    {
        const Vector2d & v = line[i].norm;
        const Vector2d o ( - line[i].dist * v );
        const Vector2d p = 8. * v.leftPerpendicular();
        draw ( Segment2d ( o - p, o + p ), 0, 0.5f, 1 );
    }
    double t0 = timeInSec();
    const Def<Vector2d> p1 = getNearPoint1 ( line );
    double t1 = timeInSec();
    const Def<Vector2d> p2 = getNearPoint10_9 ( line );
    double t2 = timeInSec();
    double q1 = 0, q2 = 0, q3 = 0;
    for ( i = 0; i < n; ++i )
    {
        double t = line[i] % p1;
        q1 += t * root9 ( t );
        t = line[i] % p2;
        q2 += t * root9 ( t );
    }
display << q1 - q2 << t1-t0 << t2-t1 << NL;
/*    const Def<Vector2d> p3 = getNearPointUg ( line );
    const Def<Vector2d> p4 = getNearPointU ( line );
    if ( p4.isDef )
    {
        if ( qmod ( p3 - p4 ) > 1e-18 )
        {
            display << "dif3 =" << qmod ( p3 - p4 ) << NL;
        }
    }
    else display << "getNearPointU return false" << NL;
    if ( p1.isDef != p2.isDef )
    {
        display << "ok1 != ok2" << NL;
    }
    drawCross ( p1, 1, 1, 0 );
    drawCross ( p3, 0, 1, 0 );*/
    drawCross ( p1, 1, 1, 0 );
    drawCross ( p2, 1, 0, 0 );
display << "end" << NL;
}

void getNearPoint2_test2()
{
    static PRand rand;
    const nat nn = 20;
    FixArray<Line2d, nn> line;
    double n1[3], n2[3], nu[3];
    for ( nat n = 3; n <= nn; n += 1 )
    {
        for ( nat j = 0; j < 500; ++j )
        {
            for ( nat i = 0; i < n; ++i )
            {
                double t = rand();
                line[i].dist = 0.25 * t * t;
                double a = 2. * 3.14159265358979323846 * rand();
                Vector2d & v = line[i].norm;
                v.x = cos(a);
                v.y = sin(a);
            }
            CArrRef<Line2d> ref ( line, 0, n );
            const Def<Vector2d> p1 = getNearPoint1 ( ref );
            if ( p1.isDef )
            {
                n1[0] = _norm1 ( ref, p1 );
                n2[0] = _norm2 ( ref, p1 );
                nu[0] = _normU ( ref, p1 );
                if ( n1[0] < n2[0] || n2[0] < nu[0] )
                    display << "err" << NL;
            }
            else
            {
                n1[0] = n2[0] = nu[0] = 1e9;
            }
            const Def<Vector2d> p2 = getNearPoint2 ( ref );
            if ( p2.isDef )
            {
                n1[1] = _norm1 ( ref, p2 );
                n2[1] = _norm2 ( ref, p2 );
                nu[1] = _normU ( ref, p2 );
                if ( n1[1] < n2[1] || n2[1] < nu[1] )
                    display << "err" << NL;
            }
            else
            {
                n1[1] = n2[1] = nu[1] = 1e9;
            }
            const Def<Vector2d> pu = getNearPointU ( ref );
            if ( pu.isDef )
            {
                n1[2] = _norm1 ( ref, pu );
                n2[2] = _norm2 ( ref, pu );
                nu[2] = _normU ( ref, pu );
                if ( n1[1] < n2[1] || n2[1] < nu[1] )
                    display << "err" << NL;
            }
            else
            {
                n1[2] = n2[2] = nu[2] = 1e9;
            }
            if ( n1[0] > n1[1] || n1[0] > n1[2] )
                display << "err1" << NL;
            if ( n2[1] > n2[0] || n2[1] > n2[2] )
                display << "err2" << NL;
            if ( nu[2] > nu[0] || nu[2] > nu[1] )
                display << "errU" << j << nu[2] - nu[1] << NL;
        }
    }
    display << "end" << NL;
}

void getNearPoint3_9_test1()
{
    static PRand rand;
    static PRandVector2d vrand;
    const nat n = 4;
    FixArray<Line2d, n> line;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        //double t = rand();
        line[i].dist = 0.5;// * t * t;
        Vector2d & v = line[i].norm;
        v = vrand();
        const Vector2d o ( - line[i].dist * v );
        const Vector2d p = 8. * v.leftPerpendicular();
        draw ( Segment2d ( o - p, o + p ), 0, 0.5f, 1 );
    }
    /*line[0] = Line2d ( Vector2d ( 1, 0 ), -1 );
    line[1] = Line2d ( Vector2d ( 0, 1 ), -1 );
    line[2] = Line2d ( Vector2d (-1, 0 ), -1 );
    line[3] = Line2d ( Vector2d ( 0,-1 ), -1 );*/
    const Def<Vector2d> p1 = getNearPoint2   ( line );
    const Def<Vector2d> p2 = getNearPoint4_3 ( line );
    const Def<Vector2d> p3 = getNearPoint6_5 ( line );
    const Def<Vector2d> p4 = getNearPoint8_7 ( line );
    const Def<Vector2d> p5 = getNearPoint10_9( line );
    const Def<Vector2d> p6 = getNearPoint1   ( line );
    double q1 = 0, q2 = 0, q3 = 0;
    for ( i = 0; i < n; ++i )
    {
        double t = line[i] % p1;
        q1 += t * root3 ( t );
        t = line[i] % p2;
        q2 += t * root3 ( t );
        t = line[i] % p3;
        q3 += t * root3 ( t );
    }
    display << q1 - q2 << q3 - q2 << NL;
    q1 = 0, q2 = 0, q3 = 0;
    for ( i = 0; i < n; ++i )
    {
        double t = line[i] % p2;
        q1 += t * root5 ( t );
        t = line[i] % p3;
        q2 += t * root5 ( t );
        t = line[i] % p4;
        q3 += t * root5 ( t );
    }
    display << q1 - q2 << q3 - q2 << NL;
    q1 = 0, q2 = 0, q3 = 0;
    for ( i = 0; i < n; ++i )
    {
        double t = line[i] % p3;
        q1 += t * root7 ( t );
        t = line[i] % p4;
        q2 += t * root7 ( t );
        t = line[i] % p5;
        q3 += t * root7 ( t );
    }
    display << q1 - q2 << q3 - q2 << NL;
    q1 = 0, q2 = 0, q3 = 0;
    for ( i = 0; i < n; ++i )
    {
        double t = line[i] % p4;
        q1 += t * root9 ( t );
        t = line[i] % p5;
        q2 += t * root9 ( t );
        t = line[i] % p6;
        q3 += t * root9 ( t );
    }
    display << q1 - q2 << q3 - q2 << NL;
    display << NL;
    drawCross ( p1, 1, 0, 0 );
    drawCross ( p2, 1, 0.5f, 0 );
    drawCross ( p3, 0.9f, 0.9f, 0 );
    drawCross ( p4, 0.6f, 0.9f, 0 );
    drawCross ( p5, 1, 0, 1 );
    drawCross ( p6, 0, 1, 0 );
}

void getNearPoint3_9_test2()
{
    static PRand rand;
    static PRandVector2d vrand;
    const nat n = 4;
    FixArray<Line2d, n> line;
    nat i, j;
    for ( j = 0; j < 10000; ++j )
    {
        for ( i = 0; i < n; ++i )
        {
            double t = rand();
            line[i].dist = 0.25 * t * t;
            Vector2d & v = line[i].norm;
            v = vrand();
        }
        const Def<Vector2d> p1 = getNearPoint1 ( line );
        const Def<Vector2d> p2 = getNearPoint4_3 ( line );
        const Def<Vector2d> p3 = getNearPoint2 ( line );
        double q1 = 0, q2 = 0, q3 = 0;
        for ( i = 0; i < n; ++i )
        {
            double t = line[i] % p1;
            q1 += t * root3 ( t );
            t = line[i] % p2;
            q2 += t * root3 ( t );
            t = line[i] % p3;
            q3 += t * root3 ( t );
        }
        if ( q2 > q1 || q2 > q3 ) display << j << q1-q2 << q3-q2 << NL;
    }
    display << "end" << NL;
}

inline
void _draw ( const Def<Line2d> line, float r, float g, float b )
{
    if ( ! line.isDef ) return;
    const Vector2d p = 4. * line.norm.leftPerpendicular();
    const Vector2d o = -line.dist * line.norm;
    draw ( Segment2d ( o+p, o-p), r, g, b );
}

void getLinePoint_test1()
{
    static PRand rand;
    FixArray<Vector2d, 5> point;
    FixArray<double, 5> mass;
    nat i;
    for ( i = 0; i < point.size(); ++i )
    {
        double x = 2. * rand() - 1.;
        double y = rand() - 0.5;
        y *= 0.5 * fabs ( y );
        point[i].x = x;
        point[i].y = y;
    }
    _draw ( getLineR ( point, mass ), 1, 1, 0 );
    _draw ( getLine1 ( point ), 1, 0, 0 );
    _draw ( getLineU ( point ), 0, 1, 0 );
    drawPoints ( point, 0, 1, 1 );
}

double maxDist ( const Line2d & line, CArrRef<Vector2d> point )
{
    double max = 0;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const double t = fabs ( line % point[i] );
        if ( max < t ) max = t;
    }
    return max;
}

Def<Line2d> getLineU1 ( CArrRef<Vector2d> point )
{
    Def<Line2d> res;
    Suite<Vector2d> vert;
    if ( convexNlogN ( point, vert ).size() < 2 ) return res;
    nat i1, i2;
    minConvexPolygonDiameter ( vert, res.norm, i1, i2 );
    res.dist = -0.5 * ( res.norm * ( vert[i1] + vert[i2] ) );
    res.isDef = true;
    return res;
}

void getLinePoint_test()
{
    static PRand rand;
    double eps = 1e-9;
    const nat nn = 300;
    DynArray<double> x ( nn ), y1 ( nn ), y2 ( nn );
    for ( nat j = 0; j < nn; ++j )
    {
        double sum = 0;
        const nat n = j + 5;
        x[j] = 1e-6 * n;
        DynArray<Vector2d> point ( n );
        double time1 = 0, time2 = 0;
        for ( nat k = 0; k < 50; ++k )
        {
            for ( nat i = 0; i < point.size(); ++i )
            {
                double x = 2. * rand() - 1.;
                double y = rand() - 0.5;
                y *= 0.5 * fabs ( y );
                point[i].x = x;
                point[i].y = y;
            }
            double r;
            double t0 = timeInSec();
            Line2d line1 = getLineU ( point, r );
            double t1 = timeInSec();
            Line2d line2 = getLineU1 ( point );
            double t2 = timeInSec();
            time1 += t1-t0;
            time2 += t2-t1;
            double r1 = maxDist ( line1, point );
            double r2 = maxDist ( line2, point );
if ( line1.norm * line2.norm < 0 ) line2 = - line2;
//if ( norm2 ( line1.norm - line2.norm ) > eps || fabs ( line1.dist - line2.dist) > eps )
if ( fabs ( r1 - r2 ) > eps )
{
    getLineU ( point, r );
    for ( nat i = 0; i < point.size(); ++i ) display << point[i] << NL;
    display << r1 - r2 << norm2 ( line1.norm - line2.norm ) << fabs ( line1.dist - line2.dist) << NL;
}
        }
//        display << n << time2 << time1 << NL; 
        y1[j] = time1;
        y2[j] = time2;
    }
    FixArray<double, 2> c1, c2;
    apprLSS_H ( nn, x, y1, 2, c1 );
    apprLSS_H ( nn, x, y2, 2, c2 );
display << c2[1] / c1[1] << NL;
}

void getLineSegment_test()
{
    static PRandPoint2d rand;
    FixArray<Segment2d, 3> seg;
    nat i;
    for ( i = 0; i < seg.size(); ++i )
    {
        seg[i].a = rand();
        seg[i].b = rand();
        draw ( seg[i], 0, 1, 1 );
    }
    Def<Line2d> line = getLine2 ( seg );
    if ( line.isDef )
    {
        Vector2d p = 4. * line.norm.leftPerpendicular();
        Vector2d o = -line.dist * line.norm;
        draw ( Segment2d ( o+p, o-p), 1, 1, 0 );
    }
}

void getLine_test2()
{
    static PRand randX;
    static PRand randY(333);
    FixArray<Vector2d, 100> v;
    for ( int n = 10; n <= 100; n += 10 )
    {
        double t1 = 0, t2 = 0;
        for ( int j = 0; j < 10; ++j )
        {
            for ( int i = 0; i < n; ++i )
            {
                double x = 2. * randX() - 1.;
                double y = randY() - 0.5;
                y *= 0.5 * fabs ( y );
                v[i].x = x;
                v[i].y = y;
            }
            CArrRef<Vector2d> ref ( v, 0, n );
            double d = timeInSec ();
            Def<Line2d> line1 = getLine1 ( ref );
            t1 += timeInSec () - d;
            d = timeInSec ();
            const Def<Line2d> line2 = getLine2 ( ref );
            t2 += timeInSec () - d;
        }
        display << n << t1 << t2 << NL;
    }
    display << "end" << NL;
}

double diff2 ( CArrRef<Vector2d> p, const Circle2d & cir )
{
    double e = 0;
    for ( nat i = 0; i < p.size(); ++i )
    {
        const Vector2d d = cir.o - p[i];
        double q = d * d;
        if ( q == 0 ) continue;
        q = sqrt ( q );
        e += _pow2 ( q - cir.r );
    }
    return e / p.size();
}

void getCirclePnt_test1()
{
    const nat32 nn = 7;
    FixArray<Vector2d, nn> point;
    static PRand rand;
    for ( nat32 n = nn; n <= nn; ++n )
    {
        double t1 = 0, t2 = 0;
        for ( int j = 0; j < 1; ++j )
        {
            Vector2d o;
            o.x = - rand();
            o.y = - rand();
//display << o << NL;
            for ( nat32 i = 0; i < n; ++i )
            {
                point[i].x = rand();
                point[i].y = rand();
                point[i].setNorm2();
                point[i] *= 1. - 1e-1 * ( rand() - 0.5 );
                point[i] += o;
            }
    //        point[0] += o;
            CArrRef<Vector2d> p ( point, 0, n );
            Vector2d c1, c2;
//            double r2;
//            nat jx1, jx2, jx3;
            double t = timeInSec();
            Circle2d cir1 = getCirclePnt22( p );
            Circle2d cir2 = getCirclePnt2 ( p );
    draw ( cir1, 1, 0, 0 );
    draw ( cir2, 1, 1, 0 );
    drawPoints ( p, 0, 1, 1 );
display << diff2 ( p, cir1 ) - diff2 ( p, cir2 ) << NL;
            t1 += timeInSec() - t;
        }
//        display << n << t1 << t2 << NL;
    }
    display << "end" << NL;
}

void getCirclePnt_test2()
{
    const nat32 nn = 7;
    FixArray<Vector2d, nn> point;
    static PRand rand;
    for ( nat32 j = 0; j < 1; ++j )
    {
        Vector2d o;
        o.x = rand();
        o.y = rand();
        for ( nat32 i = 0; i < nn; ++i )
        {
            point[i].x = rand();
            point[i].y = rand();
            point[i].setNorm2();
            point[i] *= 1. - 5e-2 * ( rand() - 0.5 );
            point[i] -= o;
        }
    }
    drawPoints ( point, 0, 1, 1 );
    Circle2d cir1 = getCirclePnt2 ( point );
    draw ( cir1, 1, 1, 0 );
    Circle2d cir2 = getCirclePnt22 ( point );
    draw ( cir2, 1, 0, 0 );
    Circle2d cir3 = getCirclePnt1 ( point );
    draw ( cir3, 0, 1, 0 );
}

Ellipse2d getEllipsePnt2 ( CArrRef<Vector2d> p )
{
    Ellipse2d elli;
    const nat n = p.size();
    if ( n < 5 ) return elli;
    double max = qmod ( p[0] );
    nat i, im = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( p[i] );
        if ( max < t ) max = t, im = i;
    }
    Vector2d f1 = p[im];
    max = qmod ( f1 - p[0] );
    im = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( f1 - p[i] );
        if ( max < t ) max = t, im = i;
    }
    Vector2d f2 = p[im];
    if ( f1 == f2 ) return elli;
    Circle2d cir = getCirclePnt22 ( p );
    f1 = f2 = cir.o;
    double d;
    HMatrix<double> a ( n, 3 );
    Suite<double> stor;
    CmbArray<double, 80> b ( n );
    for ( nat j = 0; j < 25; ++j )
    {
        const Vector2d v1 = f1;
        const Vector2d v2 = f2;
        for ( i = 0; i < n; ++i )
        {
            const Vector2d v = ( f1 - p[i] ).setNorm2();
            a[i][0] = v.x;
            a[i][1] = v.y;
            a[i][2] = -1;
            b[i] = v * p[i] - norm2 ( f2 - p[i] );
        }
        FixArray<double, 3> c;
        if ( ! ortholin ( a, b, c, stor ) ) return elli;
        f1.x = c[0];
        f1.y = c[1];
        d = c[2];
        for ( i = 0; i < n; ++i )
        {
            const Vector2d v = ( f2 - p[i] ).setNorm2();
            a[i][0] = v.x;
            a[i][1] = v.y;
            a[i][2] = -1;
            b[i] = v * p[i] - norm2 ( f1 - p[i] );
        }
        if ( ! ortholin ( a, b, c, stor ) ) return elli;
        f2.x = c[0];
        f2.y = c[1];
        d = c[2];
display << j << qmod ( f1 - v1 ) << qmod ( f2 - v2 ) << NL;
        if ( qmod ( f1 - v1 ) < 1e-16 * d * d && qmod ( f2 - v2 ) < 1e-16 * d * d ) break;
    }
    const Vector2d v = 0.5 * ( f2 - f1 );
    elli.a = 0.5 * d;
    elli.b = sqrt ( elli.a * elli.a - v * v );
    elli.o = 0.5 * ( f1 + f2 );
    elli.spin = Spin2d ( atan2 ( v.y, v.x ) ); 
    return elli;
}

void getEllipsePnt_test1()
{
    const nat32 nn = 7;
    FixArray<Vector2d, nn> point;
    static PRand rand;
    for ( nat32 n = nn; n <= nn; ++n )
    {
        double t1 = 0, t2 = 0;
        for ( int j = 0; j < 1; ++j )
        {
            Vector2d o;
            o.x = - rand();
            o.y = - rand();
            o.fill();
//display << o << NL;
            for ( nat32 i = 0; i < n; ++i )
            {
                point[i].x = rand();
                point[i].y = rand();
                point[i].setNorm2();
                point[i].y *= 0.5;
                point[i] += o;
            }
    //        point[0] += o;
            CArrRef<Vector2d> p ( point, 0, n );
            Vector2d c1, c2;
//            double r2;
//            nat jx1, jx2, jx3;
            double t = timeInSec();
            Ellipse2d elli0;
            elli0.a = 1;
            elli0.b = 0.5;
            elli0.o = o;
            Ellipse2d elli = getEllipsePnt2 ( p );
    draw ( elli, 1, 1, 0 );
    draw ( elli0, 1, 0, 0 );
    drawPoints ( p, 0, 1, 1 );
            t1 += timeInSec() - t;
        }
//        display << n << t1 << t2 << NL;
    }
    display << "end" << NL;
}

void getCirclePlg_test ()
{
    static PRand rand;
    DynArray<Vector2d> poly1 ( 88 );
    const double coef1 = 2 * M_PI / poly1.size();
    const Vector2d shift ( 0.2 * ( rand() - 0.5 ), 0.1 * ( rand() - 0.5 ) ); 
    const Vector2d x ( cos ( 2 * M_PI * rand() ), sin ( 2 * M_PI * rand() ) );
    const Vector2d y ( x.leftPerpendicular() );
    nat i;
    for ( i = 0; i < poly1.size(); ++i )
    {
        poly1[i] = shift + ( x * cos(coef1*i) + y * sin(coef1*i) ) * ( 0.7 + 0.3 * rand() );
    }
    drawPolygon ( poly1.reverse(), 0, 1, 1 );
    draw ( getCirclePlg ( poly1 ), 1, 1, 0 );
}

void difference ( double a, double b, CArrRef<Vector2d> poly, const Conform2d & conf )
{
    DynArray<Vector2d> temp ( poly );
    temp *= Affin2d ( ~conf );
    double dif1 = 0, dif2 = 0;
    nat i;
    for ( i = 0; i < temp.size(); ++i )
    {
        double d = getDistanceElp ( a, b, temp[i] );
        dif1 += fabs ( d );
        dif2 += d * d;
    }
    display << dif1 << dif2 << NL;
}

void getRectangle_test ()
{
    static PRand rand;
    DynArray<Vector2d> poly1 ( 44 );
    const double coef1 = 2 * M_PI / poly1.size();
    const double ratio = 0.1 + rand();
    const Vector2d shift ( 0.2 * ( rand() - 0.5 ), 0.1 * ( rand() - 0.5 ) ); 
    const Vector2d x ( cos ( 2 * M_PI * rand() ), sin ( 2 * M_PI * rand() ) );
    const Vector2d y ( x.leftPerpendicular() );
    nat i;
    for ( i = 0; i < poly1.size(); ++i )
    {
        poly1[i] = shift + ( x * cos(coef1*i) + y * sin(coef1*i) * ratio ) * ( 1. + 0.3 * rand() );
    }
    drawPolygon ( poly1.reverse(), 0, 1, 1 );
    FixArray<Vector2d, 4> poly2;
    drawPolygon ( makePolygon ( getRectanglePlg ( poly1 ), poly2 ), 1, 1, 0 );
}

Def<Vector2d> getNearPoint1a ( CArrRef<Vector2d> point )
{
    Def<Vector2d> res;
    const nat np = point.size();
    switch ( np )
    {
    case 0: return res;
    case 1: return res = point[0];
    case 2: return res = 0.5 * ( point[0] + point[1] );
    }
    Vector2d v = centerPnt ( point );
    SLU2<double> slu;
    for ( nat k = 0; k < 9; ++k )
    {
        slu.fill(0,0);
        for ( nat i = 0; i < np; ++i )
        {
            const Vector2d u = v - point[i];
            const double d = norm2 ( u );
            const double a = 1 / d;
            const double a3 = _pow3 ( a );
            slu.aa += u.y * u.y * a3;
            slu.ab -= u.x * u.y * a3;
            slu.ac += ( point[i] % u ) * u.y * a3 - a * u.x;
            slu.bb += u.x * u.x * a3;
            slu.bc += ( u % point[i] ) * u.x * a3 - a * u.y;
        }
        slu.ba = slu.ab;
        const double f1 = slu.aa * v.x + slu.ab * v.y - slu.ac;
        const double f2 = slu.ab * v.x + slu.bb * v.y - slu.bc;
        const double func = _pow2 ( f1 ) + _pow2 ( f2 );
display << k << func << v.x << v.y << NL;
        if ( func < 1e-24 ) break;
        slu.gauss ( v.x, v.y );
    }
    return v;
}

void nearPoint1_test()
{
    FixArray<Vector2d, 7> point;
    static PRand rand;
    nat i;
//for ( nat k = 0; k < 6; ++k )
    for ( i = 0; i < point.size(); ++i )
    {
        point[i].x = 2*rand() - 1;
        point[i].y = 2*rand() - 1;
    }
    drawPoints ( point, 0, 1, 1 );
    Vector2d v1 = getNearPoint1  ( point );
    Vector2d v2 = getNearPoint1a ( point );
    double sum1 = 0.;
    for ( i = 0; i < point.size(); ++i ) sum1 += sqrt ( qmod ( point[i] - v1 ) );
    double sum2 = 0.;
    for ( i = 0; i < point.size(); ++i ) sum2 += sqrt ( qmod ( point[i] - v2 ) );
    display << sum2 - sum1 << NL;
    drawCross ( v1, 1, 1, 0 );
    drawCross ( v2, 1, 0, 0 );
    return;
    drawCross ( getNearPoint1 ( point ), 1, 1, 0 );
    drawCross ( getNearPoint2 ( point ), 1, 0, 0 );
    drawCross ( getNearPointU ( point ), 0, 1, 0 );
}

void spline_test1()
{
    for ( nat i = 1; i < 16; ++i )
    {
        Vector2d v1 ( 1, 0 );
        double a = M_PI / ( i + i );
        Vector2d v2 ( cos(a), sin(a) );
        Spline2d spline ( v1, v2, v1, v2 );
        display << spline.getCurvature (0)  << spline.getCurvature (1) << NL;
    }
}

inline 
void makeSpline ( CArrRef<Vector2d> point, CArrRef<Vector2d> norm, nat k,
                  double t, Spline2d & spline1, Spline2d & spline2 ) 
{
    Vector2d v = ( point.cnext(k) - point[k] ) * t + ( point[k] - point.cprev(k) ) * ( 1 - t );
    v = v.rightPerpendicular();
    spline1 = Spline2d ( point.cprev(k), point[k], norm.cprev(k), v );
    spline2 = Spline2d ( point[k], point.cnext(k), v, norm.cnext(k) );
}

class CurvatureAvg : public MathFunc1
{
    CArrRef<Vector2d> point, norm;
    const nat k;
public:
    CurvatureAvg ( CArrRef<Vector2d> p1, CArrRef<Vector2d> p2, nat p3 ) : 
      point(p1), norm(p2), k(p3) {}
    virtual double operator () ( double t ) const
    {
        Spline2d spline1, spline2;
        makeSpline ( point, norm, k, t, spline1, spline2 );
        return spline1.getCurvatureAvg() * spline1.getLength() + spline2.getCurvatureAvg() * spline2.getLength();
    }
};

struct SplineJoint
{
    nat iarr, iheap;
    Vector2d normi;
    Spline2d spline1, spline2;
    void init ( CArrRef<Vector2d> point, CArrRef<Vector2d> norm, CArrRef<Spline2d> spline )
    {
        CurvatureAvg func ( point, norm, iarr );
        double t = fmin ( 0, 1, func, 1e-4 );
        normi = ( point.cnext(iarr) - point[iarr] ) * t + ( point[iarr] - point.cprev(iarr) ) * ( 1 - t );
        normi = normi.rightPerpendicular();
        spline1 = Spline2d ( point.cprev(iarr), point[iarr], norm.cprev(iarr), normi );
        spline2 = Spline2d ( point[iarr], point.cnext(iarr), normi, norm.cnext(iarr) );
    }
    double getValue ( CArrRef<Spline2d> spline ) const
    {
        return spline.cprev(iarr).getCurvatureAvg() + spline[iarr].getCurvatureAvg();
    }
    void setHeapHead ( MaxHeap< SortItem<double, SplineJoint *> > & heap, CArrRef<Spline2d> spline ) const
    {
        const double t = getValue ( spline );
        if ( heap[iheap]->head > t )
        {
            heap[iheap]->head = t;
            heap.down ( iheap );
        }
        else
        {
            heap[iheap]->head = t;
            heap.raise ( iheap );
        }
    }
};

inline 
void _swap ( SortItem<double, SplineJoint *> & p1, SortItem<double, SplineJoint *> & p2 )
{
    ::_swap ( p1.tail->iheap, p2.tail->iheap );
    ::_swap ( p1.tail, p2.tail );
    ::_swap ( p1.head, p2.head );
}

double sumCurvatureAvg ( CArrRef<Spline2d> spline )
{
    double sum = 0;
    for ( nat i = 0; i < spline.size(); ++i ) sum += spline[i].getCurvatureAvg() * spline[i].getLength();
    return sum;
}

void makeSpline ( CArrRef<Vector2d> point, ArrRef<Spline2d> spline )
{
    nat i;
    const nat n = point.size();
    DynArray<Vector2d> norm ( n );
    for ( i = 0; i < n; ++i )
    {
        norm[i] = ( point.cnext(i) - point.cprev(i) ).rightPerpendicular();
//drawPoint ( point[i] + norm[i], 0, 1, 1 );
    }
    for ( i = 0; i < n; ++i )
    {
        spline[i] = Spline2d ( point[i], point.cnext(i), norm[i], norm.cnext(i) );
    }
    DynArray<SplineJoint> joint ( n );
    MaxHeap< SortItem<double, SplineJoint *> > heap ( n );
    for ( i = 0; i < n; ++i )
    {
        joint[i].iarr = i;
        joint[i].iheap = i;
        joint[i].init ( point, norm, spline );
        const double val = joint[i].getValue ( spline );
//display << dif1 << dif2 << NL;
        heap << SortItem<double, SplineJoint *> ( val, joint(i) );
    }
    //double eps = 0.01;
    for ( i = 0; i < 5; ++i )
    {
display << sumCurvatureAvg ( spline ) << NL;
        nat k = heap[0]->tail->iarr;
//display << i << heap[0]->head << NL;
        norm[k] = joint[k].normi;
        spline.cprev(k) = joint[k].spline1;
        spline[k] = joint[k].spline2;
        joint.cprev(k).init ( point, norm, spline );
        joint[k].init ( point, norm, spline );
        joint.cnext(k).init ( point, norm, spline );
        joint.cprev(k).setHeapHead ( heap, spline );
        joint[k].setHeapHead ( heap, spline );
        joint.cnext(k).setHeapHead ( heap, spline );
    }/*
    for ( i = 0; i < n; ++i )
    {
        double d1 = spline[i].getCurvature (0);
        double d2 = spline.cprev(i).getCurvature (1);
        display << d1  << d2 << d1 - d2 << NL;
//drawPoint ( point[i] + norm[i], 1, 1, 0 );
    }*/
    display << NL;
}

void spline_test()
{
    nat i;
    const nat n = 11;
    static PRand rand;
    Suite<Vector2d> polygon;
    polygon.resize(n);
    const double coef1 = 2 * M_PI / polygon.size();
    const double ratio = 0.5 + rand();
    const Vector2d shift ( 0, 0 );//0.2 * ( rand() - 0.5 ), 0.1 * ( rand() - 0.5 ) ); 
    const Vector2d x ( cos ( 2 * M_PI * rand() ), sin ( 2 * M_PI * rand() ) );
    const Vector2d y ( x.leftPerpendicular() );
    for ( i = 0; i < polygon.size(); ++i )
    {
        polygon[i] = shift + ( x * cos(coef1*i) + y * sin(coef1*i) * ratio ) * ( 0.5 + 0.1 * rand() );
    }
    DynArray<Vector2d> norm ( polygon.size() );
    for ( i = 0; i < polygon.size(); ++i )
    {
        norm[i] = ( polygon.cnext(i) - polygon.cprev(i) ).rightPerpendicular();
    }
    for ( i = 0; i < polygon.size(); ++i )
    {
        Spline2d spline ( polygon[i], polygon.cnext(i), norm[i], norm.cnext(i) );
        Vector2d a = spline.getPoint ( 0 );
        for ( nat j = 1; j < 9; ++j )
        {
            Vector2d b = spline.getPoint ( 0.125 * j );
            draw ( Segment2d ( a, b ), 1, 1, 0 );
            a = b;
        }
    }
    drawPoints ( polygon, 0, 1, 1 );
    DynArray<Spline2d> spline ( n );
    makeSpline ( polygon, spline );
    for ( i = 0; i < polygon.size(); ++i )
    {
        Vector2d a = spline[i].getPoint ( 0 );
        for ( nat j = 1; j < 9; ++j )
        {
            Vector2d b = spline[i].getPoint ( 0.125 * j );
            draw ( Segment2d ( a, b ), 0, 1, 1 );
            a = b;
        }
        //display << spline.getCurvature (0)  << spline.getCurvature (1) << NL;
    }
    drawPoint ( null2d, 0, 1, 1 );
}

double getCurvatureAvg ( const Spline2d & spline )
{
    double sum = 0;
    const nat n = 100;
    const double step = 1. / n;
    for ( nat j = 1; j < n; ++j ) sum += spline.getCurvature ( step * j );
    sum += 0.5 * ( spline.getCurvature ( 0 ) + spline.getCurvature ( 1 ) );
    return sum / n;
}

void spline_test2()
{
    nat i;
    const nat n = 4;
    static PRand rand;
    Suite<Vector2d> polygon;
    polygon.resize(n);
    const double coef1 = 2 * M_PI / polygon.size();
    const double ratio = 0.5 + rand();
    const Vector2d shift ( 0.2 * ( rand() - 0.5 ), 0.1 * ( rand() - 0.5 ) ); 
    const Vector2d x ( cos ( 2 * M_PI * rand() ), sin ( 2 * M_PI * rand() ) );
    const Vector2d y ( x.leftPerpendicular() );
    for ( i = 0; i < polygon.size(); ++i )
    {
        polygon[i] = shift + ( x * cos(coef1*i) + y * sin(coef1*i) * ratio ) * ( 0.5 + 0.1 * rand() );
        //polygon[i] *= 2;
    }
    DynArray<Vector2d> norm ( polygon.size() );
    for ( i = 0; i < polygon.size(); ++i )
    {
        norm[i] = ( polygon.cnext(i) - polygon.cprev(i) ).rightPerpendicular();
    }
    for ( i = 0; i < polygon.size(); ++i )
    {
        Spline2d spline ( polygon[i], polygon.cnext(i), norm[i], norm.cnext(i) );
        Vector2d a = spline.getPoint ( 0 );
        for ( nat j = 1; j < 9; ++j )
        {
            Vector2d b = spline.getPoint ( 0.125 * j );
            draw ( Segment2d ( a, b ), 1, 1, 0 );
            a = b;
        }
        display << getCurvatureAvg ( spline ) << spline.getCurvatureAvg () << NL;
    }
    drawPoints ( polygon, 0, 1, 1 );
}

void spline_test3()
{
//    Vector2d v0(1, 1), v1(1.05, 1.05), v2(1.1,1.07);
    Vector2d v0(1.0000000, 1.0000002), v1(1.0500000, 1.0499438), v2(1.1000000, 1.0998841);
    Vector2d u1 = (v1 - v0).leftPerpendicular();
    Vector2d u2 = (v2 - v1).leftPerpendicular();
    Spline2d spline1 ( v0, v1, u1-0.2*u2, u1+u2 );
    double x = 1.025;
    Def<double> p = spline1.getParFromX ( x );
    double y = spline1.getY(p);
    Def<double> p1 = spline1.getParFromY ( y );
    double x1 = spline1.getX(p1);
    double t=x-x1;
    t=t;
    /*for ( int i = 90; i <= 105; i+=1 )
    {
        double x = 0.01*i;
        Def<double> p = spline1.getParFromX ( x );
        display << x << spline1.getY(p) << NL;
    }
    Spline2d spline2 ( v1, v2, u1+u2, u2-0.2*u1 );
    for ( i = 106; i <= 120; i+=1 )
    {
        double x = 0.01*i;
        Def<double> p = spline2.getParFromX ( x );
        display << x << spline2.getY(p) << NL;
    }*/
}

Line2d getLineUL ( CArrRef<Vector2d> point, const Vector2d & norm, double & r )
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
        ::_swap ( i0, i2 );
        line = Line2d ( point[i1], point[i0] );
        r = fabs ( line % point[i2] );
    }
    if ( r < eps || i2 == i0 || i2 == i1 ) return line;
    r *= 0.5;
    return line;
}

struct OCP_data
{
    double cosa, sina;
    Vector4d arr[5];
};

Def<Conform2d> overlayConvexPolygon ( MaxHeap<SortItem<double,nat> > & heap, ArrRef<OCP_data> & data,
                                      CCArrRef<Vector2d> & vert, CCArrRef<Line2d> & line ) 
{
    for ( nat k = 0; k < 1000; ++k )
    {
        SortItem<double,nat> & si = * heap[0];
        OCP_data & dt = data[si.tail];
        Vector4d & a0 = dt.arr[0];
        const Vector2d o ( a0.x1, a0.x2 );
        const Vector2d w ( dt.cosa + dt.sina * a0.x3, dt.sina - dt.cosa * a0.x3 );
        double max = -1;
        nat i, im, km;
        for ( i = 0; i < line.size(); ++i )
        {
            const Line2d & li = line[i];
            const Vector2d u ( w * li.norm, w % li.norm );
            nat jm = 0;
            double pmax = u * vert[0];
            for ( nat j = 1; j < vert.size(); ++j )
            {
                const double p = u * vert[j];
                if ( pmax < p ) pmax = p, jm = j;
            }
            pmax += li % o;
            if ( max < pmax ) max = pmax, im = i, km = jm;
        }
        const double dist = max + a0.x4;
        if ( dist < 1e-4 )
        {
            Conform2d res;
            res.spin = Spin2d ( atan2 ( dt.sina - dt.cosa * a0.x3, dt.cosa + dt.sina * a0.x3 ) );
            res.trans.x = a0.x1;
            res.trans.y = a0.x2;
            return res;
        }
        const Line2d & li = line[im];
        const Vector2d & vm = vert[km];
        const Vector4d cor ( li.norm.x, li.norm.y, li.norm * vm * dt.sina + li.norm % vm * dt.cosa, 1. );
        nat ib = 0;
        double sg;
        for ( i = 1; i <= 4; ++i )
        {
            const Vector4d & v = dt.arr[i];
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
            return Def<Conform2d>();
        }
        const Vector4d & v = dt.arr[ib];
        a0 -= v * ( dist * sg );
        for ( i = 1; i <= 4; ++i )
        {
            if ( i == ib ) continue;
            Vector4d & ai = dt.arr[i];
            ai -= v * ( ( cor * ai ) * sg );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
        si.head = a0.x4;
        heap.down(0);
    }
    return Def<Conform2d>();
}

Def<Conform2d> overlayConvexPolygon2 ( CCArrRef<Vector2d> & vert1, CCArrRef<Vector2d> & vert2 )
{
    nat i;
    Def<Conform2d> res;
// Приведём многоугольники к стандартному виду
    const Def<Vector2d> o1 = centerPlg ( vert1 );
    if ( ! o1.isDef ) return res;
    const Def<Vector2d> o2 = centerPlg ( vert2 );
    if ( ! o2.isDef ) return res;
    DynArray<Vector2d> poly1 ( vert1.size() ), poly2 ( vert2.size() );
    for ( i = 0; i < vert1.size(); ++i ) poly1[i] = vert1[i] - o1;
    for ( i = 0; i < vert2.size(); ++i ) poly2[i] = vert2[i] - o2;
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
// Организация очереди с приоритетом
    const nat n = 40;
    const double step = M_2PI / n;
    DynArray<OCP_data> data ( n );
    MaxHeap<SortItem<double,nat> > heap ( n );
    for ( i = 0; i < n; ++i )
    {
        const double a = i * step;
        OCP_data & d = data[i];
        d.cosa = cos(a);
        d.sina = sin(a);
        d.arr[0] = Vector4d ( 1, 1, 1, 1 );
        d.arr[1] = Vector4d (-1, 0, 0, 0 );
        d.arr[2] = Vector4d ( 0,-1, 0, 0 );
        d.arr[3] = Vector4d ( 0, 0,-1, 0 );
        d.arr[4] = Vector4d ( 0, 0, 0,-1 );
        heap << SortItem<double,nat> ( 1, i );
    }
    res = overlayConvexPolygon ( heap, data, poly1, line );
    if ( res.isDef ) res = ~conf2 * res * conf1;
    return res;
}

void overlayConvexPolygons_test()
{
    Suite<Vector2d> poly1;
    poly1.resize ( 9 );
    randConvexPolygon ( poly1 ) *= 0.6;
    Suite<Vector2d> poly2;
    poly2.resize ( 4 );
    regularPolygon ( poly2 );
drawPolygon ( poly2, 0, 1, 1 );
    DynArray<Line2d> line ( poly2.size() );
    if ( ! points2lines ( poly2, line ) ) return;
double t0 = timeInSec ();
    Vector2d conf;
    bool ok = minMaxPointsConvexPolygonNR ( poly1, poly2, conf );
double t1 = timeInSec ();
    Def<Affin2d> aff = overlayPointsOnConvexPolygon ( poly1, line );
double t2 = timeInSec ();
display << t1-t0 << t2-t1 << NL;
    Suite<Vector2d> tmp;
    if ( ok )
    {
        tmp = poly1;
        tmp += conf;
//display << area ( tmp ) << NL;
drawPolygon ( tmp, 1, 1, 0 );
    }
    if ( aff.isDef )
    {
        tmp = poly1;
        tmp *= aff;
//display << area ( tmp ) << NL;
//drawPolygon ( tmp, 1, 0, 0 );
    }
}

} // end of namespace

void approx2d_test()
{
    drawNewList2d();
//    getCirclePnt_test2();
//    getEllipsePnt_test1();
//    getEllipse_test();
//    getRectangle_test();
//    nearPoint1_test();
//    getLine2sgm3_test();
//    getPlane2_test();
//    getLineSegment_test();
//    getLinePoint_test();
//    getLine1a_test();
//    center_test ();
//    calcLine7_test();
//    getCircle1_test1();
//    getNearPoint2_test1();
//    spline_test3();
    overlayConvexPolygons_test();
    endNewList();
}