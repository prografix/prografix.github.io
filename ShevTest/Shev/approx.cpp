
#include "math.h"
#include "approx.h"
#include "mathem.h"
#include "tune.h"

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

//************************* 24.12.2019 ******************************//
//
//              Аппроксимация периодических функций.
//
//************************* 30.12.2019 ******************************//

HarmAppr::HarmAppr ( CCArrRef<Vector2d> & func, nat nr, nat k ) : coef ( nr+nr+1 ), ks ( k )
{
    const nat nv = func.size();
    if ( nv < 2 )
    {
        coef.resize();
        return;
    }
    coef[nr] = 0;
    Vector2d a = func[nv-1];
    a.x -= M_2PI;
    for ( nat j = 0; j < nv; ++j )
    {
        const Vector2d & b = func[j];
        coef[nr] += ( a.y + b.y ) * ( b.x - a.x );
        a = b;
    }
    coef[nr] *= 0.25 / M_PI;
    for ( nat i = 1; i <= nr; ++i )
    {
        const nat ki = ks * i;
        const nat ii = ki * ki;
        double & c = coef[nr-i];
        double & s = coef[nr+i];
        c = s = 0;
        Vector2d a = func[nv-1];
        a.x -= M_2PI;
        const double t = ki * a.x;
        double sa = sin ( t );
        double ca = cos ( t );
        for ( nat j = 0; j < nv; ++j )
        {
            const Vector2d & b = func[j];
            const double dx = b.x - a.x;
            if ( dx > 1e-8 )
            {
                const double p = ( b.y - a.y ) / ( b.x - a.x ) / ii;
                const double t = ki * b.x;
                double sb = sin ( t );
                double cb = cos ( t );
                c += ( b.y * sb - a.y * sa ) / ki;
                c += p * ( cb - ca );
                s += ( b.y * cb - a.y * ca ) / ki;
                s += p * ( sb - sa );
                sa = sb;
                ca = cb;
            }
            a = b;
        }
        c /= M_PI;
        s /= M_PI;
    }
}

double HarmAppr::operator () ( double a ) const
{
    const nat nc = coef.size();
    const nat nr = ( nc - 1 )  / 2;
    double res = 0.;
    for ( nat j = 0; j < nc; ++j )
    {
        if ( j > nr )
        {
            const double t = ks * ( j - nr ) * a;
            res += coef[j] * sin(t);
        }
        else
        {
            const double t = ks * ( nr - j ) * a;
            res += coef[j] * cos(t);
        }
    }
    return res;
}

//************************* 24.12.2019 ******************************//
//
//         Аппроксимация периодических функций с модификацией.
//
//************************* 30.12.2019 ******************************//

HarmApprMod::HarmApprMod ( CCArrRef<Vector2d> & func, nat nr, nat k ) : mat ( func.size(), nr+nr+1 )
{
    ks = k;
    const nat nv = func.size();
    if ( nv < 2 )
        return;
    coef.resize ( nr+nr+1 );
    mat.fill ( 0 );
    nat ai = nv - 1;
    double ax = func[ai].x - M_2PI;
    for ( nat bi = 0; bi < nv; ++bi )
    {
        const double bx = func[bi].x;
        const double dx = 0.25 * ( bx - ax );
        mat[ai][nr] += dx;
        mat[bi][nr] += dx;
        ai = bi;
        ax = bx;
    }
	nat i;
    for ( i = 1; i <= nr; ++i )
    {
        const nat ki = ks * i;
        const nat ii = ki * ki;
        const nat ci = nr - i;
        const nat si = nr + i;
        nat ai = nv - 1;
        double ax = func[ai].x - M_2PI;
        const double t = ki * ax;
        double sa = sin ( t );
        double ca = cos ( t );
        for ( nat bi = 0; bi < nv; ++bi )
        {
            const double bx = func[bi].x;
            const double dx = bx - ax;
            if ( dx > 1e-8 )
            {
                const double t = ki * bx;
                const double sb = sin ( t );
                const double cb = cos ( t );
                const double iidx = ii * dx;
                const double cd = ( cb - ca ) / iidx;
                mat[ai][ci] -= sa / ki + cd;
                mat[bi][ci] += sb / ki + cd;
                const double sd = ( sb - sa ) / iidx;
                mat[ai][si] -= ca / ki + sd;
                mat[bi][si] += cb / ki + sd;
                sa = sb;
                ca = cb;
                ax = bx;
            }
            else
            {
                const double cd = sa / ki - sa / ii;
                mat[ai][ci] -= cd;
                mat[bi][ci] += cd;
                const double sd = ca / ki + ca / ii;
                mat[ai][si] -= sd;
                mat[bi][si] += sd;
            }
            ai = bi;
        }
    }
    for ( i = 0; i < coef.size(); ++i )
    {
        double & c = coef[i];
        c = 0;
        for ( nat j = 0; j < nv; ++j ) c += mat[j][i] * func[j].y;
        c /= M_PI;
    }
}

void HarmApprMod::rebuild ( ArrRef<Vector2d> & func, nat iy, double y )
{
    const nat nv = func.size();
    const nat nc = coef.size();
    if ( nv < 2 || ! nc || iy >= nv )
        return;
    double * p = mat[iy];
    const double dy = ( y - func[iy].y ) / M_PI;
    for ( nat i = 0; i < nc; ++i ) coef[i] += p[i] * dy;
    func[iy].y = y;
}

//************************* 03.01.2020 ******************************//
//
//              Аппроксимация периодических функций.
//                  Метод наименьших квадратов.
//
//************************* 03.01.2020 ******************************//

bool HarmAppr2::rebuild ( CCArrRef<Vector2d> & func, CCArrRef<bool> & w, nat nr, nat k )
{
    rank = nr;
    ks = k;
    const nat n = func.size();
    const nat nc = 2 * nr + 1;
    if ( coef.size() != nc ) coef.resize ( nc );
    HMatrix<double> a ( n, nc );
    DynArray<double> y ( n );
    for ( nat i = 0; i < n; ++i )
    {
        double * p = a[i];
        if ( ! w[i] )
        {
            for ( nat j = 0; j < nc; ++j ) p[j] = 0;
            y[i] = 0;

        }
        else
        {
            for ( nat j = 0; j < nc; ++j )
            {
                if ( j > nr )
                {
                    const double t = ks * ( j - nr ) * func[i].x;
                    p[j] = sin ( t );
                }
                else
                {
                    const double t = ks * ( nr - j ) * func[i].x;
                    p[j] = cos ( t );
                }
            }
            y[i] = func[i].y;
        }
    }
    if ( ortholin ( a, y, coef ) ) return true;
    coef.resize();
    return false;
}

//************************* 05.02.2020 ******************************//
//
//              Аппроксимация периодических функций.
//                      Минимум 1-нормы.
//
//************************* 05.02.2020 ******************************//

HarmAppr1::HarmAppr1 ( CCArrRef<Vector2d> & func, nat nr, nat k )
{
    rank = nr;
    ks = k;
    const nat n = func.size();
    const nat nc = 2 * nr + 1;
    if ( coef.size() != nc ) coef.resize ( nc );
    DynArray2<double> a ( n, nc+1 );
    for ( nat i = 0; i < n; ++i )
    {
        double * p = a[i]();
        for ( nat j = 0; j < nc; ++j )
        {
            if ( j > nr )
            {
                const double t = ks * ( j - nr ) * func[i].x;
                p[j] = sin ( t );
            }
            else
            {
                const double t = ks * ( nr - j ) * func[i].x;
                p[j] = cos ( t );
            }
        }
        p[nc] = func[i].y;
    }
    if ( ! minNorm1 ( a, coef ) )
        coef.resize();
}
