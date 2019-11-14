
#include "math.h"

#include "tune.h"
#include "Mathem.h"
#include "moment3d.h"
#include "Polyhedron.h"

//**************************** 09.01.2008 *********************************//
//
//            Центры масс ( точек, отрезков, грани и многогранника )
//
//**************************** 17.11.2012 *********************************//

Def<Vector3d> centerOfMass ( CArrRef<Vector3d> point ) // Центр масс набора точек с весами
{
    if ( point.size() == 0 ) return Def<Vector3d>();
    Vector3d v ( 0., 0., 0. );
    for ( nat i = 0; i < point.size(); ++i ) v += point[i];
    return v / point.size();
}

Def<Vector3d> centerOfMass ( CArrRef<Vector3d> point, CArrRef<double> mass ) // Центр масс набора точек с весами
{
    double m = 0.;
    Vector3d v ( 0., 0., 0. );
    for ( nat i = 0; i < point.size(); ++i )
    {
        v += mass[i] * point[i];
        m += mass[i];
    }
    return m == 0 ? Def<Vector3d>() : Def<Vector3d> ( v / m );
}

Def<Vector3d> centerOfMass ( CArrRef<Segment3d> segm ) // Центр масс набора отрезков
{
    double m = 0.;
    Vector3d v ( 0., 0., 0. );
    for ( nat i = 0; i < segm.size(); ++i )
    {
        const Segment3d & s = segm[i];
        const double len = norm2 ( s );
        v += len * ( s.a + s.b );
        m += len;
    }
    return m == 0 ? Def<Vector3d>() : Def<Vector3d> ( v / ( m + m ) );
}

Def<Vector3d> centerOfMass ( nat i, const Polyhedron & poly ) // Центр масс i-той грани
{
    return poly.facet[i].centerOfMass ( poly.vertex );
}

Def<Vector3d> centerOfMass ( const Facet & facet, CArrRef<Vector3d> vert ) // Центр масс грани
{
    return facet.centerOfMass ( vert );
}

Def<Vector3d> centerOfMass ( const Polyhedron & poly ) // Центр масс многогранника
{
    return poly.centerOfMass();
}

//**************************** 10.01.2008 *********************************//
//
//       Вычисление моментов 2-го порядка относительно плоскости
//
//**************************** 04.09.2010 *********************************//

double momentum2pnt ( CArrRef<Vector3d> point, const Plane3d & plane )
{
    double sum = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const double d = plane % point[i];
        sum += d * d;
    }
    return sum;
}

double momentum2pnt ( CArrRef<Vector3d> point, CArrRef<double> mass, const Plane3d & plane )
{
    double sum = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const double d = plane % point[i];
        sum += d * d * mass[i];
    }
    return sum;
}

double momentum2sgm ( CArrRef<Segment3d> segm, const Plane3d & plane )
{
    double sum = 0.;
    for ( nat i = 0; i < segm.size(); ++i )
    {
        const Segment3d & s = segm[i];
        const double len = norm2 ( s );
        const double a = plane % s.a;
        const double b = plane % s.b;
        sum += ( a * ( a + b ) + b * b ) * len;
    }
    return sum / 3.;
}

double momentum2fct ( nat i, const Polyhedron & poly, const Plane3d & plane )
{
    if ( i >= poly.facet.size() ) return 0.;
    const Facet & facet = poly.facet[i];
    if ( facet.nv < 3 ) return 0.;
    const Vector3d v1 = poly.vertex[facet.index[0]];
    const double a = plane % v1;
    Vector3d v2 = poly.vertex[facet.index[1]];
    double b = plane % v2;
    double sum = 0.;
    for ( nat j = 2; j < facet.nv; ++j )
    {
        const Vector3d & v3 = poly.vertex[facet.index[j]];
        const double c = plane % v3;
        const double s = ( v2 - v1 ) % ( v3 - v2 ) * facet.plane.norm;
        sum += ( a * ( a + b + c ) + b * ( b + c ) + c * c ) * s;
        v2 = v3;
        b = c;
    }
    return sum / 12.;
}

double momentum2plh ( const Polyhedron & poly, const Plane3d & plane )
{
    double sum = 0.;
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & facet = poly.facet[i];
        for ( nat j = 2; j < facet.nv; ++j )
        {
            const Vector3d & v1 = poly.vertex[facet.index[0]];
            const Vector3d & v2 = poly.vertex[facet.index[j-1]];
            const Vector3d & v3 = poly.vertex[facet.index[j]];
            const double a = plane % v1;
            const double b = plane % v2;
            const double c = plane % v3;
            const double d = plane.dist;
            const double v = ( ( v1 - v2 ) % ( v3 - v2 ) * facet.plane.norm ) * facet.plane.dist;
            sum += ( a * ( a + b + c + d ) + b * ( b + c + d ) + c * ( c + d ) + d * d ) * v;
        }
    }
    return sum / 60.;
}

//**************************** 10.01.2008 *********************************//
//
//           Вычисление основных осей и моментов 2-го порядка
//
//**************************** 28.09.2010 *********************************//

Def<Mom3d> solve ( double xx, double xy, double xz, double yy, double yz, double zz, const Vector3d & o )
{
    Def<Mom3d> mom;
    double * a[3];
    Vector3d a0 ( xx, xy, xz );
    Vector3d a1 ( xy, yy, yz );
    Vector3d a2 ( xz, yz, zz );
    a[0] = &a0.x;
    a[1] = &a1.x;
    a[2] = &a2.x;
    double d[3];
    double * u[3];
    Vector3d u0;
    Vector3d u1;
    Vector3d u2;
    u[0] = &u0.x;
    u[1] = &u1.x;
    u[2] = &u2.x;
    jacobi ( 3, a, d, u );
    if ( d[0] <= d[1] )
    {
        if ( d[1] <= d[2] )
        {
            mom.minMom = d[0];
            mom.minNorm = Vector3d ( u0.x, u1.x, u2.x );
            mom.midMom = d[1];
            mom.midNorm = Vector3d ( u0.y, u1.y, u2.y );
            mom.maxMom = d[2];
            mom.maxNorm = Vector3d ( u0.z, u1.z, u2.z );
        }
        else
        {
            if ( d[0] <= d[2] )
            {
                mom.minMom = d[0];
                mom.minNorm = Vector3d ( u0.x, u1.x, u2.x );
                mom.midMom = d[2];
                mom.midNorm = Vector3d ( u0.z, u1.z, u2.z );
                mom.maxMom = d[1];
                mom.maxNorm = Vector3d ( -u0.y, -u1.y, -u2.y );
            }
            else
            {
                mom.minMom = d[2];
                mom.minNorm = Vector3d ( u0.z, u1.z, u2.z );
                mom.midMom = d[0];
                mom.midNorm = Vector3d ( u0.x, u1.x, u2.x );
                mom.maxMom = d[1];
                mom.maxNorm = Vector3d ( u0.y, u1.y, u2.y );
            }
        }
    }
    else
    {
        if ( d[2] <= d[1] )
        {
            mom.minMom = d[2];
            mom.minNorm = Vector3d ( u0.z, u1.z, u2.z );
            mom.midMom = d[1];
            mom.midNorm = Vector3d ( u0.y, u1.y, u2.y );
            mom.maxMom = d[0];
            mom.maxNorm = Vector3d ( -u0.x, -u1.x, -u2.x );
        }
        else
        {
            if ( d[0] <= d[2] )
            {
                mom.minMom = d[1];
                mom.minNorm = Vector3d ( u0.y, u1.y, u2.y );
                mom.midMom = d[0];
                mom.midNorm = Vector3d ( u0.x, u1.x, u2.x );
                mom.maxMom = d[2];
                mom.maxNorm = Vector3d ( -u0.z, -u1.z, -u2.z );
            }
            else
            {
                mom.minMom = d[1];
                mom.minNorm = Vector3d ( u0.y, u1.y, u2.y );
                mom.midMom = d[2];
                mom.midNorm = Vector3d ( u0.z, u1.z, u2.z );
                mom.maxMom = d[0];
                mom.maxNorm = Vector3d ( u0.x, u1.x, u2.x );
            }
        }
    }
    mom.o = o;
    mom.isDef = true;
    return mom;
}

Def<Mom3d> momentum2pnt ( CArrRef<Vector3d> point )
{
    const Def<Vector3d> o = centerOfMass ( point );
    if ( ! o.isDef ) return Def<Mom3d>();
    double xx, xy, xz, yy, yz, zz;
    xx = xy = xz = yy = yz = zz = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const Vector3d u = point[i] - o;
        xx += u.x * u.x;
        xy += u.x * u.y;
        xz += u.x * u.z;
        yy += u.y * u.y;
        yz += u.y * u.z;
        zz += u.z * u.z;
    }
    return solve ( xx, xy, xz, yy, yz, zz, o );
}

Def<Mom3d> momentum2pnt ( CArrRef<Vector3d> point, CArrRef<double> mass )
{
    const Def<Vector3d> o = centerOfMass ( point, mass );
    if ( ! o.isDef ) return Def<Mom3d>();
    double xx, xy, xz, yy, yz, zz;
    xx = xy = xz = yy = yz = zz = 0.;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const Vector3d u = point[i] - o;
        xx += u.x * u.x * mass[i];
        xy += u.x * u.y * mass[i];
        xz += u.x * u.z * mass[i];
        yy += u.y * u.y * mass[i];
        yz += u.y * u.z * mass[i];
        zz += u.z * u.z * mass[i];
    }
    return solve ( xx, xy, xz, yy, yz, zz, o );
}

Def<Mom3d> momentum2sgm ( CArrRef<Segment3d> segm )
{
    const Def<Vector3d> o = centerOfMass ( segm );
    if ( ! o.isDef ) return Def<Mom3d>();
    double xx, xy, xz, yy, yz, zz;
    xx = xy = xz = yy = yz = zz = 0.;
    for ( nat i = 0; i < segm.size(); ++i )
    {
        const Segment3d & s = segm[i];
        const double g = norm2 ( s ) / 6.;
        const Vector3d a = s.a - o;
        const Vector3d b = s.b - o;
        xx += g * ( a.x * ( a.x + a.x + b.x ) + b.x * ( a.x + b.x + b.x ) );
        xy += g * ( a.y * ( a.x + a.x + b.x ) + b.y * ( a.x + b.x + b.x ) );
        xz += g * ( a.z * ( a.x + a.x + b.x ) + b.z * ( a.x + b.x + b.x ) );
        yy += g * ( a.y * ( a.y + a.y + b.y ) + b.y * ( a.y + b.y + b.y ) );
        yz += g * ( a.z * ( a.y + a.y + b.y ) + b.z * ( a.y + b.y + b.y ) );
        zz += g * ( a.z * ( a.z + a.z + b.z ) + b.z * ( a.z + b.z + b.z ) );
    }
    return solve ( xx, xy, xz, yy, yz, zz, o );
}

Def<Mom3d> momentum2fct ( nat i, const Polyhedron & poly )
{
    const Def<Vector3d> o = centerOfMass ( i, poly );
    if ( ! o.isDef ) return Def<Mom3d>();
    double xx, xy, xz, yy, yz, zz;
    xx = xy = xz = yy = yz = zz = 0.;
    const Facet & facet = poly.facet[i];
    const Vector3d a = poly.vertex[facet.index[0]] - o;
    Vector3d b = poly.vertex[facet.index[1]] - o;
    for ( nat j = 2; j < facet.nv; ++j )
    {
        const Vector3d c = poly.vertex[facet.index[j]] - o;
        const double g = ( ( b - a ) % ( c - b ) * facet.plane.norm ) / 24.;
        xx += g * ( a.x * ( a.x + b.x + c.x ) + b.x * ( b.x + c.x ) + c.x * c.x ) * 2.;
        xy += g * ( a.x * ( a.y + b.y + c.y ) + b.x * ( b.y + c.y ) + c.x * c.y +
                    a.y * ( a.x + b.x + c.x ) + b.y * ( b.x + c.x ) + c.y * c.x );
        xz += g * ( a.x * ( a.z + b.z + c.z ) + b.x * ( b.z + c.z ) + c.x * c.z +
                    a.z * ( a.x + b.x + c.x ) + b.z * ( b.x + c.x ) + c.z * c.x );
        yy += g * ( a.y * ( a.y + b.y + c.y ) + b.y * ( b.y + c.y ) + c.y * c.y ) * 2.;
        yz += g * ( a.y * ( a.z + b.z + c.z ) + b.y * ( b.z + c.z ) + c.y * c.z +
                    a.z * ( a.y + b.y + c.y ) + b.z * ( b.y + c.y ) + c.z * c.y );
        zz += g * ( a.z * ( a.z + b.z + c.z ) + b.z * ( b.z + c.z ) + c.z * c.z ) * 2.;
        b = c;
    }
    return solve ( xx, xy, xz, yy, yz, zz, o );
}

Def<Mom3d> momentum2plh ( const Polyhedron & poly )
{
    const Def<Vector3d> o = centerOfMass ( poly );
    if ( ! o.isDef ) return Def<Mom3d>();
    double xx, xy, xz, yy, yz, zz;
    xx = xy = xz = yy = yz = zz = 0.;
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & facet = poly.facet[i];
        if ( facet.nv < 3 ) continue;
        const Vector3d a = poly.vertex[facet.index[0]] - o;
        Vector3d b = poly.vertex[facet.index[1]] - o;
        const double d = facet.plane % o;
        for ( nat j = 2; j < facet.nv; ++j )
        {
            const Vector3d c = poly.vertex[facet.index[j]] - o;
            const double g = ( ( b - a ) % ( b - c ) * facet.plane.norm ) * d / 120.;
            xx += g * ( a.x * ( a.x + b.x + c.x ) + b.x * ( b.x + c.x ) + c.x * c.x ) * 2.;
            xy += g * ( a.x * ( a.y + b.y + c.y ) + b.x * ( b.y + c.y ) + c.x * c.y +
                        a.y * ( a.x + b.x + c.x ) + b.y * ( b.x + c.x ) + c.y * c.x );
            xz += g * ( a.x * ( a.z + b.z + c.z ) + b.x * ( b.z + c.z ) + c.x * c.z +
                        a.z * ( a.x + b.x + c.x ) + b.z * ( b.x + c.x ) + c.z * c.x );
            yy += g * ( a.y * ( a.y + b.y + c.y ) + b.y * ( b.y + c.y ) + c.y * c.y ) * 2.;
            yz += g * ( a.y * ( a.z + b.z + c.z ) + b.y * ( b.z + c.z ) + c.y * c.z +
                        a.z * ( a.y + b.y + c.y ) + b.z * ( b.y + c.y ) + c.z * c.y );
            zz += g * ( a.z * ( a.z + b.z + c.z ) + b.z * ( b.z + c.z ) + c.z * c.z ) * 2.;
            b = c;
        }
    }
    return solve ( xx, xy, xz, yy, yz, zz, o );
}

//**************************** 29.11.2008 *********************************//
//
//          Создание эллипсоида по моментам 2-го порядка
//
//**************************** 29.09.2010 *********************************//

Def<Ellipsoid3d> getEllipsoid ( const Def<Mom3d> & mom )
{
    Def<Ellipsoid3d> res;
    if ( ! mom.isDef ) return res;
    const double x = fabs ( mom.minMom );
    const double y = fabs ( mom.midMom );
    const double z = fabs ( mom.maxMom );
    const double k = 15. / ( 4. * M_PI );
    const double x1 = log ( k * x );
    const double y1 = log ( k * y );
    const double z1 = log ( k * z );
    const double a1 = ( 4. * x1 - y1 - z1 ) * 0.1;
    const double b1 = ( 4. * y1 - z1 - x1 ) * 0.1;
    const double c1 = ( 4. * z1 - x1 - y1 ) * 0.1;
    if ( a1 < -307 || a1 > 308 ||
         b1 < -307 || b1 > 308 ||
         c1 < -307 || c1 > 308 ) return res;
    res.a = exp ( a1 );
    res.b = exp ( b1 );
    res.c = exp ( c1 );
    res.spin = Spin3d ( mom.minNorm, mom.midNorm, mom.maxNorm );
    res.o = mom.o;
    res.isDef = true;
    return res;
}

//*************************** 15.05.2009 **********************************//
//
//     Создание прямоугольного параллелепипеда по моментам 2-го порядка
//
//*************************** 29.09.2010 **********************************//

Def<Cuboid3d> getCuboid ( const Def<Mom3d> & mom )
{
    Def<Cuboid3d> res;
    if ( ! mom.isDef ) return res;
    const double x = fabs ( mom.minMom );
    const double y = fabs ( mom.midMom );
    const double z = fabs ( mom.maxMom );
    const double x1 = log ( 0.75 * x );
    const double y1 = log ( 0.75 * y );
    const double z1 = log ( 0.75 * z );
    const double a1 = ( 4. * x1 - y1 - z1 ) * 0.1;
    const double b1 = ( 4. * y1 - z1 - x1 ) * 0.1;
    const double c1 = ( 4. * z1 - x1 - y1 ) * 0.1;
    if ( a1 < -307 || a1 > 308 ||
         b1 < -307 || b1 > 308 ||
         c1 < -307 || c1 > 308 ) return res;
    res.a = exp ( a1 );
    res.b = exp ( b1 );
    res.c = exp ( c1 );
    res.spin = Spin3d ( mom.minNorm, mom.midNorm, mom.maxNorm );
    res.o = mom.o;
    res.isDef = true;
    return res;
}