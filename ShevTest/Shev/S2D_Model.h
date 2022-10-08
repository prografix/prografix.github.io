
#pragma once

#include "DoubleN.h"
#include "ShevList.h"
#include "ShevArray.h"
#include "AVL_Tree.h"

template <class T, nat N> class Kit
{
public:
    T a[N];

    bool operator == ( const Kit & k ) const
    {
        for ( nat j = 0; j < N; ++j ) if ( a[j] != k.a[j] ) return false;
        return true;
    }

    bool operator > ( const Kit & k ) const
    {
        for ( nat j = 0; j < N; ++j ) if ( a[j] != k.a[j] ) return a[j] > k.a[j];
        return false;
    }

    bool operator < ( const Kit & k ) const
    {
        for ( nat j = 0; j < N; ++j ) if ( a[j] != k.a[j] ) return a[j] < k.a[j];
        return false;
    }
};

template <nat N> 
class S2D_Vert : public ListItem<Double<N> >
{
public:
    double temp;

    S2D_Vert () {}
    S2D_Vert ( const Double<N> & v ) : ListItem<Double<N> > ( v ) {}
};

template <nat N> 
class S2D_Plane : public ListItem<Double<N+1> >
{
public:
    S2D_Plane & operator -= ( const Double<N> & p )
    {
        las() = base() % p;
        return *this;
    }
};

template <nat N> class S2D_Edge;
template <nat N> class S2D_Side;

template <nat N> 
class S2D_EdgePtr : public ShevItem
{
    S2D_EdgePtr ( S2D_EdgePtr & );
    void operator = ( S2D_EdgePtr & );
public:
    S2D_Edge<N> * edge;
    S2D_EdgePtr ( S2D_Edge<N> * e = 0 ) : edge ( e ) {}
};

template <nat N> 
class S2D_Edge : public ShevItem
{
    S2D_Edge ( S2D_Edge & );
    void operator = ( S2D_Edge & );
public:
    S2D_Vert<N> * v0, * v1;
    S2D_Side<N> * side[N-1];
    S2D_Plane<N> * plane[N-1];
    S2D_EdgePtr<N> edgePtr[N-1];

    S2D_Edge () : v0(0), v1(0)
    {
        for ( nat i = 0; i < N-1; ++i )
        {
            edgePtr[i].edge = this;
            side[i] = 0;
        }
    }
};

template <nat N> 
class S2D_Side : public ListItem<List<S2D_EdgePtr<N> > >
{
public:
    S2D_Plane<N> * plane[N-2];
    double value;
    Double<N> point;

    S2D_Side & del  ( S2D_EdgePtr<N> * p ) { jump ( p ); outCur(); return *this; }
    void delFir () { outFir (); }
    void delLas () { outLas (); }
    void delCur () { outCur (); }
    void delAll () { outAll (); }
    ~S2D_Side   () { outAll (); }
};

template <nat N> class S2D_ModelStor
{
public:
    List<S2D_Vert<N> > verts;
    List<S2D_Edge<N> > edges;
    List<S2D_Side<N> > sides;
    List<S2D_Plane<N> > planes;
    AVL_TreeNodeStor<Kit<S2D_Plane<N> *, N-3>, S2D_Side<N> *> nodes;
};

template <nat N> class S2D_Model
{
public:
    List<S2D_Vert<N> > verts;
    List<S2D_Edge<N> > edges;
    List<S2D_Side<N> > sides;
    List<S2D_Plane<N> > planes;

    S2D_Model & makeVoid ( S2D_ModelStor<N> & stor );

    S2D_Model & operator -= ( const Double<N> & p );

    S2D_Model & makeSimplex ( double d, S2D_ModelStor<N> & stor );

    S2D_Model & cut ( const Double<N+1> & g, S2D_ModelStor<N> & stor );
};

template <nat N> 
S2D_Model<N> & S2D_Model<N>::makeVoid ( S2D_ModelStor<N> & stor )
{
    if ( sides.top() )
    do
    {
        sides.cur()->delAll();
    }
    while ( sides.next() );
    sides.movAllAftLas ( stor.sides );
    edges.movAllAftLas ( stor.edges );
    verts.movAllAftLas ( stor.verts );
    planes.movAllAftLas ( stor.planes );
    return *this;
}

template <nat N> 
S2D_Model<N> & S2D_Model<N>::operator -= ( const Double<N> & p )
{
    if ( verts.top() )
    do
    {
        verts.cur()->base() -= p;
    }
    while ( verts.next() );
    if ( planes.top() )
    do
    {
        *planes.cur() -= p;
    }
    while ( planes.next() );
    return *this;
}

template <nat N> 
S2D_Model<N> & S2D_Model<N>::makeSimplex ( double d, S2D_ModelStor<N> & stor )
{
    nat i;
    Double<N> dn;
    dn.fill ( 0 );
    double * q = &dn.d0;
    Double<N+1> dn1;
    dn1.fill ( 0 );
    double * q1 = &dn1.d0;
    S2D_Vert<N> * varr[N+1];
    verts.resize ( N+1, stor.verts ).top();
    S2D_Plane<N> * parr[N+1];
    planes.resize ( N+1, stor.planes ).top();
    for ( i = 0; i < N; ++i )
    {
        q[i] = d;
        ( varr[i] = verts.cur() )->base() = dn;
        verts.next();
        q[i] = 0.;
        q1[i] = -1;
        ( parr[i] = planes.cur() )->base() = dn1;
        q1[i] = 0;
        planes.next();
    }
    ( varr[N] = verts.cur() )->base() = dn;
    dn1.fill ( 1 );
    q1[N] = -d;
    ( parr[N] = planes.cur() )->base() = dn1;
    for ( i = 0; i < N; ++i )
    {
        for ( nat j = i+1; j <= N; ++j )
        {
            S2D_Edge<N> * e = edges.addNewAftLas ( stor.edges );
            e->v0 = varr[i];
            e->v1 = varr[j];
            nat k = 0;
            for ( nat m = 0; m <= N; ++m )
            {
                if ( m == i || m == j ) continue;
                e->plane[k] = parr[m];
                ++k;
            }
        }
    }
    for ( i = 0; i < N-1; ++i )
    {
        for ( nat j = i+1; j < N; ++j )
        {
            for ( nat k = j+1; k <= N; ++k )
            {
                S2D_Side<N> * s = sides.addNewAftLas ( stor.sides );
                nat l = 0;
                for ( nat m = 0; m <= N; ++m )
                {
                    if ( m == i || m == j || m == k ) continue;
                    s->plane[l] = parr[m];
                    ++l;
                }
            }
        }
    }
    S2D_Plane<N> * temp[N-2];
    edges.top();
    do
    {
        S2D_Edge<N> * e = edges.cur();
        for ( i = 0; i < N-1; ++i )
        {
            nat k = 0;
            for ( nat j = 0; j < N-1; ++j )
            {
                if ( i == j ) continue;
                temp[k] = e->plane[j];
                ++k;
            }
            sides.top();
            do
            {
                S2D_Side<N> * s = sides.cur();
                for ( nat j = 0; j < N-2; ++j )
                {
                    if ( temp[j] != s->plane[j] ) goto m1;
                }
                s->addAftLas ( & e->edgePtr[i] );
                e->side[i] = s;
                goto m2;
m1:;        }
            while ( sides.next() );
m2:;    }
    }
    while ( edges.next() );
    return *this;
}

template <nat N> void put ( SuiteRef<S2D_Vert<N> *> & varr, S2D_Vert<N> * p )
{
    for ( nat i = 0; i < varr.size(); ++i )
    {
        if ( varr[i] != p ) continue;
        varr.del ( i );
        return;
    }
    varr.inc() = p;
}

template <nat N> 
S2D_Model<N> & S2D_Model<N>::cut ( const Double<N+1> & plane, S2D_ModelStor<N> & stor )
{
    if ( ! verts.size() || ! edges.size() || ! sides.size() ) return *this;
    verts.top();
    do
    {
        const double d = plane % verts.cur()->base();
        verts.cur()->temp = fabs ( d ) > 1e-10 ? d : 0.;
    }
    while ( verts.next () );

    sides.setAllInfo ( 0 );

    List<S2D_Vert<N> > vtmp;
    edges.top();
    for (;;)
    {
        S2D_Edge<N> & b = * edges.cur();
        if ( b.v0->temp >= 0 )
        {
            for ( nat i = 0; i < N-1; ++i ) b.side[i]->info = 1;
            if ( b.v1->temp < 0 )
            {
                S2D_Vert<N> * v = vtmp.addNewAftLas ( stor.verts );
//v->temp = 1234567;
                Double<N> & point = v->base();
                if ( b.v0->temp + b.v1->temp < 0 )
                {
				    point = b.v1->base();
				    point -= b.v0->base();
				    point *= b.v0->temp / ( b.v0->temp - b.v1->temp );
				    point += b.v0->base();
                }
                else
                {
				    point = b.v0->base();
				    point -= b.v1->base();
				    point *= b.v1->temp / ( b.v1->temp - b.v0->temp );
				    point += b.v1->base();
                }
                b.v0 = v;
                if ( ! edges.next() ) break;
            }
            else
            {
                for ( nat i = 0; i < N-1; ++i ) b.side[i]->del ( & b.edgePtr[i] );
                if ( edges.movCurAftLas_ ( stor.edges ) ) break;
            }
        }
        else
        {
            if ( b.v1->temp >= 0 )
            {
                for ( nat i = 0; i < N-1; ++i ) b.side[i]->info = 1;
                S2D_Vert<N> * v = vtmp.addNewAftLas ( stor.verts );
//v->temp = 1234567;
                Double<N> & point = v->base();
                if ( b.v0->temp + b.v1->temp > 0 )
                {
				    point = b.v1->base();
				    point -= b.v0->base();
				    point *= b.v0->temp / ( b.v0->temp - b.v1->temp );
				    point += b.v0->base();
                }
                else
                {
				    point = b.v0->base();
				    point -= b.v1->base();
				    point *= b.v1->temp / ( b.v1->temp - b.v0->temp );
				    point += b.v1->base();
                }
                b.v1 = v;
            }
            if ( ! edges.next() ) break;
        }
    }
    if ( ! edges.cur() ) return makeVoid ( stor );

    verts.top();
    for (;;)
    {
        if ( verts.cur()->temp >= 0 )
        {
            if ( verts.movCurAftLas_ ( stor.verts ) ) break;
        }
        else
            if ( ! verts.next() ) break;
    }
    vtmp.movAllAftLas ( verts );

    planes.addNewAftLas ( stor.planes )->base() = plane;

    List<S2D_Side<N> > stemp;
    CmbSuite<S2D_Vert<N> *, 8> varr;
    AVL_Tree<Kit<S2D_Plane<N> *, N-3>, S2D_Side<N> *> tree ( stor.nodes );
    for ( sides.top();; )
    {
        S2D_Side<N> * s = sides.cur();
        if ( ! s->info )
        {
            if ( ! sides.next() ) break;
            continue;
        }
        if ( ! s->size() )
        {
            if ( sides.movCurAftLas_ ( stor.sides ) ) break;
            continue;
        }
        varr.resize();
        s->top();
        do
        {
            S2D_Edge<N> * e = s->cur()->edge;
            put ( varr, e->v0 );
            put ( varr, e->v1 );
        }
        while ( s->next() );
if ( varr.size() > 2 )
{
    s = s;
/*    s->top();
    do
    {
        S2D_Edge<N> * e = s->cur()->edge;
        printf ( display.file, "%p %p %e %e\n", e->v0,e->v1, e->v0->temp, e->v1->temp );
    }
    while ( s->next() );
    display << display;*/
}
        if ( varr.size() > 0 )
        {
            nat i;
            Kit<S2D_Plane<N> *, N-3> key;
            S2D_Edge<N> * e = edges.addNewAftLas ( stor.edges );
            s->addAftLas ( & e->edgePtr[N-2] );
            e->v0 = varr[0];
            e->v1 = varr[1];
            for ( i = 0; i < N-2; ++i ) e->plane[i] = s->plane[i];
            e->plane[N-2] = planes.las();
            e->side[N-2] = s;
            for ( i = 0; i < N-2; ++i )
            {
                nat k = 0;
                for ( nat j = 0; j < N-2; ++j )
                {
                    if ( i == j ) continue;
                    key.a[k] = e->plane[j];
                    ++k;
                }
//double t1 = timeInSec();
                S2D_Side<N> ** si = tree.find ( key );
                if ( si )
                {
                    S2D_Side<N> * p = e->side[i] = *si;
                    p->addAftLas ( & e->edgePtr[i] );
                }
                else
                {
                    S2D_Side<N> * p = e->side[i] = stemp.addNewAftLas ( stor.sides );
                    for ( nat j = 0; j < N-3; ++j ) p->plane[j] = key.a[j];
                    p->plane[N-3] = planes.las();
                    p->addAftLas ( & e->edgePtr[i] );
                    tree.add ( key, p );
                }
            }
        }
        if ( ! sides.next() ) break;
    }
    stemp.movAllAftLas ( sides );

    return *this;
}
