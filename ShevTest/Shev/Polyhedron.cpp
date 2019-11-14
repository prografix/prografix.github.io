
#include "math.h"
#include "tune.h"
#include "ShevArray.h"
#include "Polyhedron.h"
#include "Vector2d.h"
#include "trian2d.h"
#include "func1t.h"
#include "func2d.h"
#include "func3d.h"
#include "trans.h"
#include "rand.h"
#include "intersect2d.h"

namespace Shev
{

///////////////////// Facet ///////////////////////

Facet & Facet::operator = ( const Facet & facet )
{
    if ( this == &facet ) return *this;
    nv = facet.nv;
    info = facet.info;
    plane = facet.plane;
    index = facet.index;
    return *this;
}

Facet & Facet::initPlane ( CArrRef<Vector3d> vertex )
{
    if ( nv > 2 )
    {
        nat j;
        double sum = 0.;
        Vector3d s ( 0., 0., 0. );
        for ( j = 0; j < nv; ++j )
        {
            const nat k = index[j];
            sum += qmod ( vertex[k] - vertex[index[j+1]] );
            s += vertex[k];
        }
        Vector3d v ( 0., 0., 0. );
        const Vector3d & o = vertex[index[0]];
        for ( j = 2; j < nv; ++j )
        {
            v += ( vertex[index[j-1]] - o ) % ( vertex[index[j]] - o );
        }
        if ( v * v > 1e-22 * sum * sum )
        {
            plane.norm = v.setNorm2();
            plane.dist = - ( v * s ) / nv;
            return *this;
        }
    }
    plane.norm.fill();
    plane.dist = 0.;
    return *this;
}

//**************************** 24.11.2012 *********************************//
//
//                           Площадь грани
//
//**************************** 12.06.2017 *********************************//

double Facet::getArea ( CArrRef<Vector3d> vertex ) const
{
    if ( nv < 3 ) return 0.;
    Vector3d v ( 0., 0., 0. );
    for ( nat j = 0; j < nv; ++j )
    {
        v += vertex[index[j]] % vertex[index[j+1]];
    }
    return 0.5 * ( v * plane.norm );
}

//**************************** 01.03.2014 *********************************//
//
//                          Периметр грани
//
//**************************** 01.03.2014 *********************************//

double Facet::perimeter ( CArrRef<Vector3d> vertex ) const
{
    if ( nv < 2 ) return 0.;
    double p = 0;
    for ( nat j = 0; j < nv; ++j )
    {
        p += norm2 ( vertex[index[j]] - vertex[index[j+1]] );
    }
    return p;
}

//**************************** 24.11.2012 ****************************
//
//                          Центр масс грани
//
//**************************** 24.11.2012 ****************************

Def<Vector3d> Facet::centerOfMass ( CArrRef<Vector3d> vertex ) const
{
    if ( nv > 2 )
    {
        double a = 0.;
        Vector3d v ( 0., 0., 0. );
        Vector3d v0 ( vertex[index[0]] );
        Vector3d v1 ( vertex[index[1]] );
        double p = norm1 ( v0 - vertex[index[nv-1]] ) + norm1 ( v0 - v1 );
        nat i;
        for ( i = 2; i < nv; ++i )
        {
            const Vector3d & v2 = vertex[index[i]];
            const double t = ( v1 - v0 ) % ( v2 - v0 ) * plane.norm;
            v += t * ( v0 + v1 + v2 );
            a += t;
            p += norm1 ( v1 - v2 );
            v1 = v2;
        }
        if ( p == 0 ) return vertex[index[0]];
        if ( 1e9 * fabs ( a ) > p * p ) return v / ( 3. * a );
        p = 0.;
        v.fill();
        v0 = vertex[index[nv-1]];
        for ( i = 0; i < nv; ++i )
        {
            const Vector3d & v2 = vertex[index[i]];
            const double d = norm2 ( v1 - v2 );
            v += d * ( v1 + v2 );
            p += d;
            v1 = v2;
        }
        return v / ( p + p );
    }
    if ( nv == 2 ) return 0.5 * ( vertex[index[0]] + vertex[index[1]] );
    return nv == 0 ? Def<Vector3d>() : Def<Vector3d> ( vertex[index[0]] );
}

///////////////////// Polyhedron ///////////////////////

Polyhedron & Polyhedron::initPlanes ()
{
    for ( nat i = 0; i < facet.size(); ++i ) facet[i].initPlane ( vertex );
    return *this;
}

void Polyhedron::countEdges ( nat & n1, nat & n2 ) const
{
    n1 = n2 = 0;
    for ( nat i = 0; i < facet.size(); ++i )
    {
        const Facet & f = facet[i];
        const nat * p = &f.index[f.nv+1];
        for ( nat j = 0; j < f.nv; ++j )
        {
            if ( p[j] == undef_index )
            {
                ++n2;
            }
            else
            {
                ++n1;
            }
        }
    }
    n1 /= 2;
}

Polyhedron & Polyhedron::linkFacets()
{
    Suite< SortItem <Set2<nat>, Set2<nat> > > suite12(4*facet.size());
    Suite< SortItem <Set2<nat>, Set2<nat> > > suite21(4*facet.size());
    for ( nat i = 0; i < facet.size(); ++i )
    {
        Facet & faceti = facet[i];
        if ( faceti.nv > 0 )
        {
            faceti.index[faceti.nv] = faceti.index[0];
        }
        SortItem < Set2<nat>, Set2<nat> > e;
        e.tail.a = i;
        nat j;
        for ( j = 0; j < faceti.nv; ++j )
        {
            e.tail.b = j;
            e.head.a = faceti.index[j];
            e.head.b = faceti.index[j+1];
            if ( e.head.a < e.head.b )
            {
                suite12.inc() = e;
            }
            else
            {
                _swap ( e.head.a, e.head.b );
                suite21.inc() = e;
            }
        }
        for ( j = 3 * faceti.nv; j > faceti.nv; --j ) faceti.index[j] = undef_index;
    }
    quickSort123 ( suite12 );
    quickSort123 ( suite21 );
    nat i1 = 0, i2 = 0;
    while ( i1 < suite12.size() && i2 < suite21.size() )
    {
        const SortItem < Set2<nat>, Set2<nat> > & e1 = suite12[i1];
        const SortItem < Set2<nat>, Set2<nat> > & e2 = suite21[i2];
        if ( e1.head.a == e2.head.a )
        {
            if ( e1.head.b == e2.head.b )
            {
                Facet & facet1 = facet[e1.tail.a];
                Facet & facet2 = facet[e2.tail.a];
                const nat iif1 = e1.tail.b + facet1.nv + 1;
                const nat iif2 = e2.tail.b + facet2.nv + 1;
                facet1.index[iif1] = e2.tail.a;
                facet2.index[iif2] = e1.tail.a;
                facet1.index[iif1+facet1.nv] = e2.tail.b;
                facet2.index[iif2+facet2.nv] = e1.tail.b;
                ++i1;
                ++i2;
                continue;
            }
            if ( e1.head.b < e2.head.b )
                ++i1;
            else
                ++i2;
        }
        else
        {
            if ( e1.head.a < e2.head.a )
                ++i1;
            else
                ++i2;
        }
    }
    return *this;
}

//**************************** 10.06.2009 ****************************
//
//      Создание тетраэдра ( r - макс. значение координат вершин )
//
//**************************** 14.09.2019 ****************************

Polyhedron & Polyhedron::makeTetrahedron ( double r )
{
    makeVoid();
    if ( r <= 0 ) return *this;

    if ( vertex.size() != 4 ) vertex.resize ( 4 );
    if ( facet .size() != 4 ) facet .resize ( 4 );

    const double m = -r;
    vertex[0] = Vector3d ( r, m, r );
    vertex[1] = Vector3d ( r, r, m );
    vertex[2] = Vector3d ( m, r, r );
    vertex[3] = Vector3d ( m, m, m );

    if ( facet[0].nv != 3 ) facet[0].resize ( 3 );
    if ( facet[1].nv != 3 ) facet[1].resize ( 3 );
    if ( facet[2].nv != 3 ) facet[2].resize ( 3 );
    if ( facet[3].nv != 3 ) facet[3].resize ( 3 );

    nat * p;
    p = facet[3].index();
    p[0] = 0;
    p[1] = 1;
    p[2] = 2;
    p[3] = 0;
    p = facet[2].index();
    p[0] = 0;
    p[1] = 3;
    p[2] = 1;
    p[3] = 0;
    p = facet[1].index();
    p[0] = 0;
    p[1] = 2;
    p[2] = 3;
    p[3] = 0;
    p = facet[0].index();
    p[0] = 1;
    p[1] = 3;
    p[2] = 2;
    p[3] = 1;

    return initPlanes().linkFacets();
}

//**************************** 10.06.2009 ****************************
//
//      Создание октаэдра ( r - макс. значение координат вершин )
//
//**************************** 11.10.2009 ****************************

Polyhedron & Polyhedron::makeOctahedron ( double r )
{
    makeVoid();
    if ( r <= 0 ) return *this;

    vertex.resize ( 6 );
    facet .resize ( 8 );

    const double m = -r;
    vertex[0] = Vector3d ( 0, 0, m );
    vertex[1] = Vector3d ( 0, m, 0 );
    vertex[2] = Vector3d ( m, 0, 0 );
    vertex[3] = Vector3d ( 0, 0, r );
    vertex[4] = Vector3d ( 0, r, 0 );
    vertex[5] = Vector3d ( r, 0, 0 );

    facet[0].resize ( 3 );
    facet[1].resize ( 3 );
    facet[2].resize ( 3 );
    facet[3].resize ( 3 );
    facet[4].resize ( 3 );
    facet[5].resize ( 3 );
    facet[6].resize ( 3 );
    facet[7].resize ( 3 );

    nat * p;
    p = facet[0].index();
    p[0] = 0;
    p[1] = 1;
    p[2] = 2;
    p[3] = 0;
    p = facet[1].index();
    p[0] = 0;
    p[1] = 2;
    p[2] = 4;
    p[3] = 0;
    p = facet[2].index();
    p[0] = 0;
    p[1] = 4;
    p[2] = 5;
    p[3] = 0;
    p = facet[3].index();
    p[0] = 0;
    p[1] = 5;
    p[2] = 1;
    p[3] = 0;
    p = facet[4].index();
    p[0] = 3;
    p[1] = 2;
    p[2] = 1;
    p[3] = 3;
    p = facet[5].index();
    p[0] = 3;
    p[1] = 4;
    p[2] = 2;
    p[3] = 3;
    p = facet[6].index();
    p[0] = 3;
    p[1] = 5;
    p[2] = 4;
    p[3] = 3;
    p = facet[7].index();
    p[0] = 3;
    p[1] = 1;
    p[2] = 5;
    p[3] = 3;

    return initPlanes().linkFacets();
}

//**************************** 17.10.2005 ****************************
//
// Создание прямоугольного параллелепипеда ( sx, sy, sz - половины сторон )
//
//**************************** 11.10.2009 ****************************

Polyhedron & Polyhedron::makeCuboid ( double sx, double sy, double sz )
{
    makeVoid();
    if ( sx <= 0 || sy <= 0 || sz <= 0 ) return *this;

    vertex.resize ( 8 );
    facet .resize ( 6 );

    const double mx = -sx;
    const double my = -sy;
    const double mz = -sz;
    vertex[0] = Vector3d ( mx, my, mz );
    vertex[1] = Vector3d ( mx, my, sz );
    vertex[2] = Vector3d ( mx, sy, mz );
    vertex[3] = Vector3d ( mx, sy, sz );
    vertex[4] = Vector3d ( sx, my, mz );
    vertex[5] = Vector3d ( sx, my, sz );
    vertex[6] = Vector3d ( sx, sy, mz );
    vertex[7] = Vector3d ( sx, sy, sz );

    facet[0].resize ( 4 );
    facet[1].resize ( 4 );
    facet[2].resize ( 4 );
    facet[3].resize ( 4 );
    facet[4].resize ( 4 );
    facet[5].resize ( 4 );

    nat * p;
    p = facet[0].index();
    p[0] = 0;
    p[1] = 1;
    p[2] = 3;
    p[3] = 2;
    p[4] = 0;
    p = facet[1].index();
    p[0] = 1;
    p[1] = 0;
    p[2] = 4;
    p[3] = 5;
    p[4] = 1;
    p = facet[2].index();
    p[0] = 4;
    p[1] = 0;
    p[2] = 2;
    p[3] = 6;
    p[4] = 4;
    p = facet[3].index();
    p[0] = 4;
    p[1] = 6;
    p[2] = 7;
    p[3] = 5;
    p[4] = 4;
    p = facet[4].index();
    p[0] = 6;
    p[1] = 2;
    p[2] = 3;
    p[3] = 7;
    p[4] = 6;
    p = facet[5].index();
    p[0] = 1;
    p[1] = 5;
    p[2] = 7;
    p[3] = 3;
    p[4] = 1;

    facet[0].plane = Plane3d ( Vector3d ( -1.,  0.,  0. ), mx );
    facet[1].plane = Plane3d ( Vector3d (  0., -1.,  0. ), my );
    facet[2].plane = Plane3d ( Vector3d (  0.,  0., -1. ), mz );
    facet[3].plane = Plane3d ( Vector3d (  1.,  0.,  0. ), mx );
    facet[4].plane = Plane3d ( Vector3d (  0.,  1.,  0. ), my );
    facet[5].plane = Plane3d ( Vector3d (  0.,  0.,  1. ), mz );

    return linkFacets();
}

//**************************** 31.01.2014 ****************************
//
//                          Создание призмы
//
//**************************** 11.02.2014 ****************************

Polyhedron & Polyhedron::makePrism ( CArrRef<Vector2d> vert, double z )
{
    const nat n = vert.size();
    vertex.resize ( n + n );
    facet .resize ( n + 2 );
    nat * s;
    nat i, j;
    for ( i = 0; i < n - 1; ++i )
    {
        s = facet[i].resize(4).index();
        s[0] =  i + i;
        s[1] = *s + 1;
        s[2] = *s + 3;
        s[3] = *s + 2;
    }

    s = facet[i].resize(4).index();
    s[0] =  i + i;
    s[1] = *s + 1;
    s[2] =  1;
    s[3] =  0;

    s = facet[++i].resize(n).index();
    for ( j = 0; j < n; ++j ) s[j] = j + j;

    s = facet[++i].resize(n).index();
    for ( j = 0; j < n; ++j ) s[j] = 2 * ( n - j ) - 1;

    for ( i = j = 0; i < n; ++i )
    {
        vertex[j++] = Vector3d ( vert[i].x, vert[i].y, +z );
        vertex[j++] = Vector3d ( vert[i].x, vert[i].y, -z );
    }

    return linkFacets().initPlanes();
}

//**************************** 28.12.2005 ****************************
//
//          Создание элипсоида ( x, y, z - половины осей )
//
//**************************** 11.10.2009 ****************************

Polyhedron & Polyhedron::makeEllipsoid ( nat n, double x, double y, double z, RandVector3d & qrv )
{
    DynArray<Vector3d> v ( n );
    for ( nat i = 0; i < n; ++i )
    {
        v[i] = qrv();
        v[i].x *= x;
        v[i].y *= y;
        v[i].z *= z;
    }
    return convexHull ( v, *this );
}

//**************************** 28.12.2005 ****************************
//
//          Создание элипсоида ( x, y, z - половины осей )
//
//**************************** 20.07.2019 ****************************

Polyhedron & Polyhedron::makeEllipsoid ( double x, double y, double z, nat n )
{
    DynArray<Vector3d> v ( n );
    for ( nat i = 0; i < n; ++i )
    {
        const double h = ( 1 + 2*i ) / double(n) - 1;
        const double r = sqrt ( 1 - h*h );
        const double a = i * M_2PI * 0.61803398874989484820;
        v[i] = Vector3d ( x*cos(a)*r, y*sin(a)*r, z*h );
    }
    return convexHull ( v, *this );
}

//**************************** 26.10.2015 ****************************
//
//                          Создание цилиндра
//
//**************************** 26.10.2015 ****************************

Polyhedron & Polyhedron::makeModel ( const Cylinder3d & fig, nat n )
{
    DynArray<Vector2d> vert ( n );
    regularPolygon ( vert, fig.r );
    return makePrism ( vert, fig.h ) *= Conform3d ( fig.spin, fig.o, 1 );
}

//**************************** 29.10.2015 ****************************
//
//                          Создание конуса
//
//**************************** 29.10.2015 ****************************

Polyhedron & Polyhedron::makeModel ( const Cone3d & fig, nat n )
{
    if ( n < 3 ) return makeVoid();
    const double a = 2 * M_PI / n;
    vertex.resize ( n + 1 );
    facet .resize ( n + 1 );
    nat * s;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        s = facet[i].resize(3).index();
        s[0] = i;
        s[1] = i + 1;
        s[2] = n;
        const double f = a * i;
        vertex[i] = Vector3d ( -fig.r * sin ( f ), fig.r * cos ( f ), -fig.h );
    }
    facet[n-1].index[1] = 0;
    s = facet[n].resize(n).index();
    for ( i = 0; i < n; ++i ) s[i] = n - i -1 ;
    vertex[n] = Vector3d ( 0, 0, fig.h );
    return linkFacets().initPlanes() *= Conform3d ( fig.spin, fig.o, 1 );
}

//**************************** 24.11.2012 ****************************
//
//                 Площадь поверхности многогранника
//
//**************************** 12.06.2017 ****************************

double Polyhedron::getArea () const
{
    double res = 0;
    for ( nat i = 0; i < facet.size(); ++i ) res += facet[i].getArea ( vertex );
    return res;
}

//**************************** 24.11.2012 ****************************
//
//                        Объём многогранника
//
//**************************** 24.11.2012 ****************************

double Polyhedron::volume () const
{
    double res = 0;
    for ( nat i = 0; i < facet.size(); ++i )
    {
        Vector3d v ( 0., 0., 0. );
        const Facet & f = facet[i];
        for ( nat j = 0; j < f.nv; ++j )
        {
            v += vertex[f.index[j+1]] % vertex[f.index[j]];
        }
        res += ( v * f.plane.norm ) * f.plane.dist;
    }
    return res / 6.;
}

//**************************** 24.11.2012 ****************************
//
//                     Центр масс многогранника
//
//**************************** 24.11.2012 ****************************

Def<Vector3d> Polyhedron::centerOfMass () const
{
    double m = 0.;
    Vector3d c ( 0, 0, 0 );
    for ( nat i = 0; i < facet.size(); ++i )
    {
        const Facet & f = facet[i];
        if ( f.nv < 3 ) continue;
        const Vector3d & v1 = vertex[f.index[0]];
        for ( nat j = 2; j < f.nv; ++j )
        {
            const Vector3d & v2 = vertex[f.index[j-1]];
            const Vector3d & v3 = vertex[f.index[j]];
            const double p = ( v1 % v2 * v3 ) / 6.;
            c += ( v1 + v2 + v3 ) * p;
            m += p;
        }
    }
    return m == 0 ? Def<Vector3d>() : Def<Vector3d> ( c /= ( 4 * m ) );
}

} // namespace Shev

bool trianTest ( const Facet & facet, CArrRef<Vector3d> vert )
{
    if ( facet.nv < 3 ) return false;
    DynArray<Vector2d> vert2d ( facet.nv );
    Func3to2 func = getTrans ( facet.plane.norm );
    for ( nat j = 0; j < facet.nv; ++j ) vert2d[j] = func ( vert[facet.index[j]] );
    return loopNumber ( vert2d ) == 1 && trianTestNat1L1MinTan ( vert2d );
}

void check ( const Polyhedron & poly, IPolyhedronErrorRecipient & rec )
{
    rec.start ( poly.vertex.size(), poly.facet.size() );
    const double eps = rec.getTolerance ();
    const double minArea = rec.getMinAdmArea();
    nat i;
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & facet = poly.facet[i];
        if ( facet.nv < 3 )
        {
            rec.vertexNumIsTooSmall ( i, facet.nv );
            continue;
        }
        if ( facet.index[0] != facet.index[facet.nv] )
        {
            rec.vertexIndexIsDiffer ( i, facet.index[0], facet.index[facet.nv] );
        }
        bool ok = true;
        nat j;
        for ( j = 0; j < facet.nv; ++j )
        {
            const nat l = facet.index[j];
            if ( l >= poly.vertex.size() )
            {
                rec.vertexIndexIsTooBig ( i, j );
                ok = false;
            }
            else
            {
                if ( l == facet.index[j+1] ) rec.vertexIndexIsEqual ( i, j );
                const double t = facet.plane % poly.vertex[l];
                if ( fabs ( t ) > eps )
                {
                    rec.putDeviation ( i, j, t );
                }
            }
        }
        if ( ! ok ) continue;
        if ( ! trianTest ( facet, poly.vertex ) )
        {
            rec.trianTest ( i );
        }
        const double area = facet.getArea ( poly.vertex );
        if ( area < minArea )
        {
            rec.putSmallArea ( i, area );
        }
        const nat nv2 = 2 * facet.nv;
        for ( j = facet.nv + 1; j <= nv2; ++j )
        {
            const nat ij = facet.index[j];
            if ( ij == undef_index )
            {
                rec.facetIndexIsUndef ( i, j );
                continue;
            }
            if ( ij >= poly.facet.size() )
            {
                rec.facetIndexIsTooBig ( i, j, ij );
                continue;
            }
            const nat ijn = facet.index[j+facet.nv];
            if ( ijn == undef_index )
            {
                rec.indexIndexIsUndef ( i, j );
                continue;
            }
            const Facet & neighbour = poly.facet[ij];
            if ( ijn >= neighbour.nv )
            {
                rec.indexIndexIsTooBig ( i, j+facet.nv, ijn, neighbour.nv );
                continue;
            }
            if ( facet.index[j-facet.nv-1] != neighbour.index[ijn+1] ||
                 facet.index[j-facet.nv  ] != neighbour.index[ijn  ] )
            {
                rec.neighbourIsBad ( i, j, ijn );
                continue;
            }
        }
    }
    const double vol = poly.volume();
    if ( vol < 0 ) rec.volumeIsNeg ( vol );
    rec.finish();
}

//**************************** 16.05.2006 ****************************
//
//      Вычисление расстояния от точки до поверхности многогранника
//      Если точка находится внутри, то расстояние - отрицательное
//
//**************************** 29.04.2011 ****************************

bool distance ( const Polyhedron & poly, const Vector3d & p, double & dist, 
                nat & vi, nat & ei, nat & fi )
{
    vi = ei = fi = undef_index;
    if ( poly.facet.size() == 0 ) return false;
// Поиск ближайшего ребра или вершины
    CArrRef<Vector3d> vert ( poly.vertex );
    double min = 1e300;
    nat i;
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & facet = poly.facet[i];
        if ( facet.nv < 2 ) continue;
        CArrRef<nat> findex ( facet.index, facet.nv + 1, facet.nv );
        for ( nat j = 0; j < facet.nv; ++j )
        {
            if ( findex[j] < i ) continue;
            const Vector3d & a = vert[facet.index[j]];
            const Vector3d & b = vert[facet.index[j+1]];
            const Vector3d c = b - a;
            const double d = c * c;
            if ( d == 0 ) continue;
            const Vector3d e = p - a;
            const double s = e * c;
            if ( s <= 0 )
            {
                const double t = e * e;
                if ( t == 0 )
                {
                    dist = 0.;
                    vi = facet.index[j];
                    ei = fi = undef_index;
                    return true;
                }
                if ( min > t ) min = t, vi = facet.index[j];
                continue;
            }
            if ( s >= d )
            {
                const double t = qmod ( p - b );
                if ( t == 0 )
                {
                    dist = 0.;
                    vi = facet.index[j+1];
                    ei = fi = undef_index;
                    return true;
                }
                if ( min > t ) min = t, vi = facet.index[j+1];
                continue;
            }
            const double t = e * e - s * s / d;
            if ( t == 0 )
            {
                dist = 0.;
                vi = undef_index;
                ei = j;
                fi = i;
                return true;
            }
            if ( min > t )
            {
                min = t;
                vi = undef_index;
                ei = j;
                fi = i;
            }
        }
    }
    if ( vi == undef_index && ei == undef_index ) return false;
// Поиск ближайшей грани
    Suite<Vector2d> buf; 
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & facet = poly.facet[i];
        if ( facet.nv < 3 ) continue;
        const double t = _pow2 ( facet.plane % p );
        if ( min > t && 
            isIntersect ( trans ( vert, facet, buf ), trans ( facet.plane.project ( p ), facet.plane.norm ) ) )
        {
            vi = ei = undef_index;
            fi = i;
            if ( t == 0 ) return true;
            min = t;
        }
    }
// Определение расстояния
    if ( vi == undef_index )
    {
        if ( ei == undef_index )
        {
            dist = poly.facet[fi].plane % p;
        }
        else
        {
            min = sqrt ( min );
            const Facet & facet = poly.facet[fi];
            const nat j = facet.index[facet.nv+1+ei];
            if ( j != undef_index )
            {
                const Plane3d & plane0 = facet.plane;
                const Plane3d & plane1 = poly.facet[j].plane;
                const Plane3d plane ( plane0.norm + plane1.norm, plane0.dist + plane1.dist );
                dist = plane % p < 0 ? - min : min;
            }
        }
    }
    else
    {
        double u = 2;
        for ( i = 0; i < poly.facet.size(); ++i )
        {
            const Facet & facet = poly.facet[i];
            if ( facet.nv < 2 ) continue;
            for ( nat j = 0; j < facet.nv; ++j )
            {
                const nat i0 = facet.index[j];
                if ( i0 == vi )
                {
                    const Vector3d & a = vert[i0];
                    const Vector3d & b = vert[facet.index[j+1]];
                    const Vector3d c = b - a;
                    const Vector3d e = p - a;
                    const double s = e * c;
                    const double d = c * c;
                    const double t = s * s / ( d * min );
                    if ( u > t ) u = t, ei = j, fi = i;
                }
            }
        }
        min = sqrt ( min );
        const Facet & facet = poly.facet[fi];
        const nat j = facet.index[facet.nv+1+ei];
        if ( j != undef_index )
        {
            const Plane3d & plane0 = facet.plane;
            const Plane3d & plane1 = poly.facet[j].plane;
            const Plane3d plane ( plane0.norm + plane1.norm, plane0.dist + plane1.dist );
            dist = plane % p < 0 ? - min : min;
        }
        fi = ei = undef_index;
    }
    return true;
}
