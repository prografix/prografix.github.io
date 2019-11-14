
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

void momentum2d_test()
{
    drawNewList2d ();
//    momentum2pnt_test();
    momentum4pnt_test();
    endNewList();
}