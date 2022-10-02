
#include "math.h"
#include "../Shev/rand.h"
#include "../Shev/mathem.h"
#include "../Shev/func2d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/ShevArray.h"
#include "../Shev/moment2d.h"
#include "../Shev/tune.h"

#include "../draw.h"
#include "display.h"

double timeInSec();

ArrRef<Segment2d> randSegment2d ( ArrRef<Segment2d> p )
{
    static PRandPoint2d rand;
    for ( nat i = 0; i < p.size(); ++i )
    {
        p[i].a = rand();
        p[i].b = rand();
    }
    return p;
}

void momentum2pnt_test()
{
    static PRandPoint2d prand;
    FixArray<Vector2d, 4> point;
    point << prand;
    static PRand rand;
    FixArray<double, 4> mass;
    mass << rand;
/*    point[0] = Vector2d ( 0., -1. );
    point[1] = Vector2d ( 1., 0. );
    point[2] = Vector2d ( 0., 1. );
    point[3] = Vector2d ( -1., 0. );*/
    const Mom2d mom = momentum2pnt ( point, mass );
    double min = momentum2pnt ( point, mass, Line2d ( mom.maxA, -( mom.maxA * mom.o ) ) );
    double max = momentum2pnt ( point, mass, Line2d ( mom.minA, -( mom.minA * mom.o ) ) );
    display << mom.minM << mom.maxM << NL;
    display << min << max << NL << NL;
}

void momentum2sgm_test()
{
    const nat n = 5;
    FixArray<Segment2d, n> segm;
    const Mom2d mom = momentum2sgm ( randSegment2d ( segm ) );
    double min = momentum2sgm ( segm, Line2d ( mom.maxA, -( mom.maxA * mom.o ) ) );
    double max = momentum2sgm ( segm, Line2d ( mom.minA, -( mom.minA * mom.o ) ) );
    display << mom.minM - min << mom.maxM - max << NL;
//    display << minM << maxM << NL;
//    display << min << max << NL << NL;
}

void momentum2plg_test()
{
    const nat n = 5;
    FixArray<Vector2d, n> point;
    randPolygon ( point );
/*    for ( nat i = 0; i < n; ++i )
    {
        point[i].x *= - 1.;
    }*/
    Mom2d mom = momentum2plg ( point );
    double min = momentum2plg ( point, Line2d ( mom.maxA, -( mom.maxA * mom.o ) ) );
    double max = momentum2plg ( point, Line2d ( mom.minA, -( mom.minA * mom.o ) ) );
    display << mom.minM << mom.maxM << NL;
    display << min << max << NL << NL;
}

//**************************** 29.12.2007 *********************************//
//
//       ¬ычисление момента 2-го пор€дка относительно пр€мой
//
//**************************** 25.09.2010 *********************************//

double momentum4pnt ( CArrRef<Vector2d> point, const Line2d & line )
{
    double sum = 0.;
    for ( nat i = 0; i < point.size(); ++i ) sum += _pow4 ( line % point[i] );
    return sum;
}

double momentum4sgm ( CArrRef<Segment2d> segm, const Line2d & line )
{
    double sum = 0.;
    for ( nat i = 0; i < segm.size(); ++i )
    {
        const Segment2d & s = segm[i];
        const double len = norm2 ( s );
        const double a = line % s.a;
        const double b = line % s.b;
        sum += ( a * ( a + b ) + b * b ) * len;
    }
    return sum / 3.;
}

double momentum4pnt ( CArrRef<Vector2d> point, const Vector2d & norm )
{
    double a = 0, b = 0, c = 0;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const double t = norm * point[i];
        a += t;
        b += t * t;
        c += t * t * t;
    }
    a /= point.size();
    b /= point.size();
    c /= point.size();
    double x[3];
    nat k = root3 ( 3*a, 3*b, c, x );
    return x[0];
}

void momentum4pnt_test()
{
    const nat n = 7;
    static PRandPoint2d rand;
    DynArray<Vector2d> point ( n );
    point << rand;
    drawPoints ( point, 0, 1, 1 );
    drawCross ( centerPnt ( point ), 1, 0, 0 );;
    nat i;
    Vector2d o;
    o.x = - momentum4pnt ( point, Vector2d ( 1, 0 ) );
    o.y = - momentum4pnt ( point, Vector2d ( 0, 1 ) );
    drawCross ( o, 1, 1, 0 );
    const nat m = 180;
    for ( i = 0; i < m; ++i )
    {
        double f = i * M_PI / m;
        Vector2d norm ( cos ( f ), sin ( f ) );
        double d = momentum4pnt ( point, norm );
//        display << i << momentum4pnt ( point, Line2d ( norm, d ) ) << NL;
    }
}

double func1 ( nat n, const Vector2d * p, Vector2d & o )
{
    double sum = 0;
    for ( nat i = 0; i < n; ++i ) sum += norm2 ( p[i] - o );
    return sum;
}

double func3 ( nat n, const Vector2d * p, Vector2d & o )
{
    double sum = 0;
    for ( nat i = 0; i < n; ++i ) sum += _pow3 ( norm2 ( p[i] - o ) );
    return sum;
}

double func4 ( nat n, const Vector2d * p, Vector2d & o )
{
    double sum = 0;
    for ( nat i = 0; i < n; ++i ) sum += _pow2 ( qmod ( p[i] - o ) );
    return sum;
}

Def<Vector2d> centerPnt3p ( CCArrRef<Vector2d> & point, double a )
{
    const nat n = point.size();
    if ( ! n ) return Def<Vector2d>();
    nat i;
    Vector2d o ( 0, 0 );
    for ( nat i = 0; i < n; ++i ) o += point[i];
    o /= n;
    for ( i = 0; i < 6; ++i )
    {
        Vector2d o1 ( 0, 0 );
        double c = 0;
        for ( nat j = 0; j < n; ++j )
        {
            double d = norm2 ( point[j] - o );
            o1 += d * ( a * point[j] + (1-a) * o );
            c += d;
        }
        if ( c ) o1 /= c;
        o = o1;
    }
    return o;
}

Def<Vector2d> center3pnt ( CCArrRef<Vector2d> & point )
{
    const nat n = point.size();
    if ( ! n ) return Def<Vector2d>();
    nat i;
    Vector2d o ( 0, 0 );
    for ( i = 0; i < n; ++i ) o += point[i];
    o /= n;
    for ( i = 0; i < 19; ++i )
    {
        Vector2d o1 ( 0, 0 );
        double c = 0;
        for ( nat j = 0; j < n; ++j )
        {
            double d = norm2 ( point[j] - o );
            o1 += d * ( 2 * point[j] + o );
            c += d;
        }
        if ( c ) o1 /= 3 * c;
        if ( i > 10 && norm2 (o-o1) < 1e-4 )
        {
            display << i << NL; break;
        }
//display << i << norm2 ( o - o1 ) << NL;
        o = o1;
    }
    return o;
}

Def<Vector2d> center4pnt ( CCArrRef<Vector2d> & point, double a )
{
    const nat n = point.size();
    if ( ! n ) return Def<Vector2d>();
    nat i;
    Vector2d o ( 0, 0 );
    for ( i = 0; i < n; ++i ) o += point[i];
    o /= n;
    for ( i = 0; i < 6; ++i )
    {
        Vector2d o1 ( 0, 0 );
        double c = 0;
        for ( nat j = 0; j < n; ++j )
        {
            double d = qmod ( point[j] - o );
            o1 += d * ( a * point[j] + (1-a) * o );
            c += d;
        }
        if ( c ) o1 /= c;
 //display << i << norm2 ( o- o1 ) << NL;
        if ( norm2 ( o- o1 ) < 1e-4 ) break;
        o = o1;
    }
    if(i>9 )
        i=i;
    return o;
}

void testmom ()
{
    nat i;
    static PRand rand;
    const nat n = 9;
    FixArray<Vector2d, n> p;
    /*p[0] = Vector2d ( 0, 0 );
    p[1] = Vector2d ( 1, 0 );
    p[2] = Vector2d ( 0, 1 );
    p[3] = 
    p[4] = 
    p[5] = Vector2d ( 100, 100 );*/
    //Vector2d o = centerPnt4p ( p );
    //double f = func4 ( n, p(), o );
    for ( nat j = 0; j < 100; ++j )
    {
        for ( i = 0; i < n; ++i )
        {
            p[i].x = 9*rand();
            p[i].y = 6*rand();
        }
        if ( j == 54 )
        {
            Vector2d o = center4pnt ( p );
            drawPoint ( o, 1, 0, 0 );
            drawPoints  ( p, 0, 1, 1 );
        }
 //break;
        /*double f = func4 ( n, p(), o );
        double f1 = func4 ( n, p(), Vector2d ( o.x+1e-4, o.y ) ) - f;
        double f2 = func4 ( n, p(), Vector2d ( o.x, o.y+1e-4 ) ) - f;
        double f3 = func4 ( n, p(), Vector2d ( o.x-1e-4, o.y ) ) - f;
        double f4 = func4 ( n, p(), Vector2d ( o.x, o.y-1e-4 ) ) - f;
        display << f1 << f2 << f3 << f4 << NL;
        /*
        double a = 1.5 + i*0.01;
        Vector2d o1 = centerPnt4p ( p, a );
        display << a << func1 ( n, p(), o1 ) - f << NL;*/
    }
    display << NL;
}


void momentum2d_test()
{
    drawNewList2d ();
//    momentum2pnt_test();
    testmom ();
//    momentum4pnt_test();
    endNewList();
}