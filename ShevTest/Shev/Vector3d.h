
#pragma once

#include "Template.h"

//////////////////////////////// Вектор ///////////////////////////////////////

class Vector3d
{
    template <typename T> void mul ( const T & t, ... )
    {
        *this = t ( *this );
    }

    template <typename T> void mul ( const T & t, double d )
    {
        x *= d;
        y *= d;
        z *= d;
    }

    template <typename T> void mul ( const T & t, const Vector3d & v )
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }
public :
    double x, y, z;

    Vector3d () {}
    Vector3d ( double a, double b, double c ) : x ( a ), y ( b ), z ( c ) {}
 
    Vector3d operator - () const
    {
        return Vector3d ( - x, - y, - z );
    }
 
    Vector3d & operator += ( const Vector3d & v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return * this;
    }

    Vector3d & operator -= ( const Vector3d & v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return * this;
    }

    Vector3d & operator /= ( const Vector3d & v )
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return * this;
    }

    template <typename T> Vector3d & operator *= ( const T & t )
    {
        mul ( t, t );
        return * this;
    }

    Vector3d & operator /= ( const double d )
    {
        x /= d;
        y /= d;
        z /= d;
        return * this;
    }

    Vector3d & fill ( double d = 0 )
    {
        x = y = z = d;
        return * this;
    }

    bool operator ! () const
    {
        return !x && !y && !z;
    }

// Получение перпендикуляра к данному вектору
    Vector3d perpendicular () const;

// Задание векторных норм
    Vector3d & setNorm1 ( double p = 1 ); // единичная норма
    Vector3d & setNorm2 ( double p = 1 ); // квадратичная норма
    Vector3d & setNormU ( double p = 1 ); // бесконечная норма
};

const Vector3d null3d ( 0, 0, 0 );

inline Vector3d operator + ( const Vector3d& a, const Vector3d& b )
{
    return Vector3d ( a.x + b.x, a.y + b.y, a.z + b.z );
}

inline Vector3d operator - ( const Vector3d& a, const Vector3d& b )
{
    return Vector3d ( a.x - b.x, a.y - b.y, a.z - b.z );
}
 
inline Vector3d operator * ( const Vector3d& a, double d )
{
    return Vector3d ( a.x * d, a.y * d, a.z * d );
}
 
inline Vector3d operator / ( const Vector3d& a, double d )
{
    return Vector3d ( a.x / d, a.y / d, a.z / d );
}

inline Vector3d operator * ( double d, const Vector3d& a )
{
    return Vector3d ( a.x * d, a.y * d, a.z * d );
}

inline double operator * ( const Vector3d& a, const Vector3d& b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double qmod ( const Vector3d& a )
{
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

inline Vector3d operator % ( const Vector3d& a, const Vector3d& b )
{
    return Vector3d ( a.y * b.z - a.z * b.y,
                      a.z * b.x - a.x * b.z,
                      a.x * b.y - a.y * b.x );
}

inline bool operator != ( const Vector3d & a, const Vector3d & b )
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline bool operator == ( const Vector3d & a, const Vector3d & b )
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline void reper ( const Vector3d & x, Vector3d & y, Vector3d & z )
{
    y = x.perpendicular ();
    z = x % y;
}

// Вычисление векторных норм

double norm1 ( const Vector3d & v ); // единичная норма
double norm2 ( const Vector3d & v ); // квадратичная норма
double normU ( const Vector3d & v ); // бесконечная норма

//////////////////////////////// Отрезок //////////////////////////////////////

class Segment3d
{
public:
    Vector3d a, b;

    Segment3d () {}
    Segment3d ( const Vector3d & va, const Vector3d & vb ) : a(va), b(vb) {}

    Def<Vector3d> project ( const Vector3d & p ) const; // проекция точки на отрезок

    Vector3d nearPoint ( const Vector3d & p ) const; // ближайшая точка отрезка к данной

    double qdis ( const Vector3d & p ) const // квадрат расстояния до точки p
    {
        return ::qmod ( p - nearPoint ( p ) );
    }
    double qmod () const { return ::qmod ( b - a ); } // квадрат длины отрезка

    template <typename T> Segment3d & operator *= ( const T & t )
    {
        a *= t, b *= t;
        return * this;
    }
};

inline bool operator != ( const Segment3d & s1, const Segment3d & s2 )
{
    return ( s1.a != s2.a ) || ( s1.b != s2.b );
}

inline bool operator == ( const Segment3d & s1, const Segment3d & s2 )
{
    return ( s1.a == s2.a ) && ( s1.b == s2.b );
}

// Вычисление норм

inline double norm1 ( const Segment3d & s ) { return norm1 ( s.b - s.a ); } // единичная норма
inline double norm2 ( const Segment3d & s ) { return norm2 ( s.b - s.a ); } // квадратичная норма
inline double normU ( const Segment3d & s ) { return normU ( s.b - s.a ); } // бесконечная норма

///////////////////////////////// Прямая //////////////////////////////////////

class Line3d
{
public:
    Vector3d dir;
    Vector3d point;

    Line3d () {}
    Line3d ( const Vector3d & a, const Vector3d & b ) : dir(a), point(b) {}

    Vector3d project ( const Vector3d & p ) const // проекция точки p на прямую
    {
        return point + dir * ( dir * ( p - point ) );
    }

    double qdis ( const Vector3d & p ) const // квадрат расстояния до точки p
    {
        return qmod ( p - project ( p ) );
    }
};

///////////////////////////// Плоскость ///////////////////////////////////////

class Plane3d
{
    template <typename T> void mul ( const T & t, ... )
    {
        *this = t ( *this );
    }

    template <typename T> void mul ( const T & t, double d )
    {
        dist *= d;
    }
public:
    Vector3d norm;
    double   dist;

    Plane3d () {}
    Plane3d ( const Vector3d &, const Vector3d &, const Vector3d & );
    Plane3d ( const Vector3d & v, const double & d ) : norm ( v ), dist ( d ) {}

    double operator % ( const Vector3d & v ) const
    {
        return norm.x * v.x + norm.y * v.y + norm.z * v.z + dist;
    }

    Plane3d operator - () const
    {
        return Plane3d ( - norm, - dist );
    }

    template <typename T> Plane3d & operator *= ( const T & t )
    {
        mul ( t, t );
        return *this;
    }

    Plane3d & operator += ( const Vector3d & p )
    {
        dist -= norm * p;
        return *this;
    }

    Plane3d & operator -= ( const Vector3d & p )
    {
        dist += norm * p;
        return *this;
    }

    Plane3d & setNorm2 ( double p = 1 )
    {
        if ( !!norm )
        {
            double t = norm2 ( norm );
            t = p / t;
            norm *= t;
            dist *= t;
        }
        return *this;
    }

    Vector3d project ( const Vector3d & v ) const; // Проекция точки на плоскость

    Segment3d project ( const Segment3d & s ) const // Проекция отрезка на плоскость
    {
        return Segment3d ( project ( s.a ), project ( s.b ) );
    }

    Vector3d mirror ( const Vector3d & v ) const; // Отражение точки относительно плоскости
    Plane3d  mirror ( const Plane3d  & p ) const; // Отражение плоскости
};

inline bool operator == ( const Plane3d & p1, const Plane3d & p2 )
{
    return p1.norm == p2.norm && p1.dist == p2.dist;
}

inline bool operator != ( const Plane3d & p1, const Plane3d & p2 )
{
    return p1.norm != p2.norm || p1.dist != p2.dist;
}

inline double operator % ( const Vector3d & a, const Plane3d & b )
{
    return b.norm.x * a.x + b.norm.y * a.y + b.norm.z * a.z + b.dist;
}

///////////////////////////////// Поворот /////////////////////////////////////

class Spin3d
{
    double t, x, y, z;
public:
    Spin3d () { t = 1.; x = y = z = 0.; }
    Spin3d ( Vector3d, double );
    Spin3d ( Vector3d v1, Vector3d v2 );
    Spin3d ( const Vector3d & x, const Vector3d & y, const Vector3d & z );
    Spin3d ( const double & a, const double & b, const double & c, const double & d );

    void getReper ( Vector3d & ax, Vector3d & ay, Vector3d & az ) const;
    Vector3d rotX () const;
    Vector3d rotY () const;
    Vector3d rotZ () const;
    double getAngle () const;
    Vector3d getAxis () const;

    Spin3d operator ~ () const
    {
        Spin3d s = *this;
        s.t = - t;
        return s;
    }

    friend Spin3d operator * ( const Spin3d & l, const Spin3d & r );
};

//////////////////////// Ортогональное преобразование /////////////////////////

class LinTran3d;

class Ortho3d
{
    Vector3d x, y, z;
public:
    Ortho3d () : x(1,0,0), y(0,1,0), z(0,0,1) {}

    explicit Ortho3d ( const Spin3d & spin )
    {
        spin.getReper ( x, y, z );
    }

    Ortho3d ( Vector3d a, double b );

    double getX ( const Vector3d & p ) const { return x * p; }
    double getY ( const Vector3d & p ) const { return y * p; }
    double getZ ( const Vector3d & p ) const { return z * p; }

    Vector3d operator () ( const Vector3d & p ) const
    {
        return Vector3d ( x * p, y * p, z * p );
    }

    Segment3d operator () ( const Segment3d & p ) const
    {
        return Segment3d ( (*this) ( p.a ), (*this) ( p.b ) );
    }

    Plane3d operator() ( const Plane3d & p ) const
    {
        return Plane3d ( Vector3d ( x * p.norm, y * p.norm, z * p.norm ), p.dist );
    }

    const LinTran3d & operator* () const
    {
        return *(const LinTran3d *) this;
    }

    Ortho3d operator ~ () const;
};

/////////////////////////// Подобие ( вариант 1 ) /////////////////////////////

class Conform3d
{
public:
    Spin3d   spin;
    Vector3d trans;
    double   magn;

    explicit Conform3d ( const Spin3d & s, double m = 1 ) : spin ( s ), trans (0.,0.,0.), magn (m) {}
    explicit Conform3d ( const Vector3d & t, double m = 1 ) : trans (t), magn (m) {}
    explicit Conform3d ( double m = 1 ) : trans(0.,0.,0.), magn(m) {}

    Conform3d ( const Spin3d & s, const Vector3d & t, double m = 1. ) : spin(s), trans(t), magn(m) {}

    Def<Conform3d> operator~ () const
    {
        Def<Conform3d> res;
        if ( magn != 0 )
        {
            res.spin = ~spin;
            res.magn = 1. / magn;
            res.trans = (-res.magn) * Ortho3d ( res.spin ) ( trans );
            res.isDef = true;
        }
        return res;
    }

    Vector3d operator() ( const Vector3d & v ) const
    {
        return Ortho3d ( spin ) ( v ) * magn + trans;
    }

    Segment3d operator () ( const Segment3d & p ) const
    {
        return Segment3d ( (*this) ( p.a ), (*this) ( p.b ) );
    }

    Plane3d operator() ( const Plane3d & p ) const
    {
        const Vector3d v = Ortho3d ( spin ) ( p.norm );
        return Plane3d ( v, magn * p.dist - v * trans );
    }
};

inline Conform3d operator * ( const Conform3d & l, const Conform3d & r )
{
    return Conform3d ( l.spin * r.spin, l ( r.trans ), l.magn * r.magn );
}

/////////////////////////// Подобие ( вариант 2 ) /////////////////////////////

class Similar3d
{
    double   magn;
    Vector3d ox, oy, oz;
    Vector3d ax, ay, az, trans;
public:
    explicit Similar3d ( const Spin3d & spin ) : magn ( 1. ), trans ( 0., 0., 0. )
    {
        spin.getReper ( ox, oy, oz );
        ax = ox;
        ay = oy;
        az = oz;
    }

    explicit Similar3d ( const Conform3d & c ) : magn ( c.magn ), trans ( c.trans )
    {
        c.spin.getReper ( ox, oy, oz );
        ax = ox * magn;
        ay = oy * magn;
        az = oz * magn;
    }

    double getX ( const Vector3d & p ) const { return ax * p + trans.x; }
    double getY ( const Vector3d & p ) const { return ay * p + trans.y; }
    double getZ ( const Vector3d & p ) const { return az * p + trans.z; }

    Vector3d operator () ( const Vector3d & p ) const
    {
        return Vector3d ( ax * p + trans.x, ay * p + trans.y, az * p + trans.z );
    }

    Segment3d operator () ( const Segment3d & p ) const
    {
        return Segment3d ( (*this) ( p.a ), (*this) ( p.b ) );
    }

    Plane3d operator() ( const Plane3d & p ) const
    {
        const Vector3d v ( ox*p.norm, oy*p.norm, oz*p.norm );
        return Plane3d ( v, magn * p.dist - v * trans );
    }

    void getReper ( Vector3d & x, Vector3d & y, Vector3d & z ) const
    {
        x = ox;
        y = oy;
        z = oz;
    }

    Similar3d & setTrans ( const Vector3d & v )
    {
        trans = v;
        return *this;
    }
};

////////////////////////// Линейное преобразование ////////////////////////////

class Sphere3d;
class Ellipsoid3d;

class LinTran3d
{
public:
    Vector3d x, y, z;

    LinTran3d () {}

    LinTran3d ( const Vector3d & a, const Vector3d & b, const Vector3d & c ) : 
      x ( a ), y ( b ), z ( c ) {}
    
    explicit LinTran3d ( const Spin3d & spin )
    {
        spin.getReper ( x, y, z );
    }

    LinTran3d & makeIdent ()
    {
        x.x = 1; x.y = 0; x.z = 0;
        y.x = 0; y.y = 1; y.z = 0;
        z.x = 0; z.y = 0; z.z = 1;
        return * this;
    }

    Vector3d operator () ( const Vector3d & p ) const
    {
        return Vector3d ( x * p, y * p, z * p );
    }

    Segment3d operator () ( const Segment3d & p ) const
    {
        return Segment3d ( (*this) ( p.a ), (*this) ( p.b ) );
    }

    Plane3d     operator() ( const Plane3d     & p ) const;
    Ellipsoid3d operator() ( const Sphere3d    & p ) const;
    Ellipsoid3d operator() ( const Ellipsoid3d & p ) const;

    LinTran3d & operator *= ( double d )
    {
        x *= d; y *= d; z *= d;
        return * this;
    }

    Def<LinTran3d> operator ~ () const;

    friend inline LinTran3d operator * ( const LinTran3d & l, const LinTran3d & r )
    {
        return LinTran3d ( l.x.x * r.x + l.x.y * r.y + l.x.z * r.z, 
                           l.y.x * r.x + l.y.y * r.y + l.y.z * r.z, 
                           l.z.x * r.x + l.z.y * r.y + l.z.z * r.z );
    }
};

////////////////////////// Аффинное преобразование ////////////////////////////

class Affin3d
{
public:
    LinTran3d t;
    Vector3d s;

    Affin3d () {}

    Affin3d ( const Vector3d & a, const Vector3d & b, const Vector3d & c, const Vector3d & d ) : 
        t ( a, b, c ), s ( d ) {}

    Affin3d ( const LinTran3d & a, const Vector3d & b ) : t ( a ), s ( b ) {}
    
    explicit Affin3d ( const LinTran3d & a ) : t ( a ), s ( 0., 0., 0. ) {}

    explicit Affin3d ( const Spin3d & spin ) : t ( spin ), s ( 0., 0., 0. ) {}

    explicit Affin3d ( const Conform3d & conf ) : t ( conf.spin ), s ( conf.trans )
    {
        t *= conf.magn;
    }

    Affin3d & makeIdent ()
    {
        t.makeIdent();
        s.x = s.y = s.z = 0;
        return * this;
    }

    Vector3d operator () ( const Vector3d & p ) const
    {
        return t ( p ) + s;
    }

    Segment3d operator () ( const Segment3d & p ) const
    {
        return Segment3d ( (*this) ( p.a ), (*this) ( p.b ) );
    }

    Plane3d operator() ( const Plane3d & p ) const
    {
        Plane3d r = t ( p );
        r.dist -= s * r.norm;
        return r;
    }

    Ellipsoid3d operator() ( const Sphere3d    & p ) const;
    Ellipsoid3d operator() ( const Ellipsoid3d & p ) const;

    Def<Affin3d> operator ~ () const
    {
        const Def<LinTran3d> lin ( ~t );
        return lin.isDef ? Affin3d ( lin, lin ( -s ) ) : Def<Affin3d>();
    }

    friend inline Affin3d operator * ( const Affin3d & l, const Affin3d & r )
    {
        return Affin3d ( l.t * r.t, l ( r.s ) );
    }
};

///////////////////////////////// Круг ////////////////////////////////////////

class Circle3d
{
public:
    Vector3d o; // центр
    double   r; // радиус
    Spin3d spin;// поворот

    Circle3d () {}
    Circle3d ( const Vector3d & a, double b ) : o ( a ), r ( b ) {}

    Circle3d & operator *= ( const Conform3d & c )
    {
        o *= c; r *= c.magn; spin = c.spin * spin; return *this;
    }

    Circle3d & operator *= ( const Spin3d & s )
    {
        o *= Ortho3d ( s ); spin = s * spin; return *this;
    }

    Circle3d & operator *= ( const double & d )
    {
        o *= d; r *= d; return *this;
    }
};

////////////////////////////////// Эллипс /////////////////////////////////////

class Ellipse3d
{
public:
    Vector3d o;  // центр
    double a, b; // полуоси
    Spin3d spin; // поворот

    Ellipse3d () {}
    Ellipse3d ( const Vector3d & o1, double a1, double b1 ) : o ( o1 ), a ( a1 ), b ( b1 ) {}

    Vector3d getNorm () const
    {
        return spin.rotZ();
    }

    double getArea () const;

    double getPerimeter () const;

    Ellipse3d & operator *= ( const Conform3d & c )
    {
        o *= c; a *= c.magn; b *= c.magn; spin = c.spin * spin; return *this;
    }

    Ellipse3d & operator *= ( const Spin3d & s )
    {
        o *= Ortho3d ( s ); spin = s * spin; return *this;
    }

    Ellipse3d & operator *= ( const double & d )
    {
        o *= d; a *= d; b *= d; return *this;
    }
};

///////////////////////////// Треугольник /////////////////////////////////////

class Triangle3d
{
public:
    Vector3d a, b, c;

    Triangle3d () {}
    Triangle3d ( const Vector3d & pa, const Vector3d & pb, const Vector3d & pc ) : a ( pa ), b ( pb ), c ( pc ) {}

    Vector3d getNormal () const
    {
        return ( ( a - b ) % ( b - c ) ).setNorm2();
    }

    double getArea () const // Площадь поверхности треугольника с двух сторон
    {
        return norm2 ( ( a - b ) % ( b - c ) );
    }
};

/////////////////////////////// Прямоугольник /////////////////////////////////

class Rectangle3d
{
public:
    Vector3d o;  // центр
    double a, b; // полуоси
    Spin3d spin; // поворот

    Rectangle3d () {}
    Rectangle3d ( const Vector3d & o1, double a1, double b1 ) : o ( o1 ), a ( a1 ), b ( b1 ) {}

    void getVerts ( Vector3d vert[4] ) const;

    Vector3d getNorm () const
    {
        return spin.rotZ();
    }

    double getArea () const
    {
        return 4 * a * b;
    }

    double getPerimeter () const
    {
        return 4 * ( a + b );
    }

    Rectangle3d & operator *= ( const Conform3d & c )
    {
        o *= c; a *= c.magn; b *= c.magn; spin = c.spin * spin; return *this;
    }

    Rectangle3d & operator *= ( const Spin3d & s )
    {
        o *= Ortho3d ( s ); spin = s * spin; return *this;
    }

    Rectangle3d & operator *= ( const double & d )
    {
        o *= d; a *= d; b *= d; return *this;
    }
};

/////////////////////////////////// Ромб //////////////////////////////////////

class Rhombus3d
{
public:
    Vector3d o;  // центр
    double a, b; // полуоси
    Spin3d spin; // поворот

    Rhombus3d () {}
    Rhombus3d ( const Vector3d & o1, double a1, double b1 ) : o ( o1 ), a ( a1 ), b ( b1 ) {}

    void getVerts ( Vector3d vert[4] ) const;

    Vector3d getNorm () const
    {
        return spin.rotZ();
    }

    double getArea () const
    {
        return 2 * a * b;
    }

    double getPerimeter () const;

    Rhombus3d & operator *= ( const Conform3d & c )
    {
        o *= c; a *= c.magn; b *= c.magn; spin = c.spin * spin; return *this;
    }

    Rhombus3d & operator *= ( const Spin3d & s )
    {
        o *= Ortho3d ( s ); spin = s * spin; return *this;
    }

    Rhombus3d & operator *= ( const double & d )
    {
        o *= d; a *= d; b *= d; return *this;
    }
};

////////////////////////////// Параллелограмм /////////////////////////////////

class Parallelogram3d
{
    Vector3d o, a, b; 
public:
    Parallelogram3d () {}

    Parallelogram3d ( const Vector3d & v1, const Vector3d & v2, const Vector3d & v3 ) :
        o ( v2 ), a ( v3 - v2 ), b ( v1 - v2 ) {}

    void getVerts ( Vector3d vert[4] ) const
    {
        vert[0] = o + b;
        vert[1] = o;
        vert[2] = o + a;
        vert[3] = o + a + b;
    }

    Vector3d getNorm () const
    {
        return ( a % b ).setNorm2();
    }

    double getArea () const
    {
        return norm2 ( a % b );
    }

    double getPerimeter () const
    {
        return 2 * ( norm2 ( a ) + norm2 ( b ) );
    }

    Parallelogram3d & operator *= ( const Conform3d & c )
    {
        Similar3d sim ( c );
        o *= sim;
        a *= sim.setTrans ( null3d );
        b *= sim;
        return *this;
    }
};

//////////////////////////////// Сфера ////////////////////////////////////////

class Sphere3d
{
public:
    double   r; // радиус
    Vector3d o; // центр

    Sphere3d () {}
    Sphere3d ( double a, const Vector3d & b ) : r ( a ), o ( b ) {}
    double getArea () const;
};

//////////////////////////////// Эллипсоид ////////////////////////////////////

class Ellipsoid3d
{
public:
    double a, b, c; // полуоси
    Spin3d spin;    // поворот
    Vector3d o;     // центр

    Affin3d getAffin3d () const
    {
        LinTran3d t ( spin );
        t.x.x *= a; t.x.y *= b; t.x.z *= c;
        t.y.x *= a; t.y.y *= b; t.y.z *= c;
        t.z.x *= a; t.z.y *= b; t.z.z *= c;
        return Affin3d ( t, o );
    }
    double getArea () const;
};

///////////////////////////////// Цилиндр /////////////////////////////////////

class Cylinder3d
{
public:
    double r, h; // радиус и половина высоты
    Spin3d spin; // поворот
    Vector3d o;  // центр

    double getArea () const;
};

////////////////////////////////// Конус //////////////////////////////////////

class Cone3d
{
public:
    double r, h; // радиус основания и половина высоты
    Spin3d spin; // поворот
    Vector3d o;  // центр

    double getArea () const;
};

///////////////////////// Прямоугольный параллелепипед ////////////////////////

class Cuboid3d
{
public:
    double a, b, c; // полуоси
    Spin3d spin;    // поворот
    Vector3d o;     // центр

    Affin3d getAffin3d () const
    {
        LinTran3d t ( spin );
        t.x.x *= a; t.x.y *= b; t.x.z *= c;
        t.y.x *= a; t.y.y *= b; t.y.z *= c;
        t.z.x *= a; t.z.y *= b; t.z.z *= c;
        return Affin3d ( t, o );
    }
    double getArea () const;
};

/////////////////////////////// Объёмы фигур //////////////////////////////////

double volume ( const Sphere3d      & );
double volume ( const Ellipsoid3d   & );
double volume ( const Cylinder3d    & );
double volume ( const Cone3d        & );
double volume ( const Cuboid3d      & );

