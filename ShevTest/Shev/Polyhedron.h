
#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "ShevArray.h"
#include "Vector3d.h"

class Vector2d;
class RandVector3d;

namespace Shev
{
    const nat undef_index = 0xFFFFFFFF;

    class Facet
    {
        Facet ( const Facet & );
    public:
        Plane3d plane;
        nat nv;
        DynArray<nat> index;
        int info;

        Facet () : plane ( Vector3d ( 0., 0., 0. ), 0. ), nv(0), info(-123456789) {}

        Facet & operator = ( const Facet & );

        Facet & operator += ( const Vector3d & v )
        {
            plane += v;
            return *this;
        }

        Facet & operator -= ( const Vector3d & v )
        {
            plane -= v;
            return *this;
        }

        template <class T> Facet & operator *= ( const T & t )
        {
            plane *= t;
            return *this;
        }

        Facet & resize ( nat n = 0 )
        {
            index.resize ( ( nv = n ) > 0 ? 3 * nv + 1 : 0 );
            return *this;
        }

        Facet & initPlane ( CArrRef<Vector3d> vertex );

        double getArea ( CArrRef<Vector3d> vertex ) const; // ������� �����

        double perimeter ( CArrRef<Vector3d> vertex ) const; // �������� �����

        Def<Vector3d> centerOfMass ( CArrRef<Vector3d> vertex ) const; // ����� ���� �����
    };

    class Polyhedron
    {
        Polyhedron ( const Polyhedron & );
    public:
        DynArray<Vector3d> vertex;
        DynArray<Facet> facet;

        Polyhedron () {}
        Polyhedron ( nat nv, nat nf ) : vertex(nv), facet(nf) {}
        explicit Polyhedron ( const Tetrahedron & fig ) { *this = fig; }
        explicit Polyhedron ( const Cuboid3d & fig ) { *this = fig; }

        Polyhedron & linkFacets (); // ������� �����
        Polyhedron & initPlanes (); // ��������� ���������

        void countEdges ( nat & n1, nat & n2 ) const;

        Polyhedron & operator = ( const Polyhedron & poly )
        {
            if ( this != & poly )
            {
                vertex = poly.vertex;
                facet = poly.facet;
            }
            return *this;
        }

        Polyhedron & operator = ( const Tetrahedron & fig );

        Polyhedron & operator = ( const Cuboid3d & fig )
        {
            return makeCuboid ( fig.a, fig.b, fig.c ) *= Conform3d ( fig.spin, fig.o, 1 );
        }

        Polyhedron & operator += ( const Vector3d & v )
        {
            vertex += v;
            facet  += v;
            return *this;
        }

        Polyhedron & operator -= ( const Vector3d & v )
        {
            vertex -= v;
            facet  -= v;
            return *this;
        }

        template <class T> Polyhedron & operator *= ( const T & t )
        {
            vertex *= t;
            facet  *= t;
            return *this;
        }

        Polyhedron & operator *= ( const Conform3d & conf )
        {
            return *this *= Similar3d ( conf );
        }

        Polyhedron & operator *= ( const Spin3d & spin )
        {
            return *this *= Ortho3d ( spin );
        }
        // ������ ������������
        Polyhedron & makeVoid ()  
        {
            vertex.resize ( 0 ); facet.resize ( 0 ); return *this;
        }
        // �������� ( r - ����. �������� ��������� ������ )
        Polyhedron & makeTetrahedron ( double r ); 
        // �������  ( r - ����. �������� ��������� ������ ) 
        Polyhedron & makeOctahedron  ( double r ); 
        // ������������� �������������� ( x, y, z - �������� ������ )
        Polyhedron & makeCuboid ( double x, double y, double z );
        // ��� ( r - �������� ������� )
        Polyhedron & makeCube ( double r )
        {
            return makeCuboid ( r, r, r );
        }
        // ������
        Polyhedron & makePrism ( CCArrRef<Vector2d> & vert, double z );
        // ��������� ( x, y, z - �������� ���� )
        Polyhedron & makeEllipsoid ( nat n, double x, double y, double z, RandVector3d & qrv );
        Polyhedron & makeEllipsoid ( double x, double y, double z, nat n = 1095 );
        Polyhedron & makeModel ( const Ellipsoid3d & fig, nat n = 1095 )
        {
            return makeEllipsoid ( fig.a, fig.b, fig.c, n ) *= Conform3d ( fig.spin, fig.o, 1 );
        }
        // ����� ( r - ������ )
        Polyhedron & makeSphere ( double r, nat n = 1095 )
        {
            return makeEllipsoid ( r, r, r, n );
        }
        Polyhedron & makeModel ( const Sphere3d & fig, nat n = 1095 )
        {
            return makeSphere ( fig.r, n ) += fig.o;
        }
        // �������
        Polyhedron & makeModel ( const Cylinder3d & fig, nat n );
        // �����
        Polyhedron & makeModel ( const Cone3d & fig, nat n );
        // ������� ����������� �������������
        double getArea () const;
        // ����� �������������
        double volume () const;
        // ����� ���� �������������
        Def<Vector3d> centerOfMass () const;
    };

} // namespace Shev

using namespace Shev;

inline
void _swap ( Facet & facet1, Facet & facet2 )
{
    _swap ( facet1.plane, facet2.plane );
    _swap ( facet1.index, facet2.index );
    _swap ( facet1.info, facet2.info );
    _swap ( facet1.nv, facet2.nv );
}

bool trianTest ( const Facet & facet, CArrRef<Vector3d> vert );

inline
void _swap ( Polyhedron & poly1, Polyhedron & poly2 )
{
    _swap ( poly1.vertex, poly2.vertex );
    _swap ( poly1.facet, poly2.facet );
}

class IPolyhedronErrorRecipient
{
public:
    virtual void start ( nat nv, nat nf ) = 0;
    virtual double getTolerance () = 0;
    virtual double getMinAdmArea () = 0;
    virtual void putSmallArea ( nat i, double a ) = 0;
    virtual void putDeviation ( nat i, nat j, double d ) = 0;
    virtual void vertexNumIsTooSmall ( nat i, nat n ) = 0;
    virtual void vertexIndexIsTooBig ( nat i, nat j ) = 0;
    virtual void vertexIndexIsEqual  ( nat i, nat j ) = 0;
    virtual void vertexIndexIsDiffer ( nat i, nat j, nat l ) = 0;
    virtual void  facetIndexIsUndef  ( nat i, nat j ) = 0;
    virtual void  facetIndexIsTooBig ( nat i, nat j, nat n ) = 0;
    virtual void  indexIndexIsUndef  ( nat i, nat j ) = 0;
    virtual void  indexIndexIsTooBig ( nat i, nat j, nat n, nat m ) = 0;
    virtual void    neighbourIsBad   ( nat i, nat n, nat m ) = 0;
    virtual void trianTest ( nat i ) = 0;
    virtual void volumeIsNeg ( double vol ) = 0;
    virtual void finish () = 0;
};

void check ( const Polyhedron & poly, IPolyhedronErrorRecipient & rec );


//**************************** 16.05.2006 ****************************
//
//      ���������� ���������� �� ����� �� ����������� �������������
//      ���� ����� ��������� ������, �� ���������� - �������������
//
//**************************** 29.04.2011 ****************************

bool distance ( const Polyhedron & poly, const Vector3d & point, double & dist, 
                nat & vi, nat & ei, nat & fi );

inline bool distance ( const Polyhedron & poly, const Vector3d & point, double & dist )
{
    nat vi, ei, fi;
    return distance ( poly, point, dist, vi, ei, fi );
}

#endif