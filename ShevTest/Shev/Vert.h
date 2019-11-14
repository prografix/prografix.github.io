
#ifndef VERT_H
#define VERT_H

#include "Vector3d.h"
#include "ShevList.h"

class Vert : public ShevItem
{
public:
    Vector3d point;
    double  d;

    Vert () {}
    Vert ( const Vector3d & v ) : point ( v ) {}

    template <typename T> Vert & operator *= ( const T & t )
    {
        point *= t;
        return *this;
    }
};

typedef List<Vert> VertList;

#endif
