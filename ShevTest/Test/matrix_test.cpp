
#include "math.h"

#include "../Shev/mathem.h"
#include "../Shev/cmatrix.h"
#include "../Shev/rand.h"
#include "../Shev/ShevArray2.h"

#include "display.h"

double timeInSec();

namespace {

void message ( const IMatrix<double> & a )
{
    for ( nat i = 0; i < a.nRow; ++i )
    {
        for ( nat j = 0; j < a.nCol; ++j )
        {
            const double t = a[i][j];
//            if ( fabs ( t ) < 1e-13 ) t = 0.;
            display << t;
        }
        display << NL;
    }
}

bool inverse2 ( const IMatrix<double> & a, IMatrix<double> & b ) // a * b = 1
{
    const nat n = a.nRow;
    if ( b.nRow != n || b.nCol != a.nCol ) return false;
    if ( a.nCol != n || n == 0 ) return false;
    DynArray2<double> data ( n, n+n );
    nat i, j;
    for ( i = 0; i < n; ++i )
    {
        const double * p = a[i];
        ArrRef<double> r = data[i];
        for ( j = 0; j < n; ++j )
        {
            r[j] = p[j];
            r[j+n] = 0.;
        }
        r[i+n] = 1.;
    }
    if ( ! slu_gauss ( data ) ) return false;
    for ( i = 0; i < n; ++i )
    {
        double * p = b[i];
        double * r = & data[i][n];
        for ( j = 0; j < n; ++j ) p[j] = r[j];
    }
    return true;
}

void inverse_test()
{
    static PRand rand;
    for ( nat n = 3; n < 12; ++n )
    {
        HMatrix<double> A ( n, n ), B ( n, n ), C ( n, n );
        for ( nat j = 0; j < n; ++j )
        {
            for ( nat i = 0; i < n; ++i )
            {
                A[j][i] = 1. / ( 1 + i + j );
//                A[j][i] = rand();
            }
        }
        double max1 = 0., max2 = 0, t1=0, t2=0;
        double t0 = timeInSec();
        if ( inverse2 ( A, B ) )
        {
            t1 = timeInSec() - t0;
            multi ( A, B, C );
            for ( nat i = 0; i < n; ++i ) C[i][i] -= 1.;
            for ( nat j = 0; j < n; ++j )
            {
                for ( nat i = 0; i < n; ++i )
                {
                    double a = fabs ( C[j][i] );
                    if ( max1 < a ) max1 = a;
                }
            }
        }
        t0 = timeInSec();
        if ( inverse ( A, B ) )
        {
            t2 = timeInSec() - t0;
            multi ( A, B, C );
            for ( nat i = 0; i < n; ++i ) C[i][i] -= 1.;
            for ( nat j = 0; j < n; ++j )
            {
                for ( nat i = 0; i < n; ++i )
                {
                    double a = fabs ( C[j][i] );
                    if ( max2 < a ) max2 = a;
                }
            }
        }
        display << n << t1 / t2 << max1 / max2 << NL;
    }
}

void matrix()
{
    const nat m = 7;
    const nat n = 4;
    HMatrix<double> aa ( m, n ), U ( m, m ), W ( m, n ), V ( n, n );
    aa[0][0] =-0.0220; aa[0][1] = 0.0745; aa[0][2] = 0.0202; aa[0][3] =-0.0624;
    aa[1][0] = 0.0022; aa[1][1] =-0.0065; aa[1][2] = 0.0319; aa[1][3] =-0.017;
    aa[2][0] =-0.0151; aa[2][1] = 0.0332; aa[2][2] =-0.0030; aa[2][3] =-0.0062;
    aa[3][0] =-0.0117; aa[3][1] = 0.0193; aa[3][2] = 0.0043; aa[3][3] =-0.0002;
    aa[4][0] = 0.0310; aa[4][1] =-0.0427; aa[4][2] =-0.0179; aa[4][3] =-0.0054;
    aa[5][0] = 0.0184; aa[5][1] =-0.0524; aa[5][2] =-0.0216; aa[5][3] = 0.0407;
    aa[6][0] = 0.0006; aa[6][1] =-0.0446; aa[6][2] =-0.0299; aa[6][3] = 0.077;
    if ( svd ( aa, U, W, V ) )
    {
//        message(U);
        W *= 100;
        message(W);
//        message(V);
        HMatrix<double> b ( m, n );
        multi ( U, W, b );
        multi ( b, V, b );
        minus ( b, aa, b );
//        message(b);
    }
    return;
    static PRand rand;
    double max = 0.;
    for ( nat k = 0; k < 10; ++k )
    {
        const nat m = 1 + nat ( 10. * rand() );
        const nat n = 1 + nat ( 10. * rand() );
        HMatrix<double> a ( m, n ), U ( m, m ), W ( m, n ), V ( n, n );
/*
        a[0][0] = 22; a[0][1] = 10; a[0][2] =  2; a[0][3] =   3; a[0][4] =  7;
        a[1][0] = 14; a[1][1] =  7; a[1][2] = 10; a[1][3] =   0; a[1][4] =  8;
        a[2][0] = -1; a[2][1] = 13; a[2][2] = -1; a[2][3] = -11; a[2][4] =  3;
        a[3][0] = -3; a[3][1] = -2; a[3][2] = 13; a[3][3] =  -2; a[3][4] =  4;
        a[4][0] =  9; a[4][1] =  8; a[4][2] =  1; a[4][3] =  -2; a[4][4] =  4;
        a[5][0] =  9; a[5][1] =  1; a[5][2] = -7; a[5][3] =   5; a[5][4] = -1;
        a[6][0] =  2; a[6][1] = -6; a[6][2] =  6; a[6][3] =   5; a[6][4] =  1;
        a[7][0] =  4; a[7][1] =  5; a[7][2] =  0; a[7][3] =  -2; a[7][4] =  2;
/*/       for ( nat j = 0; j < a.nRow; ++j )
        {
            for ( nat i = 0; i < a.nCol; ++i )
            {
                a[j][i] = int ( 10. * rand() );
            }
        }//*/
        double n1 = norm1(a);
        double n2 = norm2(a);
        double nF = normF(a);
        double nU = normU(a);
/*        if ( n2 > nF ) message("n2 > nF");
        if ( nF > n2 * sqrt(double(a.nCol)) ) message("nF > n2 * sqrt(double(a.nCol))");
        if ( nU > n2 * sqrt(double(a.nCol)) ) message("nU > n2 * sqrt(double(a.nCol))");
        if ( n2 > nU * sqrt(double(a.nRow)) ) message("n2 > nU * sqrt(double(a.nRow))");
        if ( n1 > n2 * sqrt(double(a.nRow)) ) message("n1 > n2 * sqrt(double(a.nRow))");
        if ( n2 > n1 * sqrt(double(a.nCol)) ) message("n2 > n1 * sqrt(double(a.nCol))");
        if ( n2 * n2 > n1 * nU ) message("n2 * n2 > n1 * nU");*/
display << n1 << n2 << nF << nU << NL;
        if ( svd ( a, U, W, V ) )
        {
//            message(a);
//            message("");
//            message(W);
//            message("");
//            trans ( U, U );
            HMatrix<double> b ( m, n );
            multi ( U, W, b );
            multi ( b, V, b );
            minus ( b, a, b );
//            message(b);
            for ( nat j = 0; j < a.nRow; ++j )
            {
                for ( nat i = 0; i < a.nCol; ++i )
                {
                    double t = fabs ( b[j][i] );
                    if ( t > max ) max = t;
                }
            }
    //        message("");
    //        message(W);
    //        message(V);
    /*        double da, db;
            determinant ( a, da );
            determinant ( W, db );
            message(da, db);
    /*        for ( nat j = 0; j < U.nRow; ++j )
            {
                double s1 = 0;
                double s2 = 0;
                for ( nat i = 0; i < U.nCol; ++i )
                {
                    s1 += U[j][i] * U[j][i];
                    s2 += U[i][j] * U[i][j];
                }
                message(s1,s2);
            }
            message("");*/
        }
        else
        {
//            message("no");
        }
    }
//    message(max);
//    message("end");
}

void cmatrix_test ()
{
    nat i, j;
    static PRand  rand;
    SMatrix<Complex, 6, 7> a, b, c;
    for ( i = 0; i < a.nRow; ++i )
    for ( j = 0; j < a.nCol; ++j )
    {
        a[i][j] = c[i][j] = Complex ( rand() * 2 - 1, rand() * 2 - 1 );
    }
    c *= 2.;
    plus ( a, a, a );
    minus ( a, c, a );
    double sum = 0.;
    for ( i = 0; i < a.nRow; ++i )
    for ( j = 0; j < a.nCol; ++j )
    {
        sum += abs ( a[i][j] );
    }
    display << sum << NL;
    trans ( a, b );
    multi ( a, b, c );
    sum = 0.;
    for ( i = 0; i < c.nRow; ++i )
    for ( j = 0; j < c.nCol; ++j )
    {
        sum += fabs ( c[i][i].im );
    }
    display << sum << NL;
}

} // namespace

void matrix_test ()
{
//    inverse_test();
    cmatrix_test ();
//    matrix();
}