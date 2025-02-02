
#pragma once

#include "ShevArray.h"

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

//************************************* SemiRib *******************************************//

struct SemiRib
{
    unsigned int next;
    unsigned int twin;
    unsigned int vert;
};

//************************************ 17.07.2007 *****************************************//
//
//              Локальная оптимизация заданной триангуляции optiL1
//
//************************************ 08.01.2021 *****************************************//

template <class T> class TriDiag
{
public:
    T value; // Качество диагонали
    nat rib, facet1, facet2; // Индексы полуребра и соседних граней
};

template <> class TriDiag<void>
{
public:
    nat rib, facet1, facet2; // Индексы полуребра и соседних граней
};

class IDiagFunc
{
public:
    virtual IDiagFunc & link ( CArrRef<SemiRib> & r ) = 0;
    virtual bool operator () ( nat r, nat s1, nat s2 ) = 0;
};

void optiL ( IDiagFunc & change, ArrRef<SemiRib> & rib );
void optiL ( IDiagFunc & change, CCArrRef<Set3<nat> > & trian, DynArray<SemiRib> & rib );

template <class T>
void maxL1 ( const Func3a<T,nat> & quality, const Func2a<T,T> & merge, ArrRef<SemiRib> & rib )
{
    const nat nr = rib.size();
    if ( nr < 6 || nr % 3 != 0 ) return;
    const nat nf = nr / 3;
    DynArray<T> value ( nf ); // Массив качества граней
    nat i;
    for ( i = 0; i < nf; ++i )
    {
        const SemiRib & a = rib[3 * i];
        const SemiRib & b = rib[a.next];
        const SemiRib & c = rib[b.next];
        value[i] = quality ( a.vert, b.vert, c.vert );
    }
    DynArray<nat> dinx ( nr, nr );
    Suite<TriDiag<T> > diag ( nr/2 ); // Массив диагоналей
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
            dinx[ia] = dinx[a.twin] = diag.size();
            TriDiag<T> & d = diag.inc();
            d.value = merge ( value[d.facet1=i], value[d.facet2=a.twin/3] );
            d.rib = ia;
        }
        if ( b.twin < nr )
        {
            dinx[ib] = dinx[b.twin] = diag.size();
            TriDiag<T> & d = diag.inc();
            d.value = merge ( value[d.facet1=i], value[d.facet2=b.twin/3] );
            d.rib = ib;
        }
        if ( c.twin < nr )
        {
            dinx[ic] = dinx[c.twin] = diag.size();
            TriDiag<T> & d = diag.inc();
            d.value = merge ( value[d.facet1=i], value[d.facet2=c.twin/3] );
            d.rib = ic;
        }
    }
    const nat nd = diag.size();
    Suite<nat> stack ( nd, nd ); // Очередь на проверку оптимальности диагоналей
    for ( i = 0; i < nd; ++i ) stack[i] = i;
    while ( stack.size() > 0 )
    {
        TriDiag<T> & d = diag[stack.las()];
        stack.dec();
        const nat a1 = d.rib;
        const nat b1 = rib[a1].next;
        const nat c1 = rib[b1].next;
        const nat a2 = rib[a1].twin;
        const nat b2 = rib[a2].next;
        const nat c2 = rib[b2].next;
        const T q3 = quality ( rib[c2].vert, rib[b1].vert, rib[c1].vert );
        if ( d.value >= q3 )
        {
            d.rib += nr;
            continue;
        }
        const T q4 = quality ( rib[c1].vert, rib[b2].vert, rib[c2].vert );
        const T qq = merge ( q3, q4 );
        if ( d.value < qq )
        {
            d.value = qq;
            value[d.facet1] = q3;
            value[d.facet2] = q4;
            rib[a1].vert = rib[c1].vert;
            rib[a1].next = c2;
            rib[c2].next = b1;
            rib[b1].next = a1;
            rib[a2].vert = rib[c2].vert;
            rib[a2].next = c1;
            rib[c1].next = b2;
            rib[b2].next = a2;
            i = dinx[c1];
            if ( i < nr )
            {
                TriDiag<T> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
                ( di.facet1 == d.facet1 ? di.facet1 : di.facet2 ) = d.facet2;
                di.value = merge ( value[di.facet1], value[di.facet2] );
            }
            i = dinx[b1];
            if ( i < nr )
            {
                TriDiag<T> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
                di.value = merge ( value[di.facet1], value[di.facet2] );
            }
            i = dinx[c2];
            if ( i < nr )
            {
                TriDiag<T> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
                ( di.facet1 == d.facet2 ? di.facet1 : di.facet2 ) = d.facet1;
                di.value = merge ( value[di.facet1], value[di.facet2] );
            }
            i = dinx[b2];
            if ( i < nr )
            {
                TriDiag<T> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
                di.value = merge ( value[di.facet1], value[di.facet2] );
            }
        }
        d.rib += nr;
    }
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
