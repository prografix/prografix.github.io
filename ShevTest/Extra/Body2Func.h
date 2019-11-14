
#ifndef BODY2FUNC_H
#define BODY2FUNC_H

#include "../shev/template.h"
#include "../shev/lists.h"

class Vert;
class Body;
class OList;
class ZOList;
class Spin3d;
struct Mom3d;
class Plane3d;
class BodyStor;
class Vector2d;
class Vector3d;
class Conform3d;
class Similar3d;

template <class T> class CArrRef;
template <class T> class DynArrRef;

// �������� �������������

nat           facetCount   ( const Body & body ); // �-�� ������
nat           vertexCount  ( const Body & body ); // �-�� ������
double        volume       ( const Body & body ); // �����
Def<Vector3d> centerOfMass ( const Body & body ); // ����� ����
bool          isConvex     ( const Body & body ); // �������� �� ����������

// ������� �� ������� ������������

// ������������ �������������
Body * clone ( const Body & body );
// ������������ ��������� ������������ ������ �� ������ v
Def<double> maxSP ( const Body & body, const Vector3d & v );
// ����������� ��������� ������������ ������ �� ������ v
Def<double> minSP ( const Body & body, const Vector3d & v );
// ������������ ��������� ������������ ������ ��������� ����� �� ������ v
Def<double> maxSPLastFacet ( const Body & body, const Vector3d & v );
// ����������� ��������� ������������ ������ ��������� ����� �� ������ v
Def<double> minSPLastFacet ( const Body & body, const Vector3d & v );
// �������� ��������� ������ �� ��������� XY
DynArrRef<Vector2d> & projectVertexXY ( const List<Vert> & verts, DynArrRef<Vector2d> & res );
// �������� ��������� ������ �� ��������� YZ ����� �������������� sim
DynArrRef<Vector2d> & projectVertexYZ ( const List<Vert> & verts, const Similar3d & sim, DynArrRef<Vector2d> & res );
// �������� ������������� �� ��������� YZ ����� �������������� sim
DynArrRef<Vector2d> & projectYZ ( const Body & body, const Conform3d & sim, List2p & stor, DynArrRef<Vector2d> & proj );
DynArrRef<Vector2d> & projectYZ ( const Body & body, const Similar3d & sim, List2p & stor, DynArrRef<Vector2d> & proj );
void makeProjectionYZ ( const Body & body, const Similar3d & trans, ZOList & zol, List2p & istor, OList & ostor );
void makeProjectionYZ ( const Body & body, const Conform3d & trans, ZOList & zol, List2p & istor, OList & ostor );
// ���������� �������� ���� � �������� 2-�� �������
Def<Mom3d> momentum2plh ( const Body & body );

// ���������������

void delSelf ( Body * body );

// ���������� �������������� �����

Body & makeVoid           ( Body & body );                                      // ������ ������������ ( 0 ������, 0 ������, 0 ���� )
Body & makeVoid           ( Body & body, BodyStor & stor );                     // ������ ������������ ( 0 ������, 0 ������, 0 ���� )
Body & makeCube           ( Body & body, double r );                            // ��� ( r - �������� ������� )
Body & makeOctahedron     ( Body & body, double r );                            // �������  ( r - ����. �������� ��������� ������ )
Body & makeTetrahedron    ( Body & body, double r );                            // �������� ( r - ����. �������� ��������� ������ )
Body & makePrism          ( Body & body, CArrRef<Vector2d> vert, double z );    // ������
Body & makeSphere         ( Body & body, double r, nat n );                     // ����� ( r - ������, n - �-�� ������ )
Body & makeCuboid         ( Body & body, double x, double y, double z );        // ������������� �������������� ( x, y, z - �������� ������ )
Body & makeEllipsoid      ( Body & body, double x, double y, double z, nat n ); // ��������� ( x, y, z - �������� ����, n - �-�� ������ )

void cut ( Body & body, const Plane3d & plane, int mark = 0 );

void swap ( Body & body1, Body & body2 );

Body & copy ( const Body & source, BodyStor & stor, Body & target );

//**************************** 21.10.2008 *********************************//
//
//        ��������� �������� �������� �������� ������ �� ��������� XY
//  factor - ��� ���������� ��������� ������� �������������� ��� ���������
//
//**************************** 09.02.2016 *********************************//

template <class T> class SuiteRef;

SuiteRef<Vector2d> & convexProjectionXY ( const Body & body, double factor, SuiteRef<Vector2d> & res );


//**************************** 21.01.2011 *********************************//
//
//        ��������� �������� �������� �������� ������ �� ��������� YZ
//  factor - ��� ���������� ��������� ������� �������������� ��� ���������
//
//**************************** 09.02.2016 *********************************//

class Line2d;

SuiteRef<Vector2d> & convexProjectionYZ ( const Body & body, const Similar3d & sim, double factor, SuiteRef<Vector2d> & res );
DynArrRef<Line2d>  & convexProjectionYZ ( const Body & body, const Similar3d & sim, double factor, DynArrRef<Line2d>  & res );


//**************************** 11.05.2010 *********************************//
//
//              ��������� �������������� �� �����
//
//**************************** 11.05.2010 *********************************//

class ZOList;

Body & circumcision ( Body & body, const ZOList & zol, double eps );

unsigned int controlSum1 ( const Body & body );
unsigned int controlSum2 ( const Body & body );
unsigned int controlSum3 ( const Body & body );

void clearVerts ( Body & body );
void clearBones ( Body & body );
void clearSides ( Body & body );
void unionSides ( Body & body, double eps );
void clearNearSides ( Body & body, double eps );
void minimize3Sides ( Body & body, double eps );
void minimize4Sides ( Body & body, double eps );


//**************************** 04.02.2014 ****************************
//
//    �������� ��������� ���� ( ���������� �-�� �������� ���� )
//
//**************************** 04.02.2014 ****************************

nat delSmallBones ( Body & body, double eps );


//**************************** 16.03.2006 ****************************
//
//          �������� ������������ ������� ���� Body
//
//**************************** 23.10.2006 ****************************

class Vert;
class Bone;
class Side;

class IBodyErrorRecipient
{
public:
    virtual void start () = 0;
    virtual double getTolerance () = 0;
    virtual double getMinAdmArea () = 0;
    virtual void putDeviation ( const Side * side, const Vert * vert, double d ) = 0;
    virtual void putSmallArea ( const Side * side, double a ) = 0;
    virtual void vertList ( const char * str ) = 0;
    virtual void boneList ( const char * str ) = 0;
    virtual void sideList ( const char * str ) = 0;
    virtual void boneIsNull ( const Bone * bone ) = 0;
    virtual void boneHasNullSide ( const Bone * bone ) = 0;
    virtual void boneHasEqualSides ( const Bone * bone ) = 0;
    virtual void boneHasOutVert ( const Bone * bone, const Vert * vert ) = 0;
    virtual void sideHasNotBone ( const Side * side, const Bone * bone ) = 0;
    virtual void sideHasNotOrder ( const Side * side, const Vert * v0, const Vert * v1 ) = 0;
    virtual void sideHasNotInBone ( const Side * side, const Bone * bone ) = 0;
    virtual void sideIntersect ( const Side * side ) = 0;
    virtual void volumeIsNeg ( double vol ) = 0;
    virtual void finish () = 0;
};

void check ( const Body & body, IBodyErrorRecipient & ber );
void test ( const Body & body, double tol, double area );


//**************************** ����������� ****************************

enum PolyhedronDoctorReturn doctorPolyhedron ( const Body & poly1, Body & poly2, class IPolyhedronDoctorReport & rep );


//**************************** 16.09.2013 ****************************
//
//                       �������������� ����
//
//**************************** 16.09.2013 ****************************

void sortBones ( Body & body );


namespace Shev
{
    class Polyhedron;
}

Body & copy ( const Shev::Polyhedron & poly, Body & body );

bool copy ( const Body & body, Shev::Polyhedron & poly );

#endif