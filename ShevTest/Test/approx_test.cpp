
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

struct ContFrag2
{
    Suite<Vector2d> vert;
    double len2, sina, cosa;
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
            q += qmod ( ( b - o ) % ( a - o ) ) / ci.len2;
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
    double fxfx = 0, fxfy = 0, fyfy = 0;
    double fxpx = 0, fypx = 0, spxx = 0;
    double fxpy = 0, fypy = 0, spyy = 0;
    double fxpxy = 0, fypxy = 0;
    double fxxpy = 0, fxypy = 0, fyypy = 0;
    double fxxpyy = 0, fxypyy = 0, fyypyy = 0;
    for ( nat k = 0; k < cont.size(); ++k )
    {
        const ContFrag2 & ci = cont[k];
        const nat n = ci.vert.size();
        const Vector2d f ( ci.sina, ci.cosa );
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
        
        //axd += ( py - n * vb.z ) * ( ( f.x * va.x + f.y * va.y ) * vb.z - va.z * ( f.x * vb.x + f.y * vb.y ) ) / L2 * f.x;
        //ayd += ( py - n * vb.z ) * ( ( f.x * va.x + f.y * va.y ) * vb.z - va.z * ( f.x * vb.x + f.y * vb.y ) ) / L2 * f.y;
        //azd += ( n * ( f.x * vb.x + f.y * vb.y ) - px ) * ( ( f.x * va.x + f.y * va.y ) * vb.z - va.z * ( f.x * vb.x + f.y * vb.y ) ) / L2;
        //bxd += ( n * va.z - py ) * ( ( f.x * va.x + f.y * va.y ) * vb.z - va.z * b.x ) / L2 * f.x;
        //byd += ( n * va.z - py ) * ( ( f.x * va.x + f.y * va.y ) * vb.z - va.z * b.x ) / L2 * f.y;
        //bzd += ( px - n * ( f.x * va.x + f.y * va.y ) ) * ( ( f.x * va.x + f.y * va.y ) * vb.z - va.z * ( f.x * vb.x + f.y * vb.y ) ) / L2;
        //axax += ( n * vb.z * vb.z - 2 * vb.z * py + pyy ) / L2 * fxx;
        //axay += ( n * vb.z * vb.z - 2 * vb.z * py + pyy ) / L2 * fxy;
        //ayay += ( n * vb.z * vb.z - 2 * vb.z * py + pyy ) / L2 * fyy;
        //axbx += ( vb.z * py - n * vb.z * va.z - pyy + py * va.z ) / L2 * fxx;
        //axby += ( vb.z * py - n * vb.z * va.z - pyy + py * va.z ) / L2 * fxy;
        // aybx = axby;
        //ayby += ( vb.z * py - n * vb.z * va.z - pyy + py * va.z ) / L2 * fyy;
        //axaz += ( ( f.x * vb.x + f.y * vb.y ) * py - n * ( f.x * vb.x + f.y * vb.y ) * vb.z - pxy + px * vb.z ) / L2 * f.x;
        //ayaz += ( ( f.x * vb.x + f.y * vb.y ) * py - n * ( f.x * vb.x + f.y * vb.y ) * vb.z - pxy + px * vb.z ) / L2 * f.y;
        //axbz += ( ( n * ( f.x * va.x + f.y * va.y ) - px ) * vb.z - py * ( f.x * va.x + f.y * va.y ) + pxy ) / L2 * f.x;
        //aybz += ( ( n * ( f.x * va.x + f.y * va.y ) - px ) * vb.z - py * ( f.x * va.x + f.y * va.y ) + pxy ) / L2 * f.y;
        //azaz += ( n * ( f.x * vb.x * f.x * vb.x + 2 * f.x * vb.x * f.y * vb.y + f.y * vb.y * f.y * vb.y ) - 2 * ( f.x * vb.x + f.y * vb.y ) * px + pxx ) / L2;
        //azbx += ( ( f.x * vb.x + f.y * vb.y ) * ( n * va.z - py ) ) / L2 * f.x;
        //azby += ( ( f.x * vb.x + f.y * vb.y ) * ( n * va.z - py ) ) / L2 * f.y;
        //azbz += ( ( f.x * vb.x + f.y * vb.y ) * px - n * ( f.x * vb.x + f.y * vb.y ) * ( f.x * va.x + f.y * va.y ) - pxx + px * ( f.x * va.x + f.y * va.y ) ) / L2;
        //bxbx += ( n * va.z * va.z - 2 * va.z * py + pyy ) / L2 * fxx;
        //bxby += ( n * va.z * va.z - 2 * va.z * py + pyy ) / L2 * fxy;
        //bxbz += ( a.x * py - n * a.x * va.z - pxy + px * va.z ) / L2 * f.x;
        //byby += ( n * va.z * va.z - 2 * va.z * py + pyy ) / L2 * fyy;
        //bybz += ( ( f.x * va.x + f.y * va.y ) * py - n * ( f.x * va.x + f.y * va.y ) * va.z - pxy + px * va.z ) / L2 * f.y;
        //bzbz += ( n * ( f.x * va.x * f.x * va.x + 2 * f.x * va.x * f.y * va.y + f.y * va.y * f.y * va.y ) - 2 * ( f.x * va.x + f.y * va.y ) * px + pxx ) / L2;

        const double pn = n / ci.len2;
        px /= ci.len2;
        py /= ci.len2;
        pxx /= ci.len2;
        pxy /= ci.len2;
        pyy /= ci.len2;

        fxfx += fxx * pn;
        fxfy += fxy * pn;
        fyfy += fyy * pn;
        fxpx += f.x * px;
        fypx += f.y * px;
        spxx += pxx;
        fxpy += f.x * py;
        fypy += f.y * py;
        spyy += pyy;
        fxpxy += f.x * pxy;
        fypxy += f.y * pxy;
        fxxpy += fxx * py;
        fxypy += fxy * py;
        fyypy += fyy * py;
        fxxpyy += fxx * pyy;
        fxypyy += fxy * pyy;
        fyypyy += fyy * pyy;
    }
    const double vx = va.y * vb.z - va.z * vb.y;
    const double vy = va.z * vb.x - va.x * vb.z;
    const double azxx = va.z * fxfx - fxxpy;
    const double bzxx = vb.z * fxfx - fxxpy;
    const double azxy = va.z * fxfy - fxypy;
    const double bzxy = vb.z * fxfy - fxypy;
    const double azyy = va.z * fyfy - fyypy;
    const double bzyy = vb.z * fyfy - fyypy;

    const double axpx = va.z * fxpx - fxpxy;
    const double bxpx = vb.z * fxpx - fxpxy;
    const double aypx = va.z * fypx - fypxy;
    const double bypx = vb.z * fypx - fypxy;

    const double aa = fxfx * va.x * va.x + 2 * fxfy * va.x * va.y + fyfy * va.y * va.y;
    const double ab = fxfx * va.x * vb.x + fxfy * ( va.x * vb.y + va.y * vb.x ) + fyfy * va.y * vb.y;
    const double bb = fxfx * vb.x * vb.x + 2 * fxfy * vb.x * vb.y + fyfy * vb.y * vb.y;
    const double ap = fxpx * va.x + fypx * va.y;
    const double bp = fxpx * vb.x + fypx * vb.y;
    
    const double azaz = bb - bp - bp + spxx;
    const double azbz = bp - ab + ap - spxx;
    const double bzbz = aa - ap - ap + spxx;
    
    const double axax = ( bzxx - fxxpy ) * vb.z + fxxpyy;
    const double axay = ( bzxy - fxypy ) * vb.z + fxypyy;
    const double ayay = ( bzyy - fyypy ) * vb.z + fyypyy;
    const double axbx = fxxpy * va.z - fxxpyy - azxx * vb.z;
    const double axby = fxypy * va.z - fxypyy - azxy * vb.z;
    const double ayby = fyypy * va.z - fyypyy - azyy * vb.z;
    const double bxbx = ( azxx - fxxpy ) * va.z + fxxpyy;
    const double bxby = ( azxy - fxypy ) * va.z + fxypyy;
    const double byby = ( azyy - fyypy ) * va.z + fyypyy;
    
    const double axaz = bxpx - bzxx * vb.x - bzxy * vb.y;
    const double ayaz = bypx - bzxy * vb.x - bzyy * vb.y;
    const double axbz = bzxx * va.x + bzxy * va.y - bxpx;
    const double aybz = bzxy * va.x + bzyy * va.y - bypx;
    const double azbx = azxx * vb.x + azxy * vb.y - axpx;
    const double azby = azxy * vb.x + azyy * vb.y - aypx;
    const double bxbz = axpx - azxx * va.x - azxy * va.y;
    const double bybz = aypx - azxy * va.x - azyy * va.y;

    const double axd = bzxx * vy - bzxy * vx;
    const double ayd = bzxy * vy - bzyy * vx;
    const double azd = ( bp - bb ) * va.z + ( ab - ap ) * vb.z;
    const double bxd = azxy * vx - azxx * vy;
    const double byd = azyy * vx - azxy * vy;
    const double bzd = ( ap - aa ) * vb.z + ( ab - bp ) * va.z;

    //display << qfunc ( va, vb, n, coef ) << qfunc ( va, vb, cont ) << NL;
    display << axax * va.x + axay * va.y + axaz * va.z + axbx * vb.x + axby * vb.y + axbz * vb.z + axd;
    display << axay * va.x + ayay * va.y + ayaz * va.z + axby * vb.x + ayby * vb.y + aybz * vb.z + ayd;
    display << axaz * va.x + ayaz * va.y + azaz * va.z + azbx * vb.x + azby * vb.y + azbz * vb.z + azd;
    display << axbx * va.x + axby * va.y + azbx * va.z + bxbx * vb.x + bxby * vb.y + bxbz * vb.z + bxd;
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
        ci.len2 = qmod ( b - a );
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