
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

struct Coef
{
    double ax, ay, az, bx, by, bz, c;
};

double qfunc ( const Vector2d & a, const Vector2d & b, const double L, const Vector2d & o )
{
    return qmod ( ( b - o ) % ( a - o ) / L );
}

double qfunc ( const Vector2d & a, const Vector2d & b, const double L, CCArrRef<Vector2d> & vert )
{
    double sum = 0;
    for ( nat i = 0; i < vert.size(); ++i ) sum += qfunc ( a, b, L, vert[i] );
    return sum;
}

double qfunc ( const Vector3d & va, const Vector3d & vb, const Vector2d & f, const double L, CArrRef<Vector2d> & vert )
{
    Vector2d a ( f.x * va.x + f.y * va.y, va.z );
    Vector2d b ( f.x * vb.x + f.y * vb.y, vb.z );
    double sum = 0;
    for ( nat i = 0; i < vert.size(); ++i ) sum += qfunc ( a, b, L, vert[i] );
    return sum;
}

double qfunc ( const Vector3d & va, const Vector3d & vb, nat n, const Coef * coef )
{
    double q = 0;
    for ( nat i = 0; i < n; ++i )
    {
        const Coef & r = coef[i];
        const double t = va.x * r.ax + va.y * r.ay + va.z * r.az + vb.x * r.bx + vb.y * r.by + vb.z * r.bz + r.c;
        q += t * t;
    }
    return q;
}

double dqdax ( const Vector3d & va, const Vector3d & vb, nat n, const Coef * coef )
{
    double e = 1e-5;
    Vector3d v = va;
    v.x += e;
    return ( qfunc ( v, vb, n, coef ) - qfunc ( va, vb, n, coef ) ) / e;
}

double dqday ( const Vector3d & va, const Vector3d & vb, nat n, const Coef * coef )
{
    double e = 1e-5;
    Vector3d v = va;
    v.y += e;
    return ( qfunc ( v, vb, n, coef ) - qfunc ( va, vb, n, coef ) ) / e;
}

double dqdaz ( const Vector3d & va, const Vector3d & vb, nat n, const Coef * coef )
{
    double e = 1e-5;
    Vector3d v = va;
    v.z += e;
    return ( qfunc ( v, vb, n, coef ) - qfunc ( va, vb, n, coef ) ) / e;
}

double dqdbx ( const Vector3d & va, const Vector3d & vb, nat n, const Coef * coef )
{
    double e = 1e-5;
    Vector3d v = vb;
    v.x += e;
    return ( qfunc ( va, v, n, coef ) - qfunc ( va, vb, n, coef ) ) / e;
}

double dqdby ( const Vector3d & va, const Vector3d & vb, nat n, const Coef * coef )
{
    double e = 1e-5;
    Vector3d v = vb;
    v.y += e;
    return ( qfunc ( va, v, n, coef ) - qfunc ( va, vb, n, coef ) ) / e;
}

double dqdbz ( const Vector3d & va, const Vector3d & vb, nat n, const Coef * coef )
{
    double e = 1e-5;
    Vector3d v = vb;
    v.z += e;
    return ( qfunc ( va, v, n, coef ) - qfunc ( va, vb, n, coef ) ) / e;
}

double dqdax ( const Vector3d & va, const Vector3d & vb, const Vector2d & f, const double L, CArrRef<Vector2d> & vert )
{
    double e = 1e-4;
    Vector3d v = va;
    v.x += e;
    return ( qfunc ( v, vb, f, L, vert ) - qfunc ( va, vb, f, L, vert ) ) / e;
}

double dqdby ( const Vector2d & a, const Vector2d & b, const double L, CArrRef<Vector2d> & vert )
{
    double e = 1e-4;
    Vector2d v = b;
    v.y += e;
    return ( qfunc ( a, v, L, vert ) - qfunc ( a, b, L, vert ) ) / e;
}

void run_test1()
{
    const nat n = 3;
    static PRandVector2d rand;
    static PRandVector3d rand3;
    const Vector2d f = rand();
    const Vector3d va = rand3();
    const Vector3d vb = rand3();
    const Vector2d a ( f.x * va.x + f.y * va.y, va.z );
    const Vector2d b ( f.x * vb.x + f.y * vb.y, vb.z );
    const Vector2d an = a * n;
    const Vector2d bn = b * n;
    const double L2 = qmod ( b - a );
    const double L = sqrt ( L2 );
    const double d = ( a % b ) / L;
    const double s = ( a % b ) / L2;
    const double fxx = f.x * f.x;
    const double fxy = f.x * f.y;
    const double fyy = f.y * f.y;
    //
    FixArray<Vector2d, n> vert;
    vert[0] = rand();
    vert[1] = rand();
    vert[2] = rand();
    double q = 0;
    double px = 0, py = 0, pxx = 0, pxy = 0, pyy = 0;
    Coef coef[n];
    for ( nat i = 0; i < vert.size(); ++i )
    {
        const Vector2d & p = vert[i];
        px += p.x;
        py += p.y;
        pxx += p.x * p.x;
        pxy += p.x * p.y;
        pyy += p.y * p.y;

        const Vector2d pa = ( p - a ) / L;
        const Vector2d bp = ( b - p ) / L;
        const double ax = f.x * bp.y;
        const double ay = f.y * bp.y;
        const double az = - bp.x;
        const double bx = f.x * pa.y;
        const double by = f.y * pa.y;
        const double bz = - pa.x;
        coef[i].ax = ax;
        coef[i].ay = ay;
        coef[i].az = az;
        coef[i].bx = bx;
        coef[i].by = by;
        coef[i].bz = bz;
        coef[i].c = - d;
        const double t = va.x * ax + va.y * ay + va.z * az + vb.x * bx + vb.y * by + vb.z * bz - d;
        q += t * t;
    }
    const double azaz = ( bn.x * b.x - 2 * b.x * px + pxx ) / L2;
    const double azbz = ( b.x * px - bn.x * a.x - pxx + px * a.x ) / L2;
    const double bzbz = ( an.x * a.x - 2 * a.x * px + pxx ) / L2;

    const double aaxy = ( a.x * py - an.x * a.y - pxy + px * a.y ) / L2;
    const double aayy = ( an.y * a.y - 2 * a.y * py + pyy ) / L2;
    const double abxy = ( bn.y * a.x - b.y * px - py * a.x + pxy ) / L2;
    const double abyx = ( bn.x * a.y - b.x * py - px * a.y + pxy ) / L2;
    const double abyy = ( b.y * py - bn.y * a.y - pyy + py * a.y ) / L2;
    const double bbxy = ( b.x * py - bn.x * b.y - pxy + px * b.y ) / L2;
    const double bbyy = ( bn.y * b.y - 2 * b.y * py + pyy ) / L2;
    const double as = ( an.y - py ) * s;
    const double bs = ( py - bn.y ) * s;

    const double axax = bbyy * fxx;
    const double axay = bbyy * fxy;
    const double axaz = bbxy * f.x;
    const double axbx = abyy * fxx;
    const double axby = abyy * fxy;
    const double axbz = abxy * f.x;
    const double axd = bs * f.x;
    const double ayay = bbyy * fyy;
    const double ayaz = bbxy * f.y;
    const double aybx = abyy * fxy;
    const double ayby = abyy * fyy;
    const double aybz = abxy * f.y;
    const double ayd = bs * f.y;
    const double azbx = abyx * f.x;
    const double azby = abyx * f.y;
    const double azd = ( bn.x - px ) * s;
    const double bxbx = aayy * fxx;
    const double bxby = aayy * fxy;
    const double bxbz = aaxy * f.x;
    const double bxd = as * f.x;
    const double byby = aayy * fyy;
    const double bybz = aaxy * f.y;
    const double byd = as * f.y;
    const double bzd = ( px - an.x ) * s;

   // display << q << qfunc ( a, b, L, vert ) << qfunc ( va , vb, n, coef ) << NL;
    display << axax * va.x + axay * va.y + axaz * va.z + axbx * vb.x + axby * vb.y + axbz * vb.z + axd;
    display << axay * va.x + ayay * va.y + ayaz * va.z + aybx * vb.x + ayby * vb.y + aybz * vb.z + ayd;
    display << axaz * va.x + ayaz * va.y + azaz * va.z + azbx * vb.x + azby * vb.y + azbz * vb.z + azd;
    display << axbx * va.x + aybx * va.y + azbx * va.z + bxbx * vb.x + bxby * vb.y + bxbz * vb.z + bxd;
    display << axby * va.x + ayby * va.y + azby * va.z + bxby * vb.x + byby * vb.y + bybz * vb.z + byd;
    display << axbz * va.x + aybz * va.y + azbz * va.z + bxbz * vb.x + bybz * vb.y + bzbz * vb.z + bzd << NL;
    display << 0.5 * dqdax ( va, vb, n, coef );
    display << 0.5 * dqday ( va, vb, n, coef );
    display << 0.5 * dqdaz ( va, vb, n, coef );
    display << 0.5 * dqdbx ( va, vb, n, coef );
    display << 0.5 * dqdby ( va, vb, n, coef );
    display << 0.5 * dqdbz ( va, vb, n, coef ) << NL;
}

struct ContFrag2
{
    Suite<Vector2d> vert;
    double length, sina, cosa;
};

struct Dist4
{
    nat id, n;
    Vector2d a, b, v;
    Line2d lna;
    double ox, oy, oxx, oxy, oyy;
};

double qfunc ( const Vector3d & va, const Vector3d & vb, CCArrRef<ContFrag2> & cont )
{
    double q = 0;
    for ( nat i = 0; i < cont.size(); ++i )
    {
        const ContFrag2 & ci = cont[i];
        const Vector2d f ( ci.sina, ci.cosa );
        const Vector2d a ( f.x * va.x + f.y * va.y, va.z );
        const Vector2d b ( f.x * vb.x + f.y * vb.y, vb.z );
        for ( nat j = 0; j < ci.vert.size(); ++j )
        {
            const Vector2d & o = ci.vert[j];
            q += qmod ( ( b - o ) % ( a - o ) / ci.length );
        }
    }
    return q;
}

double dqdax ( const Vector3d & va, const Vector3d & vb, CCArrRef<ContFrag2> & cont )
{
    double e = 1e-5;
    Vector3d v = va;
    v.x += e;
    return ( qfunc ( v, vb, cont ) - qfunc ( va, vb, cont ) ) / e;
}

double dqday ( const Vector3d & va, const Vector3d & vb, CCArrRef<ContFrag2> & cont )
{
    double e = 1e-5;
    Vector3d v = va;
    v.y += e;
    return ( qfunc ( v, vb, cont ) - qfunc ( va, vb, cont ) ) / e;
}

double dqdaz ( const Vector3d & va, const Vector3d & vb, CCArrRef<ContFrag2> & cont )
{
    double e = 1e-5;
    Vector3d v = va;
    v.z += e;
    return ( qfunc ( v, vb, cont ) - qfunc ( va, vb, cont ) ) / e;
}

double dqdbx ( const Vector3d & va, const Vector3d & vb, CCArrRef<ContFrag2> & cont )
{
    double e = 1e-5;
    Vector3d v = vb;
    v.x += e;
    return ( qfunc ( va, v, cont ) - qfunc ( va, vb, cont ) ) / e;
}

double dqdby ( const Vector3d & va, const Vector3d & vb, CCArrRef<ContFrag2> & cont )
{
    double e = 1e-5;
    Vector3d v = vb;
    v.y += e;
    return ( qfunc ( va, v, cont ) - qfunc ( va, vb, cont ) ) / e;
}

double dqdbz ( const Vector3d & va, const Vector3d & vb, CCArrRef<ContFrag2> & cont )
{
    double e = 1e-5;
    Vector3d v = vb;
    v.z += e;
    return ( qfunc ( va, v, cont ) - qfunc ( va, vb, cont ) ) / e;
}

void run_test ( CCArrRef<ContFrag2> & cont, const Vector3d & va, const Vector3d & vb )
{
    double axax = 0, axay = 0, axaz = 0, axbx = 0, axby = 0, axbz = 0, axd = 0;
    double           ayay = 0, ayaz = 0, aybx = 0, ayby = 0, aybz = 0, ayd = 0;
    double                               azbx = 0, azby = 0,           azd = 0;
    double                               bxbx = 0, bxby = 0, bxbz = 0, bxd = 0;
    double                                         byby = 0, bybz = 0, byd = 0;
    double                                                             bzd = 0;
    //nat nn = 0;
    double fxfx = 0, fxfy = 0, fyfy = 0;
    double fxpx = 0, fypx = 0, spxx = 0;
    for ( nat k = 0; k < cont.size(); ++k )
    {
        const ContFrag2 & ci = cont[k];
        const nat n = ci.vert.size();
        const Vector2d f ( ci.sina, ci.cosa );
        const Vector2d a ( f.x * va.x + f.y * va.y, va.z );
        const Vector2d b ( f.x * vb.x + f.y * vb.y, vb.z );
        const double L2 = ci.length * ci.length;
        const double fxx = f.x * f.x;
        const double fxy = f.x * f.y;
        const double fyy = f.y * f.y;
        //
        double px = 0, py = 0, pxx = 0, pxy = 0, pyy = 0;
        for ( nat i = 0; i < ci.vert.size(); ++i )
        {
            const Vector2d & p = ci.vert[i];
            px += p.x;
            py += p.y;
            pxx += p.x * p.x;
            pxy += p.x * p.y;
            pyy += p.y * p.y;
        }
        //sn += s * n;
        //spx += s * px;
        const double aaxy = ( a.x * py - n *a.x * a.y - pxy + px * a.y ) / L2;
        const double aayy = ( n * a.y * a.y - 2 * a.y * py + pyy ) / L2;
        const double abxy = ( n * b.y * a.x - b.y * px - py * a.x + pxy ) / L2;
        const double abyx = ( n * b.x * a.y - b.x * py - px * a.y + pxy ) / L2;
        const double abyy = ( b.y * py - n * b.y * a.y - pyy + py * a.y ) / L2;
        const double bbxy = ( b.x * py - n * b.x * b.y - pxy + px * b.y ) / L2;
        const double bbyy = ( n * b.y * b.y - 2 * b.y * py + pyy ) / L2;
        const double as = ( n * a.y - py ) * ( a.x * vb.z - va.z * b.x ) / L2;
        const double bs = ( py - n * b.y ) * ( a.x * vb.z - va.z * b.x ) / L2;

        axax += bbyy * fxx;
        axay += bbyy * fxy;
        axaz += bbxy * f.x;
        axbx += abyy * fxx;
        axby += abyy * fxy;
        axbz += abxy * f.x;
        axd += bs * f.x;
        ayay += bbyy * fyy;
        ayaz += bbxy * f.y;
        aybx += abyy * fxy;
        ayby += abyy * fyy;
        aybz += abxy * f.y;
        ayd += bs * f.y;
        //azaz += ( n * ( f.x * vb.x * f.x * vb.x + 2 * f.x * vb.x * f.y * vb.y + f.y * vb.y * f.y * vb.y ) - 2 * ( f.x * vb.x + f.y * vb.y ) * px + pxx ) / L2;
        azbx += abyx * f.x;
        azby += abyx * f.y;
        //azbz += ( ( f.x * vb.x + f.y * vb.y ) * px - n * ( f.x * vb.x + f.y * vb.y ) * ( f.x * va.x + f.y * va.y ) - pxx + px * ( f.x * va.x + f.y * va.y ) ) / L2;
        azd += ( n * b.x - px ) * ( a.x * vb.z - va.z * b.x ) / L2;
        bxbx += aayy * fxx;
        bxby += aayy * fxy;
        bxbz += aaxy * f.x;
        bxd += as * f.x;
        byby += aayy * fyy;
        bybz += aaxy * f.y;
        byd += as * f.y;
        //bzbz += ( n * ( f.x * va.x * f.x * va.x + 2 * f.x * va.x * f.y * va.y + f.y * va.y * f.y * va.y ) - 2 * ( f.x * va.x + f.y * va.y ) * px + pxx ) / L2;
        //bzd += ( px - n * a.x ) * ( a.x * vb.z - va.z * b.x ) / L2;
        bzd += ( n * a.x * ( f.x * vb.x + f.y * vb.y ) ) * va.z / L2;

        fxfx += n * fxx / L2;
        fxfy += n * fxy / L2;
        fyfy += n * fyy / L2;
        fxpx += f.x * px / L2;
        fypx += f.y * px / L2;
        spxx += pxx / L2;
    }
    const double aa = fxfx * va.x * va.x + 2 * fxfy * va.x * va.y + fyfy * va.y * va.y;
    const double bb = fxfx * vb.x * vb.x + 2 * fxfy * vb.x * vb.y + fyfy * vb.y * vb.y;
    const double ap = fxpx * va.x + fypx * va.y;
    const double bp = fxpx * vb.x + fypx * vb.y;

    const double azaz = bb - 2 * bp + spxx;
    const double azbz = bp - fxfx * vb.x * va.x - fxfy * vb.x * va.y - fxfy * vb.y * va.x - fyfy * vb.y * va.y + ap - spxx;
    const double bzbz = aa - 2 * ap + spxx;
    bzd -= ( aa - ap ) * vb.z + bp * va.z;

    //display << qfunc ( va, vb, n, coef ) << qfunc ( va, vb, cont ) << NL;
    display << axax * va.x + axay * va.y + axaz * va.z + axbx * vb.x + axby * vb.y + axbz * vb.z + axd;
    display << axay * va.x + ayay * va.y + ayaz * va.z + aybx * vb.x + ayby * vb.y + aybz * vb.z + ayd;
    display << axaz * va.x + ayaz * va.y + azaz * va.z + azbx * vb.x + azby * vb.y + azbz * vb.z + azd;
    display << axbx * va.x + aybx * va.y + azbx * va.z + bxbx * vb.x + bxby * vb.y + bxbz * vb.z + bxd;
    display << axby * va.x + ayby * va.y + azby * va.z + bxby * vb.x + byby * vb.y + bybz * vb.z + byd;
    display << axbz * va.x + aybz * va.y + azbz * va.z + bxbz * vb.x + bybz * vb.y + bzbz * vb.z + bzd << NL;
    display << 0.5 * dqdax ( va, vb, cont );
    display << 0.5 * dqday ( va, vb, cont );
    display << 0.5 * dqdaz ( va, vb, cont );
    display << 0.5 * dqdbx ( va, vb, cont );
    display << 0.5 * dqdby ( va, vb, cont );
    display << 0.5 * dqdbz ( va, vb, cont ) << NL;
}

void run_test()
{
    const nat n = 3;
    static PRandVector2d rand;
    static PRandVector3d rand3;
    const Vector3d va = rand3();
    const Vector3d vb = rand3();
    FixArray<ContFrag2, 3> cont;
    for ( nat i = 0; i < cont.size(); ++i )
    {
        ContFrag2 & ci = cont[i];
        const Vector2d f = rand();
        ci.sina = f.x;
        ci.cosa = f.y;
        ci.vert.resize(n);
        for ( nat j = 0; j < n; ++j )
        {
            ci.vert[j] = rand();
        }
        const Vector2d a ( f.x * va.x + f.y * va.y, va.z );
        const Vector2d b ( f.x * vb.x + f.y * vb.y, vb.z );
        ci.length = norm2 ( b - a );
    }
    run_test ( cont, va, vb );
}

} // end of namespace

void approx_test ()
{
    drawNewList2d();
//    harmAppr_test();
//    breakthrough_test();
//    spline_test();
    run_test();
    endNewList();
}