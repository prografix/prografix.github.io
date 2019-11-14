
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

} // namespace

void vector2d_test ()
{
//    spin2d_test2();
//    conform2d_test();
//    affin2d_test ();
    perimeter_test ();
//    area_test ();
}