
#include "math.h"

#include "../Shev/LinAlg.h"
#include "../Shev/complex.h"
#include "../Shev/rand.h"
#include "../Shev/Mathem.h"
#include "../Shev/ShevArray2.h"

#include "display.h"

double timeInSec();

namespace {

void SLU2_test()
{
//    static PRand rand;
    static PNormalRand rand;
    SLU2<double, FixArray<double,2> > slu;
    slu.aa = rand();   slu.ab = rand();   slu.ac[0] = rand();   slu.ac[1] = rand();
    slu.ba = rand();   slu.bb = rand();   slu.bc[0] = rand();   slu.bc[1] = rand();
    FixArray<double,2> x, y;
    if ( ! slu.gauss ( x, y ) )
    {
        display << "error" << NL;
        return;
    }
    //display << slu.aa*x + slu.ab*y - slu.ac << slu.ba*x + slu.bb*y - slu.bc << NL;
    display << slu.aa*x[0] + slu.ab*y[0] - slu.ac[0] << slu.ba*x[0] + slu.bb*y[0] - slu.bc[0]
            << slu.aa*x[1] + slu.ab*y[1] - slu.ac[1] << slu.ba*x[1] + slu.bb*y[1] - slu.bc[1] << NL;
}

void SLU3_test()
{
//    static PRand rand;
    static PNormalRand rand;
    SLU3<double, FixArray<double,2> > slu;
    slu.aa = rand();   slu.ab = rand();   slu.ac = rand();   slu.ad[0] = rand();   slu.ad[1] = rand();
    slu.ba = rand();   slu.bb = rand();   slu.bc = rand();   slu.bd[0] = rand();   slu.bd[1] = rand();
    slu.ca = rand();   slu.cb = rand();   slu.cc = rand();   slu.cd[0] = rand();   slu.cd[1] = rand();
    FixArray<double,2> x, y, z;
    if ( ! slu.gauss ( x, y, z ) )
    {
        display << "error" << NL;
        return;
    }
    display << slu.aa*x[0] + slu.ab*y[0] + slu.ac*z[0] - slu.ad[0] 
            << slu.ba*x[0] + slu.bb*y[0] + slu.bc*z[0] - slu.bd[0] 
            << slu.ca*x[0] + slu.cb*y[0] + slu.cc*z[0] - slu.cd[0];
    display << slu.aa*x[1] + slu.ab*y[1] + slu.ac*z[1] - slu.ad[1] 
            << slu.ba*x[1] + slu.bb*y[1] + slu.bc*z[1] - slu.bd[1] 
            << slu.ca*x[1] + slu.cb*y[1] + slu.cc*z[1] - slu.cd[1] << NL;
}

void SLU4_test()
{
//    static PRand rand;
    static PNormalRand rand;
/*    
    SLU4<double, FixArray<double,2> > slu;
    slu.aa = rand();   slu.ab = rand();   slu.ac = rand();   slu.ad = rand();   slu.ae[0] = rand();   slu.ae[1] = rand();
    slu.ba = rand();   slu.bb = rand();   slu.bc = rand();   slu.bd = rand();   slu.be[0] = rand();   slu.be[1] = rand();
    slu.ca = rand();   slu.cb = rand();   slu.cc = rand();   slu.cd = rand();   slu.ce[0] = rand();   slu.ce[1] = rand();
    slu.da = rand();   slu.db = rand();   slu.dc = rand();   slu.dd = rand();   slu.de[0] = rand();   slu.de[1] = rand();
    FixArray<double,2> xa, xb, xc, xd;
    if ( ! slu.gauss ( xa, xb, xc, xd ) )
    {
        display << "error" << NL;
        return;
    }
    nat i = 0;
    display << slu.aa*xa[i] + slu.ab*xb[i] + slu.ac*xc[i] + slu.ad*xd[i] - slu.ae[i] 
            << slu.ba*xa[i] + slu.bb*xb[i] + slu.bc*xc[i] + slu.bd*xd[i] - slu.be[i] 
            << slu.ca*xa[i] + slu.cb*xb[i] + slu.cc*xc[i] + slu.cd*xd[i] - slu.ce[i] 
            << slu.da*xa[i] + slu.db*xb[i] + slu.dc*xc[i] + slu.dd*xd[i] - slu.de[i] << NL;
    i = 1;
    display << slu.aa*xa[i] + slu.ab*xb[i] + slu.ac*xc[i] + slu.ad*xd[i] - slu.ae[i] 
            << slu.ba*xa[i] + slu.bb*xb[i] + slu.bc*xc[i] + slu.bd*xd[i] - slu.be[i] 
            << slu.ca*xa[i] + slu.cb*xb[i] + slu.cc*xc[i] + slu.cd*xd[i] - slu.ce[i] 
            << slu.da*xa[i] + slu.db*xb[i] + slu.dc*xc[i] + slu.dd*xd[i] - slu.de[i] << NL;
/*/
    SLU4<Complex> slu;
    slu.aa = Complex(rand(),rand());   slu.ab = rand();   slu.ac = rand();   slu.ad = rand();   slu.ae = Complex(rand(),rand());
    slu.ba = Complex(rand(),rand());   slu.bb = rand();   slu.bc = rand();   slu.bd = rand();   slu.be = Complex(rand(),rand());
    slu.ca = Complex(rand(),rand());   slu.cb = rand();   slu.cc = rand();   slu.cd = rand();   slu.ce = Complex(rand(),rand());
    slu.da = Complex(rand(),rand());   slu.db = rand();   slu.dc = rand();   slu.dd = rand();   slu.de = Complex(rand(),rand());
    Complex xa, xb, xc, xd;
    if ( ! slu.gauss ( xa, xb, xc, xd ) )
    {
        display << "error" << NL;
        return;
    }
    display << ( slu.aa*xa + slu.ab*xb + slu.ac*xc + slu.ad*xd - slu.ae ).re
            << ( slu.ba*xa + slu.bb*xb + slu.bc*xc + slu.bd*xd - slu.be ).re
            << ( slu.ca*xa + slu.cb*xb + slu.cc*xc + slu.cd*xd - slu.ce ).re
            << ( slu.da*xa + slu.db*xb + slu.dc*xc + slu.dd*xd - slu.de ).re << NL;
    display << ( slu.aa*xa + slu.ab*xb + slu.ac*xc + slu.ad*xd - slu.ae ).im
            << ( slu.ba*xa + slu.bb*xb + slu.bc*xc + slu.bd*xd - slu.be ).im
            << ( slu.ca*xa + slu.cb*xb + slu.cc*xc + slu.cd*xd - slu.ce ).im
            << ( slu.da*xa + slu.db*xb + slu.dc*xc + slu.dd*xd - slu.de ).im << NL;
//*/
}

void sluGaussRow_test ()
{
    static PNormalRand rand;
    const nat nRow = 4, nCol = nRow+1, mCol = nRow;
    CmbArray2<double, nRow*nCol> data ( nRow, nCol ), data2 ( nRow, nCol );
    nat i, j;
    for ( i = 0; i < nRow; ++i )
    {
        for ( j = 0; j < nCol; ++j ) data[i][j] = (3*rand());
    }
//    data[0][0] = 2; data[0][1] =-3; data[0][2] = 2;
//    data[1][0] = 1; data[1][1] = 2; data[1][2] = 3;
    data2 = data;
    CmbArray<nat, 6> index ( nCol ), index2 ( nCol );
    CmbArray<double, 6> x ( mCol ), x2 ( mCol );
    if ( sluGaussRow ( data, nRow, nCol, index(), mCol ) )
    {/*
        for ( i = 0; i < nRow; ++i )
        {
            double sum = 0;
            for ( j = 0; j < mCol; ++j ) sum += data2[i][index[j]] * data[j][nCol-1];
            display << data2[i][nCol-1] << sum << NL;
        }*/
        for ( i = 0; i < mCol; ++i ) x[index[i]] = data[i][nCol-1];
        for ( i = 0; i < mCol; ++i ) display << x[i];
        display << NL;
    }
    else display << "err" << NL;
    if ( slu_gauss ( data2, mCol, index2 ) )
    {
        for ( i = 0; i < mCol; ++i ) x2[index2[i]] = data2[i][nCol-1];
        for ( i = 0; i < mCol; ++i ) display << x2[i];
        display << NL;
    }
    display << NL;
}

void sluGaussRow_test2 ()
{
    static PNormalRand rand;
    const nat nRow = 4, nCol = nRow+10;
    CmbArray2<double, nRow*nCol> data ( nRow, nCol ), data2 ( nRow, nCol );
    CmbArray<nat, 6> index ( nCol ), index2 ( nCol );
    double time1 = 0, time2 = 0;
    nat i, j, k;
    for ( k = 0; k < 100; ++k )
    {
        for ( i = 0; i < nRow; ++i )
        {
            for ( j = 0; j < nCol; ++j ) data[i][j] = (3*rand());
        }
        data2 = data;
        double t0 = timeInSec();
        sluGaussRow ( data, nRow, nCol, index(), nCol );
        double t1 = timeInSec();
        slu_gauss ( data2, nCol, index2 );
        double t2 = timeInSec();
        time1 += t1-t0;
        time2 += t2-t1;
    }
    display << time2 / time1 << NL;
}

inline double write ( double x ) { return fabs(x) < 1e-14 ? 0 : x; }

void sluGaussRow_test3 () // Обращение матриц
{
    static PNormalRand rand;
    const nat nRow = 4, nCol = 2*nRow;
    CmbArray2<double, nRow*nCol> data ( nRow, nCol ), data2 ( nRow, nCol );
    CmbArray<nat, nCol> index ( nCol );
    nat i, j, k;
    for ( i = 0; i < nRow; ++i )
    {
        for ( j = 0; j < nRow; ++j )
        {
            data[i][j] = (3*rand());
            data[i][j+nRow] = i == j ? 1 : 0;
        }
    }
    data2 = data;
    sluGaussRow ( data, nRow, nCol, index(), nRow );
    for ( i = 0; i < nRow; ++i )
    {
        for ( j = 0; j < nRow; ++j )
        {
            double sum = 0;
            for ( k = 0; k < nRow; ++k )
            {
                sum += data2[i][index[k]] * data[k][nRow+j];
            }
            display << write ( sum );
        }
        display << NL;
    }
}

} // namespace

void linalg_test ()
{
    sluGaussRow_test3 ();
//    SLU4_test();
}
