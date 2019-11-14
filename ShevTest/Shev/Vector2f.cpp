
#include "math.h"
#include "Template.h"
#include "vector2f.h"

// ������� ��������� ����

Vector2f & Vector2f::setNorm1 ( float p ) // ��������� �����
{
    const float t = fabsf ( x ) + fabsf ( y );
    if ( t > 0 )
    {
        p /= t;
        x *= p;
        y *= p;
    }
    return * this;
}

Vector2f & Vector2f::setNorm2 ( float p ) // ������������ �����
{
    const float t = x * x + y * y;
    if ( t > 0 )
    {
        p /= sqrtf ( t );
        x *= p;
        y *= p;
    }
    return * this;
}

Vector2f & Vector2f::setNormU ( float p ) // ����������� �����
{
    const float t = _max ( fabsf ( x ), fabsf ( y ) );
    if ( t > 0 )
    {
        p /= t;
        x *= p;
        y *= p;
    }
    return * this;
}

// ���������� ��������� ����

float norm1 ( const Vector2f & v ) // ��������� �����
{
    return fabsf ( v.x ) + fabsf ( v.y );
}

float norm2 ( const Vector2f & v ) // ������������ �����
{
    return sqrtf ( v.x * v.x + v.y * v.y );
}

float normU ( const Vector2f & v ) // ����������� �����
{
    return _max ( fabsf ( v.x ), fabsf ( v.y ) );
}