
#pragma once

#include <math.h>

#include "ShevList.h"
#include "ShevArray.h"
#include "DoubleN.h"


//*******************************************************************//
//
//         Проволочная модель N-мерного многогранника
//
//*******************************************************************//

template <nat16 N> 
class WireVert : public ShevItem
{
public:
    Double<N>  coor; // координаты данной вершины
    double     dist; // расстояние до плоскости
};

template <nat16 N> struct WireSegmId
{
    nat16 num[N-1]; // номера гиперплоскостей
};

inline bool operator == ( const WireSegmId<5> & a, const WireSegmId<5> & b )
{
    return a.num[0] == b.num[0] && a.num[1] == b.num[1] && a.num[2] == b.num[2];
}

template <nat16 N> 
class WireSegm : public ShevItem
{
public:
    WireVert<N> * a, * b; // концы отрезка
    WireSegmId<N> id;     // код ребра
};

template <nat16 N> 
class WireModel2 // N-мерный многограник
{
    void separat ( const Double<N+1> & g, List< WireVert<N> > & temp );
    void newVert ( List< WireVert<N> > & vstor, List< WireSegm<N> > & sstor );
//    void newLink ( List< Vertex<N> > & list );
public:
    nat16 count;
    List< WireVert<N> > vlist;
    List< WireSegm<N> > slist;
    
    WireModel2 & simplex ( double d, List< WireVert<N> > & vstor, List< WireSegm<N> > & sstor );
    WireModel2 & cut ( const Double<N+1> & g, List< WireVert<N> > & vstor, List< WireSegm<N> > & sstor );
//    WireModel2 & cut ( const Double<N+1> & g, List< WireVert<N> > & vstor, List< WireSegm<N> > & sstor, WireModel2 & part );
};

template <nat16 N> 
WireModel2<N> & WireModel2<N>::simplex ( double d, List< WireVert<N> > & vstor, List< WireSegm<N> > & sstor )
{
    Double<N> dn;
    dn.fill ( 0 );
    double * q = &dn.d0;
    WireVert<N> * varr[N+1];
    nat16 i;
    vlist.resize ( N+1, vstor ).top();
    for ( i = 0; i < N; ++i )
    {
        q[i] = d;
        ( varr[i] = vlist.cur() )->coor = dn;
        vlist.next();
        q[i] = 0.;
    }
    ( varr[N] = vlist.cur() )->coor = dn;
    for ( i = 0; i < N; ++i )
    {
        for ( nat16 j = i+1; j <= N; ++j )
        {
            WireSegm<N> * s = slist.addNewAftLas ( sstor );
            s->a = varr[i];
            s->b = varr[j];
            nat k = 0;
            nat16 * p = s->id.num;
            for ( nat16 m = 0; m <= N; ++m )
            {
                if ( m == i || m == j ) continue;
                p[k] = m;
                ++k;
            }
        }
    }
    count = N + 1;
    return *this;
}

template <nat16 N> 
void WireModel2<N>::separat ( const Double<N+1> & g, List< WireVert<N> > & temp )
{
    for ( vlist.top();; )
    {
        WireVert<N> * vert = vlist.cur();
        double d = g % vert->coor;
        if ( fabs ( d ) < 1e-10 ) d = 0.;
        if ( ( vert->dist = d ) >= 0 )
        {
            if ( vlist.movCurAftLas_ ( temp ) ) break;
            continue;
        }
        if ( vlist.next() == 0 ) break;
    }
}

template <nat16 N> 
void WireModel2<N>::newVert ( List< WireVert<N> > & vstor, List< WireSegm<N> > & sstor )
{
    List< WireSegm<N> > temp1, temp2;
    WireSegmId<N> id;
    nat16 * num2 = id.num;
    num2[N-1] = count;
    for ( slist.top();; )
    {
        WireSegm<N> * segm = slist.cur();
        const WireVert<N> * a = segm->a;
        const WireVert<N> * b = segm->b;
        if ( a->dist < 0 )
        {
            if ( b->dist < 0 )
            {
                if ( slist.next() ) break;
            }
            else
            {
                WireVert<N> * vert = vlist.addNewBefFir ( vstor );
				vert->coor = b->coor;
				vert->coor -= a->coor;
				vert->coor *= b->dist / ( a->dist - b->dist );
				vert->coor += b->coor;
                segm->b = vert;
                nat16 * num1 = segm->id.num;
                for ( nat i = 0; i < N-1; ++i )
                {
                    WireSegm<N> * s = temp2.addNewBefFir ( sstor );
                    s->a = vert;
                    nat16 j, k = 0;
                    for ( j = 0; j < N-1; ++j )
                    {
                        if ( j == i ) continue;
                        num2[k++] = num1[j];
                    }
                }
                if ( slist.movCurAftLas_ ( temp1 ) ) break;
            }
        }
        else
        {
            if ( b->dist < 0 )
            {
                WireVert<N> * vert = vlist.addNewBefFir ( vstor );
				vert->coor = b->coor;
				vert->coor -= a->coor;
				vert->coor *= b->dist / ( a->dist - b->dist );
				vert->coor += b->coor;
                segm->a = vert;
                if ( slist.movCurAftLas_ ( temp1 ) ) break;
            }
            else
            {
                if ( slist.movCurAftLas_ ( sstor ) ) break;
            }
        }
    }
    temp1.movAllAftLas ( sstor );
    temp2.movAllAftLas ( sstor );
}

template <nat16 N> 
WireModel2<N> & WireModel2<N>::WireModel2::cut (const Double<N+1> & g, List< WireVert<N> > & vstor, List< WireSegm<N> > & sstor)
{
    if ( vlist.size() == 0 ) return *this;
    List< WireVert<N> > vtemp;
	separat ( g, vtemp );
    if ( vtemp.size() == 0 ) return *this;
    if ( vlist.size() == 0 )
    {
        vtemp.movAllAftLas ( vstor );
        slist.movAllAftLas ( sstor );
        return *this;
    }
    vtemp.movAllAftLas ( vlist );
	newVert ( vstor, sstor );
    ++count;
    return *this;
}