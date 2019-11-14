
#ifndef VECTOR2I_H
#define VECTOR2I_H

class Vector2i
{
public :
    int x, y;
    Vector2i () {}
    Vector2i ( int i, int j ) : x ( i ), y ( j ) {}

    Vector2i & operator += ( const Vector2i & u )
    {
        x += u.x;
        y += u.y;
        return * this;
    }

    Vector2i & operator -= ( const Vector2i & u )
    {
        x -= u.x;
        y -= u.y;
        return * this;
    }

    Vector2i & operator *= ( int i )
    {
        x *= i;
        y *= i;
        return * this;
    }

    Vector2i & operator /= ( int i )
    {
        x /= i;
        y /= i;
        return * this;
    }

    Vector2i & fill ( int d = 0 )
    {
        x = y = d;
        return * this;
    }

    bool operator ! () const
    {
        return !x && !y;
    }

    Vector2i operator - () const
    {
        return Vector2i ( -x, -y );
    }

    Vector2i leftPerpendicular () const
    {
        return Vector2i ( -y, x );
    }

    Vector2i rightPerpendicular () const
    {
        return Vector2i ( y, -x );
    }
};

inline Vector2i operator + ( const Vector2i& a, const Vector2i& b )
{
    return Vector2i ( a.x + b.x, a.y + b.y );
}

inline Vector2i operator - ( const Vector2i& a, const Vector2i& b )
{
    return Vector2i ( a.x - b.x, a.y - b.y );
}

inline Vector2i operator * ( const Vector2i& a, int i )
{
    return Vector2i ( a.x * i, a.y * i );
}

inline Vector2i operator * ( int i, const Vector2i& a )
{
    return Vector2i ( a.x * i, a.y * i );
}

inline Vector2i operator / ( const Vector2i& a, int i )
{
    return Vector2i ( a.x / i, a.y / i );
}

inline int operator * ( const Vector2i& a, const Vector2i& b )
{
    return a.x * b.x + a.y * b.y;
}

inline int qmod ( const Vector2i& a )
{
    return a.x * a.x + a.y * a.y;
}

inline int operator % ( const Vector2i& a, const Vector2i& b )
{
    return a.x * b.y - a.y * b.x;
}

inline bool operator != ( const Vector2i& a, const Vector2i& b )
{
    return ( a.x != b.x ) || ( a.y != b.y );
}

inline bool operator == ( const Vector2i& a, const Vector2i& b )
{
    return ( a.x == b.x ) && ( a.y == b.y );
}

#endif
