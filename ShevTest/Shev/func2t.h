
#pragma once

#include "ShevArray.h"

//*************************************************************************//
//
//                  Двойная площадь многоугольника
//
//**************************** 15.09.2013 *********************************//

template <class Scalar, class Vector> 
Scalar tempArea2 ( CArrRef<Vector> vert )
{
    if ( vert.size() < 3 ) return 0;
    Scalar a = 0;
    Vector v1 = vert[1] - vert[0];
    for ( nat i = 2; i < vert.size(); ++i )
    {
        Vector v2 = vert[i] - vert[0];
        a += v1 % v2;
        v1 = v2;
    }
    return a;
}


//**************************** 18.08.2004 *********************************//
//
//          dist - квадрат расстояния от точки до границы многоугольника
//          index - номер ближайшего ребра или вершины
//          в зависимости от истинности isBone
//
//**************************** 07.02.2014 *********************************//

template <class Scalar, class Vector> 
bool tempDist2 ( CArrRef<Vector> vert, const Vector & point,
                 Scalar & dist, nat & index, bool & isBone )
{
    const nat nv = vert.size();
    if ( nv == 0 ) return false;
    isBone = false;
    dist = qmod ( vert[0] - point );
    index = 0;
    if ( nv < 3 )
    {
        if ( nv == 1 ) return true;
        const Vector & a = vert[0];
        const Vector & b = vert[1];
        const Vector c = b - a;
        if ( !c ) return true;
        const Vector d = point - a;
        Scalar t = d * c;
        if ( t <= 0 ) return true;
        const Scalar q = c * c;
        if ( t >= q )
        {
            dist = qmod ( point - b );
            index = 1;
        }
        else
        {
            t = d % c;
            dist = ( t * t ) / q;
            if ( t < 0 ) index = 1;
            isBone = true;
        }
        return true;
    }
    nat i;
    for ( i = 0; i < nv; ++i )
    {
        const Vector & a = vert[i?i-1:nv-1];
        const Vector & b = vert[i];
        const Vector c = b - a;
        if ( !c ) continue;
        const Vector d = point - a;
        Scalar t = d * c;
        if ( t < 0 ) continue;
        const Scalar q = c * c;
        if ( t >= q )
        {
            t = qmod ( b - point );
            if ( dist > t )
            {
                dist = t;
                index = i;
                isBone = false;
            }
        }
        else
        {
            t = d % c;
            t = ( t * t ) / q;
            if ( dist > t )
            {
                dist = t;
                index = i;
                isBone = true;
            }
        }
    }
    return true;
}
