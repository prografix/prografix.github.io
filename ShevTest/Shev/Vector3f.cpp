
#include "math.h"
#include "Template.h"
#include "Vector3f.h"

//////////////////////////////// ¬ектор ///////////////////////////////////////

// ѕолучение перпендикул€ра к данному вектору

Vector3f Vector3f::perpendicular () const
{
    float ax = fabsf ( x );
    float ay = fabsf ( y );
    float az = fabsf ( z );
    if ( ax >= ay )
    {
        if ( ax >= az )
        {
            if ( ! ax ) return Vector3f (1.f,0.f,0.f);
            float d = 1.f / sqrtf ( ax * ax + ay * ay );
            return Vector3f (-y*d, x*d, 0.f );
        }
    }
    else
    {
        if ( ay >= az )
        {
            float d = 1.f / sqrtf ( ay * ay + az * az );
            return Vector3f ( 0.f,-z*d, y*d );
        }
    }
    float d = 1.f / sqrtf ( ax * ax + az * az );
    return Vector3f ( z*d, 0.f,-x*d );
}

// «адание векторных норм

Vector3f & Vector3f::setNorm1 ( float p ) // единична€ норма
{
    const float t = fabsf ( x ) + fabsf ( y ) + fabsf ( z );
    if ( t > 0 )
    {
        p /= t;
        x *= p;
        y *= p;
        z *= p;
    }
    return * this;
}

Vector3f & Vector3f::setNorm2 ( float p ) // квадратична€ норма
{
    const float t = x * x + y * y + z * z;
    if ( t > 0 )
    {
        p /= sqrtf ( t );
        x *= p;
        y *= p;
        z *= p;
    }
    return * this;
}

Vector3f & Vector3f::setNormU ( float p ) // бесконечна€ норма
{
    const float t = _max ( fabsf ( x ), fabsf ( y ), fabsf ( z ) );
    if ( t > 0 )
    {
        p /= t;
        x *= p;
        y *= p;
        z *= p;
    }
    return * this;
}

float norm1 ( const Vector3f & v ) // единична€ норма
{
    return fabsf ( v.x ) + fabsf ( v.y ) + fabsf ( v.z );
}

float norm2 ( const Vector3f & v ) // квадратична€ норма
{
    return sqrtf ( v.x * v.x + v.y * v.y + v.z * v.z );
}

float normU ( const Vector3f & v ) // бесконечна€ норма
{
    return _max ( fabsf ( v.x ), fabsf ( v.y ), fabsf ( v.z ) );
}

///////////////////////////// ѕлоскость ///////////////////////////////////////

Plane3f::Plane3f ( const Vector3f & x, const Vector3f & y, const Vector3f & z )
{
    Vector3f v = x;
    v += y;
    v += z;
    v /= 3.f;
    norm = x % y + y % z + z % x;
    if ( ! norm )
    {
        Vector3f xy = x - y;
        Vector3f yz = y - z;
        Vector3f zx = z - x;
        float d1 = qmod ( xy );
        float d2 = qmod ( yz );
        float d3 = qmod ( zx );
        if ( d1 >= d2 )
            if ( d1 >= d3 ) norm = xy.perpendicular();
            else            norm = zx.perpendicular();
        else
            if ( d2 >= d3 ) norm = yz.perpendicular();
            else            norm = zx.perpendicular();
    }
    else
        norm.setNorm2 ();
    dist = - ( v * norm );
}