
#ifndef VECTOR2F_H
#define VECTOR2F_H

//////////////////////////////// ¬ектор ///////////////////////////////////////

class Vector2f
{
    template <typename T> void mul ( const T & t, ... )
    {
        *this = t ( *this );
    }

    template <typename T> void mul ( const T & t, double d )
    {
        x *= d; y *= d;
    }
public :
    float x, y;

    Vector2f () {}
    Vector2f ( float a, float b ) : x ( a ), y ( b ) {}
 
    Vector2f & operator += ( const Vector2f & u )
    {
        x += u.x;
        y += u.y;
        return * this;
    }

    Vector2f & operator -= ( const Vector2f & u )
    {
        x -= u.x;
        y -= u.y;
        return * this;
    }
 
    Vector2f & operator *= ( const Vector2f & u )
    {
        x *= u.x; y *= u.y; return * this;
    }

    Vector2f & operator /= ( const Vector2f & u )
    {
        x /= u.x; y /= u.y; return * this;
    }

    template <typename T> Vector2f & operator *= ( const T & t )
    {
        mul ( t, t ); return * this;
    }

    Vector2f & operator /= ( const float d )
    {
        x /= d;
        y /= d;
        return * this;
    }

    Vector2f & fill ( float d = 0 )
    {
        x = y = d;
        return * this;
    }

    bool operator ! () const
    {
        return !x && !y;
    }

    Vector2f operator - () const
    {
        return Vector2f ( - x, - y );
    }

    Vector2f leftPerpendicular () const
    {
        return Vector2f ( -y, x );
    }

    Vector2f rightPerpendicular () const
    {
        return Vector2f ( y, -x );
    }

// «адание векторных норм
    Vector2f & setNorm1 ( float p = 1 ); // единична€ норма
    Vector2f & setNorm2 ( float p = 1 ); // квадратична€ норма
    Vector2f & setNormU ( float p = 1 ); // бесконечна€ норма
};

inline Vector2f operator + ( const Vector2f & a, const Vector2f & b )
{
        return Vector2f ( a.x + b.x, a.y + b.y );
}

inline Vector2f operator - ( const Vector2f & a, const Vector2f & b )
{
    return Vector2f ( a.x - b.x, a.y - b.y );
}

inline Vector2f operator * ( const Vector2f & a, const float & d )
{
    return Vector2f ( a.x * d, a.y * d );
}

inline Vector2f operator * ( const float & d, const Vector2f & a )
{
    return Vector2f ( a.x * d, a.y * d );
}

inline Vector2f operator / ( const Vector2f & a, const float & d )
{
    return Vector2f ( a.x / d, a.y / d );
}

inline float operator * ( const Vector2f & a, const Vector2f & b )
{
    return a.x * b.x + a.y * b.y;
}

inline float qmod ( const Vector2f & a )
{
    return a.x * a.x + a.y * a.y;
}

inline float operator % ( const Vector2f & a, const Vector2f & b )
{
    return a.x * b.y - a.y * b.x;
}

inline bool operator != ( const Vector2f & a, const Vector2f & b )
{
    return a.x != b.x || a.y != b.y;
}

inline bool operator == ( const Vector2f & a, const Vector2f & b )
{
    return a.x == b.x && a.y == b.y;
}

float norm1 ( const Vector2f & v ); // единична€ норма
float norm2 ( const Vector2f & v ); // квадратична€ норма
float normU ( const Vector2f & v ); // бесконечна€ норма

//////////////////////////////// ќтрезок //////////////////////////////////////

class Segment2f
{
public:
    Vector2f a, b;

    Segment2f () {}
    Segment2f ( const Vector2f & va, const Vector2f & vb ) : a(va), b(vb) {}
};

inline bool operator != ( const Segment2f & s1, const Segment2f & s2 )
{
    return ( s1.a != s2.a ) || ( s1.b != s2.b );
}

inline bool operator == ( const Segment2f & s1, const Segment2f & s2 )
{
    return ( s1.a == s2.a ) && ( s1.b == s2.b );
}

inline float length ( const Segment2f & s )
{
    return norm2 ( s.b - s.a );
}

#endif