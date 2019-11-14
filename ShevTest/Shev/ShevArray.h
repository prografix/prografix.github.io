
#pragma once

#include "typedef.h"
#include "template.h"

//#define ARRAY_TEST
#ifdef ARRAY_TEST
void outOfRange ( const char * name, nat size, nat index );
#endif

#define CCArrRef const CArrRef

/********************** CArrRef ************************/
/************ Ссылка на константный массив *************/

template <class T> class CArrRef
{
    void operator= ( CCArrRef & );
protected:
    union { const T * con; T * var; } _data;
    nat _size;
public:
    CArrRef () : _size(0) { _data.con = 0; }
    CArrRef ( const T * d, nat n ) :  _size(n) { _data.con = d; }
    CArrRef ( CCArrRef<T> & a ) : _data(a._data), _size(a._size) {}
// Указатель на i-ый элемент:
    const T * operator() ( nat i = 0 ) const { return _size > i ? _data.con + i : 0; }
#ifdef ARRAY_TEST
    void error ( nat n, nat i ) const { outOfRange ( "CArrRef", n, i ); }
    CArrRef ( CCArrRef<T> & a, nat i, nat n ) : _size(n) { _data.con = a(i); if ( a._size < i + n ) error ( a._size, i + n ); }
// Ссылка на i-ый элемент:
    const T & operator[] ( nat i ) const { if ( _size <= i ) error ( _size, i ); return _data.con[i]; }
// Ссылка на i-ый элемент от конца:
    const T & las ( nat i = 0 ) const { if ( _size <= i ) error ( _size, i ); return _data.con[_size-1-i]; }
// Ссылка на предыдущий элемент в цикле:
    const T & cprev ( nat i )        const { if ( _size <= i ) error ( _size, i ); return _data.con[i>0 ? i-1:_size-1]; }
    const T & cprev ( nat i, nat k ) const { if ( _size <= i ) error ( _size, i ); return _data.con[(i+_size-k)%_size]; }
// Ссылка на следущий элемент в цикле:
    const T & cnext ( nat i )        const { if ( _size <= i ) error ( _size, i ); return _data.con[i+1<_size?i+1:0]; }
    const T & cnext ( nat i, nat k ) const { if ( _size <= i ) error ( _size, i ); return _data.con[(i+k)%_size]; }
#else
    CArrRef ( CCArrRef<T> & a, nat i, nat n ) : _size(n) { _data.con = a(i); }
// Ссылка на i-ый элемент:
    const T & operator[] ( nat i ) const { return _data.con[i]; }
// Ссылка на i-ый элемент от конца:
    const T & las ( nat i = 0 ) const { return _data.con[_size-1-i]; }
// Ссылка на предыдущий элемент в цикле:
    const T & cprev ( nat i )        const { return _data.con[i>0 ? i-1:_size-1]; }
    const T & cprev ( nat i, nat k ) const { return _data.con[(i+_size-k)%_size]; }
// Ссылка на следущий элемент в цикле:
    const T & cnext ( nat i )        const { return _data.con[i+1<_size?i+1:0]; }
    const T & cnext ( nat i, nat k ) const { return _data.con[(i+k)%_size]; }
#endif
// Количество элементов
    nat size () const { return _size; }
};

/********************* MutCArrRef **********************/
/******* Изменяемая ссылка на константный массив *******/

template <class T> class MutCArrRef : public CArrRef<T>
{
    void operator= ( const MutCArrRef & );
public:
    void reset ( CCArrRef<T> & a )
    {
        _data.con = a();
        _size = a.size();
    }
};

/*********************** ArrRef ************************/
/****************** Ссылка на массив *******************/

template <class T> class ArrRef : public CArrRef<T>
{
public:
    ArrRef () {}
    ArrRef ( T * d, nat n ) : CArrRef<T>( d, n ) {}
    CCArrRef<T> & operator * () const { return *this; }
// Указатель на i-ый элемент:
    T * operator() ( nat i = 0 ) { return _size > i ? _data.var + i : 0; }
    const T * operator() ( nat i = 0 ) const { return _size > i ? _data.var + i : 0; }
#ifdef ARRAY_TEST
    void error ( nat n, nat i ) const { outOfRange ( "ArrRef", n, i ); }
    ArrRef ( ArrRef<T> a, nat i, nat n ) : CArrRef<T>( a(i), n ) { if ( a._size < i + n ) error ( a._size, i + n ); }
// Ссылка на i-ый элемент:
    T & operator[] ( nat i ) { if ( _size <= i ) error ( _size, i ); return _data.var[i]; }
// Ссылка на i-ый элемент от конца:
    T & las ( nat i = 0 ) { if ( _size <= i ) error ( _size, i ); return _data.var[_size-1-i]; }
// Ссылка на предыдущий элемент в цикле:
    T & cprev ( nat i )        { if ( _size <= i ) error ( _size, i ); return _data.var[i>0 ? i-1:_size-1]; }
    T & cprev ( nat i, nat k ) { if ( _size <= i ) error ( _size, i ); return _data.var[(i+_size-k)%_size]; }
// Ссылка на следущий элемент в цикле:
    T & cnext ( nat i )        { if ( _size <= i ) error ( _size, i ); return _data.var[i+1<_size?i+1:0]; }
    T & cnext ( nat i, nat k ) { if ( _size <= i ) error ( _size, i ); return _data.var[(i+k)%_size]; }
// Ссылка на i-ый элемент:
    const T & operator[] ( nat i ) const { if ( _size <= i ) error ( _size, i ); return _data.var[i]; }
// Ссылка на i-ый элемент от конца:
    const T & las ( nat i = 0 ) const { if ( _size <= i ) error ( _size, i ); return _data.var[_size-1-i]; }
// Ссылка на предыдущий элемент в цикле:
    const T & cprev ( nat i )        const { if ( _size <= i ) error ( _size, i ); return _data.var[i>0 ? i-1:_size-1]; }
    const T & cprev ( nat i, nat k ) const { if ( _size <= i ) error ( _size, i ); return _data.var[(i+_size-k)%_size]; }
// Ссылка на следущий элемент в цикле:
    const T & cnext ( nat i )        const { if ( _size <= i ) error ( _size, i ); return _data.var[i+1<_size?i+1:0]; }
    const T & cnext ( nat i, nat k ) const { if ( _size <= i ) error ( _size, i ); return _data.var[(i+k)%_size]; }
#else
    ArrRef ( ArrRef<T> a, nat i, nat n ) : CArrRef<T>( a(i), n ) {}
// Ссылка на i-ый элемент:
    T & operator[] ( nat i ) { return _data.var[i]; }
// Ссылка на i-ый элемент от конца:
    T & las ( nat i = 0 ) { return _data.var[_size-1-i]; }
// Ссылка на предыдущий элемент в цикле:
    T & cprev ( nat i )        { return _data.var[i>0 ? i-1:_size-1]; }
    T & cprev ( nat i, nat k ) { return _data.var[(i+_size-k)%_size]; }
// Ссылка на следущий элемент в цикле:
    T & cnext ( nat i ) { return _data.var[i+1<_size?i+1:0]; }
    T & cnext ( nat i, nat k ) { return _data.var[(i+k)%_size]; }
// Ссылка на i-ый элемент:
    const T & operator[] ( nat i ) const { return _data.var[i]; }
// Ссылка на i-ый элемент от конца:
    const T & las ( nat i = 0 ) const { return _data.var[_size-1-i]; }
// Ссылка на предыдущий элемент в цикле:
    const T & cprev ( nat i )        const { return _data.var[i>0 ? i-1:_size-1]; }
    const T & cprev ( nat i, nat k ) const { return _data.var[(i+_size-k)%_size]; }
// Ссылка на следущий элемент в цикле:
    const T & cnext ( nat i )        const { return _data.var[i+1<_size?i+1:0]; }
    const T & cnext ( nat i, nat k ) const { return _data.var[(i+k)%_size]; }
#endif
// Операторы присваивания
    ArrRef & operator= ( CCArrRef<T> & p )
    {
        const nat n = _min ( _size, p.size() );
        for ( nat i = 0; i < n; ++i ) _data.var[i] = p[i];
        return *this;
    }
    ArrRef & operator= ( const ArrRef & a )
    {
        return *this = *a;
    }
// Изменение порядка следования элементов на обратный
    ArrRef & reverse ()
    {
        if ( _size < 2 ) return *this;
        const nat n = _size - 1;
        const nat m = _size / 2;
        for ( nat i = 0; i < m; ++i ) _swap ( _data.var[i], _data.var[n-i] );
        return *this;
    }
// Заполнение всех элементов заданным значением
    ArrRef & fill ( const T & t )
    {
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = t;
        return *this;
    }
// Выполнение оператора <= для всех элементов массива
    template <class S> ArrRef & operator << ( S & s )
    {
        for ( nat i = 0; i < _size; ++i ) _data.var[i] <= s;
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
        for ( nat i = 0; i < n; ++i ) _data.var[i] = a[i];
        return *this;
    }

    FixArrRef & operator+= ( const FixArrRef & a )
    {
        for ( nat i = 0; i < n; ++i ) _data.var[i] += a[i];
        return *this;
    }

    FixArrRef & operator-= ( const FixArrRef & a )
    {
        for ( nat i = 0; i < n; ++i ) _data.var[i] -= a[i];
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

    FixArray & operator+= ( const FixArray & a )
    {
        for ( nat i = 0; i < n; ++i ) stor[i] += a[i];
        return *this;
    }

    FixArray & operator-= ( const FixArray & a )
    {
        for ( nat i = 0; i < n; ++i ) stor[i] -= a[i];
        return *this;
    }

    FixArray & operator*= ( const T & t )
    {
        for ( nat i = 0; i < n; ++i ) stor[i] *= t;
        return *this;
    }
// Заполнение всех элементов заданным значением
    FixArray & fill ( const T & t )
    {
        ArrRef<T>::fill ( t );
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

    DynArrRef & operator= ( CCArrRef<T> & r )
    {
        if ( _data.var == r() ) return *this;
        if ( _size != r.size() ) resize ( r.size() );
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
        return *this;
    }

    DynArrRef & operator= ( const DynArrRef & a )
    {
        return *this = *a;
    }
};

/********************** DynArray ***********************/
/**************** Динамический массив ******************/

template <class T> class DynArray : public DynArrRef<T>
{
    DynArray ( const DynArray & );
public:
explicit DynArray ( nat n = 0 ) : DynArrRef<T> ( n > 0 ? new T[n] : 0, n ) {}
explicit DynArray ( CCArrRef<T> & r ) : DynArrRef<T> ( new T[r.size()], r.size() )
     {
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
     }

    ~DynArray () { delete[] _data.var; }

    DynArray & operator= ( CCArrRef<T> & r )
    {
        if ( _data.var == r() ) return *this;
        if ( _size != r.size() ) resize ( r.size() );
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
        return *this;
    }

    DynArray & operator= ( const DynArray & a )
    {
        return *this = *a;
    }

    virtual DynArrRef<T> & resize ( nat n = 0 )
    {
        delete[] _data.var;
        _data.var = 0; // на случай исключения в new
        if ( ( _size = n ) > 0 ) _data.var = new T[n];
        return *this;
    }

    DynArray & swap ( DynArray & a )
    {
        _swap ( _size, a._size );
        _swap ( _data, a._data );
        return *this;
    }

    friend inline void _swap ( DynArray & a1, DynArray & a2 )
    {
        a1.swap ( a2 );
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
explicit CmbArray ( CCArrRef<T> & r ) : DynArrRef<T> ( r.size() > N ? new T[r.size()] : stor, r.size() )
     {
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
     }

    ~CmbArray () { if ( _data.var != stor ) delete[] _data.var; }

    CmbArray & operator= ( CCArrRef<T> & r )
    {
        if ( _data.var == r() ) return *this;
        resize ( r.size() );
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
        return *this;
    }

    CmbArray & operator= ( const CmbArray & a )
    {
        return *this = *a;
    }

    CmbArray & operator -= ( const CmbArray & a )
    {
        const nat n = _min ( _size, a._size );
        for ( nat i = 0; i < n; ++i ) _data.var[i] -= a[i];
        return *this;
    }

    virtual DynArrRef<T> & resize ( nat n = 0 )
    {
        if ( _size == n ) return *this;
        if ( _data.var != stor )
        {
            delete[] _data.var;
            _data.var = stor;
        }
        if ( ( _size = n ) > N ) _data.var = new T[_size];
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
        if ( i < --_size ) _data.var[i] = _data.var[_size];
    }

    virtual void resizeAndCopy ( nat n ) = 0;
protected:
    nat real_size;
    SuiteRef ( T * d, nat n ) : DynArrRef<T>( d, n ) {}
public:
    SuiteRef & operator= ( CCArrRef<T> & r )
    {
        if ( _data.var == r() ) return *this;
        resize ( r.size() );
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
        return *this;
    }

    SuiteRef & operator= ( const SuiteRef & a )
    {
        return *this = *a;
    }

    virtual DynArrRef<T> & resize ( nat n = 0 )
    {
        if ( n > real_size ) resizeAndCopy ( n );
        else _size = n;
        return *this;
    }

    SuiteRef & add ( const T & t )
    {
        inc() = t;
        return *this;
    } 

    SuiteRef & addIfHasNot ( const T & t )
    {
        for ( nat i = 0; i < _size; ++i ) if ( _data.var[i] == t ) return *this;
        inc() = t;
        return *this;
    }

    T & inc ()
    {
        if ( _size == real_size ) resizeAndCopy ( _size + 1 );
        else ++_size;
        return _data.var[_size-1];
    }

    SuiteRef & inc ( nat n )
    {
        if ( _size + n > real_size ) resizeAndCopy ( _size + n );
        else _size += n;
        return *this;
    }

    SuiteRef & dec ( nat n = 1 )
    {
        _size = _size > n ? _size - n : 0;
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

    SuiteRef & delAndShift ( nat i )
    {
        if ( i < _size )
        {
            --_size;
#if _MSC_VER > 1200 // MS VC 6.0
            for ( ; i < _size; ++i ) copyFunc ( _data.var[i], _data.var[i+1] );
#else
            for ( ; i < _size; ++i ) _data.var[i] = _data.var[i+1];
#endif
        }
#ifdef ARRAY_TEST
        else outOfRange ( "SuiteRef::delAndShift", _size, i );
#endif ARRAY_TEST
        return *this;
    }

    SuiteRef & addAftLas ( CCArrRef<T> & a )
    {
        const nat s = _size;
        inc ( a.size() );
        for ( nat i = s; i < _size; ++i ) _data.var[i] = a[i-s];
        return *this;
    }
};

/*********************** Suite *************************/
/********** Последовательный набор элементов ***********/

template <class T> class Suite : public SuiteRef<T>
{
    Suite ( const Suite & );

    virtual void resizeAndCopy ( nat n )
    {
        real_size = _max ( real_size + n, 8u );
        T * tmp = new T[real_size];
#if _MSC_VER > 1200 // MS VC 6.0
        for ( nat i = 0; i < _size; ++i ) copyFunc ( tmp[i], _data.var[i] );
#else
        for ( nat i = 0; i < _size; ++i ) tmp[i] = _data.var[i];
#endif
        delete[] _data.var;
        _data.var = tmp;
        _size = n;
    }
public:
    Suite () : SuiteRef<T>(0, real_size=0) {}
    explicit Suite ( nat n, nat m = 0 ) : SuiteRef<T>((real_size=_max(n,m)) > 0 ? new T[real_size] : 0, m) {}
    ~Suite () { delete[] _data.var; }

    Suite & operator= ( CCArrRef<T> & r )
    {
        if ( _data.var == r() ) return *this;
        _size = r.size();
        if ( real_size < _size )
        {
            real_size = _size;
            delete[] _data.var;
            _data.var = 0; // на случай исключения в new
            _data.var = new T[_size];
        }
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
        return *this;
    }

    Suite & operator= ( const Suite & a )
    {
        return *this = *a;
    }

    Suite & swap ( Suite & a )
    {
        _swap ( real_size, a.real_size );
        _swap ( _size, a._size );
        _swap ( _data, a._data );
        return *this;
    }

    friend inline void _swap ( Suite & a1, Suite & a2 )
    {
        a1.swap ( a2 );
    }
};

#if _MSC_VER > 1200 // MS VC 6.0
template <class T> inline void copyFunc ( T & a, T & b ) { a = b; }
template <class T> inline void copyFunc ( Suite   <T> & a, Suite   <T> & b ) { _swap ( a, b ); }
template <class T> inline void copyFunc ( DynArray<T> & a, DynArray<T> & b ) { _swap ( a, b ); }
#endif

/********************* CmbSuite ************************/
/********** Комбинированный набор элементов ************/

template <class T, nat N> class CmbSuite : public SuiteRef<T>
{
    T stor[N];
    CmbSuite ( const CmbSuite & );

    virtual void resizeAndCopy ( nat n )
    {
        real_size = _max ( real_size + n, 8u );
        T * tmp = new T[real_size];
#if _MSC_VER > 1200 // MS VC 6.0
        for ( nat i = 0; i < _size; ++i ) copyFunc ( tmp[i], _data.var[i] );
#else
        for ( nat i = 0; i < _size; ++i ) tmp[i] = _data.var[i];
#endif
        if ( _data.var != stor ) delete[] _data.var;
        _data.var = tmp;
        _size = n;
    }
public:
    CmbSuite () : SuiteRef<T>(stor, 0) { real_size = N; }
    explicit CmbSuite ( nat n, nat m = 0 ) : 
        SuiteRef<T> ( ( real_size = _max(n,m) ) > N ? new T[real_size] : ( real_size = N, stor ), m ) {}
    ~CmbSuite () { if ( _data.var != stor ) delete[] _data.var; }

    CmbSuite & operator= ( CCArrRef<T> & r )
    {
        if ( _data.var == r() ) return *this;
        _size = r.size();
        if ( real_size < _size )
        {
            real_size = _size;
            if ( _data.var != stor )
            {
                delete[] _data.var;
                _data.var = stor; // на случай исключения в new
            }
            _data.var = new T[_size];
        }
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
        return *this;
    }

    CmbSuite & operator= ( const CmbSuite & a )
    {
        return *this = *a;
    }
};

/********************* LtdSuiteRef *********************/
/*** Ссылка на набор элементов ограниченного размера ***/

template <class T> class LtdSuiteRef : public SuiteRef<T>
{
    virtual void resizeAndCopy ( nat n )
    {
#ifdef ARRAY_TEST
        if ( n > real_size ) outOfRange ( "LtdSuiteRef::resizeAndCopy", real_size, n );
#endif
        _size = real_size;
    }
public:
    LtdSuiteRef ( ArrRef<T> a, nat i, nat n ) : SuiteRef<T>( a(i), 0 )
    {
        real_size = n;
#ifdef ARRAY_TEST
        if ( a.size() < i + n ) outOfRange ( "LtdSuiteRef", a.size(), i + n );
#endif
    }

    LtdSuiteRef & operator= ( CCArrRef<T> & r )
    {
        _size = r.size();
#ifdef ARRAY_TEST
        if ( _size > real_size ) outOfRange ( "LtdSuiteRef::operator=", real_size, _size );
#endif
        if ( _size > real_size ) _size = real_size;
        for ( nat i = 0; i < _size; ++i ) _data.var[i] = r[i];
        return *this;
    }

    LtdSuiteRef & operator= ( const LtdSuiteRef & a )
    {
        return *this = *a;
    }
};

/*************** Операции с массивами без их изменения ***************/

template <class T> inline 
bool operator == ( CCArrRef<T> & a, CCArrRef<T> & b )
{
    if ( a.size() != b.size() ) return false;
    for ( nat i = 0; i < a.size(); ++i ) 
        if ( a[i] != b[i] ) return false;
    return true;
}

template <class T> inline 
bool operator != ( CCArrRef<T> & a, CCArrRef<T> & b )
{
    if ( a.size() != b.size() ) return true;
    for ( nat i = 0; i < a.size(); ++i ) 
        if ( a[i] != b[i] ) return true;
    return false;
}

template <class T1, class T2> inline 
T1 & operator += ( T1 & t, CCArrRef<T2> & a )
{
    for ( nat i = 0; i < a.size(); ++i ) t += a[i];
    return t;
}

template <class T1, class T2> inline 
T1 & operator << ( T1 & t, CCArrRef<T2> & a )
{
    for ( nat i = 0; i < a.size(); ++i ) t << a[i];
    return t;
}

template <class A, class B> inline 
nat count ( const A & a, const B & b, nat from = 0 )
{
    nat n = 0;
    for ( nat i = from; i < a.size(); ++i ) if ( a[i] == b ) ++n;
    return n;
}

template <class A, class B> inline 
nat firEqu ( const A & a, const B & b, nat from = 0 )
{
    for ( nat i = from; i < a.size(); ++i ) if ( a[i] == b ) return i;
    return a.size();
}

template <class A, class B> inline 
nat lasEqu ( const A & a, const B & b )
{
    for ( nat i = a.size(); i > 0; --i ) if ( a[i-1] == b ) return i-1;
    return a.size();
}

template <class A, class B> inline 
bool hasEqu ( const A & a, const B & b )
{
    for ( nat i = 0; i < a.size(); ++i ) if ( a[i] == b ) return true;
    return false;
}

template <class T> inline 
nat firMin ( const T & a )
{
    nat m = 0;
    for ( nat i = 1; i < a.size(); ++i ) if ( a[i] < a[m] ) m = i;
    return m;
}

template <class T> inline 
nat firMax ( const T & a )
{
    nat m = 0;
    for ( nat i = 1; i < a.size(); ++i ) if ( a[i] > a[m] ) m = i;
    return m;
}

template <class T1, class T2> inline 
bool findMinMax ( const T1 & a, T2 & min, T2 & max )
{
    if ( ! a.size() ) return false;
    max = min = a[0];
    for ( nat i = 1; i < a.size(); ++i )
    {
        const T2 & t = a[i];
        if ( min > t ) min = t; else
        if ( max < t ) max = t;
    }
    return true;
}

template <class T> inline 
Def<T> amean ( CCArrRef<T> & a )
{
    if ( ! a.size() ) return Def<T>();
    T res ( a[0] );
    for ( nat i = 1; i < a.size(); ++i ) res += a[i];
    return res / a.size();
}

/**************** Операции с упорядоченными массивами ****************/

template <class A, class B> inline 
nat firEqu123 ( const A & a, const B & b )
{
    if ( ! a.size() ) return 0;
    int prior = -1, last = a.size() - 1;
    while ( prior + 1 < last )
    {
        const int i = ( prior + last ) / 2;
        if ( a[i] < b ) prior = i; else last = i;
    }
    return a[last] == b ? last : a.size();
}

template <class A, class B> inline 
nat lasEqu123 ( const A & a, const B & b )
{
    if ( ! a.size() ) return 0;
    nat from = 0, before = a.size();
    while ( from + 1 < before )
    {
        const nat i = ( from + before ) / 2;
        if ( a[i] > b ) before = i; else from = i;
    }
    return a[from] == b ? from : a.size();
}

template <class A, class B> inline 
bool hasEqu123 ( const A & a, const B & b )
{
    if ( ! a.size() ) return 0;
    nat from = 0, before = a.size();
    while ( from + 1 < before )
    {
        const nat i = ( from + before ) / 2;
        if ( a[i] > b ) before = i; else from = i;
    }
    return a[from] == b;
}

/**************** Операции с массивами с изменениями *****************/

template <class T> inline 
ArrRef<T> & operator <<= ( ArrRef<T> & a, nat k )
{
    const nat n = a.size();
    if ( n < 2 || ! ( k %= n ) ) return a;
    const nat m = n - k;
    if ( m < k ) return a >>= m;
    nat i;
    CmbArray<T, 64> b ( k );
    for ( i = 0; i < k; ++i ) b[i] = a[i];
    for ( i = 0; i < m; ++i ) a[i] = a[i+k];
    for ( i = 0; i < k; ++i ) a[i+m] = b[i];
    return a;
}

template <class T> inline 
ArrRef<T> & operator >>= ( ArrRef<T> & a, nat k )
{
    const nat n = a.size();
    if ( n < 2 || ! ( k %= n ) ) return a;
    const nat m = n - k;
    if ( m < k ) return a <<= m;
    nat i;
    CmbArray<T, 64> b ( k );
    for ( i = 0; i < k; ++i ) b[i] = a[i+m];
    for ( i = m; --i > 0; ) a[i+k] = a[i]; a[k] = a[0];
    for ( i = 0; i < k; ++i ) a[i] = b[i];
    return a;
}

template <class T1, class T2> inline 
ArrRef<T1> & operator += ( ArrRef<T1> & a, const T2 & b )
{
    for ( nat i = 0; i < a.size(); ++i ) a[i] += b;
    return a;
}

template <class T1, class T2> inline 
ArrRef<T1> & operator -= ( ArrRef<T1> & a, const T2 & b )
{
    for ( nat i = 0; i < a.size(); ++i ) a[i] -= b;
    return a;
}

template <class T1, class T2> inline 
ArrRef<T1> & operator *= ( ArrRef<T1> & a, const T2 & b )
{
    for ( nat i = 0; i < a.size(); ++i ) a[i] *= b;
    return a;
}

template <class T1, class T2> inline 
ArrRef<T1> & operator /= ( ArrRef<T1> & a, const T2 & b )
{
    for ( nat i = 0; i < a.size(); ++i ) a[i] /= b;
    return a;
}

template <class A, class B, class C> inline 
A & sum ( A & a, const B & b, const C & c )
{
    const nat n = _min ( a.size(), b.size(), c.size() );
    for ( nat i = 0; i < n; ++i ) a[i] = b[i] + c[i];
    return a;
}
