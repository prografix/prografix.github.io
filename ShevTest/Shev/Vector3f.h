
#ifndef VECTOR3F_H
#define VECTOR3F_H

//////////////////////////////// Вектор ///////////////////////////////////////

class Vector3f
{
    template <typename T> void mul ( const T & t, ... )
    {
        *this = t ( *this );
    }

    template <typename T> void mul ( const T & t, float d )
    {
        x *= d;
        y *= d;
        z *= d;
    }
public :
    float x, y, z;

    Vector3f () {}
    Vector3f ( float a, float b, float c ) : x ( a ), y ( b ), z ( c ) {}
 
    Vector3f operator - ()
    {
        return Vector3f ( - x, - y, - z );
    }
 
    Vector3f & operator += ( const Vector3f & v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return * this;
    }

    Vector3f & operator -= ( const Vector3f & v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return * this;
    }

    Vector3f & operator *= ( const Vector3f & v )
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return * this;
    }

    Vector3f & operator /= ( const Vector3f & v )
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return * this;
    }

    template <typename T> Vector3f & operator *= ( const T & t )
    {
        mul ( t, t );
        return * this;
    }

    Vector3f & operator /= ( const float d )
    {
        x /= d;
        y /= d;
        z /= d;
        return * this;
    }

    Vector3f & fill ( float d = 0 )
    {
        x = y = z = d;
        return * this;
    }

    bool operator ! () const
    {
        return !x && !y && !z;
    }

// Получение перпендикуляра к данному вектору
    Vector3f perpendicular () const;

// Задание векторных норм
    Vector3f & setNorm1 ( float p = 1 ); // единичная норма
    Vector3f & setNorm2 ( float p = 1 ); // квадратичная норма
    Vector3f & setNormU ( float p = 1 ); // бесконечная норма
};

inline Vector3f operator + ( const Vector3f& a, const Vector3f& b )
{
    return Vector3f ( a.x + b.x, a.y + b.y, a.z + b.z );
}

inline Vector3f operator - ( const Vector3f& a, const Vector3f& b )
{
    return Vector3f ( a.x - b.x, a.y - b.y, a.z - b.z );
}
 
inline Vector3f operator * ( const Vector3f& a, float d )
{
    return Vector3f ( a.x * d, a.y * d, a.z * d );
}
 
inline Vector3f operator / ( const Vector3f& a, float d )
{
    return Vector3f ( a.x / d, a.y / d, a.z / d );
}

inline Vector3f operator * ( float d, const Vector3f& a )
{
    return Vector3f ( a.x * d, a.y * d, a.z * d );
}

inline float operator * ( const Vector3f& a, const Vector3f& b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float qmod ( const Vector3f& a )
{
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

inline Vector3f operator % ( const Vector3f& a, const Vector3f& b )
{
    return Vector3f ( a.y * b.z - a.z * b.y,
                      a.z * b.x - a.x * b.z,
                      a.x * b.y - a.y * b.x );
}

inline bool operator != ( const Vector3f& a, const Vector3f& b )
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline bool operator == ( const Vector3f& a, const Vector3f& b )
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline void reper ( const Vector3f & x, Vector3f & y, Vector3f & z )
{
    y = x.perpendicular ();
    z = x % y;
}

float norm1 ( const Vector3f & v ); // единичная норма
float norm2 ( const Vector3f & v ); // квадратичная норма
float normU ( const Vector3f & v ); // бесконечная норма

//////////////////////////////// Отрезок //////////////////////////////////////

class Segment3f
{
public:
    Vector3f a, b;

    Segment3f () {}
    Segment3f ( const Vector3f & va, const Vector3f & vb ) : a(va), b(vb) {}
};

inline bool operator != ( const Segment3f & s1, const Segment3f & s2 )
{
    return ( s1.a != s2.a ) || ( s1.b != s2.b );
}

inline bool operator == ( const Segment3f & s1, const Segment3f & s2 )
{
    return ( s1.a == s2.a ) && ( s1.b == s2.b );
}

inline double length ( const Segment3f & s )
{
    return norm2 ( s.b - s.a );
}

///////////////////////////// Плоскость ///////////////////////////////////////

class Plane3f
{
public:
    Vector3f norm;
    float    dist;
    Plane3f () {}
    Plane3f ( const Vector3f &, const Vector3f &, const Vector3f & );
    Plane3f ( const Vector3f & v, const float & d ) : norm ( v ),
                                                      dist ( d ) {}
    float operator % ( const Vector3f & v ) const
    {
        return norm.x * v.x + norm.y * v.y + norm.z * v.z + dist;
    }
};

inline bool operator == ( const Plane3f & p1, const Plane3f & p2 )
{
   return p1.norm == p2.norm && p1.dist == p2.dist;
}

inline bool operator != ( const Plane3f & p1, const Plane3f & p2 )
{
   return p1.norm != p2.norm || p1.dist != p2.dist;
}

#endif