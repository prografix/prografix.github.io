
#include "math.h"
#include "mathem.h"
#include "Complex.h"

//************************ 08.04.2006 *************************//
//
//             Абсолютное значение ( модуль числа )
//
//************************ 08.04.2006 *************************//

double abs ( const Complex & c )
{
    const double re = fabs ( c.re );
    const double im = fabs ( c.im );
    if ( re > im )
    {
        if ( im == 0 ) return re;
        const double t = im / re;
        return re * sqrt ( 1. + t * t );
    }
    else
    {
        if ( re == 0 ) return im;
        const double t = re / im;
        return im * sqrt ( 1. + t * t );
    }
}

//************************ 18.11.2018 *************************//
//
//                         Экспонента
//
//************************ 18.11.2018 *************************//

Complex exp ( const Complex & c )
{
    double e = exp ( c.re );
    return Complex ( e * cos ( c.im ), e * sin ( c.im ) );
}

//************************ 08.04.2006 *************************//
//
//                     Квадратный корень
//
//************************ 08.04.2006 *************************//

Complex sqrt ( const Complex & c )
{
    if ( c.im != 0 )
    {
        if ( c.re < 0 )
        {
            const double b = sqrt ( 0.5 * ( abs ( c ) - c.re ) );
            return Complex ( 0.5 * c.im / b, b );
        }
        else
        {
            const double a = sqrt ( 0.5 * ( abs ( c ) + c.re ) );
            return Complex ( a, 0.5 * c.im / a );
        }
    }
    if ( c.re > 0 )
    {
        return Complex ( sqrt(c.re), 0. );
    }
    if ( c.re < 0 )
    {
        return Complex ( 0., sqrt(-c.re) );
    }
    return Complex ( 0., 0. );
}

//************************ 24.12.2011 *************************//
//
//                     Кубические корни
//
//************************ 24.12.2011 *************************//

double cubic ( double a, double b );

void root3 ( Complex x, Complex & r1, Complex & r2, Complex & r3 )
{
    if ( ! x )
    {
        r1 = r2 = r3 = x;
        return;
    }
    const double s = root3 ( qmod ( x ) );
    if ( x.re > 0 )
    {
        r1.re = cubic ( -0.75 * s, -0.25 * x.re );
    }
    else
    if ( x.re < 0 )
    {
        r1.re = - cubic ( -0.75 * s, 0.25 * x.re );
    }
    else
    {
        r1.re = 0;
    }
    const double t = s - r1.re * r1.re;
    r1.im = t > 0 ? sqrt ( t ) : 0;
    if ( r1.im * ( 3 * r1.re * r1.re - t ) * x.im < 0 ) r1.im = - r1.im;
    r1 -= 1./3. * ( r1 - x / ( r1 * r1 ) );
    r2 = r1 * Complex ( -0.5, 0.866025403784443864 );
    r2 -= 1./3. * ( r2 - x / ( r2 * r2 ) );
    r3 = r2 * Complex ( -0.5, 0.866025403784443864 );
    r3 -= 1./3. * ( r3 - x / ( r3 * r3 ) );
}

//************************ 02.12.2011 *************************//
//
//      Решение алгебраического уравнения 2-ой степени
//      с действительными коэффициентами
//      x^2 + a * x + b = 0
//
//************************ 02.12.2011 *************************//

void root2 ( double a, double b, Complex & x1, Complex & x2 )
{
    if ( b == 0 )
    {
        x1.re = -a;
        x2.re = x1.im = x2.im = 0;
    }
    else
    {
        a *= -0.5;
        double d = a * a - b;
        if ( d < 0 )
        {
            x1.re = x2.re = a;
            x1.im = sqrt ( -d );
            x2.im = - x1.im;
        }
        else
        {
            d = sqrt ( d );
            x1.re = a > 0 ? a + d : a - d;
            x2.re = b / x1.re;
            x1.im = x2.im = 0;
        }
    }
}

//************************ 17.12.2011 *************************//
//
//      Решение алгебраического уравнения 2-ой степени
//      с комплексными коэффициентами
//      x^2 + a * x + b = 0
//
//************************ 17.12.2011 *************************//

void root2 ( const Complex & a, const Complex & b, Complex & x1, Complex & x2 )
{
    if ( ! b )
    {
        x1 = b;
        x2 = -a;
    }
    else
    {
        const Complex c ( -0.5 * a.re, -0.5 * a.im );
        const Complex d = sqrt ( c * c - b );
        x1 = c - d;
        x2 = c + d;
    }
}

//************************ 06.12.2011 *************************//
//
//      Решение неполного алгебраического уравнения 3-й степени
//      с действительными коэффициентами
//      x^3 + p * x + q = 0
//
//************************ 06.12.2011 *************************//

void root3s ( double p, double q, double & x1, Complex & x2, Complex & x3 )
{
    if ( q < 0 )
    {
        x1 = cubic ( p, q );
    }
    else
    if ( q > 0 )
    {
        x1 = -cubic ( p, -q );
    }
    else
    {
        x1 = 0;
    }
    root2 ( x1, p + x1 * x1, x2, x3 );
}

//************************ 03.01.2012 *************************//
//
//      Решение неполного алгебраического уравнения 3-й степени
//      с комплексными коэффициентами
//      x^3 + p * x + q = 0
//
//************************ 03.01.2012 *************************//

void root3s ( const Complex & p, const Complex & q, Complex & x1, Complex & x2, Complex & x3 )
{
// Частные случаи
    if ( ! p )
    {
        root3 ( -q, x1, x2, x3 );
        return;
    }
    if ( ! q )
    {
        x1 = q;
        x2 = sqrt ( -p );
        x3 = -x2;
        return;
    }
// Решение Кардано
    const Complex s ( p / 3 );
    const Complex t ( -0.5 * q );
    const Complex c = sqrt ( s * s * s + t * t );
    Complex z1, z2, z3;
    if ( qmod ( t + c ) > qmod ( t - c ) )
        root3 ( t + c, z1, z2, z3 );
    else
        root3 ( t - c, z1, z2, z3 );
    x1 = z1 - s / z1;
    x2 = z2 - s / z2;
    x3 = z3 - s / z3;
// Уточняем корни методом Ньютона
    const Complex p1 = x1 * x1 * 3 + p;
    if ( qmod ( p1 ) > 1e-6 ) x1 -= ( x1 * ( x1 * x1 + p ) + q ) / p1;
    const Complex p2 = x2 * x2 * 3 + p;
    if ( qmod ( p2 ) > 1e-6 ) x2 -= ( x2 * ( x2 * x2 + p ) + q ) / p2;
    const Complex p3 = x3 * x3 * 3 + p;
    if ( qmod ( p3 ) > 1e-6 ) x3 -= ( x3 * ( x3 * x3 + p ) + q ) / p3;
}

//************************ 06.12.2011 *************************//
//
//      Решение полного алгебраического уравнения 3-й степени
//      с действительными коэффициентами
//      x^3 + a * x^2 + b * x + c = 0
//
//************************ 06.12.2011 *************************//

void root3 ( double a, double b, double c, double & x1, Complex & x2, Complex & x3 )
{
    if ( ! c )
    {
        x1 = c;
    }
    else
    {
        const double a3 = a / 3.;
        const double p = b - a3 * a;
        const double q = c - ( a3 * a3 + p ) * a3;
        if ( q < 0 )
        {
            x1 = cubic ( p, q );
        }
        else
        if ( q > 0 )
        {
            x1 = -cubic ( p, -q );
        }
        else
        {
            x1 = 0;
        }
        x1 -= a3;
        const double t = x1 * ( x1 * 3. + a * 2. ) + b;
        if ( fabs ( t ) > 1e-3 ) x1 -= ( x1 * ( x1 * ( x1 + a ) + b ) + c ) / t;
        a += x1;
        b += x1 * a;
    }
    root2 ( a, b, x2, x3 );
}

//************************ 04.01.2012 *************************//
//
//      Решение полного алгебраического уравнения 3-й степени
//      с комплексными коэффициентами
//      x^3 + a * x^2 + b * x + c = 0
//
//************************ 04.01.2012 *************************//

void root3 ( const Complex & a, const Complex & b, const Complex & c, 
             Complex & x1, Complex & x2, Complex & x3 )
{
// Частные случаи
    if ( ! a )
    {
        root3s ( b, c, x1, x2, x3 );
    }
    else
    if ( ! c )
    {
        x1 = c;
        root2 ( a, b, x2, x3 );
    }
    else
// Полное уравнение
    {
        const Complex a3 = a / 3.;
        const Complex p = b - a3 * a;
        const Complex q = c - ( a3 * a3 + p ) * a3;
        root3s ( p, q, x1, x2, x3 );
// Уточнение корней методом Ньютона
        x1 -= a3;
        const Complex p1 = x1 * ( x1 * 3. + a * 2. ) + b;
        if ( qmod ( p1 ) > 1e-6 ) x1 -= ( x1 * ( x1 * ( x1 + a ) + b ) + c ) / p1;
        x2 -= a3;
        const Complex p2 = x2 * ( x2 * 3. + a * 2. ) + b;
        if ( qmod ( p2 ) > 1e-6 ) x2 -= ( x2 * ( x2 * ( x2 + a ) + b ) + c ) / p2;
        x3 -= a3;
        const Complex p3 = x3 * ( x3 * 3. + a * 2. ) + b;
        if ( qmod ( p3 ) > 1e-6 ) x3 -= ( x3 * ( x3 * ( x3 + a ) + b ) + c ) / p3;
    }
}

//************************ 10.12.2011 *************************//
//
//      Решение неполного алгебраического уравнения 4-й степени
//      с действительными коэффициентами
//      x^4 + p * x^2 + q * x + r = 0
//
//************************ 10.12.2011 *************************//

inline
void tuneRoot4s ( double p, double q, double r, Complex & x )
{
    const Complex t = x * ( x * x * 4 + p * 2 ) + q;
    if ( qmod ( t ) > 1e-6 )
    {
        x -= ( x * ( x * ( x * x + p ) + q ) + r ) / t;
    }
}

void root4s ( double p, double q, double r, 
              Complex & x1, Complex & x2, Complex & x3, Complex & x4 )
{
    if ( r == 0 )
    {
        x1 = x2.im = 0;
        root3s ( p, q, x2.re, x3, x4 );
        return;
    }
    if ( q == 0 )
    {
m1:     root2 ( p, r, x1, x2 );
        x1 = sqrt ( x1 );
        x3 = - x1;
        x2 = sqrt ( x2 );
        x4 = - x2;
        return;
    }
    double x[3];
    const nat n = root3 ( p + p, p * p - 4 * r, -q * q, x );
    double a = x[0];
    if ( n == 3 )
    {
        if ( a < x[1] ) a = x[1];
        if ( a < x[2] ) a = x[2];
    }
    if ( a <= 0 ) goto m1;
    const double c = p + a;
    a = sqrt ( a );
    const double b = q / a;
    root2 (  a, 0.5 * ( c - b ), x1, x2 );
    root2 ( -a, 0.5 * ( c + b ), x3, x4 );
// Уточнение корней методом Ньютона
    tuneRoot4s ( p, q, r, x1 );
    tuneRoot4s ( p, q, r, x2 );
    tuneRoot4s ( p, q, r, x3 );
    tuneRoot4s ( p, q, r, x4 );
}

//************************ 04.02.2012 *************************//
//
//      Решение неполного алгебраического уравнения 4-й степени
//      с комплексными коэффициентами
//      x^4 + p * x^2 + q * x + r = 0
//
//************************ 04.02.2012 *************************//

inline
void tuneRoot4s ( const Complex & p, const Complex & q, const Complex & r, Complex & x )
{
    const Complex t = x * ( x * x * 4 + p * 2 ) + q;
    if ( qmod ( t ) > 1e-6 )
    {
        x -= ( x * ( x * ( x * x + p ) + q ) + r ) / t;
    }
}

void root4s ( const Complex & p, const Complex & q, const Complex & r, 
              Complex & x1, Complex & x2, Complex & x3, Complex & x4 )
{
    if ( ! r )
    {
        x1 = r;
        root3s ( p, q, x2, x3, x4 );
        return;
    }
    if ( q == 0 )
    {
m1:     Complex y1, y2;
        root2 ( p, r, y1, y2 );
        x1 = sqrt ( y1 );
        x2 = - x1;
        x3 = sqrt ( y2 );
        x4 = - x3;
        return;
    }
    root3 ( p + p, p * p - 4. * r, -q * q, x1, x2, x3 );
    const double s1 = qmod ( x1 );
    const double s2 = qmod ( x2 );
    const double s3 = qmod ( x3 );
    Complex a = s1 > s2 ? ( s1 > s3 ? x1 : x3 ) : ( s2 > s3 ? x2 : x3 );
    if ( ! a ) goto m1;
    const Complex pa = p + a;
    a = sqrt ( a );
    const Complex b = q / a;
    root2 (  a, 0.5 * ( pa - b ), x1, x2 );
    root2 ( -a, 0.5 * ( pa + b ), x3, x4 );
// Уточнение корней методом Ньютона
    tuneRoot4s ( p, q, r, x1 );
    tuneRoot4s ( p, q, r, x2 );
    tuneRoot4s ( p, q, r, x3 );
    tuneRoot4s ( p, q, r, x4 );
}

//************************ 10.12.2011 *************************//
//
//      Решение полного алгебраического уравнения 4-й степени
//      с действительными коэффициентами
//      x^4 + a * x^3 + b * x^2 + c * x + d = 0
//
//************************ 10.12.2011 *************************//

inline
void tuneRoot4 ( double a, double b, double c, double d, Complex & x )
{
    const Complex t = x * ( x * ( x * 4 + a * 3 ) + b * 2 ) + c;
    if ( qmod ( t ) > 1e-6 )
    {
        x -= ( x * ( x * ( x * ( x + a ) + b ) + c ) + d ) / t;
    }
}

void root4 ( double a, double b, double c, double d, 
             Complex & x1, Complex & x2, Complex & x3, Complex & x4 )
{
    if ( a == 0 )
    {
        root4s ( b, c, d, x1, x2, x3, x4 );
        return;
    }
    if ( d == 0 )
    {
        x1 = x2.im = 0;
        root3 ( a, b, c, x2.re, x3, x4 );
        return;
    }
    const double e = a / 4;
    const double h = e * e;
    const double p = -6 * h + b;
    const double q =  8 * h * e - 2 * b * e + c;
    const double r = -3 * h * h + b * h - c * e + d;
    root4s ( p, q, r, x1, x2, x3, x4 );
// Уточнение корней методом Ньютона
    tuneRoot4 ( a, b, c, d, x1 -= e );
    tuneRoot4 ( a, b, c, d, x2 -= e );
    tuneRoot4 ( a, b, c, d, x3 -= e );
    tuneRoot4 ( a, b, c, d, x4 -= e );
}

//************************ 04.02.2012 *************************//
//
//      Решение полного алгебраического уравнения 4-й степени
//      с комплексными коэффициентами
//      x^4 + a * x^3 + b * x^2 + c * x + d = 0
//
//************************ 04.02.2012 *************************//

inline
void tuneRoot4 ( const Complex & a, const Complex & b, const Complex & c, 
                 const Complex & d, Complex & x )
{
    const Complex t = x * ( x * ( x * 4 + a * 3 ) + b * 2 ) + c;
    if ( qmod ( t ) > 1e-6 )
    {
        x -= ( x * ( x * ( x * ( x + a ) + b ) + c ) + d ) / t;
    }
}

void root4 ( const Complex & a, const Complex & b, const Complex & c, const Complex & d, 
             Complex & x1, Complex & x2, Complex & x3, Complex & x4 )
{
    if ( ! a )
    {
        root4s ( b, c, d, x1, x2, x3, x4 );
        return;
    }
    if ( ! d )
    {
        x1 = d;
        root3 ( a, b, c, x2, x3, x4 );
        return;
    }
    const Complex e = a / 4;
    const Complex h = e * e;
    const Complex p = -6 * h + b;
    const Complex q =  8 * h * e - 2 * b * e + c;
    const Complex r = -3 * h * h + b * h - c * e + d;
    root4s ( p, q, r, x1, x2, x3, x4 );
// Уточнение корней методом Ньютона
    tuneRoot4 ( a, b, c, d, x1 -= e );
    tuneRoot4 ( a, b, c, d, x2 -= e );
    tuneRoot4 ( a, b, c, d, x3 -= e );
    tuneRoot4 ( a, b, c, d, x4 -= e );
}

//************************ 12.12.2011 *************************//
//
//      Решение алгебраического уравнения 5-ой степени
//      с действительными коэффициентами
//      x^5 + a * x^4 + b * x^3 + c * x^2 + d * x + e = 0
//
//************************ 12.12.2011 *************************//

double root5 ( double & a, double & b, double & c, double & d, double e );

void root5 ( double a, double b, double c, double d, double e, double & x1,
             Complex & x2, Complex & x3, Complex & x4, Complex & x5 )
{
    x1 = root5 ( a, b, c, d, e );
    root4 ( a, b, c, d, x2, x3, x4, x5 );
}