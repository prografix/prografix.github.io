
#include "math.h"
#include "tune.h"
#include "Template.h"
#include "Vector2d.h"

//////////////////////////////// Вектор ///////////////////////////////////////

// Задание векторных норм

Vector2d & Vector2d::setNorm1 ( double p ) // единичная норма
{
    double t = fabs ( x ) + fabs ( y );
    if ( t > 0 )
    {
        p /= t;
        x *= p;
        y *= p;
    }
    return * this;
}

Vector2d & Vector2d::setNorm2 ( double p ) // квадратичная норма
{
    double t = x * x + y * y;
    if ( t > 0 )
    {
        p /= sqrt ( t );
        x *= p;
        y *= p;
    }
    return * this;
}

Vector2d & Vector2d::setNormU ( double p ) // бесконечная норма
{
    double t = _max ( fabs ( x ), fabs ( y ) );
    if ( t > 0 )
    {
        p /= t;
        x *= p;
        y *= p;
    }
    return * this;
}

// Вычисление векторных норм

double norm1 ( const Vector2d & v ) // единичная норма
{
    return fabs ( v.x ) + fabs ( v.y );
}

double norm2 ( const Vector2d & v ) // квадратичная норма
{
    return sqrt ( v.x * v.x + v.y * v.y );
}

double normU ( const Vector2d & v ) // бесконечная норма
{
    return _max ( fabs ( v.x ), fabs ( v.y ) );
}

//////////////////////////////// Отрезок //////////////////////////////////////

Def<Vector2d> Segment2d::project ( const Vector2d & p ) const // проекция точки на отрезок
{
    const Vector2d u = b - a;
    const Vector2d w = p - a;
    const double s = u * w;
    if ( s < 0 ) return Def<Vector2d>();
    const double q = u * u;
    if ( s > q ) return Def<Vector2d>();
    return q > 0 ? a + u * ( s / q ) : a;
}

Vector2d Segment2d::nearPoint ( const Vector2d & p ) const // ближайшая точка отрезка к данной
{
    const Vector2d u = b - a;
    const Vector2d w = p - a;
    const double s = u * w;
    if ( s <= 0 ) return a;
    const double q = u * u;
    return s >= q ? b : a + u * ( s / q );
}

//////////////////////////////// Прямая ///////////////////////////////////////

Line2d::Line2d ( const Vector2d & a, const Vector2d & b )
{
    norm.x = b.y - a.y;
    norm.y = a.x - b.x;
    const double t = norm.x * norm.x + norm.y * norm.y;
    if ( t )
    {
        const double d = sqrt ( t );
        norm.x /= d;
        norm.y /= d;
    }
    else
    {
        norm.x = 1.;
        norm.y = 0.;
    }
    dist = - ( norm * a );
}

///////////////////////////////// Поворот /////////////////////////////////////

Spin2d::Spin2d ( double a, bool degree )
{
    if ( degree ) a *= 3.14159265358979323846 / 180.;
    x =   cos ( a );
    y = - sin ( a );
}

Spin2d::Spin2d ( const Vector2d & a, const Vector2d & b )
{
    x = a * b;
    y = b % a;
    const double q = sqrt ( x * x + y * y );
    if ( q > 0 )
    {
        x /= q;
        y /= q;
    }
    else
    {
        x = 1;
        y = 0;
    }
}

double Spin2d::getAngle () const
{
    return atan2 ( -y, x );
}

Spin2d operator * ( const Spin2d & l, const Spin2d & r )
{
    Spin2d s ( l.x * r.x - l.y * r.y, l.x * r.y + l.y * r.x );
    double d = s.x * s.x + s.y * s.y;
    if ( fabs ( d - 1. ) > 1e-14 )
    {
        d = sqrt ( d );
        s.x /= d;
        s.y /= d;
    }
    return s;
}

#define R2H 0.70710678118654752440 // sqrt ( 2 ) / 2
#define R3H 0.86602540378443864676 // sqrt ( 3 ) / 2

const Spin2d Spin2d::d030 ( R3H,-0.5 );
const Spin2d Spin2d::d045 ( R2H,-R2H );
const Spin2d Spin2d::d060 ( 0.5,-R3H );
const Spin2d Spin2d::d090 ( 0.0,-1.0 );
const Spin2d Spin2d::d120 (-0.5,-R3H );
const Spin2d Spin2d::d135 (-R2H,-R2H );
const Spin2d Spin2d::d150 (-R3H,-0.5 );
const Spin2d Spin2d::d180 (-1.0, 0.0 );
const Spin2d Spin2d::d210 (-R3H, 0.5 );
const Spin2d Spin2d::d225 (-R2H, R2H );
const Spin2d Spin2d::d240 (-0.5, R3H );
const Spin2d Spin2d::d270 ( 0.0, 1.0 );
const Spin2d Spin2d::d300 ( 0.5, R3H );
const Spin2d Spin2d::d315 ( R2H, R2H );
const Spin2d Spin2d::d330 ( R3H, 0.5 );

///////////////////////////////// Круг ////////////////////////////////////////

// Площадь
double Circle2d::getArea () const
{
    return r * r * M_PI;
}

// Периметр
double Circle2d::getPerimeter () const
{
    return 2 * M_PI * r;
}

////////////////////////////////// Эллипс /////////////////////////////////////

// Площадь
double Ellipse2d::getArea () const
{
    return a * b * M_PI;
}

// Периметр
double Ellipse2d::getPerimeter () const
{
    const double c = 3 * _pow2 ( ( a - b ) / ( a + b ) );
    return M_PI * ( a + b ) * ( 1 + c / ( 10 + sqrt ( 4 - c ) ) );
}

// Преобразование окружности в эллипс
Affin2d Ellipse2d::getAffin2d() const
{
    LinTran2d t ( spin );
    t.x.x *= a; t.x.y *= b; 
    t.y.x *= a; t.y.y *= b; 
    return Affin2d ( t, o );
}

// Вычисление фокусов
void Ellipse2d::getFoci ( Vector2d & f1, Vector2d & f2 ) const
{
    if ( a > b )
    {
        f1.x = sqrt ( a * a - b * b );
        f2.x = - f1.x;
        f1.y = f2.y = 0;
    }
    else
    {
        f1.y = sqrt ( b * b - a * a );
        f2.y = - f1.y;
        f1.x = f2.x = 0;
    }
    ( f1 *= spin ) += o;
    ( f2 *= spin ) += o;
}


///////////////////////////// Треугольник /////////////////////////////////////

// Площадь
double Triangle2d ::getArea () const
{
    return 0.5 * ( a - b ) % ( b - c );
}

// Периметр
double Triangle2d::getPerimeter () const
{
    return norm2 ( a - b ) + norm2 ( b - c ) + norm2 ( c - a );
}

/////////////////////////////// Прямоугольник /////////////////////////////////

// Площадь
double Rectangle2d::getArea () const
{
    return a * b * 4;
}

// Периметр
double Rectangle2d::getPerimeter () const
{
    return 4 * ( a + b );
}

/////////////////////////////////// Ромб //////////////////////////////////////

// Площадь
double Rhombus2d::getArea () const
{
    return a * b * 2;
}

// Периметр
double Rhombus2d::getPerimeter () const
{
    return 4 * sqrt ( a * a + b * b );
}

////////////////////////////// Параллелограмм /////////////////////////////////

// Вершины
void Parallelogram2d::getVerts ( Vector2d vert[4] ) const
{
    vert[0] = o + b;
    vert[1] = o;
    vert[2] = o + a;
    vert[3] = o + a + b;
}

// Площадь
double Parallelogram2d::getArea () const
{
    return a % b;
}

// Периметр
double Parallelogram2d::getPerimeter () const
{
    return 2 * ( norm2 ( a ) + norm2 ( b ) );
}

