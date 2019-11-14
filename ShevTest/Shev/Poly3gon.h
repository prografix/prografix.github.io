
#pragma once

#include "ShevArray.h"

template <class I> class Trigon
{
public:
    nat a, b, c; // Номера вершин 
    I info;
};

template <> class Trigon<void>
{
public:
    nat a, b, c; // Номера вершин 
};

template <class V, class I> class Poly3gon
{
    Poly3gon ( Poly3gon & );
public:
    DynArray<V> vertex;         // массив вершин
    DynArray< Trigon<I> > side; // массив треугольников

    Poly3gon () {}
    Poly3gon ( nat nv, nat ns ) : vertex(nv), side(ns) {}

    Poly3gon & resize ( nat nv, nat ns )
    {
        vertex.resize ( nv );
        side.resize ( ns );
        return *this;
    }
};

template <class T, class I> inline
void swap ( Poly3gon<T,I> & p1, Poly3gon<T,I> & p2 )
{
    p1.vertex.swap ( p2.vertex );
    p1.side.swap ( p2.side );
}

template <class A, class B> inline void assign ( A & a, const B & b )
{
    a = (A) b;
}

template <class A, class B> inline void assignXYZ ( A & a, const B & b )
{
    assign ( a.x, b.x );
    assign ( a.y, b.y );
    assign ( a.z, b.z );
}

class Vector3f;
class Vector3d;
class Plane3d;

template <class T>
inline void makePlane ( const Trigon<T> & t, const Vector3f * vertex, Plane3d & plane )
{
    Vector3d va, vb, vc;
    assignXYZ ( va, vertex[t.a] );
    assignXYZ ( vb, vertex[t.b] );
    assignXYZ ( vc, vertex[t.c] );
    plane = Plane3d ( va, vb, vc );
}

template <class T>
inline void makePlane ( const Trigon<T> & t, const Vector3d * vertex, Plane3d & plane )
{
    plane = Plane3d ( vertex[t.a], vertex[t.b], vertex[t.c] );
}
