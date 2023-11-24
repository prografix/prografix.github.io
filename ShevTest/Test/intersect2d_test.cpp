
#include "math.h"

#include "../draw.h"

#include "../Shev/rand.h"
#include "../Shev/func2d.h"
#include "../Shev/opti2d.h"
#include "../Shev/trian2d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/ShevArray.h"
#include "../Shev/intersect2d.h"

#include "display.h"

double timeInSec ();

bool isIntersect ( const Vector2d & a, const Vector2d & b, const Vector2d & c, const Vector2d & d )
{
    const Vector2d v = b - a;
    const Vector2d u = d - c;
    const double p = v % u;
    if ( p == 0 ) return false;
    const Vector2d w = a - c;
    const double s = ( v % w ) / p;
    const double t = ( u % w ) / p;
    return s >= 0. && s <= 1. && t >= 0. && t <= 1.;
}

bool isIntersect ( ArrRef<Vector2d> poly )
{
    nat i;
    for ( i = 3; i < poly.size(); ++i )
    {
        for ( nat j = 0; j < i - 2; ++j )
        {
            if ( isIntersect ( poly[j], poly[j+1], poly[i-1], poly[i] ) ) return true;
        }
    }
    for ( i = 1; i < poly.size() - 2; ++i )
    {
        if ( isIntersect ( poly[i], poly[i+1], poly.las(), poly[0] ) ) return true;
    }
     return false;
}

namespace
{

void intersectPointPolygon()
{
    FixArray<Vector2d, 9> vert;
    randPolygon ( vert );
    drawPolygon ( vert, 0, 1, 1 );
    static QRandPoint2d prand;
    for ( nat i = 0; i < 9; ++i )
    {
        const Vector2d p ( prand() );
        if ( isIntersect ( p, vert ) )
            drawPoint ( p, 1, 1, 0 );
        else
            drawPoint ( p, 1, 0, 0 );
    }
}

void intersectPointPolygon2()
{
    FixArray<Vector2d, 9> vert;
    static QRandPoint2d prand;
    for ( nat i = 0; i < 1999; ++i )
    {
m1:     randPolygon ( vert );
        nat k;
        for ( k = 0; k < vert.size(); ++k )
        {
            Vector2d & v = vert[k];
            v.x = int ( 128 * v.x ) / 128.;
            v.y = int ( 128 * v.y ) / 128.;
        }
        for ( k = 0; k < vert.size(); ++k )
        {
            nat k1 = k > 0 ? k - 1 : vert.size() - 1;
            nat k2 = k1 > 0 ? k1 - 1 : vert.size() - 1;
            if ( fabs ( ( vert[k2] - vert[k] ) % ( vert[k1] - vert[k] ) ) < 1e-2 )
                goto m1;
        }
        if ( isIntersect ( vert ) ) goto m1;
        for ( nat j = 0; j < 9999; ++j )
        {
            Vector2d p ( prand() );
            p.x = int ( 128 * p.x ) / 128.;
            p.y = int ( 128 * p.y ) / 128.;
            if ( isIntersect ( p, vert ) )
            {
                double dist;
                distance ( vert, p, dist );
                if ( dist > 0 )
                {
                    display << "err1" << i << j << NL;
                    isIntersect ( p, vert );
                    for ( k = 0; k < vert.size(); ++k ) display << 1000*vert[k] << NL;
                    display << 1000*p << NL;
                    /*for ( k = 0; k < vert.size(); ++k )
                    {
                        nat k1 = k > 0 ? k - 1 : vert.size() - 1;
                        nat k2 = k1 > 0 ? k1 - 1 : vert.size() - 1;
                        display <<  ( vert[k2] - vert[k] ) % ( vert[k1] - vert[k] ) << NL;
                    }*/
                    return;
                }
            }
            else
            {
                double dist;
                distance ( vert, p, dist );
                if ( dist <= 0 )
                {
                    display << "err2" << i << j << NL;
/*                    isIntersect ( p, vert );
                    for ( k = 0; k < vert.size(); ++k ) display << 1000*vert[k] << NL;
                    display << 1000*p << NL;
                    /*for ( k = 0; k < vert.size(); ++k )
                    {
                        nat k1 = k > 0 ? k - 1 : vert.size() - 1;
                        nat k2 = k1 > 0 ? k1 - 1 : vert.size() - 1;
                        display <<  ( vert[k2] - vert[k] ) % ( vert[k1] - vert[k] ) << NL;
                    }*
                    return;*/
                }
            }
        }
    }
     display << "end" << NL;
}

void intersectLineSegment1()
{
    static PRandPoint2d rand;
    const Segment2d seg ( rand(), rand() );
    const Segment2d tmp ( rand(), rand() );
    const Line2d line ( tmp.a, tmp.b );
    const Def<Vector2d> p = intersection ( line, seg );
    draw ( seg, 1, 0, 0 );
    draw ( tmp, 1, 1, 0 );
    if ( p.isDef )
    {
        display << line % p << NL;
        drawPoint ( p, 0, 1, 1 );
    }
}

void intersectLineSegment()
{
    static PRandPoint2d rand;
    nat nn = 0, n0 = 0;
    for ( nat i = 0; i < 10000; ++i )
    {
        const Segment2d seg ( rand(), rand() );
        const Segment2d tmp ( rand(), rand() );
        const Line2d line ( tmp.a, tmp.b );
        const Def<Vector2d> p = intersection ( line, seg );
        if ( p.isDef )
        {
            ++nn;
            const double d = line % p;
            if ( d == 0 ) ++n0;
        }
    }
    display << nn << n0 << NL;
}

void intersectTwoSegments()
{
    static PRandPoint2d rand;
    const Segment2d s1 ( rand(), rand() );
    const Segment2d s2 ( rand(), rand() );
    const Def<Vector2d> p = intersection ( s1, s2 );
    draw ( s1, 1, 0, 0 );
    draw ( s2, 1, 1, 0 );
    if ( p.isDef )
    {
        drawPoint ( p, 0, 1, 1 );
    }
}

void cutLineSegment()
{
    static PRandPoint2d rand;
    nat nn = 0;
    for ( nat i = 0; i < 1000; ++i )
    {
        const Segment2d seg ( rand(), rand() );
        const Segment2d tmp ( rand(), rand() );
        const Line2d line ( tmp.a, tmp.b );
        const Def<Segment2d> res = cut ( line, seg );
        if ( res.isDef )
        {
            const double da = line % seg.a;
            const double db = line % seg.b;
            const double max = _max ( da, db );
            if ( max > 1e-16 )
                display << max << NL;
            ++nn;
        }
    }
    display << nn << NL;
}

void intersectLineCircle()
{
    static PRand rand;
    static PRandPoint2d prand;
    nat nn = 0, n0 = 0;
    for ( nat i = 0; i < 10000; ++i )
    {
        const Circle2d cir ( prand(), rand() );
        const Segment2d tmp ( prand(), prand() );
        const Line2d line ( tmp.a, tmp.b );
        const Def<Segment2d> seg = intersection ( line, cir );
        if ( seg.isDef )
        {
            const double da = fabs ( line % seg.a );
            const double db = fabs ( line % seg.b );
            const double ca = fabs ( norm2 ( seg.a - cir.o ) - cir.r );
            const double cb = fabs ( norm2 ( seg.b - cir.o ) - cir.r );
            const double e = 5e-16;
            if ( ca > e || cb > e || da > e || db > e )
                display << ca << cb << da << db << NL;
            ++n0;
        }
        ++nn;
    }
    display << nn << n0 << NL;
}

void intersectSegmentCircle()
{
    static PRand rand;
    static PRandPoint2d prand;
    const Circle2d cir ( prand(), rand() );
    const Segment2d seg ( 1.5*prand(), 1.5*prand() );
    draw ( seg, 1, 0, 0 );
    draw ( cir, 0, 1, 1 );
    const Def<Segment2d> res = intersection ( seg, cir );
    if ( res.isDef )
    {
        draw ( res, 1, 1, 0 );
        const double ca = norm2 ( res.a - cir.o ) - cir.r;
        const double cb = norm2 ( res.b - cir.o ) - cir.r;
        const double e = 5e-16;
        if ( ca > e || cb > e )
            display << ca << cb << NL;
    }
}

void intersectLineEllipse()
{
    static PRand rand;
    static PRandPoint2d prand;
    nat nn = 0, n0 = 0;
    for ( nat i = 0; i < 1000; ++i )
    {
        Ellipse2d e;
        e.a = 0.1 + rand();
        e.b = 0.1 + rand();
        e.spin = Spin2d ( rand() );
        e.o = prand();
        const Segment2d tmp ( prand(), prand() );
        const Line2d line ( tmp.a, tmp.b );
        const Def<Segment2d> seg = intersection ( line, e );
        if ( seg.isDef )
        {
            const double da = fabs ( line % seg.a );
            const double db = fabs ( line % seg.b );
            const double e = 5e-16;
            if ( da > e || db > e )
                display << da << db << NL;
            ++n0;
        }
        ++nn;
    }
    display << nn << n0 << NL;
}

void intersectSegmentEllipse()
{
    static PRand rand;
    static PRandPoint2d prand;
    Ellipse2d e;
    e.a = 0.1 + rand();
    e.b = 0.1 + rand();
    e.spin = Spin2d ( rand() );
    e.o = prand();
    const Segment2d seg ( 1.5*prand(), 1.5*prand() );
    draw ( seg, 1, 0, 0 );
    draw ( e, 0, 1, 1 );
    const Def<Segment2d> res = intersection ( seg, e );
    if ( res.isDef )
    {
        draw ( res, 1, 1, 0 );
    }
}

void intersectLinePolygon()
{
    FixArray<Vector2d, 9> vert;
    randPolygon ( vert );
    drawPolygon ( vert, 0, 1, 1 );
    FixArray<Vector2d,4> quad;
    quad[0] = Vector2d ( 2, 2 );
    quad[1] = Vector2d (-2, 2 );
    quad[2] = Vector2d (-2,-2 );
    quad[3] = Vector2d ( 2,-2 );
    static PRandPoint2d prand;
    const Segment2d tmp ( prand(), prand() );
    const Line2d line ( tmp.a, tmp.b );
    DynArray<Segment2d> seg;
    intersection ( quad, line, seg );
    nat i;
    for ( i = 0; i < seg.size(); ++i ) draw ( seg[i], 1, 1, 0 );
    intersection ( vert, line, seg );
    for ( i = 0; i < seg.size(); ++i ) draw ( seg[i], 1, 0, 1 );
}

void intersectSegmentPolygon()
{
    FixArray<Vector2d, 9> vert;
    randPolygon ( vert );
    vert.reverse();
    drawPolygon ( vert, 0, 1, 1 );
    static PRandPoint2d prand;
    const Segment2d seg ( prand(), prand() );
    draw ( seg, 1, 1, 0 );
    Suite<Segment2d> res;
    intersection ( vert, seg, res );
    for ( nat i = 0; i < res.size(); ++i ) draw ( res[i], 1, 0, 1 );
}

void cutLinePolygon()
{
    FixArray<Vector2d, 10> vert;
    FixArray<Vector2d,4> quad;
    quad[0] = Vector2d ( 2, 2 );
    quad[1] = Vector2d (-2, 2 );
    quad[2] = Vector2d (-2,-2 );
    quad[3] = Vector2d ( 2,-2 );
    static PRandPoint2d prand;
    Vector2d a, b;
//    for ( nat j = 0; j < 15; ++j )
    {
        randPolygon ( vert );
        a = prand();
        b = prand();
    }
    vert.reverse();
    drawPolygon ( vert, 0, 1, 1 );
    const Line2d line ( a, b );
    DynArray<Segment2d> seg;
    if ( intersection ( quad, line, seg ).size() > 0 ) draw ( seg[0], 1, 1, 0 );
    Suite< Suite<Vector2d> > res;
    cut ( vert, line, res );
    for ( nat i = 0; i < res.size(); ++i )
    {
        drawPolygon ( res[i], 1, 0, 1 );
    }
}

void cutLinePolygon2()
{
    static PRandPoint2d prand;
    FixArray<Vector2d, 12> poly;
    Suite< Suite<Vector2d> > res; 
    for ( nat j = 0; j < 1000; ++j )
    {
        randPolygon ( poly );
        const Line2d line ( prand(), prand() );
        const double ax = fabs ( line.norm.x );
        const double ay = fabs ( line.norm.y );
        const bool xgy = ax > ay;
        double kx, ky, kd;
        if ( xgy )
        {
            kx = line.norm.x > 0 ? 1 : -1;
            ky = line.norm.y / ax;
            kd = line.dist / ax;
        }
        else
        {
            kx = line.norm.x / ay;
            ky = line.norm.y > 0 ? 1 : -1;
            kd = line.dist / ay;
        }
        cut ( poly, line, res );
        for ( nat k = 0; k < res.size(); ++k )
        {
            Suite<Vector2d> & p = res[k];
            if ( xgy )
            {
                for ( nat i = 0; i < p.size(); ++i )
                {
                    double t = ky * p[i].y + kd;
                    double d = kx * p[i].x + t;
                    if ( d > 0 ) display << j << "d > 0" << NL;
                }
            }
            else
            {
                for ( nat i = 0; i < p.size(); ++i )
                {
                    double t = kx * p[i].x + kd;
                    double d = ky * p[i].y + t;
                    if ( d > 0 ) display << j << "d > 0" << NL;
                }
            }
        }
    }
    display << "end" << NL;
}

void intersect1c_test()
{
    Suite<Vector2d> conv ( 9, 9 );
    FixArray<Vector2d, 12> poly;
    randConvexPolygon ( conv );
    randPolygon ( poly );
    drawPolygon ( conv, 0.4f, 0.8f, 0.8f );
    drawPolygon ( poly, 0.8f, 0.4f, 0.8f );
    Suite< Suite<Vector2d> > res; 
    intersect1c ( conv, poly, res );
    for ( nat i = 0; i < res.size(); ++i )
    {
        drawPolygon ( res[i], 1, 1, 0 );
    }
}

Def<Vector2d> getInsidePoint ( CArrRef<Vector2d> poly )
{
    Def<Vector2d> res;
    const nat n = poly.size();
    if ( n < 3 ) return res;
    nat i, im = 0;
    double max = qmod ( poly.las() - poly[0] );
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( poly[i] - poly[i-1] );
        if ( max < t ) max = t, im = i;
    }
    for ( i = 1; i < n; ++i )
    {
        nat j = im + i;
        if ( j >= n ) j -= n;
    }
    return res;
}

void intersectPolygons()
{
    FixArray<Vector2d, 5> poly1;
    poly1[0] = Vector2d(1,-1);
    poly1[1] = Vector2d(0,0);
    poly1[2] = Vector2d(-0.5,2);
    poly1[3] = Vector2d(-1,0);
    poly1[4] = Vector2d(-2,-1);
    poly1 *= 0.5;
    FixArray<Vector2d, 6> poly2;
    poly2[0] = Vector2d(-1,0);
    poly2[1] = Vector2d(-0.5,-1);
    poly2[2] = Vector2d(0,0);
    poly2[3] = Vector2d(0,2);
    poly2[4] = Vector2d(-0.5,1);
    poly2[5] = Vector2d(-1,2);
    poly2 *= 0.5;
    nat i;
 /*   for ( i = 0; i < 1; ++i )
    {
        randPolygon ( poly1 );
        randPolygon ( poly2 );
    }
    poly1.reverse();
    poly2.reverse();*/
    drawPolygon ( poly1, 0.4f, 0.8f, 0.8f );
    drawPolygon ( poly2, 0.8f, 0.4f, 0.8f );
    Suite< Suite<Vector2d> > res; 
    intersection ( poly1, poly2, res );
    for ( i = 0; i < res.size(); ++i )
    {
        drawPolygon ( res[i], 1, 1, 0 );
    }
}

void intersectPolygons2()
{
    FixArray<Vector2d, 7> poly1;
    FixArray<Vector2d, 7> poly2;
    nat i, j;
    for ( i = 0; i < 5000; ++i )
    {
        randPolygon ( poly1 );
        randPolygon ( poly2 );
//poly1 = poly2;
        double a1 = area ( poly1 );
        double a2 = area ( poly2 );
        Suite< Suite<Vector2d> > res; 
        if ( ! intersection ( poly1, poly2, res ) )
        {
            intersection ( poly1, poly2, res );
            display << i << "err" << NL;
            continue;
        }
        double a3 = 0;
        for ( j = 0; j < res.size(); ++j ) a3 += area ( res[j] );
        poly1.reverse();
        poly2.reverse();
        if ( ! intersection ( poly1, poly2, res ) )
        {
            display << i << "err" << NL;
            continue;
        }
        double a4 = 0;
        for ( j = 0; j < res.size(); ++j ) a4 += area ( res[j] );
        double t = a1 + a2 - a3 + a4;
        if ( fabs ( t ) > 1e-9 )
        {
            display << i << t << NL;
            drawPolygon ( poly1, 0.4f, 0.8f, 0.8f );
            drawPolygon ( poly2, 0.8f, 0.4f, 0.8f );
            intersection ( poly1, poly2, res );
            for ( j = 0; j < res.size(); ++j )
            {
                drawPolygon ( res[j], 1, 1, 0 );
            }
            break;
        }
        poly1.reverse();
        intersection ( poly1, poly2, res );
        double a5 = 0;
        for ( j = 0; j < res.size(); ++j ) a5 += area ( res[j] );
        t = a1 - a3 - a5;
        if ( fabs ( t ) > 1e-9 )
        {
            display << i << t << NL;
            drawPolygon ( poly1, 0.4f, 0.8f, 0.8f );
            drawPolygon ( poly2, 0.8f, 0.4f, 0.8f );
            intersection ( poly1, poly2, res );
            for ( j = 0; j < res.size(); ++j )
            {
                drawPolygon ( res[j], 1, 1, 0 );
            }
            break;
        }
        poly1.reverse();
        poly2.reverse();
        intersection ( poly1, poly2, res );
        double a6 = 0;
        for ( j = 0; j < res.size(); ++j ) a6 += area ( res[j] );
        t = a2 - a3 - a6;
        if ( fabs ( t ) > 1e-9 )
        {
            display << i << t << NL;
            drawPolygon ( poly1, 0.4f, 0.8f, 0.8f );
            drawPolygon ( poly2, 0.8f, 0.4f, 0.8f );
            intersection ( poly1, poly2, res );
            for ( j = 0; j < res.size(); ++j )
            {
                drawPolygon ( res[j], 1, 1, 0 );
            }
            break;
        }
    }
    display << "end" << NL;
}

void intersectHalfPlanes()
{
    Suite<Vector2d> poly1, poly2;
    poly1.resize ( 7 );
    poly2.resize ( 7 );
    randConvexPolygon ( poly1 );
    randConvexPolygon ( poly2 );
    drawPolygon ( poly1, 0, 1, 1 );
    drawPolygon ( poly2, 0, 1, 1 );
    DynArray<Line2d> line1 ( poly1.size() ), line2 ( poly2.size() );
    if ( ! points2lines ( poly1, line1 ) )
    {
        display << "! points2lines" << NL;
        return;
    }
    if ( ! points2lines ( poly2, line2 ) )
    {
        display << "! points2lines" << NL;
        return;
    }
    nat i;
    Suite<const Line2d *> line;
    for ( i = 0; i < line1.size(); ++i ) line.inc() = line1(i);
    for ( i = 0; i < line2.size(); ++i ) line.inc() = line2(i);
    for ( i = 0; i < line1.size(); ++i ) line.inc() = line1(i);
    for ( i = 0; i < line2.size(); ++i ) line.inc() = line2(i);
    Suite<Vector2d> poly;
    if ( ! intersectHalfPlanes ( line, poly ) )
    {
        display << "! intersectHalfPlanes" << NL;
        return;
    }
    drawPolygon ( poly, 1, 1, 0 );
}

} // end of namespace

void intersect2d_test ()
{
    drawNewList2d();
//    intersectPointPolygon2();
//    cutLineSegment();
//    intersectLineSegment();
//    intersectTwoSegments();
//    intersectLineCircle();
//    intersectSegmentCircle();
//    intersectLineEllipse();
//    intersectSegmentEllipse();
//    intersectLinePolygon();
//    intersectSegmentPolygon();
//    cutLinePolygon2();
//    intersect1c_test();
//    intersectPolygons();
    intersectHalfPlanes();
    endNewList();
}