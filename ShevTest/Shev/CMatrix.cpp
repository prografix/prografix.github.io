
// 10.04.2006, 18.04.2006, 20.10.2012

#include "math.h"
#include "CMatrix.h"

// Транспонирование

bool trans ( const IMatrix<Complex> & a, IMatrix<Complex> & b )
{
    if ( a.nCol != b.nRow || a.nRow != b.nCol || a.nCol == 0 ) return false;
    if ( a[0] == b[0] ) // a и b - одна и та же матрица
    {
        for ( int i = b.nRow; --i > 0; )
        {
            Complex * r = b[i];
            for ( int j = i; --j >= 0; )
            {
                Complex t = r[j];
                r[j] = ~b[j][i];
                b[j][i] = ~t;
            }
            b[i][i] = ~b[i][i];
        }
    }
    else
    {
        for ( int i = a.nRow; --i >= 0; )
        {
            const Complex * r = a[i];
            for ( int j = a.nCol; --j >= 0; )
            {
                b[j][i] = ~r[j];
            }
        }
    }
    return true;
}

//*********************** 18.04.2006 **************************//
//
//           Решение задачи наименьших квадратов 
//          при помощи преобразований Хаусхолдера.
//
//*********************** 20.10.2012 **************************//

CLSS_H::CLSS_H ( IMatrix<Complex> & p ) : a(p)
{
    if ( a.nRow < a.nCol || a.nCol <= 0 )
    {
        alpha = 0;
        return;
    }
    alpha = new Complex[a.nCol+a.nCol+a.nRow];
    beta = new double[a.nCol];
    pivot1 = new nat[a.nCol+a.nRow];
    pivot2 = pivot1 + a.nCol;
    nat i, j, k;
    for ( k = 0; k < a.nRow; ++k )
    {
        pivot2[k] = k;
        if ( k < a.nCol ) pivot1[k] = k;
    }
    for ( k = 0; k < a.nCol; ++k )
    {
        double sigma = 0.;
        nat jmax, imax;
        for ( j = k; j < a.nCol; ++j )
        {
            double s = 0.;
            double q = 0.;
            nat im;
            for ( i = k; i < a.nRow; ++i )
            {
                const Complex & c = a[i][j];
                const double t = c.re * c.re + c.im * c.im;
                if ( q < t ) q = t, im = i;
                s += t;
            }
            if ( sigma < s ) sigma = s, jmax = j, imax = im;
        }
        if ( sigma == 0 )
        {
            delete[] pivot1;
            delete[] beta;
            delete[] alpha;
            alpha = 0;
            return;
        }
        if ( jmax != k ) // перестановка столбцов
        {
            i = pivot1[k];
            pivot1[k] = pivot1[jmax];
            pivot1[jmax] = i;
            for ( i = 0; i < a.nRow; ++i )
            {
                const Complex t = a[i][k];
                a[i][k] = a[i][jmax];
                a[i][jmax] = t;
            }
        }
        if ( imax != k ) // перестановка строк
        {
            i = pivot2[k];
            pivot2[k] = pivot2[imax];
            pivot2[imax] = i;
            for ( i = 0; i < a.nCol; ++i )
            {
                const Complex t = a[k][i];
                a[k][i] = a[imax][i];
                a[imax][i] = t;
            }
        }
        Complex & akk = a[k][k];
        const double absakk = abs ( akk );
        const double rel = sqrt ( sigma ) / absakk;
        alpha[k] = akk * ( - rel );
        beta[k] = rel / ( sigma + sigma * rel );
        akk *= 1. + rel;
        for ( j = k + 1; j < a.nCol; ++j )
        {
            Complex gamma = 0.;
            for ( i = k; i < a.nRow; ++i )
            {
                gamma += ~a[i][k] * a[i][j];
            }
            gamma *= beta[k];
            for ( i = k; i < a.nRow; ++i )
            {
                a[i][j] -= a[i][k] * gamma;
            }
        }
    }
}

bool CLSS_H::solve ( const Complex * b, Complex * x ) const
{
    if ( alpha == 0 || b == 0 || x == 0 ) return false;
    Complex * z = alpha + a.nCol;
    nat i, j;
    for ( i = 0; i < a.nRow; ++i )
    {
        z[i] = b[pivot2[i]];
    }
    for ( j = 0; j < a.nCol; ++j )
    {
        Complex gamma = 0.;
        for ( i = j; i < a.nRow; ++i )
        {
            gamma += ~a[i][j] * z[i];
        }
        gamma *= beta[j];
        for ( i = j; i < a.nRow; ++i )
        {
            z[i] -= a[i][j] * gamma;
        }
    }
    i = a.nCol - 1;
    Complex * y = z + a.nRow;
    y[i] = z[i] / alpha[i];
    do
    {
        --i;
        y[i] = z[i];
        for ( j = i + 1; j < a.nCol; ++j )
        {
            y[i] -= y[j] * a[i][j];
        }
        y[i] /= alpha[i];
    }
    while ( i > 0 );
    for ( i = 0; i < a.nCol; ++i )
    {
        x[pivot1[i]] = y[i];
    }
    return true;
}

CLSS_H::~CLSS_H ()
{
    if ( alpha )
    {
        delete[] pivot1;
        delete[] beta;
        delete[] alpha;
    }
}

bool lss_h ( IMatrix<Complex> & a, const Complex * b, Complex * x )
{
    CLSS_H lss ( a );
    return lss.solve ( b, x );
}