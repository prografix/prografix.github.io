
#ifndef SHEVARRAY_H
#define SHEVARRAY_H

#include "typedef.h"
#include "template.h"

//#define ARRAY_TEST
#ifdef ARRAY_TEST
void outOfRange ( const char * name, nat size, nat index );
#endif

/********************** CArrRef ************************/
/************ Ссылка на константный массив *************/

template <class T> class CArrRef
{
    void operator = ( const CArrRef & );
protected:
    const T * _data;
    nat _size;
public:
    CArrRef () : _data(0), _size(0) {}
    CArrRef ( const T * d, nat n ) : _data(d), _size(n) {}
#ifdef ARRAY_TEST
    void error ( nat n, nat i ) const { outOfRange ( "CArrRef", n, i ); }
    CArrRef ( CArrRef<T> a, nat i, nat n ) : _data(&a[i]), _size(n) { if ( a._size < i + n ) error ( a._size, i + n ); }
    const T & operator[] ( nat i ) const { if ( _size <= i ) error ( _size, i ); return _data[i]; }
    const T & las () const { if ( _size == 0 ) error ( _size, _size ); return _data[_size-1]; }
    const T & cprev ( nat i ) const { if ( _size <= i ) error ( _size, i ); return _data[i>0?i-1:_size-1]; }
    const T & cnext ( nat i ) const { if ( _size <= i ) error ( _size, i ); return _data[i+1<_size?i+1:0]; }
#else
    CArrRef ( CArrRef<T> a, nat i, nat n ) : _data(&a[i]), _size(n) {}
    const T & operator[] ( nat i ) const { return _data[i]; }
    const T & las () const { return _data[_size-1]; }
    const T & cprev ( nat i ) const { return _data[i>0?i-1:_size-1]; }
    const T & cnext ( nat i ) const { return _data[i+1<_size?i+1:0]; }
#endif
    nat size () const { return _size; }
};

/*********************** ArrRef ************************/
/****************** Ссылка на массив *******************/

template <class T> class ArrRef : public CArrRef<T>
{
protected:
    ArrRef ( T * d, nat n ) : CArrRef<T>( d, n ) {}
public:
    ArrRef () {}
#ifdef ARRAY_TEST
    void error ( nat n, nat i ) const { outOfRange ( "ArrRef", n, i ); }
    ArrRef ( ArrRef<T> a, nat i, nat n ) : CArrRef<T>( &a[i], n ) { if ( a._size < i + n ) error ( a._size, i + n ); }
    T & operator[] ( nat i ) { if ( _size <= i ) error ( _size, i );return ((T*)_data)[i]; }
    T & las () { if ( _size == 0 ) error ( _size, _size ); return ((T*)_data)[_size-1]; }
    T & cprev ( nat i ) { if ( _size <= i ) error ( _size, i ); return ((T*)_data)[i>0?i-1:_size-1]; }
    T & cnext ( nat i ) { if ( _size <= i ) error ( _size, i ); return ((T*)_data)[i+1<_size?i+1:0]; }
    const T & operator[] ( nat i ) const { if ( _size <= i ) error ( _size, i ); return _data[i]; }
    const T & las () const { if ( _size == 0 ) error ( _size, _size ); return _data[_size-1]; }
    const T & cprev ( nat i ) const { if ( _size <= i ) error ( _size, i ); return _data[i>0?i-1:_size-1]; }
    const T & cnext ( nat i ) const { if ( _size <= i ) error ( _size, i ); return _data[i+1<_size?i+1:0]; }
#else
    ArrRef ( ArrRef<T> a, nat i, nat n ) : CArrRef<T>( &a[i], n ) {}
    T & operator[] ( nat i ) { return ((T*)_data)[i]; }
    T & las () { return ((T*)_data)[_size-1]; }
    T & cprev ( nat i ) { return ((T*)_data)[i>0?i-1:_size-1]; }
    T & cnext ( nat i ) { return ((T*)_data)[i+1<_size?i+1:0]; }
    const T & operator[] ( nat i ) const { return _data[i]; }
    const T & las () const { return _data[_size-1]; }
    const T & cprev ( nat i ) const { return _data[i>0?i-1:_size-1]; }
    const T & cnext ( nat i ) const { return _data[i+1<_size?i+1:0]; }
#endif
    ArrRef & operator= ( const CArrRef<T> & p )
    {
        const nat n = _min ( _size, p.size() );
        for ( nat i = 0; i < n; ++i ) ((T*)_data)[i] = p[i];
        return *this;
    }
    ArrRef & operator= ( const ArrRef & a )
    {
        return this->operator= ( ( const CArrRef<T> &) a );
    }
// Изменение порядка следования элементов на обратный
    ArrRef & reverse ()
    {
        if ( _size < 2 ) return *this;
        const nat n = _size - 1;
        const nat m = _size / 2;
        for ( nat i = 0; i < m; ++i ) _swap ( ((T*)_data)[i], ((T*)_data)[n-i] );
        return *this;
    }
// Применение указанной процедуры для элементов с первого до последнего
    template <class P> 
    ArrRef & for012p ( P & p )
    {
        for ( nat i = 0; i < _size; ++i ) p ( ((T*)_data)[i] );
        return *this;
    }
};

/*********************** FixArrRef ***********************/
/*********** Ссылка на массив постоянной длины ***********/

template <class T, nat n> class FixArrRef : public ArrRef<T>
{
protected:
    FixArrRef ( T * d ) : ArrRef<T>( d, n ) {}
public:
explicit FixArrRef ( ArrRef<T> a, nat i = 0 ) : ArrRef<T> ( a, i, n ) {}
    FixArrRef & operator= ( const FixArrRef & a )
    {
        for ( nat i = 0; i < n; ++i ) stor[i] = a[i];
        return *this;
    }
    FixArrRef & operator+= ( const FixArrRef & a )
    {
        for ( nat i = 0; i < n; ++i ) ((T*)_data)[i] += a[i];
        return *this;
    }
    FixArrRef & operator-= ( const FixArrRef & a )
    {
        for ( nat i = 0; i < n; ++i ) ((T*)_data)[i] -= a[i];
        return *this;
    }
    FixArrRef & operator*= ( const FixArrRef & a )
    {
        for ( nat i = 0; i < n; ++i ) ((T*)_data)[i] *= a[i];
        return *this;
    }
    FixArrRef & operator/= ( const FixArrRef & a )
    {
        for ( nat i = 0; i < n; ++i ) ((T*)_data)[i] /= a[i];
        return *this;
    }
    FixArrRef & operator%= ( const FixArrRef & a )
    {
        for ( nat i = 0; i < n; ++i ) ((T*)_data)[i] %= a[i];
        return *this;
    }

    friend inline void _swap ( FixArrRef & a1, FixArrRef & a2 )
    {
        for ( nat i = 0; i < n; ++i ) _swap ( a1[i], a2[i] );
    }
};

/*********************** FixArray ************************/
/*************** Массив постоянной длины *****************/

template <class T, nat n> class FixArray : public FixArrRef<T, n>
{
    T stor[n];
    FixArray ( const FixArray & );
public:
    FixArray () : FixArrRef<T, n> ( stor ) {}
    FixArray & operator= ( const FixArray & a )
    {
        for ( nat i = 0; i < n; ++i ) stor[i] = a.stor[i];
        return *this;
    }
};

/********************** DynArrRef **********************/
/******* Ссылка на массив с изменяемым размером ********/

template <class T> class DynArrRef : public ArrRef<T>
{
protected:
    DynArrRef ( T * d, nat n ) : ArrRef<T>( d, n ) {}
public:
    virtual DynArrRef & resize ( nat n = 0 ) = 0;
};

/********************** DynArray ***********************/
/**************** Динамический массив ******************/

template <class T> class DynArray : public DynArrRef<T>
{
    DynArray ( const DynArray & );
public:
explicit DynArray ( nat n = 0 ) : DynArrRef<T> ( n > 0 ? new T[n] : 0, n ) {}
explicit DynArray ( const CArrRef<T> & r ) : DynArrRef<T> ( new T[r.size()], r.size() )
     {
        for ( nat i = 0; i < _size; ++i ) ((T*)_data)[i] = r[i];
     }

    ~DynArray () { delete[] (T*)_data; }

    DynArray & operator= ( const CArrRef<T> & r )
    {
        if ( _size != r.size() )
        {
            delete[] (T*)_data;
            _data = ( _size = r.size() ) > 0 ? new T[_size] : 0;
        }
        for ( nat i = 0; i < _size; ++i ) ((T*)_data)[i] = r[i];
        return *this;
    }

    DynArray & operator= ( const DynArray & a )
    {
        return this->operator= ( ( const CArrRef<T> &) a );
    }

    DynArrRef<T> & resize ( nat n = 0 )
    {
        delete[] (T*)_data;
        _data = ( _size = n ) > 0 ? new T[n] : 0;
        return *this;
    }

    friend inline void _swap ( DynArray & a1, DynArray & a2 )
    {
        _swap ( a1._size, a2._size );
        _swap ( a1._data, a2._data );
    }
};

/********************** CmbArray ***********************/
/************** Комбинированный массив *****************/

template <class T, nat N> class CmbArray : public DynArrRef<T>
{
    T stor[N];
    CmbArray ( const CmbArray & );
public:
explicit CmbArray ( nat n = 0 ) : DynArrRef<T> ( n > N ? new T[n] : stor, n ) {}
explicit CmbArray ( const CArrRef<T> & r ) : DynArrRef<T> ( r.size() > N ? new T[r.size()] : stor, r.size() )
     {
        for ( nat i = 0; i < _size; ++i ) ((T*)_data)[i] = r[i];
     }

    ~CmbArray () { if ( _data != stor ) delete[] (T*)_data; }

    CmbArray & operator= ( const CArrRef<T> & r )
    {
        if ( _size != r.size() )
        {
            if ( _data != stor ) delete[] (T*)_data;
            _data = ( _size = r.size() ) > N ? new T[_size] : stor;
        }
        for ( nat i = 0; i < _size; ++i ) ((T*)_data)[i] = r[i];
        return *this;
    }

    CmbArray & operator= ( const CmbArray & a )
    {
        return this->operator= ( ( const CArrRef<T> &) a );
    }

    DynArrRef<T> & resize ( nat n = 0 )
    {
        if ( _size == n ) return *this;
        if ( _data != stor ) delete[] (T*)_data;
        _data = ( _size = n ) > N ? new T[n] : stor;
        return *this;
    }
};

/********************** SuiteRef ***********************/
/***** Ссылка на последовательный набор элементов ******/

template <class T> class SuiteRef : public DynArrRef<T>
{
    SuiteRef ( const SuiteRef & );

    void _del ( nat i )
    {
        if ( i < --_size ) ((T*)_data)[i] = _data[_size];
    }

    virtual void resizeAndCopy ( nat n )
    {
        T * tmp = new T[n];
        for ( nat i = 0; i < _size; ++i ) tmp[i] = _data[i];
        delete[] (T*)_data;
        _data = tmp;
        real_size = n;
    }
protected:
    nat real_size;
    SuiteRef ( T * d, nat n ) : DynArrRef<T>( d, n ) {}
public:
    SuiteRef & operator= ( const CArrRef<T> & r )
    {
        resize ( r.size() );
        for ( nat i = 0; i < _size; ++i ) ((T*)_data)[i] = r[i];
        return *this;
    }

    SuiteRef & operator= ( const SuiteRef & a )
    {
        return this->operator= ( ( const CArrRef<T> &) a );
    }

    DynArrRef<T> & resize ( nat n = 0 )
    {
        if ( n > real_size ) resizeAndCopy ( n );
        _size = n;
        return *this;
    }

    virtual T & inc ()
    {
        if ( _size == real_size ) resizeAndCopy ( _size + _size );
        return ((T*)_data)[_size++];
    }

    SuiteRef & dec ()
    {
        if ( _size > 0 ) --_size;
        return *this;
    }

    SuiteRef & del ( nat i )
    {
        if ( i < _size ) _del ( i );
#ifdef ARRAY_TEST
        else outOfRange ( "SuiteRef::del", _size, i );
#endif ARRAY_TEST
        return *this;
    }

    void movAftLas ( nat i, SuiteRef<T> & suite )
    {
        if ( i < _size )
        {
            suite.inc() = _data[i];
            _del ( i );
        }
#ifdef ARRAY_TEST
        else outOfRange ( "SuiteRef::movAftLas", _size, i );
#endif ARRAY_TEST
    }

    virtual SuiteRef & addAftLas ( const CArrRef<T> & a )
    {
        const nat n = _size + a.size();
        nat s = real_size;
        if ( s < n )
        {
            while ( s < n ) s += s;
            resizeAndCopy ( s );
        }
        for ( nat i = 0; i < a.size(); ++i ) ((T*)_data)[_size+i] = a[i];
        _size = n;
        return *this;
    }
};

/*********************** Suite *************************/
/********** Последовательный набор элементов ***********/

template <class T> class Suite : public SuiteRef<T>
{
    Suite ( const Suite & );
public:
    Suite () : SuiteRef<T>(new T[real_size=16], 0) {}
    explicit Suite ( nat n, nat m = 0 ) : SuiteRef<T>(new T[real_size=n<m?m:n==0?16:n], m) {}
    ~Suite () { delete[] (T*)_data; }

    Suite & operator= ( const CArrRef<T> & r )
    {
        _size = r.size();
        if ( real_size < _size )
        {
            real_size = _size;
            delete[] (T*)_data;
            _data = new T[_size];
        }
        for ( nat i = 0; i < _size; ++i ) ((T*)_data)[i] = r[i];
        return *this;
    }

    Suite & operator= ( const Suite & a )
    {
        return this->operator= ( ( const CArrRef<T> &) a );
    }

    friend inline void _swap ( Suite & a1, Suite & a2 )
    {
        _swap ( a1.real_size, a2.real_size );
        _swap ( a1._size, a2._size );
        _swap ( a1._data, a2._data );
    }
};

/********************* CmbSuite ************************/
/********** Комбинированный набор элементов ************/

template <class T, nat N> class CmbSuite : public SuiteRef<T>
{
    T stor[N];
    CmbSuite ( const CmbSuite & );

    virtual void resizeAndCopy ( nat n )
    {
        T * tmp = new T[n];
        for ( nat i = 0; i < _size; ++i ) tmp[i] = _data[i];
        if ( _data != stor ) delete[] (T*)_data;
        _data = tmp;
        real_size = n;
    }
public:
    CmbSuite () : SuiteRef<T>(stor, 0) { real_size = N; }
    explicit CmbSuite ( nat n, nat m = 0 ) : 
        SuiteRef<T> ( ( real_size = n < m ? m : n ) > N ? new T[real_size] : ( real_size = N, stor ), m ) {}
    ~CmbSuite () { if ( _data != stor ) delete[] (T*)_data; }

    CmbSuite & operator= ( const CArrRef<T> & r )
    {
        _size = r.size();
        if ( real_size < _size )
        {
            real_size = _size;
            if ( _data != stor ) delete[] (T*)_data;
            _data = new T[_size];
        }
        for ( nat i = 0; i < _size; ++i ) ((T*)_data)[i] = r[i];
        return *this;
    }

    CmbSuite & operator= ( const CmbSuite & a )
    {
        return this->operator= ( ( const CArrRef<T> &) a );
    }
};

/********************* LtdSuiteRef *********************/
/*** Ссылка на набор элементов ограниченного размера ***/

template <class T> class LtdSuiteRef : public SuiteRef<T>
{
public:
    LtdSuiteRef ( ArrRef<T> a, nat i, nat n ) : SuiteRef<T>( &a[i], 0 )
    {
        real_size = n;
#ifdef ARRAY_TEST
        if ( a.size() < i + n ) outOfRange ( "LtdSuiteRef", a.size(), i + n );
#endif
    }

    LtdSuiteRef & operator= ( const CArrRef<T> & r )
    {
        _size = r.size();
#ifdef ARRAY_TEST
        if ( _size > real_size ) outOfRange ( "LtdSuiteRef::operator=", real_size, _size );
#endif
        if ( _size > real_size ) _size = real_size;
        for ( nat i = 0; i < _size; ++i ) ((T*)_data)[i] = r[i];
        return *this;
    }

    LtdSuiteRef & operator= ( const LtdSuiteRef & a )
    {
        return this->operator= ( ( const CArrRef<T> &) a );
    }

    DynArrRef<T> & resize ( nat n = 0 )
    {
#ifdef ARRAY_TEST
        if ( n > real_size ) outOfRange ( "LtdSuiteRef::resize", real_size, n );
#endif
        _size = n < real_size ? n : real_size;
        return *this;
    }

    T & inc ()
    {
        if ( _size < real_size ) ++_size;
#ifdef ARRAY_TEST
        else outOfRange ( "LtdSuiteRef::inc", real_size, _size );
#endif
        return ((T*)_data)[_size-1];
    }

    SuiteRef<T> & addAftLas ( const CArrRef<T> & a )
    {
        nat n = _size + a.size();
        if ( n > real_size ) n = real_size;
        for ( nat i = _size; i < n; ++i ) ((T*)_data)[i] = a[i-_size];
        _size = n;
        return *this;
    }
};

/**************** Операции с массивами без изменений *****************/

template <class T> inline 
bool operator == ( const CArrRef<T> & a, const CArrRef<T> & b )
{
    if ( a.size() != b.size() ) return false;
    for ( nat i = 0; i < a.size(); ++i ) 
        if ( a[i] != b[i] ) return false;
    return true;
}

template <class T> inline 
bool operator != ( const CArrRef<T> & a, const CArrRef<T> & b )
{
    if ( a.size() != b.size() ) return true;
    for ( nat i = 0; i < a.size(); ++i ) 
        if ( a[i] != b[i] ) return true;
    return false;
}

template <class T> inline 
nat count ( const CArrRef<T> & a, const T & b )
{
    nat n = 0;
    for ( nat i = 0; i < a.size(); ++i ) if ( a[i] == b ) ++n;
    return n;
}

template <class T> inline 
nat firEqu ( const CArrRef<T> & a, const T & b )
{
    for ( nat i = 0; i < a.size(); ++i ) if ( a[i] == b ) return i;
    return a.size();
}

template <class T> inline 
nat lasEqu ( const CArrRef<T> & a, const T & b )
{
    for ( nat i = a.size(); i > 0; --i ) if ( a[i-1] == b ) return i-1;
    return a.size();
}

template <class T> inline 
nat firMin ( const CArrRef<T> & a )
{
    nat m = 0;
    for ( nat i = 1; i < a.size(); ++i ) if ( a[i] < a[m] ) m = i;
    return m;
}

template <class T> inline 
nat firMax ( const CArrRef<T> & a )
{
    nat m = 0;
    for ( nat i = 1; i < a.size(); ++i ) if ( a[i] > a[m] ) m = i;
    return m;
}

template <class T> inline 
Def<T> amean ( const CArrRef<T> & a )
{
    if ( a.size() == 0 ) return Def<T>();
    T res ( a[0] );
    for ( nat i = 1; i < a.size(); ++i ) res += a[i];
    return res / a.size();
}

/*********************** CFor012 ************************/

template <class T> class CFor012
{
public:
    const CArrRef<T> & ref;
    explicit CFor012 ( const CArrRef<T> & a ) : ref ( a ) {}
};

template <class T> inline 
CFor012<T> cfor012 ( const CArrRef<T> & a )
{
    return CFor012<T> ( a );
}

template <class T1, class T2> inline 
T1 & operator += ( T1 & t, const CFor012<T2> & a )
{
    for ( nat i = 0; i < a.ref.size(); ++i ) t += a.ref[i];
    return t;
}

template <class T1, class T2> inline 
T1 & operator << ( T1 & t, const CFor012<T2> & a )
{
    for ( nat i = 0; i < a.ref.size(); ++i ) t << a.ref[i];
    return t;
}

/********* Операции с упорядоченными массивами **********/

template <class T> inline 
nat lasEqu123 ( CArrRef<T> a, const T & b )
{
    if ( a.size() == 0 ) return 0;
    nat from = 0, before = a.size();
    while ( from + 1 < before )
    {
        const nat i = ( from + before ) / 2;
        if ( a[i] > b ) before = i; else from = i;
    }
    return a[from] == b ? from : a.size();
}

/**************** Операции с массивами с изменениями *****************/

template <class T> inline 
ArrRef<T> & operator <<= ( ArrRef<T> & a, nat n )
{
    if ( a.size() < 2 ) return a;
    T t;
    while ( n > 0 )
    {
        --n;
        t = a[0];
        for ( nat i = 1; i < a.size(); ++i ) a[i-1] = a[i];
        a[a.size()-1] = t;
    }
    return a;
}

template <class T> inline 
ArrRef<T> & operator >>= ( ArrRef<T> & a, nat n )
{
    if ( a.size() < 2 ) return a;
    T t;
    while ( n > 0 )
    {
        --n;
        t = a[a.size()-1];
        for ( nat i = a.size()-1; i > 0; --i ) a[i] = a[i-1];
        a[0] = t;
    }
    return a;
}

/*********************** For012 ************************/

template <class T> class For012
{
    ArrRef<T> & ref;
public:
    explicit For012 ( ArrRef<T> & a ) : ref ( a ) {}
    template <class T2> ArrRef<T> & operator = ( const T2 & a )
    {
        for ( nat i = 0; i < ref.size(); ++i ) ref[i] = a;
        return ref;
    }
    template <class T2> ArrRef<T> & operator += ( const T2 & a )
    {
        for ( nat i = 0; i < ref.size(); ++i ) ref[i] += a;
        return ref;
    }
    template <class T2> ArrRef<T> & operator -= ( const T2 & a )
    {
        for ( nat i = 0; i < ref.size(); ++i ) ref[i] -= a;
        return ref;
    }
    template <class T2> ArrRef<T> & operator *= ( const T2 & a )
    {
        for ( nat i = 0; i < ref.size(); ++i ) ref[i] *= a;
        return ref;
    }
    template <class T2> ArrRef<T> & operator /= ( const T2 & a )
    {
        for ( nat i = 0; i < ref.size(); ++i ) ref[i] /= a;
        return ref;
    }
    template <class T2> ArrRef<T> & operator %= ( const T2 & a )
    {
        for ( nat i = 0; i < ref.size(); ++i ) ref[i] %= a;
        return ref;
    }
};

template <class T> inline 
For012<T> for012 ( ArrRef<T> & a )
{
    return For012<T> ( a );
}

/********************** For012f ************************/

template <class T> class For012f
{
    ArrRef<T> & ref;
public:
    explicit For012f ( ArrRef<T> & a ) : ref ( a ) {}
    template <class T2> ArrRef<T> & operator = ( T2 & f )
    {
        for ( nat i = 0; i < ref.size(); ++i ) ref[i] = f();
        return ref;
    }
};

template <class T> inline 
For012f<T> for012f ( ArrRef<T> & a )
{
    return For012f<T> ( a );
}

#endif