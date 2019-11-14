
#pragma once

#include "sorting.h"
#include "ShevArray.h"

//************************ Алгоритмы **************************//
//
//      _median3
//      Сравнения: 3 в худшем случае, 2.667 в среднем
//
//      _median4
//      Сравнения: 4 в худшем случае, 4 в среднем
//
//      _median5a
//      Сравнения: 6 в худшем случае, 6 в среднем
//
//      _median5b
//      Сравнения: 7 в худшем случае, 5.867 в среднем
//
//      _median6
//      Сравнения: 9 в худшем случае, 8.333 в среднем
//
//      _median7
//      Сравнения: 10 в худшем случае, 9.549 в среднем
//
//      _median8
//      Сравнения: 14 в худшем случае, 12.819 в среднем
//
//      _median9a
//      Сравнения: 14 в худшем случае, 13.711 в среднем
//
//      _median9b
//      Сравнения: 16 в худшем случае, 13.524 в среднем
//
//      _median10
//      Сравнения: 18 в худшем случае, 17.867 в среднем
//
//      _median11
//      Сравнения: 20 в худшем случае, 18.997 в среднем
//
//      _median13
//      Сравнения: 24 в худшем случае, 24 в среднем
//
//*************************************************************//

//*********************** 29.11.2014 **************************//
//
//      Медиана трёх чисел.
//      Сравнения: 3 в худшем случае, 2.667 в среднем
//
//*********************** 29.11.2014 **************************//

template <class T> nat __median3 ( const T * a )
{
    if ( a[0] <= a[1] )
    {
        if ( a[1] <= a[2] ) return 1;
        return a[0] >= a[2] ? 0 : 2;
    }
    if ( a[1] >= a[2] ) return 1;
    return a[0] <= a[2] ? 0 : 2;
}

template <class T> inline T _median3 ( const T * a )
{
    return a[__median3(a)];
}

//*********************** 03.12.2014 **************************//
//
//      Медиана четырёх чисел ( 4 сравнения ).
//
//*********************** 03.12.2014 **************************//

template <class T> T _median4 ( const T * a )
{
    nat i1, i2, i3, i4;
    if ( a[0] < a[1] )
    {
        i1 = 0;
        i2 = 1;
    }
    else
    {
        i1 = 1;
        i2 = 0;
    }
    if ( a[2] < a[3] )
    {
        i3 = 2;
        i4 = 3;
    }
    else
    {
        i3 = 3;
        i4 = 2;
    }
    return ( _max ( a[i1], a[i3] ) + _min ( a[i2], a[i4] ) ) / 2;
}

//*********************** 30.09.2004 **************************//
//
//      Медиана пяти чисел ( 6 сравненией ).
//
//*********************** 19.11.2011 **************************//

template <class T> nat __median5a ( const T * a )
{
    nat i1, i2, i3, i4;
    if ( a[0] <= a[1] )
    {
        i1 = 0;
        i2 = 1;
    }
    else
    {
        i1 = 1;
        i2 = 0;
    }
    if ( a[2] <= a[3] )
    {
        i3 = 2;
        i4 = 3;
    }
    else
    {
        i3 = 3;
        i4 = 2;
    }
    if ( a[i2] <= a[i4] )
    {
        if ( a[i3] <= a[4] ) i4 = 4;
        else
        {
            i4 = i3;
            i3 = 4;
        }
    }
    else
    {
        if ( a[i1] <= a[4] ) i2 = 4;
        else
        {
            i2 = i1;
            i1 = 4;
        }
    }
    if ( a[i2] > a[i4] ) return a[i1] > a[i4] ? i1 : i4;
    return a[i2] > a[i3] ? i2 : i3;
}

template <class T> inline T _median5a ( const T * a )
{
    return a[__median5a(a)];
}

//*********************** 09.11.2004 **************************//
//
//      Медиана пяти чисел. 
//      Сравнения: 7 в худшем случае, 5.867 в среднем
//
//*********************** 19.11.2011 **************************//

template <class T> nat __median5b ( const T * a )
{
    nat i1, i2, i3;

    if ( a[1] <= a[2] )
    {
        if ( a[2] <= a[3] )
        {
            i1 = 1;
            i2 = 2;
            i3 = 3;
        }
        else
        {
            if ( a[1] <= a[3] )
            {
                i1 = 1;
                i2 = 3;
                i3 = 2;
            }
            else
            {
                i1 = 3;
                i2 = 1;
                i3 = 2;
            }
        }
    }
    else
    {
        if ( a[1] <= a[3] )
        {
            i1 = 2;
            i2 = 1;
            i3 = 3;
        }
        else
        {
            if ( a[2] <= a[3] )
            {
                i1 = 2;
                i2 = 3;
                i3 = 1;
            }
            else
            {
                i1 = 3;
                i2 = 2;
                i3 = 1;
            }
        }
    }

    if ( a[i2] < a[4] )
    {
        if ( a[i2] < a[0] )
        {
            if ( a[4] < a[0] )
                return a[4] > a[i3] ? i3 : 4;
            else
                return a[0] > a[i3] ? i3 : 0;
        }
    }
    else
    {
        if ( a[i2] > a[0] )
        {
            if ( a[4] > a[0] )
                return a[4] < a[i1] ? i1 : 4;
            else
                return a[0] < a[i1] ? i1 : 0;
        }
    }
    return i2;
}

template <class T> inline T _median5b ( const T * a )
{
    return a[__median5b(a)];
}

//*********************** 08.12.2014 **************************//
//
//      Медиана шести чисел. 
//      Сравнения: 9 в худшем случае, 8.333 в среднем
//
//*********************** 21.02.2015 **************************//

template <class T> T _median6 ( const T * a )
{
    SemiRef<const T> a0(a[0]), a1(a[1]), a2(a[2]),
                     a3(a[3]), a4(a[4]), a5(a[5]);
// Упорядочивание первой тройки
    sort123 ( a0, a1, a2 );
// Упорядочивание второй тройки
    sort123 ( a3, a4, a5 );
// Отбрасывание крайних чисел и вычисление медианы
    if ( a1 > a4 )
        return ( _max ( a0, a4 ) + _min ( a1, a5 ) ) / 2;
    else
        return ( _max ( a1, a3 ) + _min ( a2, a4 ) ) / 2;
}

//*********************** 06.11.2004 **************************//
//
//      Медиана семи чисел.
//      Сравнения: 10 в худшем случае, 9.549 в среднем
//
//*********************** 19.11.2011 **************************//

template <class T> nat __median7 ( const T * a )
{
    nat i1, i2, i3, i4, i5, i6;

    if ( a[0] <= a[1] )
    {
        i1 = 0;
        i2 = 1;
    }
    else
    {
        i1 = 1;
        i2 = 0;
    }

    if ( a[2] <= a[3] )
    {
        i3 = 2;
        i4 = 3;
    }
    else
    {
        i3 = 3;
        i4 = 2;
    }

    if ( a[4] <= a[5] )
    {
        i5 = 4;
        i6 = 5;
    }
    else
    {
        i5 = 5;
        i6 = 4;
    }

    if ( a[i4] > a[i6] )
    {
        _swap ( i3, i5 );
        _swap ( i4, i6 );
    }

    if ( a[i2] <= a[i4] )
    {
        if ( a[i5] <= a[6] )
        {
            i6 = 6;
        }
        else
        {
            i6 = i5;
            i5 = 6;
        }
        if ( a[i6] <= a[i2] )
        {
            if ( a[i6] >= a[i3] )
            {
                return a[i6] > a[i1] ? i6 : i1;
            }
            if ( a[i2] <= a[i3] ) return i2;
            return a[i1] > a[i3] ? i1 : i3;
        }
        if ( a[i5] <= a[i3] )
        {
            if ( a[i2] >= a[i3] ) return i2;
            return a[i6] < a[i3] ? i6 : i3;
        }
        else
        {
            if ( a[i2] >= a[i5] ) return i2;
            return a[i5] < a[i4] ? i5 : i4;
        }
    }

    if ( a[i4] <= a[6] )
    {
        if ( a[i1] <= a[i5] )
        {
            if ( a[i5] <= a[6] )
            {
                if ( a[i5] <= a[i4] ) return i4;
                return a[i5] < a[i2] ? i5 : i2;
            }
            if ( a[6] <= a[i1] ) return i1;
            return a[6] < a[i2] ? 6 : i2;
        }
        if ( a[i1] <= a[6] )
        {
            if ( a[i1] <= a[i4] ) return i4;
            return a[i6] < a[i1] ? i6 : i1;
        }
        if ( a[6] <= a[i5] ) return i5;
        return a[6] < a[i6] ? 6 : i6;
    }

    if ( a[i5] < a[i1] ) _swap ( i1, i5 );
    if ( a[i4] <= a[i5] ) return a[i1] > a[i4] ? i1 : i4;
    if ( a[i3] >= a[i5] ) return a[i3] > a[6] ? i3 : 6;
    return a[i5] > a[6] ? i5 : 6;
}

template <class T> inline T _median7 ( const T * a )
{
    return a[__median7(a)];
}

//*********************** 12.02.2015 **************************//
//
//      Медиана восьми чисел.
//      Сравнения: 14 в худшем случае, 12.819 в среднем
//
//*********************** 21.02.2015 **************************//

template <class T> T _median8 ( const T * a )
{
    SemiRef<const T> a0(a[0]), a1(a[1]), a2(a[2]), a3(a[3]),
                     a4(a[4]), a5(a[5]), a6(a[6]), a7(a[7]);
// Упорядочивание первой четвёрки
    sort123 ( a0, a1, a2, a3 );
// Упорядочивание второй четвёрки
    sort123 ( a4, a5, a6, a7 );
// Отбрасывание крайних чисел и вычисление медианы
    if ( a2 < a6 )
    {
        if ( a2 < a5 )
            return ( _max ( a2, a4 ) + _min ( a3, a5 ) ) / 2;
        else
            return ( _max ( a1, a5 ) + a2 ) / 2;
    }
    else
    {
        if ( a1 < a6 )
            return ( _max ( a1, a5 ) + a6 ) / 2;
        else
            return ( _max ( a0, a6 ) + _min ( a1, a7 ) ) / 2;
    }
}

//*********************** 11.12.2004 **************************//
//
//      Медиана девяти чисел.
//      Сравнения: 14 в худшем случае, 13.711 в среднем
//
//*********************** 19.11.2011 **************************//

template <class T> nat __median9a ( const T * a )
{
    nat i1, i2, i3, i4, i5, i6, i7, i8;

    if ( a[0] <= a[1] )
    {
        i1 = 0;
        i2 = 1;
    }
    else
    {
        i1 = 1;
        i2 = 0;
    }

    if ( a[2] <= a[3] )
    {
        i3 = 2;
        i4 = 3;
    }
    else
    {
        i3 = 3;
        i4 = 2;
    }

    if ( a[i2] > a[i4] )
    {
        _swap ( i1, i3 );
        _swap ( i2, i4 );
    }

    if ( a[4] <= a[5] )
    {
        i5 = 4;
        i6 = 5;
    }
    else
    {
        i5 = 5;
        i6 = 4;
    }

    if ( a[6] <= a[7] )
    {
        i7 = 6;
        i8 = 7;
    }
    else
    {
        i7 = 7;
        i8 = 6;
    }

    if ( a[i6] > a[i8] )
    {
        _swap ( i5, i7 );
        _swap ( i6, i8 );
    }

    if ( a[i2] > a[i6] )
    {
        _swap ( i1, i5 );
        _swap ( i2, i6 );
        _swap ( i3, i7 );
        _swap ( i4, i8 );
    }

    if ( a[8] < a[i7] )
    {
        i8 = i7;
        i7 = 8;
    }
    else
    {
        i8 = 8;
    }

    if ( a[i2] > a[i7] )
    {
        if ( a[i2] <= a[i8] )
        {
            if ( a[i3] <= a[i5] )
            {
                if ( a[i5] <= a[i8] )
                {
                    if ( a[i5] <= a[i2] ) return i2;
                    return a[i5] < a[i4] ? i5 : i4;
                }
                if ( a[i8] <= a[i3] ) return i3;
                return a[i8] < a[i4] ? i8 : i4;
            }
            if ( a[i3] <= a[i8] )
            {
                if ( a[i3] <= a[i2] ) return i2;
                return a[i6] < a[i3] ? i6 : i3;
            }
            if ( a[i8] <= a[i5] ) return i5;
            return a[i8] < a[i6] ? i8 : i6;
        }
        if ( a[i5] < a[i3] ) _swap ( i3, i5 );
        if ( a[i2] <= a[i5] )
        {
            return a[i3] > a[i2] ? i3 : i2;
        }
        if ( a[i1] >= a[i5] )
        {
            return a[i1] > a[i8] ? i1 : i8;
        }
        return a[i5] > a[i8] ? i5 : i8;
    }
    if ( a[i4] <= a[i6] )
    {
        if ( a[i4] <= a[i8] )
        {
            if ( a[i5] > a[i7] ) _swap ( i5, i7 );
            if ( a[i4] <= a[i7] )
            {
                return a[i5] > a[i4] ? i5 : i4;
            }
            return a[i3] > a[i7] ? i3 : i7;
        }
        if ( a[i5] > a[i3] ) _swap ( i3, i5 );
        if ( a[i3] <= a[i8] )
        {
            return a[i7] > a[i3] ? i7 : i3;
        }
        return a[i5] > a[i8] ? i5 : i8;
    }
    if ( a[i6] <= a[i8] )
    {
        if ( a[i3] > a[i7] ) _swap ( i3, i7 );
        if ( a[i6] <= a[i7] )
        {
            return a[i3] > a[i6] ? i3 : i6;
        }
        return a[i5] > a[i7] ? i5 : i7;
    }
    if ( a[i3] > a[i5] ) _swap ( i3, i5 );
    if ( a[i5] <= a[i8] )
    {
        return a[i5] > a[i7] ? i5 : i7;
    }
    return a[i3] > a[i8] ? i3 : i8;
}

template <class T> inline T _median9a ( const T * a )
{
    return a[__median9a(a)];
}

//*********************** 17.10.2005 **************************//
//
//      Медиана девяти чисел.
//      Сравнения: 16 в худшем случае, 13.524 в среднем
//
//*********************** 19.11.2011 **************************//

template <class T> nat __median9b ( const T * a )
{
    nat i0, i1, i2; // первая тройка

    if ( a[0] <= a[1] )
    {
        if ( a[1] <= a[2] )
        {
            i0 = 0;
            i1 = 1;
            i2 = 2;
        }
        else
        {
            if ( a[0] <= a[2] )
            {
                i0 = 0;
                i1 = 2;
                i2 = 1;
            }
            else
            {
                i0 = 2;
                i1 = 0;
                i2 = 1;
            }
        }
    }
    else
    {
        if ( a[0] <= a[2] )
        {
            i0 = 1;
            i1 = 0;
            i2 = 2;
        }
        else
        {
            if ( a[1] <= a[2] )
            {
                i0 = 1;
                i1 = 2;
                i2 = 0;
            }
            else
            {
                i0 = 2;
                i1 = 1;
                i2 = 0;
            }
        }
    }

    nat i3, i4, i5; // вторая тройка

    if ( a[3] <= a[4] )
    {
        if ( a[4] <= a[5] )
        {
            i3 = 3;
            i4 = 4;
            i5 = 5;
        }
        else
        {
            if ( a[3] <= a[5] )
            {
                i3 = 3;
                i4 = 5;
                i5 = 4;
            }
            else
            {
                i3 = 5;
                i4 = 3;
                i5 = 4;
            }
        }
    }
    else
    {
        if ( a[3] <= a[5] )
        {
            i3 = 4;
            i4 = 3;
            i5 = 5;
        }
        else
        {
            if ( a[4] <= a[5] )
            {
                i3 = 4;
                i4 = 5;
                i5 = 3;
            }
            else
            {
                i3 = 5;
                i4 = 4;
                i5 = 3;
            }
        }
    }

    nat i6, i7, i8; // третья тройка

    if ( a[6] <= a[7] )
    {
        if ( a[7] <= a[8] )
        {
            i6 = 6;
            i7 = 7;
            i8 = 8;
        }
        else
        {
            if ( a[6] <= a[8] )
            {
                i6 = 6;
                i7 = 8;
                i8 = 7;
            }
            else
            {
                i6 = 8;
                i7 = 6;
                i8 = 7;
            }
        }
    }
    else
    {
        if ( a[6] <= a[8] )
        {
            i6 = 7;
            i7 = 6;
            i8 = 8;
        }
        else
        {
            if ( a[7] <= a[8] )
            {
                i6 = 7;
                i7 = 8;
                i8 = 6;
            }
            else
            {
                i6 = 8;
                i7 = 7;
                i8 = 6;
            }
        }
    }
    
    nat j0, j1, j2, j3, j4; // пятёрка оставшихся

    if ( a[i1] <= a[i4] )
    {
        if ( a[i4] <= a[i7] )
        {
            j0 = i2;
            j1 = i3;
            j2 = i4;
            j3 = i5;
            j4 = i6;
        }
        else
        {
            if ( a[i1] <= a[i7] )
            {
                j0 = i2;
                j1 = i6;
                j2 = i7;
                j3 = i8;
                j4 = i3;
            }
            else
            {
                j0 = i8;
                j1 = i0;
                j2 = i1;
                j3 = i2;
                j4 = i3;
            }
        }
    }
    else
    {
        if ( a[i1] <= a[i7] )
        {
            j0 = i5;
            j1 = i0;
            j2 = i1;
            j3 = i2;
            j4 = i6;
        }
        else
        {
            if ( a[i4] <= a[i7] )
            {
                j0 = i5;
                j1 = i6;
                j2 = i7;
                j3 = i8;
                j4 = i0;
            }
            else
            {
                j0 = i8;
                j1 = i3;
                j2 = i4;
                j3 = i5;
                j4 = i0;
            }
        }
    }
// медиана из пяти
    if ( a[j2] < a[j4] )
    {
        if ( a[j2] < a[j0] )
        {
            if ( a[j4] < a[j0] )
                return a[j4] > a[j3] ? j3 : j4;
            else
                return a[j0] > a[j3] ? j3 : j0;
        }
    }
    else
    {
        if ( a[j2] > a[j0] )
        {
            if ( a[j4] > a[j0] )
                return a[j4] < a[j1] ? j1 : j4;
            else
                return a[j0] < a[j1] ? j1 : j0;
        }
    }
    return j2;
}

template <class T> inline T _median9b ( const T * a )
{
    return a[__median9b(a)];
}

//*********************** 19.02.2015 **************************//
//
//      Медиана 10-ти чисел
//      Сравнения: 18 в худшем случае, 17.867 в среднем
//
//*********************** 21.02.2015 **************************//

template <class T> T _median10 ( const T * a )
{
    SemiRef<const T> a0(a[0]), a1(a[1]), a2(a[2]), a3(a[3]), a4(a[4]),
                     a5(a[5]), a6(a[6]), a7(a[7]), a8(a[8]), a9(a[9]);
// Упорядочивание первой пятёрки
    sort123 ( a0, a1, a2, a3, a4 );
// Упорядочивание второй пятёрки
    sort123 ( a5, a6, a7, a8, a9 );
// Отбрасывание крайних чисел и вычисление медианы
    if ( a2 < a7 )
    {
        if ( a3 < a6)
            return ( _max ( a3, a5 ) + _min ( a4, a6 ) ) / 2;
        else
            return ( _max ( a2, a6 ) + _min ( a3, a7 ) ) / 2;
    }
    else
    {
        if ( a1 < a8 )
            return ( _max ( a1, a7 ) + _min ( a2, a8 ) ) / 2;
        else
            return ( _max ( a0, a8 ) + _min ( a1, a9 ) ) / 2;
    }
}

//*********************** 19.11.2011 **************************//
//
//      Медиана 11-ти чисел
//      Сравнения: 20 в худшем случае, 18.997 в среднем
//
//*********************** 23.11.2011 **************************//

template <class T> nat __median11 ( const T * a )
{
    nat i, i1, i2, i3, i4, i5, i6, i7;

    if ( a[0] <= a[1] )
    {
        i1 = 0;
        i2 = 1;
    }
    else
    {
        i1 = 1;
        i2 = 0;
    }

    if ( a[2] <= a[3] )
    {
        i3 = 2;
        i4 = 3;
    }
    else
    {
        i3 = 3;
        i4 = 2;
    }

    if ( a[i2] > a[i4] )
    {
        _swap ( i1, i3 );
        _swap ( i2, i4 );
    }

    if ( a[4] <= a[5] )
    {
        i5 = 4;
        i6 = 5;
    }
    else
    {
        i5 = 5;
        i6 = 4;
    }

    i = 5;
    for (;;)
    {
m1:     if ( ++i == 11 )
        {
            return a[i4] > a[i6] ? i4 : i6;
        }
        if ( a[i6] > a[i] )
        {
            i7 = i6;
            i6 = i;
            if ( a[i4] > a[i7] ) goto m3;
            if ( a[i5] > a[i6] ) _swap ( i5, i6 );
        }
        else
        {
            i7 = i;
            if ( a[i4] > a[i7] ) goto m2;
        }
    }
    for (;;)
    {
m2:     if ( ++i == 11 )
        {
            if ( a[i2] > a[i3] )
            {
                return a[i2] > a[i7] ? i2 : i7;
            }
            else
            {
                return a[i3] > a[i7] ? i3 : i7;
            }
        }
        if ( a[i3] > a[i] )
        {
            i4 = i3;
            i3 = i;
        }
        else
        {
            i4 = i;
        }
        if ( a[i2] > a[i4] )
        {
            _swap ( i1, i3 );
            _swap ( i2, i4 );
        }
        if ( a[i4] <= a[i7] ) goto m1;
    }
    for (;;)
    {
m3:     if ( ++i == 11 )
        {
            if ( a[i2] > a[i3] )
            {
                return a[i2] > a[i7] ? i2 : i7;
            }
            else
            {
                return a[i3] > a[i7] ? i3 : i7;
            }
        }
        if ( a[i3] > a[i] )
        {
            i4 = i3;
            i3 = i;
        }
        else
        {
            i4 = i;
        }
        if ( a[i2] > a[i4] )
        {
            _swap ( i1, i3 );
            _swap ( i2, i4 );
        }
        if ( a[i4] <= a[i7] )
        {
            if ( a[i5] > a[i6] ) _swap ( i5, i6 );
            goto m1;
        }
    }
}

template <class T> inline T _median11 ( const T * a )
{
    return a[__median11(a)];
}

//*********************** 02.02.2006 **************************//
//
//          Медиана 13-ти чисел ( 24 сравнения )
//
//*********************** 19.11.2011 **************************//

template <class T> nat __median13 ( const T * a )
{
    nat i1, i2, i3, i4, i5, i6, i7, i8;

    if ( a[0] <= a[1] )
    {
        i1 = 0;
        i2 = 1;
    }
    else
    {
        i1 = 1;
        i2 = 0;
    }

    if ( a[2] <= a[3] )
    {
        i3 = 2;
        i4 = 3;
    }
    else
    {
        i3 = 3;
        i4 = 2;
    }

    if ( a[4] <= a[5] )
    {
        i5 = 4;
        i6 = 5;
    }
    else
    {
        i5 = 5;
        i6 = 4;
    }

    if ( a[6] <= a[7] )
    {
        i7 = 6;
        i8 = 7;
    }
    else
    {
        i7 = 7;
        i8 = 6;
    }

    if ( a[i2] > a[i4] )
    {
        _swap ( i1, i3 );
        _swap ( i2, i4 );
    }

    if ( a[i6] > a[i8] )
    {
        _swap ( i5, i7 );
        _swap ( i6, i8 );
    }

    for ( nat i = 8; i < 13; ++i )
    {
        if ( a[i4] > a[i8] )
        {
            if ( a[i] < a[i3] )
            {
                i4 = i3;
                i3 = i;
            }
            else
            {
                i4 = i;
            }

            if ( a[i2] > a[i4] )
            {
                _swap ( i1, i3 );
                _swap ( i2, i4 );
            }
        }
        else
        {
            if ( a[i] < a[i7] )
            {
                i8 = i7;
                i7 = i;
            }
            else
            {
                i8 = i;
            }

            if ( a[i6] > a[i8] )
            {
                _swap ( i5, i7 );
                _swap ( i6, i8 );
            }
        }
    }

    if ( a[i4] > a[i8] )
    {
        if ( a[i8] < a[i2] )
            return a[i2] < a[i3] ? i3 : i2;
        else
            return a[i8] < a[i3] ? i3 : i8;
    }
    else
    {
        if ( a[i4] < a[i6] )
            return a[i6] < a[i7] ? i7 : i6;
        else
            return a[i4] < a[i7] ? i7 : i4;
    }
}

template <class T> inline T _median13 ( const T * a )
{
    return a[__median13(a)];
}

//*********************** 17.12.2014 **************************//
//
//      После вызова функции select выполняется условие:
//          если j < i, то a[j] <= a[i],
//          если j > i, то a[j] >= a[i]
//
//*********************** 23.01.2016 **************************//

template <class T> bool select ( ArrRef<T> a, nat i )
{
    nat n = a.size();
    if ( i >= n ) return false;
    T * b = a();
    for(;;)
    {
        if ( !i )
        {
            const nat j = firMin ( CArrRef<T> ( b, n ) );
            if ( j ) _swap ( b[0], b[j] );
            break;
        }
        const nat n1 = n - 1;
        if ( i == n1 )
        {
            const nat j = firMax ( CArrRef<T> ( b, n ) );
            if ( j < n1 ) _swap ( b[n1], b[j] );
            break;
        }
        if ( n < 6 )
        {
            switch ( n )
            {
            case 3: sort3_123 ( b ); break;
            case 4: sort4_123 ( b ); break;
            case 5: sort5_123 ( b ); break;
            case 6: sort6_123 ( b ); break;
            }
            break;
        }
        _swap ( b[0], b[n/2] );
        const T r = b[0];
        nat j1 = 0;
        nat j2 = n1;
        do
        {
            if ( b[j2] <= r )
            {
                do
                {
                    if ( ++j1 == j2 ) goto m1;
                }
                while ( b[j1] <= r );
                _swap ( b[j1], b[j2] );
            }
        }
        while ( --j2 > j1 );
m1:     if ( i <= j1 )
        {
            do
            {
                _swap ( b[0], b[j1] );
                do
                {
                    if ( j1 == i ) return true;
                }
                while ( b[--j1] == r );
            }
            while ( b[0] == r );
            n = j1 + 1;
        }
        else
        {
            i -= ++j1;
            n -= j1;
            b += j1;
        }
    }
    return true;
}

//*********************** 20.12.2014 **************************//
//
//                  Медиана множества чисел
//  После вызова этой функции порядок чисел может быть изменён
//
//*********************** 23.01.2016 **************************//

template <class T> T median ( ArrRef<T> a )
{
    const nat n = a.size();
    // Частные случаи
    switch ( n )
    {
    case  0: return 0;
    case  1: return a[0];
    case  2: return ( a[0] + a[1] ) / 2;
    case  3: return _median3 ( a() );
    case  4: return _median4 ( a() );
    case  5: return _median5b( a() );
    case  6: return _median6 ( a() );
    case  7: return _median7 ( a() );
    case  8: return _median8 ( a() );
    case  9: return _median9b( a() );
    case 10: return _median10( a() );
    case 11: return _median11( a() );
    case 13: return _median13( a() );
    }
    // Общий случай
    nat m = n / 2;
    select ( a, m );
    T d = a[m];
    if ( ! ( n & 1 ) )
    {
        T max = a[--m];
        for (;;)
        {
            if ( max < a[--m] ) max = a[m];
            if ( !m ) break;
        }
        d = ( max + d ) / 2;
    }
    return d;
}
