
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "typedef.h"

// Несколько простых функций

template<class T> inline 
T _squ ( const T & i )
{
    return i * i;
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

/************************ Set1 ***************************/

template<class A>
struct Set1
{
    A a;

    Set1 () {}
    explicit Set1 ( const A & x ) : a(x) {}
    Set1 & operator= ( const A & x ) { a = x; return *this; }
    operator A & () { return a; }
};

template<class A> inline
bool operator < ( const Set1<A> & x, const Set1<A> & y ) { return x.a < y.a; }

template<class A> inline
bool operator > ( const Set1<A> & x, const Set1<A> & y ) { return x.a > y.a; }

template<class A> inline
bool operator <= ( const Set1<A> & x, const Set1<A> & y ) { return x.a <= y.a; }

template<class A> inline
bool operator >= ( const Set1<A> & x, const Set1<A> & y ) { return x.a >= y.a; }

template<class A> inline
bool operator == ( const Set1<A> & x, const Set1<A> & y ) { return x.a == y.a; }

template<class A> inline
bool operator != ( const Set1<A> & x, const Set1<A> & y ) { return x.a != y.a; }

/************************ Set2 ***************************/

template < class A, class B = A > 
struct Set2
{
    A a;
    B b;

    Set2 () {}
    Set2 ( const A & x, const B & y ) : a(x), b(y) {} 
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

template < class A, class B, class C, class D > 
inline bool operator != ( const Set4<A,B,C,D>& a, const Set4<A,B,C,D>& b )
{
    return a.a != b.a || a.b != b.b || a.c != b.c || a.d != b.d;
}

template < class A, class B, class C, class D > 
inline bool operator == ( const Set4<A,B,C,D>& a, const Set4<A,B,C,D>& b )
{
    return a.a == b.a && a.b == b.b && a.c == b.c && a.d == b.d;
}

/***********************  Merge... *************************/

template<class A, class B> struct Merge2 : public A, public B {};

template<class A, class B, class C> struct Merge3 : public A, public B, public C {};

/************************ SortItem *************************/

template <class H, class T> 
struct SortItem
{
    H head;
    T tail;

    SortItem () {}
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

/************************ Pow2 ***************************/

class Pow2
{
public:
    template <class T> void operator () ( T & v ) { v *= v; }
};

/************************ Neg... *************************/

class NegX
{
public:
    template <class T> void operator () ( T & v ) { v.x = - v.x; }
};

class NegY
{
public:
    template <class T> void operator () ( T & v ) { v.y = - v.y; }
};

class NegXY
{
public:
    template <class T> void operator () ( T & v ) { v.x = - v.x; v.y = - v.y; }
};

/************************* Def ***************************/

template <class T> struct BaseType { typedef T type; };
template <> struct BaseType <nat> { typedef Set1<nat> type; };
template <> struct BaseType <double> { typedef Set1<double> type; };

template <class T> struct Def : public BaseType<T>::type
{
    typedef typename BaseType<T>::type base;
    bool isDef;

    Def () : isDef(false) {}
    Def ( const T & v, bool d = true ) : base ( v ), isDef ( d ) {}
    Def & operator = ( const T & v ) { isDef = true; *(base*) this = v; return *this; }
};

#endif