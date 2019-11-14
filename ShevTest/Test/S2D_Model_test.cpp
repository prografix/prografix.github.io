
#include <math.h>
#include "../shev/S2D_Model.h"

#include "display.h"

double timeInSec();

#include "../shev/Vector2d.h"
#include "../shev/Vector3d.h"
#include "../shev/Polyhedron.h"
#include "../shev/func3d.h"
#include "../shev/func1t.h"
#include "../shev/file.h"

namespace
{

template <nat N> void testSimplex ( S2D_Model<N> & model )
{
    model.sides.top();
    do
    {
        S2D_Side<N> * s = model.sides.cur();
        if ( s->size() != 3 )
            display << "testSimplex: s->size() = " << s->size() << NL;
    }
    while ( model.sides.next() );
    model.edges.top();
    do
    {
        S2D_Edge<N> * e = model.edges.cur();
        const Double<N> & v0 = e->v0->base();
        const Double<N> & v1 = e->v1->base();
        for ( nat j = 0; j < N-1; ++j )
        {
            if ( ! e->side[j] )
                display << "testSimplex:" << "e->side[" << j << "] = 0" << NL;
            const Double<N+1> & plane = e->plane[j]->base();
            const double d0 = fabs ( plane % v0 );
            const double d1 = fabs ( plane % v1 );
            if ( fabs ( d0 ) > 1e-14 || fabs ( d1 ) > 1e-14 )
                display << "testSimplex: plane % v" << d0 << d1 << NL;
        }
    }
    while ( model.edges.next() );
}

template <nat N> void testModel ( S2D_Model<N> & model )
{
    nat nfe = 0;
    if ( model.sides.top() )
    do
    {
        S2D_Side<N> * s = model.sides.cur();
        if ( s->size() < 3 )
            display << "testModel: s->size() = " << s->size() << NL;
        if ( s->top() )
        do
        {
            S2D_Edge<N> * e = s->cur()->edge;
            if ( s != e->side[s->cur()-e->edgePtr] )
                display << "testModel: s != e->side" << NL;
        }
        while ( s->next() );
        nfe += s->size();
    }
    while ( model.sides.next() );
    if ( model.edges.size() * (N-1) != nfe )
        display << "!!! ne * (N-1) != nfe #" << model.edges.size() * (N-1) << nfe << NL;
    DynArray<const S2D_Vert<N> *> varr ( model.verts.size() );
    model.verts.top();
    for ( nat i = 0; i < varr.size(); ++i )
    {
        varr[i] = model.verts.cur();
        model.verts.next();
    }
    quickSort123 ( varr );
    DynArray<Suite<const S2D_Edge<N> *> > earr ( model.verts.size() );
    if ( model.edges.top() )
    do
    {
        S2D_Edge<N> * e = model.edges.cur();
        if ( e->v0 == e->v1 )
            display << "!!! e->v0 == e->v1" << NL;
        const Double<N> & v0 = e->v0->base();
        const Double<N> & v1 = e->v1->base();
        for ( nat j = 0; j < N-1; ++j )
        {
            const Double<N+1> & plane = e->plane[j]->base();
            const double d0 = fabs ( plane % v0 );
            const double d1 = fabs ( plane % v1 );
            if ( fabs ( d0 ) > 1e-14 || fabs ( d1 ) > 1e-14 )
                display << "testModel: plane % v" << d0 << d1 << NL;
            S2D_Side<N> * s = e->side[j];
            if ( model.sides.has ( s ) )
            {
                if ( ! s->has ( & e->edgePtr[j] ) )
                    display << "testModel: ! s->has ( e->edgePtr[j] )" << NL;
            }
            else
                display << "testModel: ! sides.has ( s )" << NL;
        }
        nat i0 = lasEqu123 ( varr, e->v0 );
        if ( i0 < varr.size() )
        {
            earr[i0].inc() = e;
        }
        else
            display << "!!! unknown e->v0" << NL;
        nat i1 = lasEqu123 ( varr, e->v1 );
        if ( i1 < varr.size() )
        {
            earr[i1].inc() = e;
        }
        else
            display << "!!! unknown e->v1" << NL;
    }
    while ( model.edges.next() );
    for ( nat i = 0; i < earr.size(); ++i )
    {
        Suite<const S2D_Edge<N> *> & arr = earr[i];
        nat n = arr.size();
        if ( n != N )
        {
            n = n;
            DynArray<Set2<Set4<const S2D_Plane<N> *> > > sarr ( n );
            for ( nat j = 0; j < n; ++j )
            {
                Set2<Set4<const S2D_Plane<N> *> > & s = sarr[j];
                const S2D_Edge<N> * e = arr[j];
                /*if ( e->v0 < e->v1 )
                {
                    s.a = e->v0;
                    s.b = e->v1;
                }
                else
                {
                    s.b = e->v0;
                    s.a = e->v1;
                }*/
                s.a.a = e->plane[0];
                s.a.b = e->plane[1];
                s.a.c = e->plane[2];
                s.a.d = e->plane[3];
                s.b.a = e->plane[4];
                s.b.b = e->plane[5];
                s.b.c = e->plane[6];
                s.b.d = e->plane[7];
//for ( nat k = 0; k < N-1; ++k ) display << nat(e->plane[k]);
//display << NL;
            }
            quickSort123 ( sarr );
            for ( nat j = 0; j < n; ++j )
            {
                Set2<Set4<const S2D_Plane<N> *> > & s = sarr[j];
                //display << nat(s.a.a) << nat(s.a.b) << nat(s.a.c) << nat(s.a.d) << 
                //    nat(s.b.a) << nat(s.b.b) << nat(s.b.c) << nat(s.b.d) << NL;
                printf ( display.file, "%p %p %p %p %p %p %p %p \n", 
                    s.a.a, s.a.b, s.a.c, s.a.d, s.b.a, s.b.b, s.b.c, s.b.d );
            }
display << NL;
        }
    }
    if ( model.verts.size() * N != model.edges.size() * 2 ) 
        display << "!!! nv * N != ne * 2 #" << model.verts.size() * N << model.edges.size() * 2 << NL;
    display << "nv =" << model.verts.size() << "ne =" << model.edges.size() <<"ns =" << model.sides.size() << NL;
}

} // end of namespace

#define N 9

void cutTest ( S2D_Model<N> & model, S2D_ModelStor<N> & stor )
{
    Double<N+1> dn1;
    for ( nat j = 0; j < N; ++j )
    {
        dn1.fill ( 0 );
        double * q1 = &dn1.d0;
        q1[j] = 10;
        dn1.las() = -10;
        model.cut ( dn1, stor );
    }
}/*
 time = 0.032 
0 0 0 
nv = 512 ne = 2304 ns = 4608 
end 
time = 0.031 
0 0 0 
nv = 512 ne = 2304 ns = 4608 
end 
time = 0.028 
0 0 0 
nv = 512 ne = 2304 ns = 4608 
end 
time = 0.030 
0 0 0 
nv = 512 ne = 2304 ns = 4608 
end 
time = 0.030 
0 0 0 
nv = 512 ne = 2304 ns = 4608 
end */

static double time1, time2;
static nat scount;

static void simplex ()
{
    scount = 0;
    time1 = 0, time2 = 0;
    S2D_ModelStor<N> stor;
    S2D_Model<N> model;
    model.makeSimplex ( 100, stor );
    Double<N> dn;
    dn.fill ( 1 );
    model -= dn;
/*    Double<N+1> dn1;
    dn1.fill ( 1 );
    dn1.las() = 0;*/
//    testSimplex ( model );
//    model.cut ( dn1, stor );
    double t1 = timeInSec();
    cutTest ( model, stor );
    double t2 = timeInSec();
display << "time =" << t2-t1 << NL;
display << time1 << time2 << scount << NL;
    testModel ( model );
    model.makeVoid ( stor );
    display << "end" << NL;
}

static
bool maxParallelogramInConvexPolyhedron ( CArrRef<Vector2d> poly, CArrRef<Plane3d> plane, 
                                          S2D_Model<9> & model, S2D_ModelStor<9> & stor,
                                          bool (*func) ( const S2D_Model<9> & model, Double<9> & best ),
                                          Parallelogram3d & res
                                        )
{
    for ( nat i = 0; i < 1000; ++i )
    {
// ѕоиск максимального решени€
        Double<9> best;
        if ( ! func ( model, best ) ) return false;
// ѕоиск максимального нарушени€ ограничений дл€ выбранного решени€
        nat km;
        Vector2d pm;
        double max = 0.;
        for ( nat j = 0; j < poly.size(); ++j )
        {
            const Vector2d & p = poly[j];
            const Vector3d v ( best.d6*p.y + best.d3*p.x + best.d0,
                               best.d7*p.y + best.d4*p.x + best.d1,
                               best.d8*p.y + best.d5*p.x + best.d2 );
            for ( nat k = 0; k < plane.size(); ++k )
            {
                const double t = plane[k] % v;
                if ( max < t ) max = t, pm = p, km = k;
            }
        }
// ≈сли нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
//display << i << model.verts.size() << NL;
            Vector3d a ( best.d0, best.d1, best.d2 ), 
                     b ( best.d3, best.d4, best.d5 ), 
                     c ( best.d6, best.d7, best.d8 );
            res = Parallelogram3d ( a - b - c, a + b - c, a + b + c );
            return true;
        }
// ѕрименение ограничени€ к области допустимых преобразований
        const Vector3d & n = plane[km].norm;
        Double<10> g;
        g.d0 = n.x;
        g.d1 = n.y;
        g.d2 = n.z;
        g.d3 = n.x * pm.x;
        g.d4 = n.y * pm.x;
        g.d5 = n.z * pm.x;
        g.d6 = n.x * pm.y;
        g.d7 = n.y * pm.y;
        g.d8 = n.z * pm.y;
        g.d9 = plane[km].dist;
        model.cut ( g, stor );
  //  testModel ( model );
        /*if ( model.verts.top() )
        do display << g % model.verts.cur()->base() << NL; while ( model.verts.next() );
        display << NL;*/
    }
    return false;
}

static
Def<Parallelogram3d> maxParallelogramInConvexPolyhedron ( const Polyhedron & poly, bool (*func) ( const S2D_Model<9> & model, Double<9> & best ) )
{
    Def<Parallelogram3d> res;
// ѕриведение многогранника к стандартному положению
    DynArray<Plane3d> plane ( poly.facet.size() );
    nat i;
    for ( i = 0; i < poly.facet.size(); ++i ) plane[i] = poly.facet[i].plane;
    const Segment3d seg = dimensions ( poly.vertex );
    const double max = normU ( seg );
    if ( max == 0 ) return res;
    const double coef = 2. / max;
    const Conform3d conf ( -0.5 * coef * ( seg.a + seg.b ), coef );
    plane *= Similar3d ( conf );
    FixArray<Vector2d, 4> coor;
    coor[0] = Vector2d(-1, 1);
    coor[1] = Vector2d(-1,-1);
    coor[2] = Vector2d( 1,-1);
    coor[3] = Vector2d( 1, 1);
    S2D_Model<9> model;
    S2D_ModelStor<9> stor;
    model.makeSimplex ( 2*9, stor );
    Double<9> dn;
    dn.fill ( 1. );
    dn.d3 = dn.d6 = 0;
    model -= dn;
    Double<10> g;
    g.d3 = 1;
    g.d6 = -1;
    g.d0 = g.d1 = g.d2 = g.d4 = g.d5 = g.d7 = g.d8 = g.d9 = 0;
    model.cut ( g, stor );
    if ( maxParallelogramInConvexPolyhedron ( coor, plane, model, stor, func, res ) )
    {
        res.isDef = true;
        res *= ~conf;
    }
    return res;
}

static bool maxParallelogramPerimeter ( const S2D_Model<9> & model, Double<9> & best )
{
    Show< S2D_Vert<9> > show ( model.verts );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Double<9> & pc = *show.cur();
        const double t = sqrt ( pc.d3 * pc.d3 + pc.d4 * pc.d4 + pc.d5 * pc.d5 ) +
                         sqrt ( pc.d6 * pc.d6 + pc.d7 * pc.d7 + pc.d8 * pc.d8 );
        if ( max < t ) max = t, best = pc;
    }
    while ( show.next() );
    return max > 0;
}

Def<Parallelogram3d> maxParallelogramInConvexPolyhedronP2 ( const Polyhedron & poly )
{
    return maxParallelogramInConvexPolyhedron ( poly, maxParallelogramPerimeter );
}

static bool maxParallelogramArea ( const S2D_Model<9> & model, Double<9> & best )
{
    Show< S2D_Vert<9> > show ( model.verts );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Double<9> & pc = *show.cur();
        const Vector3d a1 ( pc.d3, pc.d4, pc.d5 );
        const Vector3d a2 ( pc.d6, pc.d7, pc.d8 );
        const double t = qmod ( a1 % a2 );
        if ( max < t ) max = t, best = pc;
    }
    while ( show.next() );
    return max > 0;
}

Def<Parallelogram3d> maxParallelogramInConvexPolyhedronA2 ( const Polyhedron & poly )
{
    return maxParallelogramInConvexPolyhedron ( poly, maxParallelogramArea );
}

void S2D_Model_test ()
{
    simplex();
}