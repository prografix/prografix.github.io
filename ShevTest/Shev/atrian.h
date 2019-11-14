
#pragma once

#include "ShevArray.h"
#include "SemiRib.h"

//*****************************************************************************************//

template <class F, class A1, class A2=A1>
class Func2a // Функция двух аргументов
{
public:
    virtual F operator () ( A1 a1, A2 a2 ) const = 0;
};

template <class F, class A1, class A2=A1, class A3=A2>
class Func3a // Функция трёх аргументов
{
public:
    virtual F operator () ( A1 a1, A2 a2, A3 a3 ) const = 0;
};

//*****************************************************************************************//

template <class T> // Минимум двух аргументов
class Min2a : public Func2a<T,T>
{
public:
    virtual T operator () ( T a, T b ) const
    {
        return a < b ? a : b;
    }
};

template <class T> // Сумма двух аргументов
class Sum2a : public Func2a<T,T>
{
public:
    virtual T operator () ( T a, T b ) const
    {
        return a + b;
    }
};

template <class T> // Сумма положительных аргументов, иначе минимум
class PosSum2a : public Func2a<T,T>
{
public:
    virtual T operator () ( T a, T b ) const
    {
        if ( a > 0 && b > 0 )
        {
            return a + b;
        }
        return a < b ? a : b;
    }
};

//************************************ 17.07.2007 *****************************************//
//
//              Локальная оптимизация заданной триангуляции optiL1
//
//************************************ 23.08.2018 *****************************************//

template <class T>
void maxL1 ( const Func3a<T,nat> & quality, const Func2a<T,T> & merge, ArrRef<SemiRib> rib )
{
    const nat nr = rib.size();
    if ( nr < 6 || nr % 3 != 0 ) return;
    const nat nf = nr / 3;
    typedef ListItemA<T> Item;
    DynArray<Item*> diag ( nr );
    DynArray<nat> finx ( nr );
    DynArray<T> value ( nf );
    List<Item> list;
    nat i;
    for ( i = 0; i < nf; ++i )
    {
        const SemiRib & a = rib[3 * i];
        const SemiRib & b = rib[a.next];
        const SemiRib & c = rib[b.next];
        value[i] = quality ( a.vert, b.vert, c.vert );
    }
    for ( i = 0; i < nf; ++i )
    {
        const nat ia = 3 * i;
        const SemiRib & a = rib[ia];
        const nat ib = a.next;
        const SemiRib & b = rib[ib];
        const nat ic = b.next;
        const SemiRib & c = rib[ic];
        if ( a.twin < nr )
        {
            list.addAftCur ( diag[ia] = diag[a.twin] = new Item ( merge ( value[i], value[a.twin/3] ) ) );
            list.cur()->info = ia;
        }
        else
        {
            diag[ia] = 0;
        }
        if ( b.twin < nr )
        {
            list.addAftCur ( diag[ib] = diag[b.twin] = new Item ( merge ( value[i], value[b.twin/3] ) ) );
            list.cur()->info = ib;
        }
        else
        {
            diag[ib] = 0;
        }
        if ( c.twin < nr )
        {
            list.addAftCur ( diag[ic] = diag[c.twin] = new Item ( merge ( value[i], value[c.twin/3] ) ) );
            list.cur()->info = ic;
        }
        else
        {
            diag[ic] = 0;
        }
        finx[ia] = i;
        finx[ib] = i;
        finx[ic] = i;
    }
    list.top();
    do
    {
        Item * item = list.cur();
        const nat a1 = item->info;
        const nat b1 = rib[a1].next;
        const nat c1 = rib[b1].next;
        const nat a2 = rib[a1].twin;
        const nat b2 = rib[a2].next;
        const nat c2 = rib[b2].next;
        const T q3 = quality ( rib[c2].vert, rib[b1].vert, rib[c1].vert );
        const T q4 = quality ( rib[c1].vert, rib[b2].vert, rib[c2].vert );
        const T qq = merge ( q3, q4 );
        if ( item->a < qq )
        {
            item->a = qq;
            value[finx[c2] = finx[a1]] = q3;
            value[finx[c1] = finx[a2]] = q4;
            rib[a1].vert = rib[c1].vert;
            rib[a1].next = c2;
            rib[c2].next = b1;
            rib[b1].next = a1;
            rib[a2].vert = rib[c2].vert;
            rib[a2].next = c1;
            rib[c1].next = b2;
            rib[b2].next = a2;
            Item * it = diag[c1];
            if ( it != 0 )
            {
                if ( (nat) it->info >= nr )
                {
                    it->info -= nr;
                    list.jump ( it );
                    list.movCurAftLas ( list );
                }
                it->a = merge ( value[finx[it->info]], value[finx[rib[it->info].twin]] );
            }
            it = diag[b1];
            if ( it != 0 )
            {
                if ( (nat) it->info >= nr )
                {
                    it->info -= nr;
                    list.jump ( it );
                    list.movCurAftLas ( list );
                }
                it->a = merge ( value[finx[it->info]], value[finx[rib[it->info].twin]] );
            }
            it = diag[c2];
            if ( it != 0 )
            {
                if ( (nat) it->info >= nr )
                {
                    it->info -= nr;
                    list.jump ( it );
                    list.movCurAftLas ( list );
                }
                it->a = merge ( value[finx[it->info]], value[finx[rib[it->info].twin]] );
            }
            it = diag[b2];
            if ( it != 0 )
            {
                if ( (nat) it->info >= nr )
                {
                    it->info -= nr;
                    list.jump ( it );
                    list.movCurAftLas ( list );
                }
                it->a = merge ( value[finx[it->info]], value[finx[rib[it->info].twin]] );
            }
            list.jump ( item );
        }
        item->info += nr;
    }
    while ( list.next() );
}

//************************************ 18.07.2007 *****************************************//
//
//      Триангуляция многоугольника при помощи глобальной оптимизации optiG1
//
//************************************ 27.08.2011 *****************************************//

template <class T> 
SuiteRef<Set3<nat> > & maxG1 ( const Func3a<T,nat> & quality, const Func2a<T,T> & merge, nat nv, SuiteRef<Set3<nat> > & res )
{
    res.resize(0);
    if ( nv <= 2 ) return res;
    if ( nv == 3 )
    {
        Set3<nat> & t = res.inc();
        t.a = 0;
        t.b = 1;
        t.c = 2;
        return res;
    }

// Заполнение массивов a и b

    const nat n2 = (nv-1) * (nv-2) / 2;
    DynArray<T> a ( n2 );
    DynArray<nat> b ( n2 + nv );
    for ( nat k = 2; k < nv; ++k )
    {
        for ( nat i1 = nv - k; --i1 < nv; )
        {
            const nat i2 = i1 + k;
            nat im = i1 + 1;
            T s = quality ( i1, im, i2 );
            const int j = (i2-1) * (i2-2) / 2 + i1;
            if ( k == 2 )
            {
                b[j] = im;
                a[j] = s;
                continue;
            }
            T sum = merge ( s, a[j+1] );
            const nat ik = i2 - 1;
            for ( nat i = i1+2; i < ik; ++i )
            {
                const T q = quality ( i1, i, i2 );
                const T s = merge ( q, merge ( a[(i-1)*(i-2)/2+i1], a[j+i-i1] ) );
                if ( sum < s ) sum = s, im = i;
            }
            s = merge ( quality ( i1, ik, i2 ), a[(ik-1)*(ik-2)/2+i1] );
            if ( sum < s ) sum = s, im = ik;
            b[j] = im;
            a[j] = sum;
        }
    }

// Заполнение массива res

    nat count = 0;
    ArrRef<nat> c ( b, n2, nv );
    c[count++] = 0;
    c[count++] = nv - 1;
    while ( count > 0 )
    {
        const nat j2 = c[--count];
        const nat j1 = c[--count];
        const nat j = b[(j2-1)*(j2-2)/2+j1];
        res.inc() = Set3<nat> ( j1, j, j2 );
        if ( j2 - j > 1 )
        {
            c[count++] = j;
            c[count++] = j2;
        }
        if ( j - j1 > 1 )
        {
            c[count++] = j1;
            c[count++] = j;
        }
    }
    return res;
}
