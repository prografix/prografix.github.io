
#pragma once

class Vector4d
{
public:
    double x1, x2, x3, x4;
    Vector4d () {}
    Vector4d ( double a, double b, double c, double d ) : x1(a), x2(b), x3(c), x4(d) {}
 
    Vector4d operator - () const
    {
        return Vector4d ( - x1, - x2, - x3, - x4 );
    }

    Vector4d & operator += ( const Vector4d & v )
    {
        x1 += v.x1;
        x2 += v.x2;
        x3 += v.x3;
        x4 += v.x4;
        return * this;
    }

    Vector4d & operator -= ( const Vector4d & v )
    {
        x1 -= v.x1;
        x2 -= v.x2;
        x3 -= v.x3;
        x4 -= v.x4;
        return * this;
    }

    Vector4d & operator *= ( const double d )
    {
        x1 *= d;
        x2 *= d;
        x3 *= d;
        x4 *= d;
        return * this;
    }

    Vector4d & operator /= ( const double d )
    {
        x1 /= d;
        x2 /= d;
        x3 /= d;
        x4 /= d;
        return * this;
    }

    Vector4d & fill ( double d = 0 )
    {
        x1 = x2 = x3 = x4 = d;
        return * this;
    }

    bool operator ! () const
    {
        return !x1 && !x2 && !x3 && !x4;
    }
};

inline Vector4d operator + ( const Vector4d & a, const Vector4d & b )
{
    return Vector4d ( a.x1 + b.x1, a.x2 + b.x2, a.x3 + b.x3, a.x4 + b.x4 );
}

inline Vector4d operator - ( const Vector4d & a, const Vector4d & b )
{
    return Vector4d ( a.x1 - b.x1, a.x2 - b.x2, a.x3 - b.x3, a.x4 - b.x4 );
}
 
inline Vector4d operator * ( const Vector4d & a, const double d )
{
    return Vector4d ( a.x1 * d, a.x2 * d, a.x3 * d, a.x4 * d );
}
 
inline Vector4d operator / ( const Vector4d & a, const double d )
{
    return Vector4d ( a.x1 / d, a.x2 / d, a.x3 / d, a.x4 / d );
}
 
inline Vector4d operator * ( const double d, const Vector4d & a )
{
    return Vector4d ( a.x1 * d, a.x2 * d, a.x3 * d, a.x4 * d );
}

inline double operator * ( const Vector4d & a, const Vector4d & b )
{
    return a.x1 * b.x1 + a.x2 * b.x2 + a.x3 * b.x3 + a.x4 * b.x4;
}

inline double qmod ( const Vector4d & a )
{
    return a.x1 * a.x1 + a.x2 * a.x2 + a.x3 * a.x3 + a.x4 * a.x4;
}

inline bool operator == ( const Vector4d & a, const Vector4d & b )
{
    return a.x1 == b.x1 && a.x2 == b.x2 && a.x3 == b.x3 && a.x4 == b.x4;
}

inline bool operator != ( const Vector4d & a, const Vector4d & b )
{
    return a.x1 != b.x1 || a.x2 != b.x2 || a.x3 != b.x3 || a.x4 != b.x4;
}

class Plane4d
{
public:
    Vector4d norm;
    double   dist;

    Plane4d () {}
    Plane4d ( const Vector4d & v, const double & d ) : norm ( v ), dist ( d ) {}

    double operator % ( const Vector4d & v ) const
    {
        return norm * v + dist;
    }

    Plane4d operator - () const
    {
        return Plane4d ( - norm, - dist );
    }
};

#include "ShevArray.h"

//**************************** 22.04.2019 *********************************//
//
//                Пересечение четырёх гиперплоскостей
//
//**************************** 22.04.2019 *********************************//

Def<Vector4d> intersection ( const Plane4d & plane1, const Plane4d & plane2, const Plane4d & plane3, const Plane4d & plane4 );


//************************ 09.05.2019 *******************************//
//
//      Вычисление ближайшей точки к заданным плоскостям
//      Минимум суммы квадратов расстояний ( 2-норма )
//
//************************ 09.05.2019 *******************************//

Def<Vector4d> getNearPoint2 ( CCArrRef<Plane4d> & plane );


//************************ 28.04.2019 *******************************//
//
//      Вычисление ближайшей точки к заданным плоскостям
//      Минимум максимального расстояния ( бесконечная норма )
//
//************************ 04.05.2019 *******************************//

Def<Vector4d> getNearPointU ( CCArrRef<Plane4d> & plane );
