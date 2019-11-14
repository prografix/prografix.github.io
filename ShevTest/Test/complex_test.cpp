
#include "math.h"

#include "../Shev/rand.h"
#include "../Shev/mathem.h"
#include "../Shev/CMatrix.h"
#include "../Shev/template.h"
#include "display.h"

namespace
{

void cbrt_test ()
{
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
        const Complex a ( rand(), rand() );
        Complex x1, x2, x3;
        root3 ( a, x1, x2, x3 );
        const Complex e1 = x1 * x1 * x1 - a;
        const Complex e2 = x2 * x2 * x2 - a;
        const Complex e3 = x3 * x3 * x3 - a;
        const double m1 = _max ( fabs ( e1.re ), fabs ( e1.im ) );
        const double m2 = _max ( fabs ( e2.re ), fabs ( e2.im ) );
        const double m3 = _max ( fabs ( e3.re ), fabs ( e3.im ) );
        const double max = _max ( m1, m2, m3 );
        if ( max > 3e-15 ) // 1.5e-15 - real coef
            display << "max =" << max << i << NL;
    }
    display << "end" << NL;
}

void root2_test ()
{
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
/*
        const double a = rand();
        const double b = rand();
*/
        const Complex a ( rand(), rand() );
        const Complex b ( rand(), rand() );
        Complex x1, x2;
        root2 ( a, b, x1, x2 );
        const Complex e1 = x1 * ( x1 + a ) + b;
        const Complex e2 = x2 * ( x2 + a ) + b;
        const double m1 = _max ( fabs ( e1.re ), fabs ( e1.im ) );
        const double m2 = _max ( fabs ( e2.re ), fabs ( e2.im ) );
        const double max = _max ( m1, m2 );
        if ( max > 3e-15 ) // 1.5e-15 - real coef
            display << "max =" << max << i << NL;
    }
    display << "end" << NL;
}

void root3s_test ()
{
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
/*
        const double a = rand();
        const double b = rand();
*/
        const Complex a ( rand(), rand() );
        const Complex b ( rand(), rand() );
//        double x1;
        Complex x1, x2, x3;
        root3s ( a, b, x1, x2, x3 );
        const Complex e1 = x1 * ( x1 * x1 + a ) + b;
        const Complex e2 = x2 * ( x2 * x2 + a ) + b;
        const Complex e3 = x3 * ( x3 * x3 + a ) + b;
        const double m1 = _max ( fabs ( e1.re ), fabs ( e1.im ) );
        const double m2 = _max ( fabs ( e2.re ), fabs ( e2.im ) );
        const double m3 = _max ( fabs ( e3.re ), fabs ( e3.im ) );
        const double max = _max ( m1, m2, m3 );
        if ( max > 3e-15 )
        {
            root3s ( a, b, x1, x2, x3 );
            display << "max =" << max << i << NL;
        }
    }
    display << "end" << NL;
}

void root3_test ()
{
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
/*
        const double a = rand();
        const double b = rand();
        const double c = rand();
*/
        const Complex a ( rand(), rand() );
        const Complex b ( rand(), rand() );
        const Complex c ( rand(), rand() );
//        double x1;
        Complex x1, x2, x3;
        root3 ( a, b, c, x1, x2, x3 );
        const Complex e1 = x1 * ( x1 * ( x1 + a ) + b ) + c;
        const Complex e2 = x2 * ( x2 * ( x2 + a ) + b ) + c;
        const Complex e3 = x3 * ( x3 * ( x3 + a ) + b ) + c;
        const double m1 = _max ( fabs ( e1.re ), fabs ( e1.im ) );
        const double m2 = _max ( fabs ( e2.re ), fabs ( e2.im ) );
        const double m3 = _max ( fabs ( e3.re ), fabs ( e3.im ) );
        const double max = _max ( m1, m2, m3 );
        if ( max > 8e-15 )
            display << "max =" << max << i << NL;
    }
    display << "end" << NL;
}

inline
Complex func4s ( double p, double q, double r, const Complex & x )
{
    return x * ( x * ( x * x + p ) + q ) + r;
}

inline
Complex func4s ( const Complex & p, const Complex & q, const Complex & r, const Complex & x )
{
    return x * ( x * ( x * x + p ) + q ) + r;
}

void root4s_test ()
{
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
/*
        const double a = rand();
        const double b = rand();
        const double c = rand();
*/
        const Complex a ( rand(), rand() );
        const Complex b ( rand(), rand() );
        const Complex c ( rand(), rand() );
        Complex x1, x2, x3, x4;
        root4s ( a, b, c, x1, x2, x3, x4 );
        const Complex e1 = func4s ( a, b, c, x1 );
        const Complex e2 = func4s ( a, b, c, x2 );
        const Complex e3 = func4s ( a, b, c, x3 );
        const Complex e4 = func4s ( a, b, c, x4 );
        const double m1 = _max ( fabs ( e1.re ), fabs ( e1.im ) );
        const double m2 = _max ( fabs ( e2.re ), fabs ( e2.im ) );
        const double m3 = _max ( fabs ( e3.re ), fabs ( e3.im ) );
        const double m4 = _max ( fabs ( e4.re ), fabs ( e4.im ) );
        const double max = _max ( _max ( m1, m2 ), _max ( m3, m4 ) );
        if ( max > 1e-14 )
        {
            display << "max =" << max << i << NL;
            root4s ( a, b, c, x1, x2, x3, x4 );
        }
    }
    display << "end" << NL;
}

inline
Complex func4 ( double a, double b, double c, double d, const Complex & x )
{
    return x * ( x * ( x * ( x + a ) + b ) + c ) + d;
}

inline
Complex func4 ( const Complex & a, const Complex & b, const Complex & c, 
                const Complex & d, const Complex & x )
{
    return x * ( x * ( x * ( x + a ) + b ) + c ) + d;
}

void root4_test ()
{
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
/*
        const double a = rand();
        const double b = rand();
        const double c = rand();
        const double d = rand();
*/
        const Complex a ( rand(), rand() );
        const Complex b ( rand(), rand() );
        const Complex c ( rand(), rand() );
        const Complex d ( rand(), rand() );
        Complex x1, x2, x3, x4;
        root4 ( a, b, c, d, x1, x2, x3, x4 );
        const Complex e1 = func4 ( a, b, c, d, x1 );
        const Complex e2 = func4 ( a, b, c, d, x2 );
        const Complex e3 = func4 ( a, b, c, d, x3 );
        const Complex e4 = func4 ( a, b, c, d, x4 );
        const double m1 = _max ( fabs ( e1.re ), fabs ( e1.im ) );
        const double m2 = _max ( fabs ( e2.re ), fabs ( e2.im ) );
        const double m3 = _max ( fabs ( e3.re ), fabs ( e3.im ) );
        const double m4 = _max ( fabs ( e4.re ), fabs ( e4.im ) );
        const double max = _max ( _max ( m1, m2 ), _max ( m3, m4 ) );
        if ( max > 4e-14 )
        {
            display << "max =" << max << i << NL;
            root4 ( a, b, c, d, x1, x2, x3, x4 );
        }
    }
    display << "end" << NL;
}

inline
Complex func5 ( double a, double b, double c, double d, double e, const Complex & x )
{
    return x * ( x * ( x * ( x * ( x + a ) + b ) + c ) + d ) + e;
}

void root5_test ()
{
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
        const double a = rand();
        const double b = rand();
        const double c = rand();
        const double d = rand();
        const double e = rand();
        double x1;
        Complex x2, x3, x4, x5;
        root5 ( a, b, c, d, e, x1, x2, x3, x4, x5 );
        const Complex e1 = func5 ( a, b, c, d, e, x1 );
        const Complex e2 = func5 ( a, b, c, d, e, x2 );
        const Complex e3 = func5 ( a, b, c, d, e, x3 );
        const Complex e4 = func5 ( a, b, c, d, e, x4 );
        const Complex e5 = func5 ( a, b, c, d, e, x5 );
        const double m1 = _max ( fabs ( e1.re ), fabs ( e5.re ), fabs ( e5.im ) );
        const double m2 = _max ( fabs ( e2.re ), fabs ( e2.im ) );
        const double m3 = _max ( fabs ( e3.re ), fabs ( e3.im ) );
        const double m4 = _max ( fabs ( e4.re ), fabs ( e4.im ) );
        const double max = _max ( _max ( m1, m2 ), _max ( m3, m4 ) );
        if ( max > 2e-13 )
        {
            root5 ( a, b, c, d, e, x1, x2, x3, x4, x5 );
            display << "max =" << max << i << NL;
        }
    }
    display << "end" << NL;
}

void sqrt_test ()
{
    static PRand  rand;
    for ( nat i = 0; i <= 50; i += 1 )
    {
        Complex c ( rand() * 2 - 1, rand() * 2 - 1 );
        Complex t = sqrt(c);
        display << abs(c-t*t) << NL;
    }
}

void lin_test_clss ()
{
    const nat n = 99;
    SMatrix<Complex, n, n> mat;
    nat i, j;
    static PRand rand1, rand2 ( 13 );
    Complex * const * a = mat;
    Complex b[n], x1[n], x2[n];
    for ( i = 0; i < n; ++i )
    {
        for ( j = 0; j < n; ++j )
        {
            a[i][j] = Complex ( 2 * rand1() - 1., 2 * rand2() - 1. ) / ( 1 + i + j );
        }
        x1[i] = Complex ( 2 * rand1() - 1., 2 * rand2() - 1.);
    }
    for ( i = 0; i < n; ++i )
    {
        b[i] = 0.;
        for ( j = 0; j < n; ++j )
        {
            b[i] += a[i][j] * x1[j];
        }
    }
//    CMatrix mat1 ( mat );
//    CLSS_H sl ( mat1 );
//    if ( sl.solve ( b, x2 ) )
    if ( lss_h ( mat, b, x2 ) )
    {
        double d1 = 0.;
        double d2 = 0.;
        for ( i = 0; i < n; ++i )
        {
            d1 += qmod ( x1[i] - x2[i] );
            Complex s = b[i];
            for ( j = 0; j < n; ++j )
            {
                s -= a[i][j] * x2[j];
            }
            d2 += qmod ( s );
        }
        display << sqrt(d1) << sqrt(d2) << NL;
    }
}

} // namespace

void complex_test ()
{
//    cbrt_test();
    root4_test();
//    lin_test_clss ();
}