
#include "math.h"
#include "stdlib.h"

#include "../Shev/tune.h"
#include "../Shev/rand.h"
#include "../Shev/func3d.h"
#include "../Shev/mathem.h"
#include "../Shev/Vector2d.h"
#include "../Shev/Vector4d.h"
#include "../Shev/ShevArray.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/StringFile.h"
#include "../draw.h"

#include "display.h"

double timeInSec();

namespace
{
inline nat32 qr ( nat32 a, nat32 b )
{
    const nat32 t = a > b ? a - b : b - a;
    return t * t;
}

double randtest2 ( PRand & rand, nat32 nd, nat32 * buf, nat32 ni )
{
    if ( nd < 2 || ni == 0 ) return 0.;
    nat32 i;
    const nat32 nc = nd * nd;
    for ( i = 0; i < nc; ++i ) buf[i] = 0;
    const double dd = nd;
    const nat32 nn = nc * ni;
    for ( i = 0; i < nn; ++i )
    {
        const nat32 x = nat32 ( dd * rand() );
        const nat32 y = nat32 ( dd * rand() );
        ++buf[x+y*nd];
    }
    nat32 disp = 0;
    for ( i = 0; i < nc; ++i )
    {
        disp += qr ( buf[i], ni );
    }
    return double ( disp ) / nc;
}

double randtest3 ( PRand & rand, nat32 nd, nat32 * buf, nat32 ni )
{
    if ( nd < 2 || ni == 0 ) return 0.;
    nat32 i;
    const nat32 nc = nd * nd * nd;
    for ( i = 0; i < nc; ++i ) buf[i] = 0;
    const double dd = nd;
    const nat32 nn = nc * ni;
    for ( i = 0; i < nn; ++i )
    {
        const nat32 x = nat32 ( dd * rand() );
        const nat32 y = nat32 ( dd * rand() );
        const nat32 z = nat32 ( dd * rand() );
        ++buf[x+nd*(y+z*nd)];
    }
    nat32 disp = 0;
    for ( i = 0; i < nc; ++i )
    {
        disp += qr ( buf[i], ni );
    }
    return double ( disp ) / nc;
}
/*
class PRandM : public PRand
{
    nat32 next;
public:
    PRandM ( nat32 u = 0 ) : next (u*4+1) {}
    virtual double value ();
};
const double p32 = 1. / 4294967296.;

double PRandM::value()
{
    next = next * 1220703125;
    return next * p32;
}
*/
void rand_test1 ()
{/*
    nat32 buf[10000];
    static PRand  rand;
    nat32 count = 0;
    double sum1 = 0;
    double sum2 = 0;
    for ( nat32 i = 10; i <= 50; i += 1 )
    {
        double disp = randtest3 ( rand, 10, buf, i );
        double x = sqrt(2.*disp) - sqrt(2.*i);
        sum1 += x;
        sum2 += x * x;
        ++count;
    }
    sum1 /= count;
    sum2 /= count;
    sum2 -= sum1 * sum1;
    display << sum1 << sum2 << NL;
/*/
    double t0 = timeInSec();
    double max = 0.;
    for ( nat32 i = 0; i < 0x7FFFFFFF; i += 1 )
    {
        PRand rand(i);
        double t = rand();
        if ( max < t ) max = t;
        if ( t == 1. )
        {
        PRand rand(i);
        double t = rand();
            display << i << NL;
//            break;
        }
    }
    double t1 = timeInSec();
    display << 1. - max << NL;
    display << t1-t0 << NL;//*/
}

void rand_test2 ()
{
    nat32 buf[128];
    static PRand  rand;
    const nat32 m = 1000;
    for ( nat32 n = 2; n <= 128; n += 1 )
    {
        nat32 i;
        for ( i = 0; i < n; i += 1 ) buf[i] = 0;
        for ( i = 0; i < n*m; i += 1 )
        {
            ++buf[rand.number(n)];
        }
        double d1 = 0;
        for ( i = 0; i < n; i += 1 ) d1 += qr ( buf[i], m );
        for ( i = 0; i < n; i += 1 ) buf[i] = 0;
        for ( i = 0; i < n*m; i += 1 )
        {
            ++buf[::rand() % n];
        }
        double d2 = 0;
        for ( i = 0; i < n; i += 1 ) d2 += qr ( buf[i], m );
        display << n << d1/n << d2/n << NL;
    }
}

void testNormalRand ()
{
    static PNormalRand rand;
    nat32 i, j, n = 100;
    for ( i = 1; i <= 20; ++i )
    {
        double sum1 = 0, sum2 = 0;
        double sum3 = 0, sum4 = 0, pre = 0;
        for ( j = 1; j <= n; ++j )
        {
            double g = rand();
            sum1 += g;
            sum2 += g * g;
            sum3 += fabs ( g );
            sum4 += g * pre;
            pre = g;
        }
        display << sum1 / n << sum2 / n << sum3 / ( 0.798 * n ) << sum4 / n << NL;
    }
}

void testRandVector4d ()
{
    const nat32 nn = 2000;
    Vector4d v[nn];
    QRandVector4d rand;
    for ( nat32 n = 0; n < nn; ++n )
    {
        v[n] = rand ();
        if ( n % 10 != 9 ) continue;
        double min = 4., max = 0.;
        nat32 im = n + 1, jm = n + 1;
        for ( nat32 i = 0; i <= n; ++i )
        {
            double q = 4.;
            nat32 km = n + 1;
            for ( nat32 j = 0; j <= n; ++j )
            {
                if ( i == j ) continue;
                const double t = qmod ( v[i] - v[j] );
                if ( q > t ) q = t, km = j;
            }
            if ( max < q ) max = q, im = i, jm = km;
            if ( min > q ) min = q;
        }
//        display << sqrt(min*(n+1)) << sqrt(max*(n+1)) << NL;
//        display << n+1 << sqrt(min) << sqrt(max) << im << jm << NL;
        display << n+1 << sqrt(min/max) << NL;
    }
}

template<class A, class B> inline 
A & shuffle1 ( A & a, B & b )
{
    for ( nat i = 0; i < a.size(); ++i ) _swap ( a[i], a[b.number(a.size())] );
    return a;
}

void test_shuffle ()
{
    FixArray<nat, 19> arr, arr1, arr2;
    arr1.fill(0);
    arr2.fill(0);
    static PRand rand;
    nat j;
    for ( nat i = 0; i < 20; ++i )
    {
        for ( j = 0; j < arr.size(); ++j ) arr[j] = j;
        sum ( arr1, arr1, shuffle ( arr, rand ) );
        for ( j = 0; j < arr.size(); ++j ) arr[j] = j;
        sum ( arr2, arr2, shuffle ( arr, rand ) );
    }
        display << arr1[firMax ( arr1 )] - arr1[firMin ( arr1 )] <<
                   arr2[firMax ( arr2 )] - arr2[firMin ( arr2 )] << NL;
}

void testRandPoint2d ()
{
    const nat32 nn = 500;
    Vector2d v;
    static QRandPoint2d rand1;
    static QRand2Point2d rand2;
    for ( nat32 n = 0; n < nn; ++n )
    {
        v = rand1();
        v /= 2;
        v.x -= 0.6;
        drawPoint ( v, 0, 1, 1 );
        v = rand2();
        v /= 2;
        v.x += 0.6;
        drawPoint ( v, 0, 1, 1 );
    }
}

void testRandPoint2d2 ()
{
    nat i, j;
    const nat m = 8;
    const nat m2 = m*m;
    nat nn[m][m];
    for ( nat n = m2; n <= 30*m2; n += 8 )
    {
        for ( i = 0; i < m; ++i )
        {
            for ( j = 0; j < m; ++j ) nn[i][j] = 0;
        }
        QRand2Point2d rand;
        for ( i = 0; i < n; ++i )
        {
            const Vector2d v = rand ();
            const nat ii = (nat) floor ( m * ( v.x + 1 ) / 2 );
            const nat jj = (nat) floor ( m * ( v.y + 1 ) / 2 );
            ++nn[ii][jj];
        }
        nat max = 0, min = m2;
        double avg = n / m2, sigma = 0;
        for ( i = 0; i < m; ++i )
        {
            for ( j = 0; j < m; ++j )
            {
                const nat k = nn[i][j];
                if ( max < k ) max = k; else
                if ( min > k ) min = k;
                sigma += _pow2 ( k - avg );
            }
        }
        sigma = sqrt ( sigma / m2 );
        display << n << min << max << sigma << NL;
    }
}

double getSigma ( CCArrRef<double> & data )
{
    const nat n = data.size();
    double avg = amean ( data ), sigma = 0;
    for ( nat i = 0; i < n; ++i )
    {
        sigma += _pow2 ( data[i] - avg );
    }
    return sqrt ( sigma / n );
}

//double g3 = 1.22074408460575947536;
//double g1 = 1.61803398874989484820;

void calcSigma ( const Polyhedron & poly, double & edgeSigma, double & areaSigma )
{
    Suite<double> edge, area;
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & f = poly.facet[i];
        CArrRef<nat> p = f.index;
        for ( nat j = 0; j < f.nv; ++j )
        {
            double t = sqrt ( qmod ( poly.vertex[p[j]] - poly.vertex[p[j+1]] ) );
            edge.inc() = t;
        }
        double t = f.getArea ( poly.vertex );
        area.inc() = t;
    }
    edgeSigma = getSigma ( edge );
    areaSigma = getSigma ( area );
}

void testRandVector3d()
{
    Polyhedron poly;
    Suite<double> edge, area;
    for ( nat n = 2840; n <= 2880; n += 1 )
    {
 //if(n!=1095) continue;
        DynArray<Vector3d> v ( n );
        //v << QRand2Vector3d();
        {
            for ( nat i = 0; i < n; ++i )
            {
                double z = ( 1 + 2*i ) / double(n) - 1;
                double r = sqrt ( 1 - z*z );
                double a = i * M_2PI * 0.61803398874989484820;
                v[i] = Vector3d ( cos(a)*r, sin(a)*r, z );
            }
        }
        convexHull ( v, poly );
//if(n == 950) 
//        draw ( poly, 0, 1, 1, 1, VM_WIRE );
        edge.resize();
        area.resize();
	    double max_edge = 0.0, min_edge = 1e300;
	    double max_facet = 0.0, min_facet = 1e300;
        for ( nat i = 0; i < poly.facet.size(); ++i )
        {
            const Facet & f = poly.facet[i];
            CArrRef<nat> p = f.index;
            for ( nat j = 0; j < f.nv; ++j )
            {
                double t = sqrt ( qmod ( poly.vertex[p[j]] - poly.vertex[p[j+1]] ) );
                if ( max_edge < t ) max_edge = t;
                if ( min_edge > t ) min_edge = t;
                edge.inc() = t;
            }
            double t = f.getArea ( poly.vertex );
            if ( max_facet < t ) max_facet = t;
            if ( min_facet > t ) min_facet = t;
            area.inc() = t;
        }
        double s = sqrt ( double ( n ) );
//        display << n << s*min_edge << s*max_edge << n*min_facet << n*max_facet << NL;
        display << n << 100000*  getSigma ( edge ) << 100000* getSigma ( area ) << NL;
    }
}
const double g11 = 1/1.61803398874989484820;
const double g21 = 1/1.32471795724474602596;
const double g22 = g21 * g21;

const double g31 = 1/1.22074408460575947536;
const double g32 = g31 * g31;
const double g33 = g32 * g31;

void testRandOrtho3d()
{
    const nat n = 900;
    static QRand q1(2), q2(3), q3(5);
    DynArray<Vector3d> v1(n), v2(n), v3(n);
    double t1 = 0, t2 = 0, t3 = 0, f = q1();
    for ( nat i = 0; i < n; ++i )
    {
        /*
        const double t1 = q1();
        const double t2 = q2();
        const double t3 = q3();
        /*/
        t1 = ( 0.5 + i ) / n;
        //if ( ( t1 += g31 ) > 1 ) t1 -= 1;
        if ( ( t2 += g11 ) > 1 ) t2 -= 1;
        //if ( ( t3 += g33 ) > 1 ) t3 -= 1;
        if ( ( t3 += sqrt(f) ) > 1 ) t3 -= 1;
        //const double t3 = q1();
        //*/
        const LinTran3d & r = * getRandOrtho3d ( t1, t2, t3 );
        v1[i] = r.x;
        v2[i] = r.y;
        v3[i] = r.z;
    }
    Polyhedron poly1, poly2, poly3;
    double  edge1, area1, edge2, area2, edge3, area3;
    convexHull ( v1, poly1 );
    calcSigma ( poly1, edge1, area1 );
    convexHull ( v2, poly2 );
    calcSigma ( poly2, edge2, area2 );
    convexHull ( v3, poly3 );
    calcSigma ( poly3, edge3, area3 );
    draw ( poly2, 0, 1, 1, 1, VM_WIRE );
//    draw ( poly2, 1, 1, 0, 1, VM_WIRE );
//    draw ( poly3, 1, 1, 1, 1, VM_WIRE );
    display << f << 10000*edge1 << 10000*edge2 << 10000*edge3 << NL;
}

void testRandOrtho3d2()
{
    for ( nat n = 2200; n <= 2300; n += 1 )
    {
        //QRand q1(2), q2(3), q3(5);
        DynArray<Vector3d> v1(n), v2(n), v3(n);
        double t2 = 0, t3 = 0;
        for ( nat i = 0; i < n; ++i )
        {
            /*
            const double t1 = q1();
            const double t2 = q2();
            const double t3 = q3();
            /*/
            const double t1 = ( 0.5 + i ) / n;
            if ( ( t2 += g11 ) > 1 ) t2 -= 1;
            if ( ( t3 += 0.52785731707610565877  ) > 1 ) t3 -= 1;
            //if ( ( t3 += 0.52788364907610567655  ) > 1 ) t3 -= 1;
            //*/
            const LinTran3d & r = * getRandOrtho3d ( t1, t2, t3 );
            v1[i] = r.x;
            v2[i] = r.y;
            v3[i] = r.z;
        }
        Polyhedron poly1, poly2, poly3;
        //double  edge1, area1, edge2, area2, edge3, area3;
        double  edge2, area2, edge3, area3;
        //convexHull ( v1, poly1 );
        //calcSigma ( poly1, edge1, area1 );
        convexHull ( v2, poly2 );
        calcSigma ( poly2, edge2, area2 );
        convexHull ( v3, poly3 );
        calcSigma ( poly3, edge3, area3 );
        display << n << 10000*(edge2 + edge3) << NL;
    }
}

void testRandOrtho3d3()
{
    QRand qr(2);
    const nat n = 2286;
    DynArray<Vector3d> v1(n), v2(n), v3(n);
    nat jm;
    double min = 1e9, fm;
    double a = 0.52785731707610565877;
    double h = 1e-9;
    for ( int j = -500; j <= 500; ++j )
    {
        double t2 = 0, t3 = 0;
        double r = qr();
//if(j==536) display << r * 25*25 << NL;
        //double f = 1 - 927/(625*M_PI); // 1
        //double f = 1 - r*3/M_PI; // 1
        //double f = 1 - root4(r); // 2
        //double f = r*2/M_PI; // 3
        double f = a + j * h;
        for ( nat i = 0; i < n; ++i )
        {
            const double t1 = ( 0.5 + i ) / n;
            if ( ( t2 += g11 ) > 1 ) t2 -= 1;
            if ( ( t3 += f ) > 1 ) t3 -= 1;
            const LinTran3d & r = * getRandOrtho3d ( t1, t2, t3 );
            v1[i] = r.x;
            v2[i] = r.y;
            v3[i] = r.z;
        }
        Polyhedron poly1, poly2, poly3;
        double  edge2, area2, edge3, area3;
//        double  edge1, area1, edge2, area2, edge3, area3;
//        convexHull ( v1, poly1 );
//        calcSigma ( poly1, edge1, area1 );
        convexHull ( v2, poly2 );
        calcSigma ( poly2, edge2, area2 );
        convexHull ( v3, poly3 );
        calcSigma ( poly3, edge3, area3 );
        double t = 1e4*(edge2 + edge3);
        if ( min > t ) min = t, jm = j, fm = f;
        StringWriteFile file;
        printf ( file, "%d %.5f %.20f", j, t, f );
        display << file() << NL;
    }
    /*StringWriteFile file;
    printf ( file, "// %4d/9000 %.3f %.20f", jm, min, fm );
    display << file() << NL;*/
}

//  700 1202.09442 0.52785797307610557993
//  900 1041.96287 0.52788364907610567655 
// 2273  700.63299 0.52785731707610565877 
// 2286  699.92013 0.52785722807610557794 

// 6745/9000 1071.544 q2/r2 0.24954171196414902667 
// 4717/9000 1061.656 q3/r2 0.52787822195575504480 
// 1721/9000 1096.396 q5/r2 0.41679702110259855274 

// 5365/9000 1084.689 q2/r3 0.25068785492034901807 
// 2860/9000 1081.007 q3/r3 0.56239072860705663359 
// 1339/9000 1080.281 q5/r3 0.08350794293558749359 

// 8956/9000 1109.018 1-q2/r2 0.47169835763522174332 
// 6904/9000 1103.019 1-q3/r2 0.47230140177568475934 
// 7578/9000 1092.456 1-q5/r2 0.41680095429433352549 

// 6304/9000 1106.950 1-q2/r3 0.69983706097673152868 
// 5230/9000 1084.050 1-q3/r3 0.56238943931107936435 
// 6428/9000 1043.982 1-q5/r3 0.52788451907610571822 * 1


// 6693/9000 1094.535 q2*2/pi 0.24953443470120895142 
// 0643/9000 1081.415 q3*2/pi 0.56239112949893330650
// 0120/9000 1081.421 q5*2/pi 0.24955495076809190990 

// 8624/9000 1045.603 q2*3/pi 0.52788073226927343384 *
// 7790/9000 1054.736 q3*3/pi 0.25067934490346690523 *
// 4884/9000 1087.158 q5*3/pi 0.08317819297845872483 

// 8746/9000 1088.207 1-q2*2/pi 0.47213869582436558403 
// 2377/9000 1081.200 1-q3*2/pi 0.56239061524496394284 
// 6692/9000 1108.538 1-q5*2/pi 0.52831058529923264899 ^

// 3218/9000 1102.636 1-q2*3/pi 0.24953159890701503975 
// 7132/9000 1053.761 1-q3*3/pi 0.25067883064949745275 *
// 0536/9000 1044.354 1-q5*3/pi 0.52788277681220163728 * 2
}

void rand_test()
{
    drawNewList2d();
//    rand_test2 ();
//    testRandVector4d ();
//    testNormalRand ();
//    test_shuffle ();
    testRandVector3d();
//    testRandOrtho3d3();
    endNewList();
}