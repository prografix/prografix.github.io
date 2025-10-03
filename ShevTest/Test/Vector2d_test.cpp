
#include "math.h"

#include "../Shev/Vector2d.h"
#include "../Shev/func2d.h"
#include "../Shev/rand.h"
#include "../Shev/tune.h"

#include "display.h"

namespace
{

void spin2d_test ()
{
    static PRand rand;
    static PRandVector2d vrand;
    for ( nat i = 0; i < 99; ++i )
    {
        double a = 6. * ( rand() - 0.5 );
        const Spin2d spin ( a );

        double e = fabs ( a - spin.getAngle() );
        if ( e > 1e-16 ) 
            display << "err1" << e << NL;

        Vector2d ax, ay, va, vb, vc;
        spin.getReper ( ax, ay );
        va = vrand();
        va *= a;
        vb = Vector2d ( ax * va, ay * va );
        vc = spin ( va );
        if ( qmod ( vb - vc ) > 1e-9 ) 
            display << "err2" << NL;

        double b = 6. * ( rand() - 0.5 );
        Spin2d spin2 ( b );
        Spin2d spin3 = spin2 * spin;
        vb = spin2 ( spin ( va ) );
        vc = spin3 ( va );
        if ( qmod ( vb - vc ) > 1e-9 ) 
            display << "err3" << NL;

        Spin2d spin4 = ~spin;
        vb = spin  ( va );
        vc = spin4 ( vb );
        if ( qmod ( va - vc ) > 1e-9 ) 
            display << "err4" << NL;
    }
    display << "end" << NL;
}

void spin2d_test2 ()
{
    static PRandVector2d vrand;
    for ( nat i = 0; i < 99; ++i )
    {
        const Vector2d a = vrand();
        const Vector2d b = vrand();
        const Spin2d spin ( a, b );
        const Vector2d v = spin ( a );
        if ( qmod ( v - b ) > 1e-12 ) 
            display << i << "err" << NL;
    }
    display << "end" << NL;
}

void conform2d_test ()
{
    static PRand rand;
    static PRandVector2d vrand;
    nat i;
    for ( i = 0; i < 99; ++i )
    {
        const double a = 6. * ( rand() - 0.5 );
        const Spin2d spin ( a );
        Vector2d trans = vrand();
        const Conform2d conf1 ( spin, trans, rand()+1 );
        const Conform2d conf2 = ~ conf1;
        Line2d line1;
        line1.norm = vrand();
        line1.dist = a;
        Line2d line2 = conf1 ( line1 );
        Line2d line3 = conf2 ( line2 );
        if ( qmod ( line1.norm - line3.norm ) > 1e-22 || fabs ( line1.dist - line3.dist ) > 1e-15 ) 
            display << "err1" << NL;
        
        const double b = 6. * ( rand() - 0.5 );
        const Spin2d spin2 ( b );
        Vector2d trans2 = vrand();
        const Conform2d conf3 ( spin2, trans2, rand()+1 );
        const Conform2d conf4 = conf1 * conf3;
        Vector2d va = vrand ();
        Vector2d vb = conf1 ( conf3 ( va ) );
        Vector2d vc = conf4 ( va );
        if ( qmod ( vb - vc ) > 1e-22 ) 
            display << "err2" << NL;

    }
//    Conform2d & operator*= ( const Conform2d & c )
    display << "end" << NL;
}

void affin2d_test ()
{
    static PRandPoint2d rand;
    Affin2d a ( rand(), rand(), rand() );
    Affin2d b = ~a;
    Affin2d c = b * a;
    Affin2d d = a * b;
}

void perimeter_test ()
{/*
    Rhombus2d romb;
    Ellipse2d elli;
    Rectangle2d rect;
    romb.a = elli.a = rect.a = 1;
    for ( nat i = 1; i < 17; ++i )
    {
        romb.b = elli.b = rect.b = i;
        display << i;
        display << getPerimeter ( romb ) / getPerimeter ( elli );
        display << getPerimeter ( elli ) / getPerimeter ( rect ) << NL;
    }*/
    Ellipse2d elli;
    elli.a = 1;
    for ( nat i = 0; i <= 32; ++i )
    {
        elli.b = 1 - i / 32.;
        const double h = _pow2 ( ( elli.a - elli.b ) / ( elli.a + elli.b ) );
        //double p = M_PI * ( elli.a + elli.b ) * ( 64 + 3 * h * h ) / ( 64 - 16 * h );
        double p = M_PI * ( elli.a + elli.b ) * ( 135168 - h*(85760 + h*(5568 - 3867*h)))/
                                                ( 135168 - h*(119552 - h*(22208 + 345*h)));
        display << i << elli.b << getPerimeter ( elli ) << p << NL;
    }

}

void area_test ()
{
    Parallelogram2d fig ( Vector2d ( 0,0 ), Vector2d ( 1,0 ), Vector2d ( 1,1 ) );
    display << fig.getArea() << NL;
}

double sfunc ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    const Vector2d v = b - a;
    const double d = v * v;
    if ( ! d )
        return 0;
    return ( o - a ) * v / d;
}

Vector2d dsda ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = a;
    double f0 = sfunc ( v, b, o );
    v.x += e;
    double fx = sfunc ( v, b, o );
    v.x -= e;
    v.y += e;
    double fy = sfunc ( v, b, o );
    return Vector2d ( fx-f0, fy-f0 ) / e;
}

Vector2d dsdb ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = b;
    double f0 = sfunc ( a, v, o );
    v.x += e;
    double fx = sfunc ( a, v, o );
    v.x -= e;
    v.y += e;
    double fy = sfunc ( a, v, o );
    return Vector2d ( fx-f0, fy-f0 ) / e;
}

Vector2d rfunc ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    return a + ( b - a ) * sfunc ( a, b, o ) - o;
}

Vector2d drdax ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = a;
    v.x += e;
    return ( rfunc ( v, b, o ) - rfunc ( a, b, o ) ) / e;
}

Vector2d drday ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = a;
    v.y += e;
    return ( rfunc ( v, b, o ) - rfunc ( a, b, o ) ) / e;
}

Vector2d drdbx ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = b;
    v.x += e;
    return ( rfunc ( a, v, o ) - rfunc ( a, b, o ) ) / e;
}

Vector2d drdby ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = b;
    v.y += e;
    return ( rfunc ( a, v, o ) - rfunc ( a, b, o ) ) / e;
}

double qfunc ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    return qmod ( a + ( b - a ) * sfunc ( a, b, o ) - o );
}

double dqdax ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = a;
    v.x += e;
    return ( qfunc ( v, b, o ) - qfunc ( a, b, o ) ) / e;
}

double dqday ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = a;
    v.y += e;
    return ( qfunc ( v, b, o ) - qfunc ( a, b, o ) ) / e;
}

double dqdbx ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = b;
    v.x += e;
    return ( qfunc ( a, v, o ) - qfunc ( a, b, o ) ) / e;
}

double dqdby ( const Vector2d & a, const Vector2d & b, const Vector2d & o )
{
    double e = 1e-4;
    Vector2d v = b;
    v.y += e;
    return ( qfunc ( a, v, o ) - qfunc ( a, b, o ) ) / e;
}

void run_test()
{
    static PRandVector2d rand;
    const Vector2d a = rand();
    const Vector2d b = rand();
    const Vector2d o = rand();
    const Vector2d v = b - a;
    const Vector2d u = o - a;
    const double s = sfunc ( a, b, o );
//    display << b * ( u1 - 2 * s * v1 ) - a * ( u1 + v1 - 2 * s * v1 ) + s + o * v1 << sfunc ( a, b, o ) << NL;
//    display << - ( u + v - 2 * s * v ) / (v*v) << NL << dsda ( a, b, o ) << NL;
//    display << ( u - 2 * s * v ) / (v*v) << NL << dsdb ( a, b, o ) << NL;
    const Vector2d r = a + v * s - o;
//    display << ( 1 - s ) * a + s * b + v * ( o * v - a * ( u + v - 2 * s * v ) + b * ( u - 2 * s * v ) ) / (v*v) - o << NL << r << NL;
//    display << ( 1 - s ) * a + s * b + v * ( o * v + a * ( r - ( 1 - s ) * v ) - b * ( r + s * v ) ) / (v*v) - o << NL << r << NL;
//    const Vector2d wa = ( 2 * s * v - u - v ) / (v*v);
//    const Vector2d wb = ( u - 2 * s * v ) / (v*v);
    const double d = norm2 ( v );
    const Vector2d vn = v / d;
    const Vector2d rn = r / d;
    const Vector2d wa = ( rn - ( 1 - s ) * vn );
    const Vector2d wb = -( rn + s * vn );
    const Vector2d wo = vn * ( o * vn ) - o;
    const Vector2d ax ( 1-s + vn.x * wa.x, vn.y * wa.x );
    const Vector2d ay ( vn.x * wa.y, 1-s + vn.y * wa.y );
    const Vector2d bx ( s + vn.x * wb.x, vn.y * wb.x );
    const Vector2d by ( vn.x * wb.y, s + vn.y * wb.y );
//    display << ax * a.x + ay * a.y + bx * b.x + by * b.y + wo << NL << r << NL;
//    display << ax << NL << drdax ( a, b, o ) << NL;
//    display << ay << NL << drday ( a, b, o ) << NL;
//    display << bx << NL << drdbx ( a, b, o ) << NL;
//    display << by << NL << drdby ( a, b, o ) << NL;
//    display << _pow2 ( ax.x * a.x + ay.x * a.y + bx.x * b.x + by.x * b.y + wo.x ) +
//               _pow2 ( ax.y * a.x + ay.y * a.y + bx.y * b.x + by.y * b.y + wo.y ) << qfunc ( a, b, o ) << NL;
//    display << ax * ax * a.x + ax * ay * a.y + ax * bx * b.x + ax * by * b.y + (1-s) * wo.x << 0.5 * dqdax ( a, b, o ) << NL;
//    display << ay * ax * a.x + ay * ay * a.y + ay * bx * b.x + ay * by * b.y + (1-s) * wo.y << 0.5 * dqday ( a, b, o ) << NL;
//    display << bx * ( ax * a.x + ay * a.y + bx * b.x + by * b.y ) + s * wo.x << 0.5 * dqdbx ( a, b, o ) << NL;
    display << by * ( ax * a.x + ay * a.y + bx * b.x + by * b.y ) + s * wo.y << 0.5 * dqdby ( a, b, o ) << NL;
}

} // namespace

void vector2d_test ()
{
    run_test();
//    spin2d_test2();
//    conform2d_test();
//    affin2d_test ();
//    perimeter_test ();
//    area_test ();
}