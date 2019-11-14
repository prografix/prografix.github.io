
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

// Свойства многогранника

nat           facetCount   ( const Body & body ); // К-во граней
nat           vertexCount  ( const Body & body ); // К-во вершин
double        volume       ( const Body & body ); // Объём
Def<Vector3d> centerOfMass ( const Body & body ); // Центр масс
bool          isConvex     ( const Body & body ); // Проверка на выпуклость

// Функции не менящие многогранник

// Клонирование многогранника
Body * clone ( const Body & body );
// Максимальное скалярное произведение вершин на вектор v
Def<double> maxSP ( const Body & body, const Vector3d & v );
// Минимальное скалярное произведение вершин на вектор v
Def<double> minSP ( const Body & body, const Vector3d & v );
// Максимальное скалярное произведение вершин последней грани на вектор v
Def<double> maxSPLastFacet ( const Body & body, const Vector3d & v );
// Минимальное скалярное произведение вершин последней грани на вектор v
Def<double> minSPLastFacet ( const Body & body, const Vector3d & v );
// Проекция множества вершин на плоскость XY
DynArrRef<Vector2d> & projectVertexXY ( const List<Vert> & verts, DynArrRef<Vector2d> & res );
// Проекция множества вершин на плоскость YZ после преобразования sim
DynArrRef<Vector2d> & projectVertexYZ ( const List<Vert> & verts, const Similar3d & sim, DynArrRef<Vector2d> & res );
// Проекция многогранника на плоскость YZ после преобразования sim
DynArrRef<Vector2d> & projectYZ ( const Body & body, const Conform3d & sim, List2p & stor, DynArrRef<Vector2d> & proj );
DynArrRef<Vector2d> & projectYZ ( const Body & body, const Similar3d & sim, List2p & stor, DynArrRef<Vector2d> & proj );
void makeProjectionYZ ( const Body & body, const Similar3d & trans, ZOList & zol, List2p & istor, OList & ostor );
void makeProjectionYZ ( const Body & body, const Conform3d & trans, ZOList & zol, List2p & istor, OList & ostor );
// Вычисление основных осей и моментов 2-го порядка
Def<Mom3d> momentum2plh ( const Body & body );

// Самоуничтожение

void delSelf ( Body * body );

// Построение геометрических фигур

Body & makeVoid           ( Body & body );                                      // Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )
Body & makeVoid           ( Body & body, BodyStor & stor );                     // Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )
Body & makeCube           ( Body & body, double r );                            // Куб ( r - половина стороны )
Body & makeOctahedron     ( Body & body, double r );                            // Октаэдр  ( r - макс. значение координат вершин )
Body & makeTetrahedron    ( Body & body, double r );                            // Тетраэдр ( r - макс. значение координат вершин )
Body & makePrism          ( Body & body, CArrRef<Vector2d> vert, double z );    // Призма
Body & makeSphere         ( Body & body, double r, nat n );                     // Сфера ( r - радиус, n - к-во вершин )
Body & makeCuboid         ( Body & body, double x, double y, double z );        // Прямоугольный параллелепипед ( x, y, z - половины сторон )
Body & makeEllipsoid      ( Body & body, double x, double y, double z, nat n ); // Эллипсоид ( x, y, z - половины осей, n - к-во вершин )

void cut ( Body & body, const Plane3d & plane, int mark = 0 );

void swap ( Body & body1, Body & body2 );

Body & copy ( const Body & source, BodyStor & stor, Body & target );

//**************************** 21.10.2008 *********************************//
//
//        Получение выпуклой оболочки проекции модели на плоскость XY
//  factor - это допустимое изменение площади многоугольника при упрощении
//
//**************************** 09.02.2016 *********************************//

template <class T> class SuiteRef;

SuiteRef<Vector2d> & convexProjectionXY ( const Body & body, double factor, SuiteRef<Vector2d> & res );


//**************************** 21.01.2011 *********************************//
//
//        Получение выпуклой оболочки проекции модели на плоскость YZ
//  factor - это допустимое изменение площади многоугольника при упрощении
//
//**************************** 09.02.2016 *********************************//

class Line2d;

SuiteRef<Vector2d> & convexProjectionYZ ( const Body & body, const Similar3d & sim, double factor, SuiteRef<Vector2d> & res );
DynArrRef<Line2d>  & convexProjectionYZ ( const Body & body, const Similar3d & sim, double factor, DynArrRef<Line2d>  & res );


//**************************** 11.05.2010 *********************************//
//
//              Обрезание многоугольника по краям
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
//    Удаление маленьких рёбер ( возвращает к-во удалённых рёбер )
//
//**************************** 04.02.2014 ****************************

nat delSmallBones ( Body & body, double eps );


//**************************** 16.03.2006 ****************************
//
//          Проверка правильности объекта типа Body
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


//**************************** Исправление ****************************

enum PolyhedronDoctorReturn doctorPolyhedron ( const Body & poly1, Body & poly2, class IPolyhedronDoctorReport & rep );


//**************************** 16.09.2013 ****************************
//
//                       Упорядочивание рёбер
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