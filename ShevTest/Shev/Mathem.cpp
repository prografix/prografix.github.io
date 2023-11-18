
#include "math.h"
#include "Mathem.h"
#include "LinAlg.h"
#include "heap.h"
#include "func1t.h"

//*********************** 13.05.2004 ************************//
//
//                   Арифметические корни
//
//*********************** 25.06.2013 ************************//

static double _root3 ( double x )
{
    double s = 1.;
    while ( x < 1. )
    {
        x *= 8.;
        s *= 0.5;
    }
    while ( x > 8. )
    {
        x *= 0.125;
        s *= 2.;
    }
    double r = 1.5;
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    r -= 1./3. * ( r - x / ( r * r ) );
    return r * s;
}

double root3 ( double x )
{
    if ( x > 0 ) return _root3 ( x ); else
    if ( x < 0 ) return-_root3 (-x ); else
    return 0.;
}

double root4 ( double x )
{
    return sqrt ( sqrt ( x ) );
}

static double _root5 ( double x )
{
    double s = 1.;
    while ( x < 1. )
    {
        x *= 32.;
        s *= 0.5;
    }
    while ( x > 32. )
    {
        x *= 0.03125;
        s *= 2.;
    }
    double r = 1.5;
    r -= 1./5. * ( r - x / _pow2 ( r * r ) );
    r -= 1./5. * ( r - x / _pow2 ( r * r ) );
    r -= 1./5. * ( r - x / _pow2 ( r * r ) );
    r -= 1./5. * ( r - x / _pow2 ( r * r ) );
    r -= 1./5. * ( r - x / _pow2 ( r * r ) );
    r -= 1./5. * ( r - x / _pow2 ( r * r ) );
    r -= 1./5. * ( r - x / _pow2 ( r * r ) );
    return r * s;
}

double root5 ( double x )
{
    if ( x > 0 ) return _root5 ( x ); else
    if ( x < 0 ) return-_root5 (-x ); else
    return 0.;
}

double root6 ( double x )
{
    return sqrt ( root3 ( x ) );
}

static double _root7 ( double x )
{
    double s = 1.;
    while ( x < 1. )
    {
        x *= 128.;
        s *= 0.5;
    }
    while ( x > 128. )
    {
        x *= 0.0078125;
        s *= 2.;
    }
    double r = 1.5;
    r -= 1./7. * ( r - x / _pow3 ( r * r ) );
    r -= 1./7. * ( r - x / _pow3 ( r * r ) );
    r -= 1./7. * ( r - x / _pow3 ( r * r ) );
    r -= 1./7. * ( r - x / _pow3 ( r * r ) );
    r -= 1./7. * ( r - x / _pow3 ( r * r ) );
    r -= 1./7. * ( r - x / _pow3 ( r * r ) );
    r -= 1./7. * ( r - x / _pow3 ( r * r ) );
    r -= 1./7. * ( r - x / _pow3 ( r * r ) );
    return r * s;
}

double root7 ( double x )
{
    if ( x > 0 ) return _root7 ( x ); else
    if ( x < 0 ) return-_root7 (-x ); else
    return 0.;
}

double root8 ( double x )
{
    return sqrt ( sqrt ( sqrt ( x ) ) );
}

double root9 ( double x )
{
    return root3 ( root3 ( x ) );
}

double root10 ( double x )
{
    return sqrt ( root5 ( x ) );
}

//*************************************************************//
//
//      Решение алгебраических уравнений 2, 3 и 4 степени.
//      Возвращаемое значение - количество найденных корней.
//
//************************ 10.08.2018 *************************//

// a * x^2 + b * x + c = 0 - общее квадратное уравнение

nat root2 ( double a, double b, double c, double x[2] )
{
    if ( a == 0 )
    {
        if ( c == 0 )
        {
            x[0] = 0.;
        }
        else
        {
            if ( b == 0 ) return 0;
            x[0] = -c / b;
        }
        return 1;
    }
    else
    {
        if ( c == 0 )
        {
            x[0] = 0.;
            x[1] = -b / a;
        }
        else
        {
            b *= -0.5;
            double d = b * b - a * c;
            if ( d < 0 ) return 0;
            d = sqrt ( d );
            double t = b > 0 ? b + d : b - d;
            x[0] = c / t;
            x[1] = t / a;
        }
    }
    return 2;
}

// x^2 + a * x + b = 0 - приведённое квадратное уравнение

nat root2 ( double a, double b, double x[2] )
{
    if ( b == 0 )
    {
        x[0] = 0.;
        x[1] = -a;
    }
    else
    {
        a *= -0.5;
        double d = a * a - b;
        if ( d < 0 ) return 0;
        d = sqrt ( d );
        x[1] = a > 0 ? a + d : a - d;
        x[0] = b / x[1];
    }
    return 2;
}

// x^3 + a * x + b = 0

double cubic ( double a, double b )
{
    double s = 1.;
    while ( b + a > -1. )
    {
        a *= 4.;
        b *= 8.;
        s *= 0.5;
    }
    while ( b + a + a < -8. )
    {
        a *= 0.25;
        b *= 0.125;
        s *= 2.;
    }
    double x = 1.5;
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    x -= ( x * ( x * x + a ) + b ) / ( 3. * x * x + a );
    return x * s;
}

// x^3 + p * x + q = 0

nat root3s ( double p, double q, double * x )
{
    if ( q < 0 )
    {
        *x = cubic ( p, q );
    }
    else
    if ( q > 0 )
    {
        *x = -cubic ( p, -q );
    }
    else
    {
        *x = 0;
    }
    return 1 + root2 ( *x, p + (*x)*(*x), x+1 );
}

// x^3 + a * x^2 + b * x + c = 0

nat root3 ( double a, double b, double c, double * x )
{
    if ( c == 0 )
    {
        *x = 0;
    }
    else
    {
        const double a3 = a / 3.;
        const double p = b - a3 * a;
        const double q = c - ( a3 * a3 + p ) * a3;
        if ( q < 0 )
        {
            *x = cubic ( p, q );
        }
        else
        if ( q > 0 )
        {
            *x = -cubic ( p, -q );
        }
        else
        {
            *x = 0;
        }
        *x -= a3;
        const double t = *x * ( *x * 3. + a * 2. ) + b;
        if ( fabs ( t ) > 1e-3 )
        {
            *x -= ( *x * ( *x * ( *x + a ) + b ) + c ) / t;
        }
        a += *x;
        b += *x * a;
    }
    return 1 + root2 ( a, b, x+1 );
}

// x^4 + p * x^2 + q * x + r = 0

nat root4s ( double p, double q, double r, double * x )
{
    if ( r == 0 ) // это условие добавлено 21.10.2008
    {
        *x = 0;
        return 1 + root3s ( p, q, x+1 );
    }
    if ( q == 0 )
    {
m1:     double t[2];
        nat n = root2 ( p, r, t );
        if ( n == 0 ) return 0;
        if ( t[0] >= 0 )
        {
            x[0] = sqrt ( t[0] );
            x[1] = - x[0];
            n = 2;
        }
        else
        {
            n = 0;
        }
        if ( t[1] >= 0 )
        {
            x[n] = sqrt ( t[1] );
            x[n+1] = - x[n];
            n += 2;
        }
        return n;
    }
    nat n = root3 ( p + p, p * p - 4. * r, -q * q, x );
    double a = *x;
    if ( n == 3 )
    {
        if ( a < x[1] ) a = x[1];
        if ( a < x[2] ) a = x[2];
    }
    if ( a <= 0 ) goto m1; // этот переход сделан 09.12.2011
    p += a;
    a = sqrt ( a );
    const double b = q / a;
    n = root2 (  a, 0.5 * ( p - b ), x   );
    n+= root2 ( -a, 0.5 * ( p + b ), x+n );
    return n;
}

// x^4 + a * x^3 + b * x^2 + c * x + d = 0

nat root4 ( double a, double b, double c, double d, double * x )
{
    if ( a == 0 )
    {
        return root4s ( b, c, d, x );
    }
    if ( d == 0 )
    {
        *x = 0;
        return 1 + root3 ( a, b, c, x+1 );
    }
    const double e = a / 4;
    const double h = e * e;
    const double p = -6 * h + b;
    const double q =  8 * h * e - 2 * b * e + c;
    const double r = -3 * h * h + b * h - c * e + d;
    const nat n = root4s ( p, q, r, x );
    for ( nat i = 0; i < n; ++i ) x[i] -= e;
    return n;
}

//************************ 22.05.2011 *************************//
//
//      Решение алгебраического уравнения 5-ой степени
//      x^5 + a * x^4 + b * x^3 + c * x^2 + d * x + e = 0
//
//************************ 12.12.2011 *************************//

double root5 ( double & a, double & b, double & c, double & d, double e )
{
    if ( e == 0 ) return 0;
    double x;
    Polynom5 func;
    func.a = 1;
    func.b = a;
    func.c = b;
    func.d = c;
    func.e = d;
    func.f = e;
    if ( e < 0 )
    {
        if ( func ( 1 ) < 0 )
        {
            double g = - e;
            if ( d < 0 ) g -= d;
            if ( c < 0 ) g -= c;
            if ( b < 0 ) g -= b;
            if ( a < 0 ) g -= a;
            zeroin ( 1, g, func, 0, x );
        }
        else
            zeroin ( 0, 1, func, 0, x );
    }
    else
    {
        if ( func ( -1 ) > 0 )
        {
            double g = - e;
            if ( d < 0 ) g += d;
            if ( c > 0 ) g -= c;
            if ( b < 0 ) g += b;
            if ( a > 0 ) g -= a;
            zeroin ( g, -1, func, 0, x );
        }
        else
            zeroin ( -1, 0, func, 0, x );
    }
    a += x;
    b += x * a;
    c += x * b;
    d += x * c;
    return x;
}

nat root5 ( double a, double b, double c, double d, double e, double * x )
{
    x[0] = root5 ( a, b, c, d, e );
    return 1 + root4 ( a, b, c, d, x+1 );
}

//************************ 26.10.2006 *************************//
//
//      Решение алгебраических уравнений n-ой степени.
//
//************************ 26.10.2006 *************************//

bool rootN ( nat n, const double * p, double * r, double * i )
{
    if ( n == 0 ) return false;
    if ( n == 1 )
    {
        r[0] = - p[0];
        i[0] = 0.;
        return true;
    }
    HMatrix<double> a ( n, n );
    a.fill ( 0. );
    a[0][0] = - p[0];
    for ( nat k = 1; k < n; ++k )
    {
        a[0][k] = - p[k];
        a[k][k-1] = 1.;
    }
    return hqr ( n, a, r, i );
}

//*********************************************************************//
//
//      Решение систем линейных уравнений методом Гаусса.
//
//*************************** 24.04.2007 ******************************//

SLU_Gauss::SLU_Gauss ( nat m, const double * const * u ) : n ( m ), cond ( 1e32 ), a ( m, m )
{
    if ( n < 1 ) return;
    nat i, j, k;
    for ( i = 0; i < n; ++i )
    {
        for ( j = 0; j < n; ++j ) a[i][j] = u[i][j];
    }
    const nat n1 = n - 1;
    ipvt.resize(n);
    ipvt[n1] = 1;
    if ( n == 1 )
    {
        if ( a[0][0] != 0 ) cond = 1.;
        return;
    }
// Вычисляем 1-норму матрицы A
    double anorm = 0.;
    for ( j = 0; j < n; ++j )
    {
        double t = 0.;
        for ( i = 0; i < n; ++i ) t += fabs ( a[i][j] );
        if ( anorm < t ) anorm = t;
    }
// Гауссово исключение с частичным выбором ведущего элемента
    for ( k = 0; k < n1; ++k )
    {
        nat m = k;
        const nat kp1 = k + 1;
        for ( i = kp1; i < n; ++i )
        {
            if ( fabs ( a[i][k] ) > fabs ( a[m][k] ) ) m = i;
        }
        ipvt[k] = m;
        double t = a[m][k];
        if ( t == 0 ) return;
        if ( m != k )
        {
            ipvt[n1] = 1 - ipvt[n1];
            a[m][k] = a[k][k];
            a[k][k] = t;
        }
        t = -1. / t;
        for ( i = kp1; i < n; ++i ) a[i][k] = a[i][k] * t;
        for ( j = kp1; j < n; ++j )
        {
            double t = a[m][j];
            a[m][j] = a[k][j];
            a[k][j] = t;
            if ( t == 0 ) continue;
            for ( i = kp1; i < n; ++i )
            {
                if ( fabs ( a[i][j] += a[i][k] * t ) < 1e-290 ) a[i][j] = 0;
            }
        }
    }
    if ( ! a[n1][n1] ) return; // Эта строка добавлена 12.05.2004
// Вычисление обусловленности
    DynArray<double> work ( n );
    for ( k = 0; k < n; ++k )
    {
        double t = 0.;
        for ( i = 0; i + 1 < k; ++i )
        {
            t += a[i][k] * work[i];
        }
        double ek = t < 0 ? -1. : 1.;
        work[k] = - ( ek + t ) / a[k][k];
    }
    for ( k = n1; k-- > 0; )
    {
        double t = 0.;
        const nat kp1 = k + 1;
        for ( i = kp1; i < n; ++i )
        {
            t += a[i][k] * work[k];
        }
        work[k] = t;
        const nat m = ipvt[k];
        if ( m != k )
        {
            t = work[m];
            work[m] = work[k];
            work[k] = t;
        }
    }
    double ynorm = 0.;
    for ( k = 0; k < n; ++k ) ynorm += fabs ( work[k] );
    cond = 1.; // для того, чтобы сработал solve
    solve ( work(), work() );
    double znorm = 0.;
    for ( k = 0; k < n; ++k ) znorm += fabs ( work[k] );
    cond = anorm * znorm / ynorm;
    if ( cond < 1. ) cond = 1.;
}

bool SLU_Gauss::solve ( const double * b, double * x ) const // b[n], x[n]
{
    if ( cond + 1. == cond ) return false;
    if ( b != x )
    {
        for ( nat i = 0; i < n; ++i ) x[i] = b[i];
    }
// Прямой ход
    const nat n1 = n - 1;
    for ( nat k = 0; k < n1; ++k )
    {
        const nat m = ipvt[k];
        const double t = x[m];
        x[m] = x[k];
        x[k] = t;
        for ( nat i = k + 1; i < n; ++i ) x[i] += a[i][k] * t;
    }
// Обратная подстановка
    for ( nat k1 = n - 1; k1-- > 0; )
    {
        const nat k = k1 + 1;
        x[k] /= a[k][k];
        const double t = - x[k];
        for ( nat i = 0; i <= k1; ++i ) x[i] += a[i][k] * t;
    }
    x[0] /= a[0][0];
    return true;
}

bool SLU_Gauss::solve ( const double * const * u, const double * b, double * x ) const
{
    DynArray<double> y ( 4*n );
    if ( ! solve ( b, y() ) ) return false;
    double d0 = 0.;
    double * r = &y[n];
    nat i, j;
    for ( i = 0; i < n; ++i )
    {
        r[i] = b[i];
        for ( j = 0; j < n; ++j ) r[i] -= u[i][j] * y[j]; 
        d0 += r[i] * r[i];
    }
    double * e = r + n;
    double * z = e + n;
    for (;;)
    {
        solve ( r, e );
        for ( i = 0; i < n; ++i )
        {
            z[i] = y[i];
            y[i] += e[i];
        }
        double d1 = 0.;
        for ( i = 0; i < n; ++i )
        {
            r[i] = b[i];
            for ( j = 0; j < n; ++j ) r[i] -= u[i][j] * y[j];
            d1 += r[i] * r[i];
        }
        if ( 4. * d1 >= d0 )
        {
            if ( d1 < d0 )
            {
                for ( i = 0; i < n; ++i ) x[i] = y[i];
            }
            else
            {
                for ( i = 0; i < n; ++i ) x[i] = z[i];
            }
            break;
        }
        d0 = d1;
    }
    return true;
}

double SLU_Gauss::determinant () const
{
    if ( cond + 1. == cond ) return 0.;
    double d = ipvt[n-1] == 1 ? 1. : -1.;
    for ( nat i = 0; i < n; ++i ) d *= a[i][i];
    return d;
}

//*************************** 31.07.2010 ******************************//
//
//      Решение систем линейных уравнений методом Гаусса.
//      Выбор ведущего элемента по столбцам.
//
//*************************** 12.03.2023 ******************************//

bool slu_gauss ( ArrRef2<double> & data )
{
    const nat n = data.size0();
    const nat n1 = data.size1();
    if ( n == 0 || n1 <= n ) return false;
// Прямой ход
    nat i, j, k;
    for ( k = 0; k < n; ++k )
    {
        const nat k1 = k + 1;
// Поиск максимального по модулю члена в k-том столбце
        nat m = k;
        double max = fabs ( data[m][k] );
        for ( i = k1; i < n; ++i )
        {
            const double t = fabs ( data[i][k] );
            if ( max < t ) max = t, m = i;
        }
        if ( max == 0 ) return false;
        const double p = 1. / data[m][k];
        ArrRef<double> rk = data[k];
        if ( m > k )
        {
// Меняем местами k-ую и m-ую строки
            ArrRef<double> rm = data[m];
            for ( i = k; i < n1; ++i )
            {
                _swap ( rk[i], rm[i] ); rk[i] *= p;
            }
        }
        else
        {
            for ( i = k1; i < n1; ++i ) rk[i] *= p;
        }
// Вычитание строк
        for ( j = k1; j < n; ++j )
        {
            ArrRef<double> rj = data[j];
            const double t = rj[k];
            for ( i = k1; i < n1; ++i ) rj[i] -= rk[i] * t;
        }
    }
// Обратная подстановка
    for ( j = n; --j > 0; )
    {
        CArrRef<double> rj = data[j];
        for ( i = 0; i < j; ++i )
        {
            ArrRef<double> ri = data[i];
            const double t = ri[j];
            for ( k = n; k < n1; ++k ) ri[k] -= rj[k] * t;
        }
    }
    return true;
}

bool slu_gauss ( ArrRef2<double> & data, const nat nc )
{
    const nat n = data.size0();
    const nat n1 = data.size1();
    if ( nc == 0 || nc > n || n1 <= n ) return false;
// Прямой ход
    nat i, j, k;
    for ( k = 0; k < nc; ++k )
    {
        const nat k1 = k + 1;
// Поиск максимального по модулю члена в k-том столбце
        nat m = k;
        double max = fabs ( data[m][k] );
        for ( i = k1; i < n; ++i )
        {
            const double t = fabs ( data[i][k] );
            if ( max < t ) max = t, m = i;
        }
        if ( max == 0 ) return false;
        const double p = 1. / data[m][k];
        double * rk = data(k);
        if ( m > k )
        {
// Меняем местами k-ую и m-ую строки
            double * rm = data(m);
            for ( i = k; i < n1; ++i )
            {
                _swap ( rk[i], rm[i] ); rk[i] *= p;
            }
        }
        else
        {
            for ( i = k1; i < n1; ++i ) rk[i] *= p;
        }
        rk[k] = 1;
// Вычитание строк
        for ( j = k1; j < n; ++j )
        {
            double * rj = data(j);
            const double t = rj[k];
            for ( i = k1; i < n1; ++i ) rj[i] -= rk[i] * t;
            rj[k] = 0;
        }
    }
// Обратная подстановка
    for ( j = nc; --j > 0; )
    {
        CArrRef<double> rj = data[j];
        for ( i = 0; i < j; ++i )
        {
            double * ri = data(i);
            const double t = ri[j];
            for ( k = nc; k < n1; ++k ) ri[k] -= rj[k] * t;
            ri[j] = 0;
        }
    }
    return true;
}

bool slu_gauss ( ArrRef2<double> & data, ArrRef<double> & x )
{
    const nat n = data.size0();
    if (  data.size1() != n + 1 || x.size() < n || ! slu_gauss ( data ) ) return false;
    for ( nat k = 0; k < n; ++k ) x[k] = data[k][n];
    return true;
}

//*************************** 24.03.2011 ******************************//
//
//      Решение систем линейных уравнений методом Гаусса
//      Общий выбор ведущего элемента
//      nc - к-во столбцов в левой части системы уравнений
//      col - массив индексов столбцов
//
//*************************** 24.03.2011 ******************************//

bool slu_gauss ( ArrRef2<double> & data, const nat nc, ArrRef<nat> & col )
{
    const nat nr = data.size0();
    const nat nn = data.size1();
    if ( nc > nn || nr > nc || nn > col.size() ) return false;
    nat i, j, k;
    for ( k = 0; k < nn; ++k ) col[k] = k;
    for ( k = 0; k < nr; ++k )
    {
        const nat k1 = k + 1;
// Поиск члена максимального по модулю
        nat mi, mj;
        double max = 0;
        for ( i = k; i < nr; ++i )
        {
            const double * r = data[i]();
            for ( j = k; j < nc; ++j )
            {
                const double t = fabs ( r[col[j]] );
                if ( max < t ) max = t, mi = i, mj = j;
            }
        }
        if ( max == 0 ) return false;
// Меняем местами k-ый и mj-ый столбцы
         _swap ( col[k], col[mj] );
        const nat ck = col[k];
        const double p = 1. / data[mi][ck];
        ArrRef<double> rk = data[k];
        if ( mi > k )
        {
// Меняем местами k-ую и mi-ую строки
            ArrRef<double> rm = data[mi];
            for ( j = k; j < nn; ++j )
            {
                const nat jj = col[j];
                _swap ( rk[jj], rm[jj] );
                rk[jj] *= p;
            }
        }
        else
        {
            for ( j = k1; j < nn; ++j ) rk[col[j]] *= p;
        }
// Вычитание строк
        for ( i = k1; i < nr; ++i )
        {
            ArrRef<double> ri = data[i];
            const double t = ri[ck];
            for ( j = k1; j < nn; ++j )
            {
                const nat cj = col[j];
                ri[cj] -= rk[cj] * t;
            }
        }
    }
// Обратная подстановка
    for ( k = nr; k < nn; ++k )
    {
        const nat ck = col[k];
        for ( j = nr; --j > 0; )
        {
            const nat cj = col[j];
            const double t = data[j][ck];
            for ( i = 0; i < j; ++i ) data[i][ck] -= data[i][cj] * t;
        }
    }
    return true;
}

//*********************************************************************//
//
//      Решение систем линейных уравнений методом
//
//      ортогонализации Грама-Шмидта ( n <= m )
//
//*************************** 19.06.2007 ******************************//

bool slu_ortho ( nat n, nat m, const double * const * a, const double * b, double * x )
{
    if ( n == 0 || n > m ) return false;
    const nat n1 = n + 1;
    const nat m1 = m + 1;
// Создаём и заполняем матрицу C[n1][m1]
    DynArray2<double> c ( n1, m1 );
    nat i, j, k;
    for ( i = 0; i < n; ++i )
    {
        const double * p = a[i];
        ArrRef<double> t = c[i];
        for ( j = 0; j < m; ++j ) t[j] = *p++;
        t[m] = b[i];
    }
    {
        ArrRef<double> t = c[n];
        for ( j = 0; j < m; ++j ) t[j] = 0.;
        t[m] = 1.;
    }
// Делаем строки матрицы С ортогональными
    for ( k = 0; k < n; ++k )
    {
        double q = 0.;
        CArrRef<double> p = c[k];
        for ( j = 0; j < m1; ++j ) q += p[j] * p[j];
        if ( q == 0 ) continue;
        q = 1. / q;
        for ( i = k+1; i < n1; ++i )
        {
            ArrRef<double> t = c[i];
            double s = 0.;
            for ( j = 0; j < m1; ++j ) s += t[j] * p[j];
            s *= q;
            for ( j = 0; j < m1; ++j ) t[j] -= s * p[j];
        }
    }
// Если решение найдено записываем результат в вектор x
    const bool ok = c[n][m] != 0;
    if ( ok )
    {
        CArrRef<double> p = c[n];
        const double t = - 1. / p[m];
        for ( i = 0; i < m; ++i ) x[i] = p[i] * t;
    }
    return ok;
}

//*************************** 01.06.2007 ******************************//
//
//      Решение систем линейных уравнений методом
//
//      ортогонализации при помощи отражений ( n <= m )
//
//*************************** 01.06.2007 ******************************//

bool slu_orthoH1 ( nat n, nat m, const double * const * a, const double * b, double * x )
{
    if ( n == 0 || n > m ) return false;
    HMatrix<double> mat ( n+1, m+1 );
    nat i, j;
    for ( i = 0; i < n; ++i )
    {
        for ( j = 0; j < m; ++j )
        {
            mat[i][j] = a[i][j];
        }
        mat[i][j] = b[i];
    }
    const nat rank = orthogonalizationH1 ( n, mat );
    if ( rank == 0 ) return false;
    const double * rn = mat[n];
    if ( rn[m] == 0 ) return false;
    const double d = -1. / rn[m];
    for ( j = 0; j < m; ++j )
    {
        x[j] = rn[j] * d;
    }
    return true;
}

nat slu_orthoH1 ( const IMatrix<double> & a, const double * b, IMatrix<double> & x )
{
    if ( a.nCol == 0 || a.nRow > a.nCol || x.nRow == 0 || x.nCol != a.nCol ) return 0;
    nat nc = a.nCol + 1;
    nat nr = a.nRow + x.nRow;
    if ( nr > nc ) nr = nc;
    HMatrix<double> mat ( nr, nc );
    nat i, j;
    for ( i = 0; i < a.nRow; ++i )
    {
        for ( j = 0; j < a.nCol; ++j )
        {
            mat[i][j] = a[i][j];
        }
        mat[i][j] = b[i];
    }
    const nat rank = orthogonalizationH1 ( a.nRow, mat );
    if ( rank == 0 ) return 0;
    nr = 0;
    for ( i = a.nRow; i < mat.nRow; ++i )
    {
        const double * ri = mat[i];
        if ( ri[a.nCol] == 0 ) continue;
        const double d = -1. / ri[a.nCol];
        double * rx = x[nr];
        for ( j = 0; j < a.nCol; ++j )
        {
            rx[j] = ri[j] * d;
        }
        ++nr;
    }
    return nr;
}


//*************************************************************//
//
//      Симметричные положительно определённые матрицы.
//              Разложение Холецкого.
//
//*************************************************************//

SM_Chol::SM_Chol ( nat m, const double * const * a ) : n(m)
{
    if ( n < 1 ) return;
    g.resize ( n*(n+1)/2 );
    const double * si = g();
    nat l = 0, i, j, k;
    for ( i = 0; i < n; ++i )
    {
        si += i;
        const double * sj = g();
        const double * ai = a[i];
        for ( j = 0; j <= i; ++j )
        {
            sj += j;
            double x = ai[j];
            const double * pi = si;
            const double * pj = sj;
            for ( k = j; k-- > 0; )
            {
                x -= (*pi++) * (*pj++);
            }
            if ( i == j )
            {
                if ( x <= 0 )
                {
                    g.resize(0);
                    return;
                }
                g[l] = 1. / sqrt(x);
            }
            else
            {
                g[l] = x * (*pj);
            }
            ++l;
        }
    }
}

bool SM_Chol::solve ( const double * b, double * x ) const
{
    if ( g.size() == 0 ) return false;
// Решение системы Ly = b
    nat p = 0, i, k;
    for ( i = 0; i < n; ++i )
    {
        double t = b[i];
        for ( k = 0; k < i; ++k )
        {
            t -= g[p] * x[k];
            ++p;
        }
        x[i] = t * g[p++];
    }
// Решение системы Ux = y;
    for ( i = n; i-- > 0; )
    {
        nat s = --p;
        double t = x[i];
        for ( k = n; --k > i; )
        {
            t -= g[s] * x[k];
            s -= k;
        }
        x[i] = t * g[s];
    }
    return true;
}

double SM_Chol::determinant () const 
{
    if ( g.size() == 0 ) return 0.;
    nat i, p = 0;
    double det = 1.;
    for ( i = 0; i < n; ++i )
    {
        det *= g[p];
        p += i + 2;
    }
    return 1. / ( det * det );
}

//*************************************************************//
//
//      Симметричные положительно определённые матрицы.
//              LDLt разложение.
//
//*************************************************************//

SM_LDLt::SM_LDLt ( nat m, const double * const * a ) : n(m)
{
    if ( n < 1 ) return;
    g.resize ( n*(n+1)/2 );
    const double * si = g();
    nat i, j, k, l = 0;
    for ( i = 0; i < n; ++i )
    {
        si += i;
        double * sj = g();
        const double * ai = a[i];
        for ( j = 0; j <= i; ++j )
        {
            sj += j;
            double x = ai[j];
            double * pj = sj;
            if ( i == j )
            {
                const double * p = g();
                for ( k = 0; k < j; ++k )
                {
                    const double y = *pj;
                    *pj *= *p;
                    x -= y * (*pj++);
                    p += k + 2;
                }
                const double a = fabs ( x );
                if ( a < 1e-100 )
                {
                    g.resize();
                    return;
                }
                g[l] = a > 1e100 ? 0 : 1 / x;
            }
            else
            {
                const double * pi = si;
                for ( k = j; k-- > 0; ) x -= (*pi++) * (*pj++);
                const double a = fabs ( x );
                if ( a > 1e100 )
                {
                    g.resize();
                    return;
                }
                g[l] = a < 1e-100 ? 0 : x;
            }
            ++l;
        }
    }
}

bool SM_LDLt::solve ( const double * b, double * x ) const
{
    if ( g.size() == 0 ) return false;
// Решение системы Ly = b
    nat p = 0, i, k;
    for ( i = 0; i < n; ++i )
    {
        double t = b[i];
        for ( k = 0; k < i; ++k )
        {
            t -= g[p] * x[k];
            ++p;
        }
        x[i] = t;
        ++p;
    }
// Решение системы DUx = y;
    for ( i = n; i-- > 0; )
    {
        nat s = --p;
        double t = x[i] * g[i*(i+3)/2];
        for ( k = n; --k > i; )
        {
            t -= g[s] * x[k];
            s -= k;
        }
        x[i] = t;
    }
    return true;
}

double SM_LDLt::determinant () const 
{
    if ( g.size() == 0 ) return 0.;
    nat i, p = 0;
    double det = 1.;
    for ( i = 0; i < n; ++i )
    {
        det *= g[p];
        p += i + 2;
    }
    return 1. / det;
}

//*********************** 22.09.2023 **************************//
//
//           Симметричные разреженные матрицы.
//
//*********************** 22.09.2023 **************************//

bool slu_LDLt ( nat n, const nat * m, double * a, const double * b, double * x )
{
    nat i, j, k, l, nn = 0;
// Заполнение индексов диагональных элементов
    DynArray<nat> di ( n );
    for ( i = 0; i < n; ++i )
    {
        if ( m[i] > i )
            return false;
        di[i] = nn += i - m[i];
        ++nn;
    }
// Построение матрицы L
    for ( i = l = 0; i < n; ++i )
    {
        const nat mi = m[i];
        const nat ni = di[i] - i;
        const double * ai = a + ni;
        for ( j = mi; j <= i; ++j )
        {
            double * aj = a + di[j] - j;
            double x = ai[j];
            if ( i == j )
            {
                for ( k = mi; k < j; ++k )
                {
                    double & ajk = aj[k];
                    const double y = ajk;
                    ajk *= a[di[k]];
                    x -= y * ajk;
                }
                const double ax = fabs ( x );
                if ( ax < 1e-100 )
                {
                    return false;
                }
                a[l] = ax > 1e100 ? 0 : 1 / x;
            }
            else
            {
                for ( k = _max ( mi, m[j] ); k < j; ++k ) x -= ai[k] * aj[k];
                const double ax = fabs ( x );
                if ( ax > 1e100 )
                {
                    return false;
                }
                a[l] = ax < 1e-100 ? 0 : x;
            }
            ++l;
        }
    }
// Решение системы Ly = b
    for ( i = j = 0; i < n; ++i, ++j )
    {
        double t = b[i];
        for ( k = m[i]; k < i; ++k ) t -= a[j++] * x[k];
        x[i] = t;
    }
// Решение системы DUx = y;
    for ( i = 0; i < n; ++i ) x[i] *= a[di[i]];
    for ( l = nn; --i > 0; )
    {
        const double t = x[i];
        const nat mi = m[i];
        const double * p = a + ( l -= i + 1 );
        l += mi;
        for ( k = mi; k < i; ++k ) x[k] -= p[k] * t;
    }
    return true;
}

bool slu_LDLt ( nat n, const Suite<SortItem<nat, double> > * data, const double * b, double * x )
{
    nat i;
    DynArray<nat> m ( n );
    nat nbuf = n * ( n + 1 ) / 2;
    for ( i = 0; i < n; ++i ) nbuf -= m[i] = data[i][0].head;
    DynArray<double> buf ( nbuf );
    for ( nat l = i = 0; i < n; ++i )
    {
        CCArrRef<SortItem<nat, double> > & arr = data[i];
        for ( nat j = m[i], k = 0; j <= i; ++j )
        {
            buf[l++] = j == arr[k].head ? arr[k++].tail: 0;
        }
    }
    return slu_LDLt ( n, m(), buf(), b, x );
}

//*********************** 04.10.2023 **************************//
//
//           Симметричные разреженные матрицы.
// Перестановка столбцов и строк для увеличения лидирующих нулей.
//
//*********************** 11.11.2023 **************************//

inline void _swap ( SortItem<nat, Set2<nat> *> & p1, SortItem<nat, Set2<nat> *> & p2 )
{
    const SortItem<nat, Set2<nat> *> p ( p1 );
    p1 = p2;
    p2 = p;
    _swap ( p1.tail->b, p2.tail->b );
}

bool slu_LDLtO ( nat n, const Suite<SortItem<nat, double> > * data, const double * b, double * x )
{
    nat i, j, k, l;
    DynArray<nat> index ( 4*n );
    nat * index2 = index ( n );
    nat * fi = index2 + n;
    nat * m = fi + n;
    // Переставим столбцы и строки для увеличения лидирующих нулей
    {
        MinHeap< SortItem<nat, Set2<nat> *> > heap ( n );
        DynArray<Set2<nat> > elem ( n );
        for ( i = 0; i < n; ++i )
        {
            Set2<nat> & e = elem[i];
            e.a = i;
            e.b = heap.size();
            heap << SortItem<nat, Set2<nat> *> ( data[i].size(), elem(i) );
        }
        DynArray<bool> flag ( n, 0 );
        SortItem<nat, Set2<nat> *> t;
        for ( l = 0; l < n; ++l )
        {
            heap >> t;
            t.tail->b = n;
            index[l] = t.tail->a;
            CCArrRef<SortItem<nat, double> > & a = data[t.tail->a];
            for ( i = 0; i < a.size(); ++i ) flag[a[i].head] = true;
            for ( i = 0; i < a.size(); ++i )
            {
                const Set2<nat> & e = elem[a[i].head];
                if ( e.b == n ) continue;
                CCArrRef<SortItem<nat, double> > & b = data[e.a];
                nat sum = 0;
                for ( j = 0; j < b.size(); ++j ) if ( ! flag[b[j].head] ) ++sum;
                heap[e.b]->head = sum;
                heap.raise ( e.b );
            }
        }
    }
    for ( i = 0; i < n; ++i ) index2[index[i]] = i;
    for ( i = 0; i < n; ++i )
    {
        CCArrRef<SortItem<nat, double> > & arr = data[i];
        nat min = index2[arr[0].head];
        for ( nat j = 1; j < arr.size(); ++j ) _mina ( min, index2[arr[j].head] );
        fi[i] = min;
    }
    nat nbuf = n * ( n + 1 ) / 2;
    nat max = 0;
    for ( i = 0; i < n; ++i )
    {
        nbuf -= m[i] = fi[index[i]];
        _maxa ( max, data[i].size() );
    }
    DynArray<double> y ( n + n + nbuf );
    double * b2 = y ( n );
    for ( i = 0; i < n; ++i ) b2[index2[i]] = b[i];
    double * buf = b2 + n;
    Suite<SortItem<nat, double> > arr2 ( max );
    for ( l = i = 0; i < n; ++i )
    {
        CCArrRef<SortItem<nat, double> > & arr = data[index[i]];
        arr2.resize ( arr.size() );
        for ( j = 0; j < arr.size(); ++j )
        {
            arr2[j].head = index2[arr[j].head];
            arr2[j].tail = arr[j].tail;
        }
        insertSort123 ( arr2 );
        for ( j = m[i], k = 0; j <= i; ++j )
        {
            buf[l++] = j == arr2[k].head ? arr2[k++].tail: 0;
        }
    }
    if ( ! slu_LDLt ( n, m, buf, b2, y() ) ) return false;
    for ( i = 0; i < n; ++i ) x[i] = y[index2[i]];
    return true;
}

//*********************** 01.06.2023 **************************//
//
//      Симметричные положительно определённые матрицы.
//              Метод сопряжённых градиентов.
//
//*********************** 16.06.2023 **************************//

static double inprod ( nat n, const double * x, const double * y )
{
    double s = 0;
    for ( nat i = 0; i < n; ++i ) s += x[i] * y[i];
    return s;
}

static void op ( nat n, const Suite<SortItem<nat, double> > * data, const double * x, double * r )
{
    for ( nat i = 0; i < n; ++i )
    {
        CCArrRef<SortItem<nat, double> > & ri = data[i];
        const nat ni = ri.size();
        double & s = r[i];
        s = 0;
        for ( nat j = 0; j < ni; ++j )
        {
             const SortItem<nat, double> & rij = ri[j];
             s += rij.tail * x[rij.head];
        }
    }
}

static double dif ( nat n, const Suite<SortItem<nat, double> > * data, const double * x, const double * b, const double * r )
{
    double sum = 0;
    for ( nat i = 0; i < n; ++i )
    {
        CCArrRef<SortItem<nat, double> > & ri = data[i];
        const nat ni = ri.size();
        double s = 0;
        for ( nat j = 0; j < ni; ++j )
        {
             const SortItem<nat, double> & rij = ri[j];
             s += rij.tail * x[rij.head];
        }
        s -= b[i];
        s -= r[i];
        sum += s * s;
    }
    return sum;
}

bool slu_cg ( nat n, const Suite<SortItem<nat, double> > * data, const double * b, double * x, bool isX )
{
    const nat sqn = (nat) sqrt ( n );
    nat i, ksqn = sqn;
    double factor = 1, rfe = 0, f = 1 + 2. / n;
    const double f2 = f * f;
    DynArray<double> buf ( 4*n );
    double * g = buf();
    double * y = g + n;
    double * d = y + n;
    double * ad = d + n;
    if ( isX )
    {
        op ( n, data, x, ad );
        for ( i = 0; i < n; ++i )
        {
            y[i] = x[i];
            g[i] = - ( d[i] = b[i] - ad[i] );
        }
    }
    else
    {
        for ( i = 0; i < n; ++i )
        {
            x[i] = y[i] = 0;
            g[i] = - ( d[i] = b[i] );
        }
    }
    double gg = inprod ( n, g, g );
    double mingg = gg;
    const nat nn = 3 * n;
    nat km = 0;
    for ( nat k = 0; k < nn; ++k )
    {
        if ( k > n && km + km < k )
            return true;
        if ( k == ksqn )
        {
            ksqn += sqn;
            rfe = dif ( n, data, y, b, g );
        }
        if ( gg <= factor * rfe )
            return true;
        op ( n, data, d, ad );
        const double dad = inprod ( n, d, ad );
        if ( ! dad )
            return false;
        const double alfa = gg / dad;
        for ( i = 0; i < n; ++i )
        {
            y[i] += alfa * d[i];
            g[i] += alfa * ad[i];
        }
        const double gg1 = inprod ( n, g, g );
        const double beta = gg1 / gg;
        gg = gg1;
        if ( mingg > gg )
        {
            mingg = gg;
            for ( i = 0; i < n; ++i ) x[i] = y[i];
            km = k;
        }
        for ( i = 0; i < n; ++i ) d[i] = beta * d[i] - g[i];
        factor *= f;
    }
    return true;
}

//*********************** 08.04.2010 **************************//
//
//      Переопределённые системы линейных уравнений ( n > m ).
//      Минимум 1-нормы вектора невязок.
//
//*********************** 24.12.2021 **************************//

bool minNorm1_Dual ( ArrRef2<double> & data, ArrRef<nat> & res )
{
    nat i, j, k;
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    const nat nRow1 = nRow - 1;
    DynArray<nat> index ( nCol );
    DynArray<double> w ( nCol );
    ArrRef<double> func = data[nRow1];
    double max1 = 0;
    for ( j = 0; j < nCol; ++j )
    {
        const double t = fabs ( func[j] );
        if ( max1 < t ) max1 = t;
    }
    if ( ! _sluGaussRow ( data, nRow, nCol, index(), nRow1, nCol ) ) return false;
    double max2 = 0;
    for ( j = nRow1; j < nCol; ++j )
    {
        const double t = fabs ( func[index[j]] );
        if ( max2 < t ) max2 = t;
    }
    if ( max2 <= 1e-12 * max1 )
    {
        for ( i = 0; i < nRow1; ++i ) res[i] = index[i];
        return true;
    }
    for ( k = 0; k < 2*nCol; ++k )
    {
        for ( j = nRow1; j < nCol; ++j )
        {
            const nat ij = index[j];
            w[ij] = func[ij] < 0 ? -1 : 1;
        }
        nat im = 0;
        double max = 0, sum = 0;
        for ( i = 0; i < nRow1; ++i )
        {
            const double * p = data[i]();
            double t = 0;
            for ( j = nRow1; j < nCol; ++j )
            {
                const nat ij = index[j];
                t -= p[ij] * w[ij];
            }
            const double a = fabs ( t );
            if ( max < a ) max = a, sum = t, im = i;
        }
        if ( max < 1 + 1e-9 )
        {
            for ( i = 0; i < nRow1; ++i ) res[i] = index[i];
            return true;
        }
        const double ds = sum < 0 ? -1 : 1;
        nat jm = nCol;
        max = -1e300;
        double * ri = data[im]();
        for ( j = nRow1; j < nCol; ++j )
        {
            const nat ij = index[j];
            const double t = ri[ij];
            if ( !t )
                continue;
            const double a = func[ij] * ds / t;
            if ( a > 0 )
                continue;
            if ( max < a ) max = a, jm = j;
        }
        if ( jm == nCol )
            return false;
        const nat ik = index[jm];
        _swap ( index[jm], index[im] );
// Нормализация строки
        const double p = 1. / ri[ik];
        for ( j = im+1; j < nCol; ++j ) ri[index[j]] *= p;
        ri[ik] = 1;
// Вычитание строк
        for ( j = 0; j < nRow; ++j )
        {
            if ( j == im ) continue;
            double * rj = data[j]();
            const double t = rj[ik];
            for ( i = im+1; i < nCol; ++i )
            {
                const nat ii = index[i];
                if ( fabs ( rj[ii] -= ri[ii] * t ) < 1e-290 ) rj[ii] = 0;
            }
            rj[ik] = 0;
        }
    }
    return false;
}

bool minNorm1 ( CArrRef2<double> & data, ArrRef<double> & x, ArrRef<nat> & index )
{
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    if ( nRow < nCol || nCol < 2 ) return false;
    const nat nCol1 = nCol - 1;
    if ( x.size() > 0 && x.size() < nCol1 ) return false;
    DynArray<double> temp ( nRow * nCol );
    ArrRef2<double> ref ( temp, 0, nCol, nRow );
    for ( nat j = 0; j < nCol; ++j )
    {
        double * p = ref[j]();
        for ( nat i = 0; i < nRow; ++i )
        {
            p[i] = data[i][j];
        }
    }
// Решаем двойственную задачу
    if ( ! minNorm1_Dual ( ref, index ) ) return false;
    if ( x.size() == 0 ) return true;
// Решаем систему линейных уравнений методом Гаусса
    ArrRef2<double> ref2 ( temp, 0, nCol1, nCol );
    for ( nat i = 0; i < nCol1; ++i ) ref2[i] = data[index[i]];
    return slu_gauss ( ref2, x );
}

bool minNorm1 ( CArrRef2<double> & data, ArrRef<double> & x )
{
    const nat nCol = data.size1();
    if ( nCol < 2 ) return false;
    return minNorm1 ( data, x, DynArray<nat>(nCol-1) );
}

bool minNorm1 ( CArrRef2<double> & data, ArrRef<nat> & index )
{
    return minNorm1 ( data, ArrRef<double>(), index );
}

//*********************** 24.07.2010 **************************//
//
//      Переопределённые системы линейных уравнений ( n > m ).
//      Минимум 2-нормы вектора невязок.
//
//*********************** 12.11.2011 **************************//

bool minNorm2 ( CArrRef2<double> & data, ArrRef<double> & x )
{
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    if ( nRow + 1 < nCol || nRow == 0 ) return false;
    const nat nCol1 = nCol - 1;
    if ( x.size() < nCol1 ) return false;
    nat i, j, k;
    DynArray2<double> work ( nCol1, nCol );
    for ( i = 0; i < nCol1; ++i )
    {
        for ( j = i; j < nCol; ++j )
        {
            double d = 0.;
            for ( k = 0; k < nRow; ++k )
            {
                CArrRef<double> rk = data[k];
                d += rk[i] * rk[j];
            }
            work[i][j] = d;
            if ( j > i && j < nCol1 ) work[j][i] = d;
        }
    }
    return slu_gauss ( work, x );
}

//*********************** 17.07.2010 **************************//
//
//      Переопределённые системы линейных уравнений ( n > m ).
//      Минимум бесконечной нормы вектора невязок.
//
//*********************** 12.11.2011 **************************//

bool minNormU ( CArrRef2<double> & data, ArrRef<double> & x, ArrRef<nat> & index )
{
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    if ( nRow < nCol ) return false;
    if ( index.size() > 0 && index.size() < nCol ) return false;
    const nat nCol1 = nCol - 1;
    if ( x.size() < nCol1 ) return false;
    nat i, j, k;

/******** Действия в nCol1-мерном пространстве ********/

// Выбираем в массив plane nCol плоскостей из множества данных
    DynArray2<double> plane ( nCol, nCol );
    for ( i = 0; i < nCol; ++i )
    {
        plane[i] = data[i];
        if ( index.size() > 0 ) index[i] = i;
    }
// Находим точки пересечения nCol1 плоскостей из множества nCol
    DynArray2<double> point ( nCol, nCol1 );
    DynArray2<double> work ( nCol1, nCol );
    for ( i = 0; i < nCol; ++i )
    {
        for ( j = k = 0; j < nCol1; )
        {
            if ( j == i ) ++k;
            work[j++] = plane[k++];
        }
        if ( ! slu_gauss ( work, point[i] ) ) return false;
    }
// Переворачиваем плоскости при необходимости
    for ( i = 0; i < nCol; ++i )
    {
        ArrRef<double> r1 = plane[i];
        CArrRef<double> r2 = point[i];
        double sum = 0;
        for ( j = 0; j < nCol1; ++j ) sum += r1[j] * r2[j];
        if ( sum <= r1[nCol1] ) continue;
        for ( j = 0; j < nCol; ++j ) r1[j] = - r1[j];
    }
// Находим максимальное абсолютное значение элементов
    double a = 0;
    for ( i = 0; i < nRow; ++i )
    {
        CArrRef<double> r = data[i];
        for ( j = 0; j < nCol1; ++j )
        {
            const double d = fabs ( r[j] );
            if ( a < d ) a = d;
        }
    }

/******** Действия в nCol-мерном пространстве ********/

// Находим точку пересечения nCol плоскостей 
    DynArray2<double> bevel ( nCol+1, nCol ), work2 ( nCol, nCol+1 );
    for ( i = 0; i < nCol; ++i )
    {
        CArrRef<double> r1 = plane[i];
        ArrRef<double> r2 = work2[i];
        for ( j = 0; j < nCol1; ++j ) r2[j] = r1[j];
        r2[nCol1] = a;
        r2[nCol] = r1[nCol1];
    }
    if ( ! slu_gauss ( work2, bevel[0] ) ) return false;
    if ( bevel[0][nCol1] < 0 ) return false;
// Находим перпендикуляры к nCol1 нормалям из множества nCol и записываем их в bevel
    HMatrix<double> mat ( nCol, nCol );
    for ( i = 0;; ++i )
    {
        nat k, m = 0;
        for ( k = 0; k < nCol; ++k )
        {
            if ( k == i ) continue;
            CArrRef<double> r = plane[k];
            double * p = mat[m++];
            for ( j = 0; j < nCol1; ++j ) p[j] = r[j];
            p[nCol1] = a;
        }
        if ( orthogonalizationH1 ( nCol1, mat ) < nCol1 ) return false;
        double * p = mat[nCol1];
        ArrRef<double> r = bevel[i+1];
        if ( p[nCol1] < 0 )
            for ( j = 0; j < nCol; ++j ) r[j] = - p[j];
        else
            for ( j = 0; j < nCol; ++j ) r[j] = p[j];
        if ( i == nCol1 ) break;
        p = mat[i];
        CArrRef<double> cr = plane[i];
        for ( j = 0; j < nCol1; ++j ) p[j] = cr[j];
        p[nCol1] = a;
    }
// Поиск главных плоскостей
    DynArray<double> cor ( nCol );
    for ( k = 0; k < nRow + nCol; ++k )
    {
        ArrRef<double> o = bevel[0];
        double max = 0.;
        nat im = 0;
        for ( i = 0; i < nRow; ++i )
        {
            CArrRef<double> r = data[i];
            double sum = 0.;
            for ( j = 0; j < nCol1; ++j ) sum += o[j] * r[j];
            sum -= r[nCol1];
            sum = fabs ( sum );
            if ( max < sum ) max = sum, im = i;
        }
        const double dist = max - a * o[nCol1];
        if ( dist <= 1e-12 * _max ( 1., max ) )
        {
            for ( j = 0; j < nCol1; ++j ) x[j] = o[j];
            return true;
        }
        CArrRef<double> rm = data[im];
        double sum = 0.;
        for ( j = 0; j < nCol1; ++j ) sum += o[j] * rm[j];
        sum -= rm[nCol1];
        if ( sum > 0 )
        {
            for ( j = 0; j < nCol1; ++j ) cor[j] = - rm[j];
        }
        else
        {
            for ( j = 0; j < nCol1; ++j ) cor[j] = rm[j];
        }
        cor[nCol1] = a;
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i <= nCol; ++i )
        {
            CArrRef<double> v = bevel[i];
            double t = 0;
            for ( j = 0; j < nCol; ++j ) t += cor[j] * v[j];
            if ( t < 1e-12 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v[nCol1] * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double u = v[nCol1] * t;
                if ( u < max ) max = u, ib = i, sm = t;
            }
        }
        if ( ib == 0 ) 
            return false;
        if ( index.size() > 0 ) index[ib-1] = im;
        CArrRef<double> v = bevel[ib];
        const double t = dist * sm;
        for ( i = 0; i < nCol; ++i ) o[i] += v[i] * t;
        for ( i = 1; i <= nCol; ++i )
        {
            if ( i == ib ) continue;
            ArrRef<double> ai = bevel[i];
            double sum = 0.;
            for ( j = 0; j < nCol; ++j ) sum += cor[j] * ai[j];
            sum *= sm;
            for ( j = 0; j < nCol; ++j ) ai[j] -= v[j] * sum;
            sum = 0.;
            for ( j = 0; j < nCol; ++j ) sum += ai[j] * ai[j];
            sum = 1 / sqrt ( sum );
            for ( j = 0; j < nCol; ++j ) ai[j] *= sum;
        }
    }
    return false;
}

bool minNormU ( CArrRef2<double> & data, ArrRef<double> & x )
{
    return minNormU ( data, x, ArrRef<nat>() );
}

//****************************** 07.03.2003 *********************************//
//
//      Метод наименьших квадратов ( n >= m ). Преобразования Хаусхолдера.
//
//****************************** 21.07.2012 *********************************//

LSS_H::LSS_H ( IMatrix<double> & p ) : a(p)
{
    if ( a.nRow < a.nCol || a.nCol <= 0 ) return;
    alpha.resize ( a.nCol + a.nCol + a.nRow );
    pivot.resize ( a.nCol );
    nat i, j, k;
    for ( k = 0; k < a.nCol; ++k ) pivot[k] = k;
    for ( k = 0; k < a.nCol; ++k )
    {
        double sigma = 0.;
        nat jmax;
        for ( j = k; j < a.nCol; ++j )
        {
            double s = 0.;
            for ( i = k; i < a.nRow; ++i ) s += a[i][j] * a[i][j];
            if ( sigma < s ) sigma = s, jmax = j;
        }
        if ( sigma == 0 )
        {
            alpha.resize(0);
            return;
        }
        if ( jmax != k ) // перестановка столбцов
        {
            _swap ( pivot[k], pivot[jmax] );
            for ( i = 0; i < a.nRow; ++i ) _swap ( a[i][k], a[i][jmax] );
        }
        double & akk = a[k][k];
        const double alphak = alpha[k] = akk < 0 ? sqrt ( sigma ) : - sqrt ( sigma );
        const double beta = 1. / ( sigma - akk * alphak );
        akk -= alphak;
        for ( j = k + 1; j < a.nCol; ++j )
        {
            double gamma = 0.;
            for ( i = k; i < a.nRow; ++i ) gamma += a[i][j] * a[i][k];
            gamma *= beta;
            for ( i = k; i < a.nRow; ++i ) a[i][j] -= a[i][k] * gamma;
        }
    }
}

bool LSS_H::solve ( CArrRef<double> b, ArrRef<double> x ) const
{
    if ( alpha.size() == 0 || b.size() < a.nRow || x.size() < a.nCol ) return false;
    ArrRef<double> z ( alpha, a.nCol, a.nRow );
    nat i, j;
    for ( i = 0; i < a.nRow; ++i ) z[i] = b[i];
    for ( j = 0; j < a.nCol; ++j )
    {
        double gamma = 0.;
        for ( i = j; i < a.nRow; ++i ) gamma += a[i][j] * z[i];
        gamma /= alpha[j] * a[j][j];
        for ( i = j; i < a.nRow; ++i ) z[i] += a[i][j] * gamma;
    }
    i = a.nCol - 1;
    ArrRef<double> y ( alpha, a.nCol + a.nRow, a.nCol );
    y[i] = z[i] / alpha[i];
    while ( i > 0 )
    {
        --i;
        y[i] = z[i];
        for ( j = i + 1; j < a.nCol; ++j ) y[i] -= y[j] * a[i][j];
        y[i] /= alpha[i];
    }
    for ( i = 0; i < a.nCol; ++i ) x[pivot[i]] = y[i];
    return true;
}

bool lss_h ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x )
{
    LSS_H lss ( a );
    return lss.solve ( b, x );
}

//****************************** 07.03.2003 *********************************//
//
//      Метод наименьших квадратов ( n >= m ). Oртогонализация Грама-Шмидта.
//
//****************************** 10.09.2015 *********************************//

bool ortholin ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x, DynArrRef<double> & p )
{
    const nat n = a.nRow;
    const nat m = a.nCol;
    if ( m == 0 || m > n || b.size() < n || x.size() < m ) return false;
    double * const * u = a;
    p.resize ( n + m*(m+1)/2 );
    double * q = p(n);
    nat i, j, l = 0;
    for ( i = 0; i < m; ++i )
    {
        double s = 0.;
        for ( j = 0; j < n; ++j ) s += _pow2 ( p[j] = u[j][i] );
        if ( ! s ) return false;
        q[l++] = s;
        double t = 0.;
        for ( j = 0; j < n; ++j ) t += p[j] * b[j];
        x[i] = t;
        for ( nat h = i + 1; h < m; ++h )
        {
            t = 0.;
            for ( j = 0; j < n; ++j ) t += p[j] * u[j][h];
            q[l++] = t;
            t /= s;
            for ( j = 0; j < n; ++j ) u[j][h] -= p[j] * t;
        }
    }
// Обратная подстановка
    --l;
    const nat m1 = m + 1;
    for ( i = m; i-- > 0; )
    {
        nat h = l - i;
        double s = x[i];
        for ( j = i + 1; j < m; ++j ) s -= q[j+h] * x[j];
        x[i] = s / q[l];
        l -= m1 - i;
    }
    return true;
}

bool ortholin ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x )
{
    CmbArray<double, 80> p;
    return ortholin ( a, b, x, p );
}

//****************************** 19.06.2007 *********************************//
//
//          Масштабирование столбцов для решения систем уравнений
//
//****************************** 21.10.2012 *********************************//

bool columnScale ( bool (*func) ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x ),
                   const IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x )
{
    const nat m = a.nCol;
    const nat n = a.nRow;
    if ( n == 0 || m == 0 ) return false;
// Делаем копию исходной матрицы
    HMatrix<double> c ( a );
// Масштабируем столбцы матрицы
    DynArray<double> sb ( n + m + 1 );
    ArrRef<double> scale ( sb, n, m );
    nat i, j;
    for ( j = 0; j < m; ++j )
    {
        double & s = scale[j];
        s = 1.;
        double max = 0.;
        for ( i = 0; i < n; ++i )
        {
            const double t = fabs ( c[i][j] );
            if ( max < t ) max = t;
        }
        if ( ! max ) continue;
        if ( max < 1. )
        {
            do s *= 2.; while ( max * s < 1. );
            for ( i = 0; i < n; ++i ) c[i][j] *= s;
        }
        else
        if ( max > 2. )
        {
            do s /= 2.; while ( max * s > 2. );
            for ( i = 0; i < n; ++i ) c[i][j] *= s;
        }
    }
// Масштабируем столбец правой части системы
    double & s = scale[m];
    s = 1.;
    double max = 0.;
    for ( i = 0; i < n; ++i )
    {
        const double t = fabs ( sb[i] = b[i] );
        if ( max < t ) max = t;
    }
    if ( max != 0 )
    {
        if ( max < 1. )
        {
            do s *= 2.; while ( max * s < 1. );
            for ( i = 0; i < n; ++i ) sb[i] *= s;
        }
        else
        if ( max > 2. )
        {
            do s /= 2.; while ( max * s > 2. );
            for ( i = 0; i < n; ++i ) sb[i] *= s;
        }
    }
// Если решение найдено масштабируем вектор x
    const bool ok = func ( c, sb, x );
    if ( ok )
    {
        const double t = 1. / s;
        for ( i = 0; i < m; ++i ) x[i] *= scale[i] * t;
    }
    return ok;
}

//****************************** 23.05.2007 *********************************//
//
//          Oртогонализация заданных векторов.
//          nr - к-во заполненных строк матрицы.
//          Возвращаемое значение - ранг системы.
//
//****************************** 23.05.2007 *********************************//

nat orthogonalizationH1 ( nat nr, IMatrix<double> & mat )
{
    if ( nr == 0 || mat.nRow == 0 ) return 0;
    if ( nr > mat.nRow ) nr = mat.nRow;
    HMatrix<double> temp ( nr, mat.nCol+1 ); // матрица для хранения параметров отображений
    CmbArray<bool, 80> mark ( nr, false ); // массив для отметки выбранных строк
    nat i, j, k;
    for ( k = 0; k < nr; ++k )
    {
// Поиск строки с максимальной нормой
        double max = 0.;
        nat im = nr;
        for ( i = 0; i < nr; ++i )
        {
            if ( mark[i] ) continue;
            double t = 0.;
            const double * row = mat[i];
            for ( j = k; j < mat.nCol; ++j ) t += row[j] * row[j];
            if ( max < t ) max = t, im = i;
        }
        if ( im == nr )
        {
            break;
        }
        mark[im] = true;
// Отображение максимальной строки на k-ую ось
        const double * rim = mat[im];
        double * rtk = temp[k];
        for ( j = k; j < mat.nCol; ++j ) rtk[j] = rim[j];
        const double s = rim[k] < 0 ? - sqrt ( max ) : sqrt ( max );
        rtk[k] += s;
        rtk[mat.nCol] = max + s * rim[k]; // половина квадрата нормы
        for ( i = 0; i < nr; ++i )
        {
            if ( mark[i] ) continue;
            double t = 0.;
            double * row = mat[i];
            for ( j = k; j < mat.nCol; ++j ) t += row[j] * rtk[j];
            t /= rtk[mat.nCol];
            for ( j = k; j < mat.nCol; ++j ) row[j] -= t * rtk[j];
        }
    }
// Заполнение матрицы mat ортогональными векторами
    mat.fill ( 0. );
    nat l;
    const nat m = mat.nRow < mat.nCol ? mat.nRow : mat.nCol;
    for ( l = 0; l < m; ++l ) mat[l][l] = 1.;
// Обратный ход
    for ( l = k; --l < k; )
    {
        const double * rtl = temp[l];
        for ( i = 0; i < m; ++i )
        {
            double t = 0.;
            double * row = mat[i];
            for ( j = l; j < mat.nCol; ++j ) t += row[j] * rtl[j];
            t /= rtl[mat.nCol];
            for ( j = l; j < mat.nCol; ++j ) row[j] -= t * rtl[j];
        }
    }
    return k;
}

//***************************************************************************//
// 
//  Метод Якоби нахождения собственных значений и собственных векторов
//                       для симметричных матриц
//    Входные данные:
//        n - размерность матрицы
//        a - исходная матрица. В процессе работы наддиагональные элементы
//            будут изменены, но их легко восстановить по поддиагональным
//    Выходные данные:
//        d - массив собственных значений
//        v - массив собственных векторов
//
//***************************************************************************//

void jacobi ( const nat n, double * const * a, double * d, double * const * v )
{
    if ( n == 0 ) return;
    CmbArray<double, 80> b ( n + n );
    ArrRef<double> z ( b, n, n );
    nat i, j;
    for ( i = 0; i < n; ++i )
    {
        z[i] = 0.;
        b[i] = d[i] = a[i][i];
        for ( j = 0; j < n; ++j ) v[i][j] = i == j ? 1. : 0.;
    }
    for ( i = 0; i < 50; ++i )
    {
        double sm = 0.;
        nat p, q;
        for ( p = 0; p < n - 1; ++p )
        {
            for ( q = p + 1; q < n; ++q ) sm += fabs ( a[p][q] );
        }
        if ( sm == 0 ) break;
        const double tresh = i < 3 ? 0.2 * sm / ( n*n ) : 0.;
        for ( p = 0; p < n - 1; ++p )
        {
            for ( q = p + 1; q < n; ++q )
            {
                const double g = 1e12 * fabs ( a[p][q] );
                if ( i >= 3 && fabs ( d[p] ) > g && fabs ( d[q] ) > g ) a[p][q] = 0.;
                else
                if ( fabs ( a[p][q] ) > tresh )
                {
                    const double theta = 0.5 * ( d[q] - d[p] ) / a[p][q];
                    double t = 1. / ( fabs(theta) + sqrt(1.+theta*theta) );
                    if ( theta < 0 ) t = - t;
                    const double c = 1. / sqrt ( 1. + t*t );
                    const double s = t * c;
                    const double tau = s / ( 1. + c );
                    const double h = t * a[p][q];
                    z[p] -= h;
                    z[q] += h;
                    d[p] -= h;
                    d[q] += h;
                    a[p][q] = 0.;
                    for ( j = 0; j < p; ++j )
                    {
                        const double g = a[j][p];
                        const double h = a[j][q];
                        a[j][p] = g - s * ( h + g * tau );
                        a[j][q] = h + s * ( g - h * tau );
                    }
                    for ( j = p+1; j < q; ++j )
                    {
                        const double g = a[p][j];
                        const double h = a[j][q];
                        a[p][j] = g - s * ( h + g * tau );
                        a[j][q] = h + s * ( g - h * tau );
                    }
                    for ( j = q+1; j < n; ++j )
                    {
                        const double g = a[p][j];
                        const double h = a[q][j];
                        a[p][j] = g - s * ( h + g * tau );
                        a[q][j] = h + s * ( g - h * tau );
                    }
                    for ( j = 0; j < n; ++j )
                    {
                        const double g = v[j][p];
                        const double h = v[j][q];
                        v[j][p] = g - s * ( h + g * tau );
                        v[j][q] = h + s * ( g - h * tau );
                    }
                }
            }
        }
        for ( p = 0; p < n; ++p )
        {
            d[p] = ( b[p] += z[p] );
            z[p] = 0.;
        }
    }
}

//************************* 20.02.2007 **************************************//
//
//        Приведение действительной матрицы к верхней форме Хессенберга
//
//************************* 20.02.2007 **************************************//

void orthes ( nat n, double * const * a )
{
    if ( n < 3 ) return;
    CmbArray<double, 80> d ( n );
    nat m, n2 = n - 2;
    for ( m = 0; m < n2; ++m )
    {
        double h = 0.;
        nat i, m1 = m + 1;
        for ( i = m1; i < n; ++i )
        {
            const double f = d[i] = a[i][m];
            h += f * f;
        }
        if ( h < 1e-100 )
        {
           a[m1][m] = 0.;
        }
        else
        {
            const double f = d[m1];
            const double g = f >= 0 ? -sqrt ( h ) : sqrt ( h );
            h -= f * g;
            d[m1] -= g;
            nat j;
            for ( j = m1; j < n; ++j )
            {
                double f = 0;
                for ( i = m1; i < n; ++i ) f += d[i] * a[i][j];
                f /= h;
                for ( i = m1; i < n; ++i ) a[i][j] -= f * d[i];
            }
            for ( i = 0; i < n; ++i )
            {
                double f = 0;
                for ( j = m1; j < n; ++j ) f += d[j] * a[i][j];
                f /= h;
                for ( j = m1; j < n; ++j ) a[i][j] -= f * d[j];
            }
            a[m1][m] = g;
        }
        for ( i = m + 2; i < n; ++i ) a[i][m] = 0.;
    }
}

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

} // namespace

//************************* 05.08.2006 **************************************//
//
//        Вычисление собственных значений действительной матрицы
//        заданной в верхней форме Хессенберга
//
//************************* 05.08.2006 **************************************//

bool hqr ( nat n, double * const * h, double * wr, double * wi )
{

    double t = 0.;
    while ( n > 0 )
    {
        nat its = 0;
        const nat n1 = n - 1;
nextit:
        nat l = n1;
        for ( ; l > 0; --l )
        {
            if ( fabs ( h[l][l-1] ) <= macheps * ( fabs ( h[l-1][l-1] ) + fabs ( h[l][l] ) ) )
            {
                break;
            }
        }
        double x = h[n1][n1];
        if ( l == n1 )
        {
// Одно собственное значение
            wr[n1] = x + t;
            wi[n1] = 0.;
            n = n1;
            continue;
        }
        const nat na = n1 - 1;
        double y = h[na][na];
        double w = h[n1][na] * h[na][n1];
        if ( l == na )
        {
// Пара собственных значений
            const double p = ( y - x ) * 0.5;
            const double q = p * p + w;
            y = sqrt ( fabs ( q ) );
            x += t;
            if ( q > 0 )
            {
// Пара действительных собственных значений
                if ( p < 0 ) y = - y;
                y += p;
                wr[na] = x + y;
                wr[n1] = x - w / y;
                wi[n1] = wi[na] = 0.;
            }
            else
            {
// Пара комплексно-сопряжённных собственных значений
                wr[na] = wr[n1] = x + p;
                wi[n1] = - y;
                wi[na] = y;
            }
            n -= 2;
            continue;
        }
        if ( its == 30 ) return false;
        if ( its == 10 || its == 20 )
        {
// Формирование дополнительного смещения
            t += x;
            for ( nat i = 0; i < n; ++i ) h[i][i] -= x;
            const double s = fabs ( h[n1][na] ) + fabs ( h[na][n1-2] );
            x = y = 0.75 * s;
            w = -0.4375 * s * s;
        }
        ++its;
// Поиск двух соседних малых по величине поддиагональных элементов
        double p, q, r;
        nat m;
        for ( m = n - 3; m >= l; --m )
        {
            const double z = h[m][m];
            double s = y - z;
            r = x - z;
            p = ( r * s - w ) / h[m+1][m] + h[m][m+1];
            q = h[m+1][m+1] - z - r - s;
            r = h[m+2][m+1];
            s = fabs(p) + fabs(q) + fabs(r);
            p /= s;
            q /= s;
            r /= s;
            if ( m == l ) break;
            if ( fabs(h[m][m-1]*(fabs(q) + fabs(r))) <= macheps * fabs(p) *
                 ( fabs(h[m-1][m-1]) + fabs(z) + fabs(h[m+1][m+1]) ) ) break;
        }
        nat i;
        for ( i = m + 2; i < n; ++i ) h[i][i-2] = 0.;
        for ( i = m + 3; i < n; ++i ) h[i][i-3] = 0.;
// Двойной шаг QR-алгоритма, выполняемый над строками с номерами от l до n и столбцами от m до n
        for ( nat k = m; k <= na; ++k )
        {
            const bool notlast = k != na;
            if ( k != m )
            {
                p = h[k][k-1];
                q = h[k+1][k-1];
                r = notlast ? h[k+2][k-1] : 0.;
                x = fabs(p) + fabs(q) + fabs(r);
                if ( x == 0 ) continue;
                p /= x;
                q /= x;
                r /= x;
            }
            double s = sqrt ( p * p + q * q + r * r );
            if ( p < 0 ) s = - s;
            if ( k != m ) h[k][k-1] = - s * x;
            else
            {
                if ( l != m ) h[k][k-1] = - h[k][k-1];
            }
            p += s;
            x = p / s;
            y = q / s;
            const double z = r / s;
            q /= p;
            r /= p;
// Изменение строки
            nat j;
            for ( j = k; j < n; ++j )
            {
                p = h[k][j] + q * h[k+1][j];
                if ( notlast )
                {
                    p += r * h[k+2][j];
                    h[k+2][j] -= p * z;
                }
                h[k+1][j] -= p * y;
                h[k][j] -= p * x;
            }
// Изменение столбца
            j = k + 3 < n1 ? k + 3 : n1;
            for ( i = l; i <= j; ++i )
            {
                p = x * h[i][k] + y * h[i][k+1];
                if ( notlast )
                {
                    p += z * h[i][k+2];
                    h[i][k+2] -= p * r;
                }
                h[i][k+1] -= p * q;
                h[i][k] -= p;
            }
        }
        goto nextit;
    }
    return true;
}

//************************* 03.12.2005 **************************************//
//
//        Поиск нуля функции на заданном интервале
//        Значения функции в ax и bx должны иметь разные знаки
//
//************************* 03.12.2005 **************************************//

inline int sign ( double d )
{
    if ( d > 0 ) return 1;
    if ( d < 0 ) return -1;
    return 0;
}

bool zeroin ( double ax, double bx, const MathFunc1 & func, double tol, double & res )
{
    double fa = func(ax);
    double fb = func(bx);
    if ( sign(fa) * sign(fb) > 0 ) return false;
    if ( tol < 0 ) tol = 0.;
// Присвоение начальных значений
    double a = ax;
    double b = bx;
    for(;;)
    {
        double c = a;
        double fc = fa;
        double d = b - a;
        double e = d;
m30:    if ( fabs(fc) < fabs(fb) )
        {
            a = b;
            b = c;
            c = a;
            fa = fb;
            fb = fc;
            fc = fa;
        }
// Проверка сходимости
        if ( fb == 0 ) break;
        const double tol1 = 2. * macheps * fabs(b) + 0.5 * tol;
        const double xm = 0.5 * ( c - b );
        if ( fabs(xm) <= tol1 ) break;
// Необходима ли бисекция?
        if ( fabs(e) < tol1 || fabs(fa) <= fabs(fb) ) goto m70;
        {
            double p, q;
            const double s = fb / fa;
            if ( a == c )
            {
// Линейная интерполяция
                p = 2. * xm * s;
                q = 1. - s;
            }
            else
            {
// Обратная квадратичная интерполяция
                q = fa / fc;
                const double r = fb / fc;
                p = s * ( 2. * xm * q * ( q - r ) - ( b - a ) * ( r - 1. ) );
                q = ( q - 1. ) * ( r - 1. ) * ( s - 1. );
            }
            if ( p > 0 ) q = -q;
            p = fabs ( p );
// Приемлема ли интерполяция
            if ( p + p >= 3.*xm*q - fabs(tol1*q) ) goto m70;
            if ( p >= fabs(0.5*e*q) ) goto m70;
            e = d;
            d = p / q;
            goto m80;
        }
m70:    e = d = xm; // бисекция
// Завершить шаг
m80:    a = b;
        fa = fb;
        if ( fabs(d) > tol1 ) b += d;
        else
        {
            if ( xm > 0 ) b += tol1;
            else          b -= tol1;
        }
        fb = func(b);
        if ( sign(fb) * sign(fc) <= 0 ) goto m30;
    }
    res = b;
    return true;
}
