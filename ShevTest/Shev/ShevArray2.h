
#pragma once

#include "ShevArray.h"

#define CCArrRef2 const CArrRef2

template <class T> class ArrRef2;

/**************************** СArrRef2 *****************************/
/************* Ссылка на константный двухмерный массив *************/

template <class T> class CArrRef2
{
    void operator= ( CCArrRef2 & );
    CArrRef<T> _data;
    nat _size0, _size1;
public:
    CArrRef2 ( const ArrRef2<T> & a ) : _data ( a(), a._size0 * a._size1 ), _size0 ( a._size0 ), _size1 ( a._size1 ) {}
    CArrRef<T> operator[] ( nat i ) const { return CArrRef<T> ( _data, i * _size1, _size1 ); }
    nat size0 () const { return _size0; }
    nat size1 () const { return _size1; }
};

/***************************** ArrRef2 *****************************/
/****************** Ссылка на двухмерный массив ********************/

template <class T> class ArrRef2 : protected ArrRef<T>
{
    void operator= ( const ArrRef2 & );
protected:
    nat _size0, _size1;
    ArrRef2 ( T * d, nat n0, nat n1 ) : ArrRef<T>(d, n0*n1), _size0(n0), _size1(n1) {}
public:
    ArrRef2 ( ArrRef<T> & a, nat i, nat n0, nat n1 ) : ArrRef<T>( a, i, n0*n1 ), _size0(n0), _size1(n1) {}
    ArrRef <T> operator[] ( nat i )       { return  ArrRef<T> ( *this, i * _size1, _size1 ); }
    CArrRef<T> operator[] ( nat i ) const { return CArrRef<T> ( *this, i * _size1, _size1 ); }
    nat size0 () const { return _size0; }
    nat size1 () const { return _size1; }
    ArrRef2 & fill ( const T & p )
    {
        ArrRef<T>::fill ( p );
        return *this;
    }
    CArrRef2<T> operator* () const { return CArrRef2<T> ( *this ); }
friend class CArrRef2<T>;
};

/**************************** FixArray2 ****************************/
/************** Двухмерный массив постоянного размера **************/

template <class T, nat n0, nat n1> class FixArray2 : public ArrRef2<T>
{
    T stor[n0*n1];
    FixArray2 ( const FixArray2 & );
public:
    FixArray2 () : ArrRef2<T> ( stor, n0, n1 ) {}
    FixArray2 & operator= ( const FixArray2 & a )
    {
        for ( nat i = 0; i < _size; ++i ) stor[i] = a.stor[i];
        return *this;
    }
};

/**************************** DynArray2 ****************************/
/************** Двухмерный массив переменного размера **************/

template <class T> class DynArray2 : public ArrRef2<T>
{
    DynArray2 ( const DynArray2 & );
public:
    DynArray2 ( nat n0, nat n1 ) : ArrRef2<T> ( new T[n0*n1], n0, n1 ) {}

    ~DynArray2 () { delete[] _data.var; }

    DynArray2 & operator= ( CCArrRef2<T> & r )
    {
        const T * p = r[0]();
        if ( _data.var == p ) return *this;
        _size0 = r.size0();
        _size1 = r.size1();
        const nat n = _size0 * _size1;
        if ( n != _size )
        {
            delete[] _data.var;
            _data.var = new T[n];
            _size = n;
        }
        for ( nat i = 0; i < n; ++i ) _data.var[i] = p[i];
        return *this;
    }

    DynArray2 & operator= ( const DynArray2 & a )
    {
        return *this = *a;
    }

    DynArray2 & swap ( DynArray2 & a )
    {
        _swap ( _size0, a._size0 );
        _swap ( _size1, a._size1 );
        _swap ( _size, a._size );
        _swap ( _data, a._data );
        return *this;
    }
};

/**************************** CmbArray2 ****************************/
/********************* Комбинированный массив  *********************/

template <class T, nat N> class CmbArray2 : public ArrRef2<T>
{
    T stor[N];
    CmbArray2 ( const CmbArray2 & );
public:
    CmbArray2 ( nat n0, nat n1 ) : ArrRef2<T> ( N < n0*n1 ? new T[n0*n1] : stor, n0, n1 ) {}

    ~CmbArray2 () { if ( _data.var != stor ) delete[] _data.var; }

    CmbArray2 & operator= ( CCArrRef2<T> & r )
    {
        const T * p = r[0]();
        if ( _data.var == p ) return *this;
        _size0 = r.size0();
        _size1 = r.size1();
        const nat n = _size0 * _size1;
        if ( n != _size )
        {
            if ( N < n )
            {
                delete[] _data.var;
                _data.var = new T[n];
            }
            _size = n;
        }
        for ( nat i = 0; i < n; ++i ) _data.var[i] = p[i];
        return *this;
    }

    CmbArray2 & operator= ( const CmbArray2 & a )
    {
        return *this = *a;
    }
};
