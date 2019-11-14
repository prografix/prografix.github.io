
#pragma once

#include "Template.h"

//////////////////////////////// Вектор ///////////////////////////////////////

class Vector2d
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
    double x, y;

    Vector2d () {}
    Vector2d ( double a, double b ) : x ( a ), y ( b ) {}
 
    Vector2d & operator += ( const Vector2d & u )
    {
        x += u.x; y += u.y; return * this;
    }

    Vector2d & operator -= ( const Vector2d & u )
    {
        x -= u.x; y -= u.y; return * this;
    }
 
    Vector2d & operator *= ( const Vector2d & u )
    {
        x *= u.x; y *= u.y; return * this;
    }

    Vector2d & operator /= ( const Vector2d & u )
    {
        x /= u.x; y /= u.y; return * this;
    }

    template <typename T> Vector2d & operator *= ( const T & t )
    {
        mul ( t, t ); return * this;
    }

    Vector2d & operator /= ( const double d )
    {
        x /= d; y /= d; return * this;
    }

    Vector2d & fill ( double d = 0 )
    {
        x = y = d; return * this;
    }

    bool operator ! () const
    {
        return !x && !y;
    }

    Vector2d operator - () const
    {
        return Vector2d ( -x, -y );
    }

    Vector2d leftPerpendicular () const
    {
        return Vector2d ( -y, x );
    }

    Vector2d rightPerpendicular () const
    {
        return Vector2d ( y, -x );
    }

// Задание векторных норм
    Vector2d & setNorm1 ( double p = 1 ); // единичная норма
    Vector2d & setNorm2 ( double p = 1 ); // квадратичная норма
    Vector2d & setNormU ( double p = 1 ); // бесконечная норма
};

const Vector2d null2d ( 0, 0 );

inline Vector2d operator + ( const Vector2d & a, const Vector2d & b )
{
    return Vector2d ( a.x + b.x, a.y + b.y );
}

inline Vector2d operator - ( const Vector2d & a, const Vector2d & b )
{
    return Vector2d ( a.x - b.x, a.y - b.y );
}

inline Vector2d operator * ( const Vector2d & a, const double & d )
{
    return Vector2d ( a.x * d, a.y * d );
}

inline Vector2d operator * ( const double & d, const Vector2d & a )
{
    return Vector2d ( a.x * d, a.y * d );
}

inline Vector2d operator / ( const Vector2d & a, const double & d )
{
    return Vector2d ( a.x / d, a.y / d );
}

inline double operator * ( const Vector2d & a, const Vector2d & b )
{
    return a.x * b.x + a.y * b.y;
}

inline double qmod ( const Vector2d & a )
{
        return a.x * a.x + a.y * a.y;
}

inline double operator % ( const Vector2d & a, const Vector2d & b )
{
    return a.x * b.y - a.y * b.x;
}

inline bool operator != ( const Vector2d & a, const Vector2d & b )
{
    return a.x != b.x || a.y != b.y;
}

inline bool operator == ( const Vector2d & a, const Vector2d & b )
{
    return a.x == b.x && a.y == b.y;
}

// Вычисление векторных норм
double norm1 ( const Vector2d & v ); // единичная норма
double norm2 ( const Vector2d & v ); // квадратичная норма
double normU ( const Vector2d & v ); // бесконечная норма

//////////////////////////////// Отрезок //////////////////////////////////////

class Segment2d
{
public:
    Vector2d a, b;

    Segment2d () {}
    Segment2d ( const Vector2d & va, const Vector2d & vb ) : a(va), b(vb) {}

    Def<Vector2d> project ( const Vector2d & p ) const; // проекция точки на отрезок

    Vector2d nearPoint ( const Vector2d & p ) const; // ближайшая точка отрезка к данной

    double qdis ( const Vector2d & p ) const // квадрат расстояния до точки p
    {
        return ::qmod ( p - nearPoint ( p ) );
    }
    double qmod () const { return ::qmod ( b - a ); } // квадрат длины отрезка

    template <typename T> Segment2d & operator *= ( const T & t )
    {
        a *= t, b *= t;
        return * this;
    }
};

inline bool operator != ( const Segment2d & s1, const Segment2d & s2 )
{
    return s1.a != s2.a || s1.b != s2.b;
}

inline bool operator == ( const Segment2d & s1, const Segment2d & s2 )
{
    return s1.a == s2.a && s1.b == s2.b;
}

// Вычисление норм
inline double norm1 ( const Segment2d & s ) { return norm1 ( s.b - s.a ); } // единичная норма
inline double norm2 ( const Segment2d & s ) { return norm2 ( s.b - s.a ); } // квадратичная норма
inline double normU ( const Segment2d & s ) { return normU ( s.b - s.a ); } // бесконечная норма

//////////////////////////////// Прямая ///////////////////////////////////////

class Line2d
{
public:
    Vector2d norm;
    double  dist;

    Line2d () {}
    Line2d ( const Vector2d & v, const double & d ) : norm ( v ), dist ( d ) {}
    Line2d ( const Vector2d & a, const Vector2d & b );

    double operator % ( const Vector2d & v ) const
    {
        return norm.x * v.x + norm.y * v.y + dist;
    }

    Line2d operator - () const
    {
        return Line2d ( - norm, - dist );
    }

    template<class T> Line2d & operator *= ( const T & t )
    {
        return *this = t ( *this );
    }

    Vector2d project ( const Vector2d & v ) const
    {
        return v - ( *this % v ) * norm;
    }

    Segment2d project ( const Segment2d & s ) const
    {
        return Segment2d ( project ( s.a ), project ( s.b ) );
    }

    Vector2d mirror ( const Vector2d & v ) const
    {
        return v - 2 * ( *this % v ) * norm;
    }

    Segment2d mirror ( const Segment2d & s ) const
    {
        return Segment2d ( mirror ( s.a ), mirror ( s.b ) );
    }
};

inline bool operator != ( const Line2d & a, const Line2d & b )
{
    return a.norm != b.norm || a.dist != b.dist;
}

inline bool operator == ( const Line2d & a, const Line2d & b )
{
    return a.norm == b.norm && a.dist == b.dist;
}

inline double operator % ( const Vector2d & a, const Line2d & b )
{
    return b.norm.x * a.x + b.norm.y * a.y + b.dist;
}

///////////////////////////////// Поворот /////////////////////////////////////

class Spin2d
{
    double x, y;

    Spin2d ( double a, double b ) : x(a), y(b) {}
public:
    Spin2d () : x(1.), y(0.) {}

    explicit Spin2d ( double a, bool degree = false );

    Spin2d ( const Vector2d & a, const Vector2d & b );

    double getAngle () const;

    void getReper ( Vector2d & ax, Vector2d & ay ) const
    {
        ax.x =  x; ax.y = y;
        ay.x = -y; ay.y = x;
    }

    Vector2d operator () ( const Vector2d & v ) const
    {
        return Vector2d ( v.x * x + v.y * y, v.y * x - v.x * y );
    }

    Spin2d operator ~ () const
    {
        return Spin2d ( x, -y );
    }

    friend Spin2d operator * ( const Spin2d & l, const Spin2d & r );

    static const Spin2d d030;
    static const Spin2d d045;
    static const Spin2d d060;
    static const Spin2d d090;
    static const Spin2d d120;
    static const Spin2d d135;
    static const Spin2d d150;
    static const Spin2d d180;
    static const Spin2d d210;
    static const Spin2d d225;
    static const Spin2d d240;
    static const Spin2d d270;
    static const Spin2d d300;
    static const Spin2d d315;
    static const Spin2d d330;
};

////////////////////////////////// Подобие ////////////////////////////////////

class Conform2d
{
public:
    double   magn;
    Spin2d   spin;
    Vector2d trans;

    Conform2d () : trans (0.,0.), magn (1.) {}

    explicit Conform2d ( const Spin2d & s ) : spin ( s ), trans (0.,0.), magn (1.) {}

    Conform2d ( const Spin2d & s, const Vector2d & t, double m = 1. ) : spin(s), trans(t), magn(m) {}

    Def<Conform2d> operator~ () const
    {
        Def<Conform2d> res;
        if ( magn != 0 )
        {
            res.spin = ~spin;
            res.magn = 1. / magn;
            res.trans = (-res.magn) * res.spin ( trans );
            res.isDef = true;
        }
        return res;
    }

    Line2d operator() ( const Line2d & p ) const
    {
        const Vector2d v = spin ( p.norm );
        return Line2d ( v, magn * p.dist - v * trans );
    }

    Vector2d operator() ( const Vector2d & v ) const
    {
        return spin ( v ) * magn + trans;
    }

    Segment2d operator () ( const Segment2d & p ) const
    {
        return Segment2d ( (*this) ( p.a ), (*this) ( p.b ) );
    }
};

inline Conform2d operator * ( const Conform2d & l, const Conform2d & r )
{
    return Conform2d ( l.spin * r.spin, l ( r.trans ), l.magn * r.magn );
}

////////////////////////// Линейное преобразование ////////////////////////////

class LinTran2d
{
public:
    Vector2d x, y;

    LinTran2d () {}

    LinTran2d ( const Vector2d & a, const Vector2d & b ) : x ( a ), y ( b ) {}
    
    explicit LinTran2d ( const Spin2d & spin )
    {
        spin.getReper ( x, y );
    }

    Vector2d operator () ( const Vector2d & p ) const
    {
        return Vector2d ( x * p, y * p );
    }

    Segment2d operator () ( const Segment2d & p ) const
    {
        return Segment2d ( (*this) ( p.a ), (*this) ( p.b ) );
    }

    LinTran2d & operator *= ( double d )
    {
        x *= d; y *= d; return *this;
    }

    Def<LinTran2d> operator ~ () const
    {
        Def<LinTran2d> res;
        double d = x % y;
        if ( d != 0 )
        {
            d = 1 / d;
            res.x.x =   y.y * d;
            res.x.y = - x.y * d;
            res.y.x = - y.x * d;
            res.y.y =   x.x * d;
            res.isDef = true;
        }
        return res;
    }

    friend inline LinTran2d operator * ( const LinTran2d & l, const LinTran2d & r )
    {
        return LinTran2d ( l.x.x * r.x + l.x.y * r.y, l.y.x * r.x + l.y.y * r.y );
    }
};

////////////////////////// Аффинное преобразование ////////////////////////////

class Affin2d
{
public:
    LinTran2d t;
    Vector2d s;

    Affin2d () {}

    Affin2d ( const Vector2d & a, const Vector2d & b, const Vector2d & c ) : t ( a, b ), s ( c ) {}

    Affin2d ( const LinTran2d & a, const Vector2d & b ) : t ( a ), s ( b ) {}

    explicit Affin2d ( const Spin2d & spin ) : t ( spin ), s ( 0., 0. ) {}

    explicit Affin2d ( const Conform2d & conf ) : t ( conf.spin ), s ( conf.trans )
    {
        t *= conf.magn;
    }

    Vector2d operator () ( const Vector2d & p ) const
    {
        return t ( p ) + s;
    }

    Segment2d operator () ( const Segment2d & p ) const
    {
        return Segment2d ( (*this) ( p.a ), (*this) ( p.b ) );
    }

    Def<Affin2d> operator ~ () const
    {
        const Def<LinTran2d> lin ( ~t );
        return lin.isDef ? Affin2d ( lin, lin ( -s ) ) : Def<Affin2d>();
    }

    friend inline Affin2d operator * ( const Affin2d & l, const Affin2d & r )
    {
        return Affin2d ( l.t * r.t, l ( r.s ) );
    }
};

///////////////////////////////// Круг ////////////////////////////////////////

class Circle2d
{
public:
    Vector2d o; // центр
    double   r; // радиус

    Circle2d () {}
    Circle2d ( const Vector2d & a, double b ) : o ( a ), r ( b ) {}

    Circle2d & operator *= ( const Conform2d & c )
    {
        o *= c; r *= c.magn; return *this;
    }
    // Площадь
    double getArea () const;
    // Периметр
    double getPerimeter () const;
};

////////////////////////////////// Эллипс /////////////////////////////////////

class Ellipse2d
{
public:
    Vector2d o;  // центр
    double a, b; // полуоси
    Spin2d spin; // поворот
    // Преобразование окружности в эллипс
    Affin2d getAffin2d() const;
    // Вычисление фокусов
    void getFoci ( Vector2d & f1, Vector2d & f2 ) const;
    // Площадь
    double getArea () const;
    // Периметр
    double getPerimeter () const;
};

///////////////////////////// Треугольник /////////////////////////////////////

class Triangle2d
{
public:
    Vector2d a, b, c;

    Triangle2d () {}
    Triangle2d ( const Vector2d & pa, const Vector2d & pb, const Vector2d & pc ) : a ( pa ), b ( pb ), c ( pc ) {}
    // Площадь
    double getArea () const;
    // Периметр
    double getPerimeter () const;
};

/////////////////////////////// Прямоугольник /////////////////////////////////

class Rectangle2d
{
public:
    Vector2d o;  // центр
    double a, b; // полуоси
    Spin2d spin; // поворот
    // Площадь
    double getArea () const;
    // Периметр
    double getPerimeter () const;
};

/////////////////////////////////// Ромб //////////////////////////////////////

class Rhombus2d
{
public:
    Vector2d o;  // центр
    double a, b; // полуоси
    Spin2d spin; // поворот
    // Площадь
    double getArea () const;
    // Периметр
    double getPerimeter () const;
};

////////////////////////////// Параллелограмм /////////////////////////////////

class Parallelogram2d
{
    Vector2d o, a, b; 
public:
    Parallelogram2d () {}
    Parallelogram2d ( const Vector2d & v1, const Vector2d & v2, const Vector2d & v3 ) :
        o ( v2 ), a ( v3 - v2 ), b ( v1 - v2 ) {}
    // Вершины
    void getVerts ( Vector2d vert[4] ) const;
    // Площадь
    double getArea () const;
    // Периметр
    double getPerimeter () const;
};

