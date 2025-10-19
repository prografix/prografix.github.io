
#pragma once

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
