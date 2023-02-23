
#ifndef POLY3GON_H
#define POLY3GON_H

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
    swap ( p1.vertex, p2.vertex );
    swap ( p1.side, p2.side );
}

#endif
