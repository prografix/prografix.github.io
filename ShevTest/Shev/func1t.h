
#pragma once

#include "ShevArray.h"
#include "rand.h"

//*********************** 06.12.2003 **************************//
//
//                 Сортировки вставками
//
//*********************** 06.07.2011 **************************//

template <class T> inline
ArrRef<T> insertSort123 ( ArrRef<T> & a )
{
    for ( nat j = 1; j < a.size(); ++j )
    {
        const T b = a[j];
        int i = j - 1;
        while ( i >= 0 && a[i] > b )
        {
            a[i+1] = a[i];
            --i;
        }
        a[i+1] = b;
    }
    return a;
}

template <class T> inline
ArrRef<T> insertSort321 ( ArrRef<T> & a )
{
    for ( nat j = 1; j < a.size(); ++j )
    {
        const T b = a[j];
        int i = j - 1;
        while ( i >= 0 && a[i] < b )
        {
            a[i+1] = a[i];
            --i;
        }
        a[i+1] = b;
    }
    return a;
}

//*********************** 05.02.2007 **************************//
//
//                   "Быстрые" сортировки
//
//*********************** 06.12.2014 **************************//

template <class T> 
ArrRef<T> quickSort123 ( ArrRef<T> & a )
{
    const nat n = a.size();
    if ( n < 2 ) return a;
    const nat small_size = sizeof(T) > 8 ? 24 : sizeof(T) > 4 ? 40 : 48;
    CmbSuite<T*, 64> buf;
    T * lo = a();
    T * hi = lo + ( n - 1 );
m1:
    nat size = ( hi - lo ) + 1;
    if ( size <= small_size )
    {
        insertSort123 ( ArrRef<T> ( a, lo - a(), size ) );
    }
    else
    {
        T * mid = lo + ( size / 2 );
        _swap ( *mid, *lo );
        T * lo1 = lo;
        T * hi1 = hi + 1;
        for (;;)
        {
            do
            {
                lo1 += 1;
            }
            while (lo1 <= hi && *lo1 <= *lo );

            do
            {
                hi1 -= 1;
            }
            while ( hi1 > lo && *hi1 >= *lo );

            if ( hi1 < lo1 )
                break;

            _swap ( *lo1, *hi1 );
        }
        _swap ( *lo, *hi1 );
        if ( hi1 - 1 - lo >= hi - lo1 )
        {
            if ( lo + 1 < hi1 )
            {
                buf.inc() = lo;
                buf.inc() = hi1 - 1;
            }

            if ( lo1 < hi )
            {
                lo = lo1;
                goto m1;
            }
        }
        else
        {
            if ( lo1 < hi )
            {
                buf.inc() = lo1;
                buf.inc() = hi;
            }

            if ( lo + 1 < hi1 )
            {
                hi = hi1 - 1;
                goto m1;
            }
        }
    }
    if ( buf.size() > 0 )
    {
        lo = buf.las(1);
        hi = buf.las(0);
        buf.dec(2);
        goto m1;
    }
    return a;
}

template <class T> 
ArrRef<T> quickSort321 ( ArrRef<T> & a )
{
    const nat n = a.size();
    if ( n < 2 ) return a;
    const nat small_size = sizeof(T) > 8 ? 24 : sizeof(T) > 4 ? 40 : 48;
    CmbSuite<T*, 64> buf;
    T * lo = a();
    T * hi = lo + ( n - 1 );
m1:
    nat size = ( hi - lo ) + 1;
    if ( size <= small_size )
    {
        insertSort321 ( ArrRef<T> ( a, lo - a(), size ) );
    }
    else
    {
        T * mid = lo + ( size / 2 );
        _swap ( *mid, *lo );
        T * lo1 = lo;
        T * hi1 = hi + 1;
        for (;;)
        {
            do
            {
                lo1 += 1;
            }
            while (lo1 <= hi && *lo1 >= *lo );

            do
            {
                hi1 -= 1;
            }
            while ( hi1 > lo && *hi1 <= *lo );

            if ( hi1 < lo1 )
                break;

            _swap ( *lo1, *hi1 );
        }
        _swap ( *lo, *hi1 );
        if ( hi1 - 1 - lo >= hi - lo1 )
        {
            if ( lo + 1 < hi1 )
            {
                buf.inc() = lo;
                buf.inc() = hi1 - 1;
            }

            if ( lo1 < hi )
            {
                lo = lo1;
                goto m1;
            }
        }
        else
        {
            if ( lo1 < hi )
            {
                buf.inc() = lo1;
                buf.inc() = hi;
            }

            if ( lo + 1 < hi1 )
            {
                hi = hi1 - 1;
                goto m1;
            }
        }
    }
    if ( buf.size() > 0 )
    {
        lo = buf.las(1);
        hi = buf.las(0);
        buf.dec(2);
        goto m1;
    }
    return a;
}

//*********************** 11.10.2005 **************************//
//
//                   Сортировки слиянием
//
//*********************** 22.08.2011 **************************//

template <class T> 
ArrRef<T> mergeSort123 ( ArrRef<T> & arr, ArrRef<T> & buf )
{
    const nat n = arr.size();
    const nat small_size = sizeof(T) > 8 ? 32 : sizeof(T) > 4 ? 50 : 64;
    if ( n < small_size ) return insertSort123 ( arr );
    nat k = small_size / 2;
    nat m = n, s = 0;
    for(;;)
    {
        if ( m > k )
        {
            insertSort123 ( ArrRef<T> ( arr, s, k ) );
            s += k;
            m -= k;
        }
        else
        {
            insertSort123 ( ArrRef<T> ( arr, s, m ) );
            break;
        }
    }
    bool e = false;
    do
    {
        T * a, * b;
        if ( e )
        {
            b = arr();
            a = buf();
        }
        else
        {
            a = arr();
            b = buf();
        }
        nat i1 = 0;
        for (;;)
        {
            T * p1 = a + i1;
            nat i2 = i1 + k;
            if ( i2 >= n )
            {
                T * a2 = a + n;
                while ( p1 < a2 ) *b++ = *p1++;
                break;
            }
            T * a2 = a + i2;
            T * p2 = a2;
            nat i3 = i2 + k;
            if ( i3 > n ) i3 = n;
            T * a3 = a + i3;
            for (;;)
            {
                if ( *p1 <= *p2 )
                {
                    *b++ = *p1++;
                    if ( p1 == a2 )
                    {
                        while ( p2 < a3 ) *b++ = *p2++;
                        break;
                    }
                }
                else
                {
                    *b++ = *p2++;
                    if ( p2 == a3 )
                    {
                        while ( p1 < a2 ) *b++ = *p1++;
                        break;
                    }
                }
            }
            i1 = i3;
        }
        e = !e;
        k <<= 1;
    }
    while ( k < n );
    if ( e )
    {
        for ( nat i = 0; i < n; ++i ) arr[i] = buf[i];
    }
    return arr;
}

template <class T> 
ArrRef<T> mergeSort321 ( ArrRef<T> & arr, ArrRef<T> & buf )
{
    const nat n = arr.size();
    const nat small_size = sizeof(T) > 8 ? 32 : sizeof(T) > 4 ? 50 : 64;
    if ( n < small_size ) return insertSort321 ( arr );
    nat k = small_size / 2;
    nat m = n, s = 0;
    for(;;)
    {
        if ( m > k )
        {
            insertSort321 ( ArrRef<T> ( arr, s, k ) );
            s += k;
            m -= k;
        }
        else
        {
            insertSort321 ( ArrRef<T> ( arr, s, m ) );
            break;
        }
    }
    bool e = false;
    do
    {
        T * a, * b;
        if ( e )
        {
            b = arr();
            a = buf();
        }
        else
        {
            a = arr();
            b = buf();
        }
        nat i1 = 0;
        for (;;)
        {
            T * p1 = a + i1;
            nat i2 = i1 + k;
            if ( i2 >= n )
            {
                T * a2 = a + n;
                while ( p1 < a2 )
                {
                    *b++ = *p1++;
                }
                break;
            }
            T * a2 = a + i2;
            T * p2 = a2;
            nat i3 = i2 + k;
            if ( i3 > n ) i3 = n;
            T * a3 = a + i3;
            for (;;)
            {
                if ( *p1 >= *p2 )
                {
                    *b++ = *p1++;
                    if ( p1 == a2 )
                    {
                        while ( p2 < a3 )
                        {
                            *b++ = *p2++;
                        }
                        break;
                    }
                }
                else
                {
                    *b++ = *p2++;
                    if ( p2 == a3 )
                    {
                        while ( p1 < a2 )
                        {
                            *b++ = *p1++;
                        }
                        break;
                    }
                }
            }
            i1 = i3;
        }
        e = !e;
        k <<= 1;
    }
    while ( k < n );
    if ( e )
    {
        for ( nat i = 0; i < n; ++i ) arr[i] = buf[i];
    }
    return arr;
}

//*********************** 30.08.2006 **************************//
//
//          Порядковые статистики ( 0 <= i < a.size() )
//
//*********************** 23.01.2016 **************************//

template <class T> 
Def<T> selectR ( CCArrRef<T> & a, nat i )
{
    Def<T> res;
    nat n = a.size();
    if ( i >= n ) return res;
    CmbArray<nat, 80> buf ( n + n );
    nat * buf1 = buf(0);
    nat * buf2 = buf(n);
    nat * s = buf1;
    nat * d = buf2;
    nat j;
    for ( j = 0; j < n; ++j ) s[j] = j;
    PRand rand;
    for(;;)
    {
        nat j1 = 0;
        nat j2 = n - 1;
        const T r = a[s[rand.number(n)]];
        for ( j = 0; j < n; ++j )
        {
            const nat k = s[j];
            if ( a[k] < r )
            {
                d[j1++] = k;
            }
            else
            if ( a[k] > r )
            {
                d[j2--] = k;
            }
        }
        if ( j1 <= i )
        {
            if ( i <= j2 )
            {
                res = r;
                break;
            }
            ++j2;
            n -= j2;
            s = d + j2;
            i -= j2;
        }
        else
        {
            n = j1;
            s = d;
        }
        d = d == buf1 ? buf2 : buf1;
    }
    return res;
}

//*********************** 30.08.2006 **************************//
//
//          Взвешенные порядковые статистики
//              0 <= p <= 1;   w[j] >= 0
//              buf.size() >= 2 * a.size(),
//              w.size() >= a.size()
//
//*********************** 03.12.2017 **************************//

template <class T1, class T2> 
Def<nat> selectR ( CCArrRef<T1> & a, CCArrRef<T2> & w, double p, ArrRef<nat> & buf )
{
    Def<nat> res;
    const nat n = a.size();
    if ( n == 0 || w.size() < n || buf.size() < n + n || p < 0 || p > 1 ) return res;
    nat * buf1 = buf(0);
    nat * buf2 = buf(n);
    nat * s = buf1;
    nat * d = buf2;
    T2 sum = 0;
    nat j, m = 0;
    for ( j = 0; j < n; ++j )
    {
        if ( w[j] > 0 )
        {
            sum += w[j];
            s[m] = j;
            ++m;
        }
        else
        if ( w[j] < 0 )
        {
            return res;
        }
    }
    if ( m == 0 ) return res;
    p *= sum;
    PRand rand;
    for(;;)
    {
        if ( m == 1 )
        {
            res = s[0];
            break;
        }
        nat j1 = 0;
        nat j2 = m - 1;
        nat k = s[rand.number(m)];
        const T1 r = a[k];
        T2 sum1 = 0;
        T2 sum2 = 0;
        for ( j = 0; j < m; ++j )
        {
            const nat i = s[j];
            const T1 t = a[i];
            if ( t < r )
            {
                sum1 += w[i];
                d[j1++] = i;
            }
            else
            if ( t > r )
            {
                sum2 += w[i];
                d[j2--] = i;
            }
        }
        if ( sum1 <= p )
        {
            p -= sum - sum2;
            if ( p <= 0 )
            {
                res = k;
                break;
            }
            ++j2;
            m -= j2;
            s = d + j2;
            sum = sum2;
        }
        else
        {
            m = j1;
            s = d;
            sum = sum1;
        }
        d = d == buf1 ? buf2 : buf1;
    }
    return res;
}

template <class T1, class T2> inline
Def<nat> selectR ( CCArrRef<T1> & a, CCArrRef<T2> & w, double p )
{
    return selectR ( a, w, p, CmbArray<nat, 80> ( 2*a.size() ) );
}


