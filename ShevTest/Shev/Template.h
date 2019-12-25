
#pragma once

#include "typedef.h"

// Несколько простых функций

template<class T> inline 
T _pow2 ( const T & i )
{
    return i * i;
}

template<class T> inline 
T _pow3 ( const T & i )
{
    return i * i * i;
}

template<class T> inline 
T _pow4 ( const T & i )
{
    return _pow2 ( i * i );
}

template<class T> inline 
T _pow5 ( const T & i )
{
    return _pow2 ( i * i ) * i;
}

template<class T> inline 
T _pow6 ( const T & i )
{
    return _pow3 ( i * i );
}

template<class T> inline 
T _pow7 ( const T & i )
{
    return _pow3 ( i * i ) * i;
}

template<class T> inline 
T _pow8 ( const T & i )
{
    return _pow4 ( i * i );
}

template<class T> inline 
T _pow9 ( const T & i )
{
    return _pow3 ( i * i * i );
}

template<class T> inline 
T _pow10 ( const T & i )
{
    return _pow5 ( i * i );
}

template<class T> inline 
void _swap ( T & i1, T & i2 )
{
    const T i ( i1 );
    i1 = i2;
    i2 = i;
}

template<class T> inline 
const T & _min ( const T & i1, const T & i2 )
{
    return i1 < i2 ? i1 : i2;
}

template<class T> inline 
const T & _max ( const T & i1, const T & i2 )
{
    return i1 > i2 ? i1 : i2;
}

template<class T> inline 
const T & _min ( const T & i1, const T & i2, const T & i3 )
{
    return i1 < i2 ? i1 < i3 ? i1 : i3 : i3 < i2 ? i3 : i2;
}

template<class T> inline 
const T & _max ( const T & i1, const T & i2, const T & i3 )
{
    return i1 > i2 ? i1 > i3 ? i1 : i3 : i3 > i2 ? i3 : i2;
}

template<class T> inline 
const T & _min ( const T & i1, const T & i2, const T & i3, const T & i4 )
{
    return _min ( i1, _min ( i2, i3, i4 ) );
}

template<class T> inline 
const T & _max ( const T & i1, const T & i2, const T & i3, const T & i4 )
{
    return _max ( i1, _max ( i2, i3, i4 ) );
}

template<class T> inline 
const T & _min ( const T & i1, const T & i2, const T & i3, const T & i4, const T & i5 )
{
    return _min ( i1, _min ( i2, i3, i4, i5 ) );
}

template<class T> inline 
const T & _max ( const T & i1, const T & i2, const T & i3, const T & i4, const T & i5 )
{
    return _max ( i1, _max ( i2, i3, i4, i5 ) );
}

/************************ Set1 ***************************/

template<class A>
struct Set1
{
    A a;

    Set1 () {}
    Set1 ( const A & x ) : a(x) {}
    Set1 & operator = ( const A & x ) { a = x; return *this; }
    operator A () const { return a; }
};

/************************ Set2 ***************************/

template < class A, class B = A > 
struct Set2
{
    A a;
    B b;

    Set2 () {}
    Set2 ( const A & x, const B & y ) : a(x), b(y) {}
 
    Set2 & operator += ( const Set2 & u )
    {
        a += u.a; b += u.b; return * this;
    }

    template < class T > Set2 & operator /= ( const T & u )
    {
        a /= u; b /= u; return * this;
    }
};

template<class A, class B> inline
bool operator < ( const Set2<A,B> & a, const Set2<A,B> & b )
{
    return a.a == b.a ? a.b < b.b : a.a < b.a;
}

template<class A, class B> inline
bool operator > ( const Set2<A,B> & a, const Set2<A,B> & b )
{
    return a.a == b.a ? a.b > b.b : a.a > b.a;
}

template<class A, class B> inline
bool operator <= ( const Set2<A,B> & a, const Set2<A,B> & b )
{
    return a.a == b.a ? a.b <= b.b : a.a < b.a;
}

template<class A, class B> inline
bool operator >= ( const Set2<A,B> & a, const Set2<A,B> & b )
{
    return a.a == b.a ? a.b >= b.b : a.a > b.a;
}

template<class A, class B> inline
bool operator != ( const Set2<A,B> & a, const Set2<A,B> & b )
{
    return a.a != b.a || a.b != b.b;
}

template<class A, class B> inline
bool operator == ( const Set2<A,B> & a, const Set2<A,B> & b )
{
    return a.a == b.a && a.b == b.b;
}

template<class A, class B> inline 
Set2<A,B> operator + ( const Set2<A,B> & a, const Set2<A,B> & b )
{
    return Set2<A,B> ( a.a + b.a, a.b + b.b );
}

template<class A, class B> inline 
Set2<A,B> operator - ( const Set2<A,B> & a, const Set2<A,B> & b )
{
    return Set2<A,B> ( a.a - b.a, a.b - b.b );
}

template<class A, class B, class C> inline 
Set2<A,B> operator / ( const Set2<A,B> & a, const C & b )
{
    return Set2<A,B> ( a.a / b, a.b / b );
}

/************************* Set3 **************************/

template < class A, class B = A, class C = B > 
struct Set3
{
	A a;
    B b;
    C c;

    Set3 () {}
    Set3 ( const A & x, const B & y, const C & z ) : a(x), b(y), c(z) {} 
};

template<class A, class B, class C> inline
bool operator < ( const Set3<A,B,C> & x, const Set3<A,B,C> & y )
{
    return x.a != y.a ? x.a < y.a : x.b != y.b ? x.b < y.b : x.c < y.c;
}

template<class A, class B, class C> inline
bool operator > ( const Set3<A,B,C> & x, const Set3<A,B,C> & y )
{
    return x.a != y.a ? x.a > y.a : x.b != y.b ? x.b > y.b : x.c > y.c;
}

template<class A, class B, class C> inline
bool operator <= ( const Set3<A,B,C> & x, const Set3<A,B,C> & y )
{
    return x.a != y.a ? x.a < y.a : x.b != y.b ? x.b < y.b : x.c <= y.c;
}

template<class A, class B, class C> inline
bool operator >= ( const Set3<A,B,C> & x, const Set3<A,B,C> & y )
{
    return x.a != y.a ? x.a > y.a : x.b != y.b ? x.b > y.b : x.c >= y.c;
}

template<class A, class B, class C> inline 
bool operator != ( const Set3<A,B,C> & x, const Set3<A,B,C> & y )
{
    return x.a != y.a || x.b != y.b || x.c != y.c;
}

template<class A, class B, class C> inline 
bool operator == ( const Set3<A,B,C> & x, const Set3<A,B,C> & y )
{
    return x.a == y.a && x.b == y.b && x.c == y.c;
}

template<class A, class B, class C> inline 
Set3<A,B,C> operator + ( const Set3<A,B,C> & x, const Set3<A,B,C> & y )
{
    return Set3<A,B,C> ( x.a + y.a, x.b + y.b, x.c + y.c );
}

template<class A, class B, class C> inline 
Set3<A,B,C> operator - ( const Set3<A,B,C> & x, const Set3<A,B,C> & y )
{
    return Set3<A,B,C> ( x.a - y.a, x.b - y.b, x.c - y.c );
}

template<class A, class B, class C, class D> inline 
Set3<A,B,C> operator * ( const Set3<A,B,C> & x, const D & y )
{
    return Set3<A,B,C> ( x.a * y, x.b * y, x.c * y );
}

template<class A, class B, class C, class D> inline 
Set3<A,B,C> operator / ( const Set3<A,B,C> & x, const D & y )
{
    return Set3<A,B,C> ( x.a / y, x.b / y, x.c / y );
}

/************************* Set4 **************************/

template < class A, class B = A, class C = B, class D = C > 
struct Set4
{
	A a;
    B b;
    C c;
    D d;

    Set4 () {}
    Set4 ( const A & x, const B & y, const C & z, const D & w ) : a(x), b(y), c(z), d(w) {} 
};

template<class A, class B, class C, class D> inline
bool operator < ( const Set4<A,B,C,D> & x, const Set4<A,B,C,D> & y )
{
    return x.a != y.a ? x.a < y.a : x.b != y.b ? x.b < y.b : x.c != y.c ? x.c < y.c : x.d < y.d;
}

template<class A, class B, class C, class D> inline
bool operator > ( const Set4<A,B,C,D> & x, const Set4<A,B,C,D> & y )
{
    return x.a != y.a ? x.a > y.a : x.b != y.b ? x.b > y.b : x.c != y.c ? x.c > y.c : x.d > y.d;
}

template<class A, class B, class C, class D> inline
bool operator <= ( const Set4<A,B,C,D> & x, const Set4<A,B,C,D> & y )
{
    return x.a != y.a ? x.a < y.a : x.b != y.b ? x.b < y.b : x.c != y.c ? x.c < y.c : x.d <= y.d;
}

template<class A, class B, class C, class D> inline
bool operator >= ( const Set4<A,B,C,D> & x, const Set4<A,B,C,D> & y )
{
    return x.a != y.a ? x.a > y.a : x.b != y.b ? x.b > y.b : x.c != y.c ? x.c > y.c : x.d >= y.d;
}

template < class A, class B, class C, class D > 
inline bool operator != ( const Set4<A,B,C,D> & a, const Set4<A,B,C,D> & b )
{
    return a.a != b.a || a.b != b.b || a.c != b.c || a.d != b.d;
}

template < class A, class B, class C, class D > 
inline bool operator == ( const Set4<A,B,C,D> & a, const Set4<A,B,C,D> & b )
{
    return a.a == b.a && a.b == b.b && a.c == b.c && a.d == b.d;
}

/***********************  Merge... *************************/

template<class A, class B> struct Merge2 : public A, public B {};

template<class A, class B, class C> struct Merge3 : public A, public B, public C {};

/************************ SortItem *************************/

template <class H, class T=H> 
struct SortItem
{
    H head;
    T tail;

    SortItem () {}
    SortItem ( const H & h ) : head(h) {} 
    SortItem ( const H & h, const T & t ) : head(h), tail(t) {} 
};

template <class H, class T> inline
bool operator < ( const SortItem<H,T> & a, const SortItem<H,T> & b )
{
    return a.head < b.head;
}

template <class H, class T> inline
bool operator > ( const SortItem<H,T> & a, const SortItem<H,T> & b )
{
    return a.head > b.head;
}

template <class H, class T> inline
bool operator <= ( const SortItem<H,T> & a, const SortItem<H,T> & b )
{
    return a.head <= b.head;
}

template <class H, class T> inline
bool operator >= ( const SortItem<H,T> & a, const SortItem<H,T> & b )
{
    return a.head >= b.head;
}

template <class H, class T> inline
bool operator == ( const SortItem<H,T> & a, const SortItem<H,T> & b )
{
    return a.head == b.head;
}

template <class H, class T> inline
bool operator != ( const SortItem<H,T> & a, const SortItem<H,T> & b )
{
    return a.head != b.head;
}

template <class H, class T> inline
void _swap ( SortItem<H,T> & a, SortItem<H,T> & b )
{
    _swap ( a.head, b.head );
    _swap ( a.tail, b.tail );
}

/************************ SemiRef *************************/

template <class T> struct SemiRef
{
    T * p;

    explicit SemiRef ( T & t ) : p(&t) {} 
};

template <class T> inline
bool operator < ( const SemiRef<T> & a, const SemiRef<T> & b )
{
    return *a.p < *b.p;
}

template <class T> inline
bool operator > ( const SemiRef<T> & a, const SemiRef<T> & b )
{
    return *a.p > *b.p;
}

template <class T> inline
bool operator <= ( const SemiRef<T> & a, const SemiRef<T> & b )
{
    return *a.p <= *b.p;
}

template <class T> inline
bool operator >= ( const SemiRef<T> & a, const SemiRef<T> & b )
{
    return *a.p >= *b.p;
}

template <class T> inline
bool operator == ( const SemiRef<T> & a, const SemiRef<T> & b )
{
    return *a.p == *b.p;
}

template <class T> inline
bool operator != ( const SemiRef<T> & a, const SemiRef<T> & b )
{
    return *a.p != *b.p;
}

template <class T> inline
T operator + ( const SemiRef<T> & a, const SemiRef<T> & b )
{
    return *a.p + *b.p;
}

/************************ Pow2 ***************************/

class Pow2 {};
template <class T> inline T & operator <= ( T & v, const Pow2 & ) { return v *= v; }

/************************ Neg... *************************/

class NegX {};
template <class T> inline T & operator <= ( T & v, const NegX & ) { v.x = - v.x; return v; }

class NegY {};
template <class T> inline T & operator <= ( T & v, const NegY & ) { v.y = - v.y; return v; }

class NegXY {};
template <class T> inline T & operator <= ( T & v, const NegXY & ) { v.x = - v.x; v.y = - v.y; return v; }

/************************* Def ***************************/

template <class T> struct BaseType { typedef T type; };
template <> struct BaseType <nat> { typedef Set1<nat> type; };
template <> struct BaseType <bool> { typedef Set1<bool> type; };
template <> struct BaseType <double> { typedef Set1<double> type; };

template <class T> struct Def : public BaseType<T>::type
{
    typedef typename BaseType<T>::type base;
    bool isDef;

    Def () : isDef(false) {}
    Def ( const T & v, bool d = true ) : base ( v ), isDef ( d ) {}
    Def & operator = ( const T & v ) { isDef = true; *(base*) this = v; return *this; }
};