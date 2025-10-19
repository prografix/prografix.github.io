
#include <math.h>
#include "tune.h"
#include "Mathem.h"
#include "LinAlg.h"
#include "Vector3d.h"

//////////////////////////////// Вектор ///////////////////////////////////////

// Получение перпендикуляра к данному вектору

Vector3d Vector3d::perpendicular () const
{
    double ax = fabs ( x );
    double ay = fabs ( y );
    double az = fabs ( z );
    if ( ax >= ay )
    {
        if ( ax >= az )
        {
            if ( ! ax ) return Vector3d (1.,0.,0.);
            double d = 1./ sqrt ( ax * ax + ay * ay );
            return Vector3d (-y*d, x*d, 0. );
        }
    }
    else
    {
        if ( ay >= az )
        {
            double d = 1./ sqrt ( ay * ay + az * az );
            return Vector3d ( 0.,-z*d, y*d );
        }
    }
    double d = 1./ sqrt ( ax * ax + az * az );
    return Vector3d ( z*d, 0.,-x*d );
}

// Задание векторных норм

Vector3d & Vector3d::setNorm1 ( double p ) // единичная норма
{
    const double t = fabs ( x ) + fabs ( y ) + fabs ( z );
    if ( t > 0 )
    {
        p /= t;
        x *= p;
        y *= p;
        z *= p;
    }
    return * this;
}

Vector3d & Vector3d::setNorm2 ( double p ) // квадратичная норма
{
    const double t = x * x + y * y + z * z;
    if ( t > 0 )
    {
        p /= sqrt ( t );
        x *= p;
        y *= p;
        z *= p;
    }
    return * this;
}

Vector3d & Vector3d::setNormU ( double p ) // бесконечная норма
{
    const double t = _max ( fabs ( x ), fabs ( y ), fabs ( z ) );
    if ( t > 0 )
    {
        p /= t;
        x *= p;
        y *= p;
        z *= p;
    }
    return * this;
}

// Вычисление векторных норм

double norm1 ( const Vector3d & v ) // единичная норма
{
    return fabs ( v.x ) + fabs ( v.y ) + fabs ( v.z );
}

double norm2 ( const Vector3d & v ) // квадратичная норма
{
    return sqrt ( v.x * v.x + v.y * v.y + v.z * v.z );
}

double normU ( const Vector3d & v ) // бесконечная норма
{
    return _max ( fabs ( v.x ), fabs ( v.y ), fabs ( v.z ) );
}

//////////////////////////////// Отрезок //////////////////////////////////////

Def<Vector3d> Segment3d::project ( const Vector3d & p ) const // проекция точки на отрезок
{
    const Vector3d u = b - a;
    const Vector3d w = p - a;
    const double s = u * w;
    if ( s < 0 ) return Def<Vector3d>();
    const double q = u * u;
    if ( s > q ) return Def<Vector3d>();
    return q > 0 ? a + u * ( s / q ) : a;
}

Vector3d Segment3d::nearPoint ( const Vector3d & p ) const // ближайшая точка отрезка к данной
{
    const Vector3d u = b - a;
    const Vector3d w = p - a;
    const double s = u * w;
    if ( s <= 0 ) return a;
    const double q = u * u;
    return s >= q ? b : a + u * ( s / q );
}

///////////////////////////// Плоскость ///////////////////////////////////////

Plane3d::Plane3d ( const Vector3d & a, const Vector3d & b, const Vector3d & c )
{
    const Vector3d ab = a - b;
    const double d1 = ab * ab;
    const Vector3d bc = b - c;
    const double d2 = bc * bc;
    const Vector3d ca = c - a;
    const double d3 = ca * ca;
    if ( d1 >= d2 && d1 >= d3 )
        norm = bc % ca;
    else
    if ( d2 >= d3 )
        norm = ca % ab;
    else
        norm = ab % bc;
    if ( ! norm )
    {
        if ( d1 >= d2 )
            if ( d1 >= d3 ) norm = ab.perpendicular();
            else            norm = ca.perpendicular();
        else
            if ( d2 >= d3 ) norm = bc.perpendicular();
            else            norm = ca.perpendicular();
    }
    else
    {
        norm.setNorm2 ();
    }
    const double da = norm * a;
    const double db = norm * b;
    const double dc = norm * c;
    dist = -0.5 * ( _min ( da, db, dc ) + _max ( da, db, dc ) );
}

// Проекция точки на плоскость
Vector3d Plane3d::project ( const Vector3d & v ) const
{
    const double d = norm * norm;
    return d > 0 ? v - ( ( *this % v ) / d ) * norm : v;
}

// Отражение точки относительно плоскости
Vector3d Plane3d::mirror ( const Vector3d & v ) const
{
    const double d = norm * norm;
    return d > 0 ? v - ( 2 * ( *this % v ) / d ) * norm : v;
}

// Отражение плоскости
Plane3d Plane3d::mirror ( const Plane3d & p ) const
{
    const double s = norm * norm;
    if ( ! s ) return p;
    const double t = 2 * ( p.norm * norm ) / s;
    return Plane3d ( p.norm - t * norm, p.dist - t * dist );
}

///////////////////////////////// Поворот /////////////////////////////////////

Spin3d::Spin3d ( Vector3d axis, double angle )
{
    if ( ! axis )
    {
        t = 1.;
        x = y = z = 0.;
    }
    else
    {
        angle *= 0.5;
        t = cos (angle);
        axis.setNorm2 ( sin (angle) );
        x = axis.x;
        y = axis.y;
        z = axis.z;
    }
}

Spin3d::Spin3d ( Vector3d v1, Vector3d v2 )
{
    t = v1.setNorm2() * v2.setNorm2();
    if ( ! v1 || ! v2 || t >= 1.)
    {
        t = 1.;
        x = y = z = 0.;
        return;
    }
    t += 1.;
    Vector3d v = v1 % v2;
    double d = t*t + v*v;
    if ( t <= 0 || d < 1e-18 )
    {
        v = v1.perpendicular();
        t = 0.;
        x = v.x;
        y = v.y;
        z = v.z;
        return;
    }
    d = 1./ sqrt ( d );
    t *= d;
    x = v.x * d;
    y = v.y * d;
    z = v.z * d;
}

Spin3d::Spin3d ( const Vector3d & vx, const Vector3d & vy, const Vector3d & vz )
{
    SMatrix<double, 4, 4> a, v;
    a[0][0] = vx.x - vy.y - vz.z;
    a[0][1] = vx.y + vy.x;
    a[0][2] = vx.z + vz.x;
    a[0][3] = vy.z - vz.y;
    a[1][1] = vy.y - vz.z - vx.x;
    a[1][2] = vy.z + vz.y;
    a[1][3] = vz.x - vx.z;
    a[2][2] = vz.z - vy.y - vx.x;
    a[2][3] = vx.y - vy.x;
    a[3][3] = vx.x + vy.y + vz.z;
    double d[4], max = 0;
    jacobi ( 4, a, d, v );
    for ( nat i = 0; i < 4; ++i )
    {
        if ( max >= d[i] ) continue;
        max = d[i];
        x = v[0][i];
        y = v[1][i];
        z = v[2][i];
        t = v[3][i];
    }
    if ( !max )
    {
        x = y = z = 0;
        t = 1;
    }
}

Spin3d::Spin3d ( const double & a, const double & b, const double & c, const double & d ) : t(a), x(b), y(c), z(d)
{
    double q = t * t + x * x + y * y + z * z;
    if ( q > 0 )
    {
        q = 1./ sqrt ( q );
        t *= q; x *= q; y *= q; z *= q;
    }
    else
    {
        t = 1.; x = y = z = 0.;
    }
}

Vector3d Spin3d::getAxis () const
{
    return ( !x && !y && !z ) ? Vector3d ( 1., 0., 0. ) : Vector3d ( x, y, z ).setNorm2();
}

double Spin3d::getAngle () const
{
    return fabs ( t ) >= 1. ? 0. : 2.* acos ( t );
}

Vector3d Spin3d::rotX () const
{
    return Vector3d ( t*t + x*x - y*y - z*z, 2. * ( x*y + z*t ),  2. * ( z*x - y*t ) );
}

Vector3d Spin3d::rotY () const
{
    return Vector3d ( 2. * ( x*y - z*t ),  t*t + y*y - z*z - x*x,  2. * ( y*z + x*t ) );
}

Vector3d Spin3d::rotZ () const
{
    return Vector3d ( 2. * ( z*x + y*t ),  2. * ( y*z - x*t ),  t*t + z*z - x*x - y*y );
}

void Spin3d::getReper ( Vector3d & ax, Vector3d & ay, Vector3d & az ) const
{
    const double xx = x * x;
    const double yy = y * y;
    const double zz = z * z;
    const double tt = t * t;
    const double xy = x * y;
    const double yz = y * z;
    const double zx = z * x;
    const double xt = x * t;
    const double yt = y * t;
    const double zt = z * t;
    ax.x = tt + xx - yy - zz; ax.y = 2. * ( xy - zt );  ax.z = 2. * ( zx + yt );
    ay.x = 2. * ( xy + zt );  ay.y = tt + yy - zz - xx; ay.z = 2. * ( yz - xt );
    az.x = 2. * ( zx - yt );  az.y = 2. * ( yz + xt );  az.z = tt + zz - xx - yy;
}

Spin3d operator * ( const Spin3d & l, const Spin3d & r )
{
    Spin3d p;
    p.t = l.t * r.t - l.x * r.x - l.y * r.y - l.z * r.z;
    p.x = l.t * r.x + l.x * r.t + l.y * r.z - l.z * r.y;
    p.y = l.t * r.y + l.y * r.t + l.z * r.x - l.x * r.z;
    p.z = l.t * r.z + l.z * r.t + l.x * r.y - l.y * r.x;
    double d = p.t * p.t + p.x * p.x + p.y * p.y + p.z * p.z;
    if ( fabs ( d - 1.) > 1e-14 )
    {
        d = 1./ sqrt ( d );
        p.t *= d; p.x *= d; p.y *= d; p.z *= d;
    }
    return p;
}

Def<Spin3d> getSpin3d ( const Vector3d & vx, const Vector3d & vy, const Vector3d & vz )
{
    SMatrix<double, 4, 4> a, v;
    a[0][0] = vx.x - vy.y - vz.z;
    a[0][1] = vx.y + vy.x;
    a[0][2] = vx.z + vz.x;
    a[0][3] = vy.z - vz.y;
    a[1][1] = vy.y - vz.z - vx.x;
    a[1][2] = vy.z + vz.y;
    a[1][3] = vz.x - vx.z;
    a[2][2] = vz.z - vy.y - vx.x;
    a[2][3] = vx.y - vy.x;
    a[3][3] = vx.x + vy.y + vz.z;
    double d[4];
    jacobi ( 4, a, d, v );
    nat im = 0;
    double max = fabs ( d[0] );
    for ( nat i = 1; i < 4; ++i ) if ( _maxa ( max, fabs ( d[i] ) ) ) im = i;
    Def<Spin3d> res;
    if ( d[im] <= 0 ) return res;
    res.x = v[0][im];
    res.y = v[1][im];
    res.z = v[2][im];
    res.t = v[3][im];
    res.isDef = true;
    return res;
}

//////////////////////// Ортогональное преобразование /////////////////////////

Ortho3d::Ortho3d ( Vector3d a, double b )
{
    const double c = cos ( b );
    const double e = 1 - c;
    if ( ! a || e <= 0 )
    {
        x.x = y.y = z.z = 1.;
        x.y = x.z = y.x = y.z = z.x = z.y = 0.;
    }
    else
    {
        a.setNorm2();

        x.x = e * a.x * a.x; x.y = e * a.x * a.y; x.z = e * a.x * a.z;
        y.x = x.y;           y.y = e * a.y * a.y; y.z = e * a.y * a.z;
        z.x = x.z;           z.y = y.z;           z.z = e * a.z * a.z;

        a *= sin ( b );
        x.x += c;   x.y -= a.z; x.z += a.y;
        y.x += a.z; y.y += c;   y.z -= a.x;
        z.x -= a.y; z.y += a.x; z.z += c;
    }
}

Ortho3d Ortho3d::operator ~ () const
{
    Ortho3d r;
    r.x.x = x.x; r.x.y = y.x; r.x.z = z.x;
    r.y.x = x.y; r.y.y = y.y; r.y.z = z.y;
    r.z.x = x.z; r.z.y = y.z; r.z.z = z.z;
    return r;
}

////////////////////////// Линейное преобразование ////////////////////////////

Def<LinTran3d> LinTran3d::operator ~ () const
{
    SLU3<double, Vector3d> slu;
    slu.aa = x.x;  slu.ab = x.y;  slu.ac = x.z;  slu.ad = Vector3d(1,0,0);
    slu.ba = y.x;  slu.bb = y.y;  slu.bc = y.z;  slu.bd = Vector3d(0,1,0);
    slu.ca = z.x;  slu.cb = z.y;  slu.cc = z.z;  slu.cd = Vector3d(0,0,1);
    Def<LinTran3d> res;
    res.isDef = slu.gauss ( res.x, res.y, res.z );
    return res;
}

Plane3d LinTran3d::operator() ( const Plane3d & p ) const
{
    Plane3d res;
    Vector3d vx, vy;
    reper ( p.norm, vx, vy );
    const Vector3d ux ( x * vx, y * vx, z * vx );
    const Vector3d uy ( x * vy, y * vy, z * vy );
    res.norm = ux % uy;
    const Vector3d v ( p.norm * p.dist );
    const Vector3d o ( x * v, y * v, z * v );
    res.dist = o * res.norm.setNorm2();
    return res;
}

Ellipsoid3d LinTran3d::operator() ( const Sphere3d & p ) const
{
    Ellipsoid3d res;
    SMatrix<double, 3, 3> m, v;
    m[0][0] = x * x;
    m[0][1] = m[1][0] = x * y;
    m[0][2] = m[2][0] = x * z;
    m[1][1] = y * y;
    m[1][2] = m[2][1] = y * z;
    m[2][2] = z * z;
    double u[3];
    jacobi ( 3, m, u, v );
    res.a = p.r * sqrt ( u[0] );
    res.b = p.r * sqrt ( u[1] );
    res.c = p.r * sqrt ( u[2] );
    res.spin = Spin3d ( Vector3d ( v[0][0], v[1][0], v[2][0] ), 
                        Vector3d ( v[0][1], v[1][1], v[2][1] ),
                        Vector3d ( v[0][2], v[1][2], v[2][2] ) );
    res.o = (*this) ( p.o );
    return res;
}

Ellipsoid3d LinTran3d::operator() ( const Ellipsoid3d & p ) const
{
    const Affin3d aff ( p.getAffin3d() );
    return Affin3d ( *this * aff.t, (*this) ( aff.s ) ) ( Sphere3d ( 1, null3d ) );
}

////////////////////////// Аффинное преобразование ////////////////////////////

Ellipsoid3d Affin3d::operator() ( const Sphere3d & p ) const
{
    Ellipsoid3d res = t ( p );
    res.o += s;
    return res;
}

Ellipsoid3d Affin3d::operator() ( const Ellipsoid3d & p ) const
{
    return ( *this * p.getAffin3d() ) ( Sphere3d ( 1, null3d ) );
}

////////////////////////////////// Эллипс /////////////////////////////////////

// Площадь
double Ellipse3d::getArea () const
{
    return a * b * M_PI;
}

// Периметр
double Ellipse3d::getPerimeter () const
{
    const double c = 3 * _pow2 ( ( a - b ) / ( a + b ) );
    return M_PI * ( a + b ) * ( 1 + c / ( 10 + sqrt ( 4 - c ) ) );
}

/////////////////////////////// Прямоугольник /////////////////////////////////

void Rectangle3d::getVerts ( Vector3d vert[4] ) const
{
    const Vector3d va = a * spin.rotX();
    const Vector3d vb = b * spin.rotY();
    vert[0] = o + va + vb;
    vert[1] = o - va + vb;
    vert[2] = o - va - vb;
    vert[3] = o + va - vb;
}

/////////////////////////////////// Ромб //////////////////////////////////////

void Rhombus3d::getVerts ( Vector3d vert[4] ) const
{
    const Vector3d va = a * spin.rotX();
    const Vector3d vb = b * spin.rotY();
    vert[0] = o + va;
    vert[1] = o + vb;
    vert[2] = o - va;
    vert[3] = o - vb;
}

double Rhombus3d::getPerimeter () const
{
    return 4 * sqrt ( a*a + b*b );
}

/////////////////////////////// Объёмы фигур //////////////////////////////////

double volume ( const Sphere3d & fig ) // Объём шара
{
    return 4./3. * M_PI * fig.r * fig.r * fig.r;
}

double volume ( const Ellipsoid3d & fig ) // Объём эллипсоида
{
    return 4./3. * M_PI * fig.a * fig.b * fig.c;
}

double volume ( const Cuboid3d & fig ) // Объём параллелепипеда
{
    return 8 * fig.a * fig.b * fig.c;
}

double volume ( const Cylinder3d & fig ) // Объём цилиндра
{
    return M_2PI * fig.r * fig.r * fig.h;
}

double volume ( const Cone3d & fig ) // Объём конуса
{
    return M_2PI/3 * fig.r * fig.r * fig.h;
}

//////////////////////////// Площади поверхностей /////////////////////////////

double Sphere3d::getArea () const
{
    return 4 * M_PI * r * r;
}

double Ellipsoid3d::getArea () const
{
    const double p = 1.6075;
    return 4 * M_PI * pow ( ( pow ( a * b, p ) + pow ( b * c, p ) + pow ( c * a, p ) ) / 3, 1/p );
}

double Cuboid3d::getArea () const
{
    return 8 * ( a * b + b * c + c * a );
}

double Cylinder3d::getArea () const
{
    return 2 * M_PI * r * ( r + 2 * h );
}

double Cone3d::getArea () const
{
    return M_PI * r * ( r + sqrt ( 4 * h * h + r * r ) );
}