
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/rand.h"
#include "../Shev/opti1d.h"
#include "../Shev/Mathem.h"

#include "display.h"

double timeInSec();

namespace {

static nat nt = 0;

class TestFunc2 : public MathFunc1
{
    const double a;
public:
    TestFunc2 ( const double p ) : a(p) {}
    double operator () ( double x ) const
    {
        ++nt;
//        return cbrt ( x - a ) * ( x - a ) - a;
        return a - root3 ( x - a ) * ( x - a );
    }
};

void goldenRatioMin_test ()
{
    static PRand rand;
    for ( nat k = 0; k < 100000; ++k )
    {
        const double x = 1e5 * ( rand() - 0.5 );
        TestFunc2 func ( x );
        const double a = x - 1e5 * rand();
        const double b = x + 1e5 * rand();
        const double eps = 1e-10;
        const double y = goldenRatioMin ( a, b, func, eps );
//if(fabs(x-y) > eps) message(fabs(x-y), eps);
    }
  //  message("end");
}

void fmin_test ()
{
    static PRand rand;
    nat n = 0, nt1 = 0, nt2 = 0;
    for ( nat k = 0; k < 2000; ++k )
    {
        const double x = 1e5 * ( rand() - 0.5 );
        TestFunc2 func ( x );
        const double a = x - 1e5 * rand();
        const double b = x + 1e5 * rand();
        const double eps = 1e-6;
        double y = fmin ( a, b, func, eps );
        nt1 += nt;
        nt = 0;
         goldenRatioMin ( a, b, func, eps );
        nt2 += nt;
        nt = 0;
        const double c = 1;
        if ( fabs(x-y) > c*eps ) 
        {
//            fmin ( a, b, func, eps );
            display << k << fabs(x-y) << c*eps << NL;
            ++n;
        }
    }
    display << "end" << n << nt1/double(nt2) << NL;
}

class MaxFunc1 : public MathFunc1
{
    const double a;
public:
    MaxFunc1 ( const double p ) : a(p) {}
    double operator () ( double x ) const
    {
        ++nt; return a - fabs ( x - a );
    }
};

class MaxFunc43 : public MathFunc1
{
    const double a;
public:
    MaxFunc43 ( const double p ) : a(p) {}
    double operator () ( double x ) const
    {
        ++nt; return a - root3 ( x - a ) * ( x - a );
    }
};

class MaxFunc2 : public MathFunc1
{
    const double a;
public:
    MaxFunc2 ( const double p ) : a(p) {}
    double operator () ( double x ) const
    {
        ++nt; return a - ( x - a ) * ( x - a );
    }
};

void fmax_test ()
{
    static PRand rand;
    nat n = 0, nt1 = 0, nt2 = 0;
    double min = 999, max = 0;
    for ( nat k = 0; k < 2000; ++k )
    {
        const double x = 1e5 * ( rand() - 0.5 );
        MaxFunc43 func ( x );
        const double a = x - 1e5 * rand();
        const double b = x + 1e5 * rand();
        const double eps = 1e-6;
        double y = fmax ( a, b, func, eps );
        nt1 += nt;
        double t1 = nt;
        nt = 0;
         goldenRatioMax ( a, b, func, eps );
        nt2 += nt;
        double s = t1 / nt;
        if ( max < s ) max = s;
        if ( min > s ) min = s;
        nt = 0;
        const double c = 2;
        if ( fabs(x-y) > c*eps ) 
        {
//            fmin ( a, b, func, eps );
            display << k << fabs(x-y) << c*eps << NL;
            ++n;
        }
    }
    display << "end" << n << nt1/double(nt2) << min << max << NL;
}

class TestFunc3 : public MathFunc1
{
    const double a;
public:
    TestFunc3 ( const double p ) : a(p) {}
    double operator () ( double x ) const
    {
        return ( x - a ) * ( a - x );
    }
};

void goldenRatioMax_test ()
{
    static PRand rand;
    for ( nat k = 0; k < 1000; ++k )
    {
        const double x = 1e5 * ( rand() - 0.5 );
        TestFunc3 func ( x );
        const double a = x - 1e5 * rand();
        const double b = x + 1e5 * rand();
        const double eps = 1e-10;
        const double y = goldenRatioMax ( a, b, func, eps );
//if(fabs(x-y) > eps) message(fabs(x-y), eps);
    }
  //  message("end");
}

inline double func ( double x, const Set2<double> & s )
{
    return x * ( x + s.a ) + s.b;
}

double func ( double x, CArrRef<Set2<double> > arr )
{
    double max = func ( x, arr[0] );
    for ( nat i = 1; i < arr.size(); ++i )
    {
        double t = func ( x, arr[i] );
        if ( max < t ) max = t;
    }
    return max;
}

void getMinMax_test ()
{
    static PRand rand;
    FixArray<Set2<double>, 5> arr;
    for ( nat k = 0; k < 1; ++k )
    {
        for ( nat i = 0; i < arr.size(); ++i )
        {
            arr[i].a = rand();
            arr[i].b = rand();
        }
    }
    double x = getMinMaxParabola1d ( arr );
    double y = func ( x, arr );
    for ( int j = -50; j <= 50; ++j )
    {
        double t = 0.01 * j;
        double f = func ( t, arr );
//        display << t << f << NL;
        if ( f < y )
        display << x << t << y - f << NL;
    }
//    display << x << y << NL;
    display << "end" << NL;
}

inline double func2 ( double x, const Set2<double> & s )
{
    return x * s.a + s.b;
}

double func ( double x, CArrRef<MutCArrRef<Set2<double> > > arr )
{
    double sum = 0;
    for ( nat i = 0; i < arr.size(); ++i )
    {
        sum += func ( x, arr[i] );
    }
    return sum;
}

Def<double> getMinSumMinMaxParabola1d ( CArrRef<MutCArrRef<Set2<double> > > arr2 )
{
    const nat m = arr2.size();
    if ( ! m ) return Def<double>();
    if ( m == 1 ) return getMinMaxParabola1d ( arr2[0] );
    CmbSuite<FixArray<nat, 2>, 4> index2;
    index2.resize ( m );
    Set2<double> g;
    g.a = g.b = 0;
    for ( nat j = 0; j < m; ++j )
    {
         CCArrRef<Set2<double> > arr = arr2[j];
        if ( ! arr.size() ) return Def<double>();
        index2[j][0] = 0;
        g += arr[0];
    }
    g /= m;
    nat i2 = m;
    double x = -0.5 * g.a;
    double y = func ( x, g );
    for(;;)
    {
        for ( nat j = 0; j < m; ++j )
        {
            FixArray<nat, 2> & index = index2[j];
            nat im = index[0];
            CCArrRef<Set2<double> > arr = arr2[j];
            double max = func ( x, arr[im] );
            for ( nat i = 0; i < arr.size(); ++i )
            {
                const double t = func ( x, arr[i] );
                if ( max < t ) max = t, im = i;
            }
            if ( hasEqu ( index, im ) ) continue;
            const Set2<double> s0 = g;
            const Set2<double> si = g + ( arr[im] - arr[index[0]] ) / m;
            const double t = -0.5 * si.a;
            const double f = t * si.a + si.b;
            if ( i2 == m )
            {
                g = si;
                if ( t * s0.a + s0.b <= f )
                {
                    index[0] = im;
                    x = t;
                }
                else
                {
                    index[1] = im;
                    x = ( si.b - s0.b ) / ( s0.a - si.a );
                    i2 = j;
                }
            }
            else
            {
                
                if ( i2 != j )
                {
                    g = si;
                    FixArray<nat, 2> & ind2 = index2[i2];
                    if ( t * s0.a + s0.b <= f )
                    {
                        index[0] = im;
                        x = t;
                        i2 = m;
                    }
                    else
                    {
                        index[1] = im;
                        x = ( si.b - s0.b ) / ( s0.a - si.a );
                        i2 = j;
                    }
                    if ( func2 ( x, arr[ind2[0]] ) < func2 ( x, arr[ind2[1]] ) )
                    {
                        g += ( arr[ind2[1]] - arr[ind2[0]] ) / m;
                        ind2[0] = ind2[1];
                    }
                }
                else
                {
                    const Set2<double> s1 = g + ( arr[index[1]] - arr[index[0]] ) / m;
                    if ( t * s0.a + s0.b <= f && t * s1.a + s1.b <= f )
                    {
                        index[0] = im;
                        x = t;
                        i2 = m;
                        g = si;
                    }
                    else
                    {
                        if ( x + x + si.a > 0 == s1.a > s0.a )
                        {
                            index[1] = im;
                            x = ( si.b - s0.b ) / ( s0.a - si.a );
                        }
                        else
                        {
                            index[0] = im;
                            x = ( si.b - s1.b ) / ( s1.a - si.a );
                            g = si;
                        }
                    }
                }
            }
        }
        const double z = func ( x, g );
        const double z1 = func ( x, arr2 ) / m;
        if ( fabs ( z - z1 ) > 1e-9 )
            i2=i2;
        if ( z <= y )
        {
            for ( nat j = 0; j < m; ++j )
            {
                FixArray<nat, 2> & index = index2[j];
                nat im = index[0];
                CCArrRef<Set2<double> > arr = arr2[j];
                double max = func ( x, arr[im] );
                for ( nat i = 0; i < arr.size(); ++i )
                {
                    const double t = func ( x, arr[i] );
                    if ( max < t ) max = t, im = i;
                }
                if ( arr.size() == 2 )
                {
if ( fabs ( func ( x, arr[0] ) - func ( x, arr[1] ) ) > 1e-9 )
im = im;
                }
                if ( ! hasEqu ( index, im ) )
                j=j;
            }
            break;
        }
        y = z;
    }
    return x;
}

void getMinSumMinMax_test ()
{
    static PRand rand;
    const nat N = 3;
    FixArray2<Set2<double>, N, 5> arr;
    FixArray<MutCArrRef<Set2<double> >, N> arr2;
    double sum = 0;
    for ( nat k = 0; k < 10; ++k )
    {
        sum = 0;
        for ( nat i = 0; i < arr.size0(); ++i )
        {
            ArrRef<Set2<double> > ref = arr[i];
            for ( nat j = 0; j < arr.size1(); ++j )
            {
                ref[j].a = rand();
                ref[j].b = rand();
                sum += ref[j].a;
            }
            arr2[i].reset ( ref );
        }
    }
    sum /= N + N;
    double x = getMinSumMinMaxParabola1d ( arr2 );
    double y = func ( x, arr2 );
    for ( int j = -50; j <= 50; ++j )
    {
        double t = 0.01 * j;
        double f = func ( t, arr2 );
        display << t << f << NL;
//        if ( f < y )
//        display << x << t << y - f << NL;
    }
//    display << x << y << NL;
    display << "end" << NL;
}

} // end of namespace

void opti1d_test ()
{
//    getMinMax_test ();
    getMinSumMinMax_test ();
}
