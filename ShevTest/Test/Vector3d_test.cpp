
#include "math.h"

#include "../Shev/template.h"
#include "../Shev/Vector3d.h"
#include "../Shev/func3d.h"
#include "../Shev/typedef.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/rand.h"

#include "display.h"

double timeInSec();

namespace
{

static bool makeNormal ( const Vector3d & a, const Vector3d & b, Vector3d & c )
{
    const double ax = fabs ( a.x );
    const double ay = fabs ( a.y );
    const double az = fabs ( a.z );
    if ( ax >= ay && ax >= az )
    {
    }
    else
    if ( ay >= az )
    {
    }
    else
    {
    }
    return false;
}

Plane3d makePlane ( const Vector3d & a, const Vector3d & b, const Vector3d & c )
{
    Plane3d plane;
    const Vector3d ab = a - b;
    const double d1 = ab * ab;
    const Vector3d bc = b - c;
    const double d2 = bc * bc;
    const Vector3d ca = c - a;
    const double d3 = ca * ca;
    if ( d1 >= d2 && d1 >= d3 )
    {
        if ( ! makeNormal ( bc, ca, plane.norm ) ) plane.norm = ab.perpendicular();
    }
    else
    if ( d2 >= d3 )
    {
        if ( ! makeNormal ( ca, ab, plane.norm ) ) plane.norm = bc.perpendicular();
    }
    else
    {
        if ( ! makeNormal ( ab, bc, plane.norm ) ) plane.norm = ca.perpendicular();
    }
    const double da = plane.norm * a;
    const double db = plane.norm * b;
    const double dc = plane.norm * c;
    plane.dist = -0.5 * ( _min ( da, db, dc ) + _max ( da, db, dc ) );
    return plane;
}

void plane3d_test ()
{
    static PRandPoint3d rand;
    Vector3d a = rand();
    Vector3d b = rand();
    Vector3d c = rand();
    Plane3d plane ( a, b, c );
    double da = fabs ( plane % a );
    double db = fabs ( plane % b );
    double dc = fabs ( plane % c );
    double t1 = _max ( da, db, dc );
//    display << t1 << t2 << NL;
/*
    a = rand();
    b = plane.mirror ( a );
    c = plane.mirror ( b );
    display << "mirror" << norm2 ( a - b ) << norm2 ( a - c ) << NL;
*/
    Plane3d plane2 ( rand(), rand(), rand() );
    plane2.norm *= 3;
    plane2.dist *= 3;
    plane.norm *= 0.5;
    plane.dist *= 0.5;
    plane = plane2.mirror ( plane );
    a = plane2.mirror ( a );
    b = plane2.mirror ( b );
    c = plane2.mirror ( c );
    display << "mirror plane" << plane % a << plane % b << plane % c << NL;
}

void affin3d_test ()
{
    static PRandVector3d rand;
    Affin3d a ( rand(), rand(), rand(), rand() );
    Affin3d b ( rand(), rand(), rand(), rand() );
    Affin3d c = a * b;
    Vector3d v = rand();
    Vector3d v1 = c ( v );
    Vector3d v2 = a ( b ( v ) );
    display << qmod ( v1 - v2 ) << NL;
    a = ~b;
    c = a * b;
    display << norm1 ( c.t.x ) << norm1 ( c.t.y ) << norm1 ( c.t.z ) << NL;
    c = b * a;
    display << norm1 ( c.t.x ) << norm1 ( c.t.y ) << norm1 ( c.t.z ) << NL;
    double time = 0;
    for ( nat i = 0; i < 1000; ++i )
    {
        double t1 = timeInSec();
        a = ~b;
        double t2 = timeInSec();
        time += t2 - t1;
    }
    display << time << NL;
}

void affin3d_test2 ()
{
    static PRandVector3d rand;
    const Affin3d a ( rand(), rand(), rand(), rand() );
    double time = 0;
    double max = 0;
    for ( nat i = 0; i < 100; ++i )
    {
        Plane3d plane1 ( rand(), rand().x );
        double t1 = timeInSec();
        Plane3d plane2 = a ( plane1 );
        double t2 = timeInSec();
        time += t2 - t1;
        for ( nat j = 0; j < 100; ++j )
        {
            const Vector3d v1 ( plane1.project ( rand() ) );
            const Vector3d v2 ( a ( v1 ) );
            const double d = fabs ( plane2 % v2 );
            if ( max < d ) max = d;
        }
    }
    display << max << time << NL;
}

void spin3d_test ()
{
    static PRandVector3d rand;
    double max = 0;
    for ( nat i = 0; i < 10000; ++i )
    {
        Spin3d s1 ( rand(), rand().x );
        Spin3d s2 ( rand(), rand().x );
        Vector3d v1 ( rand() );
        Vector3d v2 = Ortho3d ( s1 * s2 ) ( v1 );
        Vector3d v3 = Ortho3d ( s1 ) ( Ortho3d ( s2 ) ( v1 ) );
        const double d = qmod ( v3 - v2 );
        if ( max < d ) max = d;
    }
    display << max << NL;
}

void ortho3d_test ()
{
    static PRandVector3d rand;
    double max = 0;
    for ( nat i = 0; i < 1000; ++i )
    {
        const Vector3d a = rand();
        const double u = rand().x;
        Ortho3d s1 ( a, u );
        Spin3d s2 ( a, u );
        Vector3d v1 ( rand() );
        Vector3d v2 = s1 ( v1 );
        Vector3d v3 = Ortho3d ( s2 ) ( v1 );
        double d = qmod ( v3 - v2 );
        if ( max < d ) max = d;
        v3 = (~s1) ( v2 );
        d = qmod ( v3 - v1 );
        if ( max < d ) max = d;
    }
    display << max << NL;
}

void conform3d_test ()
{
    static PRandVector3d rand;
    double max = 0;
    for ( nat i = 0; i < 10000; ++i )
    {
        Conform3d o1 ( Spin3d ( rand(), rand().x ), rand(), rand().x );
        Conform3d o2 ( Spin3d ( rand(), rand().x ), rand(), rand().x );
        Vector3d v1 ( rand() );
        Vector3d v2 = ( o1 * o2 ) ( v1 );
        Vector3d v3 = o1 ( o2 ( v1 ) );
        o1 = ~o2;
        Conform3d c1 = o1 * o2;
        Conform3d c2 = o2 * o1;
        const double d = qmod ( v3 - v2 );
        if ( max < d ) max = d;
    }
    display << max << NL;
}

void volume_test ()
{
    Sphere3d         sphe;
    Ellipsoid3d      elli;
    Cuboid3d para;
    Cylinder3d       cyli;
    Cone3d           cone;
    sphe.o = elli.o = para.o = cyli.o = cone.o = Vector3d(0,0,0);
    static PRand rand;
    sphe.r = 
    para.a = elli.a = cyli.r = cone.r = 1;
    para.b = elli.b = cyli.h = rand();
    para.c = elli.c = cone.h = rand();
    Polyhedron poly;
    display << "sphe" << volume ( sphe ) << poly.makeModel ( sphe, 9000 ).volume() << NL;
    display << "elli" << volume ( elli ) << poly.makeModel ( elli, 9000 ).volume() << NL;
    display << "para" << volume ( para ) << ( poly = para ).volume() << NL;
    display << "cyli" << volume ( cyli ) << poly.makeModel ( cyli, 1000 ).volume() << NL;
    display << "cone" << volume ( cone ) << poly.makeModel ( cone, 1000 ).volume() << NL << NL;
}

void area_test ()
{
    Sphere3d         sphe;
    Ellipsoid3d      elli;
    Cuboid3d         para;
    Cylinder3d       cyli;
    Cone3d           cone;
    sphe.o = elli.o = para.o = cyli.o = cone.o = Vector3d(0,0,0);
    static PRand rand;
    sphe.r = 
    para.a = elli.a = cyli.r = cone.r = 1;
    para.b = elli.b = cyli.h = rand();
    para.c = elli.c = cone.h = rand();
    Polyhedron poly;
    display << "sphe" << getArea ( sphe ) << poly.makeModel ( sphe, 9000 ).getArea() << NL;
    display << "elli" << getArea ( elli ) << poly.makeModel ( elli, 9000 ).getArea() << NL;
    display << "para" << getArea ( para ) << ( poly = para ).getArea() << NL;
    display << "cyli" << getArea ( cyli ) << poly.makeModel ( cyli, 1000 ).getArea() << NL;
    display << "cone" << getArea ( cone ) << poly.makeModel ( cone, 1000 ).getArea() << NL << NL;
}

double func ( const Vector3d & a, const Vector3d & b, const Vector3d & o, const Vector3d & u )
{
    const Vector3d v = b - a;
    const double vv = v * v;
    const double uv = u * v;
    const double uu = u * u;
    const double d = vv * uu - uv * uv;
    if ( ! d )
        return 0;
    const Vector3d sv = ( u * uv - v * uu ) / d;
    const Vector3d tv = ( u * vv - v * uv ) / d;
    const double s = ( a - o ) * sv;
    const double t = ( a - o ) * tv;
    const Vector3d ps = a + v * s;
    const Vector3d pt = o + u * t;
    return qmod ( ps - pt );
}

Vector3d dfda ( const Vector3d & a, const Vector3d & b, const Vector3d & o, const Vector3d & u )
{
    double e = 1e-4;
    Vector3d v = a;
    double f0 = func ( v, b, o, u );
    v.x += e;
    double fx = func ( v, b, o, u );
    v.x -= e;
    v.y += e;
    double fy = func ( v, b, o, u );
    v.y -= e;
    v.z += e;
    double fz = func ( v, b, o, u );
    return 0.5 * Vector3d ( fx-f0, fy-f0, fz-f0 ) / e;
}

Vector3d dfdb ( const Vector3d & a, const Vector3d & b, const Vector3d & o, const Vector3d & u )
{
    double e = 1e-4;
    Vector3d v = b;
    double f0 = func ( a, v, o, u );
    v.x += e;
    double fx = func ( a, v, o, u );
    v.x -= e;
    v.y += e;
    double fy = func ( a, v, o, u );
    v.y -= e;
    v.z += e;
    double fz = func ( a, v, o, u );
    return 0.5 * Vector3d ( fx-f0, fy-f0, fz-f0 ) / e;
}

void _test()
{
    static PRandVector3d rand;
    const Vector3d a = rand();
    const Vector3d b = rand();
    const Vector3d v = b - a;
    const Vector3d o = rand();
    const Vector3d u = rand();
    const double vv = v * v;
    const double uv = u * v;
    const double uu = u * u;
    const double d = vv * uu - uv * uv;
    if ( ! d )
        return;
    const Vector3d sv = ( u * uv - v * uu ) / d;
    const Vector3d tv = ( u * vv - v * uv ) / d;
    const double s = ( a - o ) * sv;
//    const double t = ( a - o ) * tv;
    const Vector3d wx = v.x * sv - u.x * tv;
    const Vector3d wy = v.y * sv - u.y * tv;
    const Vector3d wz = v.z * sv - u.z * tv;
    const double _s = 1 - s;
    const Vector3d r = v * s + o;
    const double dx = _s * a.x + s * b.x + 
        _s * a.x * wx.x + _s * a.y * wx.y + _s * a.z * wx.z + 
         s * b.x * wx.x +  s * b.y * wx.y +  s * b.z * wx.z + 
         - r * wx - r.x;
    const double dy = _s * a.y + s * b.y +
        _s * a.x * wy.x + _s * a.y * wy.y + _s * a.z * wy.z + 
         s * b.x * wy.x +  s * b.y * wy.y +  s * b.z * wy.z + 
         - r * wy - r.y;
    const double dz = _s * a.z + s * b.z + 
        _s * a.x * wz.x + _s * a.y * wz.y + _s * a.z * wz.z + 
         s * b.x * wz.x +  s * b.y * wz.y +  s * b.z * wz.z + 
         - r * wz - r.z;
//    display << w1.z << NL;
//    display << w1z << NL;
    Vector3d va = dfda ( a, b, o, u );
    Vector3d vb = dfdb ( a, b, o, u );
    display << vb << NL;
    display << s*dx << s*dy << s*dz << NL;
}

void run_test()
{
    static PRandVector3d rand;
    const Vector3d a = rand();
    const Vector3d b = rand();
    const Vector3d v = b - a;
    const Vector3d u = rand();
    const double vv = v * v;
    const double uv = u * v;
    const double uu = u * u;
    const double d = vv * uu - uv * uv;
    if ( ! d )
        return;
    const Vector3d sv = ( u * uv - v * uu ) / d;
    const Vector3d tv = ( u * vv - v * uv ) / d;
    const Vector3d wx = v.x * sv - u.x * tv;
    const Vector3d wy = v.y * sv - u.y * tv;
    const Vector3d wz = v.z * sv - u.z * tv;
    const Vector3d w_ ( wx.x + 1, wy.y + 1, wz.z + 1 );
    double mat[6][6] = {0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0};
    double q[6] = {0,0,0,0,0,0};
    for ( nat i = 0; i < 1; ++i )
    {
        const Vector3d o = rand();
        const double s = ( a - o ) * sv;
        const double _s = 1 - s;
        const double s00 = _s * _s;
        const double s01 = s * _s;
        const double s11 = s * s;
        double * r0 = mat[0];
        double * r1 = mat[1];
        double * r2 = mat[2];
        double * r3 = mat[3];
        double * r4 = mat[4];
        double * r5 = mat[5];
        r0[0] += s00 * w_.x; r0[1] += s00 * wx.y; r0[2] += s00 * wx.z; r0[3] += s01 * w_.x; r0[4] += s01 * wx.y; r0[5] += s01 * wx.z;
        r1[0] += s00 * wy.x; r1[1] += s00 * w_.y; r1[2] += s00 * wy.z; r1[3] += s01 * wy.x; r1[4] += s01 * w_.y; r1[5] += s01 * wy.z;
        r2[0] += s00 * wz.x; r2[1] += s00 * wz.y; r2[2] += s00 * w_.z; r2[3] += s01 * wz.x; r2[4] += s01 * wz.y; r2[5] += s01 * w_.z;
        r3[0] += s01 * w_.x; r3[1] += s01 * wx.y; r3[2] += s01 * wx.z; r3[3] += s11 * w_.x; r3[4] += s11 * wx.y; r3[5] += s11 * wx.z;
        r4[0] += s01 * wy.x; r4[1] += s01 * w_.y; r4[2] += s01 * wy.z; r4[3] += s11 * wy.x; r4[4] += s11 * w_.y; r4[5] += s11 * wy.z;
        r5[0] += s01 * wx.z; r5[1] += s01 * wz.y; r5[2] += s01 * w_.z; r5[3] += s11 * wz.x; r5[4] += s11 * wz.y; r5[5] += s11 * w_.z;
        const Vector3d e = v * s + o;
        const double ex = e * wx + e.x;
        const double ey = e * wy + e.y;
        const double ez = e * wz + e.z;
        q[0] += ex * _s;
        q[1] += ey * _s;
        q[2] += ez * _s;
        q[3] += ex * s;
        q[4] += ey * s;
        q[5] += ez * s;

        const double dx = _s * a.x + s * b.x + 
            _s * a.x * wx.x + _s * a.y * wx.y + _s * a.z * wx.z + 
             s * b.x * wx.x +  s * b.y * wx.y +  s * b.z * wx.z + 
             - e * wx - e.x;
        const double dy = _s * a.y + s * b.y +
            _s * a.x * wy.x + _s * a.y * wy.y + _s * a.z * wy.z + 
             s * b.x * wy.x +  s * b.y * wy.y +  s * b.z * wy.z + 
             - e * wy - e.y;
        const double dz = _s * a.z + s * b.z + 
            _s * a.x * wz.x + _s * a.y * wz.y + _s * a.z * wz.z + 
             s * b.x * wz.x +  s * b.y * wz.y +  s * b.z * wz.z + 
             - e * wz - e.z;
    double fax = r0[0] * a.x + r0[1] * a.y + r0[2] * a.z + r0[3] * b.x + r0[4] * b.y + r0[5] * b.z - q[0];
    double fay = r1[0] * a.x + r1[1] * a.y + r1[2] * a.z + r1[3] * b.x + r1[4] * b.y + r1[5] * b.z - q[1];
    double faz = r2[0] * a.x + r2[1] * a.y + r2[2] * a.z + r2[3] * b.x + r2[4] * b.y + r2[5] * b.z - q[2];
    double fbx = r3[0] * a.x + r3[1] * a.y + r3[2] * a.z + r3[3] * b.x + r3[4] * b.y + r3[5] * b.z - q[3];
    double fby = r4[0] * a.x + r4[1] * a.y + r4[2] * a.z + r4[3] * b.x + r4[4] * b.y + r4[5] * b.z - q[4];
    double fbz = r5[0] * a.x + r5[1] * a.y + r5[2] * a.z + r5[3] * b.x + r5[4] * b.y + r5[5] * b.z - q[5];
    Vector3d va = dfda ( a, b, o, u );
    Vector3d vb = dfdb ( a, b, o, u );
    //display << fbx << fby << fbz << NL;
    display << va << NL;
    display << _s*dx << _s*dy << _s*dz << NL;
    //display << vb << NL;
    //display << s*dx << s*dy << s*dz << NL;
    }
}

} // namespace

void vector3d_test ()
{
    run_test();
//    plane3d_test ();
//    ortho3d_test ();
//    conform3d_test ();
//    affin3d_test ();
//    volume_test ();
//    area_test ();
}
