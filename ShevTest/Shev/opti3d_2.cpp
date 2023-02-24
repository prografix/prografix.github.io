
#include <math.h>

#include "tune.h"
#include "rand.h"
#include "heap.h"
#include "Mathem.h"
#include "func3d.h"
#include "opti1d.h"
#include "opti3d.h"
#include "Vector3d.h"
#include "Vector4d.h"
#include "approx3d.h"
#include "WireModel.h"
#include "Polyhedron.h"
#include "intersect3d.h"

//**************************** 09.03.2007 *********************************//
//
//          Максимальная сфера вписанная в выпуклый многогранник
//          dim - габариты многогранника
//          plane - массив указателей на плоскости граней
//
//**************************** 16.11.2011 *********************************//

Def<Sphere3d> maxSphereInConvexPolyhedron ( const Segment3d & dim, CArrRef<const Plane3d *> plane, ArrRef<nat> index ) 
{
    Def<Sphere3d> res;
    const nat n = plane.size();
    if ( n < 4 || dim.b.x < dim.a.x || dim.b.y < dim.a.y || dim.b.z < dim.a.z || index.size() < 4 ) return res;
    const double rm = ( dim.b.x - dim.a.x + dim.b.y - dim.a.y + dim.b.z - dim.a.z ) / 6.;
    const double eps = 1e-9 * rm;
    Vector4d arr[5];
    arr[0] = Vector4d ( dim.b.x, dim.b.y, dim.b.z, rm );
    arr[1] = Vector4d ( -1., 0., 0., 0. );
    arr[2] = Vector4d ( 0., -1., 0., 0. );
    arr[3] = Vector4d ( 0., 0., -1., 0. );
    arr[4] = Vector4d ( 0., 0., 0., -1. );
    index[0] = index[1] = index[2] = index[3] = n;
    nat k, n2 = n + n;
    for ( k = 0; k < n2; ++k )
    {
        Vector4d & a0 = arr[0];
        const double r = a0.x4;
        const Vector3d o ( a0.x1, a0.x2, a0.x3 );
        double max = - r;
        nat i, im;
        for ( i = 0; i < n; ++i )
        {
            const double t = *plane[i] % o;
            if ( max < t ) max = t, im = i;
        }
        const double dist = max + r;
        if ( dist <= eps )
        {
            res.o = o;
            res.r = r;
            break;
        }
        const Vector3d & norm = plane[im]->norm;
        const Vector4d cor ( norm.x, norm.y, norm.z, 1. );
        nat ib = 0;
        double sg;
        for ( i = 1; i <= 4; ++i )
        {
            const Vector4d & v = arr[i];
            double t = cor * v;
            if ( t > -1e-8 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.x4 * t;
                ib = i;
                sg = t;
            }
            else
            {
                const double s = v.x4 * t;
                if ( s < max ) max = s, ib = i, sg = t;
            }
        }
        if ( ib == 0 )
        {
            return res;
        }
        index[ib-1] = im;
        const Vector4d & v = arr[ib];
        a0 -= v * ( dist * sg );
        for ( i = 1; i <= 4; ++i )
        {
            if ( i == ib ) continue;
            Vector4d & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sg );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    if ( k == n2 )
    {
        return res;
    }
    res.isDef = true;
    return res;
}

Def<Sphere3d> maxSphereInConvexPolyhedron ( const Segment3d & dim, CArrRef<const Plane3d *> plane ) 
{
    return maxSphereInConvexPolyhedron ( dim, plane, FixArray<nat, 4>() );
}

Def<Sphere3d> maxSphereInConvexPolyhedron ( const Polyhedron & poly )
{
    if ( poly.facet.size() < 4 || poly.vertex.size() < 4 ) return Sphere3d();
    DynArray<const Plane3d *> plane ( poly.facet.size() );
    for ( nat i = 0; i < poly.facet.size(); ++i ) plane[i] = & poly.facet[i].plane;
    return maxSphereInConvexPolyhedron ( dimensions ( poly.vertex ), plane );
}

Def<Sphere3d> maxSphereInConvexPolyhedron ( const Segment3d & dim, CArrRef<Plane3d> plane, ArrRef<nat> index ) 
{
    Def<Sphere3d> res;
    const nat n = plane.size();
    if ( n < 4 || dim.b.x < dim.a.x || dim.b.y < dim.a.y || dim.b.z < dim.a.z || index.size() < 4 ) return res;
    const double rm = ( dim.b.x - dim.a.x + dim.b.y - dim.a.y + dim.b.z - dim.a.z ) / 6.;
    const double eps = 1e-9 * rm;
    Vector4d arr[5];
    arr[0] = Vector4d ( dim.b.x, dim.b.y, dim.b.z, rm );
    arr[1] = Vector4d ( -1., 0., 0., 0. );
    arr[2] = Vector4d ( 0., -1., 0., 0. );
    arr[3] = Vector4d ( 0., 0., -1., 0. );
    arr[4] = Vector4d ( 0., 0., 0., -1. );
    index[0] = index[1] = index[2] = index[3] = n;
    nat k, n2 = n + n;
    for ( k = 0; k < n2; ++k )
    {
        Vector4d & a0 = arr[0];
        const double r = a0.x4;
        const Vector3d o ( a0.x1, a0.x2, a0.x3 );
        double max = - r;
        nat i, im;
        for ( i = 0; i < n; ++i )
        {
            const double t = plane[i] % o;
            if ( max < t ) max = t, im = i;
        }
        const double dist = max + r;
        if ( dist <= eps )
        {
            res.o = o;
            res.r = r;
            break;
        }
        const Vector3d & norm = plane[im].norm;
        const Vector4d cor ( norm.x, norm.y, norm.z, 1. );
        nat ib = 0;
        double sg;
        for ( i = 1; i <= 4; ++i )
        {
            const Vector4d & v = arr[i];
            double t = cor * v;
            if ( t > -1e-8 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.x4 * t;
                ib = i;
                sg = t;
            }
            else
            {
                const double s = v.x4 * t;
                if ( s < max ) max = s, ib = i, sg = t;
            }
        }
        if ( ib == 0 )
        {
            return res;
        }
        index[ib-1] = im;
        const Vector4d & v = arr[ib];
        a0 -= v * ( dist * sg );
        for ( i = 1; i <= 4; ++i )
        {
            if ( i == ib ) continue;
            Vector4d & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sg );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    if ( k == n2 )
    {
        return res;
    }
    res.isDef = true;
    return res;
}

Def<Sphere3d> maxSphereInConvexPolyhedron ( const Segment3d & dim, CArrRef<Plane3d> plane ) 
{
    return maxSphereInConvexPolyhedron ( dim, plane, FixArray<nat, 4>() );
}

//************************* 08.08.2010 *******************************//
//
//    Максимальная фигура вращения вписанная в выпуклый многогранник
//    func - функция определяющая макс. удалённую точку фигуры
//           в заданном направлении
//    res - преобразование фигуры
//
//************************* 05.09.2010 *******************************//

class FarPoint
{
public:
    virtual Vector3d operator() ( const Vector3d & a, const Vector3d & v ) = 0;
};

bool maxSolidOfRevolutionInConvexPolyhedron ( const Polyhedron & poly, bool sym, List< Vertex<6> > & stor,
                                              Conform3d & res, FarPoint & func )
{
    if ( poly.facet.size() < 4 || poly.vertex.size() < 4 ) return false;
// Приведение многоугольника к стандартному положению
    DynArray<Plane3d> plane ( poly.facet.size() );
    nat i;
    for ( i = 0; i < poly.facet.size(); ++i ) plane[i] = poly.facet[i].plane;
    const Segment3d seg = dimensions ( poly.vertex );
    const double max = normU ( seg );
    if ( max == 0 ) return false;
    const double coef = 2. / max;
    const Conform3d conf ( Spin3d(), -0.5 * coef * ( seg.a + seg.b ), coef );
    plane *= Similar3d ( conf );
// Инициализация области допустимых преобразований
    WireModel<6> model;
    model.simplex ( 2*(6+1), stor );
    Double<6> dn;
    dn.fill ( 2. );
    if ( sym ) dn.d2 = 0;
    model.vlist -= dn;
    const double eps = 1e-6;
    for ( i = 0; i < 800; ++i )
    {
// Поиск максимального решения
        Show< Vertex<6> > show ( model.vlist );
        const Vertex<6> * ptr = 0;
        double max = -1.;
        if ( show.top() )
        do
        {
            const Vertex<6> * p = show.cur();
            const Double<6> & coor = p->coor;
            const double t = coor.d0 * coor.d0 + coor.d1 * coor.d1 + coor.d2 * coor.d2;
            if ( max < t ) max = t, ptr = p;
        }
        while ( show.next() );
        if ( ptr == 0 )
            return false;
        const Vector3d a ( ptr->coor.d0, ptr->coor.d1, ptr->coor.d2 );
        const Vector3d o ( ptr->coor.d3, ptr->coor.d4, ptr->coor.d5 );
        nat jm = 0;
        Vector3d pm;
        max = 0.;
        for ( nat j = 0; j < plane.size(); ++j )
        {
            const Plane3d & pl = plane[j];
            const Vector3d p = func ( a, pl.norm );
            const double t = p * a + pl % o;
            if ( max < t ) max = t, jm = j, pm = p;
        }
        if ( max < eps )
        {
            res.trans = o;
            res.magn = norm2 ( a );
            res.spin = Spin3d ( Vector3d(0,0,1), a );
            res = ~conf * res;
            model.vlist.movAllAftLas ( stor );
            return true;
        }
        const Vector3d & v = plane[jm].norm;
        Double<7> cor;
        cor.d0 = pm.x;
        cor.d1 = pm.y;
        cor.d2 = pm.z;
        cor.d3 = v.x;
        cor.d4 = v.y;
        cor.d5 = v.z;
        cor.d6 = plane[jm].dist;
        model.cut ( cor, stor );
    }
    model.vlist.movAllAftLas ( stor );
    return false;
}

//**************************** 08.08.2010 *********************************//
//
//    Максимальный фиксированный цилиндр вписанный в выпуклый многогранник
//    ext - отношение высоты цилиндра к его диаметру
//
//**************************** 14.08.2010 *********************************//

class CylinderFarPoint : public FarPoint
{
    const double h;
public:
    CylinderFarPoint ( double u ) : h(u) {}
    virtual Vector3d operator() ( const Vector3d & a, const Vector3d & v )
    {
        const double s = a * v; 
        return ( a - s * v ).setNorm2() + v * ( s < 0 ? -h : h );
    }
};

template<class T>
inline T & copy ( const Conform3d & conf, const double & ext, T & res )
{
    res.isDef = true;
    res.spin = conf.spin;
    res.o = conf.trans;
    res.r = conf.magn;
    res.h = conf.magn * ext;
    return res;
}

Def<Cylinder3d> maxFixCylinderInConvexPolyhedron ( const Polyhedron & poly, double ext )
{
    Conform3d conf;
    Def<Cylinder3d> res;
    List< Vertex<6> > stor;
    if ( ! maxSolidOfRevolutionInConvexPolyhedron ( poly, true, stor, conf, CylinderFarPoint(ext) ) )
        return res;
    return copy ( conf, ext, res );
}

//**************************** 14.08.2010 *********************************//
//
//    Максимальный фиксированный конус вписанный в выпуклый многогранник
//    ext - отношение высоты конуса к его диаметру
//
//**************************** 20.08.2012 *********************************//

class ConeFarPoint : public FarPoint
{
    const double h;
public:
    ConeFarPoint ( double u ) : h(u) {}
    virtual Vector3d operator() ( const Vector3d & a, const Vector3d & v )
    {
        const double s = a * v; 
        const Vector3d p = ( a - s * v ).setNorm2();
        return 2 * h * s > a * p ? v * h : p - v * h;
    }
};

Def<Cone3d> maxFixConeInConvexPolyhedron ( const Polyhedron & poly, double ext )
{
    Conform3d conf;
    Def<Cone3d> res;
    List< Vertex<6> > stor;
    if ( ! maxSolidOfRevolutionInConvexPolyhedron ( poly, false, stor, conf, ConeFarPoint(ext) ) )
        return res;
    return copy ( conf, ext, res );
}

//**************************** 18.08.2010 *********************************//
//
//    Максимальный фиксированный сфероид вписанный в выпуклый многогранник
//    ext - отношение неравных диаметров эллипсоида
//
//**************************** 18.08.2010 *********************************//

class SpheroidFarPoint : public FarPoint
{
    const double h;
public:
    SpheroidFarPoint ( double u ) : h(u*u-1) {}
    virtual Vector3d operator() ( const Vector3d & a, const Vector3d & v )
    {
        const double s = a * v;
        const double q = a * a + h * s * s;
        return q > 0 ? ( a + ( h * s ) * v ) / sqrt ( q ) : Vector3d(0,0,0);
    }
};

inline Def<Ellipsoid3d> & copy ( const Conform3d & conf, const double & ext, Def<Ellipsoid3d> & res )
{
    res.isDef = true;
    res.spin = conf.spin;
    res.o = conf.trans;
    res.a = res.b = conf.magn;
    res.c = conf.magn * ext;
    return res;
}

Def<Ellipsoid3d> maxFixSpheroidInConvexPolyhedron ( const Polyhedron & poly, double ext )
{
    Conform3d conf;
    Def<Ellipsoid3d> res;
    List< Vertex<6> > stor;
    if ( ! maxSolidOfRevolutionInConvexPolyhedron ( poly, true, stor, conf, SpheroidFarPoint(ext) ) )
        return res;
    return copy ( conf, ext, res );
}

//**************************** 24.08.2012 *********************************//
//
//     Максимальный по объёму цилиндр вписанный в выпуклый многогранник
//
//**************************** 24.08.2012 *********************************//

class MaxCylinder: public MathFunc1
{
    const Polyhedron & poly;
    mutable List< Vertex<6> > stor;
public:
    mutable double max, ext;
    mutable Conform3d conf;
    MaxCylinder ( const Polyhedron & p ) : poly(p), max(0) {}
    virtual double operator () ( double p ) const
    {
        Conform3d c;
        const double e = exp ( p );
        if ( ! maxSolidOfRevolutionInConvexPolyhedron ( poly, true, stor, c, CylinderFarPoint(e) ) )
            return 0;
        const double v = c.magn * c.magn * c.magn * e;
        if ( max < v ) max = v, ext = e, conf = c;
        return v;
    }
};

Def<Cylinder3d> maxCylinderInConvexPolyhedronV ( const Polyhedron & poly )
{
    Def<Cylinder3d> res;
    if ( poly.facet.size() < 4 ) return res;
    MaxCylinder func ( poly );
    fmax ( -5, 0, func, 0.1 );
    fmax (  0, 5, func, 0.1 );
    if ( func.max == 0 ) return res;
    return copy ( func.conf, func.ext, res );
}

//**************************** 24.08.2012 *********************************//
//
//     Максимальный по объёму конус вписанный в выпуклый многогранник
//
//**************************** 24.08.2012 *********************************//

class MaxCone: public MathFunc1
{
    const Polyhedron & poly;
    mutable List< Vertex<6> > stor;
public:
    mutable double max, ext;
    mutable Conform3d conf;
    MaxCone ( const Polyhedron & p ) : poly(p), max(0) {}
    virtual double operator () ( double p ) const
    {
        Conform3d c;
        const double e = exp ( p );
        if ( ! maxSolidOfRevolutionInConvexPolyhedron ( poly, false, stor, c, ConeFarPoint(e) ) )
            return 0;
        const double v = c.magn * c.magn * c.magn * e;
        if ( max < v ) max = v, ext = e, conf = c;
        return v;
    }
};

Def<Cone3d> maxConeInConvexPolyhedronV ( const Polyhedron & poly )
{
    Def<Cone3d> res;
    if ( poly.facet.size() < 4 ) return res;
    MaxCone func ( poly );
    fmax ( -5, 0, func, 0.1 );
    fmax (  0, 5, func, 0.1 );
    if ( func.max == 0 ) return res;
    return copy ( func.conf, func.ext, res );
}

//**************************** 24.08.2012 *********************************//
//
//    Максимальный по объёму сфероид вписанный в выпуклый многогранник
//
//**************************** 24.08.2012 *********************************//

class MaxSpheroid: public MathFunc1
{
    const Polyhedron & poly;
    mutable List< Vertex<6> > stor;
public:
    mutable double max, ext;
    mutable Conform3d conf;
    MaxSpheroid ( const Polyhedron & p ) : poly(p), max(0) {}
    virtual double operator () ( double p ) const
    {
        Conform3d c;
        const double e = exp ( p );
        if ( ! maxSolidOfRevolutionInConvexPolyhedron ( poly, true, stor, c, SpheroidFarPoint(e) ) )
            return 0;
        const double v = c.magn * c.magn * c.magn * e;
        if ( max < v ) max = v, ext = e, conf = c;
        return v;
    }
};

Def<Ellipsoid3d> maxSpheroidInConvexPolyhedronV ( const Polyhedron & poly )
{
    Def<Ellipsoid3d> res;
    if ( poly.facet.size() < 4 ) return res;
    MaxSpheroid func ( poly );
    fmax ( -5, -0.1, func, 0.1 );
    fmax (  0.1,  5, func, 0.1 );
    if ( func.max == 0 ) return res;
    const Def<Sphere3d> fig = maxSphereInConvexPolyhedron ( poly );
    if ( fig.isDef && fig.r * fig.r * fig.r > func.max )
    {
        res.isDef = true;
        res.o = fig.o;
        res.a = res.b = res.c = fig.r;
        return res;
    }
    return copy ( func.conf, func.ext, res );
}

//**************************** 21.11.2013 *********************************//
//
//      Максимальный по объёму эллипсоид вписанный в тетраэдр
//
//**************************** 21.11.2013 *********************************//

Ellipsoid3d maxEllipsoidInTetrahedronV ( Vector3d v1, Vector3d v2, Vector3d v3, Vector3d v4 )
{
    Affin3d aff;
    aff.s = ( v1 + v2 + v3 + v4 ) / 4;
    v1 -= aff.s;
    v2 -= aff.s;
    v3 -= aff.s;
    aff.t.x.x = v2.x + v3.x;
    aff.t.x.y = v3.x + v1.x;
    aff.t.x.z = v1.x + v2.x;
    aff.t.y.x = v2.y + v3.y;
    aff.t.y.y = v3.y + v1.y;
    aff.t.y.z = v1.y + v2.y;
    aff.t.z.x = v2.z + v3.z;
    aff.t.z.y = v3.z + v1.z;
    aff.t.z.z = v1.z + v2.z;
    return aff ( Sphere3d ( sqrt(1./12), Vector3d (0,0,0) ) );
}

//**************************** 01.02.2017 *********************************//
//
//   Максимальный по объёму эллипсоид вписанный в выпуклый многоугольник
//
//**************************** 01.02.2017 *********************************//
//
//**********************************************************************************************************************
//
//  Преобразование:
//  x1 = a * x + d * y + f * z + X
//  y1 =         b * y + e * z + Y
//  z1 =                 c * z + Z
//
//  Касательная плоскость к эллипсоиду:
//  nx * ( a * x + d * y + f * z + X ) + ny * ( b * y + e * z + Y ) + nz * ( c * z + Z ) + g = 0
//  nx * a * x + ( nx * d + ny * b ) * y + ( nx * f + ny * e + nz * c ) * z + nx * X + ny * Y + nz * Z + g = 0
//  ( nx * a )^2 + ( nx * d + ny * b )^2 + ( nx * f + ny * e + nz * c )^2 = ( nx * X + ny * Y + nz * Z + g )^2
//
//****************************************** Замена переменных ********************************************************
//
//  nx * nx * A + ny * ny * B + nz * nz * C + 2 * nx * ny * D + 2 * ny * nz * E + 2 * nx * nz * F = 
//  = 2 * g * ( nx * X + ny * Y + nz * Z ) + g * g
//
//  A = a * a + d * d + f * f - X * X    B = b * b + e * e - Y * Y    C = c * c - Z * Z
//  D = d * b + f * e - X * Y            E = e * c - Y * Z            F = f * c - X * Z
//
//  c = sqrt ( C + Z * Z )            e = ( E + Y * Z ) / c            f = ( F + X * Z ) / c
//  b = sqrt ( B + Y * Y - e * e )    d = ( D + X * Y - f * e ) / b    a = sqrt ( A + X * X - d * d - f * f )
//
//  Целевая функция:
//
//  Ф = ( A + X * X ) * ( B + Y * Y ) * ( C + Z * Z ) + ( D + X * Y ) * ( E + Y * Z ) * ( F + X * Z ) * 2 - 
//      ( A + X * X ) * ( E + Y * Z ) * ( E + Y * Z ) - ( B + Y * Y ) * ( F + X * Z ) * ( F + X * Z ) - 
//      ( C + Z * Z ) * ( D + X * Y ) * ( D + X * Y )
//  или
//  Ф = A * B * C + 2 * D * E * F - A * E * E - B * F * F - C * D * D + 
//      X * X * ( B * C - E * E ) + Y * Y * ( A * C - F * F ) + Z * Z * ( A * B - D * D ) +
//  2*( X * Y * ( E * F - C * D ) + Y * Z * ( D * F - A * E ) + X * Z * ( D * E - B * F ) )
//
// Производные:
//
// pA = B * C - E * E + Y * Y * C + Z * Z * B - 2 * Y * Z * E
// pB = A * C - F * F + X * X * C + Z * Z * A - 2 * X * Z * F
// pC = A * B - D * D + X * X * B + Y * Y * A - 2 * X * Y * D
// pD = 2 * ( E * F - C * D + X * Z * E + Y * Z * F - Z * Z * D - X * Y * C )   
// pE = 2 * ( F * D - A * E + Y * X * F + Z * X * D - X * X * E - Y * Z * A )  
// pF = 2 * ( D * E - B * F + Z * Y * D + X * Y * E - Y * Y * F - Z * X * B )
// pX = 2 * ( X * ( B * C - E * E ) + Y * ( E * F - C * D ) + Z * ( D * E - B * F ) )
// pY = 2 * ( Y * ( C * A - F * F ) + Z * ( F * D - A * E ) + X * ( E * F - C * D ) )
// pZ = 2 * ( Z * ( A * B - D * D ) + X * ( D * E - B * F ) + Y * ( F * D - A * E ) )
//
// Линеаризация:
//
// pA = B * ( C0 + sz0 * sz0 ) + C * ( B0 + sy0 * sy0 ) - 2 * E * ( E0 + sy0 * sz0 )
// + 2 * Y * ( sy0 * C0 - sz0 * E0 )
// + 2 * Z * ( sz0 * B0 - sy0 * E0 )
// - ( B0 * C0 - E0 * E0 + 2 * ( sy0 * sy0 * C0 - 2 * sy0 * sz0 * E0 + sz0 * sz0 * B0 ) )
//
// pB = A * ( C0 + sz0 * sz0 ) + C * ( A0 + X0 * X0 ) - 2 * F * ( F0 + X0 * sz0 ) 
// + 2 * X * ( X0 * C0 - sz0 * F0 )
// + 2 * Z * ( sz0 * A0 - X0 * F0 )
// - ( A0 * C0 - F0 * F0 + 2 * ( X0 * X0 * C0 - 2 * X0 * sz0 * F0 + sz0 * sz0 * A0 ) )
//
// pC = A * ( B0 + sy0 * sy0 ) + B * ( A0 + X0 * X0 ) - 2 * D * ( D0 + X0 * sy0 )  
// + 2 * X * ( X0 * B0 - sy0 * D0 )
// + 2 * Y * ( sy0 * A0 - X0 * D0 )
// - ( A0 * B0 - D0 * D0 + 2 * ( X0 * X0 * B0 - 2 * X0 * sy0 * D0 + sy0 * sy0 * A0 ) )
//
// pD = 2 * ( E * ( F0 + X0 * sz0 ) - C * ( D0 + X0 * sy0 ) + F * ( E0 + sy0 * sz0 ) - D * ( C0 + sz0 * sz0 )
//  + X * ( sz0 * E0 - sy0 * C0 )
//  + Y * ( sz0 * F0 - X0 * C0 )
//  + Z * ( sy0 * F0 - 2 * sz0 * D0 + X0 * E0 )
//  - E0 * F0 + C0 * D0
//  - 2 * ( sy0 * sz0 * F0 + X0 * sz0 * E0 - sz0 * sz0 * D0 - X0 * sy0 * C0 ) )
//
// pE = 2 * ( F * ( D0 + X0 * sy0 ) - A * ( E0 + sy0 * sz0 ) + D * ( F0 + X0 * sz0 ) - E * ( A0 + X0 * X0 )
//  + Y * ( X0 * F0 - sz0 * A0 )
//  + Z * ( X0 * D0 - sy0 * A0 )
//  + X * ( sz0 * D0 - 2 * X0 * E0 + sy0 * F0 )
//  - F0 * D0 + A0 * E0
//  - 2 * ( sz0 * X0 * D0 + sy0 * X0 * F0 - X0 * X0 * E0 - sy0 * sz0 * A0 ) )
//
// pF = 2 * ( D * ( E0 + sy0 * sz0 ) - B * ( F0 + X0 * sz0 ) + E * ( D0 + X0 * sy0 ) - F * ( B0 + sy0 * sy0 )
//  + Z * ( sy0 * D0 - X0 * B0 )
//  + X * ( sy0 * E0 - sz0 * B0 )
//  + Y * ( X0 * E0 - 2 * sy0 * F0 + sz0 * D0 )
//  - D0 * E0 + B0 * F0
//  - 2 * ( X0 * sy0 * E0 + sz0 * sy0 * D0 - sy0 * sy0 * F0 - sz0 * X0 * B0 ) )
//
// pX = 2 * ( X * ( B0 * C0 - E0 * E0 ) + Y * ( E0 * F0 - C0 * D0 ) + Z * ( D0 * E0 - B0 * F0 )
//          + B * ( X0 * C0 - sz0 * F0 ) + C * ( X0 * B0 - sy0 * D0 )
//          + D * ( sz0 * E0 - sy0 * C0 ) + F * ( sy0 * E0 - sz0 * B0 )
//          + E * ( sy0 * F0 - 2 * X0 * E0 + sz0 * D0 )
//  - 2 * ( X0 * ( B0 * C0 - E0 * E0 ) + sy0 * ( E0 * F0 - C0 * D0 ) + sz0 * ( D0 * E0 - B0 * F0 ) ) )
//
// pY = 2 * ( Y * ( C0 * A0 - F0 * F0 ) + Z * ( F0 * D0 - A0 * E0 ) + X * ( E0 * F0 - C0 * D0 )
//          + C * ( sy0 * A0 - X0 * D0 ) + A * ( sy0 * A0 - sz0 * E0 )
//          + E * ( X0 * F0 - sz0 * A0 ) + D * ( sz0 * F0 - X0 * C0 )
//          + F * ( sz0 * D0 - 2 * sy0 * F0 + X0 * E0 )
//  - 2 * ( sy0 * ( C0 * A0 - F0 * F0 ) + sz0 * ( F0 * D0 - A0 * E0 ) + X0 * ( E0 * F0 - C0 * D0 ) ) )
//
// pZ = 2 * ( Z * ( A0 * B0 - D0 * D0 ) + X * ( D0 * E0 - B0 * F0 ) + Y * ( F0 * D0 - A0 * E0 )
//          + A * ( sz0 * B0 - sy0 * E0 ) + B * ( sz0 * B0 - X0 * F0 )
//          + F * ( sy0 * D0 - X0 * B0 ) + E * ( X0 * D0 - sy0 * A0 )
//          + D * ( X0 * E0 - 2 * sz0 * D0 + sy0 * F0 )
//  - 2 * ( sz0 * ( A0 * B0 - D0 * D0 ) + X0 * ( D0 * E0 - B0 * F0 ) + sy0 * ( F0 * D0 - A0 * E0 ) ) )
//
//*********************************************************************************************************************


inline double funcV ( double A, double B, double C, double D, double E, double F, double X, double Y, double Z )
{
    A += X * X;
    B += Y * Y;
    C += Z * Z;
    D += X * Y;
    E += Y * Z;
    F += Z * X;
    return A * B * C + 2 * D * E * F - A * E * E - B * F * F - C * D * D;
}

void trans ( CArrRef<double> lambda, CArrRef2<double> M, 
             double & A, double & B, double & C, double & D, double & E, 
             double & F, double & X, double & Y, double & Z )
{
    const nat m = lambda.size();
    const nat m9 = m + 9;
    A = M[0][m9];
    B = M[1][m9];
    C = M[2][m9];
    D = M[3][m9];
    E = M[4][m9];
    F = M[5][m9];
    X = M[6][m9];
    Y = M[7][m9];
    Z = M[8][m9];
    for ( nat i = 0; i < m; ++i )
    {
        const nat j = i + 9;
        A += M[0][j] * lambda[i];
        B += M[1][j] * lambda[i];
        C += M[2][j] * lambda[i];
        D += M[3][j] * lambda[i];
        E += M[4][j] * lambda[i];
        F += M[5][j] * lambda[i];
        X += M[6][j] * lambda[i];
        Y += M[7][j] * lambda[i];
        Z += M[8][j] * lambda[i];
    }
}

void trans ( CArrRef<double> lambda, CArrRef2<double> M, double * par )
{
    const nat m = lambda.size();
    const nat m9 = m + 9;
    for ( nat k = 0; k < 9; ++k )
    {
        const double * row = M[k](9);
        double & p = par[k];
        p = M[k][m9];
        for ( nat i = 0; i < m; ++i ) p += row[i] * lambda[i];
    }
}

template <nat N> bool task ( ArrRef<double> lambda, CArrRef2<double> M, CArrRef2<double> bound,
                             CArrRef<Plane3d> plane, List< Vertex<N> > & stor )
{
// Инициализация области допустимых преобразований
    WireModel<N> model;
    model.simplex ( 4*(N+1), stor );
    for ( nat i = 0; i < N; ++i )
    {
        model.cut ( * ( const Double<N+1> * ) bound[i](), stor );
    }
    Double<9> par;
    double max = 0;
    const Vertex<N> * r = 0;
    if ( model.vlist.top() )
    do
    {
        const Vertex<N> * p = model.vlist.cur();
        trans ( CArrRef<double> ( &p->coor.d0, N ), M, &par.d0 );
        const double c2 = par.d2 + par.d8 * par.d8;
        if ( c2 <= 0 ) 
            continue;
        const double c = sqrt ( c2 );
        const double e = ( par.d4 + par.d7 * par.d8 ) / c;
        const double f = ( par.d5 + par.d6 * par.d8 ) / c;
        const double b2 = par.d1 + par.d7 * par.d7 - e * e;
        if ( b2 <= 0 ) 
            continue;
        const double b = sqrt ( b2 );
        const double d = ( par.d3 + par.d6 * par.d7 - f * e ) / b;
        const double a2 = par.d0 + par.d6 * par.d6 - d * d - f * f;
        if ( a2 <= 0 ) 
            continue;
        const double t = a2 * b2 * c2;
        if ( max < t ) max = t, r = p;
    }
    while ( model.vlist.next() );
    model.vlist.movAllAftLas ( stor );
    if ( ! r )
        return false;
    * ( Double<N> * ) lambda() = r->coor;
    return true;
}

struct CutStorNd
{
    List< Vertex<5> > vert5;
    List< Vertex<6> > vert6;
    List< Vertex<7> > vert7;
    List< Vertex<8> > vert8;
    List< Vertex<9> > vert9;
    List< Vertex<10> > vert10;
};

bool maxEllipsoidInConvexPolyhedronV ( CArrRef<Plane3d> plane, ArrRef<bool> act, CutStorNd & stor, Affin3d & aff )
{
    nat i;
    const nat m = act.size();
    const Vector3d & vx = aff.t.x;
    const Vector3d & vy = aff.t.y;
    const Vector3d & vz = aff.t.z;
    const Vector3d &  s = aff.s;
    double X = s.x;
    double Y = s.y;
    double Z = s.z;
    double A = vx.x * vx.x + vx.y * vx.y + vx.z * vx.z - X * X;
    double B = vy.y * vy.y + vy.z * vy.z - Y * Y;
    double C = vz.z * vz.z - Z * Z;
    double D = vx.y * vy.y + vx.z * vy.z - X * Y;
    double E = vy.z * vz.z - Y * Z;
    double F = vx.z * vz.z - X * Z;
    DynArray2<double> bound ( m, m+1 );
    CmbArray<double, 9> lambda ( m );
    const nat m9 = m + 9;
    DynArray2<double> M ( 9, m+10 );
    for ( nat step = 0; step < 9; ++step )
    {
        M.fill(0);
        M[0][1] = M[1][0] = C + Z * Z;
        M[0][2] = M[2][0] = B + Y * Y;
        M[0][4] = M[4][0] = -2 * ( E + Y * Z );
        M[0][7] = M[7][0] = 2 * ( Y * C - Z * E );
        M[0][8] = M[8][0] = 2 * ( Z * B - Y * E );
        M[0][m9] = B * C - E * E + 2 * ( Y * Y * C - 2 * Y * Z * E  + Z * Z * B );
        M[1][2] = M[2][1] = A + X * X;
        M[1][5] = M[5][1] = -2 * ( F + X * Z );
        M[1][6] = M[6][1] = 2 * ( X * C - Z * F );
        M[1][8] = M[8][1] = 2 * ( Z * A - X * F );
        M[1][m9] = A * C - F * F + 2 * ( X * X * C - 2 * X * Z * F  + Z * Z * A );
        M[2][3] = M[3][2] = -2 * ( D + X * Y );
        M[2][6] = M[6][2] = 2 * ( X * B - Y * D );
        M[2][7] = M[7][2] = 2 * ( Y * A - X * D );
        M[2][m9] = A * B - D * D + 2 * ( X * X * B - 2 * X * Y * D  + Y * Y * A );
        M[3][3] = -2 * ( C + Z * Z );
        M[3][4] = M[4][3] = 2 * ( F + X * Z );
        M[3][5] = M[5][3] = 2 * ( E + Y * Z );
        M[3][6] = M[6][3] = 2 * ( Z * E - Y * C );
        M[3][7] = M[7][3] = 2 * ( Z * F - X * C );
        M[3][8] = M[8][3] = 2 * ( Y * F - 2 * Z * D + X * E );
        M[3][m9] = 2 * ( E * F - C * D + 2 * ( Y * Z * F + X * Z * E - Z * Z * D - X * Y * C ) );
        M[4][4] = -2 * ( A + X * X );
        M[4][5] = M[5][4] = 2 * ( D + X * Y );
        M[4][6] = M[6][4] = 2 * ( Z * D - 2 * X * E + Y * F );
        M[4][7] = M[7][4] = 2 * ( X * F - Z * A );
        M[4][8] = M[8][4] = 2 * ( X * D - Y * A );
        M[4][m9] = 2 * ( D * F - A * E + 2 * ( Z * X * D + Y * X * F - X * X * E - Y * Z * A ) );
        M[5][5] = -2 * ( B + Y * Y );
        M[5][6] = M[6][5] = 2 * ( Y * E - Z * B );
        M[5][7] = M[7][5] = 2 * ( X * E - 2 * Y * F + Z * D );
        M[5][8] = M[8][5] = 2 * ( Y * D - X * B );
        M[5][m9] =2 * ( D * E - B * F + 2 * ( X * Y * E + Z * Y * D - Y * Y * F - Z * X * B ) );
        M[6][6] = 2 * ( B * C - E * E );
        M[6][7] = M[7][6] = 2 * ( E * F - C * D );
        M[6][8] = M[8][6] = 2 * ( D * E - B * F );
        M[6][m9] = 4 * ( X * ( B * C - E * E ) + Y * ( E * F - C * D ) + Z * ( D * E - B * F ) );
        M[7][7] = 2 * ( C * A - F * F );
        M[7][8] = M[8][7] = 2 * ( F * D - A * E );
        M[7][m9] = 4 * ( Y * ( C * A - F * F ) + Z * ( F * D - A * E ) + X * ( E * F - C * D ) );
        M[8][8] = 2 * ( A * B - D * D );
        M[8][m9] = 4 * ( Z * ( A * B - D * D ) + X * ( D * E - B * F ) + Y * ( F * D - A * E ) );
        for ( i = 0; i < m; ++i )
        {
            const Plane3d & p = plane[i];
            const Vector3d & norm = p.norm;
            const double & g = p.dist;
            const nat j = i + 9;
            M[0][j] = norm.x * norm.x;
            M[1][j] = norm.y * norm.y;
            M[2][j] = norm.z * norm.z;
            M[3][j] = norm.x * norm.y * 2;
            M[4][j] = norm.y * norm.z * 2;
            M[5][j] = norm.z * norm.x * 2;
            M[6][j] = norm.x * g * -2;
            M[7][j] = norm.y * g * -2;
            M[8][j] = norm.z * g * -2;
        }
        if ( ! slu_gauss ( M ) )
            return false;
        for ( i = 0; i < m; ++i )
        {
            const Plane3d & p = plane[i];
            const Vector3d & norm = p.norm;
            const double & dist = p.dist;
            const double xx = norm.x * norm.x;
            const double yy = norm.y * norm.y;
            const double zz = norm.z * norm.z;
            const double xy = norm.x * norm.y * 2;
            const double yz = norm.y * norm.z * 2;
            const double zx = norm.z * norm.x * 2;
            const double xg = norm.x * dist * -2;
            const double yg = norm.y * dist * -2;
            const double zg = norm.z * dist * -2;
            ArrRef<double> row = bound[i];
            for ( nat k = 0; k <= m; ++k )
            {
                const nat j = k + 9;
                row[k] = M[0][j] * xx + M[1][j] * yy + M[2][j] * zz +
                         M[3][j] * xy + M[4][j] * yz + M[5][j] * zx +
                         M[6][j] * xg + M[7][j] * yg + M[8][j] * zg;
            }
            row[m] -= dist * dist;
        }
        // Находим значения множителей Лагранжа
        switch ( m )
        {
        case 5:
            if ( ! task ( lambda, M, bound, plane, stor.vert5 ) ) return false;
            break;
        case 6:
            if ( ! task ( lambda, M, bound, plane, stor.vert6 ) ) return false;
            break;
        case 7:
            if ( ! task ( lambda, M, bound, plane, stor.vert7 ) ) return false;
            break;
        case 8:
            if ( ! task ( lambda, M, bound, plane, stor.vert8 ) ) return false;
            break;
        case 9:
            if ( ! task ( lambda, M, bound, plane, stor.vert9 ) ) return false;
            break;
        case 10:
            if ( ! task ( lambda, M, bound, plane, stor.vert10 ) ) return false;
            break;
        default:
            return false;
        }
        double A2, B2, C2, D2, E2, F2, X2, Y2, Z2;
        trans ( lambda, M, A2, B2, C2, D2, E2, F2, X2, Y2, Z2 );
        const double dif = ( fabs(A - A2) + fabs(B - B2) + fabs(C - C2) + fabs(D - D2) + fabs(E - E2) + fabs(F - F2) ) / 
                           ( fabs(A) + fabs(B) + fabs(C) + fabs(D) + fabs(E) + fabs(F) );
        A = A2;
        B = B2;
        C = C2;
        D = D2;
        E = E2;
        F = F2;
        X = X2;
        Y = Y2;
        Z = Z2;
        if ( dif < 1e-6 )
        {
            double c = C + Z * Z;
            if ( c <= 0 ) 
                return false;
            c = sqrt ( c );
            const double e = ( E + Y * Z ) / c;
            const double f = ( F + X * Z ) / c;
            double b = B + Y * Y - e * e;
            if ( b <= 0 ) 
                return false;
            b = sqrt ( b );
            const double d = ( D + X * Y - f * e ) / b;
            double a = A + X * X - d * d - f * f;
            if ( a <= 0 ) 
                return false;
            a = sqrt ( a );
            aff.t.x = Vector3d ( a, d, f );
            aff.t.y = Vector3d ( 0, b, e );
            aff.t.z = Vector3d ( 0, 0, c );
            aff.s   = Vector3d ( X, Y, Z );
            for ( i = 0; i < m; ++i ) act[i] = lambda[i] != 0;
            return true;
        }
    }
    return false;
}

inline double distance ( const Affin3d & b, const Plane3d & p )
{
    return ( p % b.s ) / sqrt ( _pow2 ( p.norm.x * b.t.x.x ) +
                                _pow2 ( p.norm.x * b.t.x.y + p.norm.y * b.t.y.y ) +
                                _pow2 ( p.norm.x * b.t.x.z + p.norm.y * b.t.y.z + p.norm.z * b.t.z.z ) );
}

Def<Ellipsoid3d> maxEllipsoidInConvexPolyhedronV ( const Polyhedron & poly )
{
    Def<Ellipsoid3d> res;
    const nat n = poly.facet.size();
    switch ( n )
    {
    case 0:
    case 1:
    case 2:
    case 3:
        return res;
    case 4:
        return maxEllipsoidInTetrahedronV ( poly.vertex[0], poly.vertex[1], poly.vertex[2], poly.vertex[3] );
    }
    const Affin3d stand = getEllipsoidPlg ( poly ).getAffin3d();
    Polyhedron poly2;
    poly2 = poly;
    poly2 *= ~stand;
    nat i;
// Находим первоначальное приближение
    DynArray<Plane3d> plane ( n + 6 );
    for ( i = 0; i < n; ++i ) plane[i] = poly2.facet[i].plane;
    Cuboid3d para = minCuboidAroundPointsNR ( poly2.vertex );
    const Polyhedron poly0 ( para );
    CmbSuite<nat, 10> support1, support2;
    support1.resize(6);
    for ( i = 0; i < 6; ++i )
    {
        const nat j = i + n;
        plane[j] = poly0.facet[i].plane;
        support1[i] = j;
    }
    Affin3d aff = para.getAffin3d();
// Применяем ограничения
    CutStor3d stor3d;
    CutStorNd storNd;
    FixArray<Vector3d, 8> vert;
    const Segment3d dim = dimensions ( poly0.vertex );
    for ( nat k = 0; k < 29; ++k )
    {
        // Ищем наиболее близкую плоскость
        nat im;
        double max = -1;
        for ( i = 0; i < plane.size(); ++i )
        {
            const double t = distance ( aff, plane[i] );
            if ( max < t ) max = t, im = i;
        }
        if ( max < -1 + 1e-9 || hasEqu ( support1, im )  )
        {
            return ( stand * aff ) ( Sphere3d ( 1, null3d ) );
        }
        // Добавляем новую плоскость в ограничения
        support1.inc() = im;
        const nat m = support1.size();
        CmbArray<bool, 10> act ( m );
        CmbArray<Plane3d, 10> plane1 ( m );
        for ( i = 0; i < m; ++i )
        {
            plane1[i] = plane[support1[i]];
            act[i] = true;
        }
        // Вычислим начальное приближение ( taff )
        Def<Sphere3d> sphere = maxSphereInConvexPolyhedron ( dim, plane1 );
        if ( ! sphere.isDef ) return res;
        Affin3d taff ( Conform3d ( sphere.o, sphere.r ) );
        // Поиск оптимального преобразования
        if ( ! maxEllipsoidInConvexPolyhedronV ( plane1, act, storNd, taff ) )
        {
            Polyhedron p1, p2;
            p1 = poly0;
            for ( i = 0; i < m; ++i )
            {
                if ( support1[i] >= n ) continue;
                cut ( p1, plane1[i], p2, stor3d );
                _swap ( p1, p2 );
            }
            const Affin3d stand2 = getEllipsoidPlg ( p1 ).getAffin3d();
            const Affin3d back = ~stand2;
            plane1 *= back;
            for ( i = 0; i < vert.size(); ++i ) vert[i] = back ( poly0.vertex[i] );
            Def<Sphere3d> sphere = maxSphereInConvexPolyhedron ( dimensions ( vert ), plane1 );
            if ( ! sphere.isDef ) return res;
            Affin3d taff ( Conform3d ( sphere.o, sphere.r ) );
            if ( ! maxEllipsoidInConvexPolyhedronV ( plane1, act, storNd, taff ) )
                return res;
            aff = stand2 * taff;
        }
        else
            aff = taff;
        // Удаляем лишние ограничения
        support2.resize();
        for ( i = 0; i < m; ++i )
        {
            if ( act[i] ) support2.inc() = support1[i];
        }
        support1 = support2;
    }
    return res;
}

//**************************** 05.02.2023 *********************************//
//
//          Максимальный тетраэдр вписанный в выпуклый многогранник
//
//**************************** 05.02.2023 *********************************//

Def<Tetrahedron> maxTetrahedronInConvexPolyhedronV ( const Polyhedron & poly )
{
    Def<Tetrahedron> res;
    CCArrRef<Vector3d> & vert = poly.vertex;
    if ( vert.size() < 4 ) return res;
    nat m0(0), m1(1), m2(2), m3(3);
    double max = 0, vol;
    for ( nat i0 =    0; i0 < vert.size() - 3; ++i0 )
    for ( nat i1 = i0+1; i1 < vert.size() - 2; ++i1 )
    {
        const Vector3d v1 = vert[i1] - vert[i0];
        for ( nat i2 = i1+1; i2 < vert.size() - 1; ++i2 )
        {
            const Vector3d v2 = v1 % ( vert[i2] - vert[i0] );
            for ( nat i3 = i2+1; i3 < vert.size(); ++i3 )
            {
                const double d = v2 * ( vert[i0] - vert[i3] );
                if ( _maxa ( max, fabs ( d ) ) )
                {
                    vol = d;
                    m0 = i0;
                    m1 = i1;
                    m2 = i2;
                    m3 = i3;
                }
            }
        }
    }
    if ( vol < 0 ) _swap ( m0, m1 );
    res.a = vert[m0];
    res.b = vert[m1];
    res.c = vert[m2];
    res.d = vert[m3];
    res.isDef = true;
    return res;
}

//**************************** 24.10.2021 *********************************//
//
//      Максимальный по объёму прямоугольный параллелепипед
//        вписанный в выпуклый многогранник без вращения
//
//**************************** 24.10.2021 *********************************//

void divide4 ( const Triangle3d & trian1, Triangle3d trian2[4] )
{
    trian2[0].a = 0.5 * ( trian1.a + trian1.b );
    trian2[0].b = 0.5 * ( trian1.b + trian1.c );
    trian2[0].c = 0.5 * ( trian1.c + trian1.a );
    trian2[1].a = trian1.a;
    trian2[1].b = trian2[0].a;
    trian2[1].c = trian2[0].c;
    trian2[2].a = trian1.b;
    trian2[2].b = trian2[0].b;
    trian2[2].c = trian2[0].a;
    trian2[3].a = trian1.c;
    trian2[3].b = trian2[0].c;
    trian2[3].c = trian2[0].b;
}

struct MaxCuboid
{
    Double<6> func, con[7];
    FixArray<Double<7>, 3> cor;
    void init ( const Segment3d & seg, const Triangle3d & trian )
    {
        Double<6> & vn = con[0];
        vn.d0 = seg.b.x - seg.a.x;
        vn.d1 = seg.b.y - seg.a.y;
        vn.d2 = seg.b.z - seg.a.z;
        vn.d3 = seg.b.x;
        vn.d4 = seg.b.y;
        vn.d5 = seg.b.z;
        for ( nat l = 1; l <= 6; ++l )
        {
            con[l].fill(0);
            (&con[l].d0)[l-1] = -1;
        }
        {
            Triangle3d t = trian;
            t.a /= root3 ( t.a.x * t.a.y * t.a.z );
            t.b /= root3 ( t.b.x * t.b.y * t.b.z );
            t.c /= root3 ( t.c.x * t.c.y * t.c.z );
            const Vector3d u = t.getNormal();
            const double c = 1 / ( u * t.a );
            func.d0 = c * u.x;
            func.d1 = c * u.y;
            func.d2 = c * u.z;
            func.d3 = func.d4 = func.d5 = 0;
        }
        {
            const Vector3d v = ( trian.b % trian.a ).setNorm2 ( 1e6 );
            Double<7> & dn = cor[0];
            dn.d0 = v.x;
            dn.d1 = v.y;
            dn.d2 = v.z;
            dn.d3 = dn.d4 = dn.d5 = dn.d6 = 0;
        }
        {
            const Vector3d v = ( trian.c % trian.b ).setNorm2 ( 1e6 );
            Double<7> & dn = cor[1];
            dn.d0 = v.x;
            dn.d1 = v.y;
            dn.d2 = v.z;
            dn.d3 = dn.d4 = dn.d5 = dn.d6 = 0;
        }
        {
            const Vector3d v = ( trian.a % trian.c ).setNorm2 ( 1e6 );
            Double<7> & dn = cor[2];
            dn.d0 = v.x;
            dn.d1 = v.y;
            dn.d2 = v.z;
            dn.d3 = dn.d4 = dn.d5 = dn.d6 = 0;
        }
    }
};

Def<Cuboid3d> maxCuboidInConvexPolyhedronNR ( const Polyhedron & poly, const Spin3d & spin )
{
    Def<Cuboid3d> res;
    const nat nf = poly.facet.size();
    const nat nv = poly.vertex.size();
    if ( nf < 4 || nv < 4 ) return res;
    Vector3d ax, ay, az;
    (~spin).getReper ( ax, ay, az );
    DynArray<Double<7> > cor ( nf );
    for ( nat l = 0; l < nf; ++l )
    {
        const Plane3d & p = poly.facet[l].plane;
        Double<7> & c = cor[l];
        c.d0 = fabs ( p.norm * ax );
        c.d1 = fabs ( p.norm * ay );
        c.d2 = fabs ( p.norm * az );
        c.d3 = p.norm.x;
        c.d4 = p.norm.y;
        c.d5 = p.norm.z;
        c.d6 = p.dist;
    }
    Suite<Triangle3d> trian1, trian2;
    Triangle3d & t = trian1.inc();
    t.a = Vector3d ( 0.998, 1e-3, 1e-3 );
    t.b = Vector3d ( 1e-3, 0.998, 1e-3 );
    t.c = Vector3d ( 1e-3, 1e-3, 0.998 );
    for ( nat i = 0; i < 5; ++i )
    {
        trian2.resize ( 4*trian1.size() );
        for ( nat j = 0; j < trian1.size(); ++j ) divide4 ( trian1[j], trian2(4*j) );
        trian1.swap ( trian2 );
    }
    const nat nn = trian1.size();
    DynArray<MaxCuboid> variant ( nn );
    MaxHeap<SortItem<double,nat> > heap ( nn );
    Def<Segment3d> seg = dimensions ( poly.vertex );
    for ( nat k = 0; k < nn; ++k )
    {
        MaxCuboid & var = variant[k];
        var.init ( seg, trian1[k] );
        heap << SortItem<double,nat> ( var.func * var.con[0], k );
    }
    const double eps = 1e-9;
    for ( nat j = 0; j < nn*nf; ++j )
    {
        MaxCuboid & var = variant[heap[0]->tail];
        Double<6> & r = var.con[0];
        double max = var.cor[0] % r;
        nat i, im = 0;
        Double<6> norm;
        for ( i = 1; i < 3; ++i )
        {
            const double t = var.cor[i] % r;
            if ( max < t ) max = t, im = i;
        }
        if ( max <= eps )
        {
            for ( i = 0; i < nf; ++i )
            {
                const double t = cor[i] % r;
                if ( max < t ) max = t, im = i;
            }
            if ( max <= eps )
            {
                res.a = r.d0;
                res.b = r.d1;
                res.c = r.d2;
                res.o.x = r.d3;
                res.o.y = r.d4;
                res.o.z = r.d5;
                res.spin = spin;
                res.isDef = true;
                return res;
            }
            norm = ( const Double<6> & ) cor[im];
        }
        else
            norm = ( const Double<6> & ) var.cor[im];
        const double dist = max;
        const double lvl = -1e-8 * sqrt ( norm * norm );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i <= 6; ++i )
        {
            const Double<6> & v = var.con[i];
            double t = norm * v;
            if ( t > lvl ) continue;
            t = -1./ t;
            if ( !ib )
            {
                max = ( var.func * v ) * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double s = ( var.func * v ) * t;
                if ( max < s ) max = s, ib = i, sm = t;
            }
        }
        if ( !ib )
        {
            return res;
        }
        const Double<6> & v = var.con[ib];
        r += v * ( dist * sm );
        for ( i = 1; i <= 6; ++i )
        {
            if ( i == ib ) continue;
            Double<6> & ai = var.con[i];
            ai += v * ( ( norm * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
        heap[0]->head = var.func * r;
        heap.down ( 0 );
    }
    return res;
}

//**************************** 03.05.2014 *********************************//
//
// Максимальный многогранник вписанный в выпуклый многогранник без вращения
//
//**************************** 03.05.2014 *********************************//

Def<Conform3d> maxPolyhedronInConvexPolyhedronNR ( const Vector4d & v0, double eps, CArrRef<Plane4d> plane ) 
{
    Def<Conform3d> res;
    Vector4d arr[5];
    arr[0] = v0;
    arr[1] = Vector4d ( -1., 0., 0., 0. );
    arr[2] = Vector4d ( 0., -1., 0., 0. );
    arr[3] = Vector4d ( 0., 0., -1., 0. );
    arr[4] = Vector4d ( 0., 0., 0., -1. );
    const nat n = plane.size();
    const nat n2 = n + n;
    for ( nat k = 0; k < n2; ++k )
    {
        Vector4d & a0 = arr[0];
        double dist = plane[0] % a0;
        nat i, im = 0;
        for ( i = 1; i < n; ++i )
        {
            const double t = plane[i] % a0;
            if ( dist < t ) dist = t, im = i;
        }
        if ( dist <= eps )
        {
            res.trans.x = a0.x1;
            res.trans.y = a0.x2;
            res.trans.z = a0.x3;
            res.magn = a0.x4;
            res.isDef = true;
            return res;
        }
        const Vector4d & norm = plane[im].norm;
        nat ib = 0;
        double sg, max;
        for ( i = 1; i <= 4; ++i )
        {
            const Vector4d & v = arr[i];
            double t = norm * v;
            if ( t > -1e-8 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.x4 * t;
                ib = i;
                sg = t;
            }
            else
            {
                const double s = v.x4 * t;
                if ( s < max ) max = s, ib = i, sg = t;
            }
        }
        if ( ib == 0 )
        {
            return res;
        }
        const Vector4d & v = arr[ib];
        a0 -= v * ( dist * sg );
        for ( i = 1; i <= 4; ++i )
        {
            if ( i == ib ) continue;
            Vector4d & ai = arr[i];
            ai -= v * ( ( norm * ai ) * sg );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return res;
}

Def<Conform3d> maxPolyhedronInConvexPolyhedronNR ( const Polyhedron & inner, const Polyhedron & outer )
{
    if ( inner.facet.size() < 4 || inner.vertex.size() < 4 ||
         outer.facet.size() < 4 || outer.vertex.size() < 4 ) return Def<Conform3d>();
    const Segment3d dim1 = dimensions ( inner.vertex );
    const Segment3d dim2 = dimensions ( outer.vertex );
    const double d1 = normU ( dim1 );
    const double d2 = normU ( dim2 );
    if ( ! d1 || ! d2 ) return Def<Conform3d>();
    DynArray<Plane4d> plane ( outer.facet.size() );
    for ( nat i = 0; i < plane.size(); ++i )
    {
        const Plane3d & p = outer.facet[i].plane;
        double max = p.norm * inner.vertex[0];
        for ( nat j = 1; j < inner.vertex.size(); ++j )
        {
            double t = p.norm * inner.vertex[j];
            if ( max < t ) max = t;
        }
        plane[i].norm = Vector4d ( p.norm.x, p.norm.y, p.norm.z, max );
        plane[i].dist = p.dist;
    }
    const Vector4d v0 ( dim2.b.x, dim2.b.y, dim2.b.z, d2 / d1 );
    return maxPolyhedronInConvexPolyhedronNR ( v0, 1e-9 * d2, plane );
}

//**************************** 30.03.2015 *********************************//
//
//     Максимальный многогранник вписанный в выпуклый многогранник 
//               с вращением вокруг заданной оси
//
//**************************** 24.02.2023 *********************************//

Def<Conform3d> maxPolyhedronInConvexPolyhedron1R ( const Polyhedron & inner, 
                                                   const Vector3d & az, nat sym,
                                                   const Polyhedron & outer )
{
    nat i;
    Def<Conform3d> err;
    const nat nv = inner.vertex.size();
    if ( nv < 2 ) return err;
    const nat np = outer.facet.size();
    if ( nv < 4 ) return err;
    DynArray<Vector3d> vert ( nv );
    DynArray<Plane3d> plane ( np );
    Def<Segment3d> dim1 = dimensions ( inner.vertex );
    Def<Segment3d> dim2 = dimensions ( outer.vertex );
    if ( ! dim2.isDef ) return err;
    const double r1 = normU ( dim1.a - dim1.b );
    const double r2 = normU ( dim2.a - dim2.b );
    if ( ! r1 || ! r2 ) return err;
    const Vector3d o1 = 0.5 * ( dim1.a + dim1.b );
    const Vector3d o2 = 0.5 * ( dim2.a + dim2.b );
    const double c1 = 4 / r1;
    for ( i = 0; i < nv; ++i )
    {
        vert[i] = ( inner.vertex[i] - o1 ) * c1;
    }
    const Conform3d conf1 = Conform3d ( c1 ) * Conform3d ( -o1 );
    const Conform3d conf2 = Conform3d ( 2 / r2 ) * Conform3d ( -o2 );
    const Similar3d sim2 ( conf2 );
    for ( i = 0; i < np; ++i )
    {
        plane[i] = sim2 ( outer.facet[i].plane );
    }
// Инициализация области допустимых преобразований
    List< Vertex<6> > stor;
    WireModel<6> model;
    model.simplex ( 10., stor );
    Double<6> dd;
    dd.d1 = dd.d2 = dd.d3 = dd.d4 = 1.;
    dd.d5 = 0;
    dd.d0 = sym > 1 ? 0. : dd.d1;
    model.vlist -= dd;
    Double<7> cor;
    if ( sym > 2 )
    {
        const double fi = ( M_PI + M_PI ) / sym;
        cor.d0 = - sin ( fi );
        cor.d1 =   cos ( fi );
        cor.d2 = cor.d3 = cor.d4 = cor.d5 = cor.d6 = 0;
        model.cut ( cor, stor );
    }
    const double eps = 1e-6;
    Vector3d ax, ay;
    reper ( az, ax, ay );
    for ( i = 0; i < 200; ++i )
    {
// Поиск максимального решения
        Double<6> best;
        best.d5 = 0.;
        if ( model.vlist.top() )
        do
        {
            Vertex<6> * v = model.vlist.cur();
            const Double<6> & d = v->coor;
            v->data = d.d0 * d.d0 + d.d1 * d.d1 - d.d5 * d.d5;
        }
        while ( model.vlist.next() );
        if ( model.vlist.top() )
        do
        {
            const Vertex<6> * vi = model.vlist.cur();
            const Double<6> & di = vi->coor;
            if ( vi->data == 0 )
            {
                if ( _maxa ( best.d5, di.d5 ) ) best = di;
            }
            else
            for ( nat j = 0; j < 6; ++j )
            {
                const Vertex<6> * vj = vi->vertex[j];
                if ( vi > vj || vi->data * vj->data >= 0 ) continue;
                const Double<6> & dj = vj->coor;
                const double b0 = dj.d0 - di.d0;
                const double b1 = dj.d1 - di.d1;
                const double b5 = dj.d5 - di.d5;
                double x[2];
                const nat n = root2 ( b0*b0 + b1*b1 - b5*b5, 2*(di.d0*b0 + di.d1*b1 - di.d5*b5), di.d0*di.d0 + di.d1*di.d1 - di.d5*di.d5, x );
                if ( ! n ) continue;
                double t = x[0];
                if ( n == 2 && fabs ( x[1] - 0.5 ) < fabs ( t - 0.5 ) ) t = x[1];
                if ( _maxa ( best.d5, di.d5 + b5*t ) )
                {
                    best.d0 = di.d0 + b0*t;
                    best.d1 = di.d1 + b1*t;
                    best.d2 = di.d2 + ( dj.d2 - di.d2 ) * t;
                    best.d3 = di.d3 + ( dj.d3 - di.d3 ) * t;
                    best.d4 = di.d4 + ( dj.d4 - di.d4 ) * t;
                }
            }
        }
        while ( model.vlist.next() );
        if ( ! best.d5 ) return err;
// Поиск максимального нарушения ограничений для выбранного решения
        const double s = best.d0;
        const double t = best.d1;
        const Vector3d o ( best.d2, best.d3, best.d4 );
        Vector3d vx, vy, vz;
        vx = s * ax + t * ay;        // p1 = ( s*p.x - t*p.y ) * ax +
        vy = s * ay - t * ax;        //      ( t*p.x + s*p.y ) * ay +
        vz = best.d5 * az;           //        h*p.z * az + o;
        double max = 0;
        nat jm, km;
        for ( nat j = 0; j < nv; ++j )
        {
            Vector3d p = o;
            p += vert[j].x * vx;
            p += vert[j].y * vy;
            p += vert[j].z * vz;
            for ( nat k = 0; k < np; ++k )
            {
                const double d = plane[k] % p;
                if ( max < d ) max = d, jm = j, km = k;
            }
        }
// Если нарушение мало, то завершение программы
        if ( max < eps )
        {
//display << i << NL;
            return ~conf2 * Conform3d ( Spin3d ( vx/best.d5, vy/best.d5, az ), o, best.d5 ) * conf1;
        }
// Применение ограничения к области допустимых преобразований
        const Vector3d & norm = plane[km].norm;
        const double xn = ax * norm;
        const double yn = ay * norm;
        const double zn = az * norm;
        const Vector3d & v = vert[jm];
        cor.d0 = xn * v.x + yn * v.y;
        cor.d1 = yn * v.x - xn * v.y;
        cor.d2 = norm.x;
        cor.d3 = norm.y;
        cor.d4 = norm.z;
        cor.d5 = zn * v.z;
        cor.d6 = plane[km].dist;
        model.cut ( cor, stor );
    }
    return err;
}

//**************************** 31.07.2019 *********************************//
//
//     Максимальный многогранник вписанный в выпуклый многогранник 
//
//**************************** 31.07.2019 *********************************//

Conform3d maxPointsInConvexPolyhedron ( CCArrRef<Vector3d> & inner, const Polyhedron & outer )
{
    Conform3d best ( 0 );
    const nat nv = inner.size();
    if ( nv < 2 ) return best;
    const nat np = outer.facet.size();
    if ( nv < 4 ) return best;
    Def<Segment3d> dim = dimensions ( outer.vertex );
    if ( ! dim.isDef ) return best;
    const double d1 = minSphereAroundPoints ( inner ).r;
    const double d2 = normU ( dim );
    if ( ! d1 || ! d2 ) return best;
    // Глобальный поиск оптимального поворота
    const Vector4d v0 ( dim.b.x, dim.b.y, dim.b.z, d2 / d1 );
    QRand q5(5);
    double b = 0;
    DynArray<Plane4d> plane ( np );
    DynArray<Vector3d> vert ( nv );
    const nat n = 2862;
    nat k;
    for ( k = 0; k < n; ++k )
    {
        const double a = ( 0.5 + k ) / n;
        if ( ( b += 0.61803398874989484820 ) > 1 ) b -= 1;
        const double c = q5();
        const Ortho3d ortho = getRandOrtho3d ( a, b, c );
        for ( nat j = 0; j < nv; ++j ) vert[j] = ortho ( inner[j] );
        for ( nat i = 0; i < np; ++i )
        {
            const Plane3d & p = outer.facet[i].plane;
            double max = p.norm * vert[0];
            for ( nat j = 1; j < nv; ++j )
            {
                double t = p.norm * vert[j];
                if ( max < t ) max = t;
            }
            plane[i].norm = Vector4d ( p.norm.x, p.norm.y, p.norm.z, max );
            plane[i].dist = p.dist;
        }
        Def<Conform3d> conf = maxPolyhedronInConvexPolyhedronNR ( v0, 1e-9 * d2, plane );
        if ( conf.isDef && best.magn < conf.magn )
        {
            best.magn = conf.magn;
            best.spin = getRandSpin3d ( a, b, c );
            best.trans = conf.trans;
        }
    }
    // Локальный поиск оптимального поворота
    QRand q2(2);
    QRand2Vector3d qv;
    const double step = 4 * M_PI / 180;
    for ( k = 0; k < 2000; ++k )
    {
        const double c = q2();
        const Spin3d spin2 ( qv(), ( c + 0.25 ) * step );
        const Spin3d spin ( spin2 * best.spin );
        const Ortho3d ortho ( spin );
        for ( nat j = 0; j < nv; ++j ) vert[j] = ortho ( inner[j] );
        for ( nat i = 0; i < np; ++i )
        {
            const Plane3d & p = outer.facet[i].plane;
            double max = p.norm * vert[0];
            for ( nat j = 1; j < nv; ++j )
            {
                double t = p.norm * vert[j];
                if ( max < t ) max = t;
            }
            plane[i].norm = Vector4d ( p.norm.x, p.norm.y, p.norm.z, max );
            plane[i].dist = p.dist;
        }
        Def<Conform3d> conf = maxPolyhedronInConvexPolyhedronNR ( v0, 1e-9 * d2, plane );
        if ( conf.isDef && best.magn < conf.magn )
        {
            best.magn = conf.magn;
            best.spin = spin;
            best.trans = conf.trans;
        }
    }
    return best;
}

// 4 0.50

// 5 2000 0.98714408934317
// 6 2000 0.98599014840494
// 6 4000 0.98637045687301
// 7 2000 0.99098404450523

// 3 0.50

// 5 1000 0.99517802869498
// 6 1000 0.99301289526858
// 7 1000 0.99299717731266

// 4 0.25

// 5 1000 0.99370074484359
// 6 1000 0.99504127159171
// 7 2000 0.99820092665806

Def<Conform3d> maxPolyhedronInConvexPolyhedron ( const Polyhedron & inner, const Polyhedron & outer )
{
    return maxPointsInConvexPolyhedron ( inner.vertex, outer );
}