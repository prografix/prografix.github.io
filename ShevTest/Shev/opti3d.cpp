
#include "math.h"

#include "tune.h"
#include "rand.h"
#include "LinAlg.h"
#include "Mathem.h"
#include "func2d.h"
#include "func3d.h"
#include "opti3d.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"
#include "approx3d.h"
#include "Polyhedron.h"
#include "intersect3d.h"

//**************************** 28.08.2003 *********************************//
//
//              Минимальная сфера вокруг данных точек
//
//**************************** 10.11.2010 *********************************//

Def<Sphere3d> minSphereAroundPoints ( CArrRef<Vector3d> data )
{
    const nat n = data.size();
    if ( n == 0 ) return Def<Sphere3d> ();
    if ( n == 1 ) return Sphere3d ( 0, data[0] );
    if ( n == 2 )
    {
        const Vector3d o = 0.5 * ( data[0] + data[1] );
        return Sphere3d ( norm2 ( data[0] - o ), o );
    }
    const Vector3d p0 = data[0];
    nat i, im = 0;
    double max = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[i] - p0 );
        if ( max < t ) max = t, im = i;
    }
    if ( im == 0 ) return Sphere3d ( 0, p0 );
    nat np = 2, ip[4], nr, ir[3];
    ip[0] = 0;
    ip[1] = im;
    Vector3d o = 0.5 * ( p0 + data[im] );
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
        if ( np > 2 && im == ip[2] ) break;
        if ( np > 3 && im == ip[3] ) break;
        const Vector3d v1 = data[im];
        Vector3d v = 0.5 * ( v1 + data[ip[0]] );
        double s = qmod ( v1 - v );
        ir[0] = ip[0];
        nr = 2;
        for ( nat i2 = 1; i2 < np; ++i2 )
        {
            const Vector3d & v2 = data[ip[i2]];
            if ( qmod ( v2 - v ) <= s ) continue;
            v = 0.5 * ( v1 + v2 );
            s = qmod ( v1 - v );
            ir[0] = ip[i2];
            nr = 2;
            for ( nat i3 = 0; i3 < i2; ++i3 )
            {
                const Vector3d & v3 = data[ip[i3]];
                if ( qmod ( v3 - v ) <= s ) continue;
                getSphere ( v1, v2, v3, v, s );
                ir[1] = ip[i3];
                nr = 3;
                for ( nat i4 = 0; i4 < i3; ++i4 )
                {
                    const Vector3d & v4 = data[ip[i4]];
                    if ( qmod ( v4 - v ) <= s ) continue;
                    getSphere ( v1, v2, v3, v4, v, s );
                    ir[2] = ip[i4];
                    nr = 4;
                }
            }
        }
        if ( s <= q ) break;
        ip[0] = ir[0];
        ip[1] = ir[1];
        ip[2] = ir[2];
        ip[nr-1] = im;
        np = nr;
        q = s;
        o = v;
    }
    return Sphere3d ( sqrt ( q ), o );
}

//**************************** 6.11.2010 *********************************//
//
//              Минимальная сфера вокруг данных сфер
//
//**************************** 16.11.2010 *********************************//

Sphere3d minSphereAroundSpheres ( const Sphere3d & s1, const Sphere3d & s2 )
{
    Sphere3d s;
    const double d = norm2 ( s1.o - s2.o );
    s.r = 0.5 * ( s1.r + s2.r + d );
    s.o = s1.o + ( s2.o - s1.o ) * ( ( s.r - s1.r ) / d );
    return s;
}

Def<Sphere3d> minSphereAroundSpheres ( const Sphere3d & s1, const Sphere3d & s2, const Sphere3d & s3 )
{
    Def<Sphere3d> s;
    const Vector3d o21 = s2.o - s1.o;
    const Vector3d o31 = s3.o - s1.o;
    const double d22 = o21 * o21;
    const double d23 = o21 * o31;
    const double d33 = o31 * o31;
    const double dd = d22 * d33 - d23 * d23;
    if ( dd == 0 ) return s;
    const double r21 = s2.r - s1.r;
    const double r31 = s3.r - s1.r;
    const double c21 = 0.5 * ( d22 - r21 * ( s2.r + s1.r ) );
    const double c31 = 0.5 * ( d33 - r31 * ( s3.r + s1.r ) );
    const double a2 = ( r21 * d33 - r31 * d23 ) / dd;
    const double a3 = ( r31 * d22 - r21 * d23 ) / dd;
    const double b2 = ( c21 * d33 - c31 * d23 ) / dd;
    const double b3 = ( c31 * d22 - c21 * d23 ) / dd;
    const Vector3d va = a2 * o21 + a3 * o31;
    const Vector3d vb = b2 * o21 + b3 * o31;
    const double a = 1 - va * va;
    const double b = s1.r + va * vb;
    const double c = s1.r * s1.r - vb * vb;
    if ( a == 0 )
    {
        if ( b == 0 ) return s;
        s.r = -0.5 * c / b;
    }
    else
    {
        const double d = b * b - a * c;
        if ( d < 0 ) return s;
        s.r = ( b + sqrt ( d ) ) / a;
    }
    s.o = s1.o + vb + va * s.r;
    s.isDef = true;
    return s;
}

Def<Sphere3d> minSphereAroundSpheres ( const Sphere3d & s1, const Sphere3d & s2, 
                                       const Sphere3d & s3, const Sphere3d & s4 )
{
    Def<Sphere3d> s;
    const Vector3d o21 = s2.o - s1.o;
    const Vector3d o31 = s3.o - s1.o;
    const Vector3d o41 = s4.o - s1.o;
    SLU3<double> slu;
    slu.aa = o21.x; slu.ab = o21.y; slu.ac = o21.z; slu.ad = s2.r - s1.r;
    slu.ba = o31.x; slu.bb = o31.y; slu.bc = o31.z; slu.bd = s3.r - s1.r;
    slu.ca = o41.x; slu.cb = o41.y; slu.cc = o41.z; slu.cd = s4.r - s1.r;
    Vector3d va, vb;
    if ( ! slu.gauss ( va.x, va.y, va.z ) ) return s;
    const double d11 = s1.r * s1.r;
    slu.ad = 0.5 * ( o21 * o21 - s2.r * s2.r + d11 );
    slu.bd = 0.5 * ( o31 * o31 - s3.r * s3.r + d11 );
    slu.cd = 0.5 * ( o41 * o41 - s4.r * s4.r + d11 );
    if ( ! slu.gauss ( vb.x, vb.y, vb.z ) ) return s;
    const double a = 1 - va * va;
    const double b = s1.r + va * vb;
    const double c = s1.r * s1.r - vb * vb;
    if ( a == 0 )
    {
        if ( b == 0 ) return s;
        s.r = -0.5 * c / b;
    }
    else
    {
        const double d = b * b - a * c;
        if ( d < 0 ) return s;
        s.r = ( b + sqrt ( d ) ) / a;
    }
    s.o = s1.o + vb + va * s.r;
    s.isDef = true;
    return s;
}

Def<Sphere3d> minSphereAroundSpheres ( CArrRef<Sphere3d> data )
{
    const nat n = data.size();
    if ( n == 0 ) return Def<Sphere3d> ();
    if ( n == 1 ) return data[0];
    nat i, im = 0, np = 2, ip[4], nr, ir[3];
    double max = data[0].r;
    for ( i = 1; i < n; ++i )
    {
        const double t = data[i].r;
        if ( max < t ) max = t, im = i;
    }
    ip[0] = im;
    Sphere3d res = data[im];
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
        for ( i = 0; i < n; ++i )
        {
            const double t = data[i].r + norm2 ( data[i].o - res.o );
            if ( max < t ) max = t, im = i;
        }
        if ( im == ip[0] || im == ip[1] ) break;
        if ( np > 2 && im == ip[2] ) break;
        if ( np > 3 && im == ip[3] ) break;
        double d = 0.5 * ( max - res.r );
        if ( d <= 0 ) break;
        const Sphere3d & v1 = data[im];
        Sphere3d s = minSphereAroundSpheres ( v1, data[ip[0]] );
        ir[0] = ip[0];
        nr = 2;
        for ( nat i2 = 1; i2 < np; ++i2 )
        {
            const Sphere3d & v2 = data[ip[i2]];
            if ( v2.r + norm2 ( v2.o - s.o ) <= s.r ) continue;
            s = minSphereAroundSpheres ( v1, v2 );
            ir[0] = ip[i2];
            nr = 2;
            for ( nat i3 = 0; i3 < i2; ++i3 )
            {
                const Sphere3d & v3 = data[ip[i3]];
                if ( v3.r + norm2 ( v3.o - s.o ) <= s.r ) continue;
                s = minSphereAroundSpheres ( v1, v2, v3 );
                ir[1] = ip[i3];
                nr = 3;
                for ( nat i4 = 0; i4 < i3; ++i4 )
                {
                    const Sphere3d & v4 = data[ip[i4]];
                    if ( v4.r + norm2 ( v4.o - s.o ) <= s.r ) continue;
                    s = minSphereAroundSpheres ( v1, v2, v3, v4 );
                    ir[2] = ip[i4];
                    nr = 4;
                }
            }
        }
        if ( s.r <= res.r ) break;
        ip[0] = ir[0];
        ip[1] = ir[1];
        ip[2] = ir[2];
        ip[nr-1] = im;
        np = nr;
        res = s;
    }
    return res;
}

//**************************** 17.04.2010 *********************************//
//
//      Минимальная сфера пересекающая все заданные плоскости
//
//**************************** 17.04.2010 *********************************//

inline
Vector4d perpedicular ( const Vector3d & a, const Vector3d & b, const Vector3d & c )
{
    const Vector3d v = ( a - c ) % ( b - c );
    const double d = c * v;
    const double f = 1 / sqrt ( v * v + d * d );
    Vector4d p;
    if ( d > 0 )
    {
        p.x1 = - v.x * f;
        p.x2 = - v.y * f;
        p.x3 = - v.z * f;
        p.x4 = d * f;
    }
    else
    {
        p.x1 = v.x * f;
        p.x2 = v.y * f;
        p.x3 = v.z * f;
        p.x4 = - d * f;
    }
    return p;
}

Def<Sphere3d> minSphere ( CArrRef<Plane3d> plane )
{
    Def<Sphere3d> res;
    const nat nn = plane.size();
    if ( nn < 3 ) return res;
    if ( nn == 3 )
    {
        return Sphere3d ( 0, intersection ( plane[0], plane[1], plane[2] ) );
    }
    const nat n1 = nn - 1;
    const nat n2 = n1 - 1;
    nat i1 = 0, i2 = 1;
    Plane3d plane1 = plane[i1];
    while ( norm1 ( plane1.norm % plane[i2].norm ) < 1e-8 )
    {
        if ( ++i2 == n2 ) return res;
    }
    Plane3d plane2 = plane[i2];
    const Vector3d v12 = plane1.norm % plane2.norm;
    nat i3 = i2 + 1;
    while ( fabs ( v12 * plane[i3].norm ) < 1e-12 )
    {
        if ( ++i3 == n1 ) return res;
    }
    Plane3d plane3 = plane[i3];
    const Vector3d v13 = plane1.norm % plane3.norm;
    const Vector3d v23 = plane2.norm % plane3.norm;
    nat i4 = i3 + 1;
    while ( fabs ( v12 * plane[i4].norm ) < 1e-12 ||
            fabs ( v13 * plane[i4].norm ) < 1e-12 ||
            fabs ( v23 * plane[i4].norm ) < 1e-12 )
    {
        if ( ++i4 == nn ) return res;
    }
    Plane3d plane4 = plane[i4];
    if ( plane1 % intersection ( plane2, plane3, plane4 ) > 0 ) plane1 = - plane1;
    if ( plane2 % intersection ( plane1, plane3, plane4 ) > 0 ) plane2 = - plane2;
    if ( plane3 % intersection ( plane1, plane2, plane4 ) > 0 ) plane3 = - plane3;
    if ( plane4 % intersection ( plane1, plane2, plane3 ) > 0 ) plane4 = - plane4;
    Def<Sphere3d> s = spherePPPP ( plane1, plane2, plane3, plane4 );
    if ( ! s.isDef ) return s;
    Vector4d arr[5];
    arr[0] = Vector4d ( s.o.x, s.o.y, s.o.z, s.r );
    arr[1] = perpedicular ( plane2.norm, plane3.norm, plane4.norm );
    arr[2] = perpedicular ( plane1.norm, plane3.norm, plane4.norm );
    arr[3] = perpedicular ( plane1.norm, plane2.norm, plane4.norm );
    arr[4] = perpedicular ( plane1.norm, plane2.norm, plane3.norm );
    nat i, j;
    for ( j = 0; j <= nn; ++j )
    {
        double max = 0.;
        nat im;
        for ( i = 0; i < nn; ++i )
        {
            const double t = fabs ( plane[i] % s.o );
            if ( max < t ) max = t, im = i;
        }
        const double dist = max - s.r;
        if ( dist <= 1e-12 * _max ( 1., s.r ) )
        {
            return s;
        }
        Plane3d pm = plane[im];
        if ( pm % s.o > 0 ) pm = - pm;
        const Vector4d cor ( pm.norm.x, pm.norm.y, pm.norm.z, 1. );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i < 5; ++i )
        {
            const Vector4d & v = arr[i];
            double t = cor * v;
            if ( t < 1e-12 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.x4 * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double u = v.x4 * t;
                if ( u < max ) max = u, ib = i, sm = t;
            }
        }
        if ( ib == 0 ) 
            return res;
        const Vector4d & v = arr[ib];
        Vector4d & a0 = arr[0];
        a0 += v * ( dist * sm );
        s.o.x = a0.x1;
        s.o.y = a0.x2;
        s.o.z = a0.x3;
        s.r = a0.x4;
        for ( i = 1; i < 5; ++i )
        {
            if ( i == ib ) continue;
            Vector4d & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return res;
}

//**************************** 07.03.2012 *********************************//
//
//      Минимальная сфера пересекающая все заданные прямые
//
//**************************** 07.03.2012 *********************************//

void minSphere ( const Line3d & line1, const Line3d & line2, Vector3d & o, double & q )
{
    const double s = line1.dir * line2.dir;
    const double d = 1 - s * s;
    double t1, t2;
    if ( d < 1e-12 )
    {
        t1 = line1.dir * line1.point;
        t2 = line2.dir * line2.point;
    }
    else
    {
        const double s1 = line1.dir * ( line1.point - line2.point );
        const double s2 = line2.dir * ( line2.point - line1.point );
        t1 = ( s1 + s2 * s ) / d;
        t2 = ( s2 + s1 * s ) / d;
    }
    const Vector3d p1 = line1.point - line1.dir * t1;
    const Vector3d p2 = line2.point - line2.dir * t2;
    q = 0.25 * qmod ( p2 - p1 );
    o = 0.5 * ( p2 + p1 );
}

void minSphere ( const Line3d & line1, const Line3d & line2, 
                 const Line3d & line3, Vector3d & o, double & q )
{
    if ( norm1 ( line1.dir % line2.dir ) < 1e-9 && norm1 ( line1.dir % line3.dir ) < 1e-9 )
    {
        o.fill();
        const Vector3d p1 = line1.project ( o );
        const Vector3d p2 = line2.project ( o );
        const Vector3d p3 = line3.project ( o );
        getSphere ( p1, p2, p3, o, q );
        return;
    }
    FixArray<Line3d, 3> line;
    line[0] = line1;
    line[1] = line2;
    line[2] = line3;
    o = getNearPoint2 ( line );
    q = 1e300;
    for ( nat i = 0; i < 25; ++i )
    {
        double q2;
        Vector3d o2;
        const Vector3d p1 = line1.project ( o );
        const Vector3d p2 = line2.project ( o );
        const Vector3d p3 = line3.project ( o );
        const double t1 = qmod ( p1 - p2 );
        const double t2 = qmod ( p2 - p3 );
        const double t3 = qmod ( p3 - p1 );
        if ( t1 >= t2 + t3 )
        {
            o2 = 0.5 * ( p1 + p2 );
            q2 = 0.25 * t1;
        }
        else
        if ( t2 >= t1 + t3 )
        {
            o2 = 0.5 * ( p3 + p2 );
            q2 = 0.25 * t2;
        }
        else
        if ( t3 >= t2 + t1 )
        {
            o2 = 0.5 * ( p1 + p3 );
            q2 = 0.25 * t3;
        }
        else
        {
            getSphere ( p1, p2, p3, o2, q2 );
        }
        if ( q2 >= q ) break;
        q = q2;
        o = o2;
    }
}

void minSphere ( const Line3d & line1, const Line3d & line2, const Line3d & line3, 
                 const Line3d & line4, Vector3d & o, double & q )
{
    FixArray<Line3d, 4> line;
    line[0] = line1;
    line[1] = line2;
    line[2] = line3;
    line[3] = line4;
    o = getNearPoint2 ( line );
    q = 1e300;
    for ( nat i = 0; i < 25; ++i )
    {
        double q2;
        Vector3d o2;
        const Vector3d p1 = line1.project ( o );
        const Vector3d p2 = line2.project ( o );
        const Vector3d p3 = line3.project ( o );
        const Vector3d p4 = line4.project ( o );
        getSphere ( p1, p2, p3, p4, o2, q2 );
        if ( q2 >= q ) break;
        q = q2;
        o = o2;
    }
}

Def<Sphere3d> minSphere ( CArrRef<Line3d> data )
{
    Def<Sphere3d> res;
    const nat n = data.size();
    if ( n <= 1 ) return res;
    Vector3d o;
    double q;
    minSphere ( data[0], data[1], o, q );
    if ( n == 2 ) return res;
    nat np = 2, ip[4], nr, ir[3];
    ip[0] = 0;
    ip[1] = 1;
    double s = q;
    for (;;)
    {
        nat im = 0;
        double max = 0;
        for ( nat i = 0; i < n; ++i )
        {
            const double t = data[i].qdis ( o );
            if ( max < t ) max = t, im = i;
        }
        if ( max <= q || im == ip[0] || im == ip[1] ) break;
        if ( np > 2 && im == ip[2] ) break;
        if ( np > 3 && im == ip[3] ) break;
        const Line3d & line1 = data[im];
        minSphere ( data[ip[0]], line1, o, q );
        ir[0] = ip[0];
        nr = 2;
        for ( nat i2 = 1; i2 < np; ++i2 )
        {
            const Line3d & line2 = data[ip[i2]];
            const double t2 = line2.qdis ( o );
            if ( t2 <= q ) continue;
            minSphere ( line1, line2, o, q );
            ir[0] = ip[i2];
            nr = 2;
            for ( nat i3 = 0; i3 < i2; ++i3 )
            {
                const Line3d & line3 = data[ip[i3]];
                const double t3 = line3.qdis ( o );
                if ( t3 <= q ) continue;
                minSphere ( line1, line2, line3, o, q );
                ir[1] = ip[i3];
                nr = 3;
                for ( nat i4 = 0; i4 < i3; ++i4 )
                {
                    const Line3d & line4 = data[ip[i4]];
                    const double t4 = line4.qdis ( o );
                    if ( t4 <= q ) continue;
                    minSphere ( line1, line2, line3, line4, o, q );
                    ir[2] = ip[i4];
                    nr = 4;
                }
            }
        }
        ip[0] = ir[0];
        ip[1] = ir[1];
        ip[2] = ir[2];
        ip[nr-1] = im;
        np = nr;
        s = q;
    }
    return Sphere3d ( sqrt ( q ), o );
}

//**************************** 10.10.2009 *********************************//
//
//          Минимальный по объёму охватывающий эллипсоид
//
//**************************** 09.03.2013 *********************************//

Def<Affin3d> minEllipsoid4 ( CArrRef<nat> ip, CArrRef<Vector3d> data )
{
    const Vector3d & v1 = data[ip[0]];
    const Vector3d & v2 = data[ip[1]];
    const Vector3d & v3 = data[ip[2]];
    const Vector3d & v4 = data[ip[3]];
    Affin3d aff;
    aff.t.x.x = 0.5 * ( v2.x - v1.x );
    aff.t.x.y = 0.5 * ( v4.x - v3.x );
    aff.t.x.z = 0.25 * ( v4.x + v3.x - v2.x - v1.x ) * M_SQRT2;
    aff.t.y.x = 0.5 * ( v2.y - v1.y );
    aff.t.y.y = 0.5 * ( v4.y - v3.y );
    aff.t.y.z = 0.25 * ( v4.y + v3.y - v2.y - v1.y ) * M_SQRT2;
    aff.t.z.x = 0.5 * ( v2.z - v1.z );
    aff.t.z.y = 0.5 * ( v4.z - v3.z );
    aff.t.z.z = 0.25 * ( v4.z + v3.z - v2.z - v1.z ) * M_SQRT2;
    aff.s.x = 0.25 * ( v1.x + v2.x + v3.x + v4.x );
    aff.s.y = 0.25 * ( v1.y + v2.y + v3.y + v4.y );
    aff.s.z = 0.25 * ( v1.z + v2.z + v3.z + v4.z );
    return ~aff;
}

Def<Affin3d> minEllipsoid9 ( CArrRef<nat> ip, CArrRef<Vector3d> data )
{
    Def<Affin3d> res;
    const nat n = ip.size();
    CmbArray<nat, 9> ir ( ip );
// Находим первую начальную точку
    nat i, im;
    double max = 0;
    const Vector3d & pp = data[ir[0]];
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[ir[i]] - pp );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) return res;
    _swap ( ir[0], ir[im] );
    const Vector3d & v1 = data[ir[0]];
// Находим вторую начальную точку
    max = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[ir[i]] - v1 );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) return res;
    if ( im != 1 ) _swap ( ir[1], ir[im] );
    const Vector3d & v2 = data[ir[1]];
// Находим третью начальную точку
    const double eps = 1e-18 * max;
    Vector3d v = v2 - v1;
    max = 0;
    for ( i = 2; i < n; ++i )
    {
        const double t = qmod ( ( data[ir[i]] - v1 ) % v );
        if ( max < t ) max = t, im = i;
    }
    if ( max < eps ) return res;
    if ( im != 2 ) _swap ( ir[2], ir[im] );
    const Vector3d & v3 = data[ir[2]];
// Находим четвёртую начальную точку
    Vector3d norm = v % ( v3 - v1 );
    if ( ! norm ) return res;
    norm.setNorm2();
    const Plane3d plane ( norm, - ( norm * v1 ) );
    max = 0.;
    for ( i = 3; i < n; ++i )
    {
        const double t = fabs ( plane % data[ir[i]] );
        if ( max < t ) max = t, im = i;
    }
    if ( max * max < eps ) return res;
    if ( im != 3 ) _swap ( ir[3], ir[im] );
    const Vector3d & v4 = data[ir[3]];
// Находим преобразование для первых четырёх точек
    Affin3d aff;
    aff.t.x.x = 0.25 * ( v1.x - v2.x + v3.x - v4.x );
    aff.t.x.y = 0.25 * ( v1.x - v2.x - v3.x + v4.x );
    aff.t.x.z = 0.25 * ( v1.x + v2.x - v3.x - v4.x );
    aff.t.y.x = 0.25 * ( v1.y - v2.y + v3.y - v4.y );
    aff.t.y.y = 0.25 * ( v1.y - v2.y - v3.y + v4.y );
    aff.t.y.z = 0.25 * ( v1.y + v2.y - v3.y - v4.y );
    aff.t.z.x = 0.25 * ( v1.z - v2.z + v3.z - v4.z );
    aff.t.z.y = 0.25 * ( v1.z - v2.z - v3.z + v4.z );
    aff.t.z.z = 0.25 * ( v1.z + v2.z - v3.z - v4.z );
    aff.s.x = 0.25 * ( v1.x + v2.x + v3.x + v4.x );
    aff.s.y = 0.25 * ( v1.y + v2.y + v3.y + v4.y );
    aff.s.z = 0.25 * ( v1.z + v2.z + v3.z + v4.z );
    res = ~aff;
// Вычисляем ненормированные параметры
    FixArray2<double, 6, 7> mat;
    for ( i = 0; i < 5; ++i )
    {
        const Vector3d v = res ( data[ir[i+4]] );
        ArrRef<double> r = mat[i];
        r[0] = v.x * v.x;
        r[1] = v.y * v.y;
        r[2] = v.z * v.z;
        r[3] = 2*( v.x * v.y - v.z );
        r[4] = 2*( v.y * v.z - v.x );
        r[5] = 2*( v.z * v.x - v.y );
        r[6] = 1;
    }
    {
        ArrRef<double> r = mat[5];
        r[0] = r[1] = r[2] = r[6] = 1;
        r[3] = r[4] = r[5] = 0;
    }
    if ( ! slu_gauss ( mat ) ) return Def<Affin3d>();
// Нормировка
    double A = mat[0][6];
    double B = mat[1][6];
    double C = mat[2][6];
    double D = mat[3][6];
    double E = mat[4][6];
    double F = mat[5][6];
    double M2 = A * B - D * D;
    double M3 = C * M2 + E * ( D * F - A * E ) + F * ( D * E - B * F );
    double coef = 3 / ( A + B + C + ( E * E + _pow2 ( D * E - A * F ) / M2 ) / A + 
        _pow2 ( ( F*E - A*D ) * M2 + ( A*E - D*F ) * ( A*F - D*E ) ) / ( A*A * M2*M3 ) );
    A *= coef;
    B *= coef;
    C *= coef;
    D *= coef;
    E *= coef;
    F *= coef;
    M2 *= coef * coef;
    M3 *= coef * coef * coef;
    const double m1 = sqrt ( A );
    const double m2 = sqrt ( M2 );
    const double m3 = sqrt ( M3 );
    const double w1 = A * E - D * F;
    const double w2 = D * E - A * F;
    aff.t.x = Vector3d ( A, D, F ) / m1; 
    aff.t.y = Vector3d ( 0, m2/m1, w1 / ( m1*m2 ) );
    aff.t.z = Vector3d ( 0, 0, m3 / m2 );
    aff.s = Vector3d ( -E/m1, w2 / (m1*m2), ( ( F*E - A*D)*M2 - w1 * w2 )/(A*m2*m3) );
    res = aff * res;
    res.t.x /= M_SQRT2;
    res.t.y /= M_SQRT2;
    res.t.z /= M_SQRT2;
    res.s   /= M_SQRT2;
    return res;
}

Def<Affin3d> minEllipsoid5_8 ( CArrRef<nat> ip, CArrRef<Vector3d> data )
{
    Def<Affin3d> res;
    const nat n = ip.size();
    CmbArray<nat, 9> ir ( ip );
// Находим первую начальную точку
    nat i, im;
    double max = 0;
    const Vector3d & pp = data[ir[0]];
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[ir[i]] - pp );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) return res;
    _swap ( ir[0], ir[im] );
// Находим вторую начальную точку
    const Vector3d & p0 = data[ir[0]];
    max = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[ir[i]] - p0 );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) return res;
    if ( im != 1 ) _swap ( ir[1], ir[im] );
// Находим третью начальную точку
    const double eps = 1e-18 * max;
    Vector3d v10 = data[ir[1]] - p0;
    max = 0;
    for ( i = 2; i < n; ++i )
    {
        const double t = qmod ( ( data[ir[i]] - p0 ) % v10 );
        if ( max < t ) max = t, im = i;
    }
    if ( max < eps ) return res;
    if ( im != 2 ) _swap ( ir[2], ir[im] );
// Находим четвёртую начальную точку
    Vector3d norm = v10 % ( data[ir[2]] - p0 );
    if ( ! norm ) return res;
    norm.setNorm2();
    const Plane3d plane ( norm, - ( norm * p0 ) );
    max = 0.;
    for ( i = 3; i < n; ++i )
    {
        const double t = fabs ( plane % data[ir[i]] );
        if ( max < t ) max = t, im = i;
    }
    if ( max * max < eps ) return res;
    if ( im != 3 ) _swap ( ir[3], ir[im] );
    const Vector3d & v1 = data[ir[0]];
    const Vector3d & v2 = data[ir[1]];
    const Vector3d & v3 = data[ir[2]];
    const Vector3d & v4 = data[ir[3]];
    Affin3d aff;
    aff.t.x.x = 0.5 * ( v2.x - v1.x );
    aff.t.x.y = 0.5 * ( v4.x - v3.x );
    aff.t.x.z = 0.25 * ( v4.x + v3.x - v2.x - v1.x ) * M_SQRT2;
    aff.t.y.x = 0.5 * ( v2.y - v1.y );
    aff.t.y.y = 0.5 * ( v4.y - v3.y );
    aff.t.y.z = 0.25 * ( v4.y + v3.y - v2.y - v1.y ) * M_SQRT2;
    aff.t.z.x = 0.5 * ( v2.z - v1.z );
    aff.t.z.y = 0.5 * ( v4.z - v3.z );
    aff.t.z.z = 0.25 * ( v4.z + v3.z - v2.z - v1.z ) * M_SQRT2;
    aff.s.x = 0.25 * ( v1.x + v2.x + v3.x + v4.x );
    aff.s.y = 0.25 * ( v1.y + v2.y + v3.y + v4.y );
    aff.s.z = 0.25 * ( v1.z + v2.z + v3.z + v4.z );
    res = ~aff;
    res.s.z += 1/M_SQRT2;
    Vector3d vert[7];
    for ( i = 1; i < n; ++i ) vert[i-1] = res ( data[ir[i]] );
    const nat m = n - 1;
    const nat m8 = m + 8;
    CmbMatrix<double, 8, 7> A ( 8, m );
    CmbMatrix<double, 15, 15> M ( m8, m8 );
    FixArray<double, 8> r;
    CmbArray<double, 30> buf ( m8 + m8 );
    ArrRef<double> c1 ( buf, 0, m8 );
    ArrRef<double> c2 ( buf, m8, m8 );
    ArrRef<double> la ( c2, 8, m );
    double & a = c2[0];
    double & d = c2[1];
    double & f = c2[2];
    double & b = c2[3];
    double & e = c2[4];
    double & g = c2[5];
    double & c = c2[6];
    double & h = c2[7];
    a = b = c = 1;
    d = e = f = g = 0;
    h = -1/M_SQRT2;
// Находим приближённое значение лямбда-множителей
    r[1] = r[2] = r[4] = r[5] = r[7] = 0;
    r[0] = - b * c;
    r[3] = - a * c;
    r[6] = - a * b;
    for ( i = 0; i < m; ++i )
    {
        const Vector3d & vi = vert[i];
        const double u = vi.x * a + vi.y * d + vi.z * f;
        const double v = vi.y * b + vi.z * e + g;
        const double w = vi.z * c + h;
        A[0][i] = u * vi.x;
        A[1][i] = u * vi.y;
        A[2][i] = u * vi.z;
        A[3][i] = v * vi.y;
        A[4][i] = v * vi.z;
        A[5][i] = v;
        A[6][i] = w * vi.z;
        A[7][i] = w;
    }
    lss_h ( A, r, la );
// Вычисляем параметры преобразования
    for ( nat k = 0; k < 9; ++k )
    {
        double da = b * c;
        double dd = 0;
        double df = 0;
        double db = a * c;
        double de = 0;
        double dg = 0;
        double dc = a * b;
        double dh = 0;
        double func1 = 0;
        for ( i = 0; i < m; ++i )
        {
            const Vector3d & vi = vert[i];
            const double u = vi.x * a + vi.y * d + vi.z * f;
            const double v = vi.y * b + vi.z * e + g;
            const double w = vi.z * c + h;
            da += la[i] * u * vi.x;
            dd += la[i] * u * vi.y;
            df += la[i] * u * vi.z;
            db += la[i] * v * vi.y;
            de += la[i] * v * vi.z;
            dg += la[i] * v;
            dc += la[i] * w * vi.z;
            dh += la[i] * w;
            func1 += _pow2 ( 0.5 * ( u * u + v * v + w * w - 1.5 ) );
        }
        double func2 = _pow2 ( da ) + _pow2 ( dd ) + _pow2 ( df ) + _pow2 ( db ) +
                       _pow2 ( de ) + _pow2 ( dg ) + _pow2 ( dc ) + _pow2 ( dh );
        if ( func1 < 1e-24 && func2 < 1e-18 ) break;
        c1[0] = b * c;
        c1[3] = a * c;
        c1[6] = a * b;
        c1[1] = c1[2] = c1[4] = c1[5] = c1[7] = 0;
        M.fill(0);
        M[3][0] = c;
        M[6][0] = b;
        M[6][3] = a;
        for ( i = 0; i < m; ++i )
        {
            const nat j = i + 8;
            const Vector3d & vi = vert[i];
            const double u = vi.x * a + vi.y * d + vi.z * f;
            const double v = vi.y * b + vi.z * e + g;
            const double w = vi.z * c + h;
            c1[0] += la[i] * u * vi.x;
            c1[1] += la[i] * u * vi.y;
            c1[2] += la[i] * u * vi.z;
            c1[3] += la[i] * v * vi.y;
            c1[4] += la[i] * v * vi.z;
            c1[5] += la[i] * v;
            c1[6] += la[i] * w * vi.z;
            c1[7] += la[i] * w;
            c1[j] = 0.75 + 0.5 * ( u * u + v * v + w * w );
            M[0][0] += la[i] * vi.x * vi.x;
            M[1][0] += la[i] * vi.x * vi.y;
            M[1][1] += la[i] * vi.y * vi.y;
            M[2][0] += la[i] * vi.x * vi.z;
            M[2][1] += la[i] * vi.y * vi.z;
            M[2][2] += la[i] * vi.z * vi.z;
            M[3][3] += la[i] * vi.y * vi.y;
            M[4][3] += la[i] * vi.y * vi.z;
            M[4][4] += la[i] * vi.z * vi.z;
            M[5][3] += la[i] * vi.y;
            M[5][4] += la[i] * vi.z;
            M[5][5] += la[i];
            M[6][6] += la[i] * vi.z * vi.z;
            M[7][6] += la[i] * vi.z;
            M[7][7] += la[i];
            M[j][0] = M[0][j] = u * vi.x;
            M[j][1] = M[1][j] = u * vi.y;
            M[j][2] = M[2][j] = u * vi.z;
            M[j][3] = M[3][j] = v * vi.y;
            M[j][4] = M[4][j] = v * vi.z;
            M[j][5] = M[5][j] = v;
            M[j][6] = M[6][j] = w * vi.z;
            M[j][7] = M[7][j] = w;
        }
        for ( i = 0; i < 7; ++i )
            for ( nat j = i+1; j < 8; ++j ) M[i][j] = M[j][i];
        const SLU_Gauss slu ( m8, M );
        if ( ! slu.solve ( c1(), c2() ) ) return Def<Affin3d>();
    }
    return Affin3d ( Vector3d ( a, d, f ), Vector3d ( 0, b, e ),
                     Vector3d ( 0, 0, c ), Vector3d ( 0, g, h ) ) * res;
}

Def<Ellipsoid3d> minEllipsoidV ( CArrRef<Vector3d> data )
{
    Def<Ellipsoid3d> res;
    const nat n = data.size();
    if ( n < 4 ) return res;
// Находим первую начальную точку
    nat i, im;
    double max = 0;
    for ( i = 1; i < n; ++i )
    {
        const double t = qmod ( data[i] - data[0] );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) return res;
    CmbSuite<nat, 9> ir, ip;
    ip.inc() = im;
// Находим вторую начальную точку
    const Vector3d & p0 = data[im];
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( data[i] - p0 );
        if ( max < t ) max = t, im = i;
    }
    if ( max == 0 ) return res;
    ip.inc() = im;
// Находим третью начальную точку
    const double eps = 1e-18 * max;
    Vector3d v = data[ip[1]] - p0;
    max = 0;
    for ( i = 0; i < n; ++i )
    {
        const double t = qmod ( ( data[i] - p0 ) % v );
        if ( max < t ) max = t, im = i;
    }
    if ( max < eps ) return res;
    ip.inc() = im;
// Находим четвёртую начальную точку
    Vector3d norm = v % ( data[ip[2]] - p0 );
    if ( ! norm ) return res;
    norm.setNorm2();
    const Plane3d plane ( norm, - ( norm * p0 ) );
    max = 0.;
    for ( i = 0; i < n; ++i )
    {
        const double t = fabs ( plane % data[i] );
        if ( max < t ) max = t, im = i;
    }
    if ( max * max < eps ) return res;
    ip.inc() = im;
    if ( plane % data[im] > 0 ) _swap ( ip[2], ip[3] );
    Def<Affin3d> aff = minEllipsoid4 ( ip, data );
    if ( ! aff.isDef ) 
        return res;
    double q = ( aff.t.x % aff.t.y ) * aff.t.z;
    const double r2 = 1.5 + 1e-12;
// Проверка остальных точек
    for (;;)
    {
        max = 0;
        for ( i = 0; i < n; ++i )
        {
            const double t = qmod ( aff ( data[i] ) );
            if ( max < t ) max = t, im = i;
        }
        if ( max < r2 ) break;
        for ( i = 0; i < ip.size(); ++i )
            if ( im == ip[i] ) goto m1;
        Def<Affin3d> temp;
        const Vector3d v1 = data[im];
        ir.resize ( 1 );
        ir[0] = im;
        for ( nat i2 = 2; i2 < ip.size(); ++i2 )
        {
            const Vector3d & v2 = data[ip[i2]];
            if ( temp.isDef && qmod ( temp ( v2 ) ) < r2 ) continue;
            temp.isDef = false;
            ir.resize(2)[1] = ip[i2];
            for ( nat i3 = 1; i3 < i2; ++i3 )
            {
                const Vector3d & v3 = data[ip[i3]];
                if ( temp.isDef && qmod ( temp ( v3 ) ) < r2 ) continue;
                temp.isDef = false;
                ir.resize(3)[2] = ip[i3];
                for ( nat i4 = 0; i4 < i3; ++i4 )
                {
                    const Vector3d & v4 = data[ip[i4]];
                    if ( temp.isDef && qmod ( temp ( v4 ) ) < r2 ) continue;
                    temp.isDef = false;
                    ir.resize(4)[3] = ip[i4];
                    temp = minEllipsoid4 ( ir, data );
                    for ( nat i5 = 0; i5 < i4; ++i5 )
                    {
                        const Vector3d & v5 = data[ip[i5]];
                        if ( temp.isDef && qmod ( temp ( v5 ) ) < r2 ) continue;
                        ir.resize(5)[4] = ip[i5];
                        temp = minEllipsoid5_8 ( ir, data );
                        for ( nat i6 = 0; i6 < i5; ++i6 )
                        {
                            const Vector3d & v6 = data[ip[i6]];
                            if ( temp.isDef && qmod ( temp ( v6 ) ) < r2 ) continue;
                            ir.resize(6)[5] = ip[i6];
                            temp = minEllipsoid5_8 ( ir, data );
                            for ( nat i7 = 0; i7 < i6; ++i7 )
                            {
                                const Vector3d & v7 = data[ip[i7]];
                                if ( temp.isDef && qmod ( temp ( v7 ) ) < r2 ) continue;
                                ir.resize(7)[6] = ip[i7];
                                temp = minEllipsoid5_8 ( ir, data );
                                for ( nat i8 = 0; i8 < i7; ++i8 )
                                {
                                    const Vector3d & v8 = data[ip[i8]];
                                    if ( temp.isDef && qmod ( temp ( v8 ) ) < r2 ) continue;
                                    ir.resize(8)[7] = ip[i8];
                                    temp = minEllipsoid5_8 ( ir, data );
                                    for ( nat i9 = 0; i9 < i8; ++i9 )
                                    {
                                        const Vector3d & v9 = data[ip[i9]];
                                        if ( temp.isDef && qmod ( temp ( v9 ) ) < r2 ) continue;
                                        ir.resize(9)[8] = ip[i9];
                                        temp = minEllipsoid9 ( ir, data );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        double s = fabs ( ( temp.t.x % temp.t.y ) * temp.t.z );
        if ( s >= q )
            break;
        aff = temp;
        ip = ir;
        q = s;
    }
m1: return (~aff) ( Sphere3d ( sqrt(1.5), null3d ) );
}

//**************************** 02.05.2013 *********************************//
//
//      Цилиндр с минимальным радиусом охватывающий данные точки
//
//**************************** 14.01.2018 *********************************//

Def<Cylinder3d> minCylinderR ( CArrRef<Vector3d> data )
{
    Def<Cylinder3d> res;
    if ( ! data.size() ) return res;
    const Line3d line = getLineU ( data, res.r );
    double max = line.dir * data[0];
    double min = max;
    for ( nat i = 1; i < data.size(); ++i )
    {
        const double t = line.dir * data[i];
        if ( max < t ) max = t; else
        if ( min > t ) min = t;
    }
    res.o = 0.5 * ( max + min ) * line.dir + line.project ( null3d );
    res.spin = Spin3d ( Vector3d ( 0,0,1 ), line.dir );
    res.h = 0.5 * ( max - min );
    res.isDef = true;
    return res;
}

//**************************** 09.05.2016 *********************************//
//
//  Минимальный правильный тетраэдр охватывающий данные точки без вращения
//
//**************************** 14.09.2019 *********************************//

bool minRegularTetrahedronAroundPointsNR ( CCArrRef<Vector3d> & data, const Ortho3d & ortho, Polyhedron & poly )
{
    if ( ! data.size() ) return false;
    poly.makeTetrahedron(1);
    for ( nat i = 0; i < 4; ++i )
    {
        Vector3d & v = poly.facet[i].plane.norm;
        v *= ortho;
        double d = v * data[0];
        for ( nat j = 1; j < data.size(); ++j )
        {
            const double t = v * data[j];
            if ( d < t ) d = t;
        }
        poly.facet[i].plane.dist = -d;
    }
    const Plane3d & p0 = poly.facet[0].plane;
    const Plane3d & p1 = poly.facet[1].plane;
    const Plane3d & p2 = poly.facet[2].plane;
    const Plane3d & p3 = poly.facet[3].plane;
    SLU3<double> slu;
    slu.aa = p1.norm.x; slu.ab = p1.norm.y; slu.ac = p1.norm.z; slu.ad = - p1.dist;
    slu.ba = p2.norm.x; slu.bb = p2.norm.y; slu.bc = p2.norm.z; slu.bd = - p2.dist;
    slu.ca = p3.norm.x; slu.cb = p3.norm.y; slu.cc = p3.norm.z; slu.cd = - p3.dist;
    slu.gauss ( poly.vertex[0].x, poly.vertex[0].y, poly.vertex[0].z );
    slu.aa = p0.norm.x; slu.ab = p0.norm.y; slu.ac = p0.norm.z; slu.ad = - p0.dist;
    slu.gauss ( poly.vertex[1].x, poly.vertex[1].y, poly.vertex[1].z );
    slu.ba = p1.norm.x; slu.bb = p1.norm.y; slu.bc = p1.norm.z; slu.bd = - p1.dist;
    slu.gauss ( poly.vertex[2].x, poly.vertex[2].y, poly.vertex[2].z );
    slu.ca = p2.norm.x; slu.cb = p2.norm.y; slu.cc = p2.norm.z; slu.cd = - p2.dist;
    slu.gauss ( poly.vertex[3].x, poly.vertex[3].y, poly.vertex[3].z );
    return true;
}

//**************************** 14.09.2019 *********************************//
//
//      Минимальный правильный тетраэдр охватывающий данные точки
//
//**************************** 14.09.2019 *********************************//

bool minRegularTetrahedronAroundPoints ( CCArrRef<Vector3d> & data, Polyhedron & poly )
{
    if ( ! data.size() ) return false;
    // Глобальный поиск оптимального поворота
    QRand q5(5);
    double min = 1e300, b = 0;
    const nat n = 2862;
    Polyhedron temp;
    Spin3d best;
    nat k;
    for ( k = 0; k < n; ++k )
    {
        const double a = ( 0.5 + k ) / n;
        if ( ( b += 0.61803398874989484820 ) > 1 ) b -= 1;
        const double c = q5();
        const Spin3d spin = getRandSpin3d ( a, b, c );
        minRegularTetrahedronAroundPointsNR ( data, Ortho3d ( spin ), temp );
        const double t = qmod ( temp.vertex[1] - temp.vertex[0] );
        if ( min > t )
        {
            _swap ( temp, poly );
            best = spin;
            min = t;
        }
    }
    // Локальный поиск оптимального поворота
    QRand q2(2);
    QRand2Vector3d qv;
    const double step = 4 * M_PI / 180;
    for ( k = 0; k < 2000; ++k )
    {
        const double c = q2();
        const Spin3d spin ( Spin3d ( qv(), ( c + 0.25 ) * step ) * best );
        minRegularTetrahedronAroundPointsNR ( data, Ortho3d ( spin ), temp );
        const double t = qmod ( temp.vertex[1] - temp.vertex[0] );
        if ( min > t )
        {
            _swap ( temp, poly );
            best = spin;
            min = t;
        }
    }
    return true;
}

//**************************** 28.05.2016 *********************************//
//
//    Минимальный параллелепипед охватывающий данные точки без вращения
//
//**************************** 28.05.2016 *********************************//

Def<Cuboid3d> minCuboidAroundPointsNR ( CArrRef<Vector3d> data )
{
    Def<Cuboid3d> res;
    if ( ! data.size() ) return res;
    const Segment3d diag = dimensions ( data );
    res.a = 0.5 * ( diag.b.x - diag.a.x );
    res.b = 0.5 * ( diag.b.y - diag.a.y );
    res.c = 0.5 * ( diag.b.z - diag.a.z );
    res.o = 0.5 * ( diag.b + diag.a );
    res.isDef = true;
    return res;
}

//**************************** 11.02.2010 *********************************//
//
//  Минимальный по объёму параллелепипед охватывающий выпуклый многогранник
//
//**************************** 22.11.2010 *********************************//

void minRectangleAroundConvexPolygon ( CArrRef<Vector2d> inner, const MathFunc2 & func,
                                       Vector2d & ax, double & x1, double & x2,
                                       Vector2d & ay, double & y1, double & y2 );

Def<Cuboid3d> minCuboidAroundConvexPolyhedronV ( const Polyhedron & inner )
{
    Def<Cuboid3d> res;
    const nat nv = inner.vertex.size();
    if ( inner.facet.size() < 4 || nv < 4 ) return res;
    double x1, x2, y1, y2, z1, z2;
    Vector3d ax, ay, az;
    DynArray<Vector2d> arr ( nv );
    Suite<Vector2d> suite;
    double minV = 1e300;
    for ( nat i = 0; i < inner.facet.size(); ++i )
    {
        Vector3d x, y;
        const Vector3d & z = inner.facet[i].plane.norm;
        reper ( z, x, y );
        nat j;
        for ( j = 0; j < nv; ++j ) arr[j] = Vector2d ( x * inner.vertex[j], y * inner.vertex[j] );
        double s1, s2, t1, t2;
        Vector2d s, t;
        minRectangleAroundConvexPolygon ( convexNlogN ( arr, suite ), MathMul(), s, s1, s2, t, t1, t2 );
        double min, max;
        min = max = z * inner.vertex[0];
        for ( j = 1; j < nv; ++j )
        {
            const double t = z * inner.vertex[j];
            if ( min > t ) min = t; else
            if ( max < t ) max = t;
        }
        const double vol = ( max - min ) * ( s2 - s1 ) * ( t2 - t1 );
        if ( minV > vol )
        {
            minV = vol;
            z1 = min;
            z2 = max;
            x1 = s1; x2 = s2;
            y1 = t1; y2 = t2;
            ax = s.x * x + s.y * y;
            ay = t.x * x + t.y * y;
            az = z;
        }
    }
    for ( nat i1 = inner.facet.size()-1; i1 > 1; --i1 )
    {
        const Facet & fa1 = inner.facet[i1];
        for ( nat j1 = 0; j1 < fa1.nv; ++j1 )
        {
            const nat n1 = fa1.index[j1+fa1.nv+1];
            if ( i1 < n1 ) continue;
            Vector3d v1 = fa1.plane.norm % inner.facet[n1].plane.norm;
            if ( norm1 ( v1 ) < 1e-8 ) continue;
            v1.setNorm2();
            for ( nat i2 = i1-1; i2 > 0; --i2 )
            {
                const Facet & fa2 = inner.facet[i2];
                for ( nat j2 = 0; j2 < fa2.nv; ++j2 )
                {
                    const nat n2 = fa2.index[j2+fa2.nv+1];
                    if ( i2 < n2 ) continue;
                    Vector3d v2 = fa2.plane.norm % inner.facet[n2].plane.norm;
                    if ( norm1 ( v2 ) < 1e-8 ) continue;
                    v2.setNorm2();
                    Vector3d norm = v1 % v2;
                    if ( norm1 ( norm ) < 1e-8 ) continue;
                    norm.setNorm2();
                    double min = norm * inner.vertex[fa1.index[j1]];
                    double max = norm * inner.vertex[fa2.index[j2]];
                    if ( min > max )
                    {
                        min = -min;
                        max = -max;;
                        norm = -norm;
                    }
                    if ( ( fa1.plane.norm % norm ) * v1 > 0 ) continue;
                    if ( ( norm % inner.facet[n1].plane.norm ) * v1 > 0 ) continue;
                    if ( ( fa2.plane.norm % norm ) * v2 < 0 ) continue;
                    if ( ( norm % inner.facet[n2].plane.norm ) * v2 < 0 ) continue;
                    Vector3d x, y;
                    const Vector3d & z = norm;
                    reper ( z, x, y );
                    nat j;
                    for ( j = 0; j < nv; ++j ) arr[j] = Vector2d ( x * inner.vertex[j], y * inner.vertex[j] );
                    double s1, s2, t1, t2;
                    Vector2d s, t;
                    minRectangleAroundConvexPolygon ( convexNlogN ( arr, suite ), MathMul(), s, s1, s2, t, t1, t2 );
                    const double vol = ( max - min ) * ( s2 - s1 ) * ( t2 - t1 );
                    if ( minV > vol )
                    {
                        minV = vol;
                        z1 = min;
                        z2 = max;
                        x1 = s1; x2 = s2;
                        y1 = t1; y2 = t2;
                        ax = s.x * x + s.y * y;
                        ay = t.x * x + t.y * y;
                        az = z;
                    }
                }
            }
        }
    }
    if ( minV < 1e300 )
    {
        res.a = 0.5 * ( x2 - x1 );
        res.b = 0.5 * ( y2 - y1 );
        res.c = 0.5 * ( z2 - z1 );
        res.spin = Spin3d ( ax, ay, az );
        res.o = 0.5 * ( ( x2 + x1 ) * ax + ( y2 + y1 ) * ay + ( z2 + z1 ) * az );
        res.isDef = true;
    }
    return res;
}

//**************************** 11.02.2010 *********************************//
//
//          Минимальный по площади поверхности параллелепипед
//          охватывающий выпуклый многогранник
//
//**************************** 23.11.2010 *********************************//

class MathFuncA : public MathFunc2
{
    const double c;
public:
    explicit MathFuncA ( double d ) : c(d) {}
    virtual double operator () ( double a, double b ) const { return a * b + c * ( a + b ); }
};

Def<Cuboid3d> minCuboidAroundConvexPolyhedronA ( const Polyhedron & inner )
{
    Def<Cuboid3d> res;
    const nat nv = inner.vertex.size();
    if ( inner.facet.size() < 4 || nv < 4 ) return res;
    double x1, x2, y1, y2, z1, z2;
    Vector3d ax, ay, az;
    DynArray<Vector2d> arr ( nv );
    Suite<Vector2d> suite;
    double minV = 1e300;
    for ( nat i = 0; i < inner.facet.size(); ++i )
    {
        Vector3d x, y;
        const Vector3d & z = inner.facet[i].plane.norm;
        reper ( z, x, y );
        nat j;
        double min, max;
        min = max = z * inner.vertex[0];
        for ( j = 1; j < nv; ++j )
        {
            const double t = z * inner.vertex[j];
            if ( min > t ) min = t; else
            if ( max < t ) max = t;
        }
        const MathFuncA func ( max - min );
        for ( j = 0; j < nv; ++j ) arr[j] = Vector2d ( x * inner.vertex[j], y * inner.vertex[j] );
        double s1, s2, t1, t2;
        Vector2d s, t;
        minRectangleAroundConvexPolygon ( convexNlogN ( arr, suite ), func, s, s1, s2, t, t1, t2 );
        const double area = func ( s2 - s1, t2 - t1 );
        if ( minV > area )
        {
            minV = area;
            z1 = min;
            z2 = max;
            x1 = s1; x2 = s2;
            y1 = t1; y2 = t2;
            ax = s.x * x + s.y * y;
            ay = t.x * x + t.y * y;
            az = z;
        }
    }
    for ( nat i1 = inner.facet.size()-1; i1 > 1; --i1 )
    {
        const Facet & fa1 = inner.facet[i1];
        for ( nat j1 = 0; j1 < fa1.nv; ++j1 )
        {
            const nat n1 = fa1.index[j1+fa1.nv+1];
            if ( i1 < n1 ) continue;
            Vector3d v1 = fa1.plane.norm % inner.facet[n1].plane.norm;
            if ( norm1 ( v1 ) < 1e-8 ) continue;
            v1.setNorm2();
            for ( nat i2 = i1-1; i2 > 0; --i2 )
            {
                const Facet & fa2 = inner.facet[i2];
                for ( nat j2 = 0; j2 < fa2.nv; ++j2 )
                {
                    const nat n2 = fa2.index[j2+fa2.nv+1];
                    if ( i2 < n2 ) continue;
                    Vector3d v2 = fa2.plane.norm % inner.facet[n2].plane.norm;
                    if ( norm1 ( v2 ) < 1e-8 ) continue;
                    v2.setNorm2();
                    Vector3d norm = v1 % v2;
                    if ( norm1 ( norm ) < 1e-8 ) continue;
                    norm.setNorm2();
                    double min = norm * inner.vertex[fa1.index[j1]];
                    double max = norm * inner.vertex[fa2.index[j2]];
                    if ( min > max )
                    {
                        min = -min;
                        max = -max;;
                        norm = -norm;
                    }
                    if ( ( fa1.plane.norm % norm ) * v1 > 0 ) continue;
                    if ( ( norm % inner.facet[n1].plane.norm ) * v1 > 0 ) continue;
                    if ( ( fa2.plane.norm % norm ) * v2 < 0 ) continue;
                    if ( ( norm % inner.facet[n2].plane.norm ) * v2 < 0 ) continue;
                    const MathFuncA func ( max - min );
                    Vector3d x, y;
                    const Vector3d & z = norm;
                    reper ( z, x, y );
                    nat j;
                    for ( j = 0; j < nv; ++j ) arr[j] = Vector2d ( x * inner.vertex[j], y * inner.vertex[j] );
                    double s1, s2, t1, t2;
                    Vector2d s, t;
                    minRectangleAroundConvexPolygon ( convexNlogN ( arr, suite ), func, s, s1, s2, t, t1, t2 );
                    const double area = func ( s2 - s1, t2 - t1 );
                    if ( minV > area )
                    {
                        minV = area;
                        z1 = min;
                        z2 = max;
                        x1 = s1; x2 = s2;
                        y1 = t1; y2 = t2;
                        ax = s.x * x + s.y * y;
                        ay = t.x * x + t.y * y;
                        az = z;
                    }
                }
            }
        }
    }
    if ( minV < 1e300 )
    {
        res.a = 0.5 * ( x2 - x1 );
        res.b = 0.5 * ( y2 - y1 );
        res.c = 0.5 * ( z2 - z1 );
        res.spin = Spin3d ( ax, ay, az );
        res.o = 0.5 * ( ( x2 + x1 ) * ax + ( y2 + y1 ) * ay + ( z2 + z1 ) * az );
        res.isDef = true;
    }
    return res;
}
