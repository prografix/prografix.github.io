
#include "math.h"
#include "opti1d.h"

//************************* 11.06.2008 **************************************//
//
//        Поиск минимума или максимума функции на заданном интервале 
//                  Метод "золотого сечения"
//
//************************* 26.04.2010 **************************************//

namespace {

// Вычислить macheps, относительную машинную точность

double getEps ()
{
    double eps = 1.;
    for(;;)
    {
        eps /= 2.;
        double t = 1. + eps;
        if ( t == 1. ) break;
    }
    return eps;
}

const double macheps = getEps ();

const double goldenRatio1 = 0.5 * ( sqrt ( 5. ) - 1. );
const double goldenRatio2 = 1. - goldenRatio1;

} // namespace

double goldenRatioMin ( double a, double b, const MathFunc1 & func, double eps )
{
    if ( a > b ) _swap ( a, b );
    const double min = ( fabs(a) + fabs(b) ) * macheps * 4.;
    if ( eps < min ) eps = min;
    double x1 = a * goldenRatio1 + b * goldenRatio2;
    double x2 = b * goldenRatio1 + a * goldenRatio2;
    double y1 = func ( x1 );
    double y2 = func ( x2 );
    while ( b - a > eps )
    {
        if ( y1 < y2 )
        {
            b = x2;
            x2 = x1;
            x1 = x2 * goldenRatio1 + a * goldenRatio2;
            y2 = y1;
            y1 = func ( x1 );
        }
        else
        {
            a = x1;
            x1 = x2;
            x2 = x1 * goldenRatio1 + b * goldenRatio2;
            y1 = y2;
            y2 = func ( x2 );
        }
    }
    return y1 < y2 ? x1 : x2;
}

double goldenRatioMax ( double a, double b, const MathFunc1 & func, double eps )
{
    if ( a > b ) _swap ( a, b );
    const double min = ( fabs(a) + fabs(b) ) * macheps * 4.;
    if ( eps < min ) eps = min;
    double x1 = a * goldenRatio1 + b * goldenRatio2;
    double x2 = b * goldenRatio1 + a * goldenRatio2;
    double y1 = func ( x1 );
    double y2 = func ( x2 );
    while ( b - a > eps )
    {
        if ( y1 > y2 )
        {
            b = x2;
            x2 = x1;
            x1 = x2 * goldenRatio1 + a * goldenRatio2;
            y2 = y1;
            y1 = func ( x1 );
        }
        else
        {
            a = x1;
            x1 = x2;
            x2 = x1 * goldenRatio1 + b * goldenRatio2;
            y1 = y2;
            y2 = func ( x2 );
        }
    }
    return y1 > y2 ? x1 : x2;
}

//************************* 26.04.2010 **************************************//
//
//        Поиск минимума или максимума функции на заданном интервале 
//                  Комбинированный метод 
//
//************************* 26.04.2010 **************************************//

double fmin ( double a, double b, const MathFunc1 & func, double tol )
{
    static const double eps = sqrt ( macheps ) / 2048; // делителя 2048 в книге не было
    if ( a > b ) _swap ( a, b );
// Присвоение начальных значений
    double d = 0, e = 0, x, v, w, fx, fv, fw;
    x = v = w = a + goldenRatio2 * ( b - a );
    fx = fv = fw = func ( x );
// Основной цикл
    for(;;)
    {
        const double xm = 0.5 * ( a + b );
        const double tol1 = eps * fabs ( x ) + tol / 3;
        const double tol2 = tol1 + tol1;
// Проверить критерий окончания
        if ( fabs ( x - xm ) + 0.5 * ( b - a ) <= tol2 ) break;
// Парабола или золотое сечение?
        if ( fabs ( e ) > tol1 )
        {
// Построить параболу
            double r = ( x - w ) * ( fx - fv );
            double q = ( x - v ) * ( fx - fw );
            double p = ( x - v ) * q - ( x - w ) * r;
            q = 2 * ( q - r );
            if ( q > 0 ) p = -p;
            q = fabs ( q );
            r = e;
            e = d;
// Приемлима ли парабола?
            if ( p <= q * ( a - x ) ) goto m40;
            if ( p >= q * ( b - x ) ) goto m40;
            if ( fabs ( p ) >= fabs ( 0.5 * q * r ) ) goto m40;
// Шаг параболической интерполяции
            d = p / q;
            const double u = x + d;
// Не следует вычислять слишком близко к a или b 
            if ( u - a < tol2 || b - u < tol2 ) d = xm < x ? - tol1 : tol1;
        }
        else
        {
// Золотое сечение
m40:        e = x < xm ? b - x : a - x;
            d = goldenRatio2 * e;
        }
// Не следует вычислять слишком близко к x 
        const double u = fabs ( d ) >= tol1 ? x + d : d < 0 ? x - tol1 : x + tol1;
        const double fu = func ( u );
        if ( fu <= fx )
        {
            if ( u < x ) b = x;
            else         a = x;
            v = w;
            fv = fw;
            w = x;
            fw = fx;
            x = u;
            fx = fu;
        }
        else
        {
            if ( u < x ) a = u;
            else         b = u;
            if ( fu <= fw || w == x )
            {
                v = w;
                fv = fw;
                w = u;
                fw = fu;
            }
            else
            if ( fu <= fv || v == x || v == w )
            {
                v = u;
                fv = fu;
            }
        }
    }
    return x;
}

double fmax ( double a, double b, const MathFunc1 & func, double tol )
{
    static const double eps = sqrt ( macheps ) / 2048; // делителя 2048 в книге не было
    if ( a > b ) _swap ( a, b );
// Присвоение начальных значений
    double d = 0, e = 0, x, v, w, fx, fv, fw;
    x = v = w = a + goldenRatio2 * ( b - a );
    fx = fv = fw = func ( x );
// Основной цикл
    for(;;)
    {
        const double xm = 0.5 * ( a + b );
        const double tol1 = eps * fabs ( x ) + tol / 3;
        const double tol2 = tol1 + tol1;
// Проверить критерий окончания
        if ( fabs ( x - xm ) + 0.5 * ( b - a ) <= tol2 ) break;
// Парабола или золотое сечение?
        if ( fabs ( e ) > tol1 )
        {
// Построить параболу
            double r = ( x - w ) * ( fx - fv );
            double q = ( x - v ) * ( fx - fw );
            double p = ( x - v ) * q - ( x - w ) * r;
            q = 2 * ( q - r );
            if ( q > 0 ) p = -p;
            q = fabs ( q );
            r = e;
            e = d;
// Приемлима ли парабола?
            if ( p <= q * ( a - x ) ) goto m40;
            if ( p >= q * ( b - x ) ) goto m40;
            if ( fabs ( p ) >= fabs ( 0.5 * q * r ) ) goto m40;
// Шаг параболической интерполяции
            d = p / q;
            const double u = x + d;
// Не следует вычислять слишком близко к a или b 
            if ( u - a < tol2 || b - u < tol2 ) d = xm < x ? - tol1 : tol1;
        }
        else
        {
// Золотое сечение
m40:        e = x < xm ? b - x : a - x;
            d = goldenRatio2 * e;
        }
// Не следует вычислять слишком близко к x 
        const double u = fabs ( d ) >= tol1 ? x + d : d < 0 ? x - tol1 : x + tol1;
        const double fu = func ( u );
        if ( fu >= fx )
        {
            if ( u < x ) b = x;
            else         a = x;
            v = w;
            fv = fw;
            w = x;
            fw = fx;
            x = u;
            fx = fu;
        }
        else
        {
            if ( u < x ) a = u;
            else         b = u;
            if ( fu >= fw || w == x )
            {
                v = w;
                fv = fw;
                w = u;
                fw = fu;
            }
            else
            if ( fu >= fv || v == x || v == w )
            {
                v = u;
                fv = fu;
            }
        }
    }
    return x;
}

//************************* 02.03.2017 **************************************//
//
//          Поиск минимакса на множестве парабол вида x*x + a*x + b
//
//************************* 02.03.2017 **************************************//

inline double func ( double x, const Set2<double> & s )
{
    return x * ( x + s.a ) + s.b;
}

Def<double> getMinMaxParabola1d ( CArrRef<Set2<double> > arr )
{
    const nat n = arr.size();
    if ( ! n ) return Def<double>();
    double x = -0.5 * arr[0].a;
    if ( n == 1 ) return x;
    double y = func ( x, arr[0] );
    CmbSuite<nat, 2> index;
    index.inc() = 0;
    for(;;)
    {
        nat im = index[0];
        double max = y;
        for ( nat i = 0; i < n; ++i )
        {
            const double t = func ( x, arr[i] );
            if ( max < t ) max = t, im = i;
        }
        if ( hasEqu ( index, im ) ) break;
        const Set2<double> s0 = arr[index[0]];
        const Set2<double> si = arr[im];
        const double t = -0.5 * si.a;
        const double f = t * si.a + si.b;
        if ( index.size() == 1 )
        {
            if ( t * s0.a + s0.b <= f )
            {
                index[0] = im;
                x = t;
            }
            else
            {
                index.inc() = im;
                x = ( si.b - s0.b ) / ( s0.a - si.a );
            }
        }
        else
        {
            const Set2<double> s1 = arr[index[1]];
            if ( t * s0.a + s0.b <= f && t * s1.a + s1.b <= f )
            {
                index.resize ( 1 );
                index[0] = im;
                x = t;
            }
            else
            {
                if ( x + x + si.a > 0 == s1.a > s0.a )
                {
                    index[1] = im;
                    x = ( si.b - s0.b ) / ( s0.a - si.a );
                }
                else
                {
                    index[0] = im;
                    x = ( si.b - s1.b ) / ( s1.a - si.a );
                }
            }
        }
        const double z = func ( x, arr[index[0]] );
        if ( z <= y ) break;
        y = z;
    }
    return x;
}