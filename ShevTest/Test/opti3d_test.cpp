
#include <windows.h>
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/rand.h"
#include "../Shev/func2d.h"
#include "../Shev/func3d.h"
#include "../Shev/opti2d.h"
#include "../Shev/opti3d.h"
#include "../Shev/Mathem.h"
#include "../Shev/LinAlg.h"
#include "../Shev/approx3d.h"
#include "../Shev/Vector3d.h"
#include "../Shev/Vector4d.h"
#include "../Shev/ShevArray.h"
#include "../Shev/ShevArray2.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/intersect3d.h"
#include "../Shev/Facet2Model.h"
#include "../Shev/WireModel.h"
#include "../Shev/moment3d.h"
#include "../Shev/RealFile.h"
#include "../Shev/filePYH.h"
#include "../Shev/trans.h"
#include "../Shev/HeapAndTree.h"
#include "PolyhedronErrorRecipient.h"
#include "../draw.h"

#include "display.h"
double time1=0, time2=0, time3=0, time4=0;

double timeInSec();
bool maxConvexPolyhedronInPolyhedronNR ( const Polyhedron & inner, const Polyhedron & outer, 
                                         CArrRef<Vector3d> point, Conform3d & res );
Def<Conform3d> maxPolyhedronInConvexPolyhedronNR ( const Vector4d & v0, double eps, CArrRef<Plane4d> plane );

inline 
void _swap ( SortItem<double, SortItemPtr<SortItem<Set2<const Vertex<9> *>, Double<9> > > > & p1, 
             SortItem<double, SortItemPtr<SortItem<Set2<const Vertex<9> *>, Double<9> > > > & p2 )
{
    _swap ( p1.head, p2.head );
    _swap ( p1.tail, p2.tail );
    _swap ( p1.tail.ptr->tail, p2.tail.ptr->tail );
}

#include "../Shev/approx2d.h"
#include "../Shev/func1t.h"

struct LinePart : public Segment2d
{
    double cosa, sina;
};

void findBigPeaks ( double lvl, CArrRef<double> d, Suite<nat> & res )
{
    res.resize();
    const nat n = d.size();
    Suite<SortItem<double, nat> > peak;
    for ( nat i = 0; i < n; ++i )
    {
        if ( d[i] < lvl ) continue;
        const nat i1 = i ? i - 1 : n - 1;
        const nat i2 = i < n - 1 ? i + 1 : 0;
        if ( d[i] > d[i1] && d[i] >= d[i2] )
        {
            peak.inc() = SortItem<double, nat> ( d[i], i );
        }
    }
    const nat n2 = n / 2;
    const nat m = n / 95 + 1;
    insertSort321 ( peak );
    for ( nat j = 0; j < peak.size(); ++j )
    {
        const SortItem<double, nat> & si = peak[j];
        for ( nat l = 0; l < res.size(); ++l )
        {
            nat i = res[l] > si.tail ? res[l] - si.tail : si.tail - res[l];
            if ( i > n2 ) i = n - i;
            if ( i <= m ) goto m1;
        }
        res.inc() = si.tail;
m1:;}
    insertSort123 ( res );
}

double getHighLevel ( CArrRef<double> dev )
{
// Находим 3 максимальных пика
    double array[3] = { 0., 0., 0. };
    const nat nc = dev.size();
    for ( nat i = 0; i < nc; ++i )
    {
        const nat i1 = i > 0 ? i - 1 : nc - 1;
        const nat i2 = i < nc - 1 ? i + 1 : 0;
        const double t = dev[i];
        if ( t > dev[i1] && t >= dev[i2] )
        {
            if ( t > array[0] )
            {
                array[2] = array[1];
                array[1] = array[0];
                array[0] = t;
            }
            else
            if ( t > array[1] )
            {
                array[2] = array[1];
                array[1] = t;
            }
            else
            if ( t > array[2] )
            {
                array[2] = t;
            }
        }
    }
    return array[2];
}

struct SegGroup
{
    Vector2d par;
    nat a, b;
};

bool calcProjectionGroups ( CArrRef<LinePart> layer, DynArrRef<SegGroup> & group )
{
    const nat nc = layer.size();
    if ( nc < 180 ) return false;
    const nat k = nc / 90;
    DynArray<Vector2d> v2 ( nc );
    DynArray<double> dev ( nc );
    Suite<Line2d> line;
    Suite<double> mass;
    line.resize ( k );
    mass.resize ( k );
    nat i;
    for ( i = 0; i < nc; ++i )
    {
        for ( nat j = 0; j < k; ++j )
        {
            nat l = i + j;
            if ( l >= nc ) l -= nc;
            const LinePart & part = layer[l];
            line[j].norm.x = part.cosa;
            line[j].norm.y = part.sina;
            line[j].dist = ( part.b.x - part.a.x ) / ( part.b.y - part.a.y );
        }
        v2[i] = getNearPoint2 ( line, mass );
    }
    for ( i = 0; i < nc; ++i )
    {
        dev[i] = qmod ( v2[i] - v2[i>=k?i-k:i-k+nc] );
//display << dev[i] << NL;
    }
    Suite<nat> peak;
    double highLevel = 0.125 * getHighLevel ( dev );
    findBigPeaks ( highLevel, dev, peak );
    group.resize ( peak.size() );
    for ( i = 0; i < peak.size(); ++i )
    {
        SegGroup & g = group[i];
        g.a = peak[i];
        g.b = (peak.cnext(i)+nc-1) % nc;
        const nat n = g.b > g.a ? g.b - g.a : g.b + nc - g.a;
        line.resize ( n );
        mass.resize ( n );
        for ( nat j = 0; j < n; ++j )
        {
            nat l = j + g.a;
            if ( l >= nc ) l -= nc;
            const LinePart & part = layer[l];
            line[j].norm.x = part.cosa;
            line[j].norm.y = part.sina;
            line[j].dist = ( part.b.x - part.a.x ) / ( part.b.y - part.a.y );
        }
        g.par = getNearPointR ( line, mass );
    }/*
    for ( i = 0; i < peak.size(); ++i )
    {
        SegGroup & g = group[i];
        const nat n = g.b > g.a ? g.b - g.a : g.b + nc - g.a;
        for ( nat j = 0; j < n; ++j )
        {
            nat l = j + g.a;
            if ( l >= nc ) l -= nc;
            const LinePart & part = layer[l];
            display << - g.par.x * part.cosa - g.par.y * part.sina <<
                ( part.b.x - part.a.x ) / ( part.b.y - part.a.y ) << NL;
        }
        display << NL;
    }*/
    return true;
}

void makeEdges ( CArrRef<LinePart> layer, CArrRef<SegGroup> group, DynArrRef<Segment3d> & edge )
{
    const nat nc = layer.size();
    const nat ng = group.size();
    edge.resize ( ng );
    Suite<Line2d> line;
    Suite<double> mass;
    nat i, j;
    for ( i = 0; i < ng; ++i )
    {
        const SegGroup & g = group[i];
        Segment3d & e = edge[i];
        const nat n = g.b > g.a ? g.b - g.a : g.b + nc - g.a;
        line.resize ( n );
        mass.resize ( n );
        e.a.z = 0;
        for ( j = 0; j < n; ++j )
        {
            nat l = j + g.a;
            if ( l >= nc ) l -= nc;
            const LinePart & part = layer[l];
            line[j].norm.x = part.cosa;
            line[j].norm.y = part.sina;
            line[j].dist = - part.a.x;
            e.a.z += part.a.y;
        }
        (Vector2d &) e.a = getNearPointR ( line, mass );
        e.a.z /= n;
        e.b.z = 0;
        for ( j = 0; j < n; ++j )
        {
            nat l = j + g.a;
            if ( l >= nc ) l -= nc;
            const LinePart & part = layer[l];
            line[j].dist = - part.b.x;
            e.b.z += part.b.y;
        }
        (Vector2d &) e.b = getNearPointR ( line, mass );
        e.b.z /= n;
        for ( j = 0; j < n; ++j )
        {
            nat l = j + g.a;
            if ( l >= nc ) l -= nc;
            const LinePart & part = layer[l];
            display << e.a.x * part.cosa + e.a.y * part.sina << part.a.x;
            display << e.b.x * part.cosa + e.b.y * part.sina << part.b.x << NL;
        }
        display << NL;
    }
}

void load ( IReadFile & file, Suite<LinePart> & layer )
{
    char c;
    double ax, ay, bx, by;
    for (;;)
    {
        
        if ( ! readFltDec ( file, c, ax ) || 
             ! readFltDec ( file, c, ay ) || 
             ! readFltDec ( file, c, bx ) || 
             ! readFltDec ( file, c, by ) ) return;
        LinePart & part = layer.inc();
        part.a.x = ax;
        part.a.y = ay;
        part.b.x = bx;
        part.b.y = by;
		while ( c != '\n' && file.getc ( &c ) ) continue; // доходим до конца строки
    }
}

void proba ()
{
    const char * filename = "D:/temp/rewrite/layer_segmets_1.txt";
    RealFile file ( filename, "r" );
    Suite<LinePart> layer;
    load ( file, layer );
    const nat nc = layer.size();
    const double step = M_2PI / nc;
    for ( nat i = 0; i < nc; ++i )
    {
        LinePart & part = layer[i];
        part.cosa = cos ( i * step );
        part.sina = sin ( i * step );
    }
    Suite<SegGroup> group;
    calcProjectionGroups ( layer, group );
    Suite<Segment3d> edge;
    makeEdges ( layer, group, edge );
}
namespace
{

Sphere3d minSphereAroundPoints1 ( CArrRef<Vector3d> points )
{
    nat n = points.size();
    if ( n == 0 ) return Sphere3d ();
    if ( n == 1 ) return Sphere3d ( 0, points[0] );
    if ( n == 2 )
    {
        const Vector3d o = 0.5 * ( points[0] + points[1] );
        return Sphere3d ( norm2 ( points[0] - o ), o );
    }
    DynArray<const Vector3d *> p ( n );
    for ( nat i = 0; i < n; ++i ) p[i] = & points[i];
    shuffle ( p, PRand() );
    Vector3d o = 0.5 * ( *p[0] + *p[1] );
    double q = qmod ( *p[0] - o );
    for ( nat i1 = 2; i1 < n; ++i1 )
    {
        const Vector3d & v1 = *p[i1];
        if ( qmod ( v1 - o ) <= q ) continue;
        o = 0.5 * ( v1 + *p[0] );
        q = qmod ( v1 - o );
        for ( nat i2 = 1; i2 < i1; ++i2 )
        {
            const Vector3d & v2 = *p[i2];
            if ( qmod ( v2 - o ) <= q ) continue;
            o = 0.5 * ( v1 + v2 );
            q = qmod ( v1 - o );
            for ( nat i3 = 0; i3 < i2; ++i3 )
            {
                const Vector3d & v3 = *p[i3];
                if ( qmod ( v3 - o ) <= q ) continue;
                getSphere ( v1, v2, v3, o, q );
                for ( nat i4 = 0; i4 < i3; ++i4 )
                {
                    const Vector3d & v4 = *p[i4];
                    if ( qmod ( v4 - o ) <= q ) continue;
                    getSphere ( v1, v2, v3, v4, o, q );
                }
            }
        }
    }
    return Sphere3d ( sqrt ( q ), o );
}

Def<Sphere3d> minSphereAroundPoints2 ( CArrRef<Vector3d> data )
{
    const nat n = data.size();
    if ( n == 0 ) return Def<Sphere3d> ();
    if ( n == 1 ) return Sphere3d ( 0, data[0] );
    if ( n == 2 )
    {
        const Vector3d o = 0.5 * ( data[0] + data[1] );
        return Sphere3d ( norm2 ( data[0] - o ), o );
    }
    CmbSuite<nat, 8> ip;
    nat i, j, im = 0;
    double max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( data[i] );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) return Sphere3d ( 0, null3d );
    Vector3d o = data[im];
    ip.inc() = im;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( data[i] - o );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) return Sphere3d ( 0, o );
    o = 0.5 * ( o + data[im] );
    double q = 0.25 * max;
    ip.inc() = im;
    CmbSuite<double, 8> la, mu;
    la.inc() = 0.5;
    la.inc() = 0.5;
    CmbArray<Vector3d, 5> v;
    const double tau = 1e-6;
    for (;;)
    {
        max = 0;
        for ( i = 0; i < n; ++i )
        {
            const double t = qmod ( data[i] - o );
            if ( max < t ) max = t, im = i;
        }
        if ( max <= q ) break;
        const nat m = ip.size();
        for ( i = 0; i < m; ++i )
        {
            if ( im == ip[i] ) goto m1;
        }
        const Vector3d u = data[im];
        const Vector3d ou = o - u;
        for ( i = 0; i < m; ++i ) v[i] = data[ip[i]] - u;
        la *= 0.5 * ( 1 + sqrt ( q / ( ou * ou ) ) );
        mu.resize ( m );
        for ( i = 0; i < m; ++i ) mu[i] = sqrt ( la[i] );
        CmbArray2<double, 30> A ( m, m+1 );
        for ( nat k = 0; k < 35; ++k )
        {
            max = 0;
            for ( i = 0; i < m; ++i )
            {
                double & b = A[i][m];
                b = 0;
                if ( fabs ( mu[i] ) < tau )
                {
                    mu[i] = 0;
                    for ( j = 0; j < m; ++j ) A[i][j] = 0;
                    A[i][i] = 1;
                }
                else
                {
                    for ( j = 0; j < m; ++j )
                    {
                        b += ( A[i][j] = v[i] * v[j] * mu[j] ) * mu[j];
                    }
                    const double d = b - 0.5 * v[i] * v[i];
                    if ( max < d ) max = d;
                    A[i][i] += d / mu[i];
                    b += 0.5 * d; 
                }
            }
//display << max << NL;
            slu_gauss ( A );
            for ( i = 0; i < m; ++i )
                mu[i] = A[i][m];
//if(mu.size()==4) 
display << m << k << mu[0] << mu[1] << mu[2] << max / ( tau * tau * q ) << NL;
            if ( max < tau * tau * q )
                break;
        }
        double s = 1;
        for ( i = 0; i < mu.size(); )
        {
            if ( mu[i] )
            {
                s -= ( la[i] = mu[i] * mu[i] );
                ++i;
            }
            else
            {
                mu.del(i);
                la.del(i);
                ip.del(i);
            }
        }
        la.inc() = s;
        ip.inc() = im;
        o.fill();
        for ( i = 0; i < ip.size(); ++i ) o += la[i] * data[ip[i]];
        const double g = q;
        q = 0;
        for ( i = 0; i < ip.size(); ++i )
        {
            const double t = qmod ( data[ip[i]] - o );
            if ( q < t ) q = t;
        }
        if ( q <= g ) break;
    }
m1: return Sphere3d ( sqrt ( q ), o );
}

void minSphereAroundPoints_test()
{
    FixArray<Vector3d, 40> buf;
    static PRandPoint3d rand;
    nat n = 7;
    ArrRef<Vector3d> data ( buf, 0, n );
//for ( nat i = 0; i < 138; ++i )
for ( nat i = 0; i < 4632; ++i )
    data << rand;
    const Sphere3d fig1 = minSphereAroundPoints2 ( data );
    const Sphere3d fig2 = minSphereAroundPoints ( data );
    draw ( fig1, 1, 0.5f, 0, 1, VM_WIRE );
    draw ( fig2, 1, 1, 0, 1, VM_WIRE );
//  draw ( fig2, 1, 1, 0, 1, VM_SMOOTH );
    drawPoints ( data, 0, 1, 1 );
    display << fig1.r - fig2.r << NL;
    display << "end" << NL;
}

void minSphereAroundPoints_test1()
{
    FixArray<Vector3d, 40> buf;
    static PRandPoint3d rand;
    nat n = 7;
//    for ( n = 4; n <= 9; n += 1 )
    {
        double s1 = 0, s2 = 0;
        ArrRef<Vector3d> data ( buf, 0, n );
        nat i = 0;
        for ( i = 0; i < 100000; ++i )
        {
            data << rand;
            double t1 = timeInSec();
if(i==761)
i=i;
            const Sphere3d fig1 = minSphereAroundPoints ( data );
            double t2 = timeInSec();
            const Sphere3d fig2 = minSphereAroundPoints2 ( data );
            double t3 = timeInSec();
            s1 += t2 - t1;
            s2 += t3 - t2;
            const double dr = fabs(fig1.r - fig2.r);
            const double dc = qmod(fig1.o - fig2.o);
            //if ( dr > 6e-14 || dc > 5e-28 )
                //display << n << i << dr << dc << NL;
            if ( dr > 6e-12 )
                display << n << i << dr << NL;
            if ( dr > 6e-5 )
            {
                minSphereAroundPoints2 ( data );
                draw ( fig1, 1, 0.5f, 0, 1, VM_WIRE );
                draw ( fig2, 1, 1, 0, 1, VM_WIRE );
                drawPoints ( data, 0, 1, 1 );
                break;
            }
        }
//        display << n << s1 << s2 << NL;
    }
    display << "end" << NL;
}

void minSphereAroundSpheres_test()
{
    FixArray<Sphere3d, 40> buf;
    static PRandVector3d rand;
    static PRand color;
    nat j, n = 4;
//    for ( n = 4; n <= buf.size(); n += 1 )
    {
        double s1 = 0, s2 = 0;
        ArrRef<Sphere3d> data ( buf, 0, n );
        nat i = 0;
//        for ( i = 0; i < 100; ++i )
        {
            for ( j = 0; j < data.size(); ++j )
            {
                data[j].o = 0.45*rand();
                data[j].r = 0.25;//*fabs ( rand().x );
            }
            for ( j = 0; j < data.size(); ++j )
            {
                const float r = (float) color();
                const float g = (float) color();
                const float b = (float) color();
                draw ( data[j], r, g, b, 1, VM_WIRE );
            }
            double t1 = timeInSec();
            const Sphere3d fig = minSphereAroundSpheres ( data );
            double t2 = timeInSec();
    draw ( fig, 0, 1, 1, 1, VM_WIRE );
        }
//        display << n << s1 << s2 << NL;
    }
    display << "end" << NL;
}

void minSphereAroundSpheres_test2()
{
    FixArray<Sphere3d, 500> buf;
    FixArray<Vector3d, 500> buf2;
    static PRandPoint3d rand;
    static PRand color;
    nat j, n = 4;
    for ( n = 4; n <= buf.size(); n += 1 )
    {
        double s1 = 0, s2 = 0;
        ArrRef<Sphere3d> data ( buf, 0, n );
        ArrRef<Vector3d> data2 ( buf2, 0, n );
        nat i = 0;
        for ( i = 0; i < 1000; ++i )
        {
            for ( j = 0; j < data.size(); ++j )
            {
                data[j].o = data2[j] = 0.45*rand();
                data[j].r = 0.25;
            }
            double t1 = timeInSec();
            const Sphere3d fig1 = minSphereAroundSpheres ( data );
            double t2 = timeInSec();
            const Sphere3d fig2 = minSphereAroundPoints ( data2 );
            double t3 = timeInSec();
            s1 += t2 - t1;
            s2 += t3 - t2;
            const double dr = fabs(fig1.r - fig2.r - data[0].r);
            const double dc = qmod(fig1.o - fig2.o);
            if ( dr > 6e-14 || dc > 8e-27 )
                display << n << i << dr << dc << NL;
        }
        display << n << s1 << s2 << NL;
    }
    display << "end" << NL;
}

void minSpherePlane_test()
{
    const nat nn = 150;
    FixArray<Plane3d,nn> plane;
    static PRandVector3d vrand;
    static PRand prand;
    for ( nat n = 3; n <= nn; ++n )
    {
        for ( nat i = 0; i < n; ++i )
        {
            plane[i].norm = vrand();
            plane[i].dist = prand() - 0.5;
        }
        const Def<Sphere3d> s = minSphere ( CArrRef<Plane3d> ( plane, 0, n ) );
        if ( ! s.isDef )
            display << n << "err" << NL;
        else
        {
            double max = 0;
            for ( nat i = 0; i < n; ++i )
            {
                const double t = fabs ( plane[i] % s.o ) - s.r;
                if ( max < t ) max = t;
            }
            if ( max > 1e-13 )
                display << n << max << NL;
        }
    }
    display << "end" << NL;
}

void minSpherePlane_test2()
{
    const nat nn = 150;
    FixArray<Plane3d,nn> plane;
    static PRandVector3d vrand;
    static PRand prand;
    for ( nat n = 3; n <= nn; ++n )
    {
        double t = 0;
        for ( nat j = 0; j < 100; ++j )
        {
            for ( nat i = 0; i < n; ++i )
            {
                plane[i].norm = vrand();
                plane[i].dist = prand() - 0.5;
            }
            double t0 = timeInSec();
            minSphere ( CArrRef<Plane3d> ( plane, 0, n ) );
            double t1 = timeInSec();
            t += t1 - t0;
        }
        display << n << t << NL;
    }
}

void minSphereLine_test()
{
    static PRandVector3d vrand;
    FixArray<Line3d, 5> line;
    for ( nat j = 0; j < 1; ++j )
    {
        const Vector3d o = 0.5 * vrand();
        for ( nat i = 0; i < line.size(); ++i )
        {
            line[i].dir = vrand();
            line[i].point = o + 0.2 * vrand();
        }
    }
//    line[1].dir = line[0].dir;
//    line[2].dir = line[0].dir;
    for ( nat i = 0; i < line.size(); ++i )
    {
        const Segment3d seg ( line[i].point + 2 * line[i].dir, 
                              line[i].point - 2 * line[i].dir );
        draw ( seg, 0, 1, 1 );
    }
    Def<Sphere3d> s = minSphere ( line );
    if ( s.isDef )
    {
        draw ( s, 1, 0.5f, 0, 1, VM_WIRE );
        double max = 0;
        for ( nat i = 0; i < line.size(); ++i )
        {
            double q = line[i].qdis ( s.o );
            if ( max < q ) max = q;
        }
        if ( max > ( 1 + 1e-12 ) * s.r * s.r ) display << max - s.r * s.r << NL;
    }
    else
    {
        display << "err" << NL;
    }
    display << "end" << NL;
}

Def<Sphere3d> maxSphereInConvexPolyhedron1 ( const Segment3d & seg, CArrRef<const Plane3d *> plane )
{
    Def<Sphere3d> s;
    if ( plane.size() < 4 || seg.a == seg.b ) return s;
    Plane3d tetra[4];
    tetra[0].norm = Vector3d ( -1., 0., 0. );
    tetra[1].norm = Vector3d ( 0., -1., 0. );
    tetra[2].norm = Vector3d ( 0., 0., -1. );
    tetra[3].norm = Vector3d ( 1., 1., 1. ).setNorm2();
    const double shift = ( seg.b.x - seg.a.x + seg.b.y - seg.a.y + seg.b.z - seg.a.z ) / 8.;
    tetra[0].dist = seg.a.x - shift;
    tetra[1].dist = seg.a.y - shift;
    tetra[2].dist = seg.a.z - shift;
    tetra[3].dist = - ( tetra[3].norm * seg.b ) - shift;
    const Plane3d * arr[4];
    arr[0] = tetra;
    arr[1] = tetra + 1;
    arr[2] = tetra + 2;
    arr[3] = tetra + 3;
    Vector3d dir ( 1., 1., 1. );
    Def<Vector3d> point ( Vector3d ( arr[0]->dist, arr[1]->dist, arr[2]->dist ) );
    s.r = ( *arr[3] % point ) / ( -1. - dir * arr[3]->norm );
    s.o = point + s.r * dir;
    const double eps = 1e-9 * shift;
    nat k;
    for ( k = 0; k < 2*plane.size(); ++k )
    {
        double max = - s.r;
        nat i, im;
        for ( i = 0; i < plane.size(); ++i )
        {
            const double t = *plane[i] % s.o;
            if ( max < t ) max = t, im = i;
        }
        if ( max + s.r <= eps )
        {
            break;
        }
        nat ib = 4;
        Vector3d lo;
        double lr = 0.;
        for ( i = 0; i < 4; ++i )
        {
            const Plane3d * temp[3];
            nat j, m = 0;
            for ( j = 0; j < 4; ++j )
            {
                if ( j == i ) continue;
                temp[m++] = arr[j];
            }
            point = intersection ( *temp[0], *temp[1], *temp[2] );
            if ( ! point )
            {
                continue;
            }
            dir = ( temp[0]->norm - temp[2]->norm ) % ( temp[1]->norm - temp[2]->norm );
            const Vector3d ave = ( temp[0]->norm + temp[1]->norm + temp[2]->norm ) / 3.;
            double s = dir * ave;
            if ( s > 0 )
            {
                s = - s;
                dir = - dir;
            }
            const double sm = dir * plane[im]->norm;
            if ( sm < 0 )
            {
                continue;
            }
            const double t = ( *plane[im] % point ) / ( sm - s );
            const double q = t * s;
            if ( q > lr )
            {
                lr = q;
                lo = point - t * dir;
                ib = i;
            }
        }
        if ( ib == 4 )
        {
            s.isDef = true;
            return s;
        }
        s.o = lo;
        s.r = lr;
        arr[ib] = plane[im];
    }
    return s;
}

Def<Sphere3d> maxSphereInConvexPolyhedron1 ( const Polyhedron & poly )
{
    if ( poly.facet.size() < 4 || poly.vertex.size() < 4 ) return Def<Sphere3d>();
    DynArray<const Plane3d *> plane ( poly.facet.size() );
    for ( nat i = 0; i < poly.facet.size(); ++i ) plane[i] = & poly.facet[i].plane;
    return maxSphereInConvexPolyhedron1 ( dimensions ( poly.vertex ), plane );
}

Polyhedron & randPolyhedron ( nat np, Polyhedron & poly )
{
    static PRandVector3d vrand;
    static PRand prand;
    const double cx = 0.5 * ( 1 + prand() );
    const double cy = 0.5 * ( 1 + prand() );
    const double cz = 0.5 * ( 1 + prand() );
    DynArray<Vector3d> point ( np );
    for ( nat j = 0; j < np; ++j )
    {
        point[j] = vrand();
        point[j].x *= cx;
        point[j].y *= cy;
        point[j].z *= cz;
//display << point[j] << NL;
    }
    convexHull ( point, poly );
    return poly;
}

void maxSphereInConvexPolyhedron_test ()
{
    const nat nn = 99;
    nat na[nn+1];
    double s1[nn+1], s2[nn+1];
    nat i, j;
    for ( i = 0; i <= nn; ++i )
    {
        na[i] = 0;
        s1[i] = 0;
        s2[i] = 0;
    }
    static PRand prand;
    for ( i = 0; i < 4000; ++i )
    {
        const nat np = 4 + prand.number(nn-3);
        Polyhedron poly;
        randPolyhedron ( np, poly );
//        makeCuboid ( cx, cy, cz, poly );
        Sphere3d o1, o2;
if ( i == 17956 )
{
    i = i;
}
        double t0 = timeInSec();
        o1 = maxSphereInConvexPolyhedron1 ( poly );
        double t1 = timeInSec();
        o2 = maxSphereInConvexPolyhedron ( poly );
        double t2 = timeInSec();
        if ( o1.r <= 0 || o2.r <= 0 )
        {
if ( i == 0 )
{
    for ( j = 0; j < poly.facet.size(); ++j )
    {
        poly.facet[j].info = j;
    }
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
}
            display << i << NL;
        }
        else
        {
na[poly.vertex.size()] += 1;
s1[poly.vertex.size()] += t1 - t0;
s2[poly.vertex.size()] += t2 - t1;
//display << poly.nfacets << t1 - t0 << t2 - t1 << NL;
            if ( fabs ( o1.r - o2.r ) > 1e-11 ) display << i << o1.r << o2.r << o1.r - o2.r << NL;
        }
    }
    for ( i = 0; i <= nn; ++i )
    {
        if ( na[i] == 0 ) continue;
        display << i << s1[i] / na[i] << s2[i] / na[i] << na[i] << NL;
    }
    display << "end" << NL;
}

void maxPolyhedronInConvexPolyhedron_test()
{
    double time = 0;
//    for ( nat k = 0; k < 340; ++k )
    {
        const nat np = 27;//4 + prand.number(nn-3);
        Polyhedron poly1, poly2;
        randPolyhedron ( np, poly1 );
        randPolyhedron ( np, poly2 );
//        if ( k != 333 ) continue;
//        display << k << NL;
        draw ( poly2, 0, 1, 1, 0, VM_WIRE );
        double t1 = timeInSec();
        Def<Conform3d> conf = maxPolyhedronInConvexPolyhedron ( poly1, poly2 );
        double t2 = timeInSec();
        time += t2 - t1;
        if ( conf.isDef ) 
        {
            poly1 *= conf;
            draw ( poly1, 1, 0, 0, 1, VM_WIRE );
            /*double max = 0.;
            for ( nat i = 0; i < poly1.vertex.size(); ++i )
            {
                const Vector3d & v = poly1.vertex[i];
                for ( nat j = 0; j < poly2.facet.size(); ++j )
                {
                    const double t = poly2.facet[j].plane % v;
                    if ( max < t ) max = t;
                }
            }
            if ( max > 0.3 ) display << k << max << NL;*/
        }
        else
        {
        }
    }
    display << "end" << time << NL;
}

void maxConvexPolyhedronInPolyhedronNR_test()
{
    double time = 0;
    static PRand rand;
//    for ( nat k = 0; k < 500; ++k )
    {
        const nat np = 27;//4 + prand.number(nn-3);
        Polyhedron inner, outer;
        //randPolyhedron ( np, inner );
        RealFile file1 ( "data/sample_helium_princess.pyh", "rb" );
        loadPYH (  file1, inner );
        inner *= getRandOrtho3d ( rand(), rand(), rand() );
        //randPolyhedron ( np, outer );
        RealFile file2 ( "data/octo0684.pyh", "rb" );
        loadPYH (  file2, outer );
        outer *= 0.2;
        DynArray<Vector3d> point ( 1 );
        point[0] = Vector3d(0.3,0.3,0.3);
        drawPoint ( point[0], 1, 1, 0 );
//        if ( k != 333 ) continue;
//        display << k << NL;
        draw ( outer, 0, 1, 1, 0, VM_WIRE );
        double t1 = timeInSec();
        Conform3d conf;
        if ( maxConvexPolyhedronInPolyhedronNR ( inner, outer, point, conf ) )
        {
            double t2 = timeInSec();
            /*Def<Conform3d> conf2 = maxPolyhedronInConvexPolyhedronNR ( inner, outer );
            double t3 = timeInSec();
            display << conf.magn << conf2.magn << NL;
            display << t2-t1 << t3-t2 << NL;*/
            time += t2 - t1;
            inner *= conf;
            draw ( inner, 1, 0, 0, 1, VM_WIRE );
        }
        else
        {
        }
    }
    display << "end" << time << NL;
}

void maxConvexPolyhedronInPolyhedronNR_test2()
{
    double time = 0;
    static PRand rand;
    for ( nat k = 0; k < 500; ++k )
    {
        const nat np = 27;//4 + prand.number(nn-3);
        Polyhedron inner, outer;
        //randPolyhedron ( np, inner );
        RealFile file1 ( "data/sample_helium_princess.pyh", "rb" );
        loadPYH (  file1, inner );
        inner *= getRandOrtho3d ( rand(), rand(), rand() );
        //randPolyhedron ( np, outer );
        RealFile file2 ( "data/octo0684.pyh", "rb" );
        loadPYH (  file2, outer );
        outer *= 0.2;
        DynArray<Vector3d> point ( 0 );
        //point[0] = Vector3d(0.3,0.3,0.3);
        //drawPoint ( point[0], 1, 1, 0 );
//        if ( k != 333 ) continue;
//        display << k << NL;
//        draw ( outer, 0, 1, 1, 0, VM_WIRE );
        double t1 = timeInSec();
        Conform3d conf;
        if ( maxConvexPolyhedronInPolyhedronNR ( inner, outer, point, conf ) )
        {
            double t2 = timeInSec();
            /*Def<Conform3d> conf2 = maxPolyhedronInConvexPolyhedronNR ( inner, outer );
            double t3 = timeInSec();
            display << conf.magn << conf2.magn << NL;
            display << t2-t1 << t3-t2 << NL;*/
            time += t2 - t1;
            inner *= conf;
 //           draw ( inner, 1, 0, 0, 1, VM_WIRE );
        }
        else
        {
        }
    }
    display << "end" << time << NL;
}

void triangle ( LinTran3d & t )
{
    const double A = t.x * t.x;
    const double B = t.y * t.y;
    const double C = t.z * t.z;
    const double D = t.x * t.y;
    const double E = t.y * t.z;
    const double F = t.z * t.x;

    const double c = sqrt ( C );
    const double e = E / c;
    const double f = F / c;
    const double b = sqrt ( B - e * e );
    const double d = ( D - f * e ) / b;
    const double a = sqrt ( A - d * d - f * f );

    t.x = Vector3d ( a, d, f ); 
    t.y = Vector3d ( 0, b, e );
    t.z = Vector3d ( 0, 0, c );
}

void drawP ( CArrRef<Plane3d> plane )
{
    Polyhedron poly, temp;
    poly.makeCube ( 100 );
    for ( nat i = 0; i < plane.size(); ++i )
    {
        cut ( poly, plane[i], temp );
        _swap ( poly, temp );
    }
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
}
//
//**********************************************************************************************************************
//
//  Преобразование:
//  x1 = a * x + d * y + f * z + X
//  y1 =         b * y + e * z + Y
//  z1 =                 c * z + Z
//
//  Касательная плоскость к эллипсоиду:
//  nx * ( a * x + d * y + f * z + X ) + ny * ( b * y + e * z + Y ) + nz * ( c * z + Z ) + g = 0
//  nx * a * x + ( nx * d + ny * b ) * y + ( nx * f + ny * e + nz * c ) * z + nx * X + ny * Y + nz * Z + g = 0
//  ( nx * a )^2 + ( nx * d + ny * b )^2 + ( nx * f + ny * e + nz * c )^2 = ( nx * X + ny * Y + nz * Z + g )^2
//  или более точно
//  nx * X + ny * Y + nz * Z + g + sqrt ( ( nx * a )^2 + ( nx * d + ny * b )^2 + ( nx * f + ny * e + nz * c )^2 ) = 0
//
//  Целевая функция:
//
//  Ф = a * b * c + 0.5 * sum la[i] * ( ... )
//
// Производные:
//
// pA = b * c + sum la[i] * ( xx[i] * a ) 
// pB = a * c + sum la[i] * ( xy[i] * d + yy[i] * b ) 
// pC = a * b + sum la[i] * ( xz[i] * f + yz[i] * e + zz[i] * c ) 
// pD = sum la[i] * ( xx[i] * d + xy[i] * b ) 
// pE = sum la[i] * ( xy[i] * f + yy[i] * e + zy[i] * c ) 
// pF = sum la[i] * ( xx[i] * f + xy[i] * e + zx[i] * c )
// pX = - sum la[i] * ( xx[i] * X + xy[i] * Y + xz[i] * Z + nx * g )
// pY = - sum la[i] * ( xy[i] * X + yy[i] * Y + yz[i] * Z + ny * g )
// pZ = - sum la[i] * ( xz[i] * X + yz[i] * Y + zz[i] * Z + nz * g )
//
//*********************************** Первый вариант замены переменных *************************************************
//
//  nx * nx * A + ny * ny * B + nz * nz * C + 2 * nx * ny * D + 2 * ny * nz * E + 2 * nx * nz * F = 
//  = ( nx * X + ny * Y + nz * Z + g )^2
//  или
//  nx * X + ny * Y + nz * Z + g + sqrt ( nx * nx * A + ny * ny * B + nz * nz * C + 2 * nx * ny * D + 2 * ny * nz * E + 2 * nx * nz * F )
//
//  A = a * a + d * d + f * f    B = b * b + e * e    C = c * c
//  D = d * b + f * e            E = e * c            F = f * c
//  
//  c = sqrt ( C )               e = E / c            f = F / c
//  b = sqrt ( B - e * e )    d = ( D - f * e ) / b   a = sqrt ( A - d * d - f * f )
//
//  Целевая функция:
//
//  Ф = A * B * C + 2 * D * E * F - A * E * E - B * F * F - C * D * D
//
// Производные:
//
// pA = B * C - E * E   
// pB = A * C - F * F  
// pC = A * B - D * D
// pD = 2 * ( E * F - C * D )   
// pE = 2 * ( D * F - A * E )  
// pF = 2 * ( D * E - B * F )
//
// Линеаризация:
//
// pA = B * C0 + B0 * C - 2 * E * E0 - ( B0 * C0 - E0 * E0 )
// pB = A * C0 + A0 * C - 2 * F * F0 - ( A0 * C0 - F0 * F0 )
// pC = A * B0 + A0 * B - 2 * D * D0 - ( A0 * B0 - D0 * D0 )
// pD = 2 * ( E * F0 - C * D0 + E0 * F - C0 * D - E0 * F0 + C0 * D0 )   
// pE = 2 * ( D * F0 - A * E0 + D0 * F - A0 * E - D0 * F0 + A0 * E0 )  
// pF = 2 * ( D * E0 - B * F0 + D0 * E - B0 * F - D0 * E0 + B0 * F0 )
//
//*********************************** Второй вариант замены переменных *************************************************
//
//  nx * nx * A + ny * ny * B + nz * nz * C + 2 * nx * ny * D + 2 * ny * nz * E + 2 * nx * nz * F = 
//  = 2 * g * ( nx * X + ny * Y + nz * Z ) + g * g
//
//  A = a * a + d * d + f * f - X * X    B = b * b + e * e - Y * Y    C = c * c - Z * Z
//  D = d * b + f * e - X * Y            E = e * c - Y * Z            F = f * c - X * Z
//
//  c = sqrt ( C + Z * Z )            e = ( E + Y * Z ) / c            f = ( F + X * Z ) / c
//  b = sqrt ( B + Y * Y - e * e )    d = ( D + X * Y - f * e ) / b    a = sqrt ( A + X * X - d * d - f * f )
//
//  Целевая функция:
//
//  Ф = ( A + X * X ) * ( B + Y * Y ) * ( C + Z * Z ) + ( D + X * Y ) * ( E + Y * Z ) * ( F + X * Z ) * 2 - 
//      ( A + X * X ) * ( E + Y * Z ) * ( E + Y * Z ) - ( B + Y * Y ) * ( F + X * Z ) * ( F + X * Z ) - 
//      ( C + Z * Z ) * ( D + X * Y ) * ( D + X * Y )
//  или
//  Ф = A * B * C + 2 * D * E * F - A * E * E - B * F * F - C * D * D + 
//      X * X * ( B * C - E * E ) + Y * Y * ( A * C - F * F ) + Z * Z * ( A * B - D * D ) +
//  2*( X * Y * ( E * F - C * D ) + Y * Z * ( D * F - A * E ) + X * Z * ( D * E - B * F ) )
//
// Производные:
//
// pA = B * C - E * E + Y * Y * C + Z * Z * B - 2 * Y * Z * E
// pB = A * C - F * F + X * X * C + Z * Z * A - 2 * X * Z * F
// pC = A * B - D * D + X * X * B + Y * Y * A - 2 * X * Y * D
// pD = 2 * ( E * F - C * D + X * Z * E + Y * Z * F - Z * Z * D - X * Y * C )   
// pE = 2 * ( F * D - A * E + Y * X * F + Z * X * D - X * X * E - Y * Z * A )  
// pF = 2 * ( D * E - B * F + Z * Y * D + X * Y * E - Y * Y * F - Z * X * B )
// pX = 2 * ( X * ( B * C - E * E ) + Y * ( E * F - C * D ) + Z * ( D * E - B * F ) )
// pY = 2 * ( Y * ( C * A - F * F ) + Z * ( F * D - A * E ) + X * ( E * F - C * D ) )
// pZ = 2 * ( Z * ( A * B - D * D ) + X * ( D * E - B * F ) + Y * ( F * D - A * E ) )
//
// Линеаризация:
//
// pA = B * ( C0 + sz0 * sz0 ) + C * ( B0 + sy0 * sy0 ) - 2 * E * ( E0 + sy0 * sz0 )
// + 2 * Y * ( sy0 * C0 - sz0 * E0 )
// + 2 * Z * ( sz0 * B0 - sy0 * E0 )
// - ( B0 * C0 - E0 * E0 + 2 * ( sy0 * sy0 * C0 - 2 * sy0 * sz0 * E0 + sz0 * sz0 * B0 ) )
//
// pB = A * ( C0 + sz0 * sz0 ) + C * ( A0 + X0 * X0 ) - 2 * F * ( F0 + X0 * sz0 ) 
// + 2 * X * ( X0 * C0 - sz0 * F0 )
// + 2 * Z * ( sz0 * A0 - X0 * F0 )
// - ( A0 * C0 - F0 * F0 + 2 * ( X0 * X0 * C0 - 2 * X0 * sz0 * F0 + sz0 * sz0 * A0 ) )
//
// pC = A * ( B0 + sy0 * sy0 ) + B * ( A0 + X0 * X0 ) - 2 * D * ( D0 + X0 * sy0 )  
// + 2 * X * ( X0 * B0 - sy0 * D0 )
// + 2 * Y * ( sy0 * A0 - X0 * D0 )
// - ( A0 * B0 - D0 * D0 + 2 * ( X0 * X0 * B0 - 2 * X0 * sy0 * D0 + sy0 * sy0 * A0 ) )
//
// pD = 2 * ( E * ( F0 + X0 * sz0 ) - C * ( D0 + X0 * sy0 ) + F * ( E0 + sy0 * sz0 ) - D * ( C0 + sz0 * sz0 )
//  + X * ( sz0 * E0 - sy0 * C0 )
//  + Y * ( sz0 * F0 - X0 * C0 )
//  + Z * ( sy0 * F0 - 2 * sz0 * D0 + X0 * E0 )
//  - E0 * F0 + C0 * D0
//  - 2 * ( sy0 * sz0 * F0 + X0 * sz0 * E0 - sz0 * sz0 * D0 - X0 * sy0 * C0 ) )
//
// pE = 2 * ( F * ( D0 + X0 * sy0 ) - A * ( E0 + sy0 * sz0 ) + D * ( F0 + X0 * sz0 ) - E * ( A0 + X0 * X0 )
//  + Y * ( X0 * F0 - sz0 * A0 )
//  + Z * ( X0 * D0 - sy0 * A0 )
//  + X * ( sz0 * D0 - 2 * X0 * E0 + sy0 * F0 )
//  - F0 * D0 + A0 * E0
//  - 2 * ( sz0 * X0 * D0 + sy0 * X0 * F0 - X0 * X0 * E0 - sy0 * sz0 * A0 ) )
//
// pF = 2 * ( D * ( E0 + sy0 * sz0 ) - B * ( F0 + X0 * sz0 ) + E * ( D0 + X0 * sy0 ) - F * ( B0 + sy0 * sy0 )
//  + Z * ( sy0 * D0 - X0 * B0 )
//  + X * ( sy0 * E0 - sz0 * B0 )
//  + Y * ( X0 * E0 - 2 * sy0 * F0 + sz0 * D0 )
//  - D0 * E0 + B0 * F0
//  - 2 * ( X0 * sy0 * E0 + sz0 * sy0 * D0 - sy0 * sy0 * F0 - sz0 * X0 * B0 ) )
//
// pX = 2 * ( X * ( B0 * C0 - E0 * E0 ) + Y * ( E0 * F0 - C0 * D0 ) + Z * ( D0 * E0 - B0 * F0 )
//          + B * ( X0 * C0 - sz0 * F0 ) + C * ( X0 * B0 - sy0 * D0 )
//          + D * ( sz0 * E0 - sy0 * C0 ) + F * ( sy0 * E0 - sz0 * B0 )
//          + E * ( sy0 * F0 - 2 * X0 * E0 + sz0 * D0 )
//  - 2 * ( X0 * ( B0 * C0 - E0 * E0 ) + sy0 * ( E0 * F0 - C0 * D0 ) + sz0 * ( D0 * E0 - B0 * F0 ) ) )
//
// pY = 2 * ( Y * ( C0 * A0 - F0 * F0 ) + Z * ( F0 * D0 - A0 * E0 ) + X * ( E0 * F0 - C0 * D0 )
//          + C * ( sy0 * A0 - X0 * D0 ) + A * ( sy0 * A0 - sz0 * E0 )
//          + E * ( X0 * F0 - sz0 * A0 ) + D * ( sz0 * F0 - X0 * C0 )
//          + F * ( sz0 * D0 - 2 * sy0 * F0 + X0 * E0 )
//  - 2 * ( sy0 * ( C0 * A0 - F0 * F0 ) + sz0 * ( F0 * D0 - A0 * E0 ) + X0 * ( E0 * F0 - C0 * D0 ) ) )
//
// pZ = 2 * ( Z * ( A0 * B0 - D0 * D0 ) + X * ( D0 * E0 - B0 * F0 ) + Y * ( F0 * D0 - A0 * E0 )
//          + A * ( sz0 * B0 - sy0 * E0 ) + B * ( sz0 * B0 - X0 * F0 )
//          + F * ( sy0 * D0 - X0 * B0 ) + E * ( X0 * D0 - sy0 * A0 )
//          + D * ( X0 * E0 - 2 * sz0 * D0 + sy0 * F0 )
//  - 2 * ( sz0 * ( A0 * B0 - D0 * D0 ) + X0 * ( D0 * E0 - B0 * F0 ) + sy0 * ( F0 * D0 - A0 * E0 ) ) )
//
//**********************************************************************************************************************

void test ( CArrRef<Plane3d> plane, Affin3d & aff, CArrRef2<double> M )
{
    nat i;
    const Vector3d & vx = aff.t.x;
    const Vector3d & vy = aff.t.y;
    const Vector3d & vz = aff.t.z;
    const Vector3d & s = aff.s;
    double A = vx.x * vx.x + vx.y * vx.y + vx.z * vx.z - s.x * s.x;
    double B = vy.y * vy.y + vy.z * vy.z - s.y * s.y;
    double C = vz.z * vz.z - s.z * s.z;
    double D = vx.y * vy.y + vx.z * vy.z - s.x * s.y;
    double E = vy.z * vz.z - s.y * s.z;
    double F = vx.z * vz.z - s.x * s.z;
    double X = s.x;
    double Y = s.y;
    double Z = s.z;
    const nat m = plane.size();
    const nat m9 = m + 9;
    const nat m10 = m + 10;
    DynArray2<double> L ( m9, m10 );
    L.fill(0);
    L[0][1] = L[1][0] = C + Z * Z;
    L[0][2] = L[2][0] = B + Y * Y;
    L[0][4] = L[4][0] = -2 * ( E + Y * Z );
    L[0][7] = L[7][0] = 2 * ( Y * C - Z * E );
    L[0][8] = L[8][0] = 2 * ( Z * B - Y * E );
    L[0][m9] = - ( B * C - E * E + 2 * ( Y * Y * C - 2 * Y * Z * E  + Z * Z * B ) );
    L[1][2] = L[2][1] = A + X * X;
    L[1][5] = L[5][1] = -2 * ( F + X * Z );
    L[1][6] = L[6][1] = 2 * ( X * C - Z * F );
    L[1][8] = L[8][1] = 2 * ( Z * A - X * F );
    L[1][m9] = - ( A * C - F * F + 2 * ( X * X * C - 2 * X * Z * F  + Z * Z * A ) );
    L[2][3] = L[3][2] = -2 * ( D + X * Y );
    L[2][6] = L[6][2] = 2 * ( X * B - Y * D );
    L[2][7] = L[7][2] = 2 * ( Y * A - X * D );
    L[2][m9] = - ( A * B - D * D + 2 * ( X * X * B - 2 * X * Y * D  + Y * Y * A ) );
    L[3][3] = -2 * ( C + Z * Z );
    L[3][4] = L[4][3] = 2 * ( F + X * Z );
    L[3][5] = L[5][3] = 2 * ( E + Y * Z );
    L[3][6] = L[6][3] = 2 * ( Z * E - Y * C );
    L[3][7] = L[7][3] = 2 * ( Z * F - X * C );
    L[3][8] = L[8][3] = 2 * ( Y * F - 2 * Z * D + X * E );
    L[3][m9] = - 2 * ( E * F - C * D + 2 * ( Y * Z * F + X * Z * E - Z * Z * D - X * Y * C ) );
    L[4][4] = -2 * ( A + X * X );
    L[4][5] = L[5][4] = 2 * ( D + X * Y );
    L[4][6] = L[6][4] = 2 * ( Z * D - 2 * X * E + Y * F );
    L[4][7] = L[7][4] = 2 * ( X * F - Z * A );
    L[4][8] = L[8][4] = 2 * ( X * D - Y * A );
    L[4][m9] = - 2 * ( D * F - A * E + 2 * ( Z * X * D + Y * X * F - X * X * E - Y * Z * A ) );
    L[5][5] = -2 * ( B + Y * Y );
    L[5][6] = L[6][5] = 2 * ( Y * E - Z * B );
    L[5][7] = L[7][5] = 2 * ( X * E - 2 * Y * F + Z * D );
    L[5][8] = L[8][5] = 2 * ( Y * D - X * B );
    L[5][m9] = - 2 * ( D * E - B * F + 2 * ( X * Y * E + Z * Y * D - Y * Y * F - Z * X * B ) );
    L[6][6] = 2 * ( B * C - E * E );
    L[6][7] = L[7][6] = 2 * ( E * F - C * D );
    L[6][8] = L[8][6] = 2 * ( D * E - B * F );
    L[6][m9] = - 4 * ( X * ( B * C - E * E ) + Y * ( E * F - C * D ) + Z * ( D * E - B * F ) );
    L[7][7] = 2 * ( C * A - F * F );
    L[7][8] = L[8][7] = 2 * ( F * D - A * E );
    L[7][m9] = - 4 * ( Y * ( C * A - F * F ) + Z * ( F * D - A * E ) + X * ( E * F - C * D ) );
    L[8][8] = 2 * ( A * B - D * D );
    L[8][m9] = - 4 * ( Z * ( A * B - D * D ) + X * ( D * E - B * F ) + Y * ( F * D - A * E ) );
    for ( i = 0; i < m; ++i )
    {
        const nat j = i + 9;
        const Plane3d & p = plane[i];
        const double & g = p.dist;
        L[j][0] = L[0][j] = p.norm.x * p.norm.x;
        L[j][1] = L[1][j] = p.norm.y * p.norm.y;
        L[j][2] = L[2][j] = p.norm.z * p.norm.z;
        L[j][3] = L[3][j] = p.norm.x * p.norm.y * 2;
        L[j][4] = L[4][j] = p.norm.y * p.norm.z * 2;
        L[j][5] = L[5][j] = p.norm.z * p.norm.x * 2;
        L[j][6] = L[6][j] = p.norm.x * g * -2;
        L[j][7] = L[7][j] = p.norm.y * g * -2;
        L[j][8] = L[8][j] = p.norm.z * g * -2;
        L[j][m9] = -g * g;
    }
    for ( i = 0; i < 9; ++i )
    {
        nat j;
        for ( j = 0; j < m10; ++j ) display << L[i][j];
        display << NL;
        for ( j = 0; j < m10; ++j ) display << M[i][j];
        display << NL;
    }
    slu_gauss ( L );
//    display << A << B << C << D << E << F << X << Y << Z << NL;
//    for ( i = 0; i < m9; ++i ) display << -L[i][m9];
    display << NL;
//display << A << B << C << D << E << F << X << Y << Z << NL;
/*    if(0)
    {
        for ( i = 0; i < m; ++i )
        {
            const Plane3d & p = plane1[i];
            const double & nx = p.norm.x;
            const double & ny = p.norm.y;
            const double & nz = p.norm.z;
            const double & g = p.dist;
            //  nx * nx * A + ny * ny * B + nz * nz * C + 2 * nx * ny * D + 2 * ny * nz * E + 2 * nx * nz * F = 
            //  = 2 * g * ( nx * X + ny * Y + nz * Z ) + g * g
            display << nx * nx * A + ny * ny * B + nz * nz * C + 2 * nx * ny * D + 2 * ny * nz * E + 2 * nx * nz * F - 
                2 * g * ( nx * X + ny * Y + nz * Z ) - g * g << NL;
        }
    }
//test ( plane, aff, M );
        if(0)
        {
            FixArray<double, 9> grad1;
            grad1[0] = B * C - E * E + Y * Y * C + Z * Z * B - 2 * Y * Z * E;
            grad1[1] = A * C - F * F + X * X * C + Z * Z * A - 2 * X * Z * F;
            grad1[2] = A * B - D * D + X * X * B + Y * Y * A - 2 * X * Y * D;
            grad1[3] = 2 * ( E * F - C * D + X * Z * E + Y * Z * F - Z * Z * D - X * Y * C );  
            grad1[4] = 2 * ( F * D - A * E + Y * X * F + Z * X * D - X * X * E - Y * Z * A );
            grad1[5] = 2 * ( D * E - B * F + Z * Y * D + X * Y * E - Y * Y * F - Z * X * B );
            grad1[6] = 2 * ( X * ( B * C - E * E ) + Y * ( E * F - C * D ) + Z * ( D * E - B * F ) );
            grad1[7] = 2 * ( Y * ( C * A - F * F ) + Z * ( F * D - A * E ) + X * ( E * F - C * D ) );
            grad1[8] = 2 * ( Z * ( A * B - D * D ) + X * ( D * E - B * F ) + Y * ( F * D - A * E ) );
            for ( i = 0; i < 9; ++i )
            {
                display << grad1[i] << 
                    M[i][0]*A + M[i][1]*B + M[i][2]*C + 
                    M[i][3]*D + M[i][4]*E + M[i][5]*F + 
                    M[i][6]*X+ M[i][7]*Y+ M[i][8]*Z+ M[i][m9] << NL;
            }
            display << NL;
            display << "begin" << NL;
            double dx = 1e-9;
            display << ( funcV ( A+dx, B, C, D, E, F, X, Y, Z ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[0] << NL;
            display << ( funcV ( A, B+dx, C, D, E, F, X, Y, Z ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[1] << NL;
            display << ( funcV ( A, B, C+dx, D, E, F, X, Y, Z ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[2] << NL;
            display << ( funcV ( A, B, C, D+dx, E, F, X, Y, Z ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[3] << NL;
            display << ( funcV ( A, B, C, D, E+dx, F, X, Y, Z ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[4] << NL;
            display << ( funcV ( A, B, C, D, E, F+dx, X, Y, Z ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[5] << NL;
            display << ( funcV ( A, B, C, D, E, F, X+dx, Y, Z ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[6] << NL;
            display << ( funcV ( A, B, C, D, E, F, X, Y+dx, Z ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[7] << NL;
            display << ( funcV ( A, B, C, D, E, F, X, Y, Z+dx ) - funcV ( A, B, C, D, E, F, X, Y, Z ) ) / dx << grad1[8] << NL;
            display << "end" << NL;
        }
        if(0)
        {
                display << funcV ( A, B, C, D, E, F, X, Y, Z );
                display << funcV ( A2, B2, C2, D2, E2, F2, X2, Y2, Z2 ) << NL;
        }
        if(0)
        {
            for ( i = 0; i < m; ++i )
            {
                const Plane3d & p = plane1[i];
                const Vector3d & norm = p.norm;
                const double & dist = p.dist;
                const double xx = norm.x * norm.x;
                const double yy = norm.y * norm.y;
                const double zz = norm.z * norm.z;
                const double xy = norm.x * norm.y * 2;
                const double yz = norm.y * norm.z * 2;
                const double zx = norm.z * norm.x * 2;
                const double xg = norm.x * dist * -2;
                const double yg = norm.y * dist * -2;
                const double zg = norm.z * dist * -2;
                display << A2 * xx + B2 * yy + C2 * zz +
                           D2 * xy + E2 * yz + F2 * zx +
                           X2 * xg + Y2 * yg + Z2 * zg - dist * dist << NL;
            }
        }*/
}

inline double funcV ( double A, double B, double C, double D, double E, double F, double X, double Y, double Z )
{
    A += X * X;
    B += Y * Y;
    C += Z * Z;
    D += X * Y;
    E += Y * Z;
    F += Z * X;
    return A * B * C + 2 * D * E * F - A * E * E - B * F * F - C * D * D;
}

void trans ( CArrRef<double> lambda, CArrRef2<double> M, 
             double & A, double & B, double & C, double & D, double & E, 
             double & F, double & X, double & Y, double & Z )
{
    const nat m = lambda.size();
    const nat m9 = m + 9;
    A = M[0][m9];
    B = M[1][m9];
    C = M[2][m9];
    D = M[3][m9];
    E = M[4][m9];
    F = M[5][m9];
    X = M[6][m9];
    Y = M[7][m9];
    Z = M[8][m9];
    for ( nat i = 0; i < m; ++i )
    {
        const nat j = i + 9;
        A += M[0][j] * lambda[i];
        B += M[1][j] * lambda[i];
        C += M[2][j] * lambda[i];
        D += M[3][j] * lambda[i];
        E += M[4][j] * lambda[i];
        F += M[5][j] * lambda[i];
        X += M[6][j] * lambda[i];
        Y += M[7][j] * lambda[i];
        Z += M[8][j] * lambda[i];
    }
}

void trans ( CArrRef<double> lambda, CArrRef2<double> M, double * par )
{
    const nat m = lambda.size();
    const nat m9 = m + 9;
    for ( nat k = 0; k < 9; ++k )
    {
        const double * row = M[k](9);
        double & p = par[k];
        p = M[k][m9];
        for ( nat i = 0; i < m; ++i ) p += row[i] * lambda[i];
    }
}

template <nat N> bool task ( ArrRef<double> lambda, CArrRef2<double> M, CArrRef2<double> bound,
                             CArrRef<Plane3d> plane, List< Vertex<N> > & stor )
{
// Инициализация области допустимых преобразований
    WireModel<N> model;
    model.simplex ( 4*(N+1), stor );
    for ( nat i = 0; i < N; ++i )
    {
        model.cut ( * ( const Double<N+1> * ) bound[i](), stor );
    }
    Double<9> par;
    double max = 0;
    const Vertex<N> * r = 0;
    if ( model.vlist.top() )
    do
    {
        const Vertex<N> * p = model.vlist.cur();
        trans ( CArrRef<double> ( &p->coor.d0, N ), M, &par.d0 );
        const double c2 = par.d2 + par.d8 * par.d8;
        if ( c2 <= 0 ) 
            continue;
        const double c = sqrt ( c2 );
        const double e = ( par.d4 + par.d7 * par.d8 ) / c;
        const double f = ( par.d5 + par.d6 * par.d8 ) / c;
        const double b2 = par.d1 + par.d7 * par.d7 - e * e;
        if ( b2 <= 0 ) 
            continue;
        const double b = sqrt ( b2 );
        const double d = ( par.d3 + par.d6 * par.d7 - f * e ) / b;
        const double a2 = par.d0 + par.d6 * par.d6 - d * d - f * f;
        if ( a2 <= 0 ) 
            continue;
        const double t = a2 * b2 * c2;
        if ( max < t ) max = t, r = p;
    }
    while ( model.vlist.next() );
    model.vlist.movAllAftLas ( stor );
    if ( ! r )
        return false;
    * ( Double<N> * ) lambda() = r->coor;
    return true;
}

struct CutStorNd
{
    List< Vertex<5> > vert5;
    List< Vertex<6> > vert6;
    List< Vertex<7> > vert7;
    List< Vertex<8> > vert8;
    List< Vertex<9> > vert9;
    List< Vertex<10> > vert10;
};

bool maxEllipsoidInConvexPolyhedronV ( CArrRef<Plane3d> plane, ArrRef<bool> act, CutStorNd & stor, Affin3d & aff )
{
    nat i;
    const nat m = act.size();
    const Vector3d & vx = aff.t.x;
    const Vector3d & vy = aff.t.y;
    const Vector3d & vz = aff.t.z;
    const Vector3d &  s = aff.s;
    double X = s.x;
    double Y = s.y;
    double Z = s.z;
    double A = vx.x * vx.x + vx.y * vx.y + vx.z * vx.z - X * X;
    double B = vy.y * vy.y + vy.z * vy.z - Y * Y;
    double C = vz.z * vz.z - Z * Z;
    double D = vx.y * vy.y + vx.z * vy.z - X * Y;
    double E = vy.z * vz.z - Y * Z;
    double F = vx.z * vz.z - X * Z;
    DynArray2<double> bound ( m, m+1 );
    CmbArray<double, 9> lambda ( m );
    const nat m9 = m + 9;
    DynArray2<double> M ( 9, m+10 );
    for ( nat step = 0; step < 9; ++step )
    {
        M.fill(0);
        M[0][1] = M[1][0] = C + Z * Z;
        M[0][2] = M[2][0] = B + Y * Y;
        M[0][4] = M[4][0] = -2 * ( E + Y * Z );
        M[0][7] = M[7][0] = 2 * ( Y * C - Z * E );
        M[0][8] = M[8][0] = 2 * ( Z * B - Y * E );
        M[0][m9] = B * C - E * E + 2 * ( Y * Y * C - 2 * Y * Z * E  + Z * Z * B );
        M[1][2] = M[2][1] = A + X * X;
        M[1][5] = M[5][1] = -2 * ( F + X * Z );
        M[1][6] = M[6][1] = 2 * ( X * C - Z * F );
        M[1][8] = M[8][1] = 2 * ( Z * A - X * F );
        M[1][m9] = A * C - F * F + 2 * ( X * X * C - 2 * X * Z * F  + Z * Z * A );
        M[2][3] = M[3][2] = -2 * ( D + X * Y );
        M[2][6] = M[6][2] = 2 * ( X * B - Y * D );
        M[2][7] = M[7][2] = 2 * ( Y * A - X * D );
        M[2][m9] = A * B - D * D + 2 * ( X * X * B - 2 * X * Y * D  + Y * Y * A );
        M[3][3] = -2 * ( C + Z * Z );
        M[3][4] = M[4][3] = 2 * ( F + X * Z );
        M[3][5] = M[5][3] = 2 * ( E + Y * Z );
        M[3][6] = M[6][3] = 2 * ( Z * E - Y * C );
        M[3][7] = M[7][3] = 2 * ( Z * F - X * C );
        M[3][8] = M[8][3] = 2 * ( Y * F - 2 * Z * D + X * E );
        M[3][m9] = 2 * ( E * F - C * D + 2 * ( Y * Z * F + X * Z * E - Z * Z * D - X * Y * C ) );
        M[4][4] = -2 * ( A + X * X );
        M[4][5] = M[5][4] = 2 * ( D + X * Y );
        M[4][6] = M[6][4] = 2 * ( Z * D - 2 * X * E + Y * F );
        M[4][7] = M[7][4] = 2 * ( X * F - Z * A );
        M[4][8] = M[8][4] = 2 * ( X * D - Y * A );
        M[4][m9] = 2 * ( D * F - A * E + 2 * ( Z * X * D + Y * X * F - X * X * E - Y * Z * A ) );
        M[5][5] = -2 * ( B + Y * Y );
        M[5][6] = M[6][5] = 2 * ( Y * E - Z * B );
        M[5][7] = M[7][5] = 2 * ( X * E - 2 * Y * F + Z * D );
        M[5][8] = M[8][5] = 2 * ( Y * D - X * B );
        M[5][m9] =2 * ( D * E - B * F + 2 * ( X * Y * E + Z * Y * D - Y * Y * F - Z * X * B ) );
        M[6][6] = 2 * ( B * C - E * E );
        M[6][7] = M[7][6] = 2 * ( E * F - C * D );
        M[6][8] = M[8][6] = 2 * ( D * E - B * F );
        M[6][m9] = 4 * ( X * ( B * C - E * E ) + Y * ( E * F - C * D ) + Z * ( D * E - B * F ) );
        M[7][7] = 2 * ( C * A - F * F );
        M[7][8] = M[8][7] = 2 * ( F * D - A * E );
        M[7][m9] = 4 * ( Y * ( C * A - F * F ) + Z * ( F * D - A * E ) + X * ( E * F - C * D ) );
        M[8][8] = 2 * ( A * B - D * D );
        M[8][m9] = 4 * ( Z * ( A * B - D * D ) + X * ( D * E - B * F ) + Y * ( F * D - A * E ) );
        for ( i = 0; i < m; ++i )
        {
            const Plane3d & p = plane[i];
            const Vector3d & norm = p.norm;
            const double & g = p.dist;
            const nat j = i + 9;
            M[0][j] = norm.x * norm.x;
            M[1][j] = norm.y * norm.y;
            M[2][j] = norm.z * norm.z;
            M[3][j] = norm.x * norm.y * 2;
            M[4][j] = norm.y * norm.z * 2;
            M[5][j] = norm.z * norm.x * 2;
            M[6][j] = norm.x * g * -2;
            M[7][j] = norm.y * g * -2;
            M[8][j] = norm.z * g * -2;
        }
        if ( ! slu_gauss ( M ) )
            return false;
        for ( i = 0; i < m; ++i )
        {
            const Plane3d & p = plane[i];
            const Vector3d & norm = p.norm;
            const double & dist = p.dist;
            const double xx = norm.x * norm.x;
            const double yy = norm.y * norm.y;
            const double zz = norm.z * norm.z;
            const double xy = norm.x * norm.y * 2;
            const double yz = norm.y * norm.z * 2;
            const double zx = norm.z * norm.x * 2;
            const double xg = norm.x * dist * -2;
            const double yg = norm.y * dist * -2;
            const double zg = norm.z * dist * -2;
            ArrRef<double> row = bound[i];
            for ( nat k = 0; k <= m; ++k )
            {
                const nat j = k + 9;
                row[k] = M[0][j] * xx + M[1][j] * yy + M[2][j] * zz +
                         M[3][j] * xy + M[4][j] * yz + M[5][j] * zx +
                         M[6][j] * xg + M[7][j] * yg + M[8][j] * zg;
            }
            row[m] -= dist * dist;
        }
        // Находим значения множителей Лагранжа
        switch ( m )
        {
        case 5:
            if ( ! task ( lambda, M, bound, plane, stor.vert5 ) )
                return false;
            break;
        case 6:
            if ( ! task ( lambda, M, bound, plane, stor.vert6 ) )
                return false;
            break;
        case 7:
            if ( ! task ( lambda, M, bound, plane, stor.vert7 ) )
                return false;
            break;
        case 8:
            if ( ! task ( lambda, M, bound, plane, stor.vert8 ) ) return false;
            break;
        case 9:
            if ( ! task ( lambda, M, bound, plane, stor.vert9 ) ) return false;
            break;
        case 10:
            if ( ! task ( lambda, M, bound, plane, stor.vert10 ) ) return false;
            break;
        default:
            return false;
        }
        double A2, B2, C2, D2, E2, F2, X2, Y2, Z2;
        trans ( lambda, M, A2, B2, C2, D2, E2, F2, X2, Y2, Z2 );
        const double dif = ( fabs(A - A2) + fabs(B - B2) + fabs(C - C2) + fabs(D - D2) + fabs(E - E2) + fabs(F - F2) ) / 
                           ( fabs(A) + fabs(B) + fabs(C) + fabs(D) + fabs(E) + fabs(F) );
        A = A2;
        B = B2;
        C = C2;
        D = D2;
        E = E2;
        F = F2;
        X = X2;
        Y = Y2;
        Z = Z2;
//display << step << dif << NL;
        if ( dif < 1e-6 )
        {
            double c = C + Z * Z;
            if ( c <= 0 ) 
                return false;
            c = sqrt ( c );
            const double e = ( E + Y * Z ) / c;
            const double f = ( F + X * Z ) / c;
            double b = B + Y * Y - e * e;
            if ( b <= 0 ) 
                return false;
            b = sqrt ( b );
            const double d = ( D + X * Y - f * e ) / b;
            double a = A + X * X - d * d - f * f;
            if ( a <= 0 ) 
                return false;
            a = sqrt ( a );
            aff.t.x = Vector3d ( a, d, f );
            aff.t.y = Vector3d ( 0, b, e );
            aff.t.z = Vector3d ( 0, 0, c );
            aff.s   = Vector3d ( X, Y, Z );
            for ( i = 0; i < m; ++i ) act[i] = lambda[i] != 0;
            return true;
        }
    }
    return false;
}

inline double distance ( const Affin3d & b, const Plane3d & p )
{
    return ( p % b.s ) / sqrt ( _pow2 ( p.norm.x * b.t.x.x ) +
                                _pow2 ( p.norm.x * b.t.x.y + p.norm.y * b.t.y.y ) +
                                _pow2 ( p.norm.x * b.t.x.z + p.norm.y * b.t.y.z + p.norm.z * b.t.z.z ) );
}

Def<Ellipsoid3d> maxEllipsoidInConvexPolyhedronV ( const Polyhedron & poly )
{
    Def<Ellipsoid3d> res;
    const nat n = poly.facet.size();
    switch ( n )
    {
    case 0:
    case 1:
    case 2:
    case 3:
        return res;
    case 4:
        return maxEllipsoidInTetrahedronV ( poly.vertex[0], poly.vertex[1], poly.vertex[2], poly.vertex[3] );
    }
    const Affin3d stand = getEllipsoidPlg ( poly ).getAffin3d();
    Polyhedron poly2;
    poly2 = poly;
    poly2 *= ~stand;
    nat i;
// Находим первоначальное приближение
    DynArray<Plane3d> plane ( n + 6 );
    for ( i = 0; i < n; ++i ) plane[i] = poly2.facet[i].plane;
    Cuboid3d para = minCuboidAroundPointsNR ( poly2.vertex );
    const Polyhedron poly0 ( para );
    CmbSuite<nat, 10> support1, support2;
    support1.resize(6);
    for ( i = 0; i < 6; ++i )
    {
        const nat j = i + n;
        plane[j] = poly0.facet[i].plane;
        support1[i] = j;
    }
    Affin3d aff = para.getAffin3d();
// Применяем ограничения
    CutStor3d stor3d;
    CutStorNd storNd;
    FixArray<Vector3d, 8> vert;
    const Segment3d dim = dimensions ( poly0.vertex );
    for ( nat k = 0; k < 29; ++k )
    {
        // Ищем наиболее близкую плоскость
        nat im;
        double max = -1;
        for ( i = 0; i < plane.size(); ++i )
        {
            const double t = distance ( aff, plane[i] );
            if ( max < t ) max = t, im = i;
        }
        if ( max < -1 + 1e-9 || hasEqu ( support1, im )  )
        {
            return ( stand * aff ) ( Sphere3d ( 1, null3d ) );
        }
        // Добавляем новую плоскость в ограничения
        support1.inc() = im;
        const nat m = support1.size();
//display << k << m << NL;
        CmbArray<bool, 10> act ( m );
        CmbArray<Plane3d, 10> plane1 ( m );
        for ( i = 0; i < m; ++i )
        {
            plane1[i] = plane[support1[i]];
            act[i] = true;
        }
        // Вычислим начальное приближение ( taff )
        Def<Sphere3d> sphere = maxSphereInConvexPolyhedron ( dim, plane1 );
        if ( ! sphere.isDef ) return res;
        Affin3d taff ( Conform3d ( sphere.o, sphere.r ) );
        // Поиск оптимального преобразования
        if ( ! maxEllipsoidInConvexPolyhedronV ( plane1, act, storNd, taff ) )
        {
            Polyhedron p1, p2;
            p1 = poly0;
            for ( i = 0; i < m; ++i )
            {
                if ( support1[i] >= n ) continue;
                cut ( p1, plane1[i], p2, stor3d );
                _swap ( p1, p2 );
            }
            const Affin3d stand2 = getEllipsoidPlg ( p1 ).getAffin3d();
            const Affin3d back = ~stand2;
            plane1 *= back;
            for ( i = 0; i < vert.size(); ++i ) vert[i] = back ( poly0.vertex[i] );
            Def<Sphere3d> sphere = maxSphereInConvexPolyhedron ( dimensions ( vert ), plane1 );
            if ( ! sphere.isDef ) return res;
            Affin3d taff ( Conform3d ( sphere.o, sphere.r ) );
            if ( ! maxEllipsoidInConvexPolyhedronV ( plane1, act, storNd, taff ) )
                return res;
            aff = stand2 * taff;
        }
        else
            aff = taff;
        // Удаляем лишние ограничения
        support2.resize();
        for ( i = 0; i < m; ++i )
        {
            if ( act[i] ) support2.inc() = support1[i];
        }
        support1 = support2;
    }
/*{
    getEllipsoidNearPlanes ( temp, dim, act, aff );
    Polyhedron p;
cut ( tetra, temp.las(), p );
cut ( p, temp.las(1), p );
draw ( p, 1, 1, 0, 1, VM_WIRE );
}*/
//    return aff ( Sphere3d ( 1, null3d ) );
    return res;
}

void maxEllipsoidInConvexPolyhedron_test0 ()
{
    static PRand rand;
    static PRandVector3d vrand;
    Polyhedron poly, temp;
    nat i;
    poly.makeTetrahedron(0.5);
    for ( i = 0; i < 15; ++i )
    {
        _swap ( temp, poly );
        cut ( temp, Plane3d ( vrand(), -0.25 - 0.1*rand() ), poly );
    }
//    for ( i = 0; i < 3; ++i )
    randPolyhedron ( 6, poly );
//display << poly.facet.size() << NL;
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    Def<Ellipsoid3d> el = maxEllipsoidInConvexPolyhedronV ( poly );
    if ( el.isDef )
        draw ( el, 1, 0, 0, 1, VM_WIRE );
//display <<  NL;
}

void maxEllipsoidInConvexPolyhedron_test ()
{
    Polyhedron poly;
    for ( nat j = 0; j < 30; ++j )
    {
        randPolyhedron ( j % 5 + 5, poly );
//if ( j != 24 ) continue;
            double t1 = timeInSec();
        Def<Ellipsoid3d> el = maxEllipsoidInConvexPolyhedronV ( poly );
            double t2 = timeInSec();
        if ( el.isDef )
        {
            double t3 = timeInSec();
            Def<Ellipsoid3d> sph = maxSpheroidInConvexPolyhedronV ( poly );
            double t4 = timeInSec();
            display << j << volume ( el ) - volume ( sph ) << t2-t1 << t4-t3 << NL;
        }
        else
        {
            display << "err" << j << NL;
            maxEllipsoidInConvexPolyhedronV ( poly );
        }
    }
display << "end" << NL;
}

void formula()
{
// ( ax + dy + fz + h )^2 + ( by + ez + g )^2 + ( c*z * p )^2 = 3
// A*x*x + B*y*y + C*z*z + 2*D*( xy - z ) + 2*E*( yz - x ) + 2*F*(zx - y ) + R = 3
// R = 3 - h * h - g * g - p * p;
    static PNormalRand rand;
    const double a = fabs ( rand() );
    const double b = fabs ( rand() );
    const double c = fabs ( rand() );
    const double d = rand();
    const double e = rand();
    const double f = rand();
    const double h = rand();
    const double g = rand();
    const double p = rand();

    const double A = a * a;
    const double B = b * b + d * d;
    const double C = c * c + e * e + f * f;
    const double D = a * d;
    const double E = d * f + b * e;
    const double F = a * f;
    const double H = a * h;
    const double G = d * h + b * g;
    const double P = f * h + e * g + c * p;

    const double M2 = A * B - D * D;
    const double M3 = C * M2 + E * ( D * F - A * E ) + F * ( D * E - B * F );

    const double a1 = sqrt ( A );
    const double b1 = sqrt ( M2 / A );
    const double c1 = sqrt ( M3 / M2 );
    const double d1 = D / sqrt(A);
    const double e1 = ( A*E - D*F ) / sqrt(A*M2);
    const double f1 = F / sqrt(A);
    const double h1 = H / sqrt(A);
    const double g1 = ( A*G - D*H ) / sqrt(A*M2);
    const double p1 = ( P - F*H/A - ( A*E - D*F ) * ( A*G - D*H ) / (A*M2) ) / c1;

    const double da = a1 - a;
    const double db = b1 - b;
    const double dc = c1 - c;
    const double dd = d1 - d;
    const double de = e1 - e;
    const double df = f1 - f;
    const double dh = h1 - h;
    const double dg = g1 - g;
    const double dp = p1 - p;
}

void formula2()
{
    static PNormalRand rand;
    double a = fabs ( rand() ) + 0.1;
    double b = fabs ( rand() ) + 0.1;
    double c = fabs ( rand() ) + 0.1;
    double d = rand();
    double e = rand();
    double f = rand();
    double h = - ( d * f + b * e ) / a;
    double g = - ( a * f + d * h ) / b;
    double p = - ( a * d + f * h + e * g ) / c;
    double coef = sqrt ( 3 / ( a * a + b * b + c * c + d * d + e * e + f * f + h * h + g * g + p * p ) );
    a *= coef;
    b *= coef;
    c *= coef;
    d *= coef;
    e *= coef;
    f *= coef;
    h *= coef;
    g *= coef;
    p *= coef;
    double e1 = _pow2 (  a + d + f + h ) + _pow2 (  b + e + g ) + _pow2 (  c + p );
    double e2 = _pow2 ( -a - d + f + h ) + _pow2 ( -b + e + g ) + _pow2 (  c + p );
    double e3 = _pow2 (  a - d - f + h ) + _pow2 ( -b - e + g ) + _pow2 ( -c + p );
    double e4 = _pow2 ( -a + d - f + h ) + _pow2 (  b - e + g ) + _pow2 ( -c + p );

    const double A = a * a;
    const double B = b * b + d * d;
    const double C = c * c + e * e + f * f;
    const double D = a * d;
    const double E = d * f + b * e;
    const double F = a * f;
    const double H = a * h;
    const double G = d * h + b * g;
    const double P = f * h + e * g + c * p;
    const double R = 3 - h * h - g * g - p * p;

    const double M2 = A * B - D * D;
    const double M3 = C * M2 + E * ( D * F - A * E ) + F * ( D * E - B * F );

        Affin3d aff;
        double m1 = sqrt ( A );
        double m2 = sqrt ( M2 );
        double m3 = sqrt ( M3 );
        double w1 = A * E - D * F;
        double w2 = D * E - A * F;
        aff.t.x = Vector3d ( A, D, F ) / m1; 
        aff.t.y = Vector3d ( 0, m2/m1, w1 / ( m1*m2 ) );
        aff.t.z = Vector3d ( 0, 0, m3 / m2 );
        aff.s = Vector3d ( -E/m1, w2 / (m1*m2), ( ( F*E - A*D)*M2 - w1 * w2 )/(A*m2*m3) );
        const Vector3d u1 (  1,  1,  1 );
        const Vector3d u2 ( -1, -1,  1 );
        const Vector3d u3 ( -1,  1, -1 );
        const Vector3d u4 (  1, -1, -1 );
        const double t1 = qmod ( aff ( u1 ) );
        const double t2 = qmod ( aff ( u2 ) );
        const double t3 = qmod ( aff ( u3 ) );
        const double t4 = qmod ( aff ( u4 ) );

    const double da = sqrt ( A ) - a;
    const double db = sqrt ( M2 / A ) - b;
    const double dc = sqrt ( M3 / M2 ) - c;
    const double dd = D / sqrt(A) - d;
    const double de = ( A*E - D*F ) / sqrt(A*M2) - e;
    const double df = F / sqrt(A) - f;
    const double dh = H / sqrt(A) - h;
    const double dg = ( A*G - D*H ) / sqrt(A*M2) - g;
    const double dp = ( P - F*H/A - ( A*E - D*F ) * ( A*G - D*H ) / (A*M2) ) / c - p;
}

void minEllipsoid_test1 ()
{//formula2();return;
    Suite<Vector3d> inner;
    static PRandVector3d rand;
//    for ( nat i = 0; i < 5; ++i )
    ( inner.resize ( 12 ) << rand ) *= 0.8;
/*    inner[0] = Vector3d(1,1,1);
    inner[1] = Vector3d(-1,-1,1);
    inner[2] = Vector3d(-1,1,-1);
    inner[3] = Vector3d(1,-1,-1);
    for012 ( inner ) *= 0.8;*/
    double t0 = timeInSec();
    Def<Ellipsoid3d> elli = minEllipsoidV ( inner );
    double t1 = timeInSec();
//display << "time =" << t1 - t0 << NL;
    if ( elli.isDef )
    {
        Affin3d aff = elli.getAffin3d();
        aff = ~aff;
        double max = 0;
        for ( nat i = 0; i < inner.size(); ++i )
        {
            double t = qmod ( aff ( inner[i] ) );
        //display << t - 1 << NL;
            if ( max < t ) max = t;
        }
        display << "max =" << max - 1 << NL;
    }
    draw ( elli, 0, 1, 1, 1, VM_WIRE );
    drawPoints ( inner, 1, 0, 0 );
}

void minEllipsoid_test ()
{
    Suite<Vector3d> inner;
    static PRandPoint3d rand;
    double t0 = timeInSec();
    for ( nat k = 0; k < 9000; ++k )
    {
        ( inner.resize ( 12 ) << rand ) *= 0.8;
        Def<Ellipsoid3d> elli = minEllipsoidV ( inner );
        if ( elli.isDef )
        {
            Affin3d aff = elli.getAffin3d();
            aff = ~aff;
            double max = 0;
            for ( nat i = 0; i < inner.size(); ++i )
            {
                double t = qmod ( aff ( inner[i] ) );
                if ( max < t ) max = t;
            }
            max -= 1;
            if ( max > 3e-11 )
            {
                minEllipsoidV ( inner );
                display << k << max << NL;
                draw ( elli, 0, 1, 1, 1, VM_WIRE );
                drawPoints ( inner, 1, 0, 0 );
                break;
            }
        }
        else
            display << k << "err" << NL;
    }
    double t1 = timeInSec();
    display << "end" << t1 - t0 << NL;
}

void minCuboidAroundConvexPolyhedronV_test1()
{
    Polyhedron inner, outerV, outerA;
    Cuboid3d paraV = minCuboidAroundConvexPolyhedronV ( randPolyhedron ( 9, inner ) );
    outerV = paraV;
    Cuboid3d paraA = minCuboidAroundConvexPolyhedronA ( inner );
    outerA = paraA;
    display << ::volume ( outerV ) << getArea ( outerV ) << NL;
    display << ::volume ( outerA ) << getArea ( outerA ) << NL << NL;
    drawPoints ( inner.vertex, 1, 0, 0 );
//draw ( inner, 1, 0, 0, 1, VM_WIRE );
    draw ( outerV, 0, 1, 1, 1, VM_WIRE );
    draw ( outerA, 1, 0, 0, 1, VM_WIRE );
}

void minCuboidAroundConvexPolyhedronV_test2()
{
    Polyhedron inner;
    for ( nat n = 100; n <= 1000; n+=10 )
    {
        //randPolyhedron ( n, inner );
        inner.makeSphere ( n, 1 );
        double t1 = timeInSec();
        minCuboidAroundConvexPolyhedronV ( inner );
        double t2 = timeInSec();
        display << inner.vertex.size() << 100*(t2 - t1) << NL;
    }
}

void minRegularTetrahedronAroundPoints_test1()
{
    Suite<Vector3d> inner;
    static PRandPoint3d rand;
 //for ( nat i = 0; i < 16; ++i )
    ( inner.resize ( 6 ) << rand ) *= 0.8;
    if(0)
    {
        const double r = 0.5;
        const double m = -r;
        inner[0] = Vector3d ( r, m, r );
        inner[1] = Vector3d ( r, r, m );
        inner[2] = Vector3d ( m, r, r );
        inner[3] = Vector3d ( m, m, m );
        static PRand prand;
        inner *= getRandOrtho3d ( prand(), prand(), prand() );
    }
    drawPoints ( inner, 1, 1, 0 );
    Polyhedron outer;
    double t0 = timeInSec();
    minRegularTetrahedronAroundPoints ( inner, outer );
    display << timeInSec() - t0 << outer.volume() << NL;
    draw ( outer, 0, 1, 1, 1, VM_WIRE );
}

void maxFixSolidOfRevolutionInConvexPolyhedron_test ()
{
    static PRandPoint3d rand;
    static Polyhedron poly;
//    if ( poly.facet.size() == 0 )
    randPolyhedron ( 19, poly );
//    poly.Cuboid ( 0.5, 0.5, 0.5+fabs(rand().x) );
//    poly *= Spin3d ( rand(), rand() );
    const double u = 1.1;
    double t1 = timeInSec();
    Def<Cylinder3d> fig = maxFixCylinderInConvexPolyhedron ( poly, u );
//    Def<Cone3d> fig = maxFixConeInConvexPolyhedron ( poly, u );
//    Ellipsoid3d fig = maxFixSpheroidInConvexPolyhedron ( poly, u );
//    if ( fig.o.isDef )
    if ( fig.isDef )
    {
        display << timeInSec() - t1 << NL;
//        display << timeInSec() - t1 << 1000*fig.conf.magn << NL;
        draw ( poly, 0, 1, 1, 1, VM_WIRE );
        draw ( fig, 1, 0.7f, 0.3f, 1, VM_WIRE );
    }
    else
    {
        display << timeInSec() - t1 << NL;
        display << "err" << NL;
    }
}

void minCylinder_test()
{
    Suite<Vector3d> inner;
    static PRandPoint3d rand;
 //for ( nat i = 0; i < 16; ++i )
    ( inner.resize ( 6 ) << rand ) *= 0.8;
    drawPoints ( inner, 1, 0, 0 );
    double t0 = timeInSec();
    const Def<Cylinder3d> res = minCylinderR ( inner );
    if ( res.isDef )
    {
        Line3d line ( Ortho3d(res.spin)(Vector3d(0,0,1)), res.o );
        double m = 0;
        for ( nat i = 0; i < inner.size(); ++i )
        {
            const double t = line.qdis ( inner[i] );
            if ( m < t ) m = t;
        }
        display << "d =" << sqrt(m) - res.r << NL;
    }
    else
        display << "err" << NL;
    draw ( res, 0, 1, 1, 1, VM_WIRE );
}

void maxCylinder_test ()
{
    static PRandPoint3d rand;
    static Polyhedron poly;
    randPolyhedron ( 19, poly );
    double t0 = timeInSec();
    Def<Cylinder3d> fig = maxCylinderInConvexPolyhedronV ( poly );
//    Def<Cone3d> fig = maxConeInConvexPolyhedronV ( poly );
//    Def<Ellipsoid3d> fig = maxSpheroidInConvexPolyhedronV ( poly );
    double t1 = timeInSec();
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
//    draw ( fig, 1, 0.7f, 0.3f, 1, VM_FLAT );
    draw ( fig, 1, 0.7f, 0.3f, 1, VM_WIRE );
display << "time =" << t1 - t0 << NL;
}

void maxPolyhedronInConvexPolyhedron1R_test()
{
    double time = 0;
    static PRand rand;
    static PRandVector3d vrand;
    FixArray<Vector2d, 6> vert;
    regularPolygon ( vert );
//    for ( nat k = 0; k < 500; ++k )
    {
        const nat np = 27;//4 + prand.number(nn-3);
        Polyhedron inner, outer;
        //randPolyhedron ( np, inner );
        if(0)
        {
            RealFile file1 ( "data/sample_helium_princess.pyh", "rb" );
            loadPYH (  file1, inner );
            inner *= getRandOrtho3d ( rand(), rand(), rand() );
            RealFile file2 ( "data/octo0684.pyh", "rb" );
            loadPYH (  file2, outer );
            outer *= 0.2;
        }
        //outer.makeCube ( 0.2 );
        randPolyhedron ( 1000, outer );
        inner.makePrism ( vert, 0.4 );
        inner += Vector3d ( 0.1, 0.2, 0.3 );
//draw ( inner, 1, 1, 0, 1, VM_WIRE );
        draw ( outer, 0, 1, 1, 0, VM_WIRE );
        const Vector3d axis = vrand();
        //const Vector3d axis (0,0,1);
        double t1 = timeInSec();
        Def<Conform3d> conf = maxPolyhedronInConvexPolyhedron1R ( inner, axis, 6, outer );
        if ( conf.isDef )
        {
            double t2 = timeInSec();
            time += t2 - t1;
            inner *= conf;
            draw ( inner, 1, 0, 0, 1, VM_WIRE );
            Polyhedron inner2;
            inner2 = inner;
            draw ( inner, 1, 0, 0, 1, VM_WIRE );
            double t3 = timeInSec();
            //Def<Conform3d> conf2 = 
            for(nat i=0; i<100; ++i) maxPolyhedronInConvexPolyhedronNR ( inner, outer );
            double t4 = timeInSec();
display << (t2 - t1) / (t4 - t3) << NL;
            //inner2 *= conf2;
            //draw ( inner, 1, 1, 0, 1, VM_WIRE );
        }
        else
        {
            display << "error" << NL;
        }
    }
    display << "end" << time << NL;
}

Line3d getLineU ( CArrRef<Vector3d> data, double & r )
{
    Line3d res;
    const nat n = data.size();
// Вырожденные случаи
    if ( n == 0 )
    {
        res.point = null3d;
        res.dir = Vector3d ( 0, 0, 1 );
        r = 0;
        return res;
    }
    if ( n == 1 )
    {
m1:     res.point = data[0];
        res.dir = Vector3d ( 0, 0, 1 );
        r = 0;
        return res;
    }
    if ( n == 2 )
    {
m2:     Vector3d v = data[1] - data[0];
        if ( ! v ) goto m1;
        res.point = 0.5 * ( data[0] + data[1] );
        res.dir = v.setNorm2();
        r = 0;
        return res;
    }
// Поиск первой начальной точки
    nat i, i0 = 0;
    double max = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[i] - data[0] );
        if ( max < t ) max = t, i0 = i;
    }
    if ( max == 0 ) goto m1;
    const Vector3d & p0 = data[i0];
// Поиск второй начальной точки
    nat i1 = i0;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( data[i] - p0 );
        if ( max < t ) max = t, i1 = i;
    }
// Поиск третьей начальной точки
    Vector3d v = data[i1] - p0;
    nat i2 = i0;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( ( data[i] - p0 ) % v );
        if ( max < t ) max = t, i2 = i;
    }
    if ( max == 0 || i2 == i1 || i2 == i0 ) goto m2;
// Найдём ответ для трёх найденных точек
    if ( qmod ( data[i2] - data[i1] ) > qmod ( data[i1] - data[i0] ) )
    {
        _swap ( i0, i2 );
    }
    res.point = 0.5 * ( data[i0] + data[i2] );
    v = data[i1] - data[i0];
    double d = norm2 ( v );
    res.dir = v / d;
    r = sqrt ( res.qdis ( data[i2] ) );
// Проверим остальные точки
    nat im = 0;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = res.qdis ( data[i] );
        if ( max < t ) max = t, im = i;
    }
    max = sqrt ( max );
    const double eps = 1e-6 * d;
    if ( max < r + eps || im == i2 || im == i1 || im == i0 ) return res;
// По простому не получилось, будем искать ответ по сложному
//display << "no" << NL;
    const double coef = 1 / r;
    const Vector3d o = res.point;
    DynArray<Vector3d> temp ( data.size() );
    for ( i = 0; i < data.size(); ++i ) temp[i] = ( data[i] - o ) * coef;
// Инициализация области допустимых преобразований
    WireModel<6> model;
    List< Vertex<6> > stor;
    model.simplex ( 2*(6+1), stor );
    Double<6> dn;
    dn.fill ( 2 );
    dn.d0 = 0;
    model.vlist -= dn;
    for ( i = 0;; ++i )
    {
        model.vlist.top();
        do
        {
            Vertex<6> * p = model.vlist.cur();
            const Double<6> & pc = p->coor;
            const double t = pc.d0 * pc.d3 + pc.d1 * pc.d4 + pc.d2 * pc.d5;
            p->data = fabs ( t ) < 1e-14 ? 0 : t;
        }
        while ( model.vlist.next() );
        Double<6> best;
        max = 0;
        model.vlist.top();
        do
        {
            const Vertex<6> * p = model.vlist.cur();
            const Double<6> & pc = p->coor;
            if ( p->data > 0 ) continue;
            if ( ! p->data )
            {
                const double t = pc.d0 * pc.d0 + pc.d1 * pc.d1 + pc.d2 * pc.d2;
                if ( max < t ) max = t, best = pc;
            }
            else
            {
                const Vector3d & a1 = (const Vector3d &) pc.d0;
                const Vector3d & a2 = (const Vector3d &) pc.d3;
                for ( nat k = 0; k < 6; ++k )
                {
                    const Vertex<6> * v = p->vertex[k];
                    if ( v->data <= 0 ) continue;
                    const Double<6> & vc = v->coor;
                    const Vector3d & b1 = (const Vector3d &) vc.d0;
                    const Vector3d & b2 = (const Vector3d &) vc.d3;
                    const double C = a1 * a2;
                    const double ab = a1 * b2 + a2 * b1;
                    const double bb = b1 * b2;
                    const double A = C - ab + bb;
                    const double B = ab - C - C;
                    const double D = B * B - 4 * A * C;
                    const double t = 2 * C / ( B + ( D > 0 ? sqrt ( D ) : 0 ) );
                    const double t1 = t + 1;
                    const double e0 = a1.x * t1 - b1.x * t;
                    const double e1 = a1.y * t1 - b1.y * t;
                    const double e2 = a1.z * t1 - b1.z * t;
                    const double p = e0 * e0 + e1 * e1 + e2 * e2;
                    const double e3 = a2.x * t1 - b2.x * t;
                    const double e4 = a2.y * t1 - b2.y * t;
                    const double e5 = a2.z * t1 - b2.z * t;
                    if ( p > max )
                    {
                        max = p;
                        best.d0 = e0;
                        best.d1 = e1;
                        best.d2 = e2;
                        best.d3 = e3;
                        best.d4 = e4;
                        best.d5 = e5;
                    }
                }
            }
        }
        while ( model.vlist.next() );
        max = 0;
        nat jm = 0;
        const Vector3d & a = (const Vector3d &) best.d0;
        const Vector3d & b = (const Vector3d &) best.d3;
        for ( nat j = 0; j < n; ++j )
        {
            const double t = qmod ( a % temp[j] + b );
            if ( max < t ) max = t, jm = j;
        }
        max = sqrt ( max );
        if ( max < 1 + 1e-6 || i == 100 )
        {
            r = 1 / norm2 ( a );
            res.dir = a * r;
            res.point = ( res.dir % b ) * r;
            res.point = res.point / coef + o;
            r /= coef;
            break;
        }
        const Vector3d & p = temp[jm];
        const double qx = ( a.y * p.z - a.z * p.y + b.x );
        const double qy = ( a.z * p.x - a.x * p.z + b.y );
        const double qz = ( a.x * p.y - a.y * p.x + b.z );
        Double<7> cor;
        cor.d0 = qz * p.y - qy * p.z;
        cor.d1 = qx * p.z - qz * p.x;
        cor.d2 = qy * p.x - qx * p.y;
        cor.d3 = qx;
        cor.d4 = qy;
        cor.d5 = qz;
        cor.d6 = -max;
        double d = cor % best;
        model.cut ( cor, stor );
    }
    return res;
}

inline void check ( const Vector3d & a1, const Vector3d & a2,
                    const Vector3d & b1, const Vector3d & b2,
                    const Vector3d & c1, const Vector3d & c2,
                    double t, double & max, Double<9> & best )
{
    const Vector3d a = a1 + b2 * t;
    const Vector3d b = a1 + b2 * t;
    const Vector3d c = c1 + c2 * t;
    const double v = a % b * c;
    if ( v > max )
    {
        max = v;
        best.d0 = a.x;
        best.d1 = a.y;
        best.d2 = a.z;
        best.d3 = b.x;
        best.d4 = b.y;
        best.d5 = b.z;
        best.d6 = c.x;
        best.d7 = c.y;
        best.d8 = c.z;
    }
}

static bool maxVolume ( const WireModel<9> & model, Double<9> & best )
{
    Double<9> dt;
    Vector3d & a = ( Vector3d & ) dt.d0;
    Vector3d & b = ( Vector3d & ) dt.d3;
    Vector3d & c = ( Vector3d & ) dt.d6;
    Show< Vertex<9> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Vertex<9> * v0 = show.cur();
        const Double<9> & p0 = v0->coor;
        const Vector3d & a0 = ( const Vector3d & ) p0.d0;
        const Vector3d & b0 = ( const Vector3d & ) p0.d3;
        const Vector3d & c0 = ( const Vector3d & ) p0.d6;
        const double t = a0 % b0 * c0;
        if ( max < t ) max = t, best = p0;
        for ( nat i = 0; i < 8; ++i )
        {
            const Vertex<9> * vi = v0->vertex[i];
            const Double<9> & pi = vi->coor;
            const Vector3d & ai = ( const Vector3d & ) pi.d0;
            const Vector3d & bi = ( const Vector3d & ) pi.d3;
            const Vector3d & ci = ( const Vector3d & ) pi.d6;
            for ( nat j = i+1; j < 9; ++j )
            {
                const Vertex<9> * vj = v0->vertex[j];
                const Double<9> & pj = vj->coor;
                const Vector3d & aj = ( const Vector3d & ) pj.d0;
                const Vector3d & bj = ( const Vector3d & ) pj.d3;
                const Vector3d & cj = ( const Vector3d & ) pj.d6;
                a = ( a0 + ai + aj ) / 3;
                b = ( b0 + bi + bj ) / 3;
                c = ( c0 + ci + cj ) / 3;
                const double t = a % b * c;
                if ( max < t )
                    max = t, best = dt;
            }
        }
    }
    while ( show.next() );
    return max > 0;
}

struct MaxVolumePar
{
    const List< Vertex<9> > * vlist;
    Double<9> best;
    double max;
    nat nk, ik;
};


DWORD WINAPI maxVolumePara ( LPVOID lpParam ) 
{
    MaxVolumePar * par = ( MaxVolumePar * ) lpParam;
    Show< Vertex<9> > show ( par->vlist[0] );
    if ( ! show.top() ) return 0;
    Double<9> & best = par->best;
    const nat ik = par->ik;
    const nat nk = par->nk;
    Double<9> dt;
    Vector3d & a = ( Vector3d & ) dt.d0;
    Vector3d & b = ( Vector3d & ) dt.d3;
    Vector3d & c = ( Vector3d & ) dt.d6;
    double max = 0.;
    const nat n = show.size();
    for ( nat k = 0; k < n; ++k )
    {
        if ( k % nk == ik )
        {
            const Vertex<9> * v0 = show.cur();
            const Double<9> & p0 = v0->coor;
            const Vector3d & a0 = ( const Vector3d & ) p0.d0;
            const Vector3d & b0 = ( const Vector3d & ) p0.d3;
            const Vector3d & c0 = ( const Vector3d & ) p0.d6;
            const double t = a0 % b0 * c0;
            if ( max < t ) max = t, best = p0;
            for ( nat i = 0; i < 8; ++i )
            {
                const Vertex<9> * vi = v0->vertex[i];
                const Double<9> & pi = vi->coor;
                const Vector3d & ai = ( const Vector3d & ) pi.d0;
                const Vector3d & bi = ( const Vector3d & ) pi.d3;
                const Vector3d & ci = ( const Vector3d & ) pi.d6;
                for ( nat j = i+1; j < 9; ++j )
                {
                    const Vertex<9> * vj = v0->vertex[j];
                    const Double<9> & pj = vj->coor;
                    const Vector3d & aj = ( const Vector3d & ) pj.d0;
                    const Vector3d & bj = ( const Vector3d & ) pj.d3;
                    const Vector3d & cj = ( const Vector3d & ) pj.d6;
                    a = ( a0 + ai + aj ) / 3;
                    b = ( b0 + bi + bj ) / 3;
                    c = ( c0 + ci + cj ) / 3;
                    const double t = a % b * c;
                    if ( max < t )
                        max = t, best = dt;
                }
            }
        }
        show.next();
    }
    par->max = max;
    return 0;
}

static bool maxVolume ( const List< Vertex<9> > & vlist, Double<9> & best )
{
    nat i;
    const nat nt = 4;
    HANDLE handle[nt];
    DWORD threadId[nt];
    MaxVolumePar par[nt];
    for ( i = 0; i < nt; ++i )
    {
        MaxVolumePar & p = par[i];
        p.vlist = & vlist;
        p.nk = nt;
        p.ik = i;
        handle[i] = CreateThread ( 0, 0, maxVolumePara, par+i, 0, threadId+i );
    }
    WaitForMultipleObjects ( nt, handle, TRUE, INFINITE );
    double max = 0;
    for ( i = 0; i < nt; ++i )
    {
        CloseHandle ( handle[i] );
        const MaxVolumePar & p = par[i];
        if ( max < p.max ) max = p.max, best = p.best;
    }
    return max > 0;
}

bool maxPolyhedronInConvexPolyhedronNM ( const Polyhedron & inner, const Polyhedron & outer, LinTran3d & res )
{
    CCArrRef<Vector3d> & vert = inner.vertex;
    CCArrRef<Facet> & facet = outer.facet;
    const double d = 99;
    WireModel<9> model;
    List< Vertex<9> > stor;
    model.simplex ( d*9, stor );
    model.vlist -= Double<9>().fill(d);
    Double<10> g;
time1=0, time2=0, time3=0, time4=0;
    for ( nat i = 0; i < 120; ++i )
    {
double t0 = timeInSec();
// Поиск максимального решения
        Double<9> best;
//        if ( ! maxVolume ( model, best ) ) return false;
        if ( ! maxVolume ( model.vlist, best ) ) return false;
// Поиск максимального нарушения ограничений для выбранного решения
double t1 = timeInSec();time1 += t1-t0;
display << model.vlist.size() << t1-t0 << NL;
        nat km;
        Vector3d pm;
        double max = 0.;
        for ( nat j = 0; j < vert.size(); ++j )
        {
            const Vector3d & v = vert[j];
            const Vector3d u ( best.d0*v.x + best.d1*v.y + best.d2*v.z,
                               best.d3*v.x + best.d4*v.y + best.d5*v.z,
                               best.d6*v.x + best.d7*v.y + best.d8*v.z );
            for ( nat k = 0; k < facet.size(); ++k )
            {
                const double t = facet[k].plane % u;
                if ( max < t ) max = t, pm = v, km = k;
            }
        }
double t2 = timeInSec();time2 += t2-t1;
// Если нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            res.x = Vector3d ( best.d0, best.d1, best.d2 ), 
            res.y = Vector3d ( best.d3, best.d4, best.d5 ), 
            res.z = Vector3d ( best.d6, best.d7, best.d8 );
//display << time1 << time2 << time3 << time4 << NL;
            return true;
        }
//0.008 0.035 0.115 0 
//display << i << max << model.vlist.size() << NL;
// Применение ограничения к области допустимых преобразований
        const Vector3d & n = facet[km].plane.norm;
        g.d0 = n.x * pm.x;
        g.d1 = n.x * pm.y;
        g.d2 = n.x * pm.z;
        g.d3 = n.y * pm.x;
        g.d4 = n.y * pm.y;
        g.d5 = n.y * pm.z;
        g.d6 = n.z * pm.x;
        g.d7 = n.z * pm.y;
        g.d8 = n.z * pm.z;
        g.d9 = facet[km].plane.dist;
        model.cut ( g, stor );
double t3 = timeInSec();time3 += t3-t2;
    }
    return false;
}

typedef Set2<double, Double<9> > VolValue9;

static bool maxVolume ( WireModelEx<9, VertexEx<9, VolValue9> > & model, Double<9> & best )
{
    Double<9> dt;
    Vector3d & a = ( Vector3d & ) dt.d0;
    Vector3d & b = ( Vector3d & ) dt.d3;
    Vector3d & c = ( Vector3d & ) dt.d6;
    double max = 0.;
    if ( model.vlist.top() )
    do
    {
        VertexEx<9, VolValue9> * v0 = model.vlist.cur();
        if ( v0->extra.a == -1e300 )
        {
            const Double<9> & p0 = v0->coor;
            const Vector3d & a0 = ( const Vector3d & ) p0.d0;
            const Vector3d & b0 = ( const Vector3d & ) p0.d3;
            const Vector3d & c0 = ( const Vector3d & ) p0.d6;
            v0->extra.a = a0 % b0 * c0;
            v0->extra.b = p0;
            for ( nat i = 0; i < 8; ++i )
            {
                const Double<9> & pi = v0->vertex[i]->coor;
                const Vector3d & ai = ( const Vector3d & ) pi.d0;
                const Vector3d & bi = ( const Vector3d & ) pi.d3;
                const Vector3d & ci = ( const Vector3d & ) pi.d6;
                for ( nat j = i+1; j < 9; ++j )
                {
                    const Double<9> & pj = v0->vertex[j]->coor;
                    const Vector3d & aj = ( const Vector3d & ) pj.d0;
                    const Vector3d & bj = ( const Vector3d & ) pj.d3;
                    const Vector3d & cj = ( const Vector3d & ) pj.d6;
                    a = ( a0 + ai + aj ) / 3;
                    b = ( b0 + bi + bj ) / 3;
                    c = ( c0 + ci + cj ) / 3;
                    const double t = a % b * c;
                    if ( v0->extra.a < t ) v0->extra.a = t, v0->extra.b = dt;
                }
            }
        }
        if ( max < v0->extra.a )
            max = v0->extra.a, best = v0->extra.b;
    }
    while ( model.vlist.next() );
    return max > 0;
}

void fillNewVert ( List< VertexEx<9, VolValue9> > & vlist )
{
    if ( vlist.top() )
    do
    {
        VertexEx<9, VolValue9> ** v = vlist.cur()->vertex;
        for ( nat i = 0; i < 9; ++i ) v[i]->extra.a = -1e300;
    }
    while ( vlist.next() );
/*    ListIter< VertexEx<9, VolValue9> > iter ( vlist );
    if ( iter.top() )
    do
    {
        VertexEx<9, VolValue9> ** v = iter.cur()->vertex;
        for ( nat i = 0; i < 9; ++i ) v[i]->extra.a = -1e300;
    }
    while ( iter.next() );*/
}

struct MaxVolumeParEx
{
    List< VertexEx<9, VolValue9> > * vlist;
    Double<9> best;
    double max;
    nat nk, ik;
};

DWORD WINAPI maxVolumeParaEx ( LPVOID lpParam ) 
{
    MaxVolumeParEx * par = ( MaxVolumeParEx * ) lpParam;
/*    ListIter< VertexEx<9, VolValue9> > iter ( * par->vlist );
    if ( ! iter.top() ) return 0;
    Double<9> & best = par->best;
    const nat ik = par->ik;
    const nat nk = par->nk;
    Double<9> dt;
    Vector3d & a = ( Vector3d & ) dt.d0;
    Vector3d & b = ( Vector3d & ) dt.d3;
    Vector3d & c = ( Vector3d & ) dt.d6;
    double max = 0.;
    const nat n = iter.size();
    for ( nat k = 0; k < n; ++k )
    {
        if ( k % nk == ik )
        {
            VertexEx<9, VolValue9> * v0 = iter.cur();
            if ( v0->extra.a == -1e300 )
            {
                const Double<9> & p0 = v0->coor;
                const Vector3d & a0 = ( const Vector3d & ) p0.d0;
                const Vector3d & b0 = ( const Vector3d & ) p0.d3;
                const Vector3d & c0 = ( const Vector3d & ) p0.d6;
                v0->extra.a = a0 % b0 * c0;
                v0->extra.b = p0;
                for ( nat i = 0; i < 8; ++i )
                {
                    const Double<9> & pi = v0->vertex[i]->coor;
                    const Vector3d & ai = ( const Vector3d & ) pi.d0;
                    const Vector3d & bi = ( const Vector3d & ) pi.d3;
                    const Vector3d & ci = ( const Vector3d & ) pi.d6;
                    for ( nat j = i+1; j < 9; ++j )
                    {
                        const Double<9> & pj = v0->vertex[j]->coor;
                        const Vector3d & aj = ( const Vector3d & ) pj.d0;
                        const Vector3d & bj = ( const Vector3d & ) pj.d3;
                        const Vector3d & cj = ( const Vector3d & ) pj.d6;
                        a = ( a0 + ai + aj ) / 3;
                        b = ( b0 + bi + bj ) / 3;
                        c = ( c0 + ci + cj ) / 3;
                        double t = a % b * c;
                        if ( v0->extra.a < t ) v0->extra.a = t, v0->extra.b = dt;
                        /*
                        a = ( ai + ai + aj ) / 3;
                        b = ( bi + bi + bj ) / 3;
                        c = ( ci + ci + cj ) / 3;
                        t = a % b * c;
                        if ( v0->extra.a < t ) v0->extra.a = t, v0->extra.b = dt;
                        a = ( aj + ai + aj ) / 3;
                        b = ( bj + bi + bj ) / 3;
                        c = ( cj + ci + cj ) / 3;
                        t = a % b * c;
                        if ( v0->extra.a < t ) v0->extra.a = t, v0->extra.b = dt;*
                    }
                }
            }
            if ( max < v0->extra.a )
                max = v0->extra.a, best = v0->extra.b;
        }
        iter.next();
    }
    par->max = max;*/
    return 0;
}

static bool maxVolume ( List< VertexEx<9, VolValue9> > & vlist, Double<9> & best )
{
    nat i;
    const nat nt = 2;
    HANDLE handle[nt];
    DWORD threadId[nt];
    MaxVolumeParEx par[nt];
    for ( i = 0; i < nt; ++i )
    {
        MaxVolumeParEx & p = par[i];
        p.vlist = & vlist;
        p.nk = nt;
        p.ik = i;
        handle[i] = CreateThread ( 0, 0, maxVolumeParaEx, par+i, 0, threadId+i );
    }
    WaitForMultipleObjects ( nt, handle, TRUE, INFINITE );
    double max = 0;
    for ( i = 0; i < nt; ++i )
    {
        CloseHandle ( handle[i] );
        const MaxVolumeParEx & p = par[i];
        if ( max < p.max ) max = p.max, best = p.best;
    }
    return max > 0;
}
/*
2.954 3.475e-003 2.389 0 
66 79620 
0.283 
end 5.613 
3.278 3.771e-003 2.481 0 
72 102110 
0.342 
end 6.064 
2.606 3.961e-003 2.009 0 
67 84186 
0.385 
end 4.853 
*/

struct LinkParEx
{
    typedef SegmentEnd<9, VertexEx<9, VolValue9> > SegmEnd;
    SegmEnd * table;
    SegmEnd ** entry;
    List< VertexEx<9, VolValue9> > * vlist;
    nat nk, ik;
    long * count;
};

//static FixArray<nat, 128> nentry;
double tim1=0, tim2=0;

DWORD WINAPI newLink1 ( LPVOID lpParam ) 
{
//double t0 = timeInSec();
    LinkParEx * par = ( LinkParEx * ) lpParam;
/*    ListIter< VertexEx<9, VolValue9> > iter ( * par->vlist );
    if ( ! iter.top() ) return 0;
    typedef SegmentEnd<9, VertexEx<9, VolValue9> > SegmEnd;
    SegmEnd  * table = par->table;
    //DynArray<SegmEnd> table ( ( 9 - 1 ) * iter.size() / 2 );
    SegmEnd ** entry = par->entry;
    const nat ik = par->ik;
    const nat nk = par->nk;
    do
    {
        VertexEx<9, VolValue9> * vert = iter.cur();
        for ( nat16 i = 0; i < 9-1; ++i )
        {
            const SegmentId<9> id ( i, vert->nfacet );
            const nat16 k = id.summa & 127;
//++nentry[k];
            if ( k % nk == ik )
            {
//double ti0 = timeInSec();
                SegmEnd ** pre = & entry[k];
                SegmEnd * sb = *pre;
                while ( sb != 0 )
                {
                    if ( sb->id == id )
                    {
                        *sb->link = vert;
                        vert->vertex[i] = sb->parent;
                        *pre = sb->next;
                        goto m1;
                    }
                    pre = &sb->next;
                    sb = sb->next;
                }
                sb = & table[InterlockedIncrement(par->count)];
                sb->id = id;
                sb->parent = vert;
                sb->link = vert->vertex + i;
                sb->next = entry[k];
                entry[k] = sb;
m1:;
//double ti1 = timeInSec();tim2 += ti1-ti0;
            }
        }
    }
    while ( iter.next() );
//double t1 = timeInSec();tim1 += t1-t0;*/
    return 0;
}

static void newLink ( List< VertexEx<9, VolValue9> > & vlist )
{
    nat i;
    const nat nt = 2;
    long count = -1;
    HANDLE handle[nt];
    DWORD threadId[nt];
    LinkParEx par[nt];
    typedef SegmentEnd<9, VertexEx<9, VolValue9> > SegmEnd;
    DynArray<SegmEnd> table ( ( 9 - 1 ) * vlist.size() / 2 );
    FixArray<SegmEnd *, 128> entry;
    entry.fill ( 0 );
    for ( i = 0; i < nt; ++i )
    {
        LinkParEx & p = par[i];
        p.vlist = & vlist;
        p.table = table();
        p.entry = entry();
        p.count = & count;
        p.nk = nt;
        p.ik = i;
        handle[i] = CreateThread ( 0, 0, newLink1, par+i, 0, threadId+i );
    }
    WaitForMultipleObjects ( nt, handle, TRUE, INFINITE );
    for ( i = 0; i < nt; ++i ) CloseHandle ( handle[i] );
}

static void newLink2 ( List< VertexEx<9, VolValue9> > & list )
{
//double t0 = timeInSec();
    const nat16 N = 9;
    typedef SegmentEnd<N, VertexEx<9, VolValue9> > SegmEnd;
    DynArray<SegmEnd> table ( ( N - 1 ) * list.size() / 2 );
    FixArray<SegmEnd *, 128> entry;
    entry.fill ( 0 );
    nat n = 0;
    list.top();
    do
    {
        VertexEx<9, VolValue9> * vert = list.cur();
        for ( nat16 i = 0; i < N-1; ++i )
        {
//double ti0 = timeInSec();
            const SegmentId<N> id ( i, vert->nfacet );
//double ti1 = timeInSec();tim2 += ti1-ti0;
            const nat16 k = id.summa & 127;
            SegmEnd ** pre = & entry[k];
            SegmEnd * sb = *pre;
            while ( sb != 0 )
            {
                if ( sb->id == id )
                {
                    *sb->link = vert;
                    vert->vertex[i] = sb->parent;
                    *pre = sb->next;
                    goto m1;
                }
                pre = &sb->next;
                sb = sb->next;
            }
            sb = & table[n++];
            sb->id = id;
            sb->parent = vert;
            sb->link = vert->vertex + i;
            sb->next = entry[k];
            entry[k] = sb;
m1:;    }
    }
    while ( list.next() );
//double t1 = timeInSec();tim1 += t1-t0;
}

bool maxPolyhedronInConvexPolyhedronNM2 ( const Polyhedron & inner, const Polyhedron & outer, LinTran3d & res )
{
//nentry.fill ( 0 );
    CCArrRef<Vector3d> & vert = inner.vertex;
    CCArrRef<Facet> & facet = outer.facet;
    const double d = 99;
    Facet2Model<9, VertexEx<9, VolValue9> > model;
//    WireModelEx<9, VertexEx<9, VolValue9> > model;
    List< VertexEx<9, VolValue9> > stor;
    model.simplex ( d*9, stor );
    model.vlist -= Double<9>().fill(d);
    model.vlist.top();
    do
    {
        model.vlist.cur()->extra.a = -1e300;
    }
    while ( model.vlist.next() );
    model.newVertFunc = fillNewVert;
    model.newLinkFunc = newLink2;
    Double<10> g;
time1=0, time2=0, time3=0, time4=0;
    for ( nat i = 0; i < 200; ++i )
    {
double t0 = timeInSec();
// Поиск максимального решения
        Double<9> best;
//        if ( ! maxVolume ( model, best ) ) return false;
        if ( ! maxVolume ( model.vlist, best ) ) return false;
// Поиск максимального нарушения ограничений для выбранного решения
double t1 = timeInSec();time1 += t1-t0;
        nat km;
        Vector3d pm;
        double max = 0.;
        for ( nat j = 0; j < vert.size(); ++j )
        {
            const Vector3d & v = vert[j];
            const Vector3d u ( best.d0*v.x + best.d1*v.y + best.d2*v.z,
                               best.d3*v.x + best.d4*v.y + best.d5*v.z,
                               best.d6*v.x + best.d7*v.y + best.d8*v.z );
            for ( nat k = 0; k < facet.size(); ++k )
            {
                const double t = facet[k].plane % u;
                if ( max < t ) max = t, pm = v, km = k;
            }
        }
double t2 = timeInSec();time2 += t2-t1;
// Если нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            res.x = Vector3d ( best.d0, best.d1, best.d2 ), 
            res.y = Vector3d ( best.d3, best.d4, best.d5 ), 
            res.z = Vector3d ( best.d6, best.d7, best.d8 );
display << time1 << time2 << time3 << time4 << NL;
//display << tim1 << tim2 << NL;
display << i << model.vlist.size() << NL;
            return true;
        }
//display << i << max << model.vlist.size() << NL;
// Применение ограничения к области допустимых преобразований
        const Vector3d & n = facet[km].plane.norm;
        g.d0 = n.x * pm.x;
        g.d1 = n.x * pm.y;
        g.d2 = n.x * pm.z;
        g.d3 = n.y * pm.x;
        g.d4 = n.y * pm.y;
        g.d5 = n.y * pm.z;
        g.d6 = n.z * pm.x;
        g.d7 = n.z * pm.y;
        g.d8 = n.z * pm.z;
        g.d9 = facet[km].plane.dist;
        model.cut ( g, stor );
    /*model.vlist.top();
    do
    {
        model.vlist.cur()->extra.a = -1e300;
    }
    while ( model.vlist.next() );*/
double t3 = timeInSec();time3 += t3-t2;
    }
    return false;
}
/*
3.000 0.013 2.661 0 
72 103906 
0.934 
end 6.019 
18.803 0.018 33.972 0 
142 552202 
0.625 
end 54.826 
16.782 0.020 31.364 0 
134 448252 
0.642 
end 49.697 

5.945 0.015 2.007 0 
70 86022 
0.934 
end 8.227 
59.908 0.023 55.501 0 
153 684770 
0.625 
end 118.027 
35.448 0.024 25.271 0 
128 439886 
0.642 
end 62.216 
*/

void maxPolyhedronInConvexPolyhedronNM_test()
{
    static PRandVector3d rand;
    const Spin3d spin ( rand(), rand() );
    double time = 0;
//    for ( nat k = 0; k < 340; ++k )
//    {
        const nat np = 8;//4 + prand.number(nn-3);
        Polyhedron inner, outer;
#if 0
        RealFile file1 ( "data/inner130007.pyh", "rb" );
        loadPYH (  file1, inner );
        RealFile file2 ( "data/outer130007.pyh", "rb" );
        loadPYH (  file2, outer );
#else
        inner.makeCube ( 1 );
        randPolyhedron ( np, outer );
        inner = outer;
        inner *= spin;
#endif
        inner -= inner.centerOfMass();
        inner *= 1 / root3 ( inner.volume() );
        outer -= outer.centerOfMass();
        outer *= 1 / root3 ( outer.volume() );
        draw ( outer, 0, 1, 1, 0, VM_WIRE );
        double t1 = timeInSec();
        LinTran3d lin;
        bool ok = maxPolyhedronInConvexPolyhedronNM2 ( inner, outer, lin );
        double t2 = timeInSec();
        time += t2 - t1;
        if ( ok ) 
        {
            inner *= lin;
            draw ( inner, 1, 1, 0, 0, VM_WIRE );
            display << inner.volume() / outer.volume() << NL;
//for ( nat k = 0; k < nentry.size(); ++k ) display << k << nentry[k] << NL;
        }
        else
        {
            display << "error" << NL;
        }
//    }
    display << "end" << time << NL;
/*
    List<VertexEx<9, VolValue9> > stor;
    Facet2Model<9, VertexEx<9, VolValue9> > model;
    model.simplex ( 1, stor );
    Double<10> g;
    g.fill(0);
    model.cut ( g, stor );*/
}

} // namespace

Conform3d maxPointsInConvexPolyhedron1 ( CCArrRef<Vector3d> & inner, const Polyhedron & outer )
{
    Conform3d best ( 0 );
    const nat nv = inner.size();
    if ( nv < 2 ) return best;
    const nat np = outer.facet.size();
    if ( nv < 4 ) return best;
    Def<Segment3d> dim2 = dimensions ( outer.vertex );
    if ( ! dim2.isDef ) return best;
    const double d1 = 2 * minSphereAroundPoints ( inner ).r;
    const double d2 = normU ( dim2 );
    if ( ! d1 || ! d2 ) return best;
    QRand q2(2), q3(3), q5(5);
    DynArray<Plane4d> plane ( np );
    DynArray<Vector3d> vert ( nv );
    const nat n = 2862;
    for ( nat k = 0; k < n; ++k )
    {
        const double a = q2();
        const double b = q3();
        const double c = q5();
        const Ortho3d ortho = getRandOrtho3d ( a, b, c );
        for ( nat j = 0; j < nv; ++j ) vert[j] = ortho ( inner[j] );
        for ( nat i = 0; i < np; ++i )
        {
            const Plane3d & p = outer.facet[i].plane;
            double max = p.norm * vert[0];
            for ( nat j = 1; j < nv; ++j )
            {
                double t = p.norm * vert[j];
                if ( max < t ) max = t;
            }
            plane[i].norm = Vector4d ( p.norm.x, p.norm.y, p.norm.z, max );
            plane[i].dist = p.dist;
        }
        const Vector4d v0 ( dim2.b.x, dim2.b.y, dim2.b.z, d2 / d1 );
        Def<Conform3d> conf = maxPolyhedronInConvexPolyhedronNR ( v0, 1e-9 * d2, plane );
        if ( conf.isDef && best.magn < conf.magn )
        {
            best.magn = conf.magn;
            best.spin = getRandSpin3d ( a, b, c );;
            best.trans = conf.trans;
        }
    }
    return best;
}

Conform3d maxPointsInConvexPolyhedron2 ( CCArrRef<Vector3d> & inner, const Polyhedron & outer )
{
    Conform3d best ( 0 );
    const nat nv = inner.size();
    if ( nv < 2 ) return best;
    const nat np = outer.facet.size();
    if ( nv < 4 ) return best;
    Def<Segment3d> dim2 = dimensions ( outer.vertex );
    if ( ! dim2.isDef ) return best;
    const double d1 = 2 * minSphereAroundPoints ( inner ).r;
    const double d2 = normU ( dim2 );
    if ( ! d1 || ! d2 ) return best;
    QRand qr(5);
    double b = 0;
    DynArray<Plane4d> plane ( np );
    DynArray<Vector3d> vert ( nv );
    const nat n = 2862;
    for ( nat k = 0; k < n; ++k )
    {
        const double a = ( 0.5 + k ) / n;
        if ( ( b += 0.61803398874989484820 ) > 1 ) b -= 1;
        const double c = qr();
        const Ortho3d ortho = getRandOrtho3d ( a, b, c );
        for ( nat j = 0; j < nv; ++j ) vert[j] = ortho ( inner[j] );
        for ( nat i = 0; i < np; ++i )
        {
            const Plane3d & p = outer.facet[i].plane;
            double max = p.norm * vert[0];
            for ( nat j = 1; j < nv; ++j )
            {
                double t = p.norm * vert[j];
                if ( max < t ) max = t;
            }
            plane[i].norm = Vector4d ( p.norm.x, p.norm.y, p.norm.z, max );
            plane[i].dist = p.dist;
        }
        const Vector4d v0 ( dim2.b.x, dim2.b.y, dim2.b.z, d2 / d1 );
        Def<Conform3d> conf = maxPolyhedronInConvexPolyhedronNR ( v0, 1e-9 * d2, plane );
        if ( conf.isDef && best.magn < conf.magn )
        {
            best.magn = conf.magn;
            best.spin = getRandSpin3d ( a, b, c );;
            best.trans = conf.trans;
        }
    }
    return best;
}

void maxPolyhedronInConvexPolyhedron2_test()
{
    double time = 0;
    static PRand rand;
    static PRandVector3d vrand;
    FixArray<Vector2d, 5> vert;
    regularPolygon ( vert );
    double sum1 = 0, sum2 = 0;
    for ( nat k = 0; k < 100; ++k )
    {
        const nat np = 27;//4 + prand.number(nn-3);
        Polyhedron inner, outer;
        //randPolyhedron ( np, inner );
        //outer.makeCube ( 0.2 );
        randPolyhedron ( 100, outer );
        inner.makePrism ( vert, 0.4 );
        inner += Vector3d ( 0.1, 0.2, 0.3 );
//draw ( inner, 1, 1, 0, 1, VM_WIRE );
//        draw ( outer, 0, 1, 1, 0, VM_WIRE );
        Affin3d aff;
        double t1 = timeInSec();
        Conform3d conf1 = maxPointsInConvexPolyhedron1 ( inner.vertex, outer );
        double t2 = timeInSec();
        if ( 0 && conf1.magn > 0 )
        {
            Polyhedron temp;
            temp = inner;
            temp *= conf1;
            draw ( temp, 1, 1, 0, 1, VM_WIRE );
        }
        double t3 = timeInSec();
        Conform3d conf2 = maxPointsInConvexPolyhedron2 ( inner.vertex, outer );
        double t4 = timeInSec();
        if ( 0 && conf2.magn > 0 )
        {
            inner *= conf2;
            draw ( inner, 1, 0, 0, 1, VM_WIRE );
        }
        sum1 += conf1.magn / conf2.magn;
        sum2 += (t2-t1) / (t4-t3);
//display << k << conf1.magn / conf2.magn << (t2-t1) / (t4-t3) << NL;
    }
    display << sum1 << sum2 << NL;
    display << "end" << NL;
}


//**************************** 31.07.2019 *********************************//
//
//     Максимальный многогранник вписанный в выпуклый многогранник 
//
//**************************** 31.07.2019 *********************************//

Def<Ellipse3d> maxEllipseInConvexPolyhedronA ( const Spin3d & spin, const Polyhedron & poly )
{
    Def<Ellipse3d> res;
    const nat n = poly.facet.size();
    if ( poly.vertex.size() < 4 || n < 4 ) return res;
    Polyhedron tetra;
    tetra.makeTetrahedron ( 1 );
    minRegularTetrahedronAroundPointsNR ( poly.vertex, Ortho3d ( spin * Spin3d ( tetra.facet[0].plane.norm, Vector3d ( 0,0,1 ) ) ), tetra );
    const Facet & facet = tetra.facet[0];
    CArrRef<Vector3d> vertex = tetra.vertex;
    res.o = ( vertex[facet.index[0]] + vertex[facet.index[1]] + vertex[facet.index[2]] ) / 3;
    res.spin = spin;
    res.a = 0.5 * norm2 ( vertex[facet.index[0]] - res.o );
    res.b = res.a;
    res.isDef = true;
    draw ( tetra, 1, 1, 0, 1, VM_WIRE );
    for (;;)
    {
// Ищем наиболее близкую прямую
        nat im;
        double min = 1e300;
        for ( nat i = 0; i < n; ++i )
        {
            const double t = 0;//e ( edge[i].line );
            if ( min > t ) min = t, im = i;
        }
        //if ( min * q > 1 - 1e-14 || hasEqu ( support1, im ) ) break;
#if 0
// Находим максимальный эллипс ограниченный текущими опорными прямыми и прямой line[im]
        const Line2d & line0 = edge[im].line;
        Def<Ellipse> ee;
        double qq = 0;
        nat i1, i2, i3, i4;
        for ( i1 = 1; i1 < support1.size(); ++i1 )
        {
            const Line2d & line1 = edge[support1[i1]].line;
            if ( ee.isDef && ee ( line1 ) * qq > 1 - 1e-14 ) continue;
            ee.isDef = false;
            for ( i2 = 0; i2 < i1; ++i2 )
            {
                const Line2d & line2 = edge[support1[i2]].line;
                if ( ee.isDef && ee ( line2 ) * qq > 1 - 1e-14 ) continue;
                support3.resize(3);
                support3[0] = support1[i2];
                support3[1] = support1[i1];
                support3[2] = im;
                insertSort123 ( support3 );
                if ( check ( edge, support3 ) )
                {
                    ee = maxEllipseIn3gon ( line0, line1, line2 );
                    if ( ! ee.isDef ) return res;
                    qq = ee.a * ee.c - ee.b * ee.b;
                    if ( qq <= 0 ) return res;
                    support2 = support3;
                }
                else
                {
                    ee.isDef = false;
                }
                for ( i3 = 0; i3 < i2; ++i3 )
                {
                    const Line2d & line3 = edge[support1[i3]].line;
                    if ( ee.isDef && ee ( line3 ) * qq > 1 - 1e-14 ) continue;
                    support3.resize(4);
                    support3[0] = support1[i3];
                    support3[1] = support1[i2];
                    support3[2] = support1[i1];
                    support3[3] = im;
                    insertSort123 ( support3 );
                    if ( check ( edge, support3 ) )
                    {
                        ee = maxEllipseIn4gon ( line0, line1, line2, line3, data4 );
                        if ( ! ee.isDef ) return res;
                        qq = ee.a * ee.c - ee.b * ee.b;
                        if ( qq <= 0 ) return res;
                        support2 = support3;
                    }
                    else
                    {
                        ee.isDef = false;
                    }
                    for ( i4 = 0; i4 < i3; ++i4 )
                    {
                        const Line2d & line4 = edge[support1[i4]].line;
                        if ( ee.isDef && ee ( line4 ) * qq > 1 - 1e-14 ) continue;
                        support3.resize(5);
                        support3[0] = support1[i4];
                        support3[1] = support1[i3];
                        support3[2] = support1[i2];
                        support3[3] = support1[i1];
                        support3[4] = im;
                        insertSort123 ( support3 );
                        if ( check ( edge, support3 ) )
                        {
                            ee = maxEllipseIn5gon ( line0, line1, line2, line3, line4, data5 );
                            if ( ! ee.isDef ) return res;
                            qq = ee.a * ee.c - ee.b * ee.b;
                            if ( qq <= 0 ) return res;
                            support2 = support3;
                        }
                        else
                        {
                            ee.isDef = false;
                        }
                    }
                }
            }
        }
        if ( ! ee.isDef ) return res;
// Если площадь эллипса не уменьшилась, то выходим из цикла
        if ( q >= qq ) break;
        q = qq;
        e = ee;
        support1 = support2;
#endif
        break;
    }
    return res;
}

//**************************** 14.09.2019 *********************************//
//
//            Минимальный тетраэдр охватывающий данные точки
//
//**************************** 14.09.2019 *********************************//

bool findMax ( CCArrRef<Double<7> > & cor, const Double<6> & func, double eps, Double<6> con[] )
{
    const nat N = 6;
    const nat n = cor.size();
    if ( !n ) return true;
    Double<N> & res = con[0];
    const nat n2 = n + n;
    for ( nat k = 0; k < n2; ++k )
    {
        double max = cor[0] % res;
        nat i, im = 0;
        for ( i = 1; i < n; ++i )
        {
            const double t = cor[i] % res;
            if ( max < t ) max = t, im = i;
        }
        if ( max <= eps )
        {
            return true;
        }
        const double dist = max;
        const Double<N> & norm = ( const Double<N> & ) cor[im];
        const double lvl = -1e-8 * sqrt ( norm * norm );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i <= N; ++i )
        {
            const Double<N> & v = con[i];
            double t = norm * v;
            if ( t > lvl ) continue;
            t = -1./ t;
            if ( !ib )
            {
                max = ( func * v ) * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double s = ( func * v ) * t;
                if ( max < s ) max = s, ib = i, sm = t;
            }
        }
        if ( !ib )
        {
            return false;
        }
        const Double<N> & v = con[ib];
        res += v * ( dist * sm );
        for ( i = 1; i <= N; ++i )
        {
            if ( i == ib ) continue;
            Double<N> & ai = con[i];
            ai += v * ( ( norm * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return false;
}

double findMax ( const Vector3d & u, CCArrRef<Double<7> > & cor, Double<6> con[] )
{
    const nat N = 6;
    Double<N> func;
    func.d0 = u.x;
    func.d1 = u.y;
    func.d2 = u.z;
    func.d3 = func.d4 = func.d5 = 0;
    con[0].d0 = u.x < 0 ? -1000 : 1000;
    con[0].d1 = u.y < 0 ? -1000 : 1000;
    con[0].d2 = u.z < 0 ? -1000 : 1000;
    con[0].d3 =  con[0].d4 = con[0].d5 = 1;
    for ( nat j = 1; j <= N; ++j )
    {
        con[j].fill(0);
        (&con[j].d0)[j-1] = (&func.d0)[j-1] < 0 ? 1 : -1;
    }
    if ( ! findMax ( cor, func, 1e-9, con ) ) return 0;
    return func * con[0];
}

bool minTetrahedronAroundPoints ( CCArrRef<Vector3d> & data, Polyhedron & poly )
{
    const nat n = data.size();
    if ( n < 4 ) return false;
    if ( n == 4 )
    {
        poly.makeTetrahedron ( 1 );
        for ( nat i = 0; i < 4; ++i ) poly.vertex[i] = data[i];
        poly.initPlanes ();
        if ( poly.volume() < 0 )
        {
            _swap ( poly.vertex[0], poly.vertex[1] );
            poly.initPlanes ();
        }
        return true;
    }
    minRegularTetrahedronAroundPointsNR ( data, Ortho3d(), poly );
    const Vector3d o = 0.25 * ( poly.vertex[0] + poly.vertex[1] + poly.vertex[2] + poly.vertex[3] );
    const double r = fabs ( poly.vertex[0].x - o.x );
    if ( ! r ) return true;
    const double g = 1 / r;
    poly -= o;
    poly *= g;
    const nat N = 6;
    Double<N> con[N+1];
    DynArray<Double<N+1> > cor ( 4*n );
    double a1 = 1, a2 = 0, a3 = 0;
    double a4 = 0, a5 = 1, a6 = 0;
    double a7 = 0, a8 = 0, a9 = 1;
    double b1 = 0, b2 = 0, b3 = 0;
    double max = 0;
    nat i, j;
    for ( nat k = 0; k < 19; ++k )
    {
    for ( i = 0; i < 4; ++i )
    {
        const Plane3d & p = poly.facet[i].plane;
        for ( j = 0; j < n; ++j )
        {
            Double<N+1> & c = cor[j+i*n];
            c.d3 = p.norm.x;
            c.d4 = p.norm.y;
            c.d5 = p.norm.z;
        }
    }
    {
        for ( j = 0; j < n; ++j )
        {
            const Vector3d v = ( data[j] - o ) * g;
            for ( i = 0; i < 4; ++i )
            {
                const Plane3d & p = poly.facet[i].plane;
                Double<N+1> & c = cor[j+i*n];
                c.d0 = p.norm.x * v.x;
                c.d1 = p.norm.x * v.y;
                c.d2 = p.norm.x * v.z;
                c.d6 = p.norm.y * ( a4*v.x + a5*v.y + a6*v.z ) + p.norm.z * ( a7*v.x + a8*v.y + a9*v.z ) + p.dist;
            }
        }
        {
            const double t = findMax ( Vector3d ( a4, a5, a6 ) % Vector3d ( a7, a8, a9 ), cor, con );
//            if ( ! t ) return false;
            if ( max < t )
            {
                max = t;
                const Double<6> & res = con[0];
                a1 = res.d0; a2 = res.d1; a3 = res.d2;
                b1 = res.d3; b2 = res.d4; b3 = res.d5;
            }
display << max << NL;
        }
        for ( j = 0; j < n; ++j )
        {
            const Vector3d v = ( data[j] - o ) * g;
            for ( i = 0; i < 4; ++i )
            {
                const Plane3d & p = poly.facet[i].plane;
                Double<N+1> & c = cor[j+i*n];
                c.d0 = p.norm.y * v.x;
                c.d1 = p.norm.y * v.y;
                c.d2 = p.norm.y * v.z;
                c.d6 = p.norm.x * ( a1*v.x + a2*v.y + a3*v.z ) + p.norm.z * ( a7*v.x + a8*v.y + a9*v.z ) + p.dist;
            }
        }
        {
            const double t = findMax ( Vector3d ( a7, a8, a9 ) % Vector3d ( a1, a2, a3 ), cor, con );
//            if ( ! t ) return false;
            if ( max < t )
            {
                max = t;
                const Double<6> & res = con[0];
                a4 = res.d0; a5 = res.d1; a6 = res.d2;
                b1 = res.d3; b2 = res.d4; b3 = res.d5;
            }
display << max << NL;
        }
        for ( j = 0; j < n; ++j )
        {
            const Vector3d v = ( data[j] - o ) * g;
            for ( i = 0; i < 4; ++i )
            {
                const Plane3d & p = poly.facet[i].plane;
                Double<N+1> & c = cor[j+i*n];
                c.d0 = p.norm.z * v.x;
                c.d1 = p.norm.z * v.y;
                c.d2 = p.norm.z * v.z;
                c.d6 = p.norm.x * ( a1*v.x + a2*v.y + a3*v.z ) + p.norm.y * ( a4*v.x + a5*v.y + a6*v.z ) + p.dist;
            }
        }
        {
            const double t = findMax ( Vector3d ( a1, a2, a3 ) % Vector3d ( a4, a5, a6 ), cor, con );
//            if ( ! t ) return false;
            if ( max < t )
            {
                max = t;
                const Double<6> & res = con[0];
                a7 = res.d0; a8 = res.d1; a9 = res.d2;
                b1 = res.d3; b2 = res.d4; b3 = res.d5;
            }
display << max << NL;
        }
        {
            const Vector3d v1 ( a2, a5, a8 );
            const Vector3d v2 ( a3, a6, a9 );
            for ( j = 0; j < n; ++j )
            {
                const Vector3d v = ( data[j] - o ) * g;
                for ( i = 0; i < 4; ++i )
                {
                    const Plane3d & p = poly.facet[i].plane;
                    Double<N+1> & c = cor[j+i*n];
                    c.d0 = p.norm.x * v.x;
                    c.d1 = p.norm.y * v.x;
                    c.d2 = p.norm.z * v.x;
                    c.d6 = v.y * ( p.norm * v1 ) + v.z * ( p.norm * v2 ) + p.dist;
                }
            }
            const double t = findMax ( v1 % v2, cor, con );
//            if ( ! t ) return false;
            if ( max < t )
            {
                max = t;
                const Double<6> & res = con[0];
                a1 = res.d0; a4 = res.d1; a7 = res.d2;
                b1 = res.d3; b2 = res.d4; b3 = res.d5;
            }
display << max << NL;
        }
        {
            const Vector3d v1 ( a3, a6, a9 );
            const Vector3d v2 ( a1, a4, a7 );
            for ( j = 0; j < n; ++j )
            {
                const Vector3d v = ( data[j] - o ) * g;
                for ( i = 0; i < 4; ++i )
                {
                    const Plane3d & p = poly.facet[i].plane;
                    Double<N+1> & c = cor[j+i*n];
                    c.d0 = p.norm.x * v.y;
                    c.d1 = p.norm.y * v.y;
                    c.d2 = p.norm.z * v.y;
                    c.d6 = v.z * ( p.norm * v1 ) + v.x * ( p.norm * v2 ) + p.dist;
                }
            }
            const double t = findMax ( v1 % v2, cor, con );
//            if ( ! t ) return false;
            if ( max < t )
            {
                max = t;
                const Double<6> & res = con[0];
                a2 = res.d0; a5 = res.d1; a8 = res.d2;
                b1 = res.d3; b2 = res.d4; b3 = res.d5;
            }
display << max << NL;
        }
        {
            const Vector3d v1 ( a1, a4, a7 );
            const Vector3d v2 ( a2, a5, a8 );
            for ( j = 0; j < n; ++j )
            {
                const Vector3d v = ( data[j] - o ) * g;
                for ( i = 0; i < 4; ++i )
                {
                    const Plane3d & p = poly.facet[i].plane;
                    Double<N+1> & c = cor[j+i*n];
                    c.d0 = p.norm.x * v.z;
                    c.d1 = p.norm.y * v.z;
                    c.d2 = p.norm.z * v.z;
                    c.d6 = v.x * ( p.norm * v1 ) + v.y * ( p.norm * v2 ) + p.dist;
                }
            }
            const double t = findMax ( v1 % v2, cor, con );
//            if ( ! t ) return false;
            if ( max < t )
            {
                max = t;
                const Double<6> & res = con[0];
                a3 = res.d0; a6 = res.d1; a9 = res.d2;
                b1 = res.d3; b2 = res.d4; b3 = res.d5;
            }
display << max << NL;
        }
    }
    }
    poly *= ~Affin3d ( Vector3d ( a1, a2, a3 ), Vector3d ( a4, a5, a6 ), Vector3d ( a7, a8, a9 ), Vector3d ( b1, b2, b3 ) );
    poly *= r;
    poly += o;
    return true;
}

void minTetrahedronAroundPoints_test1()
{
    Suite<Vector3d> inner;
    static PRandPoint3d rand;
 //for ( nat i = 0; i < 16; ++i )
    ( inner.resize ( 5 ) << rand ) *= 0.8;
    if(0)
    {
        const double r = 0.5;
        const double m = -r;
        inner[0] = Vector3d ( r, m, r );
        inner[1] = Vector3d ( r, r, m );
        inner[2] = Vector3d ( m, r, r );
        inner[3] = Vector3d ( m, m, m );
        static PRand prand;
        inner *= getRandOrtho3d ( prand(), prand(), prand() );
    }
    inner[4] = ( inner[0] + inner[1] + inner[2] + inner[3] ) / 4;
    drawPoints ( inner, 1, 1, 0 );
    Polyhedron outer;
    double t0 = timeInSec();
    if ( minTetrahedronAroundPoints ( inner, outer ) )
        display << timeInSec() - t0 << outer.volume() << NL;
    else
        display << "error" << NL;
    draw ( outer, 0, 1, 1, 1, VM_WIRE );
//    minRegularTetrahedronAroundPoints ( inner, outer );
//    draw ( outer, 1, 1, 1, 1, VM_WIRE );
//    display << outer.volume() << NL;
}

void divide4 ( const Triangle3d & trian1, Triangle3d trian2[4] );

struct MaxCuboid
{
    Double<6> func, con[7];
    FixArray<Double<7>, 3> cor;
    void init ( const Segment3d & seg, const Triangle3d & trian )
    {
        Double<6> & vn = con[0];
        vn.d0 = seg.b.x - seg.a.x;
        vn.d1 = seg.b.y - seg.a.y;
        vn.d2 = seg.b.z - seg.a.z;
        vn.d3 = seg.b.x;
        vn.d4 = seg.b.y;
        vn.d5 = seg.b.z;
        for ( nat l = 1; l <= 6; ++l )
        {
            con[l].fill(0);
            (&con[l].d0)[l-1] = -1;
        }
        {
            Triangle3d t = trian;
            t.a /= root3 ( t.a.x * t.a.y * t.a.z );
            t.b /= root3 ( t.b.x * t.b.y * t.b.z );
            t.c /= root3 ( t.c.x * t.c.y * t.c.z );
            const Vector3d u = t.getNormal();
            const double c = 1 / ( u * t.a );
            func.d0 = c * u.x;
            func.d1 = c * u.y;
            func.d2 = c * u.z;
            func.d3 = func.d4 = func.d5 = 0;
        }
        {
            const Vector3d v = ( trian.b % trian.a ).setNorm2 ( 1e6 );
            Double<7> & dn = cor[0];
            dn.d0 = v.x;
            dn.d1 = v.y;
            dn.d2 = v.z;
            dn.d3 = dn.d4 = dn.d5 = dn.d6 = 0;
        }
        {
            const Vector3d v = ( trian.c % trian.b ).setNorm2 ( 1e6 );
            Double<7> & dn = cor[1];
            dn.d0 = v.x;
            dn.d1 = v.y;
            dn.d2 = v.z;
            dn.d3 = dn.d4 = dn.d5 = dn.d6 = 0;
        }
        {
            const Vector3d v = ( trian.a % trian.c ).setNorm2 ( 1e6 );
            Double<7> & dn = cor[2];
            dn.d0 = v.x;
            dn.d1 = v.y;
            dn.d2 = v.z;
            dn.d3 = dn.d4 = dn.d5 = dn.d6 = 0;
        }
    }
};

Def<Cuboid3d> maxCuboidInConvexPolyhedron ( const Polyhedron & poly, const Spin3d & spin = Spin3d() )
{
    Def<Cuboid3d> res;
    const nat nf = poly.facet.size();
    const nat nv = poly.vertex.size();
    if ( nf < 4 || nv < 4 ) return res;
    Vector3d ax, ay, az;
    (~spin).getReper ( ax, ay, az );
    DynArray<Double<7> > cor ( nf );
    for ( nat l = 0; l < nf; ++l )
    {
        const Plane3d & p = poly.facet[l].plane;
        Double<7> & c = cor[l];
        c.d0 = fabs ( p.norm * ax );
        c.d1 = fabs ( p.norm * ay );
        c.d2 = fabs ( p.norm * az );
        c.d3 = p.norm.x;
        c.d4 = p.norm.y;
        c.d5 = p.norm.z;
        c.d6 = p.dist;
    }
    Suite<Triangle3d> trian1, trian2;
    Triangle3d & t = trian1.inc();
    t.a = Vector3d ( 0.998, 1e-3, 1e-3 );
    t.b = Vector3d ( 1e-3, 0.998, 1e-3 );
    t.c = Vector3d ( 1e-3, 1e-3, 0.998 );
    for ( nat i = 0; i < 5; ++i )
    {
        trian2.resize ( 4*trian1.size() );
        for ( nat j = 0; j < trian1.size(); ++j ) divide4 ( trian1[j], trian2(4*j) );
        trian1.swap ( trian2 );
    }
    const nat nn = trian1.size();
    DynArray<MaxCuboid> variant ( nn );
    MaxHeap<SortItem<double,nat> > heap ( nn );
    Def<Segment3d> seg = dimensions ( poly.vertex );
    for ( nat k = 0; k < nn; ++k )
    {
        MaxCuboid & var = variant[k];
        var.init ( seg, trian1[k] );
        heap << SortItem<double,nat> ( var.func * var.con[0], k );
    }
    const double eps = 1e-9;
    for ( nat j = 0; j < nn*nf; ++j )
    {
        MaxCuboid & var = variant[heap[0]->tail];
        Double<6> & r = var.con[0];
        double max = var.cor[0] % r;
        nat i, im = 0;
        Double<6> norm;
        for ( i = 1; i < 3; ++i )
        {
            const double t = var.cor[i] % r;
            if ( max < t ) max = t, im = i;
        }
        if ( max <= eps )
        {
            for ( i = 0; i < nf; ++i )
            {
                const double t = cor[i] % r;
                if ( max < t ) max = t, im = i;
            }
            if ( max <= eps )
            {
                res.a = r.d0;
                res.b = r.d1;
                res.c = r.d2;
                res.o.x = r.d3;
                res.o.y = r.d4;
                res.o.z = r.d5;
                res.spin = spin;
                res.isDef = true;
                return res;
            }
            norm = ( const Double<6> & ) cor[im];
        }
        else
            norm = ( const Double<6> & ) var.cor[im];
        const double dist = max;
        const double lvl = -1e-8 * sqrt ( norm * norm );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i <= 6; ++i )
        {
            const Double<6> & v = var.con[i];
            double t = norm * v;
            if ( t > lvl ) continue;
            t = -1./ t;
            if ( !ib )
            {
                max = ( var.func * v ) * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double s = ( var.func * v ) * t;
                if ( max < s ) max = s, ib = i, sm = t;
            }
        }
        if ( !ib )
        {
            return res;
        }
        const Double<6> & v = var.con[ib];
        r += v * ( dist * sm );
        for ( i = 1; i <= 6; ++i )
        {
            if ( i == ib ) continue;
            Double<6> & ai = var.con[i];
            ai += v * ( ( norm * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
        heap[0]->head = var.func * r;
        heap.down ( 0 );
    }
    return res;
}

void makeRRHset ( nat nsym, Suite<Triangle3d> & trian )
{
    const nat n = 16;
    DynArray<DynArray<Vector3d> > vert ( n-1 );
    for ( nat i = 0; i < n-1; ++i )
    {
        DynArray<Vector3d> & vi = vert[i];
        vi.resize ( n-i );
        for ( nat j = 0; j < vi.size(); ++j )
        {
        }
    }
}

void maxCuboidInConvexPolyhedron_test()
{
    static PRandVector3d rand;
    const Spin3d spin ( rand(), rand() );
    double time = 0;
//    for ( nat k = 0; k < 340; ++k )
//    {
        const nat np = 27;//4 + prand.number(nn-3);
        Polyhedron outer;
        randPolyhedron ( np, outer );
//        if ( k != 333 ) continue;
//        display << k << NL;
        draw ( outer, 0, 1, 1, 0, VM_WIRE );
        double t1 = timeInSec();
        //Def<Cuboid3d> cube = maxCuboidInConvexPolyhedronNR ( outer );
        Def<Tetrahedron> cube = maxTetrahedronInConvexPolyhedronA ( outer );
        double t2 = timeInSec();
        time += t2 - t1;
        if ( cube.isDef ) 
        {
            draw ( cube, 1, 1, 0, 1, VM_WIRE );
        }
        else
        {
            display << "error" << NL;
        }
//    }
    display << "end" << time << NL;
}

void opti3d_test ()
{
    drawNewList2d();
//    minCuboidAroundConvexPolyhedronV_test1();
//    minRegularTetrahedronAroundPoints_test1();
//    minTetrahedronAroundPoints_test1();
//    minEllipsoid_test();
//    maxEllipsoidInConvexPolyhedron_test();
    maxCuboidInConvexPolyhedron_test();
//    maxPolyhedronInConvexPolyhedron_test();
//    maxConvexPolyhedronInPolyhedronNR_test();
//    maxPolyhedronInConvexPolyhedron1R_test();
//    maxPolyhedronInConvexPolyhedron2_test();
//    maxSphereInConvexPolyhedron_test();
//    minSphereAroundPoints_test();
//    minSphereAroundSpheres_test2();
//    minSpherePlane_test();
//    minSphereLine_test();
//    maxCylinder_test();
//    proba ();
//    maxPolyhedronInConvexPolyhedronNM_test();
    endNewList();
}