
#ifndef LISTS_H
#define LISTS_H

#include "Template.h"
#include "ShevList.h"

/***********************************************/

typedef ListItemA<int> Item1i;
typedef List<Item1i> List1i;

typedef ListItemA<nat> Item1n;
typedef List<Item1n> List1n;

typedef ListItemA<double> Item1d;
typedef List<Item1d> List1d;

typedef ListItemA<void *> Item1p;
typedef List<Item1p> List1p;

/***********************************************/

template <class T> class Item2 : public ListItem<Set2<T> >
{
public:
    typedef T Par;
    Item2 () {}
    explicit Item2 ( const Set2<T> & t, int i = -123456789 ) : ListItem<Set2<T> > ( t, i ) {}
};

typedef Item2<int> Item2i;
typedef List<Item2i> List2i;

typedef Item2<nat> Item2n;
typedef List<Item2n> List2n;

typedef Item2<double> Item2d;
typedef List<Item2d> List2d;

typedef Item2<void *> Item2p;
typedef List<Item2p> List2p;

/***********************************************/

template <class T> class Item3 : public ListItem<Set3<T> >
{
public:
    typedef T Par;
    Item3 () {}
    explicit Item3 ( const Set3<T> & t, int i = -123456789 ) : ListItem<Set3<T> > ( t, i ) {}
};

typedef Item3<nat> Item3n;
typedef List<Item3n> List3n;

/***********************************************/

template <class T1, class T2>
void put ( const T1 & t, List<T2> & list, List<T2> & stor )
{
    if ( list.top() )
    do
    {
        const T1 * s = list.cur();
        if ( s->a == t.b && s->b == t.a )
        {
            list.movCurAftLas ( stor );
            return;
        }
    }
    while ( list.next() );
    if ( t.a != t.b ) *(T1*) list.addNewAftLas ( stor ) = t;
}

/***********************************************/

template <class T>
void makeClosed ( T & list, List<ListItem<T> > & itog )
{
    T temp;
    typedef T::Par Item;
    typedef Item::Par Value;
    typedef SortItem<Set2<Value, nat>, Item *> SItem;
    Suite<SItem> vert ( list.size() );
    if ( list.top() )
    for(;;)
    {
        Item * t = list.cur();
        if ( t->a == t->b )
        {
            if ( list.delCur_() ) break;
            continue;
        }
        SItem & si = vert.inc();
        si.head.a = t->a;
        si.head.b = 0;
        si.tail = t;
        if ( ! list.next() ) break;
    }
    quickSort123 ( vert );

    SItem si0, si1;
    si0.head.b = 0;
    si1.head.b = vert.size();
    while ( list.size() > 0 )
    {
        if ( temp.size() > 0 )
        {
            si0.head.a = temp.las()->b;
            const nat i = lasEqu123 ( vert, si0 );
            if ( i < vert.size() )
            {
                SItem & si = vert[i];
                si.head.b = si1.head.b;
                list.jump ( si.tail ).movCurAftLas ( temp );
                if ( temp.fir()->a == temp.las()->b )
                {
                    ListItem<T> * p = new ListItem<T>;
                    itog.addAftLas ( p );
                    temp.movAllAftCur ( *p ); // цепочка закрыта, начинаем сначала
                }
                else
                {
                    si1.head.a = temp.las()->b;
                    if ( hasEqu123 ( vert, si1 ) )
                    {
                        temp.top();
                        while ( temp.next() )
                        {
                            if ( temp.cur()->a == temp.las()->b )
                            {
                                ListItem<T> * p = new ListItem<T>;
                                itog.addAftLas ( p );
                                temp.movBefCurBefCur ( *p ); // цепочка разделена
                                swap ( *p, temp );
                            }
                        }
                    }
                }
            }
            else
            {
                temp.delLas(); // последнее звено удаляется, т.к. у него нет продолжения
            }
        }
        else
        {
            si0.head.a = list.las()->a;
            const nat i = lasEqu123 ( vert, si0 );
            if ( i < vert.size() )
            {
                // начинаем новую цепочку
                SItem & si = vert[i];
                si.head.b = --si1.head.b;
                list.jump ( si.tail ).movCurAftLas ( temp );
            }
            else
            {
                list.delLas(); // такого не должно быть
            }
        }
    }
}

/***********************************************/

// Упорядочивание по возрастанию ( используется только оператор > )

template <class T>
List<T> & sort123 ( List<T> & list )
{
    if ( list.size() < 4 )
    {
        if ( list.size() == 2 )
        {
            if ( * list.fir() > * list.las() ) list.invert();
            return list;
        }
        if ( list.size() == 3 )
        {
            list.top();
            list.next();
            if ( * list.fir() > * list.cur() )
            {
                if ( * list.cur() > * list.las() )
                    list.invert();
                else
                if ( * list.fir() > * list.las() )
                    list.makeCurFir();
                else
                    list.makeCurLas().invert();
            }
            else
            if ( * list.cur() > * list.las() )
            {
                if ( * list.fir() > * list.las() )
                    list.makeCurLas();
                else
                    list.makeCurFir().invert();
            }
        }
        return list;
    }
    List<T> temp;
    sort123 ( list.movLasAftLas ( list.size() / 2, temp ) ).top();
    sort123 ( temp );
    for (;;)
    {
        if ( * list.cur() > * temp.fir() )
        {
            if ( ! temp.movFirBefCur ( list ).size() ) break;
        }
        else
        {
            if ( list.curIsLas() )
            {
                temp.movAllAftLas ( list );
                break;
            }
        }
        list.next();
    }
    return list;
}

// Упорядочивание по убыванию ( используется только оператор < )

template <class T>
List<T> & sort321 ( List<T> & list )
{
    if ( list.size() < 4 )
    {
        if ( list.size() == 2 )
        {
            if ( * list.fir() < * list.las() ) list.invert();
            return list;
        }
        if ( list.size() == 3 )
        {
            list.top();
            list.next();
            if ( * list.fir() < * list.cur() )
            {
                if ( * list.cur() < * list.las() )
                    list.invert();
                else
                if ( * list.fir() < * list.las() )
                    list.makeCurFir();
                else
                    list.makeCurLas().invert();
            }
            else
            if ( * list.cur() < * list.las() )
            {
                if ( * list.fir() < * list.las() )
                    list.makeCurLas();
                else
                    list.makeCurFir().invert();
            }
        }
        return list;
    }
    List<T> temp;
    sort321 ( list.movLasAftLas ( list.size() / 2, temp ) ).top();
    sort321 ( temp );
    for (;;)
    {
        if ( * list.cur() < * temp.fir() )
        {
            if ( ! temp.movFirBefCur ( list ).size() ) break;
        }
        else
        {
            if ( list.curIsLas() )
            {
                temp.movAllAftLas ( list );
                break;
            }
        }
        list.next();
    }
    return list;
}

// Упрощение цепочек

template <class T>
struct _ChainLink
{
    double head;
    T * tail;
};

template <class T>
inline bool operator <= ( const _ChainLink<T> & p1, const _ChainLink<T> & p2 )
{
    return p1.head <= p2.head;
}

template <class T, class Func, class Remove>
void simplifyChain ( List<T> & list, Func & func, Remove & remove, nat n, double eps = 1e300 )
{
    if ( list.size() <= n ) return;
    MinHeap< _ChainLink<T> > heap ( list.size() );
    _ChainLink<T> pd;
    list.top();
    nat i = 0;
    do
    {
        T * p = list.cur();
        pd.head = func ( p );
        pd.tail = p;
        p->info = i++;
        heap << pd;
    }
    while ( list.next() );
    while ( heap.size() > n && heap[0]->head < eps )
    {
        heap >> pd;
        T * pp = list.jump ( pd.tail ).cprev();
        T * pn = list.jump ( pd.tail ).cnext();
        remove ( pp, pd.tail, pn );
        double d = func ( pp );
        _ChainLink<T> & hp = *heap[pp->info];
        if ( hp.head < d )
        {
            hp.head = d;
            heap.down ( pp->info );
        }
        else
        {
            hp.head = d;
            heap.raise ( pp->info );
        }
        d = func ( pn );
        _ChainLink<T> & ht = *heap[pn->info];
        if ( ht.head < d )
        {
            ht.head = d;
            heap.down ( pn->info );
        }
        else
        {
            ht.head = d;
            heap.raise ( pn->info );
        }
    }
}

#endif