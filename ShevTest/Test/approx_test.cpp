
#include "math.h"

#include "../draw.h"

#include "../Shev/tune.h"
#include "../Shev/LinAlg.h"
#include "../Shev/Mathem.h"
#include "../Shev/Matrix.h"
#include "../Shev/approx.h"
#include "../Shev/moment2d.h"
#include "../Shev/approx2d.h"
#include "../Shev/opti1d.h"
#include "../Shev/func1d.h"
#include "../Shev/func1t.h"
#include "../Shev/func2d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/ShevArray.h"
#include "../Shev/RealFile.h"

#include "display.h"

double timeInSec ();

namespace
{

void harmAppr_test()
{
    nat i;
    const nat nv = 111;
    DynArray<Vector2d> func ( nv );
    for ( i = 0; i < nv; ++i )
    {
        Vector2d & v = func[i];
        v.x = i * M_2PI / nv;
        v.y = 0.2 - 2*fabs ( sin ( 2 * v.x + M_PI/5 ) );
    }

    const nat nr = 40;
    HarmApprMod harm2 ( func, nr );
    harm2.rebuild ( func, 19, 1 );
    HarmAppr harm ( func, nr );

    DynArray<double> y ( nv );
    for ( i = 0; i < nv; ++i )
    {
        y[i] = harm ( func[i].x );
//display << func[i].x << func[i].y << y[i] << NL;
    }

    const Vector2d o ( 0.7, 0. );
    for ( i = 1; i < nv; ++i )
    {
        const double c = 0.25;
        const Vector2d a = c * func[i-1];
        const Vector2d b = c * func[i];
        Segment2d fig ( a - o, b - o );
        draw ( fig, 0, 1, 1 );
        fig.a.y = c * y[i-1] - o.y;
        fig.b.y = c * y[i] - o.y;
        draw ( fig, 1, 0, 0 );
    }
}

void read ( const char * filename, Suite<Vector3d> & arr )
{
    RealFile file ( filename, "r" );
    char c;
    Vector3d v;
    for (;;)
    {
        
        if ( ! readFltDec ( file, c, v.x ) || 
             ! readFltDec ( file, c, v.y ) || 
             ! readFltDec ( file, c, v.z ) ) return;
        arr.inc() = v;
		while ( c != '\n' && file.getc ( &c ) ) continue; // доходим до конца строки
    }
}

void write ( const char * filename, CCArrRef<Vector2d> & arr )
{
    RealFile file ( filename, "w" );
    for ( nat i = 0; i < arr.size(); ++i )
    {
        printf ( file, "%.3f %.3f\n", arr[i].x, arr[i].y );
    }
}

void write ( const char * filename, CCArrRef<Vector3d> & arr )
{
    RealFile file ( filename, "w" );
    for ( nat i = 0; i < arr.size(); ++i )
    {
        printf ( file, "%.4e %.4e %.4e\n", arr[i].x, arr[i].y, arr[i].z );
    }
}

void remont ( HarmApprMod & harm, ArrRef<Vector2d> & func, SuiteRef<nat> & res )
{
    const nat nv = func.size();
    for ( nat k = 0; k < 100; ++k )
    {
        nat im = 0;
        double max = 0, ym = 0;
        for ( nat i = 0; i < nv; ++i )
        {
            Vector2d & v = func[i];
            double y = harm ( v.x );
            double t = 1 - v.y / y;
            if ( max < t ) max = t, ym = y, im = i;
        }
        display << k << max << im << NL;
        if ( max < 0.01 ) return;
//if ( im < 400 ) return;
        res.addIfHasNot ( im );
        harm.rebuild ( func, im, ym );
    }
}

void remont ( HarmAppr2 & harm, ArrRef<Vector2d> & func, ArrRef<bool> & w, SuiteRef<nat> & res )
{
    const nat nv = func.size();
    for ( nat k = 0; k < 100; ++k )
    {
        nat im = 0, ns = 0;
        double max = 0, sum = 0;
        for ( nat i = 0; i < nv; ++i )
        {
            if ( !w[i] ) continue;
            Vector2d & v = func[i];
            double y = harm ( v.x );
            double t = 1 - v.y / y;
            if ( max < t ) max = t, im = i;
            sum += t * t;
            ++ns;
        }
        display << k << max << im << NL;
        if ( max < 0.01 )
        {
            if ( max < 0.001 ) return;
        display << sqrt ( sum / ns ) << NL;
            if ( max < 7 * sqrt ( sum / ns ) ) return;
        }
//if ( im < 400 ) return;
        res.inc() = im;
        w[im] = 0;
        harm.rebuild ( func, w );
    }
}

void breakthrough_test()
{
//    const char * filename1 = "data/29290309-1a.txt"; //2 b10 806-1084 
//    const char * filename1 = "data/863-HPO.txt"; //2 b20 6.632e-003 800-1424
//    const char * filename1 = "data/D-01554-001-ttttt.txt"; //2 b10 8.571e-004 808-909 
//    const char * filename1 = "data/HPO-3883_false_breakthrough2.txt"; //2 b0 2.002e-003 1600-1214 
    const char * filename1 = "data/Cylinder-HPO-22.5.txt"; //2 b0 2.589e-003 400-454 
//    const char * filename1 = "data/2019_12_24_breakthrough_1600.txt"; // нарезка
//    const char * filename1 = "data/2019_12_24_Contour_break_throught.txt"; // разброс значений

//    const char * filename2 = "data/29290309-1a_test1.txt";
    const char * filename2 = "data/863-HPO_test.txt";
//    const char * filename2 = "data/D-01554-001-ttttt_test.txt";
//    const char * filename2 = "data/HPO-3883_false_breakthrough_test2.txt";
//    const char * filename2 = "data/Cylinder-HPO-22.5_test.txt";
//    const char * filename2 = "data/2019_12_24_breakthrough_1600_test.txt";
//    const char * filename2 = "data/2019_12_24_Contour_break_throught_test.txt";
    Suite<Vector3d> arr;
    read ( filename1, arr );
    const nat nv = arr.size();
    if ( nv < 100 )
    {
        display << "nv =" << nv << NL;
        return;
    }
    nat i;
    DynArray<bool> w ( nv );
    DynArray<Vector2d> func ( nv ), func0;
    for ( i = 0; i < nv; ++i )
    {
        Vector2d & v = func[i];
        v.x = arr[i].x * 1e-9;
        v.y = arr[i].y;
        w[i] = 1;
    }
    func0 = func;
    const nat nr = 30;
//    HarmAppr2 harm ( func, w, nr, 2 );
    HarmAppr1 harm ( func, nr, 2 );
    Suite<nat> bad;
//    remont ( harm, func, w, bad );
//    remont ( harm, func, bad );
    insertSort123 ( bad );
    for ( i = 0; i < bad.size(); ++i ) display << bad[i] << NL;
    display << "bad" << bad.size() << NL;
    DynArray<Vector3d> res ( nv );
    for ( i = 0; i < nv; ++i )
    {
        Vector2d & v = func0[i];
        Vector3d & u = res[i];
        u.x = v.x;
        u.y = v.y;
        u.z = harm ( v.x );
    }
    write ( filename2, res );
    display << "end" << NL;
}

class Spline3p
{
    double a, b, c, d, x01;
public:
    Spline3p ( double x0, double x1, double y0, double y1, double p0, double p1  );
    double getY ( double x ) const
    {
        return x * ( x * ( x * a + b ) + c ) + d;
    }
    double getX ( double y ) const;
};

Spline3p::Spline3p ( double x0, double x1, double y0, double y1, double p0, double p1  )
{
    SLU4<double> slu;
    slu.aa = x0*x0*x0; slu.ab = x0*x0; slu.ac = x0; slu.ad = 1; slu.ae = y0;
    slu.ba = x1*x1*x1; slu.bb = x1*x1; slu.bc = x1; slu.bd = 1; slu.be = y1;
    slu.ca = 3.*x0*x0; slu.cb = 2.*x0; slu.cc = 1.; slu.cd = 0; slu.ce = p0;
    slu.da = 3.*x1*x1; slu.db = 2.*x1; slu.dc = 1.; slu.dd = 0; slu.de = p1;
    slu.gauss ( a, b, c, d );
}

void spline_test()
{
    Vector2d v0(1, 1), v1(1.05, 1.05), v2(1.1,1.07);
    Vector2d u1 = v1 - v0;
    Vector2d u2 = v2 - v1;
    Vector2d w0 = u1 - 0.2*u2;
    Vector2d w1 = u1 + u2;
    Vector2d w2 = u2 - 0.2*u1;
    Spline3p spline1 ( v0.x, v1.x, v0.y, v1.y, w0.y/w0.x, w1.y/w1.x );
    int i;
    for ( i = 90; i <= 105; i+=1 )
    {
        double x = 0.01*i;
        display << x << spline1.getY(x) << NL;
    }
    Spline3p spline2 ( v1.x, v2.x, v1.y, v2.y, w1.y/w1.x, w2.y/w2.x );
    for ( i = 106; i <= 120; i+=1 )
    {
        double x = 0.01*i;
        display << x << spline2.getY(x) << NL;
    }
}

} // end of namespace

void approx_test ()
{
    drawNewList2d();
//    harmAppr_test();
//    breakthrough_test();
    spline_test();
    endNewList();
}