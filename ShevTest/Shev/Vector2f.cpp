
#include "math.h"
#include "Template.h"
#include "vector2f.h"

// Задание векторных норм

Vector2f & Vector2f::setNorm1 ( float p ) // единичная норма
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

Vector2f & Vector2f::setNorm2 ( float p ) // квадратичная норма
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

Vector2f & Vector2f::setNormU ( float p ) // бесконечная норма
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

// Вычисление векторных норм

float norm1 ( const Vector2f & v ) // единичная норма
{
    return fabsf ( v.x ) + fabsf ( v.y );
}

float norm2 ( const Vector2f & v ) // квадратичная норма
{
    return sqrtf ( v.x * v.x + v.y * v.y );
}

float normU ( const Vector2f & v ) // бесконечная норма
{
    return _max ( fabsf ( v.x ), fabsf ( v.y ) );
}