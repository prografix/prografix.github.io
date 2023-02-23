//
//  Файл создан Шевченко А.Д.
//  Просьба без согласования не изменять
//

class Vector3i
{
public :
    int x, y, z;
    Vector3i () {}
    Vector3i ( int i, int j, int k ) : x ( i ), y ( j ), z ( k ) {}

    Vector3i & operator += ( const Vector3i & v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return * this;
    }

    Vector3i & operator -= ( const Vector3i & v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return * this;
    }

    Vector3i & operator *= ( int i )
    {
        x *= i;
        y *= i;
        z *= i;
        return * this;
    }

    Vector3i & operator /= ( int i )
    {
        x /= i;
        y /= i;
        z /= i;
        return * this;
    }

    Vector3i & null ()
    {
        x = y = z = 0;
        return * this;
    }

    int operator ! () const
    {
        return !x && !y && !z;
    }
};

inline Vector3i operator + ( const Vector3i& a, const Vector3i& b )
{
    return Vector3i ( a.x + b.x, a.y + b.y, a.z + b.z );
}

inline Vector3i operator - ( const Vector3i& a, const Vector3i& b )
{
    return Vector3i ( a.x - b.x, a.y - b.y, a.z - b.z );
}
 
inline Vector3i operator * ( const Vector3i& a, int i )
{
    return Vector3i ( a.x * i, a.y * i, a.z * i );
}

inline Vector3i operator * ( int i, const Vector3i& a )
{
    return Vector3i ( a.x * i, a.y * i, a.z * i );
}

inline int operator * ( const Vector3i& a, const Vector3i& b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline int qmod ( const Vector3i& a )
{
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

inline Vector3i operator % ( const Vector3i& a, const Vector3i& b )
{
    return Vector3i ( a.y * b.z - a.z * b.y,
                      a.z * b.x - a.x * b.z,
                      a.x * b.y - a.y * b.x );
}

inline int operator != ( const Vector3i& a, const Vector3i& b )
{
    return ( a.x != b.x ) || ( a.y != b.y ) || ( a.z != b.z );
}

inline int operator == ( const Vector3i& a, const Vector3i& b )
{
    return ( a.x == b.x ) && ( a.y == b.y ) && ( a.z == b.z );
}
