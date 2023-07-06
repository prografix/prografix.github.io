
#include "math.h"

#include "../draw.h"

#include "../Shev/tune.h"
#include "../Shev/rand.h"
#include "../Shev/trans.h"
#include "../Shev/Mathem.h"
#include "../Shev/func1d.h"
#include "../Shev/func2d.h"
#include "../Shev/func3d.h"
#include "../Shev/opti2d.h"
#include "../Shev/approx3d.h"
#include "../Shev/Vector3d.h"
#include "../Shev/Vector4d.h"
#include "../Shev/approx2d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/ShevArray.h"
#include "../Shev/WireModel.h"
#include "../Shev/intersect2d.h"
#include "../Shev/intersect3d.h"

#include "display.h"

double timeInSec ();
bool minConvexPolyhedronDiameter ( const Polyhedron & poly, Vector3d & dir, double & d1, double & d2 );
Def<Double<4> > minMaxPointsConvexPolyhedronNR ( const double r, const double a, const double b, CCArrRef<Vector3d> & point, CCArrRef<Facet> & facet );

namespace
{

double diff2 ( CArrRef<Vector3d> p, const Sphere3d & sph )
{
    double e = 0;
    for ( nat i = 0; i < p.size(); ++i )
    {
        const Vector3d d = sph.o - p[i];
        double q = d * d;
        if ( q == 0 ) continue;
        q = sqrt ( q );
        e += _pow2 ( q - sph.r );
    }
    return sqrt ( e / p.size() );
}

void getSphere_test1()
{
    const nat n = 9;
    FixArray<Vector3d, n> point;
    static PRandVector3d vrand;
    static PRand rand;
    Vector3d o = vrand();
    for ( nat i = 0; i < n; ++i )
    {
        point[i] = vrand() * ( 1. + 5e-2 * ( rand() - 0.5 ) );
        point[i].x = fabs ( point[i].x );
        point[i].y = fabs ( point[i].y );
        point[i].z = fabs ( point[i].z );
        point[i] += o;
    }
    Sphere3d sph1 = getSpherePnt22( point );
    Sphere3d sph2 = getSpherePnt2 ( point );
/*    drawPoints ( point, 0, 1, 1 );
    draw ( sph1, 640, 1, 0, 0, 0, VM_WIRE );
    draw ( sph2, 640, 1, 1, 0, 0, VM_WIRE );*/
}

void getSphere_test2()
{
    const nat n = 9;
    FixArray<Vector3d, n> point;
    static PRandVector3d vrand;
    for ( nat j = 0; j < 10; ++j )
    {
        Vector3d o = vrand();
        for ( nat i = 0; i < n; ++i )
        {
            point[i] = vrand() * ( 1 + 1e-5*i );
            point[i] += o;
        }
        Def<Sphere3d> sph = getSpherePnt22 ( point );
        if ( sph.isDef )
        {
            display << norm1 ( sph.o - o ) << NL;
        }
        else
        {
            display << j << "err" << NL;
        }
    }
}

double _norm1 ( CArrRef<Plane3d> arr, const Vector3d & v )
{
    double s = 0;
    for ( nat i = 0; i < arr.size(); ++i ) s += fabs ( arr[i] % v );
    return s;
}

double _norm2 ( CArrRef<Plane3d> arr, const Vector3d & v )
{
    double s = 0;
    for ( nat i = 0; i < arr.size(); ++i ) s += _pow2 ( arr[i] % v );
    return sqrt ( s );
}

double _normU ( CArrRef<Plane3d> arr, const Vector3d & v )
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
Def<Vector3d> getNearPointUg ( CArrRef<Plane3d> plane )
{
    nat nn = plane.size();
    if ( nn < 3 ) return Def<Vector3d>();
    if ( nn == 3 ) return intersection ( plane[0], plane[1], plane[2] );
    nat n1 = nn - 1;
    nat n2 = n1 - 1;
    nat i0, i1, i2, i3;
    for ( i2 = 0; i2 < n2; ++i2 )
    {
        Plane3d line2 = plane[i2];
        for ( i1 = i2+1; i1 < n1; ++i1 )
        {
            Plane3d plane1 = plane[i1];
            const Vector2d p0 = intersection ( plane1, plane2 );
            for ( i0 = i1+1; i0 < nn; ++i0 )
            {
                Plane3d plane0 = plane[i0];
                const Vector3d p1 = intersection ( line2, line0 );
                const Vector3d p2 = intersection ( line0, line1 );
                if ( plane0 % p0 > 0 ) line0 = - line0;
                if ( plane1 % p1 > 0 ) line1 = - line1;
                if ( plane2 % p2 > 0 ) line2 = - line2;
                Vector3d o;
                double r;
                if ( ! circleLLL ( line0, line1, line2, o, r ) )
                    continue;
                double max = 0;
                for ( i3 = 0; i3 < nn; ++i3 )
                {
                    if ( i3 == i0 || i3 == i1 || i3 == i2 ) continue;
                    const double t = fabs ( plane[i3] % o );
                    if ( max < t ) max = t;
                }
                if ( max < r * ( 1. + 1e-6 ) ) return o;
            }
        }
    }
    return Def<Vector3d>();
}
*/
void getNearPoint_test1()
{
    const int n = 9;
    static PRand rand;
    static PRandVector3d vrand;
    FixArray<Plane3d, n> plane;
    double n1[3], n2[3], nu[3];
    for ( int j = 0; j < 1000; ++j )
    {
        const Vector3d o = vrand();
        for ( int i = 0; i < n; ++i )
        {
            plane[i].norm = vrand();
            plane[i].dist = - ( plane[i].norm * o ) + 1e-2 * rand();
        }
        if ( j == 11 )
            j = j;
        const Def<Vector3d> p1 = getNearPoint1 ( plane );
        if ( p1.isDef )
        {
            n1[0] = _norm1 ( plane, p1 );
            n2[0] = _norm2 ( plane, p1 );
            nu[0] = _normU ( plane, p1 );
            if ( n1[0] < n2[0] || n2[0] < nu[0] )
                display << "err" << NL;
        }
        else
        {
            n1[0] = n2[0] = nu[0] = 1e9;
        }
        const Def<Vector3d> p2 = getNearPoint2 ( plane );
        if ( p2.isDef )
        {
            n1[1] = _norm1 ( plane, p2 );
            n2[1] = _norm2 ( plane, p2 );
            nu[1] = _normU ( plane, p2 );
            if ( n1[1] < n2[1] || n2[1] < nu[1] )
                display << "err" << NL;
        }
        else
        {
            n1[1] = n2[1] = nu[1] = 1e9;
        }
        const Def<Vector3d> pu = getNearPointU ( plane );
        if ( pu.isDef )
        {
            n1[2] = _norm1 ( plane, pu );
            n2[2] = _norm2 ( plane, pu );
            nu[2] = _normU ( plane, pu );
            if ( n1[2] < n2[2] || n2[2] < nu[2] )
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
    display << "end" << NL;
}

void getNearPoint_test2()
{
    static PRandVector3d vrand;
    FixArray<Line3d, 12> line;
    for ( nat j = 0; j < 100; ++j )
    {
        const Vector3d o = vrand();
        for ( nat i = 0; i < line.size(); ++i )
        {
            line[i].dir = vrand();
            line[i].point = o + 1e-5 * vrand();
        }
        const Def<Vector3d> p = getNearPoint2 ( line );
        if ( p.isDef )
        {
            if ( qmod ( p - o ) > 1e-10 ) 
            {
                display << ( p - o ) << NL;
            }
        }
        else
        {
            display << "err" << NL;
        }
    }
    display << "end" << NL;
}

Def<Vector3d> getNearPoint1b ( CArrRef<Plane3d> plane, nat & ix1, nat & ix2, nat & ix3 )
{
    const nat n = plane.size();
    if ( n < 3 ) return Def<Vector3d>();
    const nat n1 = n - 1;
    const nat n2 = n - 2;
    double min = 1e300;
    nat i1, i2, i3;
    Def<Vector3d> point;
    for ( i1 = 0; i1 < n2; ++i1 )
    {
        for ( i2 = i1+1; i2 < n1; ++i2 )
        {
            for ( i3 = i2+1; i3 < n; ++i3 )
            {
                const Def<Vector3d> p = intersection ( plane[i1], plane[i2], plane[i3] );
                if ( ! p.isDef ) continue;
                double sum = 0.;
                for ( nat i = 0; i < n; ++i )
                {
                    sum += fabs ( plane[i] % p );
                }
                if ( min > sum )
                {
                    min = sum;
                    point = p;
                    ix1 = i1;
                    ix2 = i2;
                    ix3 = i3;
                }
            }
        }
    }
    return point;
}

void getNearPoint_test3()
{
    static PRand rand;
    static PRandVector3d vrand;
    const nat nn = 60;
    FixArray<Plane3d, nn> plane;
    Vector2d graph[nn];
    nat count = 0;
    for ( nat n = 3; n <= nn; n += 1 )
    {
        double t1 = 0, t2 = 0;
        CArrRef<Plane3d> ref ( plane, 0, n );
        for ( nat j = 0; j < 100; ++j )
        {
            for ( nat i = 0; i < n; ++i )
            {
                plane[i].norm = vrand() * ( 1 + rand() );
                plane[i].dist = rand();
//if ( i & 1 ) plane[i] = plane[i-1];
            }
            unsigned i1, i2, i3, j1, j2, j3;
            double d = timeInSec ();
            const Def<Vector3d> pa = getNearPoint1 ( ref, i1, i2, i3 );
            t1 += timeInSec () - d;
            d = timeInSec ();
            const Def<Vector3d> pb = getNearPoint1b ( ref, j1, j2, j3 );
            t2 += timeInSec () - d;
            if ( pa.isDef != pb.isDef )
            {
                display << "ok1 != ok2" << NL;
            }
            else
            if ( pa.isDef )
            {
                double q = qmod ( pa - pb );
                if ( q > 6e-26 )
                    display << "q =" << q << NL;
            }
        }
            graph[count].x = log ( double(n) );
            graph[count].y = log ( t2 );
//        display << graph[count] << NL;
        display << n << t1 << t2 << NL;
            ++count;
    }
//    Vector2d dir, o;
//    getLine2 ( CArrRef<Vector2d>(graph,count), dir, o );
//    display << "end" << dir.y/dir.x << (dir%o)/dir.x << NL;
}

void getEllipsoid_test ()
{
    static QRandVector3d qrv;
    Polyhedron poly;
    poly.makeEllipsoid ( 40, 0.5, 0.5, 1, qrv ) += 0.5 * qrv();
    draw ( poly, 0, 1, 1, 1, VM_FLAT );
    Def<Ellipsoid3d> elli = getEllipsoidPlg ( poly );
    draw ( elli, 1, 0, 0, 1, VM_SMOOTH );
}

void getCuboid_test ()
{
    static QRandVector3d qrv;
    Polyhedron poly;
    poly.makeEllipsoid ( 40, 0.5, 0.5, 1, qrv ) += 0.5 * qrv();
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    Def<Cuboid3d> para = getCuboid ( poly );
    draw ( para, 1, 0, 0, 1, VM_WIRE );
}
inline
double max4 ( const Plane3d & plane, CArrRef<Vector3d> v )
{
    double max = fabs ( plane % v[0] );
    for ( nat j = 1; j < 4; ++j )
    {
        const double t = fabs ( plane % v[j] );
        if ( max < t ) max = t;
    }
    return max;
}
inline
double mid4 ( const Vector3d & norm, CArrRef<Vector3d> v )
{
    double a = norm * v[0];
    double b = a;
    for ( nat j = 1; j < 4; ++j )
    {
        const double t = norm * v[j];
        if ( a > t ) a = t; else
        if ( b < t ) b = t;
    }
    return -0.5 * ( a + b );
}


Def<Plane3d> getPlaneU1 ( CArrRef<Vector3d> point )
{
    if ( point.size() == 4 )
    {
        nat i, j;
        Plane3d plane;
        double min = 1e300;
        for ( i = 0; i < 4; ++i )
        {
            Vector3d p[3];
            nat k = 0;
            for ( j = 0; j < 4; ++j )
            {
                if ( i == j ) continue;
                p[k] = point[j];
                ++k;
            }
            if ( p[0] == p[1] || p[1] == p[2] || p[2] == p[0] ) continue;
            Plane3d temp ( p[0], p[1], p[2] );
            temp.dist = mid4 ( temp.norm, point );
            const double max = max4 ( temp, point );
            if ( min > max ) min = max, plane = temp;
        }
        if ( min == 1e300 ) return Def<Plane3d>();
        for ( i = 0; i < 3; ++i )
        {
            Vector3d v1 = point[3] - point[i];
            if ( ! v1 ) continue;
            Vector3d v2;
            switch ( i )
            {
            case 0: v2 = point[2] - point[1]; break;
            case 1: v2 = point[2] - point[0]; break;
            case 2: v2 = point[0] - point[1]; break;
            }
            if ( ! v2 ) continue;
            Plane3d temp;
            temp.norm = ( v1 % v2 ).setNorm2();
            if ( ! temp.norm ) continue;
            temp.dist = mid4 ( temp.norm, point );
            const double max = max4 ( temp, point );
            if ( min > max ) min = max, plane = temp;
        }
        return plane;
    }
    if ( point.size() < 3 ) return Def<Plane3d>();
    double d1, d2;
    Plane3d plane;
    Polyhedron poly;
    if ( minConvexPolyhedronDiameter ( convexHull ( point, poly ), plane.norm, d1, d2 ) )
    {
        plane.dist = -0.5 * ( d1 + d2 );
        return plane;
    }
// Находим первую точку
    nat i, i1 = 0;
    double max = qmod ( point[0] );
    for ( i = 1; i < point.size(); ++i )
    {
        const double q = qmod ( point[i] );
        if ( max < q ) max = q, i1 = i;
    }
    const double eps = 1e-24 * max;
// Находим вторую точку
    nat i2 = 0;
    max = 0.;
    for ( i = 1; i < point.size(); ++i )
    {
        const double q = qmod ( point[i] - point[i1] );
        if ( max < q ) max = q, i2 = i;
    }
    if ( max < eps ) return Def<Plane3d>();
// Находим третью точку
    nat i3 = 0;
    max = 0.;
    const Vector3d dir = point[i2] - point[i1];
    const double dd = dir * dir;
    for ( i = 1; i < point.size(); ++i )
    {
        const Vector3d v = point[i] - point[i1];
        const double q = dd * qmod ( v ) - _pow2 ( dir * v );
        if ( max < q ) max = q, i3 = i;
    }
    if ( max < eps ) return Def<Plane3d>();
    return Plane3d ( point[i1], point[i2], point[i3] );
}

void getPlane_test ()
{
    const nat nn = 300;
    static PRandVector3d vrand;
    FixArray<Vector3d, nn> point;
    for ( nat n = 10; n <= nn; n += 10 )
    {
        nat i;
        double time1 = 0, time2 = 0;
        for ( nat k = 0; k < 10; ++k )
        {
            for ( i = 0; i < n; ++i ) point[i] = vrand();
            CArrRef<Vector3d> ref ( point, 0, n );
            double r;
            double t0 = timeInSec ();
            Plane3d plane2 = getPlaneU ( ref, r );
            double t1 = timeInSec ();
            const Def<Plane3d> planeU = getPlaneU1 ( ref );
            double t2 = timeInSec ();
            time1 += t1 - t0;
            time2 += t2 - t1;
            if ( ! planeU.isDef )
            {
                getPlaneU ( ref );
                display << "err U" << NL;
                continue;
            }
            double max2 = 0, maxU = 0;
            for ( i = 0; i < n; ++i )
            {
                const double t2 = _pow2 ( plane2 % point[i] );
                if ( max2 < t2 ) max2 = t2;
                const double tU = _pow2 ( planeU % point[i] );
                if ( maxU < tU ) maxU = tU;
            }
            if ( fabs ( max2 - maxU ) > 1e-14 )
            {
                getPlaneU ( ref );
                display << n << "max2 =" << max2 << "maxU =" << maxU << NL;
            }
        }
display << n << time1 << time2 << NL;
//        if ( sum2 > sumU )
//            display << n << "sum2 =" << sum2 << "sumU =" << sumU << NL;
    }
    display << "end" << NL;
}

void getNearPoint4_3_test1()
{
    static PRand rand;
    static PRandVector3d vrand;
    const nat n = 6;
    FixArray<Plane3d, n> plane;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        //double t = rand();
        plane[i].dist = 0.25;// * t * t;
        Vector3d & v = plane[i].norm;
        v = vrand();
    }
    const Def<Vector3d> p1 = getNearPoint1 ( plane );
    const Def<Vector3d> p2 = getNearPoint10_9( plane );
    const Def<Vector3d> p3 = getNearPoint8_7 ( plane );
    const Def<Vector3d> p4 = getNearPoint6_5 ( plane );
    const Def<Vector3d> p5 = getNearPoint4_3 ( plane );
    const Def<Vector3d> p6 = getNearPoint2 ( plane );
    double q1 = 0, q2 = 0, q3 = 0;
    for ( i = 0; i < n; ++i )
    {
        double t = plane[i] % p1;
        q1 += t * root3 ( root3 ( t ) );
        t = plane[i] % p2;
        q2 += t * root3 ( root3 ( t ) );
        t = plane[i] % p3;
        q3 += t * root3 ( root3 ( t ) );
    }
    display << q1 - q2 << q3 - q2 << NL;
    q1 = 0, q2 = 0, q3 = 0;
    for ( i = 0; i < n; ++i )
    {
        double t = plane[i] % p3;
        q1 += t * root5 ( t );
        t = plane[i] % p4;
        q2 += t * root5 ( t );
        t = plane[i] % p5;
        q3 += t * root5 ( t );
    }
    display << q1 - q2 << q3 - q2 << NL;
    q1 = 0, q2 = 0, q3 = 0;
    for ( i = 0; i < n; ++i )
    {
        double t = plane[i] % p4;
        q1 += t * root3 ( t );
        t = plane[i] % p5;
        q2 += t * root3 ( t );
        t = plane[i] % p6;
        q3 += t * root3 ( t );
    }
    display << q1 - q2 << q3 - q2 << NL << NL;
}

void getNearPoint4_3_test2()
{
    static PRand rand;
    static PRandVector3d vrand;
    const nat n = 4;
    FixArray<Plane3d, n> plane;
    nat i, j;
    for ( j = 0; j < 10000; ++j )
    {
        for ( i = 0; i < n; ++i )
        {
            //double t = rand();
            plane[i].dist = 0.25;// * t * t;
            Vector3d & v = plane[i].norm;
            v = vrand();
        }
        const Def<Vector3d> p1 = getNearPoint1 ( plane );
        const Def<Vector3d> p2 = getNearPoint4_3 ( plane );
        const Def<Vector3d> p3 = getNearPoint6_5 ( plane );
        const Def<Vector3d> p4 = getNearPoint2 ( plane );
        double q1 = 0, q2 = 0, q3 = 0;
        for ( i = 0; i < n; ++i )
        {
            double t = plane[i] % p1;
            q1 += t * root5 ( t );
            t = plane[i] % p3;
            q2 += t * root5 ( t );
            t = plane[i] % p4;
            q3 += t * root5 ( t );
        }
        if ( q2 > q1 || q2 > q3 ) display << j << q1-q2 << q3-q2 << NL;
    }
    display << "end" << NL;
}

static
void minCylinderR3 ( CArrRef<nat> ip, CArrRef<Vector3d> data, Line3d & line, double & q )
{
    const Vector3d & a = data[ip[0]];
    const Vector3d & b = data[ip[1]];
    const Vector3d & c = data[ip[2]];
    const double ab = qmod ( a - b );
    const double bc = qmod ( b - c );
    const double ca = qmod ( c - a );
    if ( ab >= bc && ab >= ca )
    {
        line.dir = ( a - b ) / sqrt ( ab );
        line.point = 0.5 * ( a + c );
        q = line.qdis ( c );
    }
    else
    if ( bc >= ab && bc >= ca )
    {
        line.dir = ( b - c ) / sqrt ( bc );
        line.point = 0.5 * ( a + b );
        q = line.qdis ( a );
    }
    else
    {
        line.dir = ( a - c ) / sqrt ( ca );
        line.point = 0.5 * ( a + b );
        q = line.qdis ( b );
    }
}

static
nat minCircle3 ( CArrRef<Vector2d> data, Circle2d & cir )
{
    const Vector2d & a = data[0];
    const Vector2d & b = data[1];
    const Vector2d & c = data[2];
    const double ab = qmod ( a - b );
    const double bc = qmod ( b - c );
    const double ca = qmod ( c - a );
    if ( ab >= bc && ab >= ca )
    {
        if ( ab >= bc + ca )
        {
            cir.r = 0.5 * sqrt ( ab );
            cir.o = 0.5 * ( a + b );
            return 2;
        }
        else
        {
            cir = circlePPP ( a, b, c );
            return 3;
        }
    }
    else
    if ( bc >= ca )
    {
        if ( bc >= ab + ca )
        {
            cir.r = 0.5 * sqrt ( bc );
            cir.o = 0.5 * ( c + b );
            return 2;
        }
        else
        {
            cir = circlePPP ( a, b, c );
            return 3;
        }
    }
    else
    {
        if ( ca >= bc + ab )
        {
            cir.r = 0.5 * sqrt ( ca );
            cir.o = 0.5 * ( a + c );
            return 2;
        }
        else
        {
            cir = circlePPP ( a, b, c );
            return 3;
        }
    }
    return 0;
}

static
nat minCircle ( CArrRef<Vector2d> data, Circle2d & cir )
{
    const nat n = minCircle3 ( data, cir );
    if ( data.size() == 3 ) return n;
    const Vector2d & p3 = data[3];
    if ( isIntersect ( p3, cir ) ) return n;
    nat im = 0;
    double max = qmod ( data[0] - p3 );
    double t = qmod ( data[1] - p3 );
    if ( max < t ) max = t, im = 1;
    t = qmod ( data[2] - p3 );
    if ( max < t ) max = t, im = 2;
    cir.r = 0.5 * sqrt ( max );
    cir.o = 0.5 * ( data[im] + p3 );
    max = 0;
    nat im2;
    for ( nat i = 0; i < 3; ++i )
    {
        if ( i == im ) continue;
        t = qmod ( data[i] - cir.o );
        if ( max < t ) max = t, im2 = i;
    }
    if ( max <= cir.r * cir.r ) return 2;
    cir = circlePPP ( data[im], data[im2], p3 );
    return 3;
}

static
void minCylinderR ( CArrRef<nat> ip, CArrRef<Vector3d> data, const Vector3d & v, const Vector3d & u,
                    Line3d & line, double & q )
{
    const Vector3d w = ( v % u ).setNorm2();
    OrthoFunc3to2 func3to2 ( w );
    CmbArray<Vector2d, 5> point;
    point.resize ( ip.size() );
    for ( nat k = 0; k < ip.size(); ++k ) point[k] = func3to2 ( data[ip[k]] );
    const Circle2d cir = minCircleAroundPoints ( point );
    const double r2 = cir.r * cir.r;
    if ( q > r2 )
    {
        OrthoFunc2to3 func2to3 ( Plane3d ( w, 0 ) );
        line.point = func2to3 ( cir.o );
        line.dir = w;
        q = r2;
    }
}

// Уточнение для 4-х точек
static
void minCylinderR4 ( CArrRef<nat> ip, CArrRef<Vector3d> data, Line3d & line, double & q )
{
    if ( ip.size() != 4 ) return;
    nat i, im = 0;
    double max = line.qdis ( data[ip[0]] );
    for ( i = 1; i < ip.size(); ++i )
    {
        const double t = line.qdis ( data[ip[i]] );
        if ( max < t ) max = t, im = i;
    }
    const Vector3d o = data[ip[im]];
    Vector3d v = ( line.project ( o ) - o ) / max;
    Vector3d u = line.dir % v;
    CmbSuite<Vector3d, 3> temp;
    for ( i = 0; i < ip.size(); ++i ) if ( i != im ) temp.inc() = data[ip[i]] - o;
// Находим приближённое значение лямбда-множителей
    SMatrix<double, 6, 5> A;
    FixArray<double, 6> r;
    A[0][0] = u.x; A[0][1] =  v.x; r[0] = - v.x;
    A[1][0] = u.y; A[1][1] =  v.y; r[1] = - v.y;
    A[2][0] = u.z; A[2][1] =  v.z; r[2] = - v.z;
    A[3][0] = v.x; A[3][1] = -u.x; r[3] = - u.x;
    A[4][0] = v.y; A[4][1] = -u.y; r[4] = - u.y;
    A[5][0] = v.z; A[5][1] = -u.z; r[5] = - u.z;
    for ( i = 2; i < 5; ++i )
    {
        const Vector3d & pi = temp[i-2];
        const double x = pi * v - 1;
        const double y = pi * u;
        A[0][i] = x * pi.x;
        A[1][i] = x * pi.y;
        A[2][i] = x * pi.z;
        A[3][i] = y * pi.x;
        A[4][i] = y * pi.y;
        A[5][i] = y * pi.z;
    }
    FixArray<double, 6> la;
    lss_h ( A, r, la );
// Вычисляем параметры преобразования
    FixArray2<double, 11, 12> M; 
    double d[11];
    for ( nat k = 0; k < 9; ++k )
    {
        d[0] = v.x + la[0] * u.x + la[1] * v.x;
        d[1] = v.y + la[0] * u.y + la[1] * v.y;
        d[2] = v.z + la[0] * u.z + la[1] * v.z;
        d[3] = u.x + la[0] * v.x - la[1] * u.x;
        d[4] = u.y + la[0] * v.y - la[1] * u.y;
        d[5] = u.z + la[0] * v.z - la[1] * u.z;
        d[6] = v * u;
        d[7] = 0.5 * ( v * v - u * u );
        double func1 = 0;
        for ( i = 0; i < 3; ++i )
        {
            const Vector3d & pi = temp[i];
            const double x = pi * v - 1;
            const double y = pi * u;
            const double a = la[i+2];
            d[0] += a * x * pi.x;
            d[1] += a * x * pi.y;
            d[2] += a * x * pi.z;
            d[3] += a * y * pi.x;
            d[4] += a * y * pi.y;
            d[5] += a * y * pi.z;
            func1 += _pow2 ( d[i+8] = 0.5 * ( x * x + y * y - 1 ) );
        }
        double func2 = _pow2 ( d[0] ) + _pow2 ( d[1] ) + _pow2 ( d[2] ) +
                       _pow2 ( d[3] ) + _pow2 ( d[4] ) + _pow2 ( d[5] );
//display << k << func1 << func2 << NL;
        if ( func1 < 1e-24 && func2 < 1e-18 ) break;
        M.fill(0);
        M[0][0] = M[1][1] = M[2][2] = 1 + la[1];
        M[3][3] = M[4][4] = M[5][5] = 1 - la[1];
        M[3][0] = M[0][3] = M[4][1] = M[1][4] = M[5][2] = M[2][5] = la[0];

        M[6][0] = M[0][6] = u.x;
        M[6][1] = M[1][6] = u.y;
        M[6][2] = M[2][6] = u.z;
        M[6][3] = M[3][6] = v.x;
        M[6][4] = M[4][6] = v.y;
        M[6][5] = M[5][6] = v.z;
        M[7][0] = M[0][7] = v.x;
        M[7][1] = M[1][7] = v.y;
        M[7][2] = M[2][7] = v.z;
        M[7][3] = M[3][7] = -u.x;
        M[7][4] = M[4][7] = -u.y;
        M[7][5] = M[5][7] = -u.z;

        M[0][11] = la[0] * u.x + la[1] * v.x;
        M[1][11] = la[0] * u.y + la[1] * v.y;
        M[2][11] = la[0] * u.z + la[1] * v.z;
        M[3][11] = la[0] * v.x - la[1] * u.x;
        M[4][11] = la[0] * v.y - la[1] * u.y;
        M[5][11] = la[0] * v.z - la[1] * u.z;
        M[6][11] = v * u;
        M[7][11] = 0.5 * ( v * v - u * u );
        for ( i = 0; i < 3; ++i )
        {
            const Vector3d & pi = temp[i];
            const double x = pi * v;
            const double y = pi * u;
            const double a = la[i+2];
            M[0][0] += a * pi.x * pi.x;
            M[0][1] += a * pi.y * pi.x;
            M[0][2] += a * pi.z * pi.x;
            M[1][1] += a * pi.y * pi.y;
            M[1][2] += a * pi.z * pi.y;
            M[2][2] += a * pi.z * pi.z;
            M[3][3] += a * pi.x * pi.x;
            M[3][4] += a * pi.y * pi.x;
            M[3][5] += a * pi.z * pi.x;
            M[4][4] += a * pi.y * pi.y;
            M[4][5] += a * pi.z * pi.y;
            M[5][5] += a * pi.z * pi.z;
            const nat j = i + 8;
            M[0][j] += ( x - 1 ) * pi.x;
            M[1][j] += ( x - 1 ) * pi.y;
            M[2][j] += ( x - 1 ) * pi.z;
            M[3][j] += y * pi.x;
            M[4][j] += y * pi.y;
            M[5][j] += y * pi.z;
            M[0][11] += a * x * pi.x;
            M[1][11] += a * x * pi.y;
            M[2][11] += a * x * pi.z;
            M[3][11] += a * y * pi.x;
            M[4][11] += a * y * pi.y;
            M[5][11] += a * y * pi.z;
            M[j][11] = 0.5 * ( x * x + y * y );
        }
        M[1][0] = M[0][1];
        M[2][0] = M[0][2];
        M[2][1] = M[1][2];
        M[4][3] = M[3][4];
        M[5][3] = M[3][5];
        M[5][4] = M[4][5];
        for ( i = 0; i < 3; ++i )
        {
            const nat j = i + 8;
            M[j][0] = M[0][j];
            M[j][1] = M[1][j];
            M[j][2] = M[2][j];
            M[j][3] = M[3][j];
            M[j][4] = M[4][j];
            M[j][5] = M[5][j];
        }/*
        for ( i = 0; i < 11; ++i )
        {
            CArrRef<double> r = M[i];
            double sum = r[0] * v.x + r[1] * v.y + r[2] * v.z + 
                         r[3] * u.x + r[4] * u.y + r[5] * u.z - r[11];
            for ( j = 6; j < 11; ++j ) sum += r[j] * la[j-6];
            if ( fabs ( sum - d[i] ) > 1e-9 )
            display << i << sum << d[i] << NL;
        }*/
        slu_gauss ( M );
        v.x = M[0][11];
        v.y = M[1][11];
        v.z = M[2][11];
        u.x = M[3][11];
        u.y = M[4][11];
        u.z = M[5][11];
        for ( i = 0; i < 5; ++i ) la[i] = M[i+6][11];
    }
    minCylinderR ( ip, data, v, u, line, q );
}

// Уточнение для 5-и точек

static
void minCylinderR5 ( CArrRef<nat> ip, CArrRef<Vector3d> data, Line3d & line, double & q )
{
    Vector3d u ( 0, line.dir.z, -line.dir.y );
    double s = u * u;
    if ( s == 0 )
    {
        u.y = sqrt ( 1 / q );
    }
    else
    {
        s = sqrt ( s * q );
        u.y /= s;
        u.z /= s;
    }
    Vector3d v = u % line.dir;
    double dx = - ( v * line.point );
    double dy = - ( u * line.point );
    const nat n = ip.size();
    const nat n1 = n + 1;
    const nat nn = n + 9;
    nat i;
    /*double d[5];
    for ( i = 0; i < n; ++i )
    {
        const Vector3d & w = data[ip[i]];
        double x = v * w + dx;
        double y = u * w + dy;
        d[i] = x*x + y*y - 1;
//        display << q << x*x + y*y << NL;
    }*/
// Находим приближённое значение лямбда-множителей
    CmbMatrix<double, 7, 7> A ( 7, n+2 );
    FixArray<double, 7> r;
    r[0] = v.x;
    r[1] = v.y;
    r[2] = v.z;
    r[3] = 0;
    r[4] = u.y;
    r[5] = u.z;
    r[6] = 0;
    A[0][n] = -u.x; A[0][n1] = -v.x;
    A[1][n] = -u.y; A[1][n1] = -v.y;
    A[2][n] = -u.z; A[2][n1] = -v.z;
    A[4][n] = -v.y; A[4][n1] =  u.y;
    A[5][n] = -v.z; A[5][n1] =  u.z;
    A[3][n] = A[3][n1] = A[6][n] = A[6][n1] = 0;
    for ( i = 0; i < n; ++i )
    {
        const Vector3d & w = data[ip[i]];
        const double x = v * w + dx;
        const double y = u * w + dy;
        A[0][i] = x * w.x;
        A[1][i] = x * w.y;
        A[2][i] = x * w.z;
        A[3][i] = x;
        A[4][i] = y * w.y;
        A[5][i] = y * w.z;
        A[6][i] = y;
    }
    FixArray<double, 7> la;
    lss_h ( A, r, la );
/*    for ( i = 0; i < n; ++i )
    {
        const Vector3d & w = data[ip[i]];
        const double x = v * w + dx;
        const double y = u * w + dy;
        r[0] -= la[i] * x * w.x;
        r[1] -= la[i] * x * w.y;
        r[2] -= la[i] * x * w.z;
        r[3] -= la[i] * x;
        r[4] -= la[i] * y * w.y;
        r[5] -= la[i] * y * w.z;
        r[6] -= la[i] * y;
    }
    r[0] += la[n] * u.x + la[n1] * v.x;
    r[1] += la[n] * u.y + la[n1] * v.y;
    r[2] += la[n] * u.z + la[n1] * v.z;
    r[4] += la[n] * v.y - la[n1] * u.y;
    r[5] += la[n] * v.z - la[n1] * u.z;
*/
    double sum = 0;
    for ( i = 0; i < n; ++i )
    {
        const Vector3d & w = data[ip[i]];
        const double x = v * w + dx;
        const double y = u * w + dy;
        sum += fabs ( la[i] * ( x * x + y * y - 1 ) );
    }
    if ( sum < 1e-12 )
    {
        return;
    }
    for ( i = 0; i < n; ++i ) la[i] = la[i] > 0 ? sqrt ( 2 * la[i] ) : 0;
    //for ( i = 0; i < n; ++i ) la[i] = sqrt ( fabs ( 2 * la[i] ) );
// Вычисляем параметры преобразования
    double f[14];
    CmbArray2<double, 14*15> M ( n+9, n+10 );
    for ( nat k = 0; k < 40; ++k )
    {
        f[0] = v.x + la[n1] * v.x;
        f[1] = v.y + la[n1] * v.y + la[n] * u.y;
        f[2] = v.z + la[n1] * v.z + la[n] * u.z;
        f[3] = f[6] = 0;
        f[4] = u.y + la[n] * v.y - la[n1] * u.y;
        f[5] = u.z + la[n] * v.z - la[n1] * u.z;
        f[12] = v * u;
        f[13] = 0.5 * ( v * v - u * u );
        for ( i = 0; i < n; ++i )
        {
            const double la2 = 0.5 * la[i] * la[i];
            const Vector3d & w = data[ip[i]];
            const double x = v * w + dx;
            const double y = u * w + dy;
            f[0] -= la2 * x * w.x;
            f[1] -= la2 * x * w.y;
            f[2] -= la2 * x * w.z;
            f[3] -= la2 * x;
            f[4] -= la2 * y * w.y;
            f[5] -= la2 * y * w.z;
            f[6] -= la2 * y;
            f[7+i] = -0.5 * la[i] * ( x * x + y * y - 1 );
        }
        const double func1 = _pow2 ( f[0] ) + _pow2 ( f[1] ) + _pow2 ( f[2] ) +
                       _pow2 ( f[3] ) + _pow2 ( f[4] ) + _pow2 ( f[5] ) + _pow2 ( f[6] );
        const double func2 = _pow2 ( f[7] ) + _pow2 ( f[8] ) + _pow2 ( f[9] ) +
                       _pow2 ( f[10] ) + _pow2 ( f[11] );
        const double func3 = _pow2 ( f[12] ) + _pow2 ( f[13] );
//display << k << func1 << func2 << func3 << NL;
        if ( func1 < 1e-24 && func2 < 1e-18 && func3 < 1e-18 ) break;
        M.fill(0);
        M[0][0] = M[1][1] = M[2][2] = 1 + la[n1];
        M[4][4] = M[5][5] = 1 - la[n1];

        M[1][4] = M[4][1] = la[n];
        M[2][5] = M[5][2] = la[n];

        M[0][7+n1] = M[7+n1][0] = v.x;
        M[1][7+n1] = M[7+n1][1] = v.y;
        M[1][7+n] = M[7+n][1] = u.y;
        M[2][7+n1] = M[7+n1][2] = v.z;
        M[2][7+n] = M[7+n][2] = u.z;
        M[4][7+n1] = M[7+n1][4] = -u.y;
        M[4][7+n] = M[7+n][4] = v.y;
        M[5][7+n1] = M[7+n1][5] = -u.z;
        M[5][7+n] = M[7+n][5] = v.z;

        M[0][nn] = la[n1] * v.x;
        M[1][nn] = la[n1] * v.y + la[n] * u.y;
        M[2][nn] = la[n1] * v.z + la[n] * u.z;
        M[4][nn] = la[n] * v.y - la[n1] * u.y;
        M[5][nn] = la[n] * v.z - la[n1] * u.z;
        M[7+n][nn] = v.y * u.y + v.z * u.z;
        M[7+n1][nn] = 0.5 * ( v * v - u * u );
        for ( i = 0; i < n; ++i )
        {
            const double la2 = 0.5 * la[i] * la[i];
            const Vector3d & w = data[ip[i]];
            const double x = v * w + dx;
            const double y = u * w + dy;
            M[0][0] -= la2 * w.x * w.x;
            M[0][1] -= la2 * w.x * w.y;
            M[0][2] -= la2 * w.x * w.z;
            M[0][3] -= la2 * w.x;
            M[0][7+i] = M[7+i][0] = - la[i] * x * w.x;
            M[0][nn] += la[i] * M[0][7+i];
            M[1][1] -= la2 * w.y * w.y;
            M[1][2] -= la2 * w.y * w.z;
            M[1][3] -= la2 * w.y;
            M[1][7+i] = M[7+i][1] = - la[i] * x * w.y;
            M[1][nn] += la[i] * M[1][7+i];
            M[2][2] -= la2 * w.z * w.z;
            M[2][3] -= la2 * w.z;
            M[2][7+i] = M[7+i][2] = - la[i] * x * w.z;
            M[2][nn] += la[i] * M[2][7+i];
            M[3][3] -= la2;
            M[3][7+i] = M[7+i][3] = - la[i] * x;
            M[3][nn] += la[i] * M[3][7+i];
            M[4][4] -= la2 * w.y * w.y;
            M[4][5] -= la2 * w.y * w.z;
            M[4][6] -= la2 * w.y;
            M[4][7+i] = M[7+i][4] = - la[i] * y * w.y;
            M[4][nn] += la[i] * M[4][7+i];
            M[5][5] -= la2 * w.z * w.z;
            M[5][6] -= la2 * w.z;
            M[5][7+i] = M[7+i][5] = - la[i] * y * w.z;
            M[5][nn] += la[i] * M[5][7+i];
            M[6][6] -= la2;
            M[6][7+i] = M[7+i][6] = - la[i] * y;
            M[6][nn] += la[i] * M[6][7+i];
            M[7+i][7+i] = - 0.5 * ( x * x + y * y - 1 );
            M[7+i][nn] = - ( x * x + y * y ) * la[i];
        }
        M[1][0] = M[0][1];
        M[2][0] = M[0][2];
        M[2][1] = M[1][2];
        M[3][0] = M[0][3];
        M[3][1] = M[1][3];
        M[3][2] = M[2][3];
        M[5][4] = M[4][5];
        M[6][4] = M[4][6];
        M[6][5] = M[5][6];
        /*for ( i = 0; i < nn; ++i )
        {
            CArrRef<double> r = M[i];
            double sum = r[0] * v.x + r[1] * v.y + r[2] * v.z + r[3] * dx
                       + r[4] * u.y + r[5] * u.z + r[6] * dy - r[nn];
            for ( nat j = 0; j < n+2; ++j ) sum  += r[j+7] * la[j];
            if ( fabs ( sum - f[i] ) > 1e-9 )
            display << i << f[i] << sum << NL;
        }*/
        slu_gauss ( M );
        v.x = M[0][nn];
        v.y = M[1][nn];
        v.z = M[2][nn];
        dx  = M[3][nn];
        u.y = M[4][nn];
        u.z = M[5][nn];
        dy  = M[6][nn];
        for ( i = 0; i < n+2; ++i ) la[i] = M[i+7][nn];
    }
    /*for ( i = 0; i < n; ++i )
    {
        const Vector3d & w = data[ip[i]];
        double x = v * w + dx;
        double y = u * w + dy;
        d[i] = x*x + y*y - 1;
        if ( d[i] > 0.01 ) display << "d["<<i<<"] =" << d[i] << NL;
    }*/
    minCylinderR ( ip, data, v, u, line, q );
}

static
void minCylinderR4_5 ( CArrRef<nat> ip, CArrRef<Vector3d> data, Line3d & line, double & q )
{
    nat i, j, nn = 0;
    double min = 1e300;
    CmbSuite<Vector2d, 5> point;
    for ( i = 1; i < ip.size(); ++i )
    {
        const Vector3d & a = data[ip[i]];
        for ( j = 0; j < i; ++j )
        {
            const Vector3d v = ( a - data[ip[j]] ).setNorm2();
            OrthoFunc3to2 func3to2 ( v );
            point.resize();
            for ( nat k = 0; k < ip.size(); ++k ) 
                if ( k != i ) point.inc() = func3to2 ( data[ip[k]] );
            Circle2d cir;
            const nat n = minCircle ( point, cir );
            if ( min > cir.r )
            {
                nn = n;
                min = cir.r;
                line.dir = v;
                OrthoFunc2to3 func2to3 ( Plane3d ( line.dir, 0 ) );
                line.point = func2to3 ( cir.o );
            }
        }
    }
    q = min * min;
    if ( ip.size() == 4 )
    {
        if ( nn == 2 ) return;
        minCylinderR4 ( ip, data, line, q ); // Уточнение
    }
    else
        minCylinderR5 ( ip, data, line, q ); // Уточнение
}

Def<Line3d> getLineU1 ( CArrRef<Vector3d> data, double & r )
{
    Def<Line3d> res;
    const nat n = data.size();
    if ( n == 0 ) return res;
    if ( n == 1 )
    {
m1:     res.point = data[0];
        res.dir = Vector3d ( 0, 0, 1 );
        res.isDef = true;
        r = 0;
        return res;
    }
    CmbSuite<nat, 5> ir, ip;
    if ( n == 2 )
    {
        ip.resize ( 2 );
        ip[0] = 0;
        ip[1] = 1;
m2:     Vector3d v = data[ip[1]] - data[ip[0]];
        res.point = 0.5 * ( data[ip[0]] + data[ip[1]] );
        res.dir = !! v ? v.setNorm2() : Vector3d ( 0, 0, 1 );;
        res.isDef = true;
        r = 0;
        return res;
    }
// Поиск начальных трёх точек
    nat i, im = 0;
    double max = qmod ( data[0] );
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[i] );
        if ( max < t ) max = t, im = i;
    }
    ip.inc() = im;
    const Vector3d & p0 = data[im];
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( data[i] - p0 );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) goto m1;
    ip.inc() = im;
    Vector3d v = data[ip[1]] - p0;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( ( data[i] - p0 ) % v );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) goto m2;
    ip.inc() = im;
    Line3d line;
    double q;
    minCylinderR3 ( ip, data, line, q );
    if ( q == 0 ) goto m2;
// Проверка остальных точек
    for (;;)
    {
        max = 0;
        for ( i = 0; i < n; ++i )
        {
            const double t = line.qdis ( data[i] );
            if ( max < t ) max = t, im = i;
        }
        if ( max < ( 1 + 1e-12 ) * q ) break;
        for ( i = 0; i < ip.size(); ++i )
            if ( im == ip[i] ) goto m3;
        Line3d temp;
        double s;
        const Vector3d v1 = data[im];
        ir.resize ( 1 );
        ir[0] = im;
        for ( nat i2 = 1; i2 < ip.size(); ++i2 )
        {
            const Vector3d & v2 = data[ip[i2]];
            if ( ir.size() > 2 && temp.qdis ( v2 ) <= s ) continue;
            ir.resize(2)[1] = ip[i2];
            for ( nat i3 = 0; i3 < i2; ++i3 )
            {
                const Vector3d & v3 = data[ip[i3]];
                if ( ir.size() > 2 && temp.qdis ( v3 ) <= s ) continue;
                ir.resize(3)[2] = ip[i3];
                minCylinderR3 ( ir, data, temp, s );
                for ( nat i4 = 0; i4 < i3; ++i4 )
                {
                    const Vector3d & v4 = data[ip[i4]];
                    if ( temp.qdis ( v4 ) <= s ) continue;
                    ir.resize(4)[3] = ip[i4];
                    minCylinderR4_5 ( ir, data, temp, s );
                    for ( nat i5 = 0; i5 < i4; ++i5 )
                    {
                        const Vector3d & v5 = data[ip[i5]];
//drawPoint ( v5, 1, 1, 0 );
                        if ( temp.qdis ( v5 ) <= s ) continue;
                        ir.resize(5)[4] = ip[i5];
                        minCylinderR4_5 ( ir, data, temp, s );
                    }
                }
            }
        }
        if ( s <= q )
        {
            return res;
        }
        ip = ir;
        q = s;
        line = temp;
    }
m3:
    r = sqrt ( q );
    return line;
}

void getLinePoint_test1()
{;
    static PRandPoint3d rand;
    const nat n = 8;
    FixArray<Vector3d, n> point;
    point << rand;
    double r1, r2;
double t0 = timeInSec ();
    Def<Line3d> line1 = getLineU1 ( point, r1 );
    if ( ! line1.isDef )
    {
        display << "error1" << NL;
        return;
    }
double t1 = timeInSec ();
    Line3d line2 = getLineU( point, r2 );
double t2 = timeInSec ();
    nat i;
    double max1 = 0, max2 = 0;
    for ( i = 0; i < n; ++i )
    {
        double q1 = line1.qdis ( point[i] );
        if ( max1 < q1 ) max1 = q1;
        double q2 = line2.qdis ( point[i] );
        if ( max2 < q2 ) max2 = q2;
    }
    max1 = sqrt ( max1 );
    max2 = sqrt ( max2 );
//    display << t1 - t0 << t2 - t1 << NL;
//    display << "r" << r1 << r2 << NL;
//    display << "m" << max1 << max2 << NL;
    display << r1 - r2 << max1 - max2 << NL;
}

void minMaxPointsConvexPolyhedronNR_test()
{
    static PRand rand;
    static QRandVector3d qrv;
    Polyhedron poly1, poly2;
    //poly2.makeCuboid ( 0.35+0.5*rand(), 0.35+0.5*rand(), 0.35+0.5*rand() );
    poly1.makeEllipsoid ( 10, 0.5, 0.75, 1, qrv );
    poly2.makeEllipsoid ( 10, 0.5, 0.75, 1, qrv );
    Vector3d res;
    res = overlayConvexPolyhedronsNR ( poly1, poly2 );
    poly1 += res;
    draw ( poly1, 1, 1, 0, 1, VM_WIRE );
    draw ( poly2, 0, 1, 1, 1, VM_WIRE );
}

Def<Double<5> > minMaxPointsConvexPolyhedron1R ( const double r, const double a, const double b, CCArrRef<Vector3d> & point, CCArrRef<Facet> & facet ) 
{
    const nat N = 5;
    Double<N> arr[N+1], cor;
    arr[0].init ( r, r, r, r, 1 );
    arr[1].init ( -1., 0., 0., 0., 0. );
    arr[2].init ( 0., -1., 0., 0., 0. );
    arr[3].init ( 0., 0., -1., 0., 0. );
    arr[4].init ( 0., 0., 0., -1., 0. );
    arr[5].init ( 0., 0., 0., 0., -1. );
    const double eps = 1e-9 * r;
    for ( nat k = 0; k < 100; ++k )
    {
        Double<N> & a0 = arr[0];
        const Vector3d o ( a0.d1, a0.d2, a0.d3 );
        const Vector2d w ( a + b * a0.d4, b - a * a0.d4 );
        double max = -1;
        nat i, im, km;
        for ( i = 0; i < facet.size(); ++i )
        {
            const Plane3d & li = facet[i].plane;
            const Vector3d u ( w.x * li.norm.x + w.y * li.norm.y, w.x * li.norm.y - w.y * li.norm.x, li.norm.z );
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
        const double dist = max + a0.d0;
        if ( dist < eps )
            return a0;
        const Plane3d & li = facet[im].plane;
        const Vector3d & vm = point[km];
        cor.init ( 1, li.norm.x, li.norm.y, li.norm.z, ( li.norm.x * vm.x + li.norm.y * vm.y ) * b + ( li.norm.x * vm.y - li.norm.y * vm.x ) * a );
        if ( ! cutSimplex ( arr, cor, dist ) )
        {
            return Def<Double<N> >();
        }
    }
    return Def<Double<N> >();
}

Def<Conform3d> minMaxPointsConvexPolyhedron1R1 ( CCArrRef<Vector3d> & point, const Polyhedron & poly )
{
    Def<Conform3d> res;
    const nat np = point.size();
    const nat nv = poly.vertex.size();
    if ( np < 3 || nv < 3 ) return res;
// Определяем общие габариты
    Def<Segment3d> dim = dimensions ( point );
    Def<Segment3d> dim2 = dimensions ( poly.vertex );
    if ( dim.a.x > dim2.a.x ) dim.a.x = dim2.a.x;
    if ( dim.b.x < dim2.b.x ) dim.b.x = dim2.b.x;
    if ( dim.a.y > dim2.a.y ) dim.a.y = dim2.a.y;
    if ( dim.b.y < dim2.b.y ) dim.b.y = dim2.b.y;
    if ( dim.a.z > dim2.a.z ) dim.a.z = dim2.a.z;
    if ( dim.b.z < dim2.b.z ) dim.b.z = dim2.b.z;
    const double r = norm2 ( dim.b - dim.a );
// Инициализация области допустимых преобразований
    const nat n = 40;
    const double step = M_2PI / n;
    double max = -1e9;
    Vector2d v;
    for ( nat i = 0; i < n; ++i )
    {
        const double a = i * step;
        const double cosa = cos(a);
        const double sina = sin(a);
        Def<Double<5> > d = minMaxPointsConvexPolyhedron1R ( r, cosa, sina, point, poly.facet );
        if ( d.isDef && max < d.d0 )
        {
            max = d.d0;
            v.x = cosa + sina * d.d4;
            v.y = sina - cosa * d.d4;
        }
    }
    if ( max > -1e9 )
    {
        v.setNorm2();
        Def<Double<4> > d4 = minMaxPointsConvexPolyhedronNR ( r, v.x, v.y, point, poly.facet );
        res.spin = Spin3d ( Vector3d(0,0,1), atan2 ( v.y, v.x ) );
        res.trans.x = d4.d1;
        res.trans.y = d4.d2;
        res.trans.z = d4.d3;
        res.isDef = true;
    }
    return res;
}
nat nnn = 0;

} // end of namespace

void minMaxPointsConvexPolyhedron_test()
{
    static PRand rand;
    static QRandVector3d qrv;
    double a = 0.35+0.5*rand();
    double b = 0.35+0.5*rand();
    double c = 0.35+0.5*rand();
    Polyhedron poly1, poly2, poly3;
    poly2.makeEllipsoid ( a, b, c, 100 );
    poly1.makeEllipsoid ( a, b, c, 200 );
    cut ( poly1, Plane3d ( Vector3d ( 1, 1, 0 ), -0.7 ), poly3 );
    Spin3d spin ( Vector3d ( 0, 0, 1 ), rand() );
    poly3 *= spin;
    poly3 += qrv();
    double t1 = timeInSec();
    Def<Conform3d> res = minMaxPointsConvexPolyhedron1R ( poly3.vertex, poly2 );
    double t2 = timeInSec();
    poly3 *= res;
    double max = 0;
    for ( nat i = 0; i < poly2.facet.size(); ++i )
    {
        const Plane3d & plane = poly2.facet[i].plane;
        for ( nat j = 0; j < poly3.vertex.size(); ++j )
        {
            _maxa ( max, plane % poly3.vertex[j] );
        }
    }
    display << max << t2-t1 << NL;
    draw ( poly3, 1, 1, 0, 1, VM_WIRE );
    draw ( poly2, 0, 1, 1, 1, VM_WIRE );
}

void minMaxPointsConvexPolyhedron_test2()
{
    static PRand rand;
    static QRandVector3d qrv;
    Polyhedron poly1, poly2, poly3;
    nat km;
    double max = 0;
    nnn = 0;
    double t1 = timeInSec();
    for ( nat k = 0; k < 1000; ++k )
    {
        double a = 0.35+0.5*rand();
        double b = 0.35+0.5*rand();
        double c = 0.35+0.5*rand();
        Polyhedron poly1, poly2, poly3;
        poly2.makeEllipsoid ( a, b, c, 100 );
        poly1.makeEllipsoid ( a, b, c, 200 );
        cut ( poly1, Plane3d ( Vector3d ( 1, 1, 0 ), -0.7 ), poly3 );
        Spin3d spin ( Vector3d ( 0, 0, 1 ), rand() );
        poly3 *= spin;
        poly3 += qrv();
        Def<Conform3d> res = minMaxPointsConvexPolyhedron1R ( poly3.vertex, poly2 );
        poly3 *= res;
        for ( nat i = 0; i < poly2.facet.size(); ++i )
        {
            const Plane3d & plane = poly2.facet[i].plane;
            for ( nat j = 0; j < poly3.vertex.size(); ++j )
            {
                if ( _maxa (max, plane % poly3.vertex[j]) ) km = k;
            }
        }
        if ( 0 )
        {
            draw ( poly3, 1, 1, 0, 1, VM_WIRE );
            draw ( poly2, 0, 1, 1, 1, VM_WIRE );
        }
    }
    double t2 = timeInSec();
    display << max << km << t2-t1 << nnn << NL;
}

void approx3d_test ()
{
    drawNewList2d();
//    getSphere_test2();
//    getNearPoint_test3();
//    getNearPoint_test2();
//    getPlane_test();
//    getEllipsoid_test();
//    getCuboid_test();
//    getNearPoint4_3_test1();
//    getLinePoint_test1();
    minMaxPointsConvexPolyhedron_test2();
    endNewList();
}