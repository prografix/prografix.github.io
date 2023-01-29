
#include "Rand.h"
#include "func1d.h"
#include "func1t.h"
#include "median.h"
#include "sorting.h"

//*********************** 30.08.2006 **************************//
//
//          Порядковые статистики ( 0 <= i < a.size() )
//
//*********************** 05.09.2012 **************************//

static double median7 ( nat n, double * a )
{
    nat i = n / 2;
    double * s = a;
    double * d = a + n;
    double res;
    for(;;)
    {
        if ( n <= 7 )
        {
            insertSort123 ( ArrRef<double> ( s, n ) );
            return s[i];
        }
        const nat m = n / 7;
        nat j;
        for ( j = 0; j < m; ++j ) d[j] = _median7 ( s + 7 * j );
        res = median7 ( m, d );
        nat j1 = 0;
        nat j2 = n - 1;
        for ( j = 0; j < n; ++j )
        {
            if ( s[j] < res ) d[j1++] = s[j]; else
            if ( s[j] > res ) d[j2--] = s[j];
        }
        if ( j1 <= i )
        {
            if ( i <= j2 ) break;
            ++j2;
            n -= j2;
            s = d + j2;
            i -= j2;
        }
        else
        {
            n = j1;
            s = d; 
        }
        d = d == a ? a + n : a;
    }
    return res;
}

Def<double> select7 ( CArrRef<double> a, nat i )
{
    Def<double> res;
    nat n = a.size();
    if ( i >= n ) return res;
    const double * s = a(0);
    CmbArray<double, 80> buf ( n + n );
    double * buf1 = buf(0);
    double * buf2 = buf(n);
    double * d = buf1;
    for(;;)
    {
        if ( n <= 7 )
        {
            FixArray<double, 7> arr;
            for ( nat k = 0; k < n; ++k ) arr[k] = s[k];
            insertSort123 ( ArrRef<double> ( arr, 0, n ) );
            res = arr[i];
            break;
        }
        const nat m = n / 7;
        nat j;
        for ( j = 0; j < m; ++j ) d[j] = _median7 ( s + 7 * j );
        res = median7 ( m, d );
        nat j1 = 0;
        nat j2 = n - 1;
        for ( j = 0; j < n; ++j )
        {
            if ( s[j] < res ) d[j1++] = s[j]; else
            if ( s[j] > res ) d[j2--] = s[j];
        }
        if ( j1 <= i )
        {
            if ( i <= j2 ) break;
            ++j2;
            n -= j2;
            s = d + j2;
            i -= j2;
        }
        else
        {
            n = j1;
            s = d; 
        }
        d = d == buf1 ? buf2 : buf1;
    }
    return res;
}

//*********************** 09.01.2015 **************************//
//
//              Сглаженные порядковые статистики
//              0 <= p <= 1
//              r - радиус окна усреднения
//              Прямоугольная функция сглаживания
//
//*********************** 09.01.2015 **************************//

Def<double> selectS ( ArrRef<SortItem<double> > s, double p, double r )
{
    Def<double> res;
    const nat n = s.size();
    if ( n == 0 || p < 0 || p > 1 ) return res;
    double sum = 0;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        SortItem<double> & item = s[i];
        if ( item.tail < 0 ) return res;
        sum += item.tail;
    }
    if ( ! sum ) return res;
    if ( n == 1 )return s[0].head;
    quickSort123 ( s );
    if ( r < 1e-6 ) r = 1e-6;
    r *= sum;
    p *= sum;
    const double a = _max ( p - r, 0. );
    const double b = _min ( p + r, sum );
    sum = 0;
    double ai = 0;
    double bi = s[0].tail;
    for ( i = 0;; )
    {
        if ( bi > a )
        {
            if ( ai >= b ) break;
            sum += s[i].head * ( _min ( b, bi ) - _max ( a, ai ) );
        }
        if ( ++i == n ) break;
        ai = bi;
        bi += s[i].tail;
    }
    return sum / ( b - a );
}

//*********************** 06.03.2015 **************************//
//
//              Сглаженные порядковые статистики
//              0 <= p <= 1
//              r - радиус окна усреднения
//              Треугольная функция сглаживания
//
//*********************** 06.03.2015 **************************//

Def<double> selectT ( ArrRef<SortItem<double> > s, double p, double r )
{
    Def<double> res;
    const nat n = s.size();
    if ( n == 0 || p < 0 || p > 1 ) return res;
    double sum = 0;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        SortItem<double> & item = s[i];
        if ( item.tail < 0 ) return res;
        sum += item.tail;
    }
    if ( ! sum ) return res;
    if ( n == 1 ) return s[0].head;
    quickSort123 ( s );
    if ( r < 1e-6 ) r = 1e-6;
    r *= sum;
    p *= sum;
    sum = 0;
    const double a = p - r;
    const double b = p + r;
    double w = 0;
    double ai = 0;
    double bi = s[0].tail;
    for ( i = 0;; )
    {
        if ( bi > a )
        {
            if ( ai >= b ) break;
            if ( bi > p )
            {
                const double c = _max ( p, ai );
                const double d = _min ( b, bi );
                const double u = ( d - c ) * ( b - d + b - c );
                sum += s[i].head * u;
                w += u;
            }
            if ( p > ai )
            {
                const double c = _max ( a, ai );
                const double d = _min ( p, bi );
                const double u = ( d - c ) * ( d - a + c - a );
                sum += s[i].head * u;
                w += u;
            }
        }
        if ( ++i == n ) break;
        ai = bi;
        bi += s[i].tail;
    }
    return sum / w;
}
