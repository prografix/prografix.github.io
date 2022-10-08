
#pragma once

#include "WireModel.h"
#include "AVL_Tree.h"
#include "Template.h"

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


//************************* 02.05.2013 ******************************//
//
//         –асширенна€ проволочна€ модель многогранника
//
//************************* 14.01.2018 ******************************//

template <nat16 N, typename T>
class Facet2Model : public WireModelEx<N, T> // N-мерный многограник
{
    void separat ( const Double<N+1> & g, List<T> & temp );
    void newVert ( List<T> & temp1, List<T> & temp2, List<T> & stor );
    void addFace ( List<T> & vert );
    void delFace ( List<T> & vert );

    enum
    {
        N1 = N*(N-1)/2,
        N2 = N - 2
    };
    nat16 code[N1][N2];

    AVL_TreeNodeStor<Kit<nat, N2>, Suite<T*> > tstor;
    AVL_Tree<Kit<nat, N2>, Suite<T*> > tree;
public:
    Facet2Model () : tree ( tstor )
    {
        nat16 k = 0;
        for ( nat16 i = 0; i < N-1; ++i )
        {
            for ( nat16 j = i+1; j < N; ++j )
            {
                nat16 n = 0;
                nat16 * p = code[k];
                for ( nat16 m = 0; m < N; ++m )
                {
                    if ( m == i || m == j ) continue;
                    p[n] = m;
                    ++n;
                }
                ++k;
            }
        }
    }
    Facet2Model & cut ( const Double<N+1> & g, List<T> & stor );
    Facet2Model & simplex ( double d, List<T> & stor )
    {
        WireModelEx<N, T>::simplex ( d, stor );
        addFace ( vlist );
        return *this;
    }
};

template <nat16 N, typename T>
void Facet2Model<N, T>::addFace ( List<T> & vert )
{
    Kit<nat, N2> key;
    Suite<T*> data;
    if ( vert.top() )
    do
    {
        T * v = vert.cur();
        for ( nat16 j = 0; j < N1; ++j )
        {
            for ( nat16 k = 0; k < N2; ++k ) key.a[k] = v->nfacet[code[j][k]];
            Suite<T*> & r = tree.add ( key, data );
            r.inc() = v;
        }
    }
    while ( vert.next() );
}

template <nat16 N, typename T>
void Facet2Model<N, T>::delFace ( List<T> & vert )
{
    Kit<nat, N2> key;
    if ( vert.top() )
    do
    {
        T * v = vert.cur();
        for ( nat16 j = 0; j < N1; ++j )
        {
            for ( nat16 k = 0; k < N2; ++k ) key.a[k] = v->nfacet[code[j][k]];
            Suite<T*> * p = tree.find ( key );
            if ( p )
            {
                p->delFirEqu ( v );
                if ( ! p->size() ) tree.del ( key );
            }
        }
    }
    while ( vert.next() );
}

template <nat16 N, typename T> 
void Facet2Model<N, T>::separat ( const Double<N+1> & g, List<T> & temp )
{
    WireModelEx<N, T>::separat ( g, temp );
    delFace ( temp );
}

template <nat16 N, typename T> 
void Facet2Model<N, T>::newVert ( List<T> & temp1, List<T> & temp2, List<T> & stor )
{
    WireModelEx<N, T>::newVert ( temp1, temp2, stor );
    addFace ( temp2 );
}

template <nat16 N, typename T> 
Facet2Model<N, T> & Facet2Model<N, T>::cut ( const Double<N+1> & g, List<T> & stor )
{
    if ( vlist.top() == 0 ) return *this;
    List<T> temp1;
	separat ( g, temp1 );
    if ( temp1.size() == 0 ) return *this;
    if ( vlist.size() == 0 )
    {
        temp1.movAllAftLas ( stor );
        return *this;
    }
    List<T> temp2;
	newVert ( temp1, temp2, stor );
    if ( newLinkFunc )
        newLinkFunc ( temp2 );
    else
    	newLink ( temp2 );
    if ( newVertFunc ) newVertFunc ( temp2 );
    temp2.movAllAftLas ( vlist );
    temp1.movAllAftLas ( stor );
    ++count;
    return *this;
}

