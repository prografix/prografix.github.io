
#include "ShevList.h"
#include "template.h"

namespace SL
{

ShevItem::~ShevItem() {}

//***************************************************************************//

bool InfoEQ::operator () ( const ShevItem * p ) { return p->info == i; }
bool InfoNE::operator () ( const ShevItem * p ) { return p->info != i; }
bool InfoLT::operator () ( const ShevItem * p ) { return p->info <  i; }
bool InfoGT::operator () ( const ShevItem * p ) { return p->info >  i; }
bool InfoLE::operator () ( const ShevItem * p ) { return p->info <= i; }
bool InfoGE::operator () ( const ShevItem * p ) { return p->info >= i; }

bool NotPredicate1::operator () ( const ShevItem * p ) { return ! pre ( p ); }
bool Or_Predicate1::operator () ( const ShevItem * p ) { return pre1 ( p ) || pre2 ( p ); }
bool AndPredicate1::operator () ( const ShevItem * p ) { return pre1 ( p ) && pre2 ( p ); }

//***************************************************************************//

void Alter::operator () ( ShevItem * ) {}

Alter none;

void InfoAsn::operator () ( ShevItem * p ) { p->info  = i; }
void InfoAdd::operator () ( ShevItem * p ) { p->info += i; }
void InfoSub::operator () ( ShevItem * p ) { p->info -= i; }
void InfoMul::operator () ( ShevItem * p ) { p->info *= i; }
void InfoDiv::operator () ( ShevItem * p ) { p->info /= i; }

void InfoAP::operator () ( ShevItem * p ) { p->info = start; start += step; }

void If::operator () ( ShevItem * p ) { if ( pre(p) ) alt1(p); else alt2(p); }

//***************************************************************************//

InfoActGen Info;

//***************************************************************************//

bool Double123Sorter::operator () ( const ShevItem * item1, const ShevItem * item2 )
{
    return array[item1->info] <= array[item2->info];
}

bool Double321Sorter::operator () ( const ShevItem * item1, const ShevItem * item2 )
{
    return array[item1->info] >= array[item2->info];
}

//***************************************************************************//

ShevList::~ShevList()
{
    while ( ( cPtr = fPtr ) != 0 )
    {
        fPtr = fPtr->nextPtr;
        delete cPtr;
    }
}

ShevList & ShevList::delAll ()
{
    while ( ( cPtr = fPtr ) != 0 )
    {
        fPtr = fPtr->nextPtr;
        delete cPtr;
    }
    lPtr = 0;
    listSize = 0;
    return *this;
}

// Количество элементов с заданным свойством

nat ShevList::count ( Predicate1 & pre ) const
{
    nat n = 0;
    const ShevItem * p = fPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) ) ++n;
        p = p->nextPtr;
    }
    return n;
}

nat ShowList::count ( Predicate1 & pre ) const
{
    nat n = 0;
    const ShevItem * p = fPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) ) ++n;
        p = p->nextPtr;
    }
    return n;
}

// Первый элемент с заданным свойством

ShevItem * ShevList::fir ( Predicate1 & pre )
{
    ShevItem * p = fPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) ) break;
        p = p->nextPtr;
    }
    return p;
}

const ShevItem * ShevList::fir ( Predicate1 & pre ) const
{
    const ShevItem * p = fPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) ) break;
        p = p->nextPtr;
    }
    return p;
}

const ShevItem * ShowList::fir ( Predicate1 & pre ) const
{
    const ShevItem * p = fPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) ) break;
        p = p->nextPtr;
    }
    return p;
}

// Последний элемент с заданным свойством

ShevItem * ShevList::las ( Predicate1 & pre )
{
    ShevItem * p = lPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) ) break;
        p = p->prevPtr;
    }
    return p;
}

const ShevItem * ShevList::las ( Predicate1 & pre ) const
{
    const ShevItem * p = lPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) ) break;
        p = p->prevPtr;
    }
    return p;
}

const ShevItem * ShowList::las ( Predicate1 & pre ) const
{
    const ShevItem * p = lPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) ) break;
        p = p->prevPtr;
    }
    return p;
}

// Первый элемент с минимальным info:

const ShevItem * ShevList::firMinInfo () const
{
    if ( listSize < 2 ) return fPtr;
    const ShevItem * r = fPtr;
    const ShevItem * p = r->nextPtr;
    while ( p != 0 )
    {
        if ( r->info > p->info ) r = p;
        p = p->nextPtr;
    }
    return r;
}

const ShevItem * ShowList::firMinInfo () const
{
    if ( listSize < 2 ) return fPtr;
    const ShevItem * r = fPtr;
    const ShevItem * p = r->nextPtr;
    while ( p != 0 )
    {
        if ( r->info > p->info ) r = p;
        p = p->nextPtr;
    }
    return r;
}

// Первый элемент с максимальным info:

const ShevItem * ShevList::firMaxInfo () const
{
    if ( listSize < 2 ) return fPtr;
    const ShevItem * r = fPtr;
    const ShevItem * p = r->nextPtr;
    while ( p != 0 )
    {
        if ( r->info < p->info ) r = p;
        p = p->nextPtr;
    }
    return r;
}

const ShevItem * ShowList::firMaxInfo () const
{
    if ( listSize < 2 ) return fPtr;
    const ShevItem * r = fPtr;
    const ShevItem * p = r->nextPtr;
    while ( p != 0 )
    {
        if ( r->info < p->info ) r = p;
        p = p->nextPtr;
    }
    return r;
}

// Сделать текущим первый элемент с заданным свойством

bool ShevList::findFir ( Predicate1 & pre )
{
    ShevItem * p = fPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) )
        {
            cPtr = p;
            return true;
        }
        p = p->nextPtr;
    }
    return false;
}

bool ShowList::findFir ( Predicate1 & pre )
{
    const ShevItem * p = fPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) )
        {
            cPtr = p;
            return true;
        }
        p = p->nextPtr;
    }
    return false;
}

// Сделать текущим последний элемент с заданным свойством

bool ShevList::findLas ( Predicate1 & pre )
{
    ShevItem * p = lPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) )
        {
            cPtr = p;
            return true;
        }
        p = p->prevPtr;
    }
    return false;
}

bool ShowList::findLas ( Predicate1 & pre )
{
    const ShevItem * p = lPtr;
    while ( p != 0 )
    {
        if ( pre ( p ) )
        {
            cPtr = p;
            return true;
        }
        p = p->prevPtr;
    }
    return false;
}

// Добавить в список один элемент перед первым

ShevList & ShevList::addBefFir ( ShevItem * m )
{
    if ( ! m ) return *this;
    if ( cPtr )
    {
        m->nextPtr = fPtr;
        fPtr = fPtr->prevPtr = m;
    }
    else
    {
        m->nextPtr = 0;
        cPtr = fPtr = lPtr = m;
    }
    m->prevPtr = 0;
    ++listSize;
    return *this;
}

// Добавить в список один элемент перед текущим и сделать его текущим

ShevList & ShevList::addBefCur ( ShevItem * m )
{
    if ( ! m ) return *this;
    if ( cPtr )
    {
        m->prevPtr = cPtr->prevPtr;
        m->nextPtr = cPtr;
        cPtr->prevPtr = m;
        if ( m->prevPtr ) m->prevPtr->nextPtr = m;
        else fPtr = m;
    }
    else
    {
        m->prevPtr = m->nextPtr = 0; fPtr = lPtr = m;
    }
    cPtr = m;
    ++listSize;
    return *this;
}

// Добавить в список один элемент после текущего и сделать его текущим

ShevList & ShevList::addAftCur ( ShevItem * m )
{
    if ( ! m ) return *this;
    if ( cPtr )
    {
        m->prevPtr = cPtr;
        m->nextPtr = cPtr->nextPtr;
        cPtr->nextPtr = m;
        if ( m->nextPtr ) m->nextPtr->prevPtr = m;
        else lPtr = m;
    }
    else
    {
        m->prevPtr = m->nextPtr = 0; fPtr = lPtr = m;
    }
    cPtr = m;
    ++listSize;
    return *this;
}

// Добавить в список один элемент после последнего

ShevList & ShevList::addAftLas ( ShevItem * m )
{
    if ( ! m ) return *this;
    if ( cPtr )
    {
        m->prevPtr = lPtr;
        lPtr = lPtr->nextPtr = m;
    }
    else
    {
        m->prevPtr = 0;
        cPtr = fPtr = lPtr = m;
    }
    m->nextPtr = 0;
    ++listSize;
    return *this;
}

// Добавить все элементы из другого списка в начало

ShevList & ShevList::addAllBefFir ( ShevList & list )
{
    if ( ! list.cPtr || this == &list ) return *this;
    if ( cPtr )
    {
        list.lPtr->nextPtr = fPtr;
        fPtr->prevPtr = list.lPtr;
        fPtr = list.fPtr;
        listSize += list.listSize;
    }
    else
    {
        cPtr = list.cPtr;
        fPtr = list.fPtr;
        lPtr = list.lPtr;
        listSize = list.listSize;
    }
    list.fPtr = list.cPtr = list.lPtr = 0;
    list.listSize = 0;
    return *this;
}

// Добавить все элементы из другого списка перед текущим

ShevList & ShevList::addAllBefCur ( ShevList & list )
{
    if ( ! list.cPtr || this == &list ) return *this;
    if ( cPtr )
    {
        list.fPtr->prevPtr = cPtr->prevPtr;
        list.lPtr->nextPtr = cPtr;
        cPtr->prevPtr = list.lPtr;
        if ( list.fPtr->prevPtr )
             list.fPtr->prevPtr->nextPtr = list.fPtr;
        else fPtr = list.fPtr;
        listSize += list.listSize;
    }
    else
    {
        fPtr = list.fPtr;
        lPtr = list.lPtr;
        listSize = list.listSize;
    }
    cPtr = list.cPtr;
    list.fPtr = list.cPtr = list.lPtr = 0;
    list.listSize = 0;
    return *this;
}

// Добавить все элементы из другого списка после текущего

ShevList & ShevList::addAllAftCur ( ShevList & list )
{
    if ( ! list.cPtr || this == &list ) return *this;
    if ( cPtr )
    {
        list.fPtr->prevPtr = cPtr;
        list.lPtr->nextPtr = cPtr->nextPtr;
        cPtr->nextPtr = list.fPtr;
        if ( list.lPtr->nextPtr )
             list.lPtr->nextPtr->prevPtr = list.lPtr;
        else lPtr = list.lPtr;
        listSize += list.listSize;
    }
    else
    {
        fPtr = list.fPtr;
        lPtr = list.lPtr;
        listSize = list.listSize;
    }
    cPtr = list.cPtr;
    list.fPtr = list.cPtr = list.lPtr = 0;
    list.listSize = 0;
    return *this;
}

// Добавить все элементы из другого списка в конец

ShevList & ShevList::addAllAftLas ( ShevList & list )
{
    if ( ! list.cPtr || this == &list ) return *this;
    if ( cPtr )
    {
        list.fPtr->prevPtr = lPtr;
        lPtr->nextPtr = list.fPtr;
        lPtr = list.lPtr;
        listSize += list.listSize;
    }
    else
    {
        cPtr = list.cPtr;
        fPtr = list.fPtr;
        lPtr = list.lPtr;
        listSize = list.listSize;
    }
    list.fPtr = list.cPtr = list.lPtr = 0;
    list.listSize = 0;
    return *this;
}

ShevList & ShevList::movCurBefFir ( ShevList & list )
{
    if ( !cPtr ) return *this;
    ShevItem * m = cPtr;
    if ( m->nextPtr ) ( cPtr = m->nextPtr )->prevPtr = m->prevPtr;
    else lPtr = cPtr = m->prevPtr;
    if ( m->prevPtr ) m->prevPtr->nextPtr = m->nextPtr;
    else fPtr = cPtr;
    --listSize;
    if ( list.lPtr )
    {
        m->nextPtr = list.fPtr;
        list.fPtr = list.fPtr->prevPtr = m;
    }
    else
    {
        m->nextPtr = 0;
        list.cPtr = list.fPtr = list.lPtr = m;
    }
    m->prevPtr = 0;
    ++list.listSize;
    return *this;
}

ShevList & ShevList::movCurBefCur ( ShevList & list )
{
    if ( !cPtr ) return *this;
    ShevItem * m = cPtr;
    if ( m->nextPtr ) ( cPtr = m->nextPtr )->prevPtr = m->prevPtr;
    else lPtr = cPtr = m->prevPtr;
    if ( m->prevPtr ) m->prevPtr->nextPtr = m->nextPtr;
    else fPtr = cPtr;
    --listSize;
    if ( list.cPtr )
    {
        m->nextPtr = list.cPtr;
        m->prevPtr = list.cPtr->prevPtr;
        list.cPtr->prevPtr = m;
        if ( m->prevPtr ) m->prevPtr->nextPtr = m;
        else list.fPtr = m;
    }
    else
    {
        m->prevPtr = m->nextPtr = 0;
        list.fPtr = list.lPtr = m;
    }
    list.cPtr = m;
    ++list.listSize;
    return *this;
}

ShevList & ShevList::movCurAftCur ( ShevList & list )
{
    if ( !cPtr ) return *this;
    ShevItem * m = cPtr;
    if ( m->nextPtr ) ( cPtr = m->nextPtr )->prevPtr = m->prevPtr;
    else lPtr = cPtr = m->prevPtr;
    if ( m->prevPtr ) m->prevPtr->nextPtr = m->nextPtr;
    else fPtr = cPtr;
    --listSize;
    if ( list.cPtr )
    {
        m->prevPtr = list.cPtr;
        m->nextPtr = list.cPtr->nextPtr;
        list.cPtr->nextPtr = m;
        if ( m->nextPtr ) m->nextPtr->prevPtr = m;
        else list.lPtr = m;
    }
    else
    {
        m->prevPtr = m->nextPtr = 0;
        list.fPtr = list.lPtr = m;
    }
    list.cPtr = m;
    ++list.listSize;
    return *this;
}

ShevList & ShevList::movCurAftLas ( ShevList & list )
{
    if ( !cPtr ) return *this;
    ShevItem * m = cPtr;
    if ( m->nextPtr ) ( cPtr = m->nextPtr )->prevPtr = m->prevPtr;
    else lPtr = cPtr = m->prevPtr;
    if ( m->prevPtr ) m->prevPtr->nextPtr = m->nextPtr;
    else fPtr = cPtr;
    --listSize;
    if ( list.lPtr )
    {
        m->prevPtr = list.lPtr;
        list.lPtr = list.lPtr->nextPtr = m;
    }
    else
    {
        m->prevPtr = 0;
        list.cPtr = list.fPtr = list.lPtr = m;
    }
    m->nextPtr = 0;
    ++list.listSize;
    return *this;
}

//**************************** 18.04.2006 **********************************
//
//    Перенос элементов с заданным свойством из одного списка в конец другого 
//
//**************************** 16.01.2007 **********************************

ShevList & ShevList::mov123AftLas ( Predicate1 & pre, ShevList & list )
{
    if ( ! fPtr ) return *this;
    ShevItem * fi = fPtr;
    while ( ! pre ( fi ) )
    {
        fi = fi->nextPtr;
        if ( fi == 0 ) return *this;
    }

    ShevItem * li = fi;
    lPtr = fi->prevPtr;
    if ( lPtr == 0 ) fPtr = 0;
    ShevItem * p = fi->nextPtr;
    if ( this == &list )
    {
        while ( p != 0 )
        {
            if ( pre ( p ) )
            {
                if ( p->prevPtr != li )
                {
                    li->nextPtr = p;
                    p->prevPtr = li;
                }
                li = p;
            }
            else
            {
                if ( p->prevPtr != lPtr )
                {
                    if ( lPtr == 0 )
                    {
                        fPtr = p;
                        fPtr->prevPtr = 0;
                    }
                    else
                    {
                        lPtr->nextPtr = p;
                        p->prevPtr = lPtr;
                    }
                }
                lPtr = p;
            }
            p = p->nextPtr;
        }
        li->nextPtr = 0;
        if ( lPtr != 0 )
        {
            lPtr->nextPtr = fi;
            fi->prevPtr = lPtr;
        }
        else
        {
            fPtr = fi;
        }
        lPtr = li;
    }
    else
    {
        nat count = 1;
        while ( p != 0 )
        {
            if ( pre ( p ) )
            {
                if ( p->prevPtr != li )
                {
                    li->nextPtr = p;
                    p->prevPtr = li;
                }
                li = p;
                ++count;
            }
            else
            {
                if ( p->prevPtr != lPtr )
                {
                    if ( lPtr == 0 )
                    {
                        fPtr = p;
                        fPtr->prevPtr = 0;
                    }
                    else
                    {
                        lPtr->nextPtr = p;
                        p->prevPtr = lPtr;
                    }
                }
                lPtr = p;
            }
            p = p->nextPtr;
        }
        li->nextPtr = 0;
        if ( ( cPtr = lPtr ) != 0 ) fPtr->prevPtr = lPtr->nextPtr = 0;
        listSize -= count;

        if ( list.lPtr != 0 )
        {
            list.lPtr->nextPtr = fi;
            fi->prevPtr = list.lPtr;
        }
        else
        {
            fi->prevPtr = 0;
            list.cPtr = list.fPtr = fi;
        }
        list.lPtr = li;
        list.listSize += count;
    }
    return *this;
}

ShevList & ShevList::mov321AftLas ( Predicate1 & pre, ShevList & list )
{
    if ( ! lPtr ) return *this;
    ShevItem * li = lPtr;
    while ( ! pre ( li ) )
    {
        li = li->prevPtr;
        if ( li == 0 ) return *this;
    }

    ShevItem * fi = li;
    fPtr = li->nextPtr;
    if ( fPtr == 0 ) lPtr = 0;
    ShevItem * p = li->prevPtr;
    if ( this == &list )
    {
        while ( p != 0 )
        {
            if ( pre ( p ) )
            {
                if ( p->nextPtr != fi )
                {
                    fi->prevPtr = p;
                    p->nextPtr = fi;
                }
                fi = p;
            }
            else
            {
                if ( p->nextPtr != fPtr )
                {
                    if ( fPtr == 0 )
                    {
                        lPtr = p;
                        lPtr->nextPtr = 0;
                    }
                    else
                    {
                        fPtr->prevPtr = p;
                        p->nextPtr = fPtr;
                    }
                }
                fPtr = p;
            }
            p = p->prevPtr;
        }
        li->nextPtr = 0;
        if ( lPtr != 0 )
        {
            lPtr->nextPtr = fi;
            fi->prevPtr = lPtr;
            fPtr->prevPtr = 0;
        }
        else
        {
            fPtr = fi;
        }
        lPtr = li;
    }
    else
    {
        nat count = 1;
        while ( p != 0 )
        {
            if ( pre ( p ) )
            {
                if ( p->nextPtr != fi )
                {
                    fi->prevPtr = p;
                    p->nextPtr = fi;
                }
                fi = p;
                ++count;
            }
            else
            {
                if ( p->nextPtr != fPtr )
                {
                    if ( fPtr == 0 )
                    {
                        lPtr = p;
                        lPtr->nextPtr = 0;
                    }
                    else
                    {
                        fPtr->prevPtr = p;
                        p->nextPtr = fPtr;
                    }
                }
                fPtr = p;
            }
            p = p->prevPtr;
        }
        li->nextPtr = 0;
        if ( ( cPtr = lPtr ) != 0 ) fPtr->prevPtr = lPtr->nextPtr = 0;
        listSize -= count;

        if ( list.lPtr != 0 )
        {
            list.lPtr->nextPtr = fi;
            fi->prevPtr = list.lPtr;
        }
        else
        {
            fi->prevPtr = 0;
            list.cPtr = list.fPtr = fi;
        }
        list.lPtr = li;
        list.listSize += count;
    }
    return *this;
}

//**************************** 18.04.2006 **********************************
//
//    Перенос элементов с заданным свойством из одного списка в начало другого 
//
//**************************** 16.01.2007 **********************************


ShevList & ShevList::mov123BefFir ( Predicate1 & pre, ShevList & list )
{
    if ( ! fPtr ) return *this;
    ShevItem * fi = fPtr;
    while ( ! pre ( fi ) )
    {
        fi = fi->nextPtr;
        if ( fi == 0 ) return *this;
    }

    ShevItem * li = fi;
    lPtr = fi->prevPtr;
    if ( lPtr == 0 ) fPtr = 0;
    ShevItem * p = fi->nextPtr;
    if ( this == &list )
    {
        while ( p != 0 )
        {
            if ( pre ( p ) )
            {
                if ( p->prevPtr != li )
                {
                    li->nextPtr = p;
                    p->prevPtr = li;
                }
                li = p;
            }
            else
            {
                if ( p->prevPtr != lPtr )
                {
                    if ( lPtr == 0 )
                    {
                        fPtr = p;
                        fPtr->prevPtr = 0;
                    }
                    else
                    {
                        lPtr->nextPtr = p;
                        p->prevPtr = lPtr;
                    }
                }
                lPtr = p;
            }
            p = p->nextPtr;
        }
        fi->prevPtr = 0;
        if ( fPtr != 0 )
        {
            fPtr->prevPtr = li;
            li->nextPtr = fPtr;
            lPtr->nextPtr = 0;
        }
        else
        {
            lPtr = li;
        }
        fPtr = fi;
    }
    else
    {
        nat count = 1;
        while ( p != 0 )
        {
            if ( pre ( p ) )
            {
                if ( p->prevPtr != li )
                {
                    li->nextPtr = p;
                    p->prevPtr = li;
                }
                li = p;
                ++count;
            }
            else
            {
                if ( p->prevPtr != lPtr )
                {
                    if ( lPtr == 0 )
                    {
                        fPtr = p;
                        fPtr->prevPtr = 0;
                    }
                    else
                    {
                        lPtr->nextPtr = p;
                        p->prevPtr = lPtr;
                    }
                }
                lPtr = p;
            }
            p = p->nextPtr;
        }
        fi->prevPtr = 0;
        if ( ( cPtr = lPtr ) != 0 ) fPtr->prevPtr = lPtr->nextPtr = 0;
        listSize -= count;

        if ( list.fPtr != 0 )
        {
            list.fPtr->prevPtr = li;
            li->nextPtr = list.fPtr;
        }
        else
        {
            li->nextPtr = 0;
            list.cPtr = list.lPtr = li;
        }
        list.fPtr = fi;
        list.listSize += count;
    }
    return *this;
}

ShevList & ShevList::mov321BefFir ( Predicate1 & pre, ShevList & list )
{
    if ( ! lPtr ) return *this;
    ShevItem * li = lPtr;
    while ( ! pre ( li ) )
    {
        li = li->prevPtr;
        if ( li == 0 ) return *this;
    }

    ShevItem * fi = li;
    fPtr = li->nextPtr;
    if ( fPtr == 0 ) lPtr = 0;
    ShevItem * p = li->prevPtr;
    if ( this == &list )
    {
        while ( p != 0 )
        {
            if ( pre ( p ) )
            {
                if ( p->nextPtr != fi )
                {
                    fi->prevPtr = p;
                    p->nextPtr = fi;
                }
                fi = p;
            }
            else
            {
                if ( p->nextPtr != fPtr )
                {
                    if ( fPtr == 0 )
                    {
                        lPtr = p;
                        lPtr->nextPtr = 0;
                    }
                    else
                    {
                        fPtr->prevPtr = p;
                        p->nextPtr = fPtr;
                    }
                }
                fPtr = p;
            }
            p = p->prevPtr;
        }
        fi->prevPtr = 0;
        if ( fPtr != 0 )
        {
            fPtr->prevPtr = li;
            li->nextPtr = fPtr;
        }
        else
        {
            lPtr = li;
        }
        fPtr = fi;
    }
    else
    {
        nat count = 1;
        while ( p != 0 )
        {
            if ( pre ( p ) )
            {
                if ( p->nextPtr != fi )
                {
                    fi->prevPtr = p;
                    p->nextPtr = fi;
                }
                fi = p;
                ++count;
            }
            else
            {
                if ( p->nextPtr != fPtr )
                {
                    if ( fPtr == 0 )
                    {
                        lPtr = p;
                        lPtr->nextPtr = 0;
                    }
                    else
                    {
                        fPtr->prevPtr = p;
                        p->nextPtr = fPtr;
                    }
                }
                fPtr = p;
            }
            p = p->prevPtr;
        }
        fi->prevPtr = 0;
        if ( ( cPtr = fPtr ) != 0 ) fPtr->prevPtr = lPtr->nextPtr = 0;
        listSize -= count;

        if ( list.fPtr != 0 )
        {
            list.fPtr->prevPtr = li;
            li->nextPtr = list.fPtr;
        }
        else
        {
            li->nextPtr = 0;
            list.cPtr = list.lPtr = li;
        }
        list.fPtr = fi;
        list.listSize += count;
    }
    return *this;
}

// Вынос элементов из списка

ShevItem * ShevList::outFir()
{
    if ( ! cPtr ) return cPtr;
    ShevItem * v = fPtr;
    if ( fPtr == lPtr )
    {
        fPtr = cPtr = lPtr = 0;
        listSize = 0;
    }
    else
    {
        ( fPtr = v -> nextPtr ) -> prevPtr = 0;
        if ( cPtr == v ) cPtr = fPtr;
        --listSize;
    }
    return v;
}

ShevItem * ShevList::outCur()
{
    if ( ! cPtr ) return cPtr;
    ShevItem * v = cPtr;
    if ( v -> nextPtr ) ( cPtr = v -> nextPtr ) -> prevPtr = v -> prevPtr;
    else lPtr = cPtr = v -> prevPtr;
    if ( v -> prevPtr ) v -> prevPtr -> nextPtr = v -> nextPtr;
    else fPtr = cPtr;
    --listSize;
    return v;
}

ShevItem * ShevList::outLas()
{
    if ( ! cPtr ) return cPtr;
    ShevItem * v = lPtr;
    if ( fPtr == lPtr )
    {
        fPtr = cPtr = lPtr = 0;
        listSize = 0;
    }
    else
    {
        ( lPtr = v -> prevPtr ) -> nextPtr = 0;
        if ( cPtr == v ) cPtr = lPtr;
        --listSize;
    }
    return v;
}

ShevList & ShevList::sort123 ()
{
    if ( listSize == 2 )
    {
        if ( fPtr->info > lPtr->info )
        {
            cPtr = fPtr;
            fPtr = lPtr;
            lPtr = cPtr;
            fPtr->nextPtr = lPtr;
            lPtr->prevPtr = fPtr;
            fPtr->prevPtr = lPtr->nextPtr = 0;
        }
        return *this;
    }
    if ( listSize < 2 ) return *this;
    ShevList list;
    movLasAftLas ( listSize / 2, list ).sort123 ();
    list.sort123 ();
    ShevItem * ptr;
    if ( fPtr->info <= list.fPtr->info )
    {
        ptr = fPtr;
        cPtr = ptr->nextPtr;
        list.cPtr = list.fPtr;
    }
    else
    {
        ptr = list.fPtr;
        list.cPtr = ptr->nextPtr;
        cPtr = fPtr;
        fPtr = ptr;
    }
    for (;;)
    {
        if ( ! cPtr )
        {
            list.cPtr->prevPtr = ptr;
            ptr->nextPtr = list.cPtr;
            cPtr = lPtr = list.lPtr;
            break;
        }
        if ( ! list.cPtr )
        {
            cPtr->prevPtr = ptr;
            ptr->nextPtr = cPtr;
            break;
        }
        if ( cPtr->info <= list.cPtr->info )
        {
            ptr->nextPtr = cPtr;
            cPtr->prevPtr = ptr;
            ptr = cPtr;
            cPtr = ptr->nextPtr;
        }
        else
        {
            ptr->nextPtr = list.cPtr;
            list.cPtr->prevPtr = ptr;
            ptr = list.cPtr;
            list.cPtr = ptr->nextPtr;
        }
    }
    list.fPtr = list.cPtr = list.lPtr = 0;
    listSize += list.listSize;
    return *this;
}

ShevList & ShevList::sort321 ()
{
    if ( listSize == 2 )
    {
        if ( fPtr->info < lPtr->info )
        {
            cPtr = fPtr;
            fPtr = lPtr;
            lPtr = cPtr;
            fPtr->nextPtr = lPtr;
            lPtr->prevPtr = fPtr;
            fPtr->prevPtr = lPtr->nextPtr = 0;
        }
        return *this;
    }
    if ( listSize < 2 ) return *this;
    ShevList list;
    movLasAftLas ( listSize / 2, list ).sort321 ();
    list.sort321 ();
    ShevItem * ptr;
    if ( fPtr->info >= list.fPtr->info )
    {
        ptr = fPtr;
        cPtr = ptr->nextPtr;
        list.cPtr = list.fPtr;
    }
    else
    {
        ptr = list.fPtr;
        list.cPtr = ptr->nextPtr;
        cPtr = fPtr;
        fPtr = ptr;
    }
    for (;;)
    {
        if ( ! cPtr )
        {
            list.cPtr->prevPtr = ptr;
            ptr->nextPtr = list.cPtr;
            cPtr = lPtr = list.lPtr;
            break;
        }
        if ( ! list.cPtr )
        {
            cPtr->prevPtr = ptr;
            ptr->nextPtr = cPtr;
            break;
        }
        if ( cPtr->info >= list.cPtr->info )
        {
            ptr->nextPtr = cPtr;
            cPtr->prevPtr = ptr;
            ptr = cPtr;
            cPtr = ptr->nextPtr;
        }
        else
        {
            ptr->nextPtr = list.cPtr;
            list.cPtr->prevPtr = ptr;
            ptr = list.cPtr;
            list.cPtr = ptr->nextPtr;
        }
    }
    list.fPtr = list.cPtr = list.lPtr = 0;
    listSize += list.listSize;
    return *this;
}

ShevList & ShevList::sort ( Predicate2 & pre )
{
    if ( listSize == 2 )
    {
        if ( ! pre ( fPtr, lPtr ) )
        {
            cPtr = fPtr;
            fPtr = lPtr;
            lPtr = cPtr;
            fPtr->nextPtr = lPtr;
            lPtr->prevPtr = fPtr;
            fPtr->prevPtr = lPtr->nextPtr = 0;
        }
        return *this;
    }
    if ( listSize < 2 ) return *this;
    ShevList list;
    movLasAftLas ( listSize / 2, list );
    sort ( pre );
    list.sort ( pre );
    ShevItem * ptr;
    if ( pre ( fPtr, list.fPtr ) )
    {
        ptr = fPtr;
        cPtr = ptr->nextPtr;
        list.cPtr = list.fPtr;
    }
    else
    {
        ptr = list.fPtr;
        list.cPtr = ptr->nextPtr;
        cPtr = fPtr;
        fPtr = ptr;
    }
    for (;;)
    {
        if ( ! cPtr )
        {
            list.cPtr->prevPtr = ptr;
            ptr->nextPtr = list.cPtr;
            cPtr = lPtr = list.lPtr;
            break;
        }
        if ( ! list.cPtr )
        {
            cPtr->prevPtr = ptr;
            ptr->nextPtr = cPtr;
            break;
        }
        if ( pre ( cPtr, list.cPtr ) )
        {
            ptr->nextPtr = cPtr;
            cPtr->prevPtr = ptr;
            ptr = cPtr;
            cPtr = ptr->nextPtr;
        }
        else
        {
            ptr->nextPtr = list.cPtr;
            list.cPtr->prevPtr = ptr;
            ptr = list.cPtr;
            list.cPtr = ptr->nextPtr;
        }
    }
    list.fPtr = list.cPtr = list.lPtr = 0;
    listSize += list.listSize;
    return *this;
}

// Имеется ли в списке данный элемент?

bool ShevList::has ( const ShevItem * m ) const
{
    const ShevItem * t = fPtr;
    while ( t )
    {
        if ( t == m ) return true;
        t = t->nextPtr;
    }
    return false;
}

bool ShowList::has ( const ShevItem * m ) const
{
    const ShevItem * t = fPtr;
    while ( t )
    {
        if ( t == m ) return true;
        t = t->nextPtr;
    }
    return false;
}

// Для всех элементов присвоить члену info значение i

ShevList & ShevList::setAllInfo ( int i )
{
    ShevItem * t = fPtr;
    while ( t )
    {
        t->info = i;
        t = t->nextPtr;
    }
    return *this;
}


// Обработать все элементы списка:

ShevList & ShevList::for_each123 ( Alter & alt )
{
    ShevItem * t = fPtr;
    while ( t )
    {
        alt ( t );
        t = t->nextPtr;
    }
    return *this;
}

ShevList & ShevList::for_each321 ( Alter & alt )
{
    ShevItem * t = lPtr;
    while ( t )
    {
        alt ( t );
        t = t->prevPtr;
    }
    return *this;
}

// Обработать некоторые элементы списка:

ShevList & ShevList::for_some123 ( Predicate1 & pre, Alter & alt )
{
    ShevItem * t = fPtr;
    while ( t )
    {
        if ( pre ( t ) ) alt ( t );
        t = t->nextPtr;
    }
    return *this;
}

ShevList & ShevList::for_some321 ( Predicate1 & pre, Alter & alt )
{
    ShevItem * t = lPtr;
    while ( t )
    {
        if ( pre ( t ) ) alt ( t );
        t = t->prevPtr;
    }
    return *this;
}

ShevList & ShevList::invert ()
{
    ShevItem * temp, * item;

    for ( item = fPtr; item; item = temp )
    {
        temp = item->nextPtr;
        item->nextPtr = item->prevPtr;
        item->prevPtr = temp;
    }
    _swap ( lPtr, fPtr );
    return *this;
}

ShevList & ShevList::makeFir ( ShevItem * p )
{
    if ( fPtr == p ) return *this;
    fPtr->prevPtr = lPtr;
    lPtr->nextPtr = fPtr;
    lPtr = p->prevPtr;
    fPtr = p;
    fPtr->prevPtr = lPtr->nextPtr = 0;
    return *this;
}

ShevList & ShevList::makeLas ( ShevItem * p )
{
    if ( lPtr == p ) return *this;
    fPtr->prevPtr = lPtr;
    lPtr->nextPtr = fPtr;
    fPtr = p->nextPtr;
    lPtr = p;
    fPtr->prevPtr = lPtr->nextPtr = 0;
    return *this;
}

// Перенос всех элементов перед текущим из одного списка в другой список перед текущим

ShevList & ShevList::movBefCurBefCur ( ShevList & list )
{
    if ( fPtr == cPtr || this == &list ) return *this;
    nat n = 0;
    ShevItem * t = cPtr->prevPtr;
    while ( t ) t = t->prevPtr, ++n;
    list.listSize += n;
    listSize -= n;
    if ( list.cPtr )
    {
        ( list.cPtr->prevPtr ? list.cPtr->prevPtr->nextPtr : list.fPtr ) = fPtr;
        fPtr->prevPtr = list.cPtr->prevPtr;
        cPtr->prevPtr->nextPtr = list.cPtr;
        list.cPtr->prevPtr = cPtr->prevPtr;
    }
    else
    {
        list.lPtr = list.cPtr = cPtr->prevPtr;
        list.lPtr->nextPtr = 0;
        list.fPtr = fPtr;
    }
    fPtr = cPtr;
    fPtr->prevPtr = 0;
    return *this;
}

// Перенос всех элементов после текущего из одного списка в другой список после текущего

ShevList & ShevList::movAftCurAftCur ( ShevList & list )
{
    if ( lPtr == cPtr || this == &list ) return *this;
    nat n = 0;
    ShevItem * t = cPtr->nextPtr;
    while ( t ) t = t->nextPtr, ++n;
    list.listSize += n;
    listSize -= n;
    if ( list.cPtr )
    {
        ( list.cPtr->nextPtr ? list.cPtr->nextPtr->prevPtr : list.lPtr ) = lPtr;
        lPtr->nextPtr = list.cPtr->nextPtr;
        cPtr->nextPtr->prevPtr = list.cPtr;
        list.cPtr->nextPtr = cPtr->nextPtr;
    }
    else
    {
        list.fPtr = list.cPtr = cPtr->nextPtr;
        list.fPtr->prevPtr = 0;
        list.lPtr = lPtr;
    }
    lPtr = cPtr;
    lPtr->nextPtr = 0;
    return *this;
}

// Перенос п первых элементов из одного списка в начало другого списка

ShevList & ShevList::movFirBefFir ( nat n, ShevList & list )
{
    if ( n == 0 || listSize == 0 || this == &list ) return *this;
    if ( n >= listSize ) return movAllBefFir (list );
    cPtr = fPtr;
    for ( nat i = n; i-- > 0; ) cPtr = cPtr->nextPtr;
    if ( list.listSize > 0 )
    {
        cPtr->prevPtr->nextPtr = list.fPtr;
        list.fPtr->prevPtr = cPtr->prevPtr;
        list.fPtr = fPtr;
        list.listSize += n;
    }
    else
    {
        list.lPtr = cPtr->prevPtr;
        list.lPtr->nextPtr = 0;
        list.cPtr = list.fPtr = fPtr;
        list.listSize = n;
    }
    fPtr = cPtr;
    fPtr->prevPtr = 0;
    listSize -= n;
    return *this;
}

// Перенос п последних элементов из одного списка в конец другого списка

ShevList & ShevList::movLasAftLas ( nat n, ShevList & list )
{
    if ( n == 0 || listSize == 0 || this == &list ) return *this;
    if ( n >= listSize ) return movAllAftLas (list );
    cPtr = lPtr;
    for ( nat i = n; i-- > 0; ) cPtr = cPtr->prevPtr;
    if ( list.lPtr == 0 )
    {
        list.fPtr = list.cPtr = cPtr->nextPtr;
        list.fPtr->prevPtr = 0;
    }
    else
    {
        cPtr->nextPtr->prevPtr = list.lPtr;
        list.lPtr->nextPtr = cPtr->nextPtr;
    }
    list.lPtr = lPtr;
    lPtr = cPtr;
    lPtr->nextPtr = 0;
    list.listSize += n;
    listSize -= n;
    return *this;
}

// Перенос всех элементов из одного списка в начало другого списка

ShevList & ShevList::movAllBefFir ( ShevList & list )
{
    if ( ! cPtr || this == &list ) return *this;
    if ( list.cPtr )
    {
        lPtr->nextPtr = list.fPtr;
        list.fPtr->prevPtr = lPtr;
        list.fPtr = fPtr;
        list.listSize += listSize;
    }
    else
    {
        list.cPtr = cPtr;
        list.fPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize = listSize;
    }
    fPtr = cPtr = lPtr = 0;
    listSize = 0;
    return *this;
}

// Перенос всех элементов из одного списка перед текущим другого списка

ShevList & ShevList::movAllBefCur ( ShevList & list )
{
    if ( ! cPtr || this == &list ) return *this;
    if ( list.cPtr )
    {
        fPtr->prevPtr = list.cPtr->prevPtr;
        lPtr->nextPtr = list.cPtr;
        list.cPtr->prevPtr = lPtr;
        if ( fPtr->prevPtr )
             fPtr->prevPtr->nextPtr = fPtr;
        else list.fPtr = fPtr;
        list.listSize += listSize;
    }
    else
    {
        list.fPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize = listSize;
    }
    list.cPtr = cPtr;
    fPtr = cPtr = lPtr = 0;
    listSize = 0;
    return *this;
}

// Перенос всех элементов из одного списка после текущего другого списка

ShevList & ShevList::movAllAftCur ( ShevList & list )
{
    if ( ! cPtr || this == &list ) return *this;
    if ( list.cPtr )
    {
        fPtr->prevPtr = list.cPtr;
        lPtr->nextPtr = list.cPtr->nextPtr;
        list.cPtr->nextPtr = fPtr;
        if ( lPtr->nextPtr )
             lPtr->nextPtr->prevPtr = lPtr;
        else list.lPtr = lPtr;
        list.listSize += listSize;
    }
    else
    {
        list.fPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize = listSize;
    }
    list.cPtr = cPtr;
    fPtr = cPtr = lPtr = 0;
    listSize = 0;
    return *this;
}

// Перенос всех элементов из одного списка в конец другого списка

ShevList & ShevList::movAllAftLas ( ShevList & list )
{
    if ( ! cPtr || this == &list ) return *this;
    if ( list.cPtr )
    {
        fPtr->prevPtr = list.lPtr;
        list.lPtr->nextPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize += listSize;
    }
    else
    {
        list.cPtr = cPtr;
        list.fPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize = listSize;
    }
    fPtr = cPtr = lPtr = 0;
    listSize = 0;
    return *this;
}

// Перенос всех элементов после текущего из одного списка в конец другого списка

ShevList & ShevList::movAftCurAftLas ( ShevList & list )
{
    if ( cPtr == 0 || cPtr->nextPtr == 0 || this == &list ) return *this;
    nat n = 0;
    lPtr = cPtr;
    while ( lPtr->nextPtr != 0 )
    {
        lPtr = lPtr->nextPtr;
        ++n;
    }
    if ( list.lPtr == 0 )
    {
        list.fPtr = list.cPtr = cPtr->nextPtr;
        list.fPtr->prevPtr = 0;
    }
    else
    {
        cPtr->nextPtr->prevPtr = list.lPtr;
        list.lPtr->nextPtr = cPtr->nextPtr;
    }
    list.lPtr = lPtr;
    lPtr = cPtr;
    lPtr->nextPtr = 0;
    list.listSize += n;
    listSize -= n;
    return *this;
}

// Обмен элементами внутри списка:

ShevList & ShevList::swapCurAndLas()
{
    if ( cPtr == lPtr ) return *this;
    if ( cPtr == fPtr )
    {
        swapFirAndLas(); return *this;
    }
    _swap ( cPtr, lPtr );
    if ( lPtr->nextPtr == cPtr )
    {
        lPtr->prevPtr->nextPtr = cPtr;
        cPtr->prevPtr = lPtr->prevPtr;
        cPtr->nextPtr = lPtr;
        lPtr->prevPtr = cPtr;
        lPtr->nextPtr = 0;
    }
    else
    {
        _swap ( cPtr->prevPtr, lPtr->prevPtr );
        _swap ( cPtr->nextPtr, lPtr->nextPtr );
        cPtr->prevPtr->nextPtr = cPtr;
        cPtr->nextPtr->prevPtr = cPtr;
        lPtr->prevPtr->nextPtr = lPtr;
    }
    return *this;
}

ShevList & ShevList::swapCurAndFir()
{
    if ( cPtr == fPtr ) return *this;
    if ( cPtr == lPtr )
    {
        swapFirAndLas(); return *this;
    }
    _swap ( cPtr, fPtr );
    if ( fPtr->prevPtr == cPtr )
    {
        fPtr->nextPtr->prevPtr = cPtr;
        cPtr->nextPtr = fPtr->nextPtr;
        cPtr->prevPtr = fPtr;
        fPtr->nextPtr = cPtr;
        fPtr->prevPtr = 0;
    }
    else
    {
        _swap ( cPtr->prevPtr, fPtr->prevPtr );
        _swap ( cPtr->nextPtr, fPtr->nextPtr );
        cPtr->nextPtr->prevPtr = cPtr;
        cPtr->prevPtr->nextPtr = cPtr;
        fPtr->nextPtr->prevPtr = fPtr;
    }
    return *this;
}

ShevList & ShevList::swapFirAndLas()
{
    if ( lPtr == fPtr ) return *this;
    _swap ( fPtr, lPtr );
    if ( listSize == 2 )
    {
        fPtr->nextPtr = lPtr;
        lPtr->prevPtr = fPtr;
        fPtr->prevPtr = lPtr->nextPtr = 0;
    }
    else
    {
        fPtr->nextPtr = lPtr->nextPtr;
        lPtr->prevPtr = fPtr->prevPtr;
        fPtr->prevPtr = lPtr->nextPtr = 0;
        fPtr->nextPtr->prevPtr = fPtr;
        lPtr->prevPtr->nextPtr = lPtr;
    }
    if ( cPtr == fPtr ) cPtr = lPtr; else
    if ( cPtr == lPtr ) cPtr = fPtr;
    return *this;
}

// Обмен элементами между двумя списками:

ShevList & ShevList::swapCurAndCur ( ShevList & list ) // Обмен текущими элементами
{
    if ( this == &list || !cPtr || !list.cPtr ) return *this;
    if ( cPtr->prevPtr ) cPtr->prevPtr->nextPtr = list.cPtr;
    if ( cPtr->nextPtr ) cPtr->nextPtr->prevPtr = list.cPtr;
    if ( list.cPtr->prevPtr ) list.cPtr->prevPtr->nextPtr = cPtr;
    if ( list.cPtr->nextPtr ) list.cPtr->nextPtr->prevPtr = cPtr;
    _swap ( cPtr->prevPtr, list.cPtr->prevPtr );
    _swap ( cPtr->nextPtr, list.cPtr->nextPtr );
    if ( fPtr == cPtr ) fPtr = list.cPtr;
    if ( lPtr == cPtr ) lPtr = list.cPtr;
    if ( list.fPtr == list.cPtr ) list.fPtr = cPtr;
    if ( list.lPtr == list.cPtr ) list.lPtr = cPtr;
    _swap ( cPtr, list.cPtr );
    return *this;
}

ShevList & ShevList::swapAllAndAll ( ShevList & list ) // Обмен всеми элементами
{
    _swap ( cPtr, list.cPtr );
    _swap ( fPtr, list.fPtr );
    _swap ( lPtr, list.lPtr );
    _swap ( listSize, list.listSize );
    return *this;
}

// Проверка на наличие ошибок ( возвращает указатель на строку с описанием ошибки или 0, если ошибок не обнаружено )

const char * ShevList::test() const
{
    if ( listSize == 0 )
    {
        if ( fPtr ) return "listSize == 0 && fPtr != 0";
        if ( cPtr ) return "listSize == 0 && cPtr != 0";
        if ( lPtr ) return "listSize == 0 && lPtr != 0";
        return 0;
    }
    if ( ! fPtr ) return "listSize > 0 && fPtr == 0";
    if ( ! cPtr ) return "listSize > 0 && cPtr == 0";
    if ( ! lPtr ) return "listSize > 0 && lPtr == 0";
    if ( fPtr->prevPtr ) return "fPtr->prevPtr != 0";
    if ( lPtr->nextPtr ) return "lPtr->nextPtr != 0";
    if ( listSize == 1 )
    {
        if ( fPtr != cPtr ) return "listSize == 1 && fPtr != cPtr";
        if ( cPtr != lPtr ) return "listSize == 1 && cPtr != lPtr";
        if ( lPtr != fPtr ) return "listSize == 1 && lPtr != fPtr";
        return 0;
    }
    if ( ! fPtr->nextPtr ) return "listSize > 1 && fPtr->nextPtr == 0";
    if ( ! lPtr->prevPtr ) return "listSize > 1 && lPtr->prevPtr == 0";
    if ( fPtr == lPtr    ) return "listSize > 1 && fPtr == lPtr";
    if ( listSize == 2 )
    {
        if ( fPtr != cPtr && cPtr != lPtr )
            return "listSize == 2 && fPtr != cPtr && cPtr != lPtr";
        if ( fPtr->nextPtr != lPtr )
            return "listSize == 2 && fPtr->nextPtr != lPtr";
        if ( lPtr->prevPtr != fPtr )
            return "listSize == 2 && lPtr->prevPtr != fPtr";
        return 0;
    }
    ShevItem * p = fPtr;
    for ( nat i = 1; i < listSize; ++i )
    {
        if ( p == lPtr ) return "p == lPtr && i < listSize";
        if ( p->nextPtr == 0 ) return "p->nextPtr == 0 && p != lPtr";
        if ( p->nextPtr == fPtr ) return "p->nextPtr == fPtr";
        if ( p->nextPtr->prevPtr != p ) return "p->nextPtr->prevPtr != p";
        p = p->nextPtr;
    }
    if ( p != lPtr ) return "p != lPtr && i == listSize";
    return 0;
}

} // namespace SL;
