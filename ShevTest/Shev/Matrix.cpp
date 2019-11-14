
// 13.03.2004, 23.03.2004, 05.06.2004, 26.06.2004, 09.02.2005, 20.02.2006, 20.10.2012

#include "math.h"
#include "Mathem.h"
#include "Matrix.h"

// Матричные нормы

double norm1 ( const IMatrix<double> & A ) // 1-норма
{
    if ( A.nCol == 0 ) return 0.;
    double res = 0.;
    for ( nat j = 0; j < A.nCol; ++j )
    {
        double t = 0.;
        for ( nat i = 0; i < A.nRow; ++i ) t += fabs ( A[i][j] );
        if ( res < t ) res = t;
    }
    return res;
}

double norm2 ( const IMatrix<double> & A ) // 2-норма
{
    if ( A.nCol == 0 ) return 0.;
    HMatrix<double> T ( A.nCol, A.nRow );
    trans ( A, T );
    HMatrix<double> S ( A.nCol, A.nCol );
    multi ( T, A, S );
    HMatrix<double> V ( A.nCol, A.nCol );
    double * d = T[0];
    jacobi ( S.nCol, S, d, V );
    double res = 0.;
    for ( int i = S.nCol; --i >= 0; ) if ( res < d[i] ) res = d[i];
    return sqrt(res);
}

double normF ( const IMatrix<double> & A ) // норма Фробениуса
{
    if ( A.nCol == 0 ) return 0.;
    double res = 0.;
    const double * t = A[0];
    for ( int i = A.nRow*A.nCol; --i >= 0; )
    {
        res += *t * *t;
        ++t;
    }
    return sqrt(res);
}

double normU ( const IMatrix<double> & A ) // бесконечная норма
{
    if ( A.nCol == 0 ) return 0.;
    double res = 0.;
    for ( nat i = 0; i < A.nRow; ++i )
    {
        double t = 0.;
        for ( nat j = 0; j < A.nCol; ++j ) t += fabs ( A[i][j] );
        if ( res < t ) res = t;
    }
    return res;
}

// Разные функции

bool determinant ( const IMatrix<double> & m, double & d )
{
    if ( m.nCol != m.nRow || m.nCol == 0 ) return false;
    SLU_Gauss slu ( m.nCol, m );
    d = slu.determinant();
    return true;
}

bool trans ( const IMatrix<double> & a, IMatrix<double> & b ) // транспонирование
{
    if ( a.nCol != b.nRow || a.nRow != b.nCol || a.nCol == 0 ) return false;
    if ( a[0] == b[0] ) // a и b - одна и та же матрица
    {
        for ( int i = b.nRow; --i > 0; )
        {
            double * r = b[i];
            for ( int j = i; --j >= 0; )
            {
                double t = r[j];
                r[j] = b[j][i];
                b[j][i] = t;
            }
        }
    }
    else
    {
        for ( int i = a.nRow; --i >= 0; )
        {
            const double * r = a[i];
            for ( int j = a.nCol; --j >= 0; )
            {
                b[j][i] = r[j];
            }
        }
    }
    return true;
}

bool inverse ( const IMatrix<double> & a, IMatrix<double> & b ) // a * b = 1
{
    if ( a.nCol != b.nCol || a.nRow != b.nRow ) return false;
    if ( a.nCol != a.nRow || a.nRow == 0 ) return false;
    SLU_Gauss slu ( a.nRow, a );
    HMatrix<double> m ( b );
    double * c = m[0];
    int i;
    for ( i = m.nRow; --i >= 0; ) c[i] = 0.;
    for ( i = m.nRow; --i >= 0; )
    {
        c[i] = 1.;
        if ( ! slu.solve ( c, m[i] ) ) return false;
        if ( i ) c[i] = 0.;
    }
    return trans ( m, b );
}

// Cингулярное разложение: A = U * W * V

bool svd ( const IMatrix<double> & A, IMatrix<double> & U, IMatrix<double> & W, IMatrix<double> & V )
{
    if ( A.nRow == 0 ) return false;
    const nat nCol = A.nCol;
    const nat nRow = A.nRow;
    if ( nCol > nRow )
    {
        HMatrix<double> C ( nCol, nRow ), B ( nCol, nRow );
        if ( ! trans ( A, C ) ) return false;
        if ( ! svd ( C, V, B, U ) ) return false;
        trans ( V, V );
        trans ( U, U );
        return trans ( B, W );
    }
    if ( nCol < 1 || nRow < 1 ) return false;
    if ( U.nCol != nRow || U.nRow != nRow ) return false;
    if ( V.nCol != nCol || V.nRow != nCol ) return false;
    if ( ! copy ( A, W ) ) return false;
    double * const * u = U;
    double * const * w = W;
    double * const * v = V;
// Хаусхолдерово приведение к двухдиагональной форме
    nat i;
    for ( i = 0; i < nCol; ++i )
    {
        if ( i + 2 > nRow ) break;
        const int i1 = i + 1;
        double s = 0.;
        nat k;
        for ( k = i; k < nRow; ++k )
        {
            s += w[k][i] * w[k][i];
        }
        if ( s > 0 )
        {
            s = sqrt ( s );
            const double f = w[i][i];
            double g = 1. / ( f < 0 ? f - s : f + s );
            w[i][i] = g < 0 ? s : -s;
            for ( k = i1; k < nRow; ++k )
            {
                w[k][i] *= g;
            }
            g = 1. + fabs(f) / s;
            for ( nat j = i1; j < nCol; ++j )
            {
                double s = w[i][j];
                for ( k = i1; k < nRow; ++k )
                {
                    s += w[k][j] * w[k][i];
                }
                const double h = g * s;
                w[i][j] -= h;
                for ( k = i1; k < nRow; ++k )
                {
                    w[k][j] -= h * w[k][i];
                }
            }
        }
        const nat i2 = i + 2;
        if ( i2 >= nCol ) continue;
        s = 0.;
        for ( k = i1; k < nCol; ++k )
        {
            s += w[i][k] * w[i][k];
        }
        if ( s > 0 )
        {
            s = sqrt ( s );
            const double f = w[i][i1];
            double g = 1. / ( f < 0 ? f - s : f + s );
            w[i][i1] = g < 0 ? s : -s;
            for ( k = i2; k < nCol; ++k )
            {
                w[i][k] *= g;
            }
            g = 1. + fabs(f) / s;
            for ( nat j = i1; j < nRow; ++j )
            {
                double s = w[j][i1];
                for ( k = i2; k < nCol; ++k )
                {
                    s += w[j][k] * w[i][k];
                }
                const double h = g * s;
                w[j][i1] -= h;
                for ( k = i2; k < nCol; ++k )
                {
                    w[j][k] -= h * w[i][k];
                }
            }
        }
    }
    U.fill ( 0. );
// Накопление левосторонних преобразований
    for ( i = nRow; --i < nRow; )
    {
        u[i][i] = 1.;
        if ( i >= nCol || i >= nRow - 1 ) continue;
        if ( w[i][i] )
        {
            double s = 1.;
            const nat i1 = i + 1;
            nat k;
            for ( k = i1; k < nRow; ++k )
            {
                s += w[k][i] * w[k][i];
            }
            const double g = 2. / s;
            for ( nat j = i; j < nRow; ++j )
            {
                double s = u[i][j];
                for ( k = i1; k < nRow; ++k )
                {
                    s += u[k][j] * w[k][i];
                }
                const double h = g * s;
                u[i][j] -= h;
                for ( k = i1; k < nRow; ++k )
                {
                    u[k][j] -= h * w[k][i];
                }
            }
            for ( k = i1; k < nRow; ++k ) w[k][i] = 0.;
        }
    }
    V.fill ( 0. );
// Накопление правосторонних преобразований
    for ( i = nCol; --i < nCol; )
    {
        v[i][i] = 1.;
        if ( i + 2 >= nCol ) continue;
        const int i1 = i + 1;
        const int i2 = i + 2;
        if ( w[i][i1] )
        {
            double s = 1.;
            nat k;
            for ( k = i2; k < nCol; ++k )
            {
                s += w[i][k] * w[i][k];
            }
            const double g = 2. / s;
            for ( nat j = i1; j < nCol; ++j )
            {
                double s = v[j][i1];
                for ( k = i2; k < nCol; ++k )
                {
                    s += v[j][k] * w[i][k];
                }
                const double h = g * s;
                v[j][i1] -= h;
                for ( k = i2; k < nCol; ++k )
                {
                    v[j][k] -= h * w[i][k];
                }
            }
            for ( k = i2; k < nCol; ++k ) w[i][k] = 0.;
        }
    }
// Вычисляем матричную норму и переносим наддиагональные элементы в массив e
    double norm = 0.;
    double * e = new double[nCol];
    for ( i = nCol; --i < nCol; )
    {
        double sum = fabs ( w[i][i] );
        if ( i > 0 )
        {
            sum += fabs ( e[i] = w[i-1][i] );
            w[i-1][i] = 0.;
        }
        else
        {
            e[i] = 0.;
        }
        if ( norm < sum ) norm = sum;
    }
// Получение диагональной формы
    for ( nat k = nCol; --k < nCol; )
    {
        nat n = 0;
m0:     nat l = k;
        for ( ; ; --l )
        {
            if ( l == 0 ) goto m2;
            const int l1 = l - 1;
            if ( fabs ( e[l] ) + norm == norm ) goto m2;
            if ( fabs ( w[l1][l1] ) + norm == norm ) goto m1;
        }
m1:
        {
            double c = 0.;
            double s = 1.;
            const nat l1 = l - 1;
            for ( nat i = l; i <= k; ++i )
            {
                const double f = s * e[i];
                e[i] *= c;
                if ( fabs ( f ) + norm == norm ) goto m2;
                const double g = w[i][i];
                const double h = w[i][i] = sqrt ( f * f + g * g );
                c =   g / h;
                s = - f / h;
                for ( nat j = 0; j < nRow; ++j )
                {
                    const double y = u[j][l1];
                    const double z = u[j][i ];
                    u[j][l1] = y * c + z * s;
                    u[j][i ] = z * c - y * s;
                }
            }
        }
m2:
        {
            double z = w[k][k];
            if ( l < k )
            {
                if ( ++n == 30 ) return false; // Проверка сходимости
                double x = w[l][l];
                double y = w[k-1][k-1];
                double g = e[k-1];
                double h = e[k];
                double f = ( ( y - z ) * ( y + z ) + ( g - h ) * ( g + h ) ) / ( 2. * h * y );
                g = sqrt ( f * f + 1. );
                f = ( ( x - z ) * ( x + z ) + h * ( y / ( f < 0 ? f - g : f + g ) - h ) ) / x;
                double c = 1.;
                double s = 1.;
                for ( nat i = l + 1; i <= k; ++i )
                {
                    const int i1 = i - 1;
                    g = e[i];
                    y = w[i][i];
                    h = s * g;
                    g = c * g;
                    e[i1] = z = sqrt ( f * f + h * h );
                    c = f / z;
                    s = h / z;
                    f = x * c + g * s;
                    g = g * c - x * s;
                    h = y * s;
                    y = y * c;
                    nat j;
                    for ( j = 0; j < nCol; ++j )
                    {
                        x = v[i1][j];
                        z = v[i ][j];
                        v[i1][j] = x * c + z * s;
                        v[i ][j] = z * c - x * s;
                    }
                    w[i1][i1] = z = sqrt ( f * f + h * h );
                    if ( z )
                    {
                        c = f / z;
                        s = h / z;
                    }
                    f = c * g + s * y;
                    x = c * y - s * g;
                    for ( j = 0; j < nRow; ++j )
                    {
                        y = u[j][i1];
                        z = u[j][i ];
                        u[j][i1] = y * c + z * s;
                        u[j][i ] = z * c - y * s;
                    }
                }
                e[l] = 0.;
                e[k] = f;
                w[k][k] = x;
                goto m0;
            }
            if ( z < 0 )
            {
                w[k][k] = -z;
                for ( nat j = 0; j < nCol; ++j ) v[k][j] = - v[k][j];
            }
        }
    }
    delete[] e;
    return true;
}
