
#pragma once

#include <math.h>

#include "ShevList.h"
#include "ShevArray.h"
#include "DoubleN.h"


//*******************************************************************//
//
//         ����������� ������ N-������� �������������
//
//*******************************************************************//


template <nat16 N> 
class Vertex : public ShevItem
{
public:
    Vertex * vertex[N]; // ��������� �� �������� ������� 
    nat16    nfacet[N]; // ������ ���������� ������
    Double<N>  coor;    // ���������� ������ �������
    double     data;    // �������������� ����������
    Vertex * tmp1, * tmp2; // ��������� ����������

    Vertex & operator += ( const Double<N> & p )
    {
        coor += p;  return *this;
    }

    Vertex & operator -= ( const Double<N> & p )
    {
        coor -= p;  return *this;
    }

    Vertex & operator *= ( const Double<N> & p )
    {
        coor *= p;  return *this;
    }
};

template <nat16 N> 
class WireModel // N-������ �����������
{
    void separat ( const Double<N+1> & g, List< Vertex<N> > & temp );
    void newVert ( List< Vertex<N> > & temp1, List< Vertex<N> > & temp2, List< Vertex<N> > & stor );
    void newLink ( List< Vertex<N> > & list );
public:
    nat16 count;
    List< Vertex<N> > vlist;

    WireModel & cut ( const Double<N+1> & g, List< Vertex<N> > & stor );
    WireModel & cut ( const Double<N+1> & g, List< Vertex<N> > & stor, WireModel & part );
    WireModel & simplex ( double d, List< Vertex<N> > & stor );
};

template <nat16 N> 
WireModel<N> & WireModel<N>::simplex ( double d, List< Vertex<N> > & stor )
{
    Double<N> dn;
    dn.fill ( 0 );
    double * q = &dn.d0;
    Vertex<N> * varr[N+1];
    nat16 i;
    vlist.resize ( N+1, stor ).top();
    for ( i = 0; i < N; ++i )
    {
        q[i] = d;
        ( varr[i] = vlist.cur() )->coor = dn;
        vlist.next();
        q[i] = 0.;
    }
    ( varr[N] = vlist.cur() )->coor = dn;
    for ( i = 0; i <= N; ++i )
    {
        nat16 k = 0;
        Vertex<N> * vert = varr[i];
        for ( nat16 l = 0; l <= N; ++l )
        {
            if ( l == i ) continue;
            vert->vertex[k] = varr[l];
            vert->nfacet[k++] = l;
        }
    }
    count = N + 1;
    return *this;
}

template <nat16 N> 
void WireModel<N>::separat ( const Double<N+1> & g, List< Vertex<N> > & temp )
{
    for(;;)
    {
        Vertex<N> * vert = vlist.cur();
        double d = g % vert->coor;
        if ( fabs ( d ) < 1e-10 ) d = 0.;
        if ( ( vert->data = d ) >= 0 )
        {
            if ( vlist.movCurAftLas_ ( temp ) ) break;
            continue;
        }
        if ( vlist.next() == 0 ) break;
    }
}

template <nat16 N> 
void WireModel<N>::newVert ( List< Vertex<N> > & temp1, List< Vertex<N> > & temp2, List< Vertex<N> > & stor )
{
	if ( temp1.size() >= vlist.size() )
	{
        vlist.top();
        do
        {
            Vertex<N> * vert1 = vlist.cur();
            Vertex<N> ** p1 = vert1->vertex;
            nat16 * num1 = vert1->nfacet;
            for ( nat16 i = 0; i < N; ++i )
            {
                Vertex<N> * vert2 = p1[i];
                if ( vert2->data < 0 ) continue;
                Vertex<N> * vert3 = temp2.addNewBefFir ( stor );
				vert3->coor = vert2->coor;
				vert3->coor -= vert1->coor;
				vert3->coor *= vert2->data / ( vert1->data - vert2->data );
				vert3->coor += vert2->coor;
                nat16 * num3 = vert3->nfacet;
                nat16 j, k = 0;
                for ( j = 0; j < N; ++j )
                {
                    if ( j == i ) continue;
                    num3[k++] = num1[j];
                }
                num3[N-1] = count;
                vert3->vertex[N-1] = vert1;
                p1[i] = vert3;
            }
        }
        while ( vlist.next() );
	}
	else
	{
        temp1.top();
        do
        {
            Vertex<N> * vert1 = temp1.cur();
            Vertex<N> ** p1 = vert1->vertex;
            nat16 * num1 = vert1->nfacet;
            for ( nat16 i = 0; i < N; ++i )
            {
                Vertex<N> * vert2 = p1[i];
                if ( vert2->data >= 0 ) continue;
                Vertex<N> * vert3 = temp2.addNewBefFir ( stor );
				vert3->coor = vert2->coor;
				vert3->coor -= vert1->coor;
				vert3->coor *= vert2->data / ( vert1->data - vert2->data );
				vert3->coor += vert2->coor;
                nat16 * num3 = vert3->nfacet;
                nat16 j, k = 0;
                for ( j = 0; j < N; ++j )
                {
                    if ( j == i ) continue;
                    num3[k++] = num1[j];
                }
                num3[N-1] = count;
                vert3->vertex[N-1] = vert2;
				Vertex<N> ** p2 = vert2->vertex;
                for ( j = 0; j < N; ++j )
                {
					if ( p2[j] == vert1 )
					{
						p2[j] = vert3;
						break;
					}
				}
            }
        }
        while ( temp1.next() );
	}
}

template <nat16 N> 
struct SegmentId;

template <> 
struct SegmentId<3>
{
    nat16 id0, summa;

    SegmentId () {}
    SegmentId ( nat16 i, const nat16 * num )
    {
        switch ( i )
        {
        case 0: id0 = num[1]; break;
        case 1: id0 = num[0]; break;
        }
        summa = id0;
    }
    friend inline bool operator == ( const SegmentId<3> & a, const SegmentId<3> & b )
    {
        return a.summa == b.summa;
    }
};

template <> 
struct SegmentId<4>
{
    nat16 id0, id1, summa;

    SegmentId () {}
    SegmentId ( nat16 i, const nat16 * num )
    {
        switch ( i )
        {
        case 0: id0 = num[1]; id1 = num[2]; break;
        case 1: id0 = num[0]; id1 = num[2]; break;
        case 2: id0 = num[0]; id1 = num[1]; break;
        }
        summa = id0 + id1;
    }
    friend inline bool operator == ( const SegmentId<4> & a, const SegmentId<4> & b )
    {
        return a.summa == b.summa && a.id1 == b.id1;
    }
};

template <> 
struct SegmentId<5>
{
    nat16 id0, id1, id2, summa;

    SegmentId () {}
    SegmentId ( nat16 i, const nat16 * num )
    {
        switch ( i )
        {
        case 0: id0 = num[1]; id1 = num[2]; id2 = num[3]; break;
        case 1: id0 = num[0]; id1 = num[2]; id2 = num[3]; break;
        case 2: id0 = num[0]; id1 = num[1]; id2 = num[3]; break;
        case 3: id0 = num[0]; id1 = num[1]; id2 = num[2]; break;
        }
        summa = id0 + id1 + id2;
    }
    friend inline bool operator == ( const SegmentId<5> & a, const SegmentId<5> & b )
    {
        return a.summa == b.summa && a.id1 == b.id1 && a.id2 == b.id2;
    }
};

template <> 
struct SegmentId<6>
{
    nat16 id0, id1, id2, id3, summa;

    SegmentId () {}
    SegmentId ( nat16 i, const nat16 * num )
    {
        switch ( i )
        {
        case 0: id0 = num[1]; id1 = num[2]; id2 = num[3]; id3 = num[4]; break;
        case 1: id0 = num[0]; id1 = num[2]; id2 = num[3]; id3 = num[4]; break;
        case 2: id0 = num[0]; id1 = num[1]; id2 = num[3]; id3 = num[4]; break;
        case 3: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[4]; break;
        case 4: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; break;
        }
        summa = id0 + id1 + id2 + id3;
    }
    friend inline bool operator == ( const SegmentId<6> & a, const SegmentId<6> & b )
    {
        return a.summa == b.summa && a.id1 == b.id1 && a.id2 == b.id2 && a.id3 == b.id3;
    }
};

template <> 
struct SegmentId<7>
{
    nat16 id0, id1, id2, id3, id4, summa;

    SegmentId () {}
    SegmentId ( nat16 i, const nat16 * num )
    {
        switch ( i )
        {
        case 0: id0 = num[1]; id1 = num[2]; id2 = num[3]; id3 = num[4]; id4 = num[5]; break;
        case 1: id0 = num[0]; id1 = num[2]; id2 = num[3]; id3 = num[4]; id4 = num[5]; break;
        case 2: id0 = num[0]; id1 = num[1]; id2 = num[3]; id3 = num[4]; id4 = num[5]; break;
        case 3: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[4]; id4 = num[5]; break;
        case 4: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[5]; break;
        case 5: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; break;
        }
        summa = id0 + id1 + id2 + id3 + id4;
    }
    friend inline bool operator == ( const SegmentId<7> & a, const SegmentId<7> & b )
    {
        return a.summa == b.summa && a.id1 == b.id1 && a.id2 == b.id2 && a.id3 == b.id3 && a.id4 == b.id4;
    }
};

template <> 
struct SegmentId<8>
{
    nat16 id0, id1, id2, id3, id4, id5, summa;

    SegmentId () {}
    SegmentId ( nat16 i, const nat16 * num )
    {
        switch ( i )
        {
        case 0: id0 = num[1]; id1 = num[2]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; break;
        case 1: id0 = num[0]; id1 = num[2]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; break;
        case 2: id0 = num[0]; id1 = num[1]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; break;
        case 3: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[4]; id4 = num[5]; id5 = num[6]; break;
        case 4: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[5]; id5 = num[6]; break;
        case 5: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[6]; break;
        case 6: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[5]; break;
        }
        summa = id0 + id1 + id2 + id3 + id4 + id5;
    }
    friend inline bool operator == ( const SegmentId<8> & a, const SegmentId<8> & b )
    {
        return a.summa == b.summa && a.id1 == b.id1 && a.id2 == b.id2 && a.id3 == b.id3 && a.id4 == b.id4 && a.id5 == b.id5;
    }
};

template <> 
struct SegmentId<9>
{
    nat16 id0, id1, id2, id3, id4, id5, id6, summa;

    SegmentId () {}
    SegmentId ( nat16 i, const nat16 * num )
    {
        switch ( i )
        {
        case 0: id0 = num[1]; id1 = num[2]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; id6 = num[7]; break;
        case 1: id0 = num[0]; id1 = num[2]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; id6 = num[7]; break;
        case 2: id0 = num[0]; id1 = num[1]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; id6 = num[7]; break;
        case 3: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[4]; id4 = num[5]; id5 = num[6]; id6 = num[7]; break;
        case 4: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[5]; id5 = num[6]; id6 = num[7]; break;
        case 5: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[6]; id6 = num[7]; break;
        case 6: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[5]; id6 = num[7]; break;
        case 7: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[5]; id6 = num[6]; break;
        }
        summa = id0 + id1 + id2 + id3 + id4 + id5 + id6;
    }
    friend inline bool operator == ( const SegmentId<9> & a, const SegmentId<9> & b )
    {
        return a.summa == b.summa && a.id1 == b.id1 && a.id2 == b.id2 && a.id3 == b.id3 
                                  && a.id4 == b.id4 && a.id5 == b.id5 && a.id6 == b.id6;
    }
};

template <> 
struct SegmentId<10>
{
    nat16 id0, id1, id2, id3, id4, id5, id6, id7, summa;

    SegmentId () {}
    SegmentId ( nat16 i, const nat16 * num )
    {
        switch ( i )
        {
        case 0: id0 = num[1]; id1 = num[2]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; id6 = num[7]; id7 = num[8]; break;
        case 1: id0 = num[0]; id1 = num[2]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; id6 = num[7]; id7 = num[8]; break;
        case 2: id0 = num[0]; id1 = num[1]; id2 = num[3]; id3 = num[4]; id4 = num[5]; id5 = num[6]; id6 = num[7]; id7 = num[8]; break;
        case 3: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[4]; id4 = num[5]; id5 = num[6]; id6 = num[7]; id7 = num[8]; break;
        case 4: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[5]; id5 = num[6]; id6 = num[7]; id7 = num[8]; break;
        case 5: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[6]; id6 = num[7]; id7 = num[8]; break;
        case 6: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[5]; id6 = num[7]; id7 = num[8]; break;
        case 7: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[5]; id6 = num[6]; id7 = num[8]; break;
        case 8: id0 = num[0]; id1 = num[1]; id2 = num[2]; id3 = num[3]; id4 = num[4]; id5 = num[5]; id6 = num[6]; id7 = num[7]; break;
        }
        summa = id0 + id1 + id2 + id3 + id4 + id5 + id6 + id7;
    }
    friend inline bool operator == ( const SegmentId<10> & a, const SegmentId<10> & b )
    {
        return a.summa == b.summa && a.id1 == b.id1 && a.id2 == b.id2 && a.id3 == b.id3 
                && a.id4 == b.id4 && a.id5 == b.id5 && a.id6 == b.id6 && a.id7 == b.id7;
    }
};

template <nat16 N, typename T> 
struct SegmentEnd
{
    SegmentId<N> id;
    T * parent;
    T ** link;
    SegmentEnd * next;
};

template <nat16 N> 
void WireModel<N>::newLink ( List< Vertex<N> > & list )
{
    typedef SegmentEnd<N, Vertex<N> > SegmEnd;
    DynArray<SegmEnd> table ( ( N - 1 ) * list.size() / 2 );
    FixArray<SegmEnd *, 64> entry;
    entry.fill ( 0 );
    nat n = 0;
    list.top();
    do
    {
        Vertex<N> * vert = list.cur();
        for ( nat16 i = 0; i < N-1; ++i )
        {
            const SegmentId<N> id ( i, vert->nfacet );
            const nat16 k = id.summa & 63;
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
}

template <nat16 N> 
WireModel<N> & WireModel<N>::cut ( const Double<N+1> & g, List< Vertex<N> > & stor )
{
    if ( vlist.top() == 0 ) return *this;
    List< Vertex<N> > temp1;
	separat ( g, temp1 );
    if ( temp1.size() == 0 ) return *this;
    if ( vlist.size() == 0 )
    {
        temp1.movAllAftLas ( stor );
        return *this;
    }
    List< Vertex<N> > temp2;
	newVert ( temp1, temp2, stor );
	newLink ( temp2 );
    temp2.movAllAftLas ( vlist );
    temp1.movAllAftLas ( stor );
    ++count;
    return *this;
}

template <nat16 N> 
WireModel<N> & WireModel<N>::cut ( const Double<N+1> & g, List< Vertex<N> > & stor, WireModel & part )
{
    if ( ! vlist.top() ) return *this;
	separat ( g, part.vlist );
    List< Vertex<N> > temp;
    if ( vlist.top() )
    do
    {
        Vertex<N> * vert1 = vlist.cur();
        Vertex<N> ** p1 = vert1->vertex;
        const nat16 * num1 = vert1->nfacet;
        for ( nat16 i = 0; i < N; ++i )
        {
            Vertex<N> * vert2 = p1[i];
            if ( vert2->data < 0 ) continue;
            Vertex<N> * vert3 = temp.addNewBefFir ( stor );
			vert3->coor = vert2->coor;
			vert3->coor -= vert1->coor;
			vert3->coor *= vert2->data / ( vert1->data - vert2->data );
			vert3->coor += vert2->coor;
            nat16 * num3 = vert3->nfacet;
            nat k = 0;
            for ( nat16 j = 0; j < N; ++j )
            {
                if ( j == i ) continue;
                num3[k++] = num1[j];
            }
            num3[N-1] = count;
            Vertex<N> ** p3 = vert3->vertex;
            p3[N-1] = vert1;
            p1[i] = vert3;
            vert3->tmp1 = vert2;
        }
    }
    while ( vlist.next() );
    part.count = ++count;
    if ( ! temp.cur() ) return *this;
	newLink ( temp );
    if ( temp.top() )
    do
    {
        Vertex<N> * vert1 = temp.cur();
        Vertex<N> * vert2 = part.vlist.addNewBefFir ( stor );
        vert2->coor = vert1->coor;
        nat16 * num1 = vert1->nfacet;
        nat16 * num2 = vert2->nfacet;
        for ( nat16 j = 0; j < N; ++j ) num2[j] = num1[j];
        vert1->tmp2 = vert2;
    }
    while ( temp.next() );
    if ( temp.top() )
    do
    {
        Vertex<N> * vert1 = temp.cur();
        Vertex<N> * vert2 = vert1->tmp2;
        Vertex<N> ** p1 = vert1->vertex;
        Vertex<N> ** p2 = vert2->vertex;
        nat16 j;
        for ( j = 0; j < N; ++j ) p2[j] = p1[j]->tmp2;
        Vertex<N> * vert3 = p1[N-1];
        Vertex<N> * vert4 = p2[N-1] = vert1->tmp1;
        Vertex<N> ** p4 = vert4->vertex;
        for ( j = 0; j < N; ++j )
        {
            if ( p4[j] == vert3 )
            {
                p4[j] = vert2;
                break;
            }
        }
    }
    while ( temp.next() );
    temp.movAllAftLas ( vlist );
    return *this;
}

template <nat16 N> 
bool copy ( const WireModel<N> & a, WireModel<N> & b, List< Vertex<N> > & stor,
            DynArrRef< SortItem <const Vertex<N> *, Vertex<N> *> > & buf )
{
    b.count = a.count;
    b.vlist.movAllBefFir ( stor );
    Show< Vertex<N> > vlist ( a.vlist );
    if ( vlist.top() == 0 ) return true;
    b.vlist.resize ( vlist.size(), stor ).top();
    if ( buf.size() != vlist.size() ) buf.resize ( vlist.size() );
    for ( nat i = 0; i < buf.size(); ++i )
    {
        buf[i].head = vlist.cur();
        buf[i].tail = b.vlist.cur();
        b.vlist.next();
        vlist.next();
    }
    insertSort123 ( buf );
    SortItem <const Vertex<N> *, Vertex<N> *> si;
    vlist.top();
    b.vlist.top();
    do
    {
        const Vertex<N> * va = vlist.cur();
        Vertex<N> * vb = b.vlist.cur();
        for ( nat16 i = 0; i < N; ++i )
        {
            si.head = va->vertex[i];
            nat j = lasEqu123 ( buf, si );
            if ( j == buf.size() )
            {
                b.vlist.movAllBefFir ( stor );
                return false;
            }
            vb->vertex[i] = buf[j].tail;
            vb->nfacet[i] = va->nfacet[i];
        }
        vb->coor = va->coor;
        vb->data = va->data;
        vb->info = va->info;
    }
    while ( vlist.next() && b.vlist.next() );
    return true;
}


//************************* 02.05.2013 ******************************//
//
//         ����������� ����������� ������ �������������
//
//************************* 14.01.2018 ******************************//


template <nat16 N, typename T, nat16 M = N> 
struct VertexEx : public ShevItem
{
    VertexEx *  vertex[N];  // ��������� �� �������� ������� 
    nat16       nfacet[N];  // ������ ���������� ������
    Double<M>   coor;       // ���������� ������ �������
    double      data;       // ��������� ����������
    VertexEx * tmp1, * tmp2;// ��������� ����������
    T           extra;      // �������������� ����������

    VertexEx & operator += ( const Double<M> & p )
    {
        coor += p;  return *this;
    }

    VertexEx & operator -= ( const Double<M> & p )
    {
        coor -= p;  return *this;
    }

    VertexEx & operator *= ( const Double<M> & p )
    {
        coor *= p;  return *this;
    }
};

template <nat16 N, typename T> 
class WireModelEx // N-������ �����������
{
protected:
    void separat ( const Double<N+1> & g, List<T> & temp );
    void newVert ( List<T> & temp1, List<T> & temp2, List<T> & stor );
    void newLink ( List<T> & list );
public:
    nat16 count;
    List<T> vlist;
    void ( * newVertFunc ) ( List<T> & vlist );
    void ( * newLinkFunc ) ( List<T> & vlist );

    WireModelEx() : newVertFunc ( 0 ), newLinkFunc ( 0 ) {}
    WireModelEx & cut ( const Double<N+1> & g, List<T> & stor );
    WireModelEx & cut ( const Double<N+1> & g, List<T> & stor, WireModelEx & part );
    WireModelEx & simplex ( double d, List<T> & stor );
};

template <nat16 N, typename T> 
WireModelEx<N, T> & WireModelEx<N, T>::simplex ( double d, List<T> & stor )
{
    Double<N> dn;
    dn.fill ( 0 );
    double * q = &dn.d0;
    T * varr[N+1];
    nat16 i;
    vlist.resize ( N+1, stor ).top();
    for ( i = 0; i < N; ++i )
    {
        q[i] = d;
        ( varr[i] = vlist.cur() )->coor = dn;
        vlist.next();
        q[i] = 0.;
    }
    ( varr[N] = vlist.cur() )->coor = dn;
    for ( i = 0; i <= N; ++i )
    {
        nat16 k = 0;
        T * vert = varr[i];
        for ( nat16 l = 0; l <= N; ++l )
        {
            if ( l == i ) continue;
            vert->vertex[k] = varr[l];
            vert->nfacet[k++] = l;
        }
    }
    count = N + 1;
    return *this;
}

template <nat16 N, typename T> 
void WireModelEx<N, T>::separat ( const Double<N+1> & g, List<T> & temp )
{
    for(;;)
    {
        T * vert = vlist.cur();
        double d = g % vert->coor;
        if ( fabs ( d ) < 1e-10 ) d = 0.;
        if ( ( vert->data = d ) >= 0 )
        {
            if ( vlist.movCurAftLas_ ( temp ) ) break;
            continue;
        }
        if ( ! vlist.next() ) break;
    }
}

template <nat16 N, typename T> 
void WireModelEx<N, T>::newVert ( List<T> & temp1, List<T> & temp2, List<T> & stor )
{
	if ( temp1.size() >= vlist.size() )
	{
        vlist.top();
        do
        {
            T * vert1 = vlist.cur();
            T ** p1 = vert1->vertex;
            nat16 * num1 = vert1->nfacet;
            for ( nat16 i = 0; i < N; ++i )
            {
                T * vert2 = p1[i];
                if ( vert2->data < 0 ) continue;
                T * vert3 = temp2.addNewBefFir ( stor );
				vert3->coor = vert2->coor;
				vert3->coor -= vert1->coor;
				vert3->coor *= vert2->data / ( vert1->data - vert2->data );
				vert3->coor += vert2->coor;
                nat16 * num3 = vert3->nfacet;
                nat16 j, k = 0;
                for ( j = 0; j < N; ++j )
                {
                    if ( j == i ) continue;
                    num3[k++] = num1[j];
                }
                num3[N-1] = count;
                vert3->vertex[N-1] = vert1;
                p1[i] = vert3;
            }
        }
        while ( vlist.next() );
	}
	else
	{
        temp1.top();
        do
        {
            T * vert1 = temp1.cur();
            T ** p1 = vert1->vertex;
            nat16 * num1 = vert1->nfacet;
            for ( nat16 i = 0; i < N; ++i )
            {
                T * vert2 = p1[i];
                if ( vert2->data >= 0 ) continue;
                T * vert3 = temp2.addNewBefFir ( stor );
				vert3->coor = vert2->coor;
				vert3->coor -= vert1->coor;
				vert3->coor *= vert2->data / ( vert1->data - vert2->data );
				vert3->coor += vert2->coor;
                nat16 * num3 = vert3->nfacet;
                nat16 j, k = 0;
                for ( j = 0; j < N; ++j )
                {
                    if ( j == i ) continue;
                    num3[k++] = num1[j];
                }
                num3[N-1] = count;
                vert3->vertex[N-1] = vert2;
				T ** p2 = vert2->vertex;
                for ( j = 0; j < N; ++j )
                {
					if ( p2[j] == vert1 )
					{
						p2[j] = vert3;
						break;
					}
				}
            }
        }
        while ( temp1.next() );
	}
}

template <nat16 N, typename T> 
void WireModelEx<N, T>::newLink ( List<T> & list )
{
    typedef SegmentEnd<N, T> SegmEnd;
    DynArray<SegmEnd> table ( ( N - 1 ) * list.size() / 2 );
    FixArray<SegmEnd *, 128> entry;
    entry.fill ( 0 );
    nat n = 0;
    list.top();
    do
    {
        T * vert = list.cur();
        for ( nat16 i = 0; i < N-1; ++i )
        {
            const SegmentId<N> id ( i, vert->nfacet );
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
}

template <nat16 N, typename T> 
WireModelEx<N, T> & WireModelEx<N, T>::cut ( const Double<N+1> & g, List<T> & stor )
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

template <nat16 N, typename T, nat16 M> 
void intersect ( const WireModelEx<N, VertexEx<N, T, M> > & model, const Double<N+1> & g, 
                 WireModelEx<N-1, VertexEx<N-1, T, M> > & res, List< VertexEx<N-1, T, M> > & stor )
{
}

