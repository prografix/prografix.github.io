
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

} // namespace

void vector3d_test ()
{
    plane3d_test ();
//    ortho3d_test ();
//    conform3d_test ();
//    affin3d_test ();
//    volume_test ();
//    area_test ();
}
