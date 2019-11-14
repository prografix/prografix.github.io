
#ifndef SORTING_H
#define SORTING_H

//************************ ��������� **************************//
//
//      ���������� ���� ��������� 
//      ���������: 1 � ������ ������, 1 � �������
//
//      ���������� ��� ���������
//      ���������: 3 � ������ ������, 2.667 � �������
//
//      ���������� 4-� ���������
//      ���������: 5 � ������ ������, 4.778 � �������
//
//      ���������� ���� ���������
//      ���������: 7 � ������ ������, 6.933 � �������
//
//      ���������� ����� ���������
//      ���������: 10 � ������ ������, 9.578 � �������
//
//*************************************************************//

#include "template.h"

template<class T> inline 
void _next ( T & i1, T & i2, T & i3 )
{
    const T i ( i3 );
    i3 = i2;
    i2 = i1;
    i1 = i;
}

template<class T> inline 
void _next ( T & i1, T & i2, T & i3, T & i4 )
{
    const T i ( i4 );
    i4 = i3;
    i3 = i2;
    i2 = i1;
    i1 = i;
}

template<class T> inline 
void _next ( T & i1, T & i2, T & i3, T & i4, T & i5 )
{
    const T i ( i5 );
    i5 = i4;
    i4 = i3;
    i3 = i2;
    i2 = i1;
    i1 = i;
}

template<class T> inline 
void _next ( T & i1, T & i2, T & i3, T & i4, T & i5, T & i6 )
{
    const T i ( i6 );
    i6 = i5;
    i5 = i4;
    i4 = i3;
    i3 = i2;
    i2 = i1;
    i1 = i;
}

/////////////////////// ������ ������� //////////////////////////

//*********************** 21.11.2009 **************************//
//
//                 ���������� ���� ��������� 
//      ���������: 1 � ������ ������, 1 � �������
//
//*********************** 21.11.2009 **************************//

template <class T> inline void sort123 ( T & a0, T & a1 )
{
    if ( a0 > a1 ) _swap ( a0, a1 );
}

template <class T> inline void sort321 ( T & a0, T & a1 )
{
    if ( a0 < a1 ) _swap ( a0, a1 );
}


//*********************** 21.11.2009 **************************//
//
//                 ���������� ��� ���������
//      ���������: 3 � ������ ������, 2.667 � �������
//
//*********************** 21.11.2009 **************************//

template <class T> inline void sort123 ( T & a0, T & a1, T & a2 )
{
    sort123 ( a1, a2 );
    if ( a0 > a1 )
    {
        _swap ( a0, a1 );
        sort123 ( a1, a2 );
    }
}

template <class T> inline void sort321 ( T & a0, T & a1, T & a2 )
{
    sort321 ( a1, a2 );
    if ( a0 < a1 )
    {
        _swap ( a0, a1 );
        sort321 ( a1, a2 );
    }
}

//*********************** 21.11.2009 **************************//
//
//                 ���������� 4-� ���������
//      ���������: 5 � ������ ������, 4.778 � �������
//
//*********************** 21.11.2009 **************************//

template <class T> inline void sort123 ( T & a0, T & a1, T & a2, T & a3 )
{
    sort123 ( a0, a3 );
    sort123 ( a1, a2 );
    if ( a2 > a3 )
    {
        _swap ( a2, a3 );
        if ( a1 > a2 ) _swap ( a1, a2 );
        else         sort123 ( a0, a1 );
    }
    else
    if ( a0 > a1 )
    {
        _swap ( a0, a1 );
        sort123 ( a1, a2 );
    }
}

template <class T> inline void sort321 ( T & a0, T & a1, T & a2, T & a3 )
{
    sort321 ( a0, a3 );
    sort321 ( a1, a2 );
    if ( a2 < a3 )
    {
        _swap ( a2, a3 );
        if ( a1 < a2 ) _swap ( a1, a2 );
        else         sort321 ( a0, a1 );
    }
    else
    if ( a0 < a1 )
    {
        _swap ( a0, a1 );
        sort321 ( a1, a2 );
    }
}

//*********************** 21.11.2009 **************************//
//
//                 ���������� ���� ���������
//      ���������: 7 � ������ ������, 6.933 � �������
//
//*********************** 21.11.2009 **************************//

template <class T> inline void sort123 ( T & a0, T & a1, T & a2, T & a3, T & a4 )
{
    sort123 ( a1, a2 );
    sort123 ( a3, a4 );
    if ( a2 > a4 )
    {
        _swap ( a1, a3 );
        _swap ( a2, a4 );
    }
    if ( a0 > a2 )
    {
        if ( a0 > a4 )
        {
            _next ( a3, a0, a4 );
            if ( a0 > a1 )
            {
                _swap ( a0, a1 );
                sort123 ( a1, a2 );
            }
        }
        else
        {
            if ( a2 > a3 )
            {
                _swap ( a0, a3 );
                sort123 ( a0, a1 );
            }
            else
            {
                _next ( a1, a0, a2 );
                sort123 ( a2, a3 );
            }
        }
    }
    else
    {
        sort123 ( a0, a1 );
        if ( a1 > a3 )
        {
            _next ( a1, a2, a3 );
            sort123 ( a0, a1 );
        }
        else sort123 ( a2, a3 );
    }
}

template <class T> inline void sort321 ( T & a0, T & a1, T & a2, T & a3, T & a4 )
{
    sort321 ( a1, a2 );
    sort321 ( a3, a4 );
    if ( a2 < a4 )
    {
        _swap ( a1, a3 );
        _swap ( a2, a4 );
    }
    if ( a0 < a2 )
    {
        if ( a0 < a4 )
        {
            _next ( a3, a0, a4 );
            if ( a0 < a1 )
            {
                _swap ( a0, a1 );
                sort321 ( a1, a2 );
            }
        }
        else
        {
            if ( a2 < a3 )
            {
                _swap ( a0, a3 );
                sort321 ( a0, a1 );
            }
            else
            {
                _next ( a1, a0, a2 );
                sort321 ( a2, a3 );
            }
        }
    }
    else
    {
        sort321 ( a0, a1 );
        if ( a1 < a3 )
        {
            _next ( a1, a2, a3 );
            sort321 ( a0, a1 );
        }
        else sort321 ( a2, a3 );
    }
}

//*********************** 16.12.2005 **************************//
//
//                 ���������� ����� ���������
//      ���������: 10 � ������ ������, 9.578 � �������
//
//*********************** 19.12.2005 **************************//

template <class T> inline void sort123 ( T & a0, T & a1, T & a2, T & a3, T & a4, T & a5 )
{
    sort123 ( a0, a1 );
    sort123 ( a2, a3 );
    if ( a1 > a3 )
    {
        _swap ( a0, a2 );
        _swap ( a1, a3 );
    }
    if ( a1 > a4 )
    {
        if ( a0 > a4 )
        {
            if ( a0 > a2 )
            {
                if ( a4 > a2 )
                {
                    _swap ( a0, a2 );
                    _next ( a3, a4, a1 );
                }
                else _next ( a2, a1, a3, a4, a0 );
            }
            else
            {
                if ( a1 >= a2 ) _next ( a1, a3, a4, a0 );
                else            _next ( a1, a2, a3, a4, a0 );
            }
        }
        else
        {
            if ( a4 > a2 )
            {
                if ( a0 > a2 ) _next ( a1, a3, a4, a2, a0 );
                else           _next ( a3, a4, a2, a1 );
            }
            else
            {
                if ( a1 >= a2 ) _next ( a3, a4, a1 );
                else            _next ( a2, a3, a4, a1 );
            }
        }
    }
    else
    {
        if ( a3 > a4 )
        {
            if ( a1 > a2 )
            {
                _swap ( a3, a4 );
                if ( a0 > a2 ) _next ( a1, a2, a0 );
                else           _swap ( a1, a2 );
            }
            else
            {
                if ( a4 >= a2 ) _swap ( a3, a4 );
                else            _next ( a3, a4, a2 );
            }
        }
        else
        {
            if ( a1 > a5 )
            {
                if ( a0 > a5 )
                {
                    if ( a0 > a2 )
                    {
                        if ( a5 > a2 )
                        {
                            _swap ( a0, a2 );
                            _next ( a3, a4, a5, a1 );
                        }
                        else _next ( a2, a1, a3, a4, a5, a0 );
                    }
                    else
                    {
                        if ( a1 >= a2 ) _next ( a1, a3, a4, a5, a0 );
                        else            _next ( a1, a2, a3, a4, a5, a0 );
                    }
                }
                else
                {
                    if ( a5 > a2 )
                    {
                        if ( a0 > a2 ) _next ( a1, a3, a4, a5, a2, a0 );
                        else           _next ( a3, a4, a5, a2, a1 );
                    }
                    else
                    {
                        if ( a1 >= a2 ) _next ( a3, a4, a5, a1 );
                        else            _next ( a2, a3, a4, a5, a1 );
                    }
                }
            }
            else
            {
                if ( a3 > a5 )
                {
                    if ( a1 > a2 )
                    {
                        if ( a0 > a2 ) _next ( a1, a2, a0 );
                        else           _swap ( a1, a2 );
                        _next ( a5, a3, a4 );
                    }
                    else
                    {
                        if ( a2 > a5 ) _next ( a3, a4, a5, a2 );
                        else           _next ( a4, a5, a3 );
                    }
                }
                else
                {
                    if ( a1 > a2 )
                    {
                        if ( a0 > a2 ) _next ( a1, a2, a0 );
                        else           _swap ( a1, a2 );
                    }
                    sort123 ( a4, a5 );
                }
            }
            return;
        }
    }
// ��������� 6-� �������
    if ( a2 > a5 )
    {
        if ( a1 > a5 )
        {
            if ( a0 > a5 ) _next ( a0, a1, a2, a3, a4, a5 );
            else           _next ( a1, a2, a3, a4, a5 );
        }
        else _next ( a2, a3, a4, a5 );
    }
    else
    {
        if ( a3 > a5 ) _next ( a3, a4, a5 );
        else         sort123 ( a4, a5 );
    }
}

template <class T> inline void sort321 ( T & a0, T & a1, T & a2, T & a3, T & a4, T & a5 )
{
    if ( a0 < a1 ) _swap ( a0, a1 );
    if ( a2 < a3 ) _swap ( a2, a3 );

    if ( a1 < a3 )
    {
        _swap ( a0, a2 );
        _swap ( a1, a3 );
    }
    if ( a1 < a4 )
    {
        if ( a0 < a4 )
        {
            if ( a0 < a2 )
            {
                if ( a4 < a2 )
                {
                    _swap ( a0, a2 );
                    _next ( a3, a4, a1 );
                }
                else _next ( a2, a1, a3, a4, a0 );
            }
            else
            {
                if ( a1 <= a2 ) _next ( a1, a3, a4, a0 );
                else            _next ( a1, a2, a3, a4, a0 );
            }
        }
        else
        {
            if ( a4 < a2 )
            {
                if ( a0 < a2 ) _next ( a1, a3, a4, a2, a0 );
                else           _next ( a3, a4, a2, a1 );
            }
            else
            {
                if ( a1 <= a2 ) _next ( a3, a4, a1 );
                else            _next ( a2, a3, a4, a1 );
            }
        }
    }
    else
    {
        if ( a3 < a4 )
        {
            if ( a1 < a2 )
            {
                _swap ( a3, a4 );
                if ( a0 < a2 ) _next ( a1, a2, a0 );
                else           _swap ( a1, a2 );
            }
            else
            {
                if ( a4 <= a2 ) _swap ( a3, a4 );
                else            _next ( a3, a4, a2 );
            }
        }
        else
        {
            if ( a1 < a5 )
            {
                if ( a0 < a5 )
                {
                    if ( a0 < a2 )
                    {
                        if ( a5 < a2 )
                        {
                            _swap ( a0, a2 );
                            _next ( a3, a4, a5, a1 );
                        }
                        else _next ( a2, a1, a3, a4, a5, a0 );
                    }
                    else
                    {
                        if ( a1 <= a2 ) _next ( a1, a3, a4, a5, a0 );
                        else            _next ( a1, a2, a3, a4, a5, a0 );
                    }
                }
                else
                {
                    if ( a5 < a2 )
                    {
                        if ( a0 < a2 ) _next ( a1, a3, a4, a5, a2, a0 );
                        else           _next ( a3, a4, a5, a2, a1 );
                    }
                    else
                    {
                        if ( a1 <= a2 ) _next ( a3, a4, a5, a1 );
                        else            _next ( a2, a3, a4, a5, a1 );
                    }
                }
            }
            else
            {
                if ( a3 < a5 )
                {
                    if ( a1 < a2 )
                    {
                        if ( a0 < a2 ) _next ( a1, a2, a0 );
                        else _swap ( a1, a2 );
                        _next ( a5, a3, a4 );
                    }
                    else
                    {
                        if ( a2 < a5 ) _next ( a3, a4, a5, a2 );
                        else           _next ( a4, a5, a3 );
                    }
                }
                else
                {
                    if ( a1 < a2 )
                    {
                        if ( a0 < a2 ) _next ( a1, a2, a0 );
                        else _swap ( a1, a2 );
                    }
                    if ( a4 < a5 ) _swap ( a4, a5 );
                }
            }
            return;
        }
    }
// ��������� 6-� �������
    if ( a2 < a5 )
    {
        if ( a1 < a5 )
        {
            if ( a0 < a5 ) _next ( a0, a1, a2, a3, a4, a5 );
            else           _next ( a1, a2, a3, a4, a5 );
        }
        else _next ( a2, a3, a4, a5 );
    }
    else
    {
        if ( a3 < a5 ) _next ( a3, a4, a5 );
        else         sort321 ( a4, a5 );
    }
}

/////////////////////// ������ ������� //////////////////////////

//*********************** 14.12.2005 **************************//
//
//                 ���������� ���� ��������� 
//      ���������: 1 � ������ ������, 1 � �������
//
//*********************** 14.12.2005 **************************//

template <class T> inline void sort2_123 ( T * a )
{
    sort123 ( a[0], a[1] );
}

template <class T> inline void sort2_321 ( T * a )
{
    sort321 ( a[0], a[1] );
}


//*********************** 14.12.2005 **************************//
//
//                 ���������� ��� ���������
//      ���������: 3 � ������ ������, 2.667 � �������
//
//*********************** 14.12.2005 **************************//

template <class T> inline void sort3_123 ( T * a )
{
    sort123 ( a[0], a[1], a[2] );
}

template <class T> inline void sort3_321 ( T * a )
{
    sort321 ( a[0], a[1], a[2] );
}


//*********************** 14.12.2005 **************************//
//
//                 ���������� 4-� ���������
//      ���������: 5 � ������ ������, 4.778 � �������
//
//*********************** 14.12.2005 **************************//

template <class T> inline void sort4_123 ( T * a )
{
    sort123 ( a[0], a[1], a[2], a[3] );
}

template <class T> inline void sort4_321 ( T * a )
{
    sort321 ( a[0], a[1], a[2], a[3] );
}


//*********************** 15.12.2005 **************************//
//
//                 ���������� ���� ���������
//      ���������: 7 � ������ ������, 6.933 � �������
//
//*********************** 15.12.2005 **************************//

template <class T> inline void sort5_123 ( T * a )
{
    sort123 ( a[0], a[1], a[2], a[3], a[4] );
}

template <class T> inline void sort5_321 ( T * a )
{
    sort321 ( a[0], a[1], a[2], a[3], a[4] );
}


//*********************** 16.12.2005 **************************//
//
//                 ���������� ����� ���������
//      ���������: 10 � ������ ������, 9.578 � �������
//
//*********************** 19.12.2005 **************************//

template <class T> inline void sort6_123 ( T * a )
{
    sort123 ( a[0], a[1], a[2], a[3], a[4], a[5] );
}

template <class T> inline void sort6_321 ( T * a )
{
    sort321 ( a[0], a[1], a[2], a[3], a[4], a[5] );
}


#endif
