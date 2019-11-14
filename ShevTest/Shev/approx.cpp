
#include "math.h"
#include "approx.h"
#include "mathem.h"
#include "Matrix.h"

//********************** 16.10.2004 *********************************//
//
//      Аппроксимация функции полиномом.
//      Обратите внимание, что m - это не степень полинома, а к-во коэффициентов.
//      Степень полинома равна m-1. 
//
//********************** 10.09.2015 *********************************//

bool apprLSS ( nat n, CArrRef<double> x, CArrRef<double> y, nat m, ArrRef<double> c,
               bool (*lss) ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x ) )
{
    if ( ! n || ! m ) return false;
    if ( n < m )
    {
        for ( nat j = n; j < m; ++j ) c[j] = 0.;
        m = n;
    }
    HMatrix<double> a ( n, m );
    for ( nat i = 0; i < n; ++i )
    {
        a[i][0] = 1.;
        for ( nat j = 1; j < m; ++j ) a[i][j] = x[i] * a[i][j-1];
    }
    return lss ( a, y, c );
}

bool apprLSS_H ( nat n, CArrRef<double> x, CArrRef<double> y, nat m, ArrRef<double> c )
{
    return apprLSS ( n, x, y, m, c, lss_h );
}

bool apprLSS_O ( nat n, CArrRef<double> x, CArrRef<double> y, nat m, ArrRef<double> c )
{
    return apprLSS ( n, x, y, m, c, ortholin );
}

// С весами w

bool apprLSS ( nat n, CArrRef<double> x, CArrRef<double> y, CArrRef<double> w, nat m, ArrRef<double> c,
               bool (*lss) ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x ) )
{
    if ( ! n || ! m ) return false;
    if ( n < m )
    {
        for ( nat j = n; j < m; ++j ) c[j] = 0.;
        m = n;
    }
    HMatrix<double> a ( n, m );
    CmbArray<double, 80> b ( n );
    for ( nat i = 0; i < n; ++i )
    {
        a[i][0] = w[i];
        for ( nat j = 1; j < m; ++j ) a[i][j] = x[i] * a[i][j-1];
        b[i] = w[i] * c[i];
    }
    return lss ( a, y, b );
}

bool apprLSS_H ( nat n, CArrRef<double> x, CArrRef<double> y, CArrRef<double> w, nat m, ArrRef<double> c )
{
    return apprLSS ( n, x, y, w, m, c, lss_h );
}

bool apprLSS_O ( nat n, CArrRef<double> x, CArrRef<double> y, CArrRef<double> w, nat m, ArrRef<double> c )
{
    return apprLSS ( n, x, y, w, m, c, ortholin );
}

//************************* 11.09.2015 ******************************//
//
//        Робастный метод аппроксимации функции полиномом.
//        Заполняет массив весов (w) значениями от 0 до 1.
//
//************************* 11.09.2015 ******************************//

inline double polynom ( CArrRef<double> c, double x )
{
    double f = 0;
    for ( nat i = c.size(); i > 0; )
    {
        f *= x;
        f += c[--i];
    }
    return f;
}

bool apprLSS_R ( nat n, CArrRef<double> x, CArrRef<double> y, ArrRef<double> w, nat m, ArrRef<double> c )
{
    if ( ! apprLSS ( n, x, y, m, c, ortholin ) ) return false;
    for ( nat j = 0; j < n; ++j ) w[j] = 1.;
    if ( n > 3 )
    {
        DynArray<double> u ( n );
        for ( nat i = 0; i < 20; ++i )
        {
            double mean = 0.;
            nat j;
            for ( j = 0; j < n; ++j )
            {
                u[j] = w[j];
                mean += ( w[j] = fabs ( polynom ( c, x[j] ) ) );
            }
            if ( mean == 0 )
            {
                for ( j = 0; j < n; ++j ) w[j] = 1.;
                break;
            }
            mean /= n;
            for ( j = 0; j < n; ++j )
            {
                const double t = w[j] / mean;
                w[j] = t < 2. ? 0.25 * _pow2 ( 2. - t ) : 0.;
            }
            apprLSS ( n, x, y, w, m, c, ortholin );
            mean = 0.;
            for ( j = 0; j < n; ++j ) mean += fabs ( w[j] - u[j] );
            if ( mean < 0.01 * n ) break;
        }
    }
    return true;
}