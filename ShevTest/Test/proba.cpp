
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/func2d.h"
#include "../Shev/func3d.h"
#include "../Shev/opti3d.h"
#include "../Shev/Mathem.h"
#include "../Shev/approx3d.h"
#include "../Shev/Vector3d.h"
#include "../Shev/ShevArray.h"
#include "../Shev/moment3d.h"
#include "../Shev/RealFile.h"
#include "../draw.h"

#include "display.h"

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
    double level = 0.125 * getHighLevel ( dev );
    findBigPeaks ( level, dev, peak );
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
        /*for ( j = 0; j < n; ++j )
        {
            nat l = j + g.a;
            if ( l >= nc ) l -= nc;
            const LinePart & part = layer[l];
            display << e.a.x * part.cosa + e.a.y * part.sina << part.a.x;
            display << e.b.x * part.cosa + e.b.y * part.sina << part.b.x << NL;
        }
        display << NL;*/
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
