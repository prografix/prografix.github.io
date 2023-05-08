
#include "math.h"

#include "../Shev/mathem.h"
#include "../Shev/LinAlg.h"
#include "../Shev/complex.h"
#include "../Shev/rand.h"
#include "../Shev/ShevArray2.h"
#include "../Shev/func1t.h"
#include "../Shev/func1d.h"
#include "../Shev/Timer.h"
#include "../Shev/tune.h"
#include "../Shev/approx2d.h"
#include "../Shev/approx3d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/Vector3d.h"

#include "display.h"

namespace {

void root2_test ()
{
    double x1[2], x2[2];
    static PNormalRand rand;
    double max10 = 0, max11 = 0, max20 = 0, max21 = 0;
    double sum10 = 0, sum11 = 0, sum20 = 0, sum21 = 0;
    for ( nat i = 0; i < 100000; ++i )
    {
        const double a = rand();
        const double b = rand();
        const double c = rand();
        nat n1 = root2 ( a, b, c, x1 );
        switch ( n1 )
        {
        case 1:
            display << i << "1" << x1[0] << NL; break;
        case 2:
            {
                double u0 = ( a * x1[0] + b ) * x1[0] + c;
                double u1 = ( a * x1[1] + b ) * x1[1] + c;
                if ( max10 < u0 ) max10 = u0;
                if ( max11 < u1 ) max11 = u1;
                sum10 += fabs ( u0 );
                sum11 += fabs ( u1 );
                if ( fabs ( x1[0] ) > fabs ( x1[1] ) )
                    i=i;
            }
        }
        nat n2 = root2 ( b / a, c / a, x2 );
        switch ( n2 )
        {
        case 1:
            display << i << "2" << x2[0] << NL; break;
        case 2:
            {
                double u0 = ( a * x2[0] + b ) * x2[0] + c;
                double u1 = ( a * x2[1] + b ) * x2[1] + c;
                if ( max20 < u0 ) max20 = u0;
                if ( max21 < u1 ) max21 = u1;
                sum20 += fabs ( u0 );
                sum21 += fabs ( u1 );
                if ( fabs ( x2[0] ) > fabs ( x2[1] ) )
                    i=i;
            }
        }
    }
    display << max10 << max11 << sum10 << sum11 << NL;
    display << max20 << max21 << sum20 << sum21 << NL << NL;
}

void _root3_test ()
{
    double max = 0.;
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
        const double x = rand();
        const double y = root3 ( x );
        const double d = fabs ( 1. - _pow3 ( y ) / x );
        if ( d > 1e-14 )
            display << "fabs(d) =" << d << i << NL;
        if ( max < d ) max = d;
    }
    display << "max =" << max << NL;
}

void _root5_test ()
{
    double max = 0.;
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
        const double x = rand();
        const double y = root5 ( x );
        const double d = fabs ( 1. - _pow5 ( y ) / x );
        if ( d > 1e-14 )
            display << "fabs(d) =" << d << i << NL;
        if ( d > 1e-15 )
            root5 ( x );
        if ( max < d ) max = d;
    }
    display << "max =" << max << NL;
}

void _root7_test ()
{
    double max = 0.;
    static PNormalRand rand;
    for ( nat i = 0; i < 1000000; ++i )
    {
        const double x = rand();
        const double y = root7 ( x );
        const double d = fabs ( 1. - _pow7 ( y ) / x );
        if ( d > 1e-14 )
            display << "fabs(d) =" << d << i << NL;
        if ( max < d ) max = d;
    }
    display << "max =" << max << NL;
}

void root3_test ()
{
    const nat m = 3;
    double co[m], x[m], max = 0.;
    static PNormalRand rand;
    for ( nat i = 0; i < 1000; ++i )
    {
        for ( nat k = 0; k < m; ++k )
        {
            co[k] = rand();
        }
        nat n = root3 ( co[0], co[1], co[2], x );
        {
            for ( nat k = 0; k < n; ++k )
            {
                double t = 1.;
                for ( nat j = 0; j < m; ++j )
                {
                    t = t * x[k] + co[j];
                }
                const double a = fabs(t);
                if ( a > 1e-14 )
                    display << "fabs(t) =" << a << i << k << NL;
                if ( max < a ) max = a;
            }
        }
        co[0] = 0.;
        n = root3s ( co[1], co[2], x );
        {
            for ( nat k = 0; k < n; ++k )
            {
                double t = 1.;
                for ( nat j = 0; j < m; ++j )
                {
                    t = t * x[k] + co[j];
                }
                const double a = fabs(t);
                if ( a > 1e-14 )
                    display << "fabs(t) =" << a << i << k << NL;
                if ( max < a ) max = a;
            }
        }
    }
    display << "max =" << max << NL;
}

void root4_test ()
{
    const nat m = 4;
    double co[m], x[m], max = 0.;
    static PNormalRand rand;
    const double eps = 3e-12;
    for ( nat i = 0; i < 1000000; ++i )
    {
        for ( nat k = 0; k < m; ++k )
        {
            co[k] = ( 3 * rand() );
        }
        nat n = root4 ( co[0], co[1], co[2], co[3], x );
        {
            for ( nat k = 0; k < n; ++k )
            {
                double t = 1.;
                for ( nat j = 0; j < m; ++j )
                {
                    t = t * x[k] + co[j];
                }
                const double a = fabs(t);
                if ( a > eps )
                    display << a << i << k << NL;
                if ( max < a ) max = a;
            }
        }
        co[0] = 0.;
        n = root4s ( co[1], co[2], co[3], x );
        {
            for ( nat k = 0; k < n; ++k )
            {
                double t = 1.;
                for ( nat j = 0; j < m; ++j )
                {
                    t = t * x[k] + co[j];
                }
                const double a = fabs(t);
                if ( a > eps )
                    display << a << i << k << NL;
                if ( max < a ) max = a;
            }
        }
    }
    display << "max =" << max << NL;
}

void root5_test ()
{
    const nat m = 5;
    double co[m], x[m], max = 0.;
    static PNormalRand rand;
    for ( nat i = 0; i < 5000; ++i )
    {
        for ( nat k = 0; k < m; ++k )
        {
            co[k] =  ( 3 * rand() );
        }
        nat n = root5 ( co[0], co[1], co[2], co[3], co[4], x );
        {
            for ( nat k = 0; k < n; ++k )
            {
                double t = 1.;
                for ( nat j = 0; j < m; ++j )
                {
                    t = t * x[k] + co[j];
                }
                const double a = fabs(t);
                if ( a > 1e-11 )
                    display << "fabs(t) =" << a << i << k << n << NL;
                if ( max < a ) max = a;
            }
        }
    }
    display << "max =" << max << NL;
}

void rootN_test ()
{
    const nat m = 6;
    double co[m], re[m], im[m];
    static PRand rand;
    for ( nat i = 0; i < 100; ++i )
    {
        for ( nat k = 0; k < m; ++k )
        {
            co[k] = 10. * ( 0.5 - rand() );
        }
        if ( rootN ( m, co, re, im ) )
        {
            for ( nat k = 0; k < m; ++k )
            {
                Complex t = 1.;
                Complex x ( re[k], im[k] );
                for ( nat j = 0; j < m; ++j )
                {
                    t = t * x + co[j];
                }
                const double a = abs(t);
                if ( a > 1e-12 )
                    display << "abs(t) =" << a << i << k << NL;
            }
        }
        else
            display << "err" << i << NL;
    }
    display << "end" << NL;
}

void lss_test1()
{
    SMatrix<double, 6, 5> a;
    a[0][0] = -74; a[0][1] =  80; a[0][2] = 18; a[0][3] = -11; a[0][4] = -4;
    a[1][0] =  14; a[1][1] = -69; a[1][2] = 21; a[1][3] =  28; a[1][4] =  0;
    a[2][0] =  66; a[2][1] = -72; a[2][2] = -5; a[2][3] =   7; a[2][4] =  1;
    a[3][0] = -12; a[3][1] =  66; a[3][2] =-30; a[3][3] = -23; a[3][4] =  3;
    a[4][0] =   3; a[4][1] =   8; a[4][2] = -7; a[4][3] =  -4; a[4][4] =  1;
    a[5][0] =   4; a[5][1] = -12; a[5][2] =  4; a[5][3] =   4; a[5][4] =  0;
    FixArray<double, 6> b;
    FixArray<double, 5> x, y, z, c;
    b[0] = 51; b[1] = -61; b[2] = -56; b[3] = 69; b[4] = 10; b[5] = -12;
//    c[0] = 1; c[1] = 2; c[2] = -1; c[3] = 3; c[4] = -4;
//    b[0] = -56; b[1] = 52; b[2] = 764; b[3] = 4096; b[4] = -13276; b[5] = 8421;
//    c[0] = 0; c[1] = 0; c[2] = 0; c[3] = 0; c[4] = 0;
    b[0] = -5; b[1] = -9; b[2] = 708; b[3] = 4165; b[4] = -13266; b[5] = 8409;
    c[0] = 1; c[1] = 2; c[2] = -1; c[3] = 3; c[4] = -4;
    ortholin ( a, b, x );
    LSS_H lss ( a );
    lss.solve ( b, y );
    SMatrix<double, 5, 5> mat2;
    mat2.fill ( 0 );
    double b2[5];
    int i;
    for ( i = 0; i < 5; ++i )
    {
        for ( int j = 0; j < 5; ++j )
        {
            for ( int k = 0; k < 6; ++k ) mat2[i][j] += a[k][i] * a[k][j];
        }
        b2[i] = 0.;
        for ( int k = 0; k < 6; ++k ) b2[i] += a[k][i] * b[k];
    }
    SLU_Gauss slug ( 5, mat2 );
    slug.solve ( b2, z() );
//    lss_h ( 6, 5, a, b, y );
    for ( i = 0; i < 5; ++i ) display << x[i]-c[i] << y[i]-c[i] << z[i]-c[i] << NL;
}

void lss_test2()
{
    SMatrix<double, 6, 5> a;
    a[0][0] =    36; a[0][1] =    -630; a[0][2] =    3360; a[0][3] =    -7560; a[0][4] =    7560;
    a[1][0] =  -630; a[1][1] =   14700; a[1][2] =  -88200; a[1][3] =   211680; a[1][4] =  -220500;
    a[2][0] =  3360; a[2][1] =  -88200; a[2][2] =  564480; a[2][3] = -1411200; a[2][4] =  1512000;
    a[3][0] = -7560; a[3][1] =  211680; a[3][2] =-1411200; a[3][3] =  3628800; a[3][4] = -3969000;
    a[4][0] =  7560; a[4][1] = -220500; a[4][2] = 1512000; a[4][3] = -3969000; a[4][4] =  4410000;
    a[5][0] = -2772; a[5][1] =   83160; a[5][2] = -582120; a[5][3] =  1552320; a[5][4] = -1746360;
    FixArray<double, 6> b;
    FixArray<double, 5> x, y, z, c;
    b[0] = 463; b[1] = -13860; b[2] = 97020; b[3] = -258720; b[4] = 291060; b[5] = -116424;
//    b[0] = -4157;   b[1] = -17820; b[2] = 93555; b[3] = -261800; b[4] = 288288; b[5] = -118944;
    c[0] = 1; c[1] = 1./2; c[2] = 1./3; c[3] = 1./4; c[4] = 1./5;
    LSS_H lss ( a );
    lss.solve ( b, y );
    ortholin ( a, b, x );
    SMatrix<double, 5, 5> mat2;
    mat2.fill ( 0 );
    double b2[5];
    int i;
    for ( i = 0; i < 5; ++i )
    {
        for ( int j = 0; j < 5; ++j )
        {
            for ( int k = 0; k < 6; ++k ) mat2[i][j] += a[k][i] * a[k][j];
        }
        b2[i] = 0;
        for ( int k = 0; k < 6; ++k ) b2[i] += a[k][i] * b[k];
        z[i] = b2[i];
    }
    SLU_Gauss slug ( 5, mat2 );
    slug.solve ( z(), z() );
    for ( i = 0; i < 5; ++i ) display << x[i]-c[i] << y[i]-c[i] << z[i]-c[i] << NL;
}

void lin_test1()
{
    double c[5];
    c[0] = 1; c[1] = 1./2; c[2] = 1./3; c[3] = 1./4; c[4] = 1./5;
/*
    SMatrix<double, 5, 5> mat;
    double * const * a = mat;
    a[0][0] =    36; a[0][1] =    -630; a[0][2] =    3360; a[0][3] =    -7560; a[0][4] =    7560;
    a[1][0] =  -630; a[1][1] =   14700; a[1][2] =  -88200; a[1][3] =   211680; a[1][4] =  -220500;
    a[2][0] =  3360; a[2][1] =  -88200; a[2][2] =  564480; a[2][3] = -1411200; a[2][4] =  1512000;
    a[3][0] = -7560; a[3][1] =  211680; a[3][2] =-1411200; a[3][3] =  3628800; a[3][4] = -3969000;
    a[4][0] =  7560; a[4][1] = -220500; a[4][2] = 1512000; a[4][3] = -3969000; a[4][4] =  4410000;
    double x[5], y[5], b[5], c[5];
    b[0] = 463; b[1] = -13860; b[2] = 97020; b[3] = -258720; b[4] = 291060;
    const SM_Chol sl1 ( 5, a );
    sl1.solve ( b, x );
    const SM_LDLt sl2 ( 5, a );
    sl2.solve ( b, y );
    for ( nat i = 0; i < 5; ++i ) display << x[i] - c[i] << NL;
/*/
    DynArray2<double> a ( 5, 6 );
    a[0][0] =    36; a[0][1] =    -630; a[0][2] =    3360; a[0][3] =    -7560; a[0][4] =     7560; a[0][5] =     463;
    a[1][0] =  -630; a[1][1] =   14700; a[1][2] =  -88200; a[1][3] =   211680; a[1][4] =  -220500; a[1][5] =  -13860;
    a[2][0] =  3360; a[2][1] =  -88200; a[2][2] =  564480; a[2][3] = -1411200; a[2][4] =  1512000; a[2][5] =   97020;
    a[3][0] = -7560; a[3][1] =  211680; a[3][2] =-1411200; a[3][3] =  3628800; a[3][4] = -3969000; a[3][5] = -258720;
    a[4][0] =  7560; a[4][1] = -220500; a[4][2] = 1512000; a[4][3] = -3969000; a[4][4] =  4410000; a[4][5] =  291060;
    slu_gauss ( a );
    for ( nat i = 0; i < 5; ++i ) display << a[i][5] - c[i] << NL;//*/
    display << NL;
}

void lin_test2()
{
    static PNormalRand rand;
    nat iless = 0, iequi = 0, igrea = 0;
    double max = 0.;
    const nat n = 11;
    SMatrix<double, n, n> mat;
    for ( nat k = 0; k < 100; ++k )
    {
        double * const * a = mat;
        double x[n], y[n], b[n];
        nat i, j;
        for ( i = 0; i < n; ++i )
        {
            for ( j = 0; j < n; ++j )
            {
                a[i][j] = rand() / ( i + j + 1 );
//                a[i][j] = 1. / ( i + j + 1 );
            }
            x[i] = i * rand();
        }
//if(k!=205) continue;
        for ( i = 0; i < n; ++i )
        {
            double d = 0.;
            for ( j = 0; j < n; ++j ) d += a[i][j] * x[j];
            b[i] = d;
        }
        double d1 = 0.;
        const nat m = n;
        if ( slu_orthoH1 ( m, n, a, b, y ) )
        {
            for ( j = 0; j < m; ++j )
            {
                double t = y[j] - x[j];
                d1 += t * t;
            }
        }
        else d1 = 1;
        SLU_Gauss sl ( n, a );
        double d2 = 0.;
        if ( sl.solve ( b, y ) )
        {
            for ( j = 0; j < n; ++j )
            {
                double t = y[j] - x[j];
                d2 += t * t;
            }
        }
        else d2 = 1;
//message(sl.condition());
        if(d2>d1) ++iless; else
        if(d2<d1) ++igrea; else
            ++iequi;
//        else 
            display << d1 << d2 << NL;
    }
    display << iless << iequi << igrea << NL;
}

void lin_test3() // для симметричных матриц
{
    PRand rand;
    SMatrix<double, 9, 9> mat, mat2;
    int im = 0;
    double max = 0.;
    for ( int k = 0; k < 100; ++k )
    {
        double * const * a = mat;
        double x[9], y[9], b[9];
        int i;
        for ( i = 0; i < 9; ++i )
        {
            for ( int j = 0; j < 9; ++j )
            {
                a[i][j] = i < j ? 0. : rand() - 0.5;
            }
            x[i] = rand() - 0.5;
        }
        mat2.fill ( 0 );
        for ( i = 0; i < 9; ++i )
        {
            for ( int j = 0; j < 9; ++j )
            {
                for ( int k = 0; k < 9; ++k )
                {
                    mat2[i][j] += a[i][k] * a[j][k];
                }
            }
        }
        double det = 1.;
        for ( i = 0; i < 9; ++i )
        {
            double d = 0.;
            for ( int j = 0; j < 9; ++j ) d += mat2[i][j] * x[j];
            y[i] = b[i] = d;
            det *= a[i][i];
        }
        det *= det;
        SM_Chol sm1 ( 9, mat2 );
        SM_LDLt sm2 ( 9, mat2 );
//message ( sm1.determinant()-det, sm2.determinant()-det ); continue;
        double d1 = 0.;
        if ( sm1.solve ( b, y ) )
        {
            for ( int j = 0; j < 9; ++j )
            {
                double t = y[j] - x[j];
                d1 += t * t;
            }
        }
        else d1 = 1;
        double d2 = 0.;
        if ( sm2.solve ( b, y ) )
        {
            for ( int j = 0; j < 9; ++j )
            {
                double t = y[j] - x[j];
                d2 += t * t;
            }
        }
        else d2 = 1;
        SLU_Gauss sl ( 9, mat2 );
        double d3 = 0.;
        if ( sl.solve ( b, y ) )
        {
            for ( int j = 0; j < 9; ++j )
            {
                double t = y[j] - x[j];
                d3 += t * t;
            }
        }
        else d3 = 1;
display << d1 << d2 << d3 << sl.condition() << NL;
    }
}

void lin_test5()
{
    static PRand rand;
    nat iless = 0, iequi = 0, igrea = 0;
    const nat n = 112;
    SMatrix<double, n, n> mat;
    for ( nat k = 0; k < 100; ++k )
    {
        double * const * a = mat;
        double x[n], y[n], b[n];
        nat i, j;
        for ( i = 0; i < n; ++i )
        {
            for ( j = 0; j < n; ++j )
            {
                a[i][j] = rand() - 0.5;
//                if ( j != k % n ) a[i][j] *= 8.;
//                a[i][j] = ( 1. + 1e-4 * rand() ) / ( i + j + 1 );
            }
            x[i] = ( rand() - 0.5 ) * 8.;
        }
//if(k!=205) continue;
        for ( i = 0; i < n; ++i )
        {
            double d = 0.;
            for ( j = 0; j < n; ++j ) d += a[i][j] * x[j];
            b[i] = d;
        }
        double d1 = 0.;
        SLU_Gauss sl ( n, a );
        if ( sl.solve ( b, y ) )
        {
            for ( j = 0; j < n; ++j )
            {
                double t = y[j] - x[j];
                d1 += t * t;
            }
        }
        else d1 = 1;
        double d2 = 0.;
        if ( sl.solve ( a, b, y ) )
        {
            for ( j = 0; j < n; ++j )
            {
                double t = y[j] - x[j];
                d2 += t * t;
            }
        }
        else d2 = 1;
//message(sl.condition());
        if(d2<d1) ++iless; else
        if(d2>d1) ++igrea; else
            ++iequi;
//        else 
//            message(d1,d2);
    }
    display << iless << iequi << igrea << NL;
}

void lin_test7()
{
    SMatrix<double, 7, 7> mat;
    double * const * a = mat;
    a[0][0] = 360360; a[0][1] = 180180; a[0][2] = 120120; a[0][3] = 90090; a[0][4] = 72072; a[0][5] = 60060; a[0][6] = 51480;
    a[1][0] = 180180; a[1][1] = 120120; a[1][2] =  90090; a[1][3] = 72072; a[1][4] = 60060; a[1][5] = 51480; a[1][6] = 45045;
    a[2][0] = 120120; a[2][1] =  90090; a[2][2] =  72072; a[2][3] = 60060; a[2][4] = 51480; a[2][5] = 45045; a[2][6] = 40040;
    a[3][0] =  90090; a[3][1] =  72072; a[3][2] =  60060; a[3][3] = 51480; a[3][4] = 45045; a[3][5] = 40040; a[3][6] = 36036;
    a[4][0] =  72072; a[4][1] =  60060; a[4][2] =  51480; a[4][3] = 45045; a[4][4] = 40040; a[4][5] = 36036; a[4][6] = 32760;
    a[5][0] =  60060; a[5][1] =  51480; a[5][2] =  45045; a[5][3] = 40040; a[5][4] = 36036; a[5][5] = 32760; a[5][6] = 30030;
    a[6][0] =  51480; a[6][1] =  45045; a[6][2] =  40040; a[6][3] = 36036; a[6][4] = 32760; a[6][5] = 30030; a[6][6] = 27720;
    SM_Chol sl1 ( 7, a );
    SM_LDLt sl2 ( 7, a );
//    message(sl1.determinant(),sl.determinant());
    double b[7], x1[7], x2[7];
    int i;
    for ( i=0; i<7; ++i) b[i] = 0.;
    b[4] = 360360.;
    sl1.solve ( b, x1 );
    sl2.solve ( b, x2 );
    for ( i=0; i<7; ++i) display << x1[i] << x2[i] << NL;
}

void appr_par ()
{
// ( 1 - t*t ) ** 2 median = 0.281
// ( 1 - t*t ) ** 4 median = 0.216
    const int n = 350;
    for ( int i = 0; i < n; ++i )
    {
        double t = 0.01 * i;
        double t1 = t * 0.281;
        double t2 = t * 0.216; 
        double t3 = t * 0.674; 
/*        double x1 = t;
        double x3 = t * t * x1;
        double x5 = t * t * x3;
        double x7 = t * t * x5;
        double x9 = t * t * x7;
//message ( t, 315*x1 - 420*x3 + 378*x5 - 180*x7 + 35*x9 );*/
        t2 = 1 - t2*t2;
        t2 *= t2;
        t2 *= t2;
        display << t << (1 - t1*t1) * (1 - t1*t1) << t2 << exp(-0.5*t3*t3) << NL;
    }
//message ( 1. - 4./3. + 6./5. - 4./7. +1./9. );
}

class TestFunc : public MathFunc1
{
    const double * a;
public:
    TestFunc ( const double * p ) : a(p) {}
    double operator () ( double x ) const
    {
        return ( x - a[0] ) * ( x - a[1] ) * ( x - a[2] );
    }
};

void zeroin_test ()
{
    double ax = 0.;
    double bx = 1.;
    double tol = 0;
    double res;
    static PRand rand;
    double max = 0.;
    double max2 = 0.;
    double time1 = 0.;
    double time2 = 0.;
    for ( int k = 0; k < 1000; ++k )
    {
        double a[3];
        a[0] = rand();
        a[1] = rand() - 1;
        a[2] = rand() - 2;
        TestFunc func ( a );
double t0 = timeInSec();
        if ( zeroin ( ax, bx, func, tol, res ) )
        {
double t1 = timeInSec();
time1 += t1 - t0;
//            message ( a, res, a - res );
            double t = fabs ( a[0] - res );
            if ( max < t ) max = t;
            double p1 = - ( a[0] + a[1] + a[2] );
            double p2 =   a[0] * a[1] + a[2] * ( a[0] + a[1] );
            double p3 = - a[0] * a[1] * a[2];
            double x[3];
double t2 = timeInSec();
            int n = root3 ( p1, p2, p3, x );
double t3 = timeInSec();
time2 += t3 - t2;
            double mx = x[0];
            if ( n > 1 && mx < x[1] ) mx = x[1];
            if ( n > 2 && mx < x[2] ) mx = x[2];
            t = fabs ( a[0] - mx );
            if ( max2 < t ) max2 = t;
        }
        else
        {
            display << "err" << NL;
        }
    }
    display << max << max2 << NL;
    display << time1 << time2 << NL;
}

void hqr_test1 ()
{
    SMatrix<double, 8, 8> a;
    a.fill ( 0. );
    a[1][0] = a[2][1] = a[3][2] = a[4][3] = a[5][4] = a[6][5] = a[7][6] = a[0][7] = 1.;
    double wr[8], wi[8];
    if ( ! hqr ( 8, a, wr, wi ) )
    {
//        message("err");
    }
    else
    {
//        for ( int i = 0; i < 8; ++i ) message ( wr[i], wi[i] );
    }
}

void hqr_test2 ()
{
    SMatrix<double, 8, 8> a;
    a[0][0] = 3; a[0][1] = 2; a[0][2] = 1; a[0][3] = 2;    a[0][4] = 1;    a[0][5] = 4; a[0][6] = 1; a[0][7] = 2;
    a[1][0] = 2; a[1][1] = 1; a[1][2] = 3; a[1][3] = 1;    a[1][4] = 2;    a[1][5] = 2; a[1][6] = 1; a[1][7] = 4;
    a[2][0] = 0; a[2][1] = 3; a[2][2] = 1; a[2][3] = 2;    a[2][4] = 1;    a[2][5] = 2; a[2][6] = 1; a[2][7] = 3;
    a[3][0] = 0; a[3][1] = 0; a[3][2] = 1; a[3][3] = 1;    a[3][4] = 2;    a[3][5] = 1; a[3][6] = 3; a[3][7] = 1;
    a[4][0] = 0; a[4][1] = 0; a[4][2] = 0; a[4][3] = 1e-7; a[4][4] = 3;    a[4][5] = 1; a[4][6] = 4; a[4][7] = 2;
    a[5][0] = 0; a[5][1] = 0; a[5][2] = 0; a[5][3] = 0;    a[5][4] = 1e-6; a[5][5] = 2; a[5][6] = 1; a[5][7] = 4;
    a[6][0] = 0; a[6][1] = 0; a[6][2] = 0; a[6][3] = 0;    a[6][4] = 0;    a[6][5] = 1; a[6][6] = 2; a[6][7] = 3;
    a[7][0] = 0; a[7][1] = 0; a[7][2] = 0; a[7][3] = 0;    a[7][4] = 0;    a[7][5] = 0; a[7][6] = 3; a[7][7] = 2;
    double wr[8], wi[8];
    if ( ! hqr ( 8, a, wr, wi ) )
    {
//        message("err");
    }
    else
    {
//        for ( int i = 0; i < 8; ++i ) message ( wr[i], wi[i] );
    }
}

void hqr_test3 ()
{
    SMatrix<double, 4, 4> a;
    a[0][0] = 1.0;  a[0][1] = 2.0;  a[0][2] = 3.0;  a[0][3] = 4.;
    a[1][0] = 1e-3; a[1][1] = 3e-3; a[1][2] = 2e-3; a[1][3] = 1e-3;
    a[2][0] = 0.0;  a[2][1] = 1e-6; a[2][2] = 3e-6; a[2][3] = 2e-6;
    a[3][0] = 0.0;  a[3][1] = 0.0;  a[3][2] = 1e-9; a[3][3] = 2e-9;
    double wr[4], wi[4];
    if ( ! hqr ( 4, a, wr, wi ) )
    {
//        message("err");
    }
    else
    {
//        for ( int i = 0; i < 4; ++i ) message ( wr[i], wi[i] );
    }
}

void hqr_test4 ()
{
    SMatrix<double, 13, 13> a;
    for ( int i = 0; i < 13; ++i )
    {
        for ( int j = 0; j < 13; ++j )
        {
            if ( i <= j )
            {
                a[i][j] = 13 - j;
            }
            else
            {
                a[i][j] = i == j + 1 ? 12 - j : 0;
            }
        }
    }
    double wr[13], wi[13];
    if ( ! hqr ( 13, a, wr, wi ) )
    {
//        message("err");
    }
    else
    {
//        for ( int i = 0; i < 13; ++i ) message ( wr[i], wi[i] );
    }
}

void jacobi_test()
{
    const nat n = 30;
    SMatrix<double, n, n> a, v;
    for ( nat i = 1; i <= n; ++i )
    {
        for ( nat j = 1; j <= n; ++j )
        {
            a[i-1][j-1] = i > j ? i : j;
        }
    }
    FixArray<double, n> d;
    jacobi ( n, a, &d[0], v );
    insertSort321 ( d );
/*    message(d[0]);
    message(d[1]);
    message(d[2]);
    message(d[15]);
    message(d[28]);
    message(d[29]);*/
}

void orthes_test ()
{
    SMatrix<double, 4, 4> a;
    a[0][0] = 1.; a[0][1] = 2.; a[0][2] = 3.; a[0][3] = 5.;
    a[1][0] = 2.; a[1][1] = 4.; a[1][2] = 1.; a[1][3] = 6.;
    a[2][0] = 1.; a[2][1] = 2.; a[2][2] =-1.; a[2][3] = 3.;
    a[3][0] = 2.; a[3][1] = 0.; a[3][2] = 1.; a[3][3] = 3.;
    orthes ( 4, a );
    //message ( a );
}

void ortho_test ()
{
    const nat nc = 13;
    const nat nr = nc - 5;
    double c[nr], v[nc];
    SMatrix<double, nc, nc> mat;
    double * const * a = mat;
    static PNormalRand rand;
    nat i, j, k;
    for ( i = 0; i < nr; ++i )
    {
        for ( j = 0; j < mat.nCol; ++j )
        {
            a[i][j] = rand();
        }
    }
//    a[0][0] = 0; a[0][1] = 0; a[0][2] = 2;
//    a[1][0] = 1; a[1][1] = 0; a[1][2] = 0;
//    for ( j = 0; j < mat.nCol; ++j ) a[nr-1][j] = a[nr-3][j];
    const HMatrix<double> mat2 ( mat );
    nat rank = orthogonalizationH1 ( nr, mat );
    if ( rank != nr )
    {
        display << "rank =" << rank << "nr =" << nr << NL;
    }
    const nat m = mat.nRow < mat.nCol ? mat.nRow : mat.nCol;
    double max = 0.;
    for ( k = 0; k < m-1; ++k )
    {
        const double * rk = mat[k];
        for ( i = k+1; i < m; ++i )
        {
            const double * ri = mat[i];
            double t = 0.;
            for ( j = 0; j < mat.nCol; ++j )
            {
                t += rk[j] * ri[j];
            }
            t = fabs ( t );
            if ( max < t ) max = t;
        }
    }
    double max2 = 0.;
    for ( k = 0; k < nr; ++k )
    {
        const double * rk = mat2[k];
        for ( i = 0; i < nr; ++i )
        {
            const double * ri = mat[i];
            c[i] = 0.;
            for ( j = 0; j < mat.nCol; ++j )
            {
                c[i] += rk[j] * ri[j];
            }
        }
        for ( j = 0; j < mat.nCol; ++j ) v[j] = 0.;
        for ( i = 0; i < nr; ++i )
        {
            const double * ri = mat[i];
            for ( j = 0; j < mat.nCol; ++j )
            {
                v[j] += c[i] * ri[j];
            }
        }
        double sum = 0.;
        for ( j = 0; j < mat.nCol; ++j )
        {
            v[j] -= rk[j];
            sum += fabs ( v[j] );
        }
        if ( max2 < sum ) max2 = sum;
    }
//display << a[2][0] << a[2][1] << a[2][2] << NL;
    display << "max =" << max << "  max2 =" << max2 << NL;
}

void slu_ortho_test ()
{
    static PRand rand;
    static PNormalRand norm;
    const nat n = 11;
    SMatrix<double, n, n> x;
    double b[n];
    double gmax = 0.;
    for ( nat kk = 0; kk < 100; ++kk )
    {
        const nat m = 1 + rand.number ( n );
        HMatrix<double> mat ( m, n );
        double * const * a = mat;
        nat i, j, l;
        for ( i = 0; i < m; ++i )
        {
            for ( j = 0; j < n; ++j )
            {
                a[i][j] = norm() / ( i + j + 1 );
            }
            b[i] = norm();
        }
        const nat k = slu_orthoH1 ( mat, b, x );
        if ( k > 0 )
        {
            double max = 0.;
            for ( l = 0; l < k; ++l )
            {
                const double * r = x[l];
                for ( i = 0; i < m; ++i )
                {
                    double d = b[i];
                    for ( j = 0; j < n; ++j )
                    {
                        d -= a[i][j] * r[j];
                    }
                    d = fabs ( d );
                    if ( max < d ) max = d;
                }
            }
            display << n << m << k << max << NL;
            if ( gmax < max ) gmax = max;
        }
        else
        {
            display << n << m << k << NL;
        }
    }
    //message(gmax);
}

void columnScale_test ()
{
    static PNormalRand rand;
    nat iless = 0, iequi = 0, igrea = 0;
    double max = 0.;
    const nat n = 11;
    SMatrix<double, n, n> a;
    for ( nat k = 0; k < 100; ++k )
    {
        FixArray<double, n> x, y, b;
        nat i, j;
        for ( i = 0; i < n; ++i )
        {
            for ( j = 0; j < n; ++j )
            {
                a[i][j] = rand() / ( i + j + 1 );
//                a[i][j] = 1. / ( i + j + 1 );
            }
            x[i] = i * rand();
        }
//if(k!=205) continue;
        for ( i = 0; i < n; ++i )
        {
            double d = 0.;
            for ( j = 0; j < n; ++j ) d += a[i][j] * x[j];
            b[i] = d;
        }
        double d1 = 0.;
//        if ( ortholin ( HMatrix<double>(a), b, y ) )
//        if ( slu_orthoH1 ( n, n, a, &b[0], &y[0] ) )
        if ( lss_h ( HMatrix<double>(a), b, y ) )
        {
            for ( j = 0; j < n; ++j )
            {
                double t = y[j] - x[j];
                d1 += t * t;
            }
        }
        else d1 = 1;
        double d2 = 0.;
//        if ( columnScale ( ortholin, a, b, y ) )
        if ( columnScale ( lss_h, a, b, y ) )
        {
            for ( j = 0; j < n; ++j )
            {
                double t = y[j] - x[j];
                d2 += t * t;
            }
        }
        else d2 = 1;
//message(sl.condition());
        if(d2>d1) ++iless; else
        if(d2<d1) ++igrea; else
            ++iequi;
//        else 
//            message(d1,d2);
    }
    display << iless << iequi << igrea << NL;
}

double ellipseCircumference1 ( double a, double b )
{
    if ( a < b ) _swap ( a, b );
    const double e = sqrt ( 1 - _pow2 ( b / a ) );
    double s = 1., c = 1.;
    for ( nat m = 1; m < 1000000; ++m )
    {
        double m2 = m + m;
        double m1 = m2 - 1;
        c *= _pow2 ( e * m1 / m2 );
        s -= c / m1;
        if ( c < 1e-12 )
            break;
    }
    return M_PI * a * ( s + s );
}

double ellipseCircumference2 ( double a, double b )
{
    double c = ( a - b ) / ( a + b );
    c *= 3 * c;
    return M_PI * ( a + b ) * ( 1 + c / ( 10 + sqrt ( 4 - c ) ) );
}

double ellipseCircumference3 ( double a, double b )
{
//    const double c = _pow2 ( M_PI / 2 ) - 2; // 0.4674
    const double k1 = 0.968;
    const double k2 = 0.525;
    return 4 * sqrt ( k1 * a * a + k2 * a * b + k1 * b * b );
}

class BestCoef : public MathFunc1
{
    CArrRef<double> arr;
public:
    BestCoef ( CArrRef<double> a ) : arr(a) {}
    double operator () ( double x ) const
    {
        const double k1 = 0.9875;
        const double k2 = x;
        double max = 0.;
        nat i;
        for ( i = 0; i < arr.size(); ++i )
        {
            const double b = double ( i ) / ( arr.size() - 1 );
            const double d = fabs ( arr[i] - 4 * sqrt ( k1 * ( 1 + b * b ) + k2 * b ) );
            if ( max < d ) max = d;
        }
        return max;
    }
};

void ellipseCircumference_test ()
{
    SMatrix<double, 101, 2> a;
    FixArray<double, 101> b;
    FixArray<double, 2> x;
    nat i;
    for ( i = 0; i < 101; ++i )
    {
        const double u = double ( i ) * 0.01;
        const double t = ellipseCircumference1 ( 1, u ) / 4;
        b[i] = t * t;
        a[i][0] = 1 + u * u;
        a[i][1] = u;
display << 1000 * ellipseCircumference3 ( 1, u ) << NL;
    }
//    const double t = goldenRatioMin ( 0.8, 1.2, BestCoef ( per1 ), 1e-5 );
    lss_h ( a, b, x );
//display << 1000 * x[0] << 1000 * x[1] << NL;
}

Display & operator << ( Display & disp, CArrRef<double> arr )
{
    for ( nat i = 0; i < arr.size(); ++i ) disp << arr[i];
    return disp;
}

double norm1 ( CArrRef2<double> data, double * x )
{
    double res = 0;
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    if ( nCol == 0 ) return res;
    const nat nCol1 = nCol - 1;
    for ( nat i = 0; i < nRow; ++i )
    {
        CArrRef<double> r = data[i];
        double sum = 0;
        for ( nat j = 0; j < nCol1; ++j ) sum += r[j] * x[j];
        sum -= r[nCol1];
        res += fabs ( sum );
    }
    return res;
}

double norm2 ( CArrRef2<double> data, double * x )
{
    double res = 0;
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    if ( nCol == 0 ) return res;
    const nat nCol1 = nCol - 1;
    for ( nat i = 0; i < nRow; ++i )
    {
        CArrRef<double> r = data[i];
        double sum = 0;
        for ( nat j = 0; j < nCol1; ++j ) sum += r[j] * x[j];
        sum -= r[nCol1];
        res += sum * sum;
    }
    return sqrt ( res );
}

double normU ( CArrRef2<double> data, double * x )
{
    double res = 0;
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    if ( nCol == 0 ) return res;
    const nat nCol1 = nCol - 1;
    for ( nat i = 0; i < nRow; ++i )
    {
        CArrRef<double> r = data[i];
        double sum = 0;
        for ( nat j = 0; j < nCol1; ++j ) sum += r[j] * x[j];
        sum -= r[nCol1];
        sum = fabs ( sum );
        if ( res < sum ) res = sum;
    }
    return res;
}

bool minNormU2 ( CArrRef2<double> & matr, ArrRef<double> & x )
{
    const nat nRow = matr.size0();
    const nat nCol = matr.size1();
    if ( nRow + 1 < nCol ) return false;
    if ( nRow + 1 == nCol )
    {
        DynArray2<double> data ( nRow, nCol );
        data = matr;
        return slu_gauss ( data, x );
    }
    const nat nCol1 = nCol - 1;
    // Поиск независимых нормалей
    DynArray2<double> data ( nCol1, nRow );
    DynArray<double> w ( nRow );
    DynArray<nat> index ( nRow );
    nat i, j;
    for ( i = 0; i < nRow; ++i )
    {
        for ( j = 0; j < nCol1; ++j ) data[j][i] = matr[i][j];
    }
    if ( ! sluGaussRow ( data, nCol1, nRow, index(), nCol1, nCol1 ) ) return false;
    // Поиск первоначального оптимума
    double max = 0, summ = 0;
    nat jm = nCol1;
    for ( j = nCol1; j < nRow; ++j )
    {
        w.fill ( 0 );
        double norm = 1;
        const nat nj = index[j];
        for ( i = 0; i < nCol1; ++i )
        {
            norm += fabs ( w[index[i]] = data[i][nj] );
        }
        w[nj] = -1;
        double sum = 0;
        for ( nat k = 0; k < nRow; ++k ) sum += data[k][nCol1] * w[k];
        const double t = fabs ( sum ) / norm;
        if ( max < t ) max = t, jm = j, summ = sum;
    }
    if ( max < 1e-14 )
    {
        DynArray2<double> temp ( nCol1, nCol );
        for ( i = 0; i < nCol1; ++i ) temp[i] = matr[index[i]];
        return slu_gauss ( temp, x );
    }
    DynArray2<double> temp ( nCol, nCol );
    const nat nj = index[jm];
    for ( i = 0; i < nCol1; ++i )
    {
        const nat ni = index[i];
        if ( summ * data[i][nj] < 0 )
        {
            ArrRef<double> t = temp[i];
            CArrRef<double> m = matr[ni];
            for ( j = 0; j < nCol; ++j ) t[j] = -m[j];
        }
        else
        {
            temp[i] = matr[ni];
        }
    }
    {
        if ( summ > 0 )
        {
            ArrRef<double> t = temp[nCol1];
            CArrRef<double> m = matr[nj];
            for ( j = 0; j < nCol; ++j ) t[j] = -m[j];
        }
        else
        {
            temp[nCol1] = matr[nj];
        }
    }
    DynArray2<double> slu ( nCol1, nCol );
    CArrRef<double> t1 = temp[nCol1];
    for ( i = 0; i < nCol1; ++i )
    {
        ArrRef<double> s = slu[i];
        CArrRef<double> t = temp[i];
        for ( j = 0; j < nCol; ++j ) s[j] = t[j] - t1[j];
    }
    bool res = slu_gauss ( data, x );
    if ( ! res || nRow == nCol )
        return res;
    double dist = t1[nCol1];
    for ( i = 0; i < nCol1; ++i ) dist += t1[i] * x[i];
    if ( dist <= 0 )
        return res;
    // Делаем конус допустимых решений
    DynArray2<double> mat ( nCol, 2*nCol );
    for ( i = 0; i < nCol; ++i )
    {
        CArrRef<double> t = temp[i];
        ArrRef<double> p = mat[i];
        p[0] = -1;
        for ( j = 1; j < nCol; ++j ) p[j] = t[j-1];
        for ( j = 0; j < nCol; ++j ) p[j+nCol] = i == j ? 1 : 0;
    }
    index.resize ( 2*nCol );
    sluGaussRow ( mat, nRow, nCol, index(), nRow, nRow );
    DynArray2<double> bevel ( nCol1, nCol );
/*    Double<5> arr[6];
    arr[0].init ( dist, res.x1, res.x2, res.x3, res.x4 );
    for ( i = 0; i < nRow; ++i )
    {
        double * d = & arr[i+1].d0;
        for ( j = 0; j < nRow; ++j ) d[index[j]] = mat[j][nRow+i];
    }
    // Нормализация векторов
    for ( i = 1; i < 6; ++i )
    {
        Double<5> & d = arr[i];
        const double c = temp[i-1].norm * Vector4d ( d.d1, d.d2, d.d3, d.d4 ) > 0 ? -1 : 1;
        d *= c / sqrt ( d * d );
    }
    // Поиск оптимума
    for ( nat k = 0; k < nn; ++k )
    {
        nat i, im = 0;
        double max = fabs ( plane[0] % res );
        for ( i = 1; i < nn; ++i )
        {
            const double t = fabs ( plane[i] % res );
            if ( max < t ) max = t, im = i;
        }
        if ( max <= ( 1 + 1e-12 ) * dist )
            return res;
        dist = max - dist;
        Plane4d pm = plane[im];
        if ( pm % res > 0 ) pm = - pm;
        Double<5> cor;
        cor.init ( 1, pm.norm.x1, pm.norm.x2, pm.norm.x3, pm.norm.x4 );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i < 6; ++i )
        {
            const Double<5> & v = arr[i];
            double t = cor * v;
            if ( t < 1e-12 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.d0 * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double u = v.d0 * t;
                if ( u < max ) max = u, ib = i, sm = t;
            }
        }
        if ( ib == 0 ) 
            break;
        const Double<5> & v = arr[ib];
        Double<5> & a0 = arr[0];
        a0 += v * ( dist * sm );
        res.x1 = a0.d1;
        res.x2 = a0.d2;
        res.x3 = a0.d3;
        res.x4 = a0.d4;
        dist = a0.d0;
        for ( i = 1; i < 6; ++i )
        {
            if ( i == ib ) continue;
            Double<5> & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }*/
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
// Поиск главных плоскостей
    DynArray<double> cor ( nCol );
    for ( nat k = 0; k < nRow + nCol; ++k )
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

void over_lin_sys1()
{
    static PRand rand;
    DynArray2<double> data ( 9, 4 );
    DynArray<nat> index ( data.size1() );
    DynArray<double> x ( data.size1()-1 );
    nat i, j;
    for ( i = 0; i < data.size0(); ++i )
    {
        for ( j = 0; j < data.size1(); ++j ) data[i][j] = rand() - 0.5;
    }
    if ( ! minNormU ( data, x, index ) )
        display << "err" << NL;
    else
    {
        for ( i = 0; i < index.size(); ++i )
        {
            CArrRef<double> r = data[index[i]];
            double sum = 0;
            for ( j = 0; j < x.size(); ++j ) sum += x[j] * r[j];
            display << fabs ( sum -= r[x.size()] ) << NL;
        }
    }
    display << "end" << NL;
}

void over_lin_sys2()
{
    static PNormalRand rand;
    DynArray2<double> data ( 9, 6 );
    DynArray<double> x1 ( data.size1()-1 ), x2 ( data.size1()-1 ), xU ( data.size1()-1 );
    nat i, j, k;
    for ( k = 0; k < 1000; ++k )
    {
        for ( i = 0; i < data.size0(); ++i )
        {
            for ( j = 0; j < data.size1(); ++j ) data[i][j] = rand();
        }
        if ( ! minNorm1 ( data, x1 ) )
        {
            minNorm1 ( data, x1 );
            display << "err 1" << NL;
            continue;
        }
        if ( ! minNorm2 ( data, x2 ) )
        {
            minNorm2 ( data, x2 );
            display << "err 2" << NL;
            continue;
        }
        if ( ! minNormU ( data, xU ) )
        {
            minNormU ( data, xU );
            display << "err U" << NL;
            continue;
        }
        const double n11 = norm1 ( data, &x1[0] );
        const double n12 = norm1 ( data, &x2[0] );
        const double n1U = norm1 ( data, &xU[0] );
        if ( n11 > n12 )
            display << k << "n11 > n12" << n11 << n12 << NL;
        if ( n11 > n1U )
            display << k << "n11 > n1U" << n11 << n1U << NL;
        const double n21 = norm2 ( data, &x1[0] );
        const double n22 = norm2 ( data, &x2[0] );
        const double n2U = norm2 ( data, &xU[0] );
        if ( n22 > n21 )
            display << k << "n22 > n21" << n22 << n21 << NL;
        if ( n22 > n2U )
            display << k << "n22 > n2U" << n22 << n2U << NL;
        const double nU1 = normU ( data, &x1[0] );
        const double nU2 = normU ( data, &x2[0] );
        const double nUU = normU ( data, &xU[0] );
        if ( nUU > nU1 )
            display << k << "nUU > nU1" << nUU << nU1 << NL;
        if ( nUU > nU2 )
            display << k << "nUU > nU2" << nUU << nU2 << NL;
    }
    display << "end of over_lin_sys2" << NL;
}

void minNorm_test2()
{
    static PRand rand;
    const nat nn = 20;
    FixArray<Line2d, nn> line;
    for ( nat n = 3; n <= nn; n += 1 )
    {
        DynArray2<double> data ( n, 3 );
        for ( nat j = 0; j < 500; ++j )
        {
            for ( nat i = 0; i < n; ++i )
            {
                double t = rand();
                line[i].dist = 0.25 * t * t;
                double a = 2. * 3.14159265358979323846 * rand();
                Vector2d & v = line[i].norm;
                v.x = cos(a);
                v.y = sin(a);
                ArrRef<double> r = data[i];
                r[0] = v.x;
                r[1] = v.y;
                r[2] = - line[i].dist;
            }
            CArrRef<Line2d> ref ( line, 0, n );
            FixArray<double, 2> x;
            Def<Vector2d> p = getNearPoint1 ( ref );
            if ( ! p.isDef )
            {
                display << "err getNearPoint1" << n << j << NL;
                continue;
            }
            if ( ! minNorm1 ( data, x ) )
            {
                display << "err minNorm1" << NL;
                continue;
            }
            if ( fabs ( p.x - x[0] ) > 1e-12 || fabs ( p.y - x[1] ) > 1e-12 )
                display << "norm1" << fabs ( p.x - x[0] ) << fabs ( p.y - x[1] ) << NL;
            p = getNearPointU ( ref );
            if ( ! p.isDef )
            {
                display << "err getNearPointU" << n << j << NL;
                continue;
            }
            if ( ! minNormU ( data, x ) )
            {
                display << "err minNormU" << NL;
                continue;
            }
            if ( fabs ( p.x - x[0] ) > 1e-12 || fabs ( p.y - x[1] ) > 1e-12 )
                display << "normU" << fabs ( p.x - x[0] ) << fabs ( p.y - x[1] ) << NL;
        }
    }
    //message ( "end" );
}

nat gg = 0;

bool maxNormUb ( ArrRef2<double> & data, ArrRef<nat> & res )
{
    nat i, j, k;
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    const nat nRow1 = nRow - 1;
    DynArray<nat> index ( nCol );
    DynArray<double> w ( nCol );
    ArrRef<double> func = data[nRow1];
#if 1
    if ( ! _sluGaussRow ( data, nRow, nCol, index(), nRow1, nCol ) ) return false;
#else
// Прямой ход
    for ( i = 0; i < nCol; ++i ) index[i] = i;
    for ( k = 0; k < nRow1; ++k )
    {
        double * rk = & data[k][0];
        double sum = 0;
        for ( j = k; j < nCol; ++j )
        {
            const nat ij = index[j];
            w[ij] = func[ij] < 0 ? -1 : 1;
            sum -= rk[ij] * w[ij];
        }
        const double ds = sum < 0 ? -1 : 1;
// Поиск максимального по модулю члена в k-ой строке
        nat jm = nCol;
        double max = -1e300;
        for ( j = k; j < nCol; ++j )
        {
            const nat ij = index[j];
            double t = rk[ij];
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
        _swap ( index[jm], index[k] );
// Нормализация строки
        const double p = 1. / rk[ik];
        for ( i = k+1; i < nCol; ++i ) rk[index[i]] *= p;
        rk[ik] = 1;
// Вычитание строк
        for ( j = k+1; j < nRow; ++j )
        {
            double * rj = data[j]();
            const double t = rj[ik];
            for ( i = k+1; i < nCol; ++i )
            {
                const nat ii = index[i];
                if ( fabs ( rj[ii] -= rk[ii] * t ) < 1e-290 ) rj[ii] = 0;
            }
            rj[ik] = 0;
        }
    }
// Обратная подстановка
    for ( j = nRow1; --j > 0; )
    {
        const double * rj = & data[j][0];
        const nat ij = index[j];
        for ( i = 0; i < j; ++i )
        {
            double * ri = & data[i][0];
            const double t = ri[ij];
            for ( k = nRow1; k < nCol; ++k )
            {
                const nat ik = index[k];
                ri[ik] -= rj[ik] * t;
            }
            ri[ij] = 0;
        }
    }
#endif
////////////////////////////////////////////////////////////////////////

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
            gg += 1+k;
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

void test ( ArrRef2<double> & data, double * w, nat * index )
{
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    const nat nRow1 = nRow - 1;
    for ( nat i = 0; i < nRow1; ++i )
    {
        for ( nat j = 0; j < nRow1; ++j )
        {
           double t = data[i][index[j]];
           if ( i == j )
           {
               if ( fabs ( t ) != 1 )
                   t = t;
           }
           else
           {
               if ( t )
                   t = t;
           }
        }
    }
    for ( nat j = 0; j < nRow1; ++j )
    {
        double sum = 0;
        const double * rj = data[j]();
        for ( nat i = 0; i < nCol; ++i )
        {
            sum += rj[i] * w[i];
        }
        if ( fabs ( sum ) > 1e-12 )
            sum = sum;
    }
}

bool maxNormUc ( ArrRef2<double> & data, double * w, nat * index )
{
    nat i, j;
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    const nat nRow1 = nRow - 1;
    CCArrRef<double> & func = data[nRow1];
    if ( ! _sluGaussRow ( data, nRow, nCol, index, nRow1, nCol ) ) return false;
// Заполнение весов
    for ( j = nRow1; j < nCol; ++j )
    {
        const nat ij = index[j];
        const double f = func[ij];
        if ( f > 0 )
            w[ij] = 1;
        else
        if ( f < 0 )
            w[ij] = -1;
        else
            w[ij] = 0;
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
        w[index[i]] = t;
        const double a = fabs ( t );
        if ( max < a ) max = a, sum = t, im = i;
    }
    if ( max < 1 + 1e-9 )
        return true;
//
    {
        // Находим допустимое решение
        for ( j = 0; j < nCol; ++j ) w[j] /= max;
//        w[index[im]] = sum < 0 ? -1 : 1;
        // Меняем свободную переменную
        double * ri = data[im]();
        nat jm = nRow1;
        max = fabs ( ri[index[jm]] );
        for ( j = nRow; j < nCol; ++j )
        {
            const double a = fabs ( ri[index[j]] );
            if ( max < a ) max = a, jm = j;
        }
        // Нормализация строки
        const nat iim = index[im];
        const nat ijm = index[jm];
        const double p = 1. / ri[ijm];
        for ( j = nRow1; j < nCol; ++j ) ri[index[j]] *= p;
        ri[iim] *= p;
        ri[ijm] = 1;
        // Вычитание строки
        for ( j = 0; j < nRow; ++j )
        {
            if ( j == im ) continue;
            double * rj = data[j]();
            const double t = rj[ijm];
            for ( i = nRow1; i < nCol; ++i )
            {
                const nat ii = index[i];
                if ( fabs ( rj[ii] -= ri[ii] * t ) < 1e-290 ) rj[ii] = 0;
            }
            if ( fabs ( rj[iim] -= ri[iim] * t ) < 1e-290 ) rj[iim] = 0;
            rj[ijm] = 0;
        }
        _swap ( index[jm], index[im] );
//test ( data, w, index );
    }
//
    for ( nat k = 0; k < nCol; ++k )
    {
        bool noChange = true;
        for ( nat jj = nRow1; jj < nCol; ++jj )
        {
            const nat ij = index[jj];
            const double f = func[ij];
            if ( ! f ) continue;
            const double v = w[ij];
            nat im = jj;
            double vn = 1;
            if ( f < 0 )
            {
                if ( v <= -1 ) continue;
                vn = -1;
            }
            else
            {
                if ( v >= 1 ) continue;
            }
            for ( i = 0; i < nRow1; ++i )
            {
                const double wi = w[index[i]];
                const double t = wi + ( v - vn ) * data[i][ij];
                if (fabs(t) < 1 + 1e-9) continue;
                vn = v - ((t > 0 ? 1. : -1.) - wi) / data[i][ij];
                im = i;
            }
            for ( i = 0; i < nRow1; ++i ) w[index[i]] += (v - vn) * data[i][ij];
            w[ij] = vn;
//test(data, w, index);
            if (im == jj)
                continue;
            noChange = false;
            // Нормализация строки
            double * ri = data[im]();
            const nat iim = index[im];
            const nat ijm = index[jj];
            const double p = 1. / ri[ijm];
            for ( j = nRow1; j < nCol; ++j ) ri[index[j]] *= p;
            ri[iim] *= p;
            ri[ijm] = 1;
            // Вычитание строк
            for ( j = 0; j < nRow; ++j )
            {
                if ( j == im ) continue;
                double * rj = data[j]();
                const double t = rj[ijm];
                for ( i = nRow1; i < nCol; ++i )
                {
                    const nat ii = index[i];
                    if ( fabs ( rj[ii] -= ri[ii] * t ) < 1e-290 ) rj[ii] = 0;
                }
                if ( fabs ( rj[iim] -= ri[iim] * t ) < 1e-290 ) rj[iim] = 0;
                rj[ijm] = 0;
            }
            _swap ( index[jj], index[im] );
//test ( data, w, index );
        }
        if ( noChange ) return true;
    }
    return false;
}

bool minNorm1b ( CArrRef2<double> & data, ArrRef<double> & x, ArrRef<nat> & index )
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
    DynArray<double> w ( nRow );
    DynArray<nat> index2 ( nRow );
    if ( ! maxNormUc ( ref, w(), index2() ) ) return false;
    {
        for ( nat j = 0; j < nCol1; ++j )
        {
            double sum = 0;
            for ( nat i = 0; i < nRow; ++i )
            {
                sum += data[i][j] * w[i];
            }
            if ( fabs ( sum ) > 1e-15 )
                sum = sum;
        }
    }
    if ( x.size() == 0 ) return true;
// Решаем систему линейных уравнений методом Гаусса
    ArrRef2<double> ref2 ( temp, 0, nCol1, nCol );
    for ( nat i = 0; i < nCol1; ++i )
    {
        ref2[i] = data[index[i]=index2[i]];
    }
    return slu_gauss ( ref2, x );
}

double calcSum ( CArrRef2<double> & data, ArrRef<double> & x )
{
    double sum = 0;
    const nat nRow = data.size0();
    const nat nCol = data.size1();
    const nat nCol1 = nCol - 1;
    for ( nat i = 0; i < nRow; ++i )
    {
        const double * p = data[i]();
        double t = p[nCol1];
        for ( nat j = 0; j < nCol1; ++j )
        {
            t += p[j] * x[j];
        }
        sum += fabs(t);
    }
    return sum;
}

void minNorm_test3()
{
    static PRand rand;
    static PRandVector3d vrand;
    const nat nn = 20;
    const double eps = 1e-13;
    double time1 = 0, time2 = 0;
    for ( nat n = 6; n <= nn; n += 1 )
    {
        DynArray2<double> data ( n, 5 );
        for ( nat j = 0; j < 500; ++j )
        {
            for ( nat i = 0; i < n; ++i )
            {
                ArrRef<double> r = data[i];
                r[0] = 2 * rand() - 1;
                r[1] = 2 * rand() - 1;
                r[2] = 2 * rand() - 1;
                r[3] = 2 * rand() - 1;
                r[4] = 2 * rand() - 1;
            }
            FixArray<double, 4> x;
            double t1 = timeInSec();
            if ( ! minNorm1 ( data, x ) )
            {
                minNorm1 ( data, x );
                display << "err minNorm1" << NL;
                continue;
            }
            double t2 = timeInSec();
            time1 += t2 - t1;
            FixArray<nat, 4> index;
            double s1 = calcSum ( data, x );
            t1 = timeInSec();
            if ( minNorm1b ( data, x, index ) )
            {
                t2 = timeInSec();
                time2 += t2 - t1;
                {
                    double s2 = calcSum ( data, x );
                    if ( s1 != s2 )
                    {
                        display << n << j << s1 << s2 << NL;
                    //display << fabs ( p.x - x[0] ) << fabs ( p.y - x[1] ) << fabs ( p.z - x[2] ) << NL;
                        minNorm1b ( data, x, index );
                    }
                }
            }
            else
            {
                display << n << j << NL;
                minNorm1b ( data, x, index );
            }
        }
    }
    display << "time1" << time1 << NL;
    display << "time2" << time2 << NL;
    display << "end" << gg << NL;
}

void slu_gauss_test()
{
    const nat nc = 3;
    const nat nr = 2;
    const nat nn = 5;
    FixArray2<double, nr, nn> a, b;
    FixArray<double, nc> x;
    FixArray<nat, nn> col;
    static PNormalRand rand;
    nat i, k, j = nc+0;
    for ( i = 0; i < nr; ++i ) for ( k = 0; k < nn; ++k ) a[i][k] = rand(); 
    b = a;
    slu_gauss ( a, nc, col );
    for ( k =  0; k < nr; ++k ) x[col[k]] = a[k][j];
    for ( k = nr; k < nc; ++k ) x[col[k]] = 0;
    for ( i = 0; i < nr; ++i )
    {
        double s = - b[i][j];
        for ( k = 0; k < nc; ++k ) s += x[k] * b[i][k];
        display << s << NL;
    }
    display << NL;
}

bool sluGaussRow ( ArrRef<Suite<SortItem<nat, double>>> & data, ArrRef<nat> & index )
{
    const nat nRow = data.size();
    const nat nCol = index.size();
    if ( nRow >= nCol ) return false;
// Прямой ход
    nat i, j, k;
    for ( i = 0; i < nCol; ++i ) index[i] = i;
    for ( k = 0; k < nRow; ++k )
    {
// Поиск максимального по модулю члена в k-ой строке
        Suite<SortItem<nat, double> > & rk = data[k];
        const nat nc = rk.size();
        if ( ! nc ) return false;
        nat im = 0;
        double max = fabs ( rk[0].tail );
        for ( i = 1; i < nc; ++i )
        {
            if ( _maxa ( max, fabs ( rk[i].tail ) ) ) im = i;
        }
        if ( ! max ) return false;
        _swap ( index[k], index[rk[im].head] );
        const nat ik = index[k];
// Нормализация строки
        const double p = 1. / rk[im].tail;
        for ( i = 0; i < nc; ++i ) rk[i].tail *= p;
        rk.delAndShift ( im );
// Вычитание строк
        for ( j = k+1; j < nRow; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const double t = rj[ik].tail;
            for ( i = k+1; i < nCol; ++i )
            {
                const nat ii = index[i];
                if ( fabs ( rj[ii].tail -= rk[ii].tail * t ) < 1e-290 ) rj[ii].tail = 0;
            }
            rj[ik].tail = 0;
        }
    }
// Обратная подстановка
    for ( j = nRow; --j > 0; )
    {
        Suite<SortItem<nat, double> > & rj = data[j];
        const nat ij = index[j];
        for ( i = 0; i < j; ++i )
        {
            Suite<SortItem<nat, double> > & ri = data[i];
            const double t = ri[ij].tail;
            for ( k = nRow; k < nCol; ++k )
            {
                const nat ik = index[k];
                ri[ik].tail -= rj[ik].tail * t;
            }
            ri[ij].tail = 0;
        }
    }
    return true;
}

} // namespace

void math_test ()
{
//   sym_test ();
//    root2_test ();
//    columnScale_test ();
//    slu_ortho_test ();
//    ortho_test ();
//    orthes_test ();
//    jacobi_test();
//    rootN_test ();
//    hqr_test4 ();
//    level_test ();
//    lin_test1();
//    file_test2 ();
//    lss_test1();
//    zeroin_test ();
//    goldenRatioMin_test ();
//    goldenRatioMin_test ();
//    ellipseCircumference_test ();
//    fmax_test();
//    over_lin_sys2();
    minNorm_test3();
//    slu_gauss_test();
}
