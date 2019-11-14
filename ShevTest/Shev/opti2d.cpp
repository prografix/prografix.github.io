
#include "math.h"

#include "func2d.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "WireModel.h"
#include "LinAlg.h"
#include "Mathem.h"
#include "opti1d.h"
#include "opti2d.h"
#include "intersect2d.h"
#include "lists.h"
#include "heap.h"

//**************************** 23.08.2003 *********************************//
//
//          Минимальная окружность вокруг данных точек
//
//**************************** 12.02.2016 *********************************//

Def<Circle2d> minCircleAroundPoints ( CArrRef<Vector2d> data, DynArrRef<nat> * index )
{
    const nat n = data.size();
    if ( n < 3 )
    {
        if ( index )
        {
            index->resize ( n );
            if ( n == 0 ) return Def<Circle2d>();
            (*index)[0] = 0;
            if ( n == 1 ) return Circle2d ( data[0], 0 );
            (*index)[1] = 1;
        }
        else
        {
            if ( n == 0 ) return Def<Circle2d>();
            if ( n == 1 ) return Circle2d ( data[0], 0 );
        }
        const Vector2d o = 0.5 * ( data[0] + data[1] );
        return Circle2d ( o, norm2 ( data[0] - o ) );
    }
    const Vector2d p0 = data[0];
    nat i, im = 0;
    double max = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[i] - p0 );
        if ( max < t ) max = t, im = i;
    }
    if ( im == 0 ) return Circle2d ( p0, 0 );
    nat np = 2, ip[3];
    ip[0] = 0;
    ip[1] = im;
    Vector2d o = 0.5 * ( p0 + data[im] );
    double q = 0.25 * max;
    for (;;)
    {
        max = 0;
        for ( i = 0; i < n; ++i )
        {
            const double t = qmod ( data[i] - o );
            if ( max < t ) max = t, im = i;
        }
        if ( max <= q || im == ip[0] || im == ip[1] ) break;
        const Vector2d pm = data[im];
        nat km = 0;
        double s = qmod ( pm - data[ip[0]] );
        double t = qmod ( pm - data[ip[1]] );
        if ( s < t ) s = t, km = 1;
        Vector2d v;
        if ( np == 2 )
        {
            s *= 0.25;
            _swap ( ip[0], ip[km] );
            v = 0.5 * ( pm + data[ip[0]] );
            if ( qmod ( v - data[ip[1]] ) > s )
            {
                np = 3;
                ip[2] = im;
                circlePPP ( data[ip[0]], data[ip[1]], pm, v, s );
            }
            else
            {
                ip[1] = im;
            }
        }
        else
        {
            if ( im == ip[2] ) break;
            t = qmod ( pm - data[ip[2]] );
            if ( s < t ) s = t, km = 2;
            s *= 0.25;
            _swap ( ip[0], ip[km] );
            v = 0.5 * ( pm + data[ip[0]] );
            double q1 = qmod ( v - data[ip[1]] );
            double q2 = qmod ( v - data[ip[2]] );
            if ( q1 < q2 ) ip[1] = ip[2], q1 = q2;
            if ( q1 > s )
            {
                circlePPP ( data[ip[0]], data[ip[1]], pm, v, s );
                ip[2] = im;
            }
            else
            {
                np = 2;
                ip[1] = im;
            }
        }
        if ( s <= q ) break;
        q = s;
        o = v;
    }
    if ( index )
    {
        index->resize ( np );
        (*index)[0] = ip[0];
        (*index)[1] = ip[1];
        if ( np == 3 ) (*index)[2] = ip[2];
    }
    return Circle2d ( o, sqrt ( q ) );
}

//**************************** 05.11.2010 *********************************//
//
//          Минимальная окружность вокруг данных окружностей
//
//**************************** 09.01.2012 *********************************//

Def<Circle2d> circleCCC ( const Circle2d & c1, const Circle2d & c2, const Circle2d & c3 )
{
    Def<Circle2d> res;
    const double s1 = 0.5 * ( c1.o * c1.o - c1.r * c1.r );
    const double s2 = 0.5 * ( c2.o * c2.o - c2.r * c2.r );
    const double s3 = 0.5 * ( c3.o * c3.o - c3.r * c3.r );
    const double x21 = c2.o.x - c1.o.x;
    const double y21 = c2.o.y - c1.o.y;
    const double z21 = c2.r - c1.r;
    const double s21 = s2 - s1;
    const double x31 = c3.o.x - c1.o.x;
    const double y31 = c3.o.y - c1.o.y;
    const double z31 = c3.r - c1.r;
    const double s31 = s3 - s1;
    const double d = x21 * y31 - x31 * y21;
    if ( d == 0 ) return res;
    const double ax = ( z21 * y31 - z31 * y21 ) / d;
    const double ay = ( x21 * z31 - x31 * z21 ) / d;
    const double bx = ( s21 * y31 - s31 * y21 ) / d;
    const double by = ( x21 * s31 - x31 * s21 ) / d;
    const double cx = bx - c1.o.x;
    const double cy = by - c1.o.y;
    const double a = ax * ax + ay * ay - 1;
    const double b = ax * cx + ay * cy + c1.r;
    const double c = cx * cx + cy * cy - c1.r * c1.r;
    if ( a == 0 )
    {
        if ( b == 0 ) return res;
        res.r = -0.5 * c / b;
    }
    else
    {
        const double D = b * b - a * c;
        if ( D < 0 ) return res;
        res.r = ( -b - sqrt ( D ) ) / a;
    }
    res.o = Vector2d ( ax * res.r + bx, ay * res.r + by );
    res.isDef = true;
    return res;
}

Def<Circle2d> minCircleAroundCircles ( CArrRef<Circle2d> data )
{
    const nat n = data.size();
    if ( n < 2 ) return n == 0 ? Def<Circle2d> () : Def<Circle2d> ( data[0] );
    nat i, im = 0, np = 2, ip[3];
    double max = data[0].r;
    for ( i = 1; i < n; ++i )
    {
        const double t = data[i].r;
        if ( max < t ) max = t, im = i;
    }
    ip[0] = im;
    Circle2d res = data[im];
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = data[i].r + norm2 ( data[i].o - res.o );
        if ( max < t ) max = t, im = i;
    }
    double d = 0.5 * ( max - res.r );
    if ( im == ip[0] || d <= 0 ) return res;
    ip[1] = im;
    res.o += ( data[im].o - res.o ) * ( d / ( max - data[im].r ) );
    res.r += d;
    for (;;)
    {
        max = 0;
        im = ip[0];
        for ( i = 0; i < n; ++i )
        {
            const double t = data[i].r + norm2 ( data[i].o - res.o );
            if ( max < t ) max = t, im = i;
        }
        if ( im == ip[0] || im == ip[1] || max <= res.r ) break;
        const Circle2d cm = data[im];
        const double d = max - cm.r;
        Def<Circle2d> c;
        nat km = 0;
        double s = data[ip[0]].r + norm2 ( cm.o - data[ip[0]].o );
        double t = data[ip[1]].r + norm2 ( cm.o - data[ip[1]].o );
        if ( s < t ) s = t, km = 1;
        if ( np == 2 )
        {
            c.r = 0.5 * ( cm.r + s );
            _swap ( ip[0], ip[km] );
            c.o = cm.o + ( data[ip[0]].o - cm.o ) * ( ( c.r - cm.r ) / ( s - data[ip[0]].r ) );
            if ( data[ip[1]].r + norm2 ( c.o - data[ip[1]].o ) > c.r )
            {
                np = 3;
                ip[2] = im;
                c = circleCCC ( data[ip[0]], data[ip[1]], cm );
                if ( ! c.isDef ) return c;
            }
            else
            {
                ip[1] = im;
            }
        }
        else
        {
            if ( im == ip[2] ) break;
            t = data[ip[2]].r + norm2 ( cm.o - data[ip[2]].o );
            if ( s < t ) s = t, km = 2;
            c.r = 0.5 * ( cm.r + s );
            _swap ( ip[0], ip[km] );
            c.o = cm.o + ( data[ip[0]].o - cm.o ) * ( ( c.r - cm.r ) / ( s - data[ip[0]].r ) );
            double r1 = data[ip[1]].r + norm2 ( c.o - data[ip[1]].o );
            double r2 = data[ip[2]].r + norm2 ( c.o - data[ip[2]].o );
            if ( r1 < r2 ) r1 = r2, ip[1] = ip[2];
            if ( r1 > c.r )
            {
                c = circleCCC ( data[ip[0]], data[ip[1]], cm );
                if ( ! c.isDef ) return c;
                ip[2] = im;
            }
            else
            {
                np = 2;
                ip[1] = im;
            }
        }
        if ( c.r <= res.r ) break;
        res = c;
    }
    return res;
}

//**************************** 31.12.2009 *********************************//
//
//          Минимальная окружность пересекающая прямые
//
//**************************** 23.05.2010 *********************************//

Def<Circle2d> minCircle ( CArrRef<Line2d> line )
{
    const nat nn = line.size();
    if ( nn < 3 )
    {
        if ( nn < 2 ) return Def<Circle2d>();
        const Def<Vector2d> p = intersection ( line[0], line[1] );
        return p.isDef ? Def<Circle2d> ( Circle2d ( p, 0 ) ) : Def<Circle2d>();
    }
    const nat n1 = nn - 1;
    const nat n2 = n1 - 1;
    nat i1 = 0, i2 = 1;
    while ( fabs ( line[i1].norm % line[i2].norm ) < 1e-12 )
    {
        if ( ++i2 == n1 ) return Def<Circle2d>();
    }
    nat i3 = i2 + 1;
    while ( fabs ( line[i1].norm % line[i3].norm ) < 1e-12 ||
            fabs ( line[i2].norm % line[i3].norm ) < 1e-12 )
    {
        if ( ++i3 == nn ) return Def<Circle2d>();
    }
    Line2d line1 = line[i1];
    Line2d line2 = line[i2];
    Line2d line3 = line[i3];
    if ( line1 % intersection ( line2, line3 ) > 0 ) line1 = - line1;
    if ( line2 % intersection ( line3, line1 ) > 0 ) line2 = - line2;
    if ( line3 % intersection ( line1, line2 ) > 0 ) line3 = - line3;
    const Vector2d v1 = line1.norm - line3.norm;
    const Vector2d v2 = line2.norm - line3.norm;
    const double p = v2 % v1;
    if ( p == 0 ) return Circle2d();
    const double d1 = line1.dist - line3.dist;
    const double d2 = line2.dist - line3.dist;
    Vector2d o;
    o.x = ( v2.y * d1 - v1.y * d2 ) / p;
    o.y = ( v1.x * d2 - v2.x * d1 ) / p;
    const double d = line3 % o;
    double r = d < 0 ? - d : 0;
    Vector3d arr[4];
    arr[0] = Vector3d ( o.x, o.y, r );
    arr[1] = Vector3d ( line1.norm.x, line1.norm.y, 1. ) % Vector3d ( line2.norm.x, line2.norm.y, 1. );
    arr[2] = Vector3d ( line2.norm.x, line2.norm.y, 1. ) % Vector3d ( line3.norm.x, line3.norm.y, 1. );
    arr[3] = Vector3d ( line3.norm.x, line3.norm.y, 1. ) % Vector3d ( line1.norm.x, line1.norm.y, 1. );
    arr[1].setNorm2 ( arr[1].z < 0 ? -1 : 1 );
    arr[2].setNorm2 ( arr[2].z < 0 ? -1 : 1 );
    arr[3].setNorm2 ( arr[3].z < 0 ? -1 : 1 );
    nat i, j;
    for ( j = 0; j < nn; ++j )
    {
        double max = 0.;
        nat im;
        for ( i = 0; i < nn; ++i )
        {
            const double t = fabs ( line[i] % o );
            if ( max < t ) max = t, im = i;
        }
        const double dist = max - r;
        if ( dist <= 1e-12 * _max ( 1., r ) )
        {
            return Circle2d ( o, r );
        }
        Line2d lm = line[im];
        if ( lm % o > 0 ) lm = - lm;
        const Vector3d cor ( lm.norm.x, lm.norm.y, 1. );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i <= 3; ++i )
        {
            const Vector3d & v = arr[i];
            double t = cor * v;
            if ( t < 1e-12 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.z * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double s = v.z * t;
                if ( s < max ) max = s, ib = i, sm = t;
            }
        }
        if ( ib == 0 ) 
            return Def<Circle2d>();
        const Vector3d & v = arr[ib];
        Vector3d & a0 = arr[0];
        a0 += v * ( dist * sm );
        o.x = a0.x;
        o.y = a0.y;
        r = a0.z;
        for ( i = 1; i <= 3; ++i )
        {
            if ( i == ib ) continue;
            Vector3d & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return Def<Circle2d>();
}

//**************************** 10.01.2009 *********************************//
//
//          Минимальный охватывающий фиксированный эллипс
//                      1e-6 <= ext <= 0.99
//
//**************************** 06.11.2009 *********************************//

Def<Ellipse2d> minEllipseAroundPoints ( double ext, CArrRef<Vector2d> points )
{
    Def<Ellipse2d> elli;
    if ( ext < 1e-6 || ext > 0.99 ) return elli;
// Преобразование множества точек к стандартному виду
    DynArray<Vector2d> temp ( points );
    const Def<Conform2d> conf = setStandardPosition ( temp );
    if ( ! conf.isDef ) return elli;
    List< Vertex<4> > stor;
    WireModel<4> model;
    model.simplex ( 10., stor );
    Double<4> d4;
    d4.d1 = d4.d2 = d4.d3 = 2.;
    d4.d0 = 0.;
    model.vlist -= d4;
    const double e2 = ext * ext;
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        Show< Vertex<4> > show ( model.vlist );
        const Vertex<4> * ptr = 0;
        double max = -1.;
        if ( show.top() )
        do
        {
            const Vertex<4> * p = show.cur();
            const double t = p->coor.d0 * p->coor.d0 + p->coor.d1 * p->coor.d1;
            if ( max < t ) max = t, ptr = p;
        }
        while ( show.next() );
        if ( ptr == 0 ) return elli;
// Поиск максимального нарушения ограничений для выбранного решения
        const Vector2d v01 ( ptr->coor.d0, ptr->coor.d1 );
        const Vector2d v23 ( ptr->coor.d2, ptr->coor.d3 );
        max = 0.;
        nat k, km = 0;
        for ( k = 0; k < temp.size(); ++k )
        {
            const Vector2d p ( v01 * temp[k] + v23.x, v01 % temp[k] + v23.y );
            const double t = p.x * p.x / e2 + p.y * p.y;
            if ( max < t ) max = t, km = k;
        }
        Vector2d p;
        double d = getDistanceElp ( ext, 1., Vector2d ( v01 * temp[km] + v23.x, v01 % temp[km] + v23.y ), p );
// Если нарушение мало, то завершение программы
        if ( d < 1e-8 )
        {
            Conform2d res = ~ ( Conform2d ( Spin2d ( atan2 ( v01.y, -v01.x ) ), -v23, sqrt ( v01 * v01 ) ) * conf );
            elli.a = res.magn * ext;
            elli.b = res.magn;
            elli.spin = res.spin;
            elli.o = res.trans;
            elli.isDef = true;
            return elli;
        }
        const Vector2d norm ( Vector2d ( p.x / e2, p.y ).setNorm2() );
// Применение ограничения к области допустимых преобразований
        Double<5> plane;
        plane.d0 = norm * temp[km];
        plane.d1 = norm % temp[km];
        plane.d2 = norm.x;
        plane.d3 = norm.y;
        plane.d4 = - ( norm * p );
        model.cut ( plane, stor );
    }
    return elli;
}

//**************************** 20.03.2009 *********************************//
//
//              Минимальный охватывающий свободный эллипс 
//                        A - минимум площади 
//
//**************************** 20.12.2010 *********************************//

namespace
{

class Ellipse
{
public:
    double a, b, c; // параметры
    Vector2d o; // центр

    double operator () ( const Vector2d & u ) const
    {
        const Vector2d v = u - o;
        return v.x * ( a * v.x + b * v.y ) + v.y * ( b * v.x + c * v.y );
    }
};

} // namespace

Def<Ellipse> minEllipseAround3PointsA ( const Vector2d & v1, const Vector2d & v2, const Vector2d & v3 )
{
    Def<Ellipse> res;
    const double x12 = v1.x - v2.x;
    const double x23 = v2.x - v3.x;
    const double x31 = v3.x - v1.x;
    const double y12 = v1.y - v2.y;
    const double y23 = v2.y - v3.y;
    const double y31 = v3.y - v1.y;
    const double d = _pow2 ( y31*x23 - y23*x31 );
    if ( d == 0 ) return res;
    res.a = ( y12 * y12 + y23 * y23 + y31 * y31 - y31 * y12 - y12 * y23 - y23 * y31 ) / d;
    res.b = ( y12 * x12 + y23 * x23 + y31 * x31 ) * ( -1.5 ) / d;
    res.c = ( x12 * x12 + x23 * x23 + x31 * x31 - x31 * x12 - x12 * x23 - x23 * x31 ) / d;
    res.o = ( v1 + v2 + v3 ) / 3;
    res.isDef = true;
    return res;
}

class EA4PA : public MathFunc1
{
public:
    double b0, d0, e0;
    double b1, d1, e1;

    double operator () ( double a ) const
    {
        const double b = b1 * a + b0;
        const double c = 1 - a;
        const double d = d1 * a + d0;
        const double e = e1 * a + e0;
        const double A = a * c - b * b;
        const double f = c * d - b * e;
        const double g = a * e - b * d;
        const double B = d * f + e * g;
        return 1.5 * B * ( 0.5 - a - b * b1 ) - A * ( d1 * f + e1 * g + 0.5 * ( e * e - d * d ) - b1 * d * e );
    }
};

class EA4PB : public MathFunc1
{
public:
    double a0, d0, e0;
    double a1, d1, e1;

    double operator () ( double b ) const
    {
        const double a = a1 * b + a0;
        const double c = 1 - a;
        const double d = d1 * b + d0;
        const double e = e1 * b + e0;
        const double A = a * c - b * b;
        const double f = c * d - b * e;
        const double g = a * e - b * d;
        const double B = d * f + e * g;
        return 1.5 * B * ( a1 * ( 0.5 - a ) - b ) - A * ( d1 * f + e1 * g + 0.5 * a1 * ( e * e - d * d ) - d * e );
    }
};

Def<Ellipse> minEllipseAround4PointsA ( const Vector2d & v1, const Vector2d & v2,
                                        const Vector2d & v3, const Vector2d & v4 )
{
    Def<Ellipse> res;
    const Vector2d u1 = v1 - v4;
    const Vector2d u2 = v2 - v4;
    const Vector2d u3 = v3 - v4;
    const double xy1 = u1.x * u1.y;
    const double xy2 = u2.x * u2.y;
    const double xy3 = u3.x * u3.y;
    SLU3<double> slu;
    slu.aa = 0.5 * ( u1.x * u1.x - u1.y * u1.y );
    slu.ba = 0.5 * ( u2.x * u2.x - u2.y * u2.y );
    slu.ca = 0.5 * ( u3.x * u3.x - u3.y * u3.y );
    slu.ab = u1.x; slu.ac = u1.y; slu.ad = -0.5 * u1.y * u1.y;
    slu.bb = u2.x; slu.bc = u2.y; slu.bd = -0.5 * u2.y * u2.y;
    slu.cb = u3.x; slu.cc = u3.y; slu.cd = -0.5 * u3.y * u3.y;
    const double w1 = slu.bb * slu.cc - slu.cb * slu.bc;
    const double w2 = slu.cb * slu.ac - slu.ab * slu.cc;
    const double w3 = slu.ab * slu.bc - slu.bb * slu.ac;
    const double da = xy1 * w1 + xy2 * w2 + xy3 * w3;
    const double db = slu.aa * w1 + slu.ba * w2 + slu.ca * w3;
    if ( fabs ( da ) > fabs ( db ) )
    {
        EA4PA f;
        slu.aa = xy1;
        slu.ba = xy2;
        slu.ca = xy3;
        f.b0 = ( slu.ad * ( slu.bb * slu.cc - slu.cb * slu.bc ) + 
                 slu.bd * ( slu.cb * slu.ac - slu.ab * slu.cc ) +
                 slu.cd * ( slu.ab * slu.bc - slu.bb * slu.ac ) ) / da;
        f.d0 = ( slu.aa * ( slu.bd * slu.cc - slu.cd * slu.bc ) + 
                 slu.ba * ( slu.cd * slu.ac - slu.ad * slu.cc ) +
                 slu.ca * ( slu.ad * slu.bc - slu.bd * slu.ac ) ) / da;
        f.e0 = ( slu.aa * ( slu.bb * slu.cd - slu.cb * slu.bd ) + 
                 slu.ba * ( slu.cb * slu.ad - slu.ab * slu.cd ) +
                 slu.ca * ( slu.ab * slu.bd - slu.bb * slu.ad ) ) / da;
        slu.ad = -0.5 * u1.x * u1.x - slu.ad;
        slu.bd = -0.5 * u2.x * u2.x - slu.bd;
        slu.cd = -0.5 * u3.x * u3.x - slu.cd;
        f.b1 = ( slu.ad * ( slu.bb * slu.cc - slu.cb * slu.bc ) + 
                 slu.bd * ( slu.cb * slu.ac - slu.ab * slu.cc ) +
                 slu.cd * ( slu.ab * slu.bc - slu.bb * slu.ac ) ) / da;
        f.d1 = ( slu.aa * ( slu.bd * slu.cc - slu.cd * slu.bc ) + 
                 slu.ba * ( slu.cd * slu.ac - slu.ad * slu.cc ) +
                 slu.ca * ( slu.ad * slu.bc - slu.bd * slu.ac ) ) / da;
        f.e1 = ( slu.aa * ( slu.bb * slu.cd - slu.cb * slu.bd ) + 
                 slu.ba * ( slu.cb * slu.ad - slu.ab * slu.cd ) +
                 slu.ca * ( slu.ab * slu.bd - slu.bb * slu.ad ) ) / da;
        double D = 1 - 4 * f.b0 * ( f.b1 + f.b0 );
        if ( D <= 0 ) return res;
        D = 0.5 * sqrt ( D );
        const double z = f.b1 * f.b1 + 1;
        const double w = 0.5 - f.b1 * f.b0;
        const double a1 = ( w - D ) / z + 1e-9;
        const double a2 = ( w + D ) / z - 1e-9;
        if ( a1 > a2 ) return res;
        if ( ! zeroin ( a1, a2, f, 0, res.a ) ) return res;
        res.b = f.b1 * res.a + f.b0;
        res.c = 1 - res.a;
        D = res.a * res.c - res.b * res.b;
        if ( D <= 0 ) return res;
        const double d = f.d1 * res.a + f.d0;
        const double e = f.e1 * res.a + f.e0;
        res.o = Vector2d ( e * res.b - d * res.c, d * res.b - e * res.a ) / D;
    }
    else
    {
        if ( db == 0 ) return res;
        EA4PB f;
        f.a0 = ( slu.ad * ( slu.bb * slu.cc - slu.cb * slu.bc ) + 
                 slu.bd * ( slu.cb * slu.ac - slu.ab * slu.cc ) +
                 slu.cd * ( slu.ab * slu.bc - slu.bb * slu.ac ) ) / db;
        f.d0 = ( slu.aa * ( slu.bd * slu.cc - slu.cd * slu.bc ) + 
                 slu.ba * ( slu.cd * slu.ac - slu.ad * slu.cc ) +
                 slu.ca * ( slu.ad * slu.bc - slu.bd * slu.ac ) ) / db;
        f.e0 = ( slu.aa * ( slu.bb * slu.cd - slu.cb * slu.bd ) + 
                 slu.ba * ( slu.cb * slu.ad - slu.ab * slu.cd ) +
                 slu.ca * ( slu.ab * slu.bd - slu.bb * slu.ad ) ) / db;
        slu.ad = - u1.x * u1.y;
        slu.bd = - u2.x * u2.y;
        slu.cd = - u3.x * u3.y;
        f.a1 = ( slu.ad * ( slu.bb * slu.cc - slu.cb * slu.bc ) + 
                 slu.bd * ( slu.cb * slu.ac - slu.ab * slu.cc ) +
                 slu.cd * ( slu.ab * slu.bc - slu.bb * slu.ac ) ) / db;
        f.d1 = ( slu.aa * ( slu.bd * slu.cc - slu.cd * slu.bc ) + 
                 slu.ba * ( slu.cd * slu.ac - slu.ad * slu.cc ) +
                 slu.ca * ( slu.ad * slu.bc - slu.bd * slu.ac ) ) / db;
        f.e1 = ( slu.aa * ( slu.bb * slu.cd - slu.cb * slu.bd ) + 
                 slu.ba * ( slu.cb * slu.ad - slu.ab * slu.cd ) +
                 slu.ca * ( slu.ab * slu.bd - slu.bb * slu.ad ) ) / db;
        double D = f.a1 * f.a1 + 4 * f.a0 * ( 1 - f.a0 );
        if ( D <= 0 ) return res;
        D = 0.5 * sqrt ( D );
        const double z = f.a1 * f.a1 + 1;
        const double w = f.a1 * ( 0.5 - f.a0 );
        const double b1 = ( w - D ) / z + 1e-9;
        const double b2 = ( w + D ) / z - 1e-9;
        if ( b1 > b2 ) return res;
        if ( ! zeroin ( b1, b2, f, 0, res.b ) ) return res;
        res.a = f.a1 * res.b + f.a0;
        res.c = 1 - res.a;
        D = res.a * res.c - res.b * res.b;
        if ( D <= 0 ) return res;
        const double d = f.d1 * res.b + f.d0;
        const double e = f.e1 * res.b + f.e0;
        res.o = Vector2d ( e * res.b - d * res.c, d * res.b - e * res.a ) / D;
    }
    const double d = res.o.x * ( res.a * res.o.x + res.b * res.o.y ) + res.o.y * ( res.b * res.o.x + res.c * res.o.y );
    if ( d == 0 ) return res;
    res.a /= d;
    res.b /= d;
    res.c /= d;
    res.o += v4;
    res.isDef = true;
    return res;
}

Def<Ellipse> minEllipseAround5PointsA ( ArrRef2<double> data, const Vector2d & v1, const Vector2d & v2,
                                        const Vector2d & v3, const Vector2d & v4, const Vector2d & v5 )
{
    Def<Ellipse> res;
    const Vector2d u1 = v1 - v5;
    const Vector2d u2 = v2 - v5;
    const Vector2d u3 = v3 - v5;
    const Vector2d u4 = v4 - v5;
    data[0][1] = u1.x * u1.y; data[0][4] = -0.5 * u1.y * u1.y; data[0][2] = u1.x; data[0][3] = u1.y;
    data[0][0] = 0.5 * u1.x * u1.x + data[0][4];
    data[1][1] = u2.x * u2.y; data[1][4] = -0.5 * u2.y * u2.y; data[1][2] = u2.x; data[1][3] = u2.y;
    data[1][0] = 0.5 * u2.x * u2.x + data[1][4];
    data[2][1] = u3.x * u3.y; data[2][4] = -0.5 * u3.y * u3.y; data[2][2] = u3.x; data[2][3] = u3.y;
    data[2][0] = 0.5 * u3.x * u3.x + data[2][4];
    data[3][1] = u4.x * u4.y; data[3][4] = -0.5 * u4.y * u4.y; data[3][2] = u4.x; data[3][3] = u4.y;
    data[3][0] = 0.5 * u4.x * u4.x + data[3][4];
    FixArray<double, 4> p;
    if ( ! slu_gauss ( data, p ) ) return res;
    res.a = p[0];
    res.b = p[1];
    res.c = 1 - res.a;
    const double D = res.a * res.c - res.b * res.b;
    if ( D <= 0 ) return res;
    const double d = p[2];
    const double e = p[3];
    res.o = Vector2d ( e * res.b - d * res.c, d * res.b - e * res.a ) / D;
    const double t = res.o.x * ( res.a * res.o.x + res.b * res.o.y ) + res.o.y * ( res.b * res.o.x + res.c * res.o.y );
    if ( t == 0 ) return res;
    res.a /= t;
    res.b /= t;
    res.c /= t;
    res.o += v5;
    res.isDef = true;
    return res;
}

Def<Ellipse2d> minEllipseAroundPointsA ( CArrRef<Vector2d> point )
{
    Def<Ellipse2d> res;
    const nat n = point.size();
    switch ( n )
    {
    case 2:
        {
            const Vector2d v = point[1] - point[0];
            if ( !! v )
            {
                res.a = 0.5 * norm2 ( v );
                res.b = 0;
                res.o = 0.5 * ( point[0] + point[1] );
                res.spin = Spin2d ( atan2 ( v.y, v.x ) );
                res.isDef = true;
                return res;
            }
        }
    case 1:
        res.a = res.b = 0;
        res.o = point[0];
        res.isDef = true;
    case 0:
        return res;
    }
    nat i, im = 0;;
    CmbArray<nat, 5> support, support2;
    support.resize ( 3 );
// Находим первую опорную точку
    double max = qmod ( point[0] );
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[i] );
        if ( max < q ) max = q, im = i;
    }
    support[0] = im;
// Находим вторую опорную точку
    max = 0.;
    for ( i = 0; i < n; ++i )
    {
        const double q = qmod ( point[i] - point[support[0]] );
        if ( max < q ) max = q, im = i;
    }
    if ( max == 0 )
    {
        res.a = res.b = 0;
        res.o = point[support[0]];
        return res;
    }
    support[1] = im;
// Находим третью опорную точку
    max = 0.;
    const Vector2d v = point[support[1]] - point[support[0]];
    for ( i = 0; i < n; ++i )
    {
        const Vector2d u = point[i] - point[support[0]];
        const double t = fabs ( u % v );
        if ( max < t ) max = t, im = i;
    }
    if ( max < 1e-12 * ( v * v ) )
    {
        res.a = 0.5 * norm2 ( v );
        res.b = 0;
        res.o = 0.5 * ( point[support[1]] + point[support[0]] );
        res.spin = Spin2d ( atan2 ( v.y, v.x ) );
        return res;
    }
    support[2] = im;
    Def<Ellipse> e = minEllipseAround3PointsA ( point[support[0]], point[support[1]], point[support[2]] );
    if ( ! e.isDef ) return res;
    double q = e.a * e.c - e.b * e.b;
    FixArray2<double, 4, 5> data;
    for (;;)
    {
// Ищем точку вне эллипса
        max = 1;
        for ( i = 0; i < n; ++i )
        {
            const double d = e ( point[i] );
            if ( max < d ) max = d, im = i;
        }
        if ( max < 1 + 1e-14 || hasEqu ( support, im ) ) break;
// Находим минимальный эллипс охватывающий текущие опорные точки и точку point[im]
        Def<Ellipse> ee;
        nat i1, i2, i3, i4;
        for ( i1 = 1; i1 < support.size(); ++i1 )
        {
            const Vector2d & v1 = point[support[i1]];
            if ( ee.isDef && ee ( v1 ) < 1 + 1e-14 ) continue;
            ee.isDef = false;
            for ( i2 = 0; i2 < i1; ++i2 )
            {
                const Vector2d & v2 = point[support[i2]];
                if ( ee.isDef && ee ( v2 ) < 1 + 1e-14 ) continue;
                ee = minEllipseAround3PointsA ( point[im], v1, v2 );
                support2.resize ( 3 );
                support2[0] = support[i2];
                support2[1] = support[i1];
                support2[2] = im;
                for ( i3 = 0; i3 < i2; ++i3 )
                {
                    const Vector2d & v3 = point[support[i3]];
                    if ( ee ( v3 ) < 1 + 1e-14 ) continue;
                    ee = minEllipseAround4PointsA ( point[im], v1, v2, v3 );
                    if ( ! ee.isDef ) return res;
                    support2.resize ( 4 );
                    support2[0] = support[i3];
                    support2[1] = support[i2];
                    support2[2] = support[i1];
                    support2[3] = im;
                    for ( i4 = 0; i4 < i3; ++i4 )
                    {
                        const Vector2d & v4 = point[support[i4]];
                        if ( ee ( v4 ) < 1 + 1e-14 ) continue;
                        ee = minEllipseAround5PointsA ( data, point[im], v1, v2, v3, v4 );
                        if ( ! ee.isDef ) return res;
                        support2.resize ( 5 );
                        support2[0] = support[i4];
                        support2[1] = support[i3];
                        support2[2] = support[i2];
                        support2[3] = support[i1];
                        support2[4] = im;
                    }
                }
            }
        }
// Если площадь эллипса не увеличилась, то выходим из цикла
        const double t = ee.a * ee.c - ee.b * ee.b;
        if ( q <= t ) break;
        q = t;
        e = ee;
        support = support2;
    }
// Заполняем результат
    res.spin = Spin2d ( 0.5 * atan2 ( e.b + e.b, e.a - e.c ) );
    Vector2d ax, ay;
    res.spin.getReper ( ax, ay );
    res.a = ay.x * ( e.c * ay.x + e.b * ay.y ) + ay.y * ( e.b * ay.x + e.a * ay.y );
    res.b = ax.x * ( e.c * ax.x + e.b * ax.y ) + ax.y * ( e.b * ax.x + e.a * ax.y );
    if ( res.a <= 0 || res.b <= 0 ) return res;
    res.a = 1 / sqrt ( res.a );
    res.b = 1 / sqrt ( res.b );
    res.o = e.o;
    res.isDef = true;
    return res;
}

//**************************** 23.05.2009 *********************************//
//
//              Минимальный охватывающий треугольник
//
//**************************** 18.02.2018 *********************************//

inline bool gamma ( const Vector2d & pc, const Vector2d & vc,
                    const Vector2d & pa, const Vector2d & va,
                    const Vector2d & p1, Vector2d & p2 )
{
    const double s = vc % va;
    if ( s == 0 ) return false;
    p2 = pa + va * ( vc % ( p1 - pc + p1 - pa ) / s );
    return true;
}

inline bool intersect ( const Vector2d & pc, const Vector2d & vc,
                        const Vector2d & pa, const Vector2d & va, Vector2d & p )
{
    const double s = vc % va;
    if ( s == 0 ) return false;
    p = pa + va * ( vc % ( pc - pa ) / s );
    return true;
}

Def<Triangle2d> minTriangleAroundConvexPolygonA ( CArrRef<Vector2d> poly )
{
    Def<Triangle2d> res;
    const nat n = poly.size();
    if ( n < 2 ) return res;
    if ( n == 3 )
    {
        return Triangle2d ( poly[0], poly[1], poly[2] );
    }
    double min = 1e300;
    nat ia = 1, ib = 2, icp = n - 1;
    for ( nat ic = 0; ic < n; ++ic )
    {
        if ( ia == ic )
        {
            if ( ++ia == n ) ia = 0;
            if ( ia == ib )
                if ( ++ib == n ) ib = 0;
        }
        const Vector2d & pc = poly[ic];
        const Vector2d vc = pc - poly[icp];
        nat ibn = ib + 1;
        if ( ibn == n ) ibn = 0;
        while ( ibn != ic && vc % ( poly[ibn] - poly[ib] ) >= 0 )
        {
            ib = ibn;
            if ( ++ibn == n ) ibn = 0;
        }
        nat iap = ia > 0 ? ia - 1 : n - 1;
        Vector2d va = poly[ia] - poly[iap];
        while ( vc % ( poly[ib] - poly[ia] ) > 0 )
        {
            Vector2d ga;
            if ( ! gamma ( pc, vc, poly[iap], va, poly[ia], ga ) ) return res;
            if ( ( poly[ib] - ga ) % ( poly[ibn] - poly[ib] ) < 0 )
            {
                if ( ibn == ic ) break;
                ib = ibn;
                if ( ++ibn == n ) ibn = 0;
            }
            else
            {
                if ( ia == ( ib > 0 ? ib - 1 : n - 1 ) )
                {
                    if ( ibn == ic ) break;
                    ib = ibn;
                    if ( ++ibn == n ) ibn = 0;
                }
                iap = ia;
                if ( ++ia == n ) ia = 0;
                va = poly[ia] - poly[iap];
            }
        }
        const Vector2d & pa = poly[iap];
        while ( ibn != ic && vc % ( pa - poly[ib] ) <= 0 )
        {
            Vector2d gb;
            if ( ! gamma ( pc, vc, pa, va, poly[ib], gb ) ) return res;
            if ( ( poly[ib] - gb ) % ( poly[ibn] - poly[ib] ) > 0 ) break;
            ib = ibn;
            if ( ++ibn == n ) ibn = 0;
        }
        Vector2d gb;
        if ( ! gamma ( pc, vc, pa, va, poly[ib], gb ) ) return res;
        const Vector2d & pb = poly[ib];
        const nat ibp = ib > 0 ? ib - 1 : n - 1;
        const Vector2d vb = poly[ibp] - pb;
        Vector2d a, b, c;
        if ( ( pb - gb ) % vb < 0 || vc % ( pa - pb ) > 0 ) 
        {
            if ( ibp == ia ) a = poly[ia];
            else
                if ( ! intersect ( pa, va, pb, vb, a ) ) return res;
            if ( ibn == ic ) b = poly[ib];
            else
                if ( ! intersect ( pb, vb, pc, vc, b ) ) return res;
            if ( vc % ( pa - 0.5 * ( a + b ) ) > 0 ) 
            {
                if ( ! gamma ( pc, vc, pb, vb, pa, a ) ) return res;
                if ( ! intersect ( pc, vc, pa, a - pa, c ) ) return res;
            }
            else
            {
                if ( iap == ic ) c = poly[ic];
                else
                    if ( ! intersect ( pc, vc, pa, va, c ) ) return res;
            }
        }
        else
        {
            a = gb;
            if ( ! intersect ( pc, vc, pb, a - pb, b ) ) return res;
            if ( iap == ic ) c = poly[ic];
            else
                if ( ! intersect ( pc, vc, pa, va, c ) ) return res;
        }
        const double area = ( b - a ) % ( c - b );
        if ( min > area )
        {
            min = area;
            res.a = a;
            res.b = b;
            res.c = c;
        }
        icp = ic;
    }
    res.isDef = true;
    return res;
}

// Простой многоугольник

Def<Triangle2d> minTriangleAroundPolygonA ( CArrRef<Vector2d> inner )
{
    return minTriangleAroundConvexPolygonA ( convexPolygon ( inner, DynArray<Vector2d>() ) );
}

// Множество точек

Def<Triangle2d> minTriangleAroundPointsA ( CArrRef<Vector2d> inner )
{
    return minTriangleAroundConvexPolygonA ( convexNlogN ( inner, Suite<Vector2d> ( inner.size() ) ) );
}

//**************************** 12.07.2009 *********************************//
//
//              Минимальный охватывающий прямоугольник
//              A - минимум площади, Р - минимум периметра
//
//**************************** 14.03.2010 *********************************//

void minRectangleAroundConvexPolygon ( CArrRef<Vector2d> inner, const MathFunc2 & func,
                                       Vector2d & ax, double & x1, double & x2,
                                       Vector2d & ay, double & y1, double & y2 )
{
    nat n = inner.size();
    switch ( n )
    {
    case 0: ax.x = ay.y = 1; ax.y = ay.x = 0; 
        x1 = x2 = y1 = y2 = 0; 
        return;
    case 1: ax.x = ay.y = 1; ax.y = ay.x = 0; 
        x1 = x2 = inner[0].x; y1 = y2 = inner[0].y; 
        return;
    case 2: ax = inner[1] - inner[0];
        if ( ! ax )
        {
            ax.x = ay.y = 1; ax.y = ay.x = 0; 
            x1 = x2 = inner[0].x; y1 = y2 = inner[0].y;
        }
        else
        {
            ay = ax.setNorm2().leftPerpendicular();
            x1 = ax * inner[0];
            x2 = ax * inner[1];
            y1 = y2 = ay * inner[0];
        }
        return;
    }
    Vector2d v1 = inner[0] - inner[n-1];
    while ( ! v1 )
    {
        if ( --n < 3 )
        {
            ax = inner[1] - inner[0];
            if ( ! ax )
            {
                ax.x = ay.y = 1; ax.y = ay.x = 0; 
                x1 = x2 = inner[0].x; y1 = y2 = inner[0].y;
            }
            else
            {
                ay = ax.setNorm2().leftPerpendicular();
                x1 = ax * inner[0];
                x2 = ax * inner[1];
                y1 = y2 = ay * inner[0];
            }
            return;
        }
        v1 = inner[0] - inner[n-1];
    }
    Vector2d v2 = v1.setNorm2().leftPerpendicular();
    double min1 = v1 * inner[0];
    double min2 = v2 * inner[0];
    double max1 = min1;
    double max2 = min2;
    nat i, imin1 = 0, imin2 = 0, imax1 = 0, imax2 = 0;
    for ( i = 1; i < n; ++i )
    {
        const Vector2d & v = inner[i];
        const double t1 = v1 * v;
        if ( max1 < t1 ) max1 = t1, imax1 = i; else
        if ( min1 > t1 ) min1 = t1, imin1 = i;
        const double t2 = v2 * v;
        if ( max2 < t2 ) max2 = t2, imax2 = i;
    }
    double min = func ( max1 - min1, max2 - min2 );
    nat gmin1 = imin1, gmin2 = 0, gmax1 = imax1, gmax2 = imax2;
    ax = v1;
    for ( i = 1; i < n; ++i )
    {
        v1 = inner[i] - inner[i-1];
        if ( ! v1 ) continue;
        v1.setNorm2();
        nat j;
        min1 = v1 * inner[imin1];
        for ( j = imin1+1; ; ++j )
        {
            if ( j == n ) j = 0;
            const double t = v1 * inner[j];
            if ( min1 > t ) min1 = t, imin1 = j;
            else break;
        }
        max1 = v1 * inner[imax1];
        for ( j = imax1+1; ; ++j )
        {
            if ( j == n ) j = 0;
            const double t = v1 * inner[j];
            if ( max1 < t ) max1 = t, imax1 = j;
            else break;
        }
        v2 = v1.leftPerpendicular();
        max2 = v2 * inner[imax2];
        for ( j = imax2+1; ; ++j )
        {
            if ( j == n ) j = 0;
            const double t = v2 * inner[j];
            if ( max2 < t ) max2 = t, imax2 = j;
            else break;
        }
        min2 = v2 * inner[i];
        const double a = func ( max1 - min1, max2 - min2 );
        if ( min > a )
        {
            min = a;
            gmin1 = imin1, gmin2 = i, gmax1 = imax1, gmax2 = imax2;
            ax = v1;
        }
    }
    x1 = ax * inner[gmin1];
    x2 = ax * inner[gmax1];
    ay = ax.leftPerpendicular();
    y1 = ay * inner[gmin2];
    y2 = ay * inner[gmax2];
}

Def<Rectangle2d> minRectangleAroundConvexPolygon ( CArrRef<Vector2d> inner, const MathFunc2 & func )
{
    Def<Rectangle2d> res;
    if ( inner.size() == 0 ) return res;
    Vector2d ax, ay;
    double x1, x2, y1, y2;
    minRectangleAroundConvexPolygon ( inner, func, ax, x1, x2, ay, y1, y2 );
    res.a = 0.5 * ( x2 - x1 );
    res.b = 0.5 * ( y2 - y1 );
    res.o = ( 0.5 * ( x2 + x1 ) ) * ax + ( 0.5 * ( y2 + y1 ) ) * ay;
    res.spin = Spin2d ( atan2 ( ax.y, ax.x ) );
    res.isDef = true;
    return res;
}

// Выпуклый многоугольник

Def<Rectangle2d> minRectangleAroundConvexPolygonA ( CArrRef<Vector2d> inner )
{
    return minRectangleAroundConvexPolygon ( inner, MathMul() );
}

Def<Rectangle2d> minRectangleAroundConvexPolygonP ( CArrRef<Vector2d> inner )
{
    return minRectangleAroundConvexPolygon ( inner, MathAdd() );
}

// Простой многоугольник

Def<Rectangle2d> minRectangleAroundPolygonA ( CArrRef<Vector2d> inner )
{
    return minRectangleAroundConvexPolygonA ( convexPolygon ( inner, DynArray<Vector2d>() ) );
}

Def<Rectangle2d> minRectangleAroundPolygonP ( CArrRef<Vector2d> inner )
{
    return minRectangleAroundConvexPolygonP ( convexPolygon ( inner, DynArray<Vector2d>() ) );
}

// Множество точек

Def<Rectangle2d> minRectangleAroundPointsA ( CArrRef<Vector2d> inner )
{
    return minRectangleAroundConvexPolygonA ( convexNlogN ( inner, Suite<Vector2d> ( inner.size() ) ) );
}

Def<Rectangle2d> minRectangleAroundPointsP ( CArrRef<Vector2d> inner )
{
    return minRectangleAroundConvexPolygonP ( convexNlogN ( inner, Suite<Vector2d> ( inner.size() ) ) );
}

//**************************** 23.04.2009 *********************************//
//
//                  Вспомогательные классы и функции
//
//**************************** 17.10.2015 *********************************//

static double findMin ( const MathFunc1 & func )
{
    nat im = 9;
    double min = func ( 1 );
    for ( nat i = 0; i < 9; ++i )
    {
        const double a = func ( ( 1 + i * 7 ) / 64. );
        if ( min > a ) min = a, im = i;
    }
    const double a = im == 0 ? 1. / 64 : ( im * 7 - 6 ) / 64.;
    const double b = im == 9 ? 1.      : ( im * 7 + 8 ) / 64.;
    return fmin ( a, b, func, 0.001 );
}

class MinRhombusAroundPoints : public MathFunc1
{
protected:
    mutable FixArray<Vector2d, 4> vect;
    mutable Def<Conform2d> conf;
    mutable double min;
    CArrRef<Vector2d> inner;
public:

    explicit MinRhombusAroundPoints ( CArrRef<Vector2d> p ) : inner(p), min(1e300)
    {
        vect[0].x = 1;
        vect[1].x = vect[3].x = 0;
        vect[2].x = -1;
        vect[0].y = vect[2].y = 0;
    }
    Def<Rhombus2d> run () const
    {
        Def<Rhombus2d> res;
        const double y = findMin ( *this );
        if ( conf.isDef )
        {
            conf = ~conf;
            res.a = conf.magn;
            res.b = conf.magn * y;
            res.spin = conf.spin;
            res.o = conf.trans;
            res.isDef = true;
        }
        return res;
    }
};

//**************************** 17.10.2015 *********************************//
//
//                    Минимальный охватывающий ромб
//              A - минимум площади, Р - минимум периметра
//
//**************************** 17.10.2015 *********************************//

class MinRhombusAroundPointsA : public MinRhombusAroundPoints
{
public:

    explicit MinRhombusAroundPointsA ( CArrRef<Vector2d> p ) : MinRhombusAroundPoints ( p ) {}
    virtual double operator () ( double y ) const
    {
        vect[1].y =  y;
        vect[3].y = -y;
        Def<Conform2d> c = maxPolygonInConvexPolygon ( inner, vect );
        if ( ! c.isDef ) return 1e300;
        const double a = y / ( c.magn * c.magn );
        if ( min > a ) min = a, conf = c;
        return a;
    }
};

Def<Rhombus2d> minRhombusAroundPointsA ( CArrRef<Vector2d> inner )
{
    return MinRhombusAroundPointsA ( inner ).run();
}

class MinRhombusAroundPointsP : public MinRhombusAroundPoints
{
public:

    explicit MinRhombusAroundPointsP ( CArrRef<Vector2d> p ) : MinRhombusAroundPoints ( p ) {}
    virtual double operator () ( double y ) const
    {
        vect[1].y =  y;
        vect[3].y = -y;
        Def<Conform2d> c = maxPolygonInConvexPolygon ( inner, vect );
        if ( ! c.isDef ) return 1e300;
        const double a = ( 1 + y * y ) / ( c.magn * c.magn );
        if ( min > a ) min = a, conf = c;
        return a;
    }
};

Def<Rhombus2d> minRhombusAroundPointsP ( CArrRef<Vector2d> inner )
{
    return MinRhombusAroundPointsP ( inner ).run();
}

//**************************** 23.05.2009 *********************************//
//
//              Минимальный охватывающий параллелограмм
//
//**************************** 16.02.2018 *********************************//

struct Apod
{
    nat i1, i2, im;
    Vector2d dir;
};

Def<Parallelogram2d> minParallelogramAroundConvexPolygonA ( CArrRef<Vector2d> inner )
{
    Def<Parallelogram2d> res;
    const nat n = inner.size();
    if ( n < 3 ) return res;
// Создание антиподальных пар
    Suite<Apod> apod ( n );
    Apod a;
    a.i1 = n - 1;
    a.im = 1;
    for ( a.i2 = 0; a.i2 < n; ++a.i2 )
    {
        a.dir = inner[a.i2] - inner[a.i1];
        if ( ! a.dir ) continue;
        a.dir.setNorm2();
        double max = a.dir % inner[a.im];
        for ( nat j = a.im+1; ; ++j )
        {
            if ( j == n ) j = 0;
            const double t = a.dir % inner[j];
            if ( max < t ) max = t, a.im = j;
            else break;
        }
        apod.inc() = a;
        a.i1 = a.i2;
    }
    if ( apod.size() < 3 ) return res;
// Поиск минимума среди допустимых двоек антиподальных пар
    double min = 1e300;
    nat i, j = 1, i1, i2;
    for ( i = 0; i < apod.size(); ++i )
    {
        const Apod & ai = apod[i];
        if ( i == j )
        {
            if ( ++j == apod.size() ) j = 0;
        }
        for(;;)
        {
            nat k = j + 1;
            if ( k == apod.size() ) k = 0;
            if ( k == i ) break;
            const Apod & ak = apod[k];
            if ( ai.dir % inner[ak.i1] > ai.dir % inner[ak.im] ) break;
            j = k;
        }
        const Apod & aj = apod[j];
        const double s = ai.dir % aj.dir;
        if ( s == 0 ) continue;
        const double d = fabs ( ( ai.dir % ( inner[ai.im] - inner[ai.i1] ) ) *
                                ( aj.dir % ( inner[aj.im] - inner[aj.i1] ) ) / s );
        if ( min > d ) min = d, i1 = i, i2 = j;
    }
    if ( min == 1e300 ) return res;
// Расчёт вершин минимального параллелограмма
    SLU2<double> slu;
    slu.aa = - apod[i1].dir.y;   slu.ab = apod[i1].dir.x;   slu.ac = apod[i1].dir % inner[apod[i1].i1];
    slu.ba = - apod[i2].dir.y;   slu.bb = apod[i2].dir.x;   slu.bc = apod[i2].dir % inner[apod[i2].i1];
    Vector2d outer[3];
    slu.gauss ( outer[0].x, outer[0].y );
    slu.ac = apod[i1].dir % inner[apod[i1].im];
    slu.gauss ( outer[1].x, outer[1].y );
    slu.bc = apod[i2].dir % inner[apod[i2].im];
    slu.gauss ( outer[2].x, outer[2].y );
    return Parallelogram2d ( outer[0], outer[1], outer[2] );
}

// Простой многоугольник

Def<Parallelogram2d> minParallelogramAroundPolygonA ( CArrRef<Vector2d> inner )
{
    return minParallelogramAroundConvexPolygonA ( convexPolygon ( inner, DynArray<Vector2d>() ) );
}

// Множество точек

Def<Parallelogram2d> minParallelogramAroundPointsA ( CArrRef<Vector2d> inner )
{
    return minParallelogramAroundConvexPolygonA ( convexNlogN ( inner, Suite<Vector2d> ( inner.size() ) ) );
}

//**************************** 17.11.2015 *********************************//
//
//                  Минимальная охватывающая трапеция
//
//**************************** 17.11.2015 *********************************//

bool minTrapezoidAroundConvexPolygonA ( FixArrRef<Vector2d, 4> & outer, CArrRef<Vector2d> inner )
{
    const nat n = inner.size();
    if ( n < 3 ) return false;
    nat im = 1;
    double min = 1e300;
    FixArray<Vector2d, 4> temp;
    for ( nat i = 0; i < n; ++i )
    {
        const nat ip = i > 0 ? i - 1 : n - 1;
        const Vector2d norm = ( inner[i] - inner[ip] ).leftPerpendicular().setNorm2();
        double max = norm * inner[im];
        for ( nat j = im+1; ; ++j )
        {
            if ( j == n ) j = 0;
            const double t = norm * inner[j];
            if ( max < t ) max = t, im = j;
            else break;
        }
        const double mid = 0.5 * ( norm * ( inner[i] + inner[im] ) );
        nat i1 = i + 1;
        if ( i1 == n ) i1 = 0;
        if ( i1 != im )
        {
            while ( norm * inner[i1] < mid )
            {
                i1 = i1 + 1;
                if ( i1 == n ) i1 = 0;
                if ( i1 == im ) break;
            }
        }
        nat i2 = im + 1;
        if ( i2 == n ) i2 = 0;
        if ( i2 != ip )
        {
            while ( norm * inner[i2] > mid )
            {
                i2 = i2 + 1;
                if ( i2 == n ) i2 = 0;
                if ( i2 == ip ) break;
            }
        }
        const Line2d line1 ( norm, - ( norm * inner[i] ) );
        const Line2d line3 ( norm, - ( norm * inner[im] ) );
        const nat i1p = i1 > 0 ? i1 - 1 : n - 1;
        const nat i2p = i2 > 0 ? i2 - 1 : n - 1;
        const Line2d line2 ( inner[i1p], inner[i1] );
        const Line2d line4 ( inner[i2p], inner[i2] );
        Def<Vector2d> p1 = intersection ( line1, line2 );
        if ( ! p1.isDef ) return false;
        Def<Vector2d> p2 = intersection ( line2, line3 );
        if ( ! p2.isDef ) return false;
        Def<Vector2d> p3 = intersection ( line3, line4 );
        if ( ! p3.isDef ) return false;
        Def<Vector2d> p4 = intersection ( line4, line1 );
        if ( ! p4.isDef ) return false;
        temp[0] = p1;
        temp[1] = p2;
        temp[2] = p3;
        temp[3] = p4;
        const double a = area ( temp );
        if ( min > a )
        {
            min = a;
            outer = temp;
        }
    }
    return true;
}

// Простой многоугольник

bool minTrapezoidAroundPolygonA ( FixArrRef<Vector2d, 4> & outer, CArrRef<Vector2d> inner )
{
    return minTrapezoidAroundConvexPolygonA ( outer, convexPolygon ( inner, DynArray<Vector2d>() ) );
}

// Множество точек

bool minTrapezoidAroundPointsA ( FixArrRef<Vector2d, 4> & outer, CArrRef<Vector2d> inner )
{
    return minTrapezoidAroundConvexPolygonA ( outer, convexNlogN ( inner, Suite<Vector2d> ( inner.size() ) ) );
}

//**************************** 22.02.2016 *********************************//
//
//                  Минимальный охватывающий N-угольник
//
//**************************** 22.02.2016 *********************************//

typedef ListItem<Set2<Segment2d, Set2<nat> > > SegmItem;
typedef List<SegmItem> SegmList;

inline void _swap ( _ChainLink<SegmItem> & p1, _ChainLink<SegmItem> & p2 )
{
    const _ChainLink<SegmItem> p ( p1 );
    p1 = p2;
    p2 = p;
    ::_swap ( p1.tail->info, p2.tail->info );
}

Vector2d intersect ( const SegmItem * pp, const SegmItem * np )
{
    const Segment2d & ps = pp->a;
    const Segment2d & ns = np->a;
    SLU2<double> slu;
    slu.aa = ps.b.y - ps.a.y; slu.ab = ps.a.x - ps.b.x; slu.ac = ps.a % ps.b;
    slu.ba = ns.b.y - ns.a.y; slu.bb = ns.a.x - ns.b.x; slu.bc = ns.a % ns.b;
    Vector2d v;
    slu.gauss ( v.x, v.y );
    return v;
}

void recalc ( SegmItem * pp, SegmItem * cp, SegmItem * np, const Vector2d & o2 )
{
    Segment2d & ps = pp->a;
    Segment2d & ns = np->a;
    SLU2<double> slu;
    slu.aa = ns.b.x - ns.a.x; slu.ab = ps.b.x - ps.a.x; slu.ac = o2.x - ns.a.x - ps.a.x;
    slu.ba = ns.b.y - ns.a.y; slu.bb = ps.b.y - ps.a.y; slu.bc = o2.y - ns.a.y - ps.a.y;
    double s, t;
    slu.gauss ( s, t );
    cp->a.a = ps.b = ps.a + t * ( ps.b - ps.a );
    cp->a.b = ns.a = ns.a + s * ( ns.b - ns.a );
    pp->info = pp->info = 0;
}

class SegmListMinArea
{
    SegmList & zol;
public:
    explicit SegmListMinArea ( SegmList & z ) : zol(z) {}
    double operator() ( const SegmItem * o )
    {
        Show<SegmItem> show ( zol );
        const SegmItem * pp = show.jump ( o ).cprev();
        const SegmItem * np = show.jump ( o ).cnext();
        const Vector2d v = intersect ( pp, np );
        return fabs ( ( v - o->a.a ) % ( v - o->a.b ) );
    }
    void operator() ( SegmItem * pp, SegmItem * cp, SegmItem * np )
    {
        zol.del ( cp );
        np->a.a = pp->a.b = intersect ( pp, np );
    }
};

bool minTrapezoidAroundConvexPolygon ( CArrRef<Vector2d> inner, SegmList & outer )
{
    const nat n = inner.size();
    if ( n < 3 ) return false;
    Set2<Segment2d, Set2<nat> > out1, out2, out3, out4;
    nat im = 1;
    double min = 1e300;
    FixArray<Vector2d, 4> temp;
    for ( nat i = 0; i < n; ++i )
    {
        const nat ip = i > 0 ? i - 1 : n - 1;
        const Vector2d norm = ( inner[i] - inner[ip] ).leftPerpendicular().setNorm2();
        double max = norm * inner[im];
        for ( nat j = im+1; ; ++j )
        {
            if ( j == n ) j = 0;
            const double t = norm * inner[j];
            if ( max < t ) max = t, im = j;
            else break;
        }
        const double mid = 0.5 * ( norm * ( inner[i] + inner[im] ) );
        nat i1 = i + 1;
        if ( i1 == n ) i1 = 0;
        if ( i1 != im )
        {
            while ( norm * inner[i1] < mid )
            {
                i1 = i1 + 1;
                if ( i1 == n ) i1 = 0;
                if ( i1 == im ) break;
            }
        }
        nat i2 = im + 1;
        if ( i2 == n ) i2 = 0;
        if ( i2 != ip )
        {
            while ( norm * inner[i2] > mid )
            {
                i2 = i2 + 1;
                if ( i2 == n ) i2 = 0;
                if ( i2 == ip ) break;
            }
        }
        const Line2d line1 ( norm, - ( norm * inner[i] ) );
        const Line2d line3 ( norm, - ( norm * inner[im] ) );
        const nat i1p = i1 > 0 ? i1 - 1 : n - 1;
        const nat i2p = i2 > 0 ? i2 - 1 : n - 1;
        const Line2d line2 ( inner[i1p], inner[i1] );
        const Line2d line4 ( inner[i2p], inner[i2] );
        Def<Vector2d> p1 = intersection ( line1, line2 );
        if ( ! p1.isDef ) return false;
        Def<Vector2d> p2 = intersection ( line2, line3 );
        if ( ! p2.isDef ) return false;
        Def<Vector2d> p3 = intersection ( line3, line4 );
        if ( ! p3.isDef ) return false;
        Def<Vector2d> p4 = intersection ( line4, line1 );
        if ( ! p4.isDef ) return false;
        temp[0] = p1;
        temp[1] = p2;
        temp[2] = p3;
        temp[3] = p4;
        const double a = area ( temp );
        if ( min > a )
        {
            min = a;
            out1.a.a = p4;
            out1.a.b = p1;
            out1.b.a = ip;
            out1.b.b = i;
            out2.a.a = p1;
            out2.a.b = p2;
            out2.b.a = i1p;
            out2.b.b = i1;
            out3.a.a = p2;
            out3.a.b = p3;
            out3.b.a = im;
            out3.b.b = im;
            out4.a.a = p3;
            out4.a.b = p4;
            out4.b.a = i2p;
            out4.b.b = i2;
        }
    }
    outer.addAftLas ( new SegmItem ( out1 ) );
    outer.addAftLas ( new SegmItem ( out2 ) );
    outer.addAftLas ( new SegmItem ( out3 ) );
    outer.addAftLas ( new SegmItem ( out4 ) );
    return true;
}

inline void recalc ( SegmList & list, SegmItem * p, CArrRef<Vector2d> inner )
{
    SegmItem * pp = list.cprev();
    list.jump ( p );
    SegmItem * np = list.cnext();
    list.jump ( p );
    p->a.a = inner[p->b.a];
    p->a.b = inner[p->b.b];
    pp->a.b = p->a.a = intersect ( pp, p );
    np->a.a = p->a.b = intersect ( np, p );
    pp->info = pp->info = 0;
}

// Подвигаем стороны N-угольника
void tuning ( SegmList & list, CArrRef<Vector2d> inner )
{
    const nat m = inner.size();
    list.setAllInfo(0);
    for ( nat i = 0; i < 4; ++i )
    {
        bool isChange = false;
        list.top();
        do
        {
            SegmItem * p = list.cur();
            if ( p->info ) continue;
            p->info = 1;
            Vector2d u = p->a.b - p->a.a;
            double t1 = 0.5 * ( u * u );
            if ( p->b.a == p->b.b )
            {
                double q = u * u;
                double a = qmod ( inner[p->b.a] - p->a.a );
                double b = qmod ( inner[p->b.a] - p->a.b );
                if ( fabs ( a - b ) < 1e-6 * q )
                    continue;
                nat j = p->b.b + 1;
                if ( j == m ) j = 0;
                double t2 = u * ( inner[j] - p->a.a );
            }
            double t2 = u * ( inner[p->b.a] - p->a.a );
            double t3 = u * ( inner[p->b.b] - p->a.a );
m1:         if ( t1 < t2 )
            {
                isChange = true;
                p->b.b = p->b.a;
                p->b.a = p->b.a > 0 ? p->b.a - 1 : m - 1;
                recalc ( list, p, inner );
                u = p->a.b - p->a.a;
                t1 = 0.5 * ( u * u );
                t3 = u * ( inner[p->b.b] - p->a.a );
                if ( t1 > t3 )
                {
                    SegmItem * p1 = list.cprev();
                    list.jump ( p );
                    SegmItem * p2 = list.cnext();
                    list.jump ( p );
                    recalc ( p1, p, p2, 2 * inner[p->b.b] );
                    p->b.a = p->b.b;
                }
                else
                {
                    t2 = u * ( inner[p->b.a] - p->a.a );
                    goto m1;
                }
            }
            else
m2:         if ( t1 > t3 )
            {
                isChange = true;
                p->b.a = p->b.b;
                if ( ++p->b.b == m ) p->b.b = 0;
                recalc ( list, p, inner );
                u = p->a.b - p->a.a;
                t1 = 0.5 * ( u * u );
                t2 = u * ( inner[p->b.a] - p->a.a );
                if ( t1 < t2 )
                {
                    SegmItem * p1 = list.cprev();
                    list.jump ( p );
                    SegmItem * p2 = list.cnext();
                    list.jump ( p );
                    recalc ( p1, p, p2, 2 * inner[p->b.a] );
                    p->b.b = p->b.a;
                }
                else
                {
                    t3 = u * ( inner[p->b.b] - p->a.a );
                    goto m2;
                }
            }
        }
        while ( list.next() );
        if ( ! isChange ) break;
    }
}

inline double area2 ( SegmList & list )
{
    double a = 0;
    list.top();
    do
    {
        SegmItem * p = list.cur();
        a += p->a.a % p->a.b;
    }
    while ( list.next() );
    return a;
}

bool minNgonAroundConvexPolygonA ( ArrRef<Vector2d> outer, CArrRef<Vector2d> inner )
{
    const nat n = outer.size();
    const nat m = inner.size();
    if ( n < 3 || m < n ) return false;
    if ( n == m )
    {
        outer = inner;
        return true;
    }
    if ( n == 3 )
    {
        Def<Triangle2d> trian = minTriangleAroundConvexPolygonA ( inner );
        if ( ! trian.isDef ) return false;
        outer[0] = trian.a;
        outer[1] = trian.b;
        outer[2] = trian.c;
        return true;
    }
// Построим N-угольник
    SegmList list;
    nat i;
    for ( i = 0; i < m; ++i )
    {
        SegmItem * p = new SegmItem;
        p->info = i;
        Set2<Segment2d, Set2<nat> > & s = *p;
        s.b.a = i;
        s.b.b = i + 1;
        if ( s.b.b == m ) s.b.b = 0;
        s.a.a = inner[s.b.a];
        s.a.b = inner[s.b.b];
        list.addAftCur ( p );
    }
    SegmListMinArea ma ( list );
    simplifyChain ( list, ma, ma, n );
// Подвигаем стороны N-угольника
    tuning ( list, inner );
    if ( n == 4 )
    {
// Проверим второй вариант
        SegmList list2;
        if (  minTrapezoidAroundConvexPolygon ( inner, list2 ) )
        {
            tuning ( list2, inner );
            if ( area2 ( list ) > area2 ( list2 ) )
                swap ( list, list2 );
        }
    }
// Запись результата
    list.top();
    for ( i = 0; i < n; ++i )
    {
        outer[i] = list.cur()->a.a;
        list.next();
    }
    return true;
}

// Простой многоугольник

bool minNgonAroundPolygonA ( ArrRef<Vector2d> outer, CArrRef<Vector2d> inner )
{
    return minNgonAroundConvexPolygonA ( outer, convexPolygon ( inner, DynArray<Vector2d>() ) );
}

// Множество точек

bool minNgonAroundPointsA ( ArrRef<Vector2d> outer, CArrRef<Vector2d> inner )
{
    return minNgonAroundConvexPolygonA ( outer, convexNlogN ( inner, Suite<Vector2d> ( inner.size() ) ) );
}

//**************************** 29.10.2016 *********************************//
//
//            Минимальный охватывающий равноугольный многоугольник
//
//**************************** 29.10.2016 *********************************//

bool minEquianglarPolygonAroundConvexPolygonA ( ArrRef<Vector2d> outer, CArrRef<Vector2d> inner )
{
    const nat n = inner.size();
    const nat m = outer.size();
    if ( n < 3 || m < 3 ) return false;
    double min = 1e300;
    DynArray<Line2d> line ( m );
    DynArray<Vector2d> vert ( m );
    const Spin2d spin ( 360. / m, true );
    for ( nat k = 0; k < n; ++k )
    {
        Vector2d v = ( inner[k] - inner.cprev(k) ).rightPerpendicular();
        if ( ! v ) continue;
        line[0].norm = v;
        line[0].dist = - ( v * inner[k] );
        nat i, jm = 0;
        for ( i = 1; i < m; ++i )
        {
            v *= spin;
            double max = v * inner.cnext ( k, jm );
            for ( nat j = jm+1; j < n; ++j )
            {
                const double t = v * inner.cnext ( k, j );
                if ( max < t ) max = t, jm = j;
                else break;
            }
            line[i].norm = v;
            line[i].dist = - ( v * inner.cnext ( k, jm ) );
        }
        for ( i = 0; i < m; ++i ) vert[i] = intersection ( line[i], line.cnext(i) );
        const double t = area ( vert );
        if ( min > t ) min = t, outer = vert;
    }
    return min < 1e300;
}

// Простой многоугольник

bool minEquianglarPolygonAroundPolygonA ( ArrRef<Vector2d> outer, CArrRef<Vector2d> inner )
{
    return minEquianglarPolygonAroundConvexPolygonA ( outer, convexPolygon ( inner, DynArray<Vector2d>() ) );
}

// Множество точек

bool minEquianglarPolygonAroundPointsA ( ArrRef<Vector2d> outer, CArrRef<Vector2d> inner )
{
    return minEquianglarPolygonAroundConvexPolygonA ( outer, convexNlogN ( inner, Suite<Vector2d> ( inner.size() ) ) );
}