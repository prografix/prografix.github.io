
#include "math.h"

#include "../shev/tune.h"
#include "body2func.h"
#include "../shev/approx3d.h"
#include "../shev/ShevList.h"
#include "../shev/ShevArray.h"
#include "../shev/Vector2d.h"
#include "../shev/Vector3d.h"
#include "../shev/moment3d.h"
#include "vert.h"
#include "body2.h"
#include "../shev/lists.h"
//#include "../shev/Polygon.h"
#include "../shev/Polyhedron.h"
#include "../shev/intersect3d.h"
#include "../shev/trans.h"
//#include "../shev/OList.h"
#include "../shev/func1t.h"
#include "../shev/func2d.h"
#include "../shev/trian2d.h"
#include "../test/Display.h"

// Свойства многогранника

nat facetCount ( const Body & body ) // К-во граней
{
    return body.sides.size();
}

nat vertexCount ( const Body & body ) // К-во вершин
{
    return body.verts.size();
}

double volume ( const Body & body ) // Объём
{
    return body.volume();
}

Def<Vector3d> centerOfMass ( const Body & body ) // Центр масс
{
    return body.centerOfMass();
}

bool isConvex ( const Body & body ) // Проверка на выпуклость
{
    Show<Bone> bones ( body.bones );
    if ( bones.top() )
    do
    {
        const Bone * b = bones.cur();
        const Vector3d v = b->s0->plane.norm % b->s1->plane.norm;
        if ( v * v < 1e-20 ) continue;
        if ( v * ( b->v1->point - b->v0->point ) < 0 )
            return false;
    }
    while ( bones.next() );
    return true;
}

// Функции не менящие многогранник

Body * clone ( const Body & body ) // Клонирование многогранника
{
    Body * res = new Body;
    copy ( body, BodyStor(), *res );
    return res;
}

// Максимальное скалярное произведение вершин на вектор v

Def<double> maxSP ( const Body & body, const Vector3d & v )
{
    if ( body.verts.size() == 0 ) return Def<double>();
    Show<Vert> show ( body.verts );
    double max = v * show.top()->point;
    while ( show.next() )
    {
        const double t = v * show.cur()->point;
        if ( max < t ) max = t;
    }
    return max;
}

// Минимальное скалярное произведение вершин на вектор v

Def<double> minSP ( const Body & body, const Vector3d & v )
{
    if ( body.verts.size() == 0 ) return Def<double>();
    Show<Vert> show ( body.verts );
    double min = v * show.top()->point;
    while ( show.next() )
    {
        const double t = v * show.cur()->point;
        if ( min > t ) min = t;
    }
    return min;
}

// Максимальное скалярное произведение вершин последней грани на вектор v

Def<double> maxSPLastFacet ( const Body & body, const Vector3d & v )
{
    if ( body.sides.size() == 0 || body.sides.las()->size() == 0 ) return Def<double>();
    Show<BonePtr> show ( *body.sides.las() );
    const Bone * b = show.top()->bone;
    double max = v * ( & b->b0 == show.cur() ? b->v0->point : b->v1->point );
    while ( show.next() )
    {
        b = show.cur()->bone;
        const double t = v * ( & b->b0 == show.cur() ? b->v0->point : b->v1->point );
        if ( max < t ) max = t;
    }
    return max;
}

// Минимальное скалярное произведение вершин последней грани на вектор v

Def<double> minSPLastFacet ( const Body & body, const Vector3d & v )
{
    if ( body.sides.size() == 0 || body.sides.las()->size() == 0 ) return Def<double>();
    Show<BonePtr> show ( *body.sides.las() );
    const Bone * b = show.top()->bone;
    double min = v * ( & b->b0 == show.cur() ? b->v0->point : b->v1->point );
    while ( show.next() )
    {
        b = show.cur()->bone;
        const double t = v * ( & b->b0 == show.cur() ? b->v0->point : b->v1->point );
        if ( min > t ) min = t;
    }
    return min;
}

// Проекция множества вершин на плоскость XY

DynArrRef<Vector2d> & projectVertexXY ( const List<Vert> & verts, DynArrRef<Vector2d> &  res )
{
    Show<Vert> show ( verts );
    if ( res.size() != show.size() ) res.resize ( show.size() );
    nat i = 0;
    if ( show.top() )
    do
    {
        const Vector3d & v = show.cur()->point;
        res[i++]= Vector2d ( v.x, v.y );
    }
    while ( show.next() );
    return res;
}

// Проекция множества  вершин на плоскость YZ после преобразования sim

DynArrRef<Vector2d> & projectVertexYZ ( const List<Vert> & verts, const Similar3d & sim, DynArrRef<Vector2d> & res )
{
    Show<Vert> show ( verts );
    if ( res.size() != show.size() ) res.resize ( show.size() );
    show.top();
    for ( nat j = 0; j < res.size(); ++j )
    {
        const Vector3d & u = show.cur()->point;
        res[j].x = sim.getY ( u );
        res[j].y = sim.getZ ( u );
        show.next();
    }
    return res;
}

// Проекция многогранника на плоскость YZ после преобразования sim
/*
void makeProjectionYZ ( const Body & body, const Similar3d & sim, ZOList & zol, List2p & pstor, OList & ostor )
{
    List2p list;
    Vector3d ax, ay, az;
    sim.getReper ( ax, ay, az );
    Show<Bone> bones ( body.bones );
    if ( bones.top() )
    do
    {
        const Bone & bone = *bones.cur();
        const bool x0 = ax * bone.s0->plane.norm > 0;
        const bool x1 = ax * bone.s1->plane.norm > 0;
        if ( x0 == x1 ) continue;
        Item2p * p = list.addNewAftLas ( pstor );
        if ( x0 )
        {
            p->a = bone.v0;
            p->b = bone.v1;
        }
        else
        {
            p->a = bone.v1;
            p->b = bone.v0;
        }
        p->info = bone.info;
    }
    while ( bones.next() );

    List< ListItem<List2p > > itog;
    makeClosed ( list, itog );
// Выбираем максимальный по площади многоугольник
    zol.movAllAftLas ( ostor );
    double max = 0;
    ZOList temp;
    if ( itog.top() )
    do
    {
        List2p * p = itog.cur();
        temp.resize ( p->size(), ostor ).end();
        p->top();
        do
        {
            const Vector3d & v = ( (const Vert*) p->cur()->a )->point;
            Otrezok * o1 = temp.cur();
            Otrezok * o2 = temp.cnext();
            o2->a.x = o1->b.x = sim.getY ( v );
            o2->a.y = o1->b.y = sim.getZ ( v );
            o2->info = p->cur()->info;
        }
        while ( p->next() );
        const double a = temp.square();
        if ( max < a )
        {
            max = a;
            swap ( zol, temp );
        }
        temp.movAllAftLas ( ostor );
        p->movAllAftLas ( pstor );
    }
    while ( itog.next() );
// Убираем петли, если они есть
    if ( zol.size() > 3 )
    {
        delSmallEdges ( zol, 1e-6 );
        if ( loopNumber ( zol ) != 1 )
        {
            doctor ( zol );
        }
    }
}

void makeProjectionYZ ( const Body & body, const Conform3d & conform, ZOList & zol, List2p & pstor, OList & ostor )
{
    makeProjectionYZ ( body, Similar3d ( conform ), zol, pstor, ostor );
}

DynArrRef<Vector2d> & projectYZ ( const Body & body, const Similar3d & sim, List2p & stor, DynArrRef<Vector2d> & proj )
{
    ZOList zol;
    makeProjectionYZ ( body, sim, zol, stor, OList() );
// Находим самую нижнюю точку и делаем её первой
    if ( zol.size() > 0 )
    {
        Otrezok * first = zol.top();
        while ( zol.next() )
        {
            Otrezok * o = zol.cur();
            if ( first->a.y > o->a.y ) first = o;
        }
        zol.makeFir ( first );
    }
    return copy ( zol, proj );
}

DynArrRef<Vector2d> & projectYZ ( const Body & body, const Conform3d & sim, List2p & stor, DynArrRef<Vector2d> & proj )
{
    return projectYZ ( body, Similar3d ( sim ), stor, proj );
}
*/
// Вычисление основных осей и моментов 2-го порядка

Def<Mom3d> momentum2plh ( const Body & body )
{
    const Def<Vector3d> o = centerOfMass ( body );
    if ( ! o.isDef ) return Def<Mom3d>();
    double xx, xy, xz, yy, yz, zz;
    xx = xy = xz = yy = yz = zz = 0.;
    Show<Side> sides ( body.sides );
    sides.top();
    do
    {
        const Side * s = sides.cur();
        if ( s->size() < 3 ) continue;
        Show<BonePtr> side ( * s );
        const Vector3d a = side.top()->vert0()->point - o;
        Vector3d b = side.next()->vert0()->point - o;
        const double d = s->plane % o;
        while ( side.next() )
        {
            const Vector3d c = side.cur()->vert0()->point - o;
            const double g = ( ( b - a ) % ( b - c ) * s->plane.norm ) * d / 120.;
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
    while ( sides.next() );
    return solve ( xx, xy, xz, yy, yz, zz, o );
}

void cut ( Body & body, const Plane3d & plane, int mark )
{
    body.cut ( plane, mark );
}

// Самоуничтожение

void delSelf ( Body * body ) { delete body; }

void swap ( Body & body1, Body & body2 )
{
    swap ( body1.sides, body2.sides );
    swap ( body1.bones, body2.bones );
    swap ( body1.verts, body2.verts );
}

Body & copy ( const Body & source, BodyStor & stor, Body & target )
{
    if ( & source == & target ) return target;
// Освобождение и выделение памяти
    target.makeVoid ( stor );
    target.sides.resize ( source.sides.size(), stor.sides );
    target.bones.resize ( source.bones.size(), stor.bones );
    target.verts.resize ( source.verts.size(), stor.verts );
    if ( ! target.verts.size() && ! target.bones.size() && ! target.sides.size() ) return target;
// Копия вершин
    Show<Vert> vshow ( source.verts );
    const nat nv = vshow.size();
    DynArray<SortItem<const Vert *, Vert *> > varr ( nv );
    target.verts.top();
    vshow.top();
    nat i;
    for ( i = 0; i < nv; ++i )
    {
        const Vert * v1 = vshow.cur();
        Vert * v2 = target.verts.cur();
        v2->point = v1->point;
        v2->info = v1->info;
        varr[i].head = v1;
        varr[i].tail = v2;
        target.verts.next();
        vshow.next();
    }
    quickSort123 ( varr );
// Копия рёбер
    Show<Bone> bshow ( source.bones );
    const nat nb = bshow.size();
    DynArray<SortItem<const BonePtr *, BonePtr *> > barr ( nb+nb );
    bshow.top();
    target.bones.top();
    for ( i = 0; i < nb; ++i )
    {
        const Bone * b1 = bshow.cur();
        Bone * b2 = target.bones.cur();
        const nat k0 = lasEqu123 ( varr, SortItem<const Vert *, Vert *> ( b1->v0 ) );
        b2->v0 = k0 < nv ? varr[k0].tail : 0;
        const nat k1 = lasEqu123 ( varr, SortItem<const Vert *, Vert *> ( b1->v1 ) );
        b2->v1 = k1 < nv ? varr[k1].tail : 0;
        b2->info = b1->info;
        b2->s0 = b2->s1 = 0;
        b2->b0.info = b2->b0.info;
        b2->b1.info = b2->b1.info;
        barr[i].head = & b1->b0;
        barr[i].tail = & b2->b0;
        barr[i+nb].head = & b1->b1;
        barr[i+nb].tail = & b2->b1;
        bshow.next();
        target.bones.next();
    }
    quickSort123 ( barr );
// Копия граней
    Show<Side> sshow ( source.sides );
    const nat ns = sshow.size();
    sshow.top();
    target.sides.top();
    for ( i = 0; i < ns; ++i )
    {
        const Side * s1 = sshow.cur();
        Side * s2 = target.sides.cur();
        s2->plane = s1->plane;
        s2->info = s1->info;
        s2->flag = s1->flag;
        Show<BonePtr> s ( *s1 );
        if ( s.top() )
        do
        {
            const BonePtr * b1 = s.cur();
            const nat k = lasEqu123 ( barr, SortItem<const BonePtr *, BonePtr *> ( b1 ) );
            if ( k < barr.size() )
            {
                BonePtr * b2 = barr[k].tail;
                s2->addAftCur ( b2 );
                ( & b1->bone->b0 == b1 ? b2->bone->s0 : b2->bone->s1 ) = s2;
            }
        }
        while ( s.next() );
        sshow.next();
        target.sides.next();
    }
    return target;
}

//****************** Построение геометрических фигур *********************//

Body & makeVoid ( Body & body ) // Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )
{
    return body.makeVoid();
}

Body & makeVoid ( Body & body, BodyStor & stor ) // Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )
{
    return body.makeVoid ( stor );
}
/*
Body & makeCube ( Body & body, double r ) // Куб ( r - половина стороны )
{
    return copy ( Polyhedron().makeCube ( r ), body );
}

Body & makeOctahedron ( Body & body, double r ) // Октаэдр  ( r - макс. значение координат вершин )
{
    return copy ( Polyhedron().makeOctahedron ( r ), body );
}

Body & makeTetrahedron ( Body & body, double r ) // Тетраэдр  ( r - макс. значение координат вершин )
{
    return copy ( Polyhedron().makeTetrahedron ( r ), body );
}

Body & makePrism ( Body & body, CArrRef<Vector2d> vert, double z ) // Призма
{
    return copy ( Polyhedron().makePrism ( vert, z ), body );
}

Body & makeSphere ( Body & body, double r, nat n ) // Сфера ( r - радиус, n - к-во вершин )
{
    return copy ( Polyhedron().makeSphere ( n, r ), body );
}

Body & makeCuboid ( Body & body, double x, double y, double z ) // Параллелепипед ( x, y, z - половины сторон )
{
    return copy ( Polyhedron().makeCuboid ( x, y, z ), body );
}

Body & makeEllipsoid ( Body & body, double x, double y, double z, nat n ) // Эллипсоид ( x, y, z - половины осей, n - к-во вершин )
{
    return copy ( Polyhedron().makeEllipsoid ( n, x, y, z ), body );
}
*/
//*********************** Преобразования многогранника *******************//

Body & operator *= ( Body & body, const Conform3d & conf )
{
    return body *= Similar3d ( conf );
}

Body & operator *= ( Body & body, const Spin3d & spin )
{
    return body *= Ortho3d ( spin );
}

//**************************** 21.10.2008 *********************************//
//
//        Получение выпуклой оболочки проекции модели на плоскость XY
//  factor - это допустимое изменение площади многоугольника при упрощении
//
//**************************** 20.11.2016 *********************************//
/*
SuiteRef<Vector2d> & convexProjectionXY ( const Body & body, double factor, SuiteRef<Vector2d> & res )
{
    projectVertexXY ( body.verts, res );
    return makeConvexPolygon ( factor, res );
}
*/
//**************************** 21.01.2011 *********************************//
//
//        Получение выпуклой оболочки проекции модели на плоскость YZ
//  factor - это допустимое изменение площади многоугольника при упрощении
//
//**************************** 20.11.2016 *********************************//
/*
SuiteRef<Vector2d> & convexProjectionYZ ( const Body & body, const Similar3d & sim, double factor, SuiteRef<Vector2d> & res )
{
    projectVertexYZ ( body.verts, sim, res );
    return makeConvexPolygon ( factor, res );
}

DynArrRef<Line2d> & convexProjectionYZ ( const Body & body, const Similar3d & sim, double factor, DynArrRef<Line2d> & res )
{
    Suite<Vector2d> vert;
    res.resize ( convexProjectionYZ ( body, sim, factor, vert ).size() );
    for ( nat i = 0; i < res.size(); ++i ) res[i] = Line2d ( vert[i], vert.cnext(i) );
    return res;
}
*/
//**************************** 11.05.2010 *********************************//
//
//              Обрезание многоугольника по краям
//
//**************************** 11.05.2010 *********************************//
/*
Body & circumcision ( Body & body, const ZOList & zol, double eps )
{
    Show<Otrezok> show ( zol );
    if ( show.top() )
    do
    {
        const Otrezok * o = show.cur();
        if ( norm1 ( o->b - o->a ) < eps ) continue;
        const Line2d line ( o->a, o->b );
        body.cut ( Plane3d ( Vector3d ( line.norm.x, line.norm.y, 0 ), line.dist ) );
    }
    while ( show.next() );
    return body;
}

nat controlSum1 ( const Body & body )
{
    nat res = 0;
    const nat * p;
    Show<Vert> verts ( body.verts );
    if ( verts.top () )
    do
    {
        p = ( const nat * ) & verts.cur()->point.x;
        res += p[0] + p[1];
        p = ( const nat * ) & verts.cur()->point.y;
        res += p[0] + p[1];
        p = ( const nat * ) & verts.cur()->point.z;
        res += p[0] + p[1];
    }
    while ( verts.next () );
    return res;
}

nat controlSum2 ( const Body & body )
{
    nat res = 0;
    Show<Side> sides ( body.sides );
    if ( sides.top () )
    do
    {
        res += sides.cur()->info;
    }
    while ( sides.next () );
    return res;
}

nat controlSum3 ( const Body & body )
{
    nat res = 0;
    const nat * p;
    Show<Side> sides ( body.sides );
    if ( sides.top () )
    do
    {
        const Plane3d & plane = sides.cur()->plane;
        p = ( const nat * ) & plane.norm.x;
        res += p[0] + p[1];
        p = ( const nat * ) & plane.norm.y;
        res += p[0] + p[1];
        p = ( const nat * ) & plane.norm.z;
        res += p[0] + p[1];
        p = ( const nat * ) & plane.dist;
        res += p[0] + p[1];
    }
    while ( sides.next () );
    return res;
}
*/
//**************************** 16.03.2006 ****************************
//
//          Проверка правильности объекта типа Body
//
//**************************** 16.03.2006 ****************************

void check ( const Body & body, IBodyErrorRecipient & ber )
{
    ber.start();
    const char * str = body.verts.test();
    if ( str != 0 )
    {
        ber.vertList ( str );
    }

    str = body.bones.test();
    if ( str != 0 )
    {
        ber.boneList ( str );
    }
    else
    {
        Show<Bone> bones ( body.bones );
        if ( bones.top() )
        do
        {
            const Bone * bone = bones.cur();
            if ( ! bone->s0 || ! bone->s1 ) ber.boneHasNullSide ( bone );
            if ( bone->s0 == bone->s1 ) ber.boneHasEqualSides ( bone );
            if ( bone->v0 == bone->v1 ) ber.boneIsNull ( bone );
            if ( bone->s0 && ! bone->s0->has ( & bone->b0 ) ) ber.sideHasNotBone ( bone->s0, bone );
            if ( bone->s1 && ! bone->s1->has ( & bone->b1 ) ) ber.sideHasNotBone ( bone->s1, bone );
            if ( ! body.verts.has ( bone->v0 ) ) ber.boneHasOutVert ( bone, bone->v0 );
            if ( ! body.verts.has ( bone->v1 ) ) ber.boneHasOutVert ( bone, bone->v1 );
        }
        while ( bones.next() );
    }

    str = body.sides.test();
    if ( str != 0 )
    {
        ber.sideList ( str );
    }
    else
    {
        Suite<Vector2d> vert2d;
        Show<Side> sides ( body.sides );
        const double tol = ber.getTolerance();
        const double minArea = ber.getMinAdmArea();
        if ( sides.top() )
        do
        {
            const Side * s = sides.cur();
            Show<BonePtr> side ( * s );
            if ( ! side.end() ) continue;
            const Bone * b = side.cur()->bone;
            const Vert * v1 = & b->b1 == side.cur() ? b->v0 : b->v1;
            OrthoFunc3to2 trans ( s->plane.norm );
            vert2d.resize();
            side.top();
            do
            {
                const Bone * b = side.cur()->bone;
                if ( & b->b0 == side.cur() )
                {
                    if ( b->s0 != s ) ber.sideHasNotInBone ( s, b );
                    const Vert * v0 = b->v0;
                    if ( v1 != v0 ) ber.sideHasNotOrder ( s, v1, v0 );
                    v1 = b->v1;
                    const double d = fabs ( s->plane % v0->point );
                    if ( d > tol ) ber.putDeviation ( s, v0, d );
                    vert2d.inc() = trans ( v0->point );
                }
                else
                {
                    if ( b->s1 != s ) ber.sideHasNotInBone ( s, b );
                    const Vert * v0 = b->v1;
                    if ( v1 != v0 ) ber.sideHasNotOrder ( s, v1, v0 );
                    v1 = b->v0;
                    const double d = fabs ( s->plane % v0->point );
                    if ( d > tol ) ber.putDeviation ( s, v0, d );
                    vert2d.inc() = trans ( v0->point );
                }
            }
            while ( side.next() );
            const double area = s->area();
            if ( area < minArea )
            {
                ber.putSmallArea ( s, area );
            }
            if ( vert2d.size() > 2 )
            {
                const int ln = loopNumber ( vert2d );
                if ( ln != 1 || ! trianTestNat1L1MinTan ( vert2d ) )
                {
                    ber.sideIntersect ( s );
                }
            }
        }
        while ( sides.next() );
    }
    const double vol = body.volume();
    if ( vol < 0 ) ber.volumeIsNeg ( vol );
    ber.finish();
}

namespace
{

class BodyErrorRecipient : public IBodyErrorRecipient
{
    double maxDev, minArea;
    const double tolerance, minAdmArea;
    const char * vertListS, * boneListS, * sideListS;
    nat deviationN, smallAreaN, boneIsNullN, boneHasNullSideN, boneHasEqualSidesN, 
        boneHasOutVertN, sideHasNotBoneN, sideHasNotOrderN, sideHasNotInBoneN, sideIntersectN;
public:

    BodyErrorRecipient ( double tol, double area ) : tolerance (tol), minAdmArea (area)
    {
    }

    virtual void start ()
    {
        maxDev = 0.;
        minArea = minAdmArea;
        vertListS = boneListS = sideListS = 0;
        deviationN = smallAreaN =
        boneIsNullN = boneHasNullSideN = boneHasEqualSidesN = boneHasOutVertN = 
        sideHasNotBoneN = sideHasNotOrderN = sideHasNotInBoneN = smallAreaN = sideIntersectN = 0;
    }
    virtual double getTolerance ()
    {
        return tolerance;
    }
    virtual double getMinAdmArea ()
    {
        return minAdmArea;
    }
    virtual void putDeviation ( const Side * side, const Vert * vert, double d )
    {
        if ( maxDev < d ) maxDev = d;
        ++deviationN;
    }
    virtual void putSmallArea ( const Side * side, double a )
    {
//display << side->size() << a << NL;
        if ( minArea > a ) minArea = a;
        ++smallAreaN;
    }
    virtual void vertList ( const char * str )
    {
        vertListS = str;
    }
    virtual void boneList ( const char * str )
    {
        boneListS = str;
    }
    virtual void sideList ( const char * str )
    {
        sideListS = str;
    }
    virtual void boneIsNull ( const Bone * bone )
    {
        ++boneIsNullN;
    }
    virtual void boneHasNullSide ( const Bone * bone )
    {
        ++boneHasNullSideN;
    }
    virtual void boneHasEqualSides ( const Bone * bone )
    {
        ++boneHasEqualSidesN;
    }
    virtual void boneHasOutVert ( const Bone * bone, const Vert * vert )
    {
        ++boneHasOutVertN;
    }
    virtual void sideHasNotBone ( const Side * side, const Bone * bone )
    {
        ++sideHasNotBoneN;
    }
    virtual void sideHasNotOrder ( const Side * side, const Vert * v0, const Vert * v1 )
    {
        ++sideHasNotOrderN;
    }
    virtual void sideHasNotInBone ( const Side * side, const Bone * bone )
    {
        ++sideHasNotInBoneN;
    }
    virtual void sideIntersect ( const Side * side )
    {
        ++sideIntersectN;
    }
    virtual void volumeIsNeg ( double vol )
    {
        display << "volume =" << vol << NL;
    }
    virtual void finish ()
    {
        if ( vertListS != 0 )
        {
            display << "vertList" << vertListS << NL;
        }
        if ( boneListS != 0 )
        {
            display << "boneList" << boneListS << NL;
        }
        if ( sideListS != 0 )
        {
            display << "sideList" << sideListS << NL;
        }

        if ( boneIsNullN != 0 )
        {
            display << "boneIsNull" << boneIsNullN << NL;
        }
        if ( boneHasNullSideN != 0 )
        {
            display << "boneHasNullSide" << boneHasNullSideN << NL;
        }
        if (boneHasEqualSidesN != 0 )
        {
            display << "boneHasEqualSides" << boneHasEqualSidesN << NL;
        }
        if ( boneHasOutVertN != 0 )
        {
            display << "boneHasOutVert" << boneHasOutVertN << NL;
        }
        if ( sideHasNotBoneN != 0 )
        {
            display << "sideHasNotBone" << sideHasNotBoneN << NL;
        }
        if ( sideHasNotOrderN != 0 )
        {
            display << "sideHasNotOrder" << sideHasNotOrderN << NL;
        }
        if ( sideHasNotInBoneN != 0 )
        {
            display << "sideHasNotInBone" << sideHasNotInBoneN << NL;
        }
        if ( sideIntersectN != 0 )
        {
            display << "sideIntersect" << sideIntersectN << NL;
        }
        if ( deviationN != 0 )
        {
            display << "deviationN =" << deviationN << "maxDeviation =" << maxDev << NL;
        }
        if ( smallAreaN != 0 )
        {
            display << "smallAreaN =" << smallAreaN << "minArea =" << minArea << NL;
        }
        display << "end test" << NL;
    }
};

} // namespace

void test ( const Body & body, double tol, double area )
{
    BodyErrorRecipient ber ( tol, area );
    check ( body, ber );
}

nat testBones ( Body & body )
{
    nat i = 0;
    if ( body.verts.top() )
    do
    {
        body.verts.cur()->info = i++;
    }
    while ( body.verts.next() );
    Suite<int> edge ( 2 * body.bones.size() );
    if ( body.sides.top() )
    do
    {
        Side * s = body.sides.cur();
        if ( s->top() )
        do
        {
            edge.inc() = ( s->vert1()->info << 16 ) + s->vert0()->info;
        }
        while ( s->next() );
    }
    while ( body.sides.next() );
    if ( edge.size() < 2 ) return 0;
    quickSort123 ( edge );
    nat n = 0;
    for ( nat j = 1; j < edge.size(); ++j )
    {
        if ( edge[j] == edge[j-1] )
        {
            ++n;
        }
    }
/*    if ( body.sides.top() )
    do
    {
        Side * s = body.sides.cur();
        if ( s->size() < 2 ) continue;
        s->end();
        Vert * vp = s->vert0();
        s->top();
        do
        {
            Vert * vn = s->vert1();
            if ( vp == vn )
                n = n;
            vp = s->vert0();
        }
        while ( s->next() );
    }
    while ( body.sides.next() );*/
    return n;
}

void clearSide2 ( Body & body, Side * s )
{
m1: s->top();
    BonePtr * p = s->cur();
    s->delCur();
    Bone * b = s->cur()->bone;
    Side * a = s->another();
    if(s->index())
        a->jump(&b->b0);
    else
        a->jump(&b->b1);
    s->delCur();
    if(a->curIsLas())
    {
        a->delCur();
        a->addAftCur(p);
    }
    else
    {
        a->delCur();
        a->addBefCur(p);
    }
    a->setCurSelf();
    Bone * bone = p->bone;
    body.bones.del(b);
    if ( bone == b )
        return;
    if ( bone->s0 == bone->s1 && a->has ( &bone->b0 ) && a->has ( &bone->b1 ) )
    {
        a->jump(&bone->b0).makeCurFir();
        a->delCur();
        for(;;)
        {
            if ( a->cur() == &bone->b1 )
            {
                a->delCur();
                break;
            }
            a->movFirAftLas ( *s );
        }
        if ( s->size() > a->size() ) swap ( *a, *s );
//display<<s->size() << a->size()<<NL;
        if ( s->top() )
        do
        {
            s->setCurSelf();
        }
        while ( s->next() );
        s->plane = a->plane;
        body.bones.del(bone);
        if ( s->size() == 2 ) goto m1;
    }
}

void clearVerts ( Body & body )
{
    body.verts.setAllInfo ( 0 );
    if ( body.bones.top() )
    do
    {
        ++ body.bones.cur()->v0->info;
        ++ body.bones.cur()->v1->info;
    }
    while ( body.bones.next() );
////////////// Удаление вершин с 2 ребрами ( 12.11.99 ) ///////////////
    Suite<Side *> side2;
    if ( body.bones.top() )
    do
    {
        Bone * b = body.bones.cur();
        if ( ! b->s0 || ! b->s1 )
        {
            continue;
        }
        if ( b->v0->info == 2 )
        {
            b->v0->info = 0;
            b->s0->jump(&b->b0);
            b->s1->jump(&b->b1);
            b->s1->cnext();
            b->v0 = b->s1->vert1();
            Bone * t = b->s1->cur()->bone;
            t->s0->jump(&t->b0);
            t->s0->delCur();
            t->s1->jump(&t->b1);
            t->s1->delCur();
            body.bones.jump(t);
            body.bones.delCur();
            body.bones.jump(b);
            if ( b->s0->size() == 2 ) side2.inc() = b->s0;
            if ( b->s1->size() == 2 ) side2.inc() = b->s1;
        }
        if ( b->v1->info == 2 ) // 28.04.2006
        {
            b->v1->info = 0;
            b->s1->jump(&b->b1);
            b->s0->jump(&b->b0);
            b->s0->cnext();
            b->v1 = b->s0->vert1();
            Bone * t = b->s0->cur()->bone;
            t->s0->jump(&t->b0);
            t->s0->delCur();
            t->s1->jump(&t->b1);
            t->s1->delCur();
            body.bones.jump(t);
            body.bones.delCur();
            body.bones.jump(b);
            if ( b->s0->size() == 2 ) side2.inc() = b->s0;
            if ( b->s1->size() == 2 ) side2.inc() = b->s1;
        }
    }
    while ( body.bones.next() );
// Удаление граней с двумя рёбрами 07.05.2011
    for ( nat i = 0; i < side2.size(); ++i ) 
    {
        Side * s = side2[i];
        if ( body.sides.has ( s ) )
        {
            if ( s->size() == 2 )
                clearSide2 ( body, s );
            if ( s->size() == 0 )
            {
                body.sides.jump ( s );
                body.sides.delCur();
            }
        }
    }
////////////////////////////////////////////////////////////////////////
    if ( body.verts.top() )
    for(;;)
    {
        if ( ! body.verts.cur()->info )
        {
            if ( body.verts.curIsLas() )
            {
                body.verts.delCur();
                break;
            }
            else
            {
                body.verts.delCur();
                continue;
            }
        }
        if ( ! body.verts.next() ) break;
    }
}

void clearBones ( Body & body )
{
    body.bones.setAllInfo ( 0 );
    if ( body.sides.top() )
    do
    {
        Side * s = body.sides.cur();
        if ( s->top() )
        do
        {
            ++ s->cur()->bone->info;
        }
        while ( s->next() );
    }
    while ( body.sides.next() );

    if ( body.bones.top() )
    for(;;)
    {
        Bone * bone = body.bones.cur();
        if ( ! bone->info )
        {
            if ( body.bones.curIsLas() )
            {
                body.bones.delCur();
                break;
            }
            else
            {
                body.bones.delCur();
                continue;
            }
        }
        Side * s0 = bone->s0;
        if ( s0 == bone->s1 && s0->has ( &bone->b0 ) && s0->has ( &bone->b1 ) )
        {
            if ( s0->jump(&bone->b0).cnext() == &bone->b1 )
            {
                s0->jump(&bone->b0).delCur();
                s0->jump(&bone->b1).delCur();
//display << s0->size() << NL;
            }
            else
            if ( s0->jump(&bone->b1).cnext() == &bone->b0 )
            {
                s0->jump(&bone->b0).delCur();
                s0->jump(&bone->b1).delCur();
//display << s0->size() << NL;
            }
            else
            {
                s0->jump(&bone->b0).makeCurFir();
                s0->delCur();
                Side * s1 = new Side ( s0->plane );
                while ( s0->cur() != &bone->b1 )
                {
                    s0->movCurAftCur ( *s1 );
                    s1->setCurSelf();
                }
                s0->delCur();
                body.sides.addAftLas ( s1 );
//display << s0->size() << s1->size() << NL;
            }
            if ( body.bones.curIsLas() )
            {
                body.bones.delCur();
                break;
            }
            else
            {
                body.bones.delCur();
                continue;
            }
        }
        if ( ! body.bones.next() ) break;
    }
}

//**************************** 04.02.2014 ****************************
//
//    Удаление маленьких рёбер ( возвращает к-во удалённых рёбер )
//
//**************************** 07.03.2014 ****************************

nat delSmallBones ( Body & body, double eps )
{
    eps = eps > 0 ? eps * eps : 0;
    BoneList bones;
    nat count = 0;
    for(;;)
    {
// Поск минимального ребра
        Bone * b = 0;
        double min = eps;
        if ( body.bones.top() )
        do
        {
            Bone * bone = body.bones.cur();
            const double t = qmod ( bone->v0->point - bone->v1->point );
            if ( min > t ) min = t, b = bone;
        }
        while ( body.bones.next() );
        if ( ! b ) return count;
        ++count;
        Vert * v0 = b->v0;
        Vert * v1 = b->v1;
        if ( min > 0 ) v0->point = 0.5 * ( v0->point + v1->point );
// Удаляем одну вершину
        if ( v0 != v1 && body.verts.has ( v1 ) )
            body.verts.del ( v1 );
// Удаляем нулевые рёбра из списка рёбер
        body.bones.top();
        for(;;)
        {
            Bone * bone = body.bones.cur();
            if ( bone->v0 == v1 ) bone->v0 = v0;
            if ( bone->v1 == v1 ) bone->v1 = v0;
            if ( bone->v0 == bone->v1 )
            {
                if ( body.bones.curIsLas() )
                {
                    body.bones.movCurAftLas ( bones );
                    break;
                }
                else
                {
                    body.bones.movCurAftLas ( bones );
                    continue;
                }
            }
            if ( ! body.bones.next() ) break;
        }
// Удаляем нулевые рёбра из граней
        if ( body.sides.top() )
        for(;;)
        {
            Side * s = body.sides.cur();
            if ( s->top() )
            for(;;)
            {
                Bone * bone = s->cur()->bone;
                if ( bone->v0 == bone->v1 )
                {
                    if ( s->curIsLas() )
                    {
                        s->delCur();
                        break;
                    }
                    else
                    {
                        s->delCur();
                        continue;
                    }
                }
                if ( ! s->next() ) break;
            }
            if ( s->size() == 0 )
            {
                if ( body.sides.curIsLas() )
                {
                    body.sides.delCur();
                    break;
                }
                else
                {
                    body.sides.delCur();
                    continue;
                }
            }
            if ( ! body.sides.next() ) break;
        }
    }
}

//**************************** 02.11.2011 ****************************
//
//              Исправление направления нормали граней
//
//**************************** 02.11.2011 ****************************

void checkNormal ( Body & body )
{
    if ( body.sides.top() )
    do
    {
        Side * s = body.sides.cur();
        if ( s->size() > 2 && s->area() < 0 )
            s->plane = - s->plane;
    }
    while ( body.sides.next() );
}

//**************************** 02.11.2011 ****************************
//
//                     Разбиение граней на части
//
//**************************** 02.11.2011 ****************************

void divideSides ( Body & body )
{
    if ( body.sides.top() )
    do
    {
        Side * s = body.sides.cur();
m1:     if ( s->size() < 4 ) continue;
        s->top();
        for(;;)
        {
            BonePtr * p = s->cur();
            Vert * a = s->vert0();
            while ( s->next() )
            {
                Vert * b = s->vert0();
                if ( a == b )
                {
                    Side * d = new Side ( s->plane );
                    body.sides.addAftLas ( d );
                    s->jump ( p );
                    for(;;)
                    {
                        s->movCurAftCur ( *d );
                        d->setCurSelf();
                        if ( s->vert0() == b ) break;
                    }
                    goto m1;
                }
            }
            s->jump(p).next();
            if ( s->curIsLas() ) break;
        }
    }
    while ( body.sides.next() );
}

//**************************** 26.10.2011 ****************************
//
//                  Удаление узких выбросов на гранях
//
//**************************** 26.10.2011 ****************************
/*
void test ( Side * s0 )
{
    s0->end();
    const Bone * b = s0->cur()->bone;
    const Vert * v1 = & b->b1 == s0->cur() ? b->v0 : b->v1;
    s0->top();
    do
    {
        const Bone * b = s0->cur()->bone;
        if ( & b->b0 == s0->cur() )
        {
            const Vert * v0 = b->v0;
            if ( v1 != v0 )
                return;
            v1 = b->v1;
        }
        else
        {
            const Vert * v0 = b->v1;
            if ( v1 != v0 )  
                return;
            v1 = b->v0;
        }
    }
    while ( s0->next() );
}
*/
void correctSides ( Body & body, double eps )
{
    if ( body.sides.top() )
    do
    {
        Side * s = body.sides.cur();
m1:     if ( s->size() < 3 ) continue;
        s->end();
        Side * sa = s->another();
        Vert * pa = s->vert0();
        Vert * pb = s->vert1();
        Vector3d va = pb->point - pa->point;
        double aa = va * va;
        s->top();
        do
        {
            Vert * pc = s->vert1();
            if ( pa == pc )
            {
                BonePtr * p2 = s->cur();
                Bone * bone2 = p2->bone;
                BonePtr * p1 = s->cprev();
                Bone * bone1 = p1->bone;
                if ( bone1 == bone2 )
                {
                    s->del ( p2 );
                }
                else
                {
                    if ( p1 == &bone1->b0 )
                    {
                        bone1->s1->jump ( &bone1->b1 ).makeCurFir();
                        bone1->s1->delCur();
                        s->jump ( p2 ).movCurBefCur ( *bone1->s1 );
                        bone1->s1->setCurSelf();
                    }
                    else
                    {
                        bone1->s0->jump ( &bone1->b0 ).makeCurFir();
                        bone1->s0->delCur();
                        s->jump ( p2 ).movCurBefCur ( *bone1->s0 );
                        bone1->s0->setCurSelf();
                    }
                }
                s->del ( p1 );
                body.bones.del ( bone1 );
                goto m1;
            }
            const Vector3d vb = pc->point - pb->point;
            const double bb = vb * vb;
            Side * sb = s->another();
            if ( sa == sb )
            {
                Bone * bone1 = s->cur()->bone;
                int i1 = s->index();
                Bone * bone2 = s->cprev()->bone;
                int i2 = s->index();
                if ( sa->size() == 2 )
                {
                    sa->delAll();
                    body.sides.del ( sa );
                    if ( s->curIsLas() )
                    {
                        s->delLas();
                        s->delFir();
                    }
                    else
                    {
                        s->delCur();
                        s->delCur();
                    }
//test ( s );
                    body.bones.del(bone1);
                    body.bones.del(bone2);
                    goto m1;
                }
                if ( sa->top() )
                do
                {
                    if ( sa->cur()->bone != bone1 ) continue;
                    if ( sa->cnext()->bone == bone2 )
                    {
                        sa->delCur();
                        s->delCur();
                        ( i1 == 0 ? bone1->v0 : bone1->v1 ) = ( i2 == 0 ? bone2->v0 : bone2->v1 );
//test ( s );
                        body.bones.del(bone2);
                        goto m1;
                    }
                    break;
                }
                while ( sa->next() );
                s->cnext();
            }
            else
            if ( va * vb < 0 && qmod ( va % vb ) < 0.01 * aa * bb )
            {
                if ( aa > bb )
                {
                    if ( sa != 0 && fabs ( sa->plane % pc->point ) < eps )
                    {
                        s->cprev();
                        Bone * bone1 = s->cur()->bone;
                        if ( sa->top() )
                        do
                        {
                            if ( sa->cur()->bone != bone1 ) continue;
                            ( s->index() == 0 ? bone1->v1 : bone1->v0 ) = pc;
                            s->cnext();
                            s->movCurBefCur ( *sa );
                            sa->setCurSelf();
//test ( s );
                            goto m1;
                        }
                        while ( sa->next() );
                        s->cnext();
                    }
                }
                else
                {
                    if ( sb != 0 && fabs ( sb->plane % pa->point ) < eps )
                    {
                        Bone * bone1 = s->cur()->bone;
                        if ( sb->top() )
                        do
                        {
                            if ( sb->cur()->bone != bone1 ) continue;
                            ( s->index() == 0 ? bone1->v0 : bone1->v1 ) = pa;
                            s->cprev();
                            s->movCurAftCur ( *sb );
                            sb->setCurSelf();
//test ( s );
                            goto m1;
                        }
                        while ( sb->next() );
                    }
                }
            }
            pa = pb;
            pb = pc;
            va = vb;
            aa = bb;
            sa = sb;
        }
        while ( s->next() );
    }
    while ( body.sides.next() );
}

//********************************************************************
//
//                  Удаление вырожденных граней
//
//********************************************************************

void clearSide3 ( Body & body, Side * s, double eps )
{
    s->top();
    Side * sc = s->another();
    const Vector3d & va = s->vert0()->point;
    s->next();
    Side * sa = s->another();
    const Vector3d & vb = s->vert0()->point;
    s->next();
    Side * sb = s->another();
    if ( sa == sb || sb == sc || sc == sa )
        return;
    const Vector3d & vc = s->vert0()->point;
    const double dc = qmod ( va - vb );
    const double da = qmod ( vb - vc );
    const double db = qmod ( vc - va );
    Bone * bone;
    if ( da >= db && da >= dc )
    {
        if ( fabs ( sa->plane % va ) > eps ) return;
//display<<"1"<<fabs ( sa->plane % va )<<NL;
        bone = s->prev()->bone;
        BonePtr * p = bone->s0 == sa ? &bone->b0 : &bone->b1;
        sa->jump ( p );
        s->movLasAftCur ( *sa );
        sa->setCurSelf();
        s->movFirAftCur ( *sa );
        sa->setCurSelf();
        sa->del(p);
        if ( sa->area() < 0 ) sa->plane = - sa->plane;
    }
    else
    if ( db >= dc )
    {
        if ( fabs ( sb->plane % vb ) > eps ) return;
//display<<"2"<<fabs ( sb->plane % vb )<<NL;
        bone = s->cur()->bone;
        BonePtr * p = bone->s0 == sb ? &bone->b0 : &bone->b1;
        sb->jump ( p );
        s->movFirAftCur ( *sb );
        sb->setCurSelf();
        s->movFirAftCur ( *sb );
        sb->setCurSelf();
        sb->del(p);
        if ( sb->area() < 0 ) sb->plane = - sb->plane;
    }
    else
    {
        if ( fabs ( sc->plane % vc ) > eps ) return;
//display<<"3"<<fabs ( sc->plane % vc )<<NL;
        bone = s->fir()->bone;
        BonePtr * p = bone->s0 == sc ? &bone->b0 : &bone->b1;
        sc->jump ( p );
        s->prev();
        s->movCurAftCur ( *sc );
        sc->setCurSelf();
        s->movLasAftCur ( *sc );
        sc->setCurSelf();
        sc->del(p);
        if ( sc->area() < 0 ) sc->plane = - sc->plane;
    }
    body.bones.del(bone);
    s->delAll();
}

void clearSides ( Body & body )
{
    if ( body.sides.top() )
    for(;;)
    {
        Side * s = body.sides.cur();
        if ( s->size() <= 3 )
        {
            if ( s->size() == 3 )
            {
                clearSide3 ( body, s, 1e-9 );
            }
            else
            if ( s->size() == 2 )
            {
                clearSide2 ( body, s );
            }
            else
            if ( s->size() == 1 )
            {
                Side * a = s->another();
                Bone * b = s->cur()->bone;
                b->s0->del(&b->b0);
                b->s1->del(&b->b1);
                body.bones.del(b);
                body.sides.jump(a).movCurAftLas(body.sides);
                body.sides.jump(s);
            }
            if ( s->size() == 0 )
            {
                if ( body.sides.curIsLas() )
                {
                    body.sides.delCur();
                    break;
                }
                else
                {
                    body.sides.delCur();
                    continue;
                }
            }
        }
        if ( ! body.sides.next() ) break;
    }
}

nat calcParts ( Body & body )
{
    if ( body.sides.size() < 2 ) return body.sides.size();
    body.sides.for_each123 ( Info = 0 );
    Suite<Side*> suite;
    int mark = 0;
    for(;;)
    {
        Side * s = ( Side * ) body.sides.fir ( Info == 0 );
        if ( s == 0 ) break;
        s->info = ++mark;
        suite.inc() = s;
        while ( suite.size() > 0 )
        {
            Side * s = suite[0];
            if ( s->top() )
            do
            {
                Side * a = s->another();
                if ( a == 0 || a->info > 0 ) continue;
                a->info = mark;
                suite.inc() = a;
            }
            while ( s->next() );
            suite.del ( 0 );
        }
    }
    return mark;
}

void clearParts ( Body & body )
{
    if ( body.sides.size() < 2 ) return;
    DynArray< SortItem<int,Side*> > arr ( body.sides.size() );
//    if ( mark < 2 ) return;
}

namespace
{ 

void del3Side ( Body & body, Side * side, const Vector3d & p )
{
    Side * side1, * side2, * side3;

    BonePtr * b1 = side->top();
    Bone * bone1 = b1->bone;
    const bool i1 = bone1->s0 == side;
    if ( i1 )
    {
        side1 = bone1->s1;
        if ( ( ( bone1->v0->point - p ) % ( bone1->v1->point - p ) ) * side1->plane.norm < 0 ) return;
        side1->jump ( & bone1->b1 );
    }
    else
    {
        side1 = bone1->s0;
        if ( ( ( bone1->v0->point - p ) % ( bone1->v1->point - p ) ) * side1->plane.norm > 0 ) return;
        side1->jump ( & bone1->b0 );
    }

    BonePtr * b2 = side->next();
    Bone * bone2 = b2->bone;
    const bool i2 = bone2->s0 == side;
    if ( i2 )
    {
        side2 = bone2->s1;
        if ( ( ( bone2->v0->point - p ) % ( bone2->v1->point - p ) ) * side2->plane.norm < 0 ) return;
        side2->jump ( & bone2->b1 );
    }
    else
    {
        side2 = bone2->s0;
        if ( ( ( bone2->v0->point - p ) % ( bone2->v1->point - p ) ) * side2->plane.norm > 0 ) return;
        side2->jump ( & bone2->b0 );
    }

    BonePtr * b3 = side->next();
    Bone * bone3 = b3->bone;
    const bool i3 = bone3->s0 == side;
    if ( i3 )
    {
        side3 = bone3->s1;
        if ( ( ( bone3->v0->point - p ) % ( bone3->v1->point - p ) ) * side3->plane.norm < 0 ) return;
        side3->jump ( & bone3->b1 );
    }
    else
    {
        side3 = bone3->s0;
        if ( ( ( bone3->v0->point - p ) % ( bone3->v1->point - p ) ) * side3->plane.norm > 0 ) return;
        side3->jump ( & bone3->b0 );
    }

    side->delAll();
    body.sides.del(side);

    side1->addAftCur(b3);
    side2->addAftCur(b1);
    side3->addAftCur(b2);

    Vert * vert = new Vert ( p );
    body.verts.addAftLas ( vert );
    if(i1)
    {
        bone1->v0 = vert;
        bone1->s0 = side2;
    }
    else
    {
        bone1->v1 = vert;
        bone1->s1 = side2;
    }
    if(i2)
    {
        bone2->v0 = vert;
        bone2->s0 = side3;
    }
    else
    {
        bone2->v1 = vert;
        bone2->s1 = side3;
    }
    if(i3)
    {
        bone3->v0 = vert;
        bone3->s0 = side1;
    }
    else
    {
        bone3->v1 = vert;
        bone3->s1 = side1;
    }
}

void calc ( Vector3d & norm, Side * s )
{
    s->top();
    do
    {
        norm += s->vert0()->point % s->vert1()->point;
    }
    while(s->next());
}

void calc ( const Vector3d & norm, Side * s, double & max, double & min )
{
    s->top();
    do
    {
        double t = norm * s->vert0()->point;
        if ( t > max ) max = t;
        if ( t < min ) min = t;
    }
    while(s->next());
}

class SideCluster : public ListItem< List<Side> >
{
public:
    Vector3d vector;
};

struct SideInfo
{
    Vector3d      vector;
    SideCluster * cluster;
    SideInfo () : cluster(0), vector(0,0,0) {}
};

typedef SortItem < double, Set2<Vector3d, Side*> > DistVertItem;

} // namespace

void minimize3Sides ( Body & body, double eps )
{
    Suite<DistVertItem> list;
    if ( body.sides.top() )
    do
    {
        Side * side = body.sides.cur();
        if ( side->size() != 3 ) continue;
        side->top();
        const Side * side1 = side->another();
        if ( side1 == 0 ) continue;
        side->next();
        const Side * side2 = side->another();
        if ( side2 == 0 || side1 == side2 ) continue;
        side->next();
        const Side * side3 = side->another();
        if ( side3 == 0 || side2 == side3 || side3 == side1 ) continue;
        const Def<Vector3d> v = intersection ( side1->plane, side2->plane, side3->plane );
        if ( ! v.isDef ) continue;
        if ( fabs ( side1->plane % v ) > 1e-8 ) continue;
        if ( fabs ( side2->plane % v ) > 1e-8 ) continue;
        if ( fabs ( side3->plane % v ) > 1e-8 ) continue;
        const double h = fabs ( side->plane % v );
        if ( h > eps ) continue;
        Vector3d vx, vy;
        reper ( side->plane.norm, vx, vy );
        side->top();
        const Vector3d va = side->vert0()->point - v;
        const Vector2d a ( vx * va, vy * va );
        side->next();
        const Vector3d vb = side->vert0()->point - v;
        const Vector2d b ( vx * vb, vy * vb );
        side->next();
        const Vector3d vc = side->vert0()->point - v;
        const Vector2d c ( vx * vc, vy * vc );
        if ( a % b < 0 || b % c < 0 || c % a < 0 ) continue;
        DistVertItem & m = list.inc();
        m.head = h;
        m.tail.a = v;
        m.tail.b = side;
    }
    while ( body.sides.next() );
    quickSort123 ( list );
    for ( nat i = 0; i < list.size(); ++i )
    {
        Side * side = list[i].tail.b;
        if ( side->size() == 3 )
        {
            del3Side ( body, side, list[i].tail.a );
        }
    }
}

void minimize4Sides ( Body & body, double eps )
{
    int n1 = 0;
    int n2 = 0;
    if ( body.sides.top() )
    do
    {
        Side * side = body.sides.cur();
        if ( side->size() != 4 ) continue;
++n1;
        const Plane3d & plane = side->plane;
        side->top();
        Side * side1 = side->another();
        const Vector3d v1 = side->vert0()->point;
        side->next();
        Side * side2 = side->another();
        if ( side2 == side1 ) continue;
        const Vector3d v2 = side->vert0()->point;
        side->next();
        Side * side3 = side->another();
        if ( side3 == side1 || side3 == side2 ) continue;
        const Vector3d v3 = side->vert0()->point;
        side->next();
        Side * side4 = side->another();
        if ( side4 == side1 || side4 == side2 || side4 == side3 ) continue;
        const Vector3d v4 = side->vert0()->point;
        const Vector3d u1 = ( v1 - v2 ) % ( v2 - v3 );
        const Vector3d u2 = ( v2 - v3 ) % ( v3 - v4 );
        const Vector3d u3 = ( v3 - v4 ) % ( v4 - v1 );
        const Vector3d u4 = ( v4 - v1 ) % ( v1 - v2 );
        if ( u1 * plane.norm < 0 ) continue;
        if ( u2 * plane.norm < 0 ) continue;
        if ( u3 * plane.norm < 0 ) continue;
        if ( u4 * plane.norm < 0 ) continue;
        const Def<Vector3d> p1 = intersection ( side1->plane, side2->plane, side3->plane );
        if ( ! p1.isDef ) continue;
        const Def<Vector3d> p2 = intersection ( side1->plane, side4->plane, side3->plane );
        if ( ! p2.isDef ) continue;
        const Def<Vector3d> p3 = intersection ( side2->plane, side3->plane, side4->plane );
        if ( ! p3.isDef ) continue;
        const Def<Vector3d> p4 = intersection ( side2->plane, side1->plane, side4->plane );
        if ( ! p4.isDef ) continue;
        const double h1 = fabs ( plane % p1 );
        const double h2 = fabs ( plane % p2 );
        const double h3 = fabs ( plane % p3 );
        const double h4 = fabs ( plane % p4 );
        const double max1 = h1 > h2 ? h1 : h2;
        const double max2 = h3 > h4 ? h3 : h4;
        if ( max1 < max2 )
        {
            if ( max1 > eps ) continue;
            if ( fabs ( side1->plane % p1 ) > 1e-8 ) continue;
            if ( fabs ( side2->plane % p1 ) > 1e-8 ) continue;
            if ( fabs ( side3->plane % p1 ) > 1e-8 ) continue;
            if ( fabs ( side1->plane % p2 ) > 1e-8 ) continue;
            if ( fabs ( side4->plane % p2 ) > 1e-8 ) continue;
            if ( fabs ( side3->plane % p2 ) > 1e-8 ) continue;
        }
++n2;
    }
    while ( body.sides.next() );
//message(body.sides.size(),n1,n2);
}

void unionSides ( Body & body, double eps )
{
    eps += eps;
    if ( ! body.bones.top() ) return;
    do
    {
        Bone * bone = body.bones.cur();
        bone->info = int ( 1e9 * ( bone->s0->plane.norm * bone->s1->plane.norm ) );
    }
    while ( body.bones.next() );
    SideInfo * si = new SideInfo[body.sides.size()];
    nat i = 0;
    body.sides.top();
    do
    {
        Side * s = body.sides.cur();
        calc ( si[i].vector, s );
        s->info = i++;
    }
    while ( body.sides.next() );
    List<SideCluster> list;
    body.bones.sort321();
    body.bones.top();
    do
    {
        Bone * bone = body.bones.cur();
        if ( bone->info < 0 ) break;
        Side * s0 = bone->s0;
        Side * s1 = bone->s1;
        if ( s0 == s1 )
        {
            continue;
        }
        SideCluster * cluster0 = si[s0->info].cluster;
        SideCluster * cluster1 = si[s1->info].cluster;
        if ( cluster0 != 0 && cluster0 == cluster1 )
        {
            continue;
        }
        Vector3d norm ( 0., 0., 0. );
        if ( cluster0 == 0 )
        {
            norm += si[s0->info].vector;
        }
        else
        {
            norm += cluster0->vector;
        }
        if ( cluster1 == 0 )
        {
            norm += si[s1->info].vector;
        }
        else
        {
            norm += cluster1->vector;
        }
        norm.setNorm2();
        if ( !norm )
        {
            continue;
        }
        double max = -1e100;
        double min =  1e100;
        if ( cluster0 == 0 )
        {
            calc ( norm, s0, max, min );
        }
        else
        {
            cluster0->top();
            do
            {
                calc ( norm, cluster0->cur(), max, min );
            }
            while ( cluster0->next() );
        }
        if ( cluster1 == 0 )
        {
            calc ( norm, s1, max, min );
        }
        else
        {
            cluster1->top();
            do
            {
                calc ( norm, cluster1->cur(), max, min );
            }
            while ( cluster1->next() );
        }
        if ( max - min > eps )
        {
            continue;
        }
        if ( cluster0 == 0 )
        {
            if ( cluster1 == 0 )
            {
                SideCluster * cluster = new SideCluster;
                cluster->vector = si[s0->info].vector + si[s1->info].vector;
                body.sides.jump ( s0 );
                body.sides.movCurAftCur ( *cluster );
                body.sides.jump ( s1 );
                body.sides.movCurAftCur ( *cluster );
                list.addAftCur ( cluster );
                si[s0->info].cluster = cluster;
                si[s1->info].cluster = cluster;
            }
            else
            {
                cluster1->vector += si[s0->info].vector;
                body.sides.jump ( s0 );
                body.sides.movCurAftCur ( *cluster1 );
                si[s0->info].cluster = cluster1;
            }
        }
        else
        {
            if ( cluster1 == 0 )
            {
                cluster0->vector += si[s1->info].vector;
                body.sides.jump ( s1 );
                body.sides.movCurAftCur ( *cluster0 );
                si[s1->info].cluster = cluster0;
            }
            else
            {
                cluster0->vector += cluster1->vector;
                cluster1->top();
                do
                {
                    si[cluster1->cur()->info].cluster = cluster0;
                }
                while ( cluster1->next() );
                cluster1->movAllAftCur ( *cluster0 );
                list.del ( cluster1 );
            }
        }
    }
    while ( body.bones.next() );
    body.bones.top();
    for(;;)
    {
        Bone * bone = body.bones.cur();
        if ( bone->info < 0 ) break;
        Side * s0 = bone->s0;
        Side * s1 = bone->s1;
        SideCluster * cluster0 = si[s0->info].cluster;
        SideCluster * cluster1 = si[s1->info].cluster;
        if ( s0 != s1 && cluster0 != 0 && cluster0 == cluster1 )
        {
            s0->jump(&bone->b0);
            s1->jump(&bone->b1);
            s0->makeCurLas();
            s1->makeCurLas();
            s0->delCur();
            s1->delCur();
            s1->movAllAftLas(*s0);
            while(s0->next()) s0->setCurSelf();
            cluster0->del(s1);
            body.bones.delCur();
            continue;
        }
        if ( ! body.bones.next() ) break;
    }
    delete[] si;
//message(body.sides.size(), list.size());
    if ( list.top() == 0 ) return;
    do
    {
        SideCluster * cluster = list.cur();
        Side * s = cluster->cur();
        bool isEqual;
        do
        {
            isEqual = false;
            s->top();
            do
            {
                BonePtr * p = s->cur();
                Bone * bone = p->bone;
                if ( bone->s0 == s && bone->s1 == s )
                {
                    if ( s->cnext()->bone == bone )
                    {
                        s->delCur();
                        s->del ( p );
                        body.bones.del ( bone );
                        isEqual = true;
                        break;
                    }
                    else
                    {
                        s->jump ( p );
                    }
                }
            }
            while ( s->next() );
        }
        while ( isEqual );
// Пересчёт параметров плоскости
        s->plane.norm = cluster->vector.setNorm2();
        double max = -1e100;
        double min =  1e100;
        calc ( s->plane.norm, s, max, min );
        s->plane.dist = - 0.5 * ( max + min );
        cluster->movAllAftLas ( body.sides );
    }
    while ( list.next() );
    clearVerts ( body );
// Уточнение вершин
    if ( ! body.verts.top() ) return;
    nat nv = 0;
    do
    {
        body.verts.cur()->info = nv++;
    }
    while ( body.verts.next() );
    List1p * lp = new List1p[nv];
    body.sides.top();
    do
    {
        Side * s = body.sides.cur();
        Vert * v = s->vert0();
        lp[v->info].addAftCur ( new Item1p ( s ) );
    }
    while ( body.sides.next() );
    DynArray<Plane3d> arr(9);
    body.verts.top();
    for ( i = 0; i < nv; ++i )
    {
        List1p & l = lp[i];
        const nat n = l.size();
        if ( n > 2 )
        {
            if ( n > arr.size() ) arr.resize ( n );
            double max1 = 0;
            l.top();
            for ( nat j = 0; j < n; ++j )
            {
                arr[j] = ( (Side*) l.cur()->a )->plane;
                double t = fabs ( arr[j] % body.verts.cur()->point );
                if ( max1 < t ) max1 = t;
                l.next();
            }
            Def<Vector3d> p = getNearPoint2 ( CArrRef<Plane3d> ( arr, 0, n ) );
            if ( p.isDef )
            {
                double max = 0;
                for ( nat j = 0; j < n; ++j )
                {
                    double t = fabs ( arr[j] % p );
                    if ( max < t ) max = t;
                }
//message(max1,max);
                body.verts.cur()->point = p;
            }
            else
            {
                int i = 0;
            }
        }
            else
            {
                int i = 0;
            }
        body.verts.next();
    }
    delete[] lp;
}

void clearNearSides ( Body & body, double eps )
{
    Suite<Vector2d> vert2d;
    if ( body.sides.top() )
    do
    {
        Side * s = body.sides.cur();
        if ( s->size() != 3 ) continue;
        s->top();
        do
        {
            Bone * b = s->cur()->bone;
            Side * s1 = s == b->s1 ? b->s0 : b->s1;
            if ( s1 == 0 ) continue;
            if ( fabs ( s->plane.norm.x + s1->plane.norm.x ) < eps && 
                 fabs ( s->plane.norm.y + s1->plane.norm.y ) < eps && 
                 fabs ( s->plane.norm.z + s1->plane.norm.z ) < eps && 
                 fabs ( s->plane.dist + s1->plane.dist ) < eps )
            {
                if ( s1->size() == 3 )
                {
                    s1->top();
                    do
                    {
                        if ( s1->cur()->bone == b ) break;
                    }
                    while ( s1->next() );
                    if ( s1->cur()->bone != b ) continue;
                    BonePtr * b0 = s->cprev();
                    BonePtr * b1 = s1->cnext();
                    if ( s->vert0() != s1->vert1() ) 
                    {
                        s->cnext();
                        goto m1;
                    }
                    BonePtr * c1 = s1->cnext();
                    s1->delAll();
                    BonePtr * c0 = s->cprev();
                    s->delAll();
                    body.bones.del ( b );
                    if ( b1->bone != b0->bone )
                    {
                        Side * s2;
                        BonePtr * b2;
                        b = b0->bone;
                        if ( b->s0 != s )
                        {
                            s2 = b->s0;
                            b2 = & b->b0;
                        }
                        else
                        {
                            s2 = b->s1;
                            b2 = & b->b1;
                        }
                        s2->jump ( b2 );
                        s2->makeCurLas ();
                        s2->delLas();
                        b = b1->bone;
                        if ( b->s0 == s1 )
                        {
                            b->s0 = s2;
                            s2->addAftLas ( & b->b0 );
                        }
                        else
                        {
                            b->s1 = s2;
                            s2->addAftLas ( & b->b1 );
                        }
                    }
                    body.bones.del ( b0->bone );
                    if ( c1->bone != c0->bone )
                    {
                        Side * s2;
                        BonePtr * b2;
                        b = c0->bone;
                        if ( b->s0 != s )
                        {
                            s2 = b->s0;
                            b2 = & b->b0;
                        }
                        else
                        {
                            s2 = b->s1;
                            b2 = & b->b1;
                        }
                        s2->jump ( b2 );
                        s2->makeCurLas ();
                        s2->delLas();
                        b = c1->bone;
                        if ( b->s0 == s1 )
                        {
                            b->s0 = s2;
                            s2->addAftLas ( & b->b0 );
                        }
                        else
                        {
                            b->s1 = s2;
                            s2->addAftLas ( & b->b1 );
                        }
                    }
                    body.bones.del ( c0->bone );
                    body.sides.del ( s1 );
                    body.sides.del ( s );
                    body.sides.prev();
                    break;
                }
m1:;            if ( s->area() < 0.999 * s1->area() )
                {
                    s1->top();
                    do
                    {
                        if ( s1->cur()->bone == b ) break;
                    }
                    while ( s1->next() );
                    BonePtr * p = s1->cur();
                    if ( p->bone != b ) continue;
// Проверка на самопересечение
                    OrthoFunc3to2 trans ( s1->plane.norm );
                    vert2d.resize();
                    do
                    vert2d.inc() = trans ( s1->vert1()->point );
                    while ( s1->cnext() != p );
                    s->cnext();
                    vert2d.inc() = trans ( s->vert1()->point );
                    s->cprev();
                    const int ln = loopNumber ( vert2d );
                    if ( ln != 1 || ! trianTestNat1L1MinTan ( vert2d ) )
                    {
                        continue;
                    }
                    s->makeCurFir();
                    s->delCur();
                    ( s->fir()->bone->s0 == s ? s->fir()->bone->s0 : s->fir()->bone->s1 ) = s1;
                    ( s->las()->bone->s0 == s ? s->las()->bone->s0 : s->las()->bone->s1 ) = s1;
                    if ( s->fir()->bone->s0 == s->fir()->bone->s1 )
                    {
                        Bone * t = s->fir()->bone;
                        s->delFir();
                        s1->jump ( p );
                        s1->makeCurFir();
                        s1->delFir();
                        s1->delLas();
                        s->movAllAftLas ( *s1 );
                        body.bones.del ( t );
                    }
                    else
                    {
                        s->movAllAftCur ( *s1 );
                        s1->del ( p );
                    }
                    body.bones.del ( b );
                    body.sides.delCur();
                    body.sides.prev();
                    break;
                }
            }
        }
        while ( s->next() );
    }
    while ( body.sides.next() );
}

void clearNearSides2 ( Body & body, double eps )
{
    if ( body.bones.top() )
    do
    {
m1:     Bone * bone = body.bones.cur();
        Side * s0 = bone->s0;
        Side * s1 = bone->s1;
        if ( s0 == 0 || s1 == 0 ) continue;
        if ( norm1 ( s0->plane.norm + s1->plane.norm ) > 1e-5 ) continue;
        double max0 = 0;
        s0->top();
        do
        {
            const double t = fabs ( s1->plane % s0->vert0()->point );
            if ( max0 < t ) max0 = t;
        }
        while ( s0->next() );
        double max1 = 0;
        s1->top();
        do
        {
            const double t = fabs ( s0->plane % s1->vert0()->point );
            if ( max1 < t ) max1 = t;
        }
        while ( s1->next() );
        if ( max0 < max1 )
        {
            if ( max0 > eps ) continue;
            s0->jump ( & bone->b0 ).makeCurFir();
            s0->delCur();
            s1->jump ( & bone->b1 ).makeCurFir();
            s1->delCur();
            s1->end();
            while ( s0->size() > 0 )
            {
                s0->movFirAftCur ( *s1 );
                s1->setCurSelf();
            }
            body.sides.del(s0);
            if ( s1->area() < 0 ) s1->plane = - s1->plane;
//test ( s1 );
            if ( body.bones.curIsLas() )
            {
                body.bones.delCur();
                break;
            }
            else
            {
                body.bones.delCur();
                goto m1;
            }
        }
        else
        {
            if ( max1 > eps ) continue;
            s0->jump ( & bone->b0 ).makeCurFir();
            s0->delCur();
            s1->jump ( & bone->b1 ).makeCurFir();
            s1->delCur();
            s0->end();
            while ( s1->size() > 0 )
            {
                s1->movFirAftCur ( *s0 );
                s0->setCurSelf();
            }
            body.sides.del(s1);
            if ( s0->area() < 0 ) s0->plane = - s0->plane;
//test ( s0 );
            if ( body.bones.curIsLas() )
            {
                body.bones.delCur();
                break;
            }
            else
            {
                body.bones.delCur();
                goto m1;
            }
        }

    }
    while ( body.bones.next() );
}

//**************************** 01.10.2009 ****************************
//
//          Проекция многогранника на плоскость
//          eps - минимальное расстояние между точками
//
//**************************** 29.08.2014 ****************************

nat doctor ( SuiteRef<Vector2d> & poly )
{
    if ( poly.size() < 4 ) return 0;
    nat kk = 0;
    for ( nat i = 1; i < poly.size() - 2; ++i )
    {
        const Vector2d & bi = poly[i];
        const Vector2d ai = poly[i-1];
        const Vector2d ci = bi - ai;
        for ( nat j = i + 2; j < poly.size(); ++j )
        {
            const Vector2d & bj = poly[j];
            const Vector2d & aj = poly[j-1];
            const Vector2d cj = bj - aj;
            const double d = cj % ci;
            if ( d == 0) continue;
            const Vector2d e = aj - ai;
            const double s = ( cj % e ) / d;
            const double t = ( ci % e ) / d;
            if ( t >= 0 && t <= 1 && s >= 0 && s <= 1 )
            {
                ++kk;
                nat m = j - i;
                if ( m + m > poly.size() )
                {
                    for ( nat k = 0; k < m; ++k ) poly[k] = poly[k+i];
                    poly.resize ( m );
                    poly.inc() = ai + ci * s;
                    i = 1;
                    break;
                }
                else
                {
                    --m;
                    poly[i] = ai + ci * s;
                    for ( nat k = j; k < poly.size(); ++k ) poly[k-m] = poly[k];
                    poly.resize ( poly.size() - m );
                }
            }
        }
    }
    return kk;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PolyhedronDoctor.h"

bool fillData ( const Body & poly, DynArrRef<Vector3d> & vertex, 
                DynArrRef<Set2<CmbArray<nat, 3>, Plane3d> > & facet, IPolyhedronDoctorReport & rep )
{
    bool badModel = false;
    nat i, nv = poly.verts.size(), ns = poly.sides.size();
    if ( ns < 2 ) rep ( PHD1_FacetNumberLess2 , ns ), badModel = true;
    if ( nv < 3 ) rep ( PHD1_VertexNumberLess3, nv ), badModel = true;
    if ( badModel ) return false;
    // Записываем координаты вершин в массив vertex
    DynArray<SortItem<const Vert *, nat> > vertIndex ( nv );
    vertex.resize ( nv );
    Show<Vert> verts ( poly.verts );
    verts.top();
    for ( i = 0; i < nv; ++i )
    {
        SortItem<const Vert *, nat> & si = vertIndex[i];
        si.head = verts.cur();
        si.tail = i;
        vertex[i] = si.head->point;
        verts.next();
    }
    quickSort123 ( vertIndex );
    // Записываем данные о гранях в массив facet
    facet.resize ( ns );
    Show<Side> sides ( poly.sides );
    sides.top();
    for ( i = 0; i < ns; ++i )
    {
        const Side * s = sides.cur();
        const nat n = s->size();
        Set2<CmbArray<nat, 3>, Plane3d> & set = facet[i];
        DynArrRef<nat> & index = set.a;
        index.resize ( n );
        Show<BonePtr> side ( *s );
        side.top();
        for ( nat j = 0; j < n; ++j )
        {
            const nat k = lasEqu123 ( vertIndex, SortItem<const Vert *, nat> ( side.cur()->vert0() ) );
            if ( k >= vertIndex.size() )
            {
                rep ( PHD2_VertexIndexOutOfRange, i, k );
                return false;
            }
            index[j] = vertIndex[k].tail;
            side.next();
        }
        set.b = s->plane;
        sides.next();
    }
    return true;
}

inline void add ( SortItem < Set2<nat>, Set2<nat> > & e, 
                  Suite< SortItem <Set2<nat>, Set2<nat> > > & suite12,
                  Suite< SortItem <Set2<nat>, Set2<nat> > > & suite21 )
{
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

void copyData ( CArrRef<Vector3d> vertex, CArrRef<Set2<CmbArray<nat, 3>, Plane3d> > facet, Body & body )
{
    nat nv = vertex.size();
    nat i;
    BodyStor stor;
    body.makeVoid ( stor );
    DynArray<Side *> sarr ( facet.size() );
    DynArray<Vert *> varr ( nv );
    // Запись вершин
    body.verts.resize ( varr.size(), stor.verts );
    body.verts.top();
    for ( i = 0; i < nv; ++i )
    {
        ( varr[i] = body.verts.cur() )->point = vertex[i];
        body.verts.next();
    }
    // Запись граней
    body.sides.resize ( sarr.size(), stor.sides );
    body.sides.top();
    for ( i = 0; i < body.sides.size(); ++i )
    {
        ( sarr[i] = body.sides.cur() )->plane = facet[i].b;
        body.sides.next();
    }
    Suite< SortItem <Set2<nat>, Set2<nat> > > suite12 ( 2*facet.size() );
    Suite< SortItem <Set2<nat>, Set2<nat> > > suite21 ( 2*facet.size() );
    for ( i = 0; i < facet.size(); ++i )
    {
        const Set2<CmbArray<nat, 3>, Plane3d> & r = facet[i];
        SortItem < Set2<nat>, Set2<nat> > e;
        e.tail.a = i;
        for ( nat j = 0; j < r.a.size(); ++j )
        {
            e.tail.b = j;
            e.head.a = r.a[j];
            e.head.b = r.a.cnext(j);
            add ( e, suite12, suite21 );
        }
    }
    quickSort123 ( suite12 );
    quickSort123 ( suite21 );
    nat nb = suite12.size();
    for ( i = 0; i < suite12.size(); ++i  )
    {
        const SortItem < Set2<nat>, Set2<nat> > & e1 = suite12[i];
        const SortItem < Set2<nat>, Set2<nat> > & e2 = suite21[i];
        Bone * b = new Bone ( varr[e1.head.a], varr[e1.head.b], sarr[e1.tail.a], sarr[e2.tail.a] );
        body.bones.addAftCur ( b );
        b->s0->addAftCur ( & b->b0 );
        b->s1->addAftCur ( & b->b1 );
        b->b0.info = e1.tail.b;
        b->b1.info = e2.tail.b;
    }
    for ( i = 0; i < sarr.size(); ++i ) sarr[i]->sort123();
}

PolyhedronDoctorReturn doctorPolyhedron ( const Body & poly1, Body & poly2, IPolyhedronDoctorReport & rep )
{
    return doctorPolyhedronShell ( poly1, poly2, false, rep );
}

PolyhedronDoctorReturn doctorTriangularPolyhedron ( const Body & poly1, Body & poly2, IPolyhedronDoctorReport & rep )
{
    return doctorPolyhedronShell ( poly1, poly2, true, rep );
}

//**************************** 16.09.2013 ****************************
//
//                       Упорядочивание рёбер
//
//**************************** 16.09.2013 ****************************

void sortBones ( Body & body )
{
    if ( body.bones.size() < 2 || body.verts.size() < 2 ) return;
    nat i = 0;
    body.verts.top();
    do
    {
        body.verts.cur()->info = i++;
    }
    while ( body.verts.next() );
    DynArray<SortItem<Set2<int>, Bone*> > arr ( body.bones.size() );
    i = 0;
    body.bones.top();
    do
    {
        Bone * bone = body.bones.cur();
        int i0 = bone->v0->info;
        int i1 = bone->v1->info;
        if ( i0 > i1 ) _swap ( i0, i1 );
        arr[i].head.a = i0;
        arr[i].head.b = i1;
        arr[i].tail = bone;
        ++i;
    }
    while ( body.bones.next() );
    quickSort123 ( arr );
    for ( i = 0; i < arr.size(); ++i ) arr[i].tail->info = i;
    body.bones.sort123();
}

//**************************** 04.10.2005 ****************************
//
//  Отсечение части многогранника плоскостями
//  Параметры poly и res могут ссылаться на один объект
//  Для новых граней поле info имеет значение mark
//
//**************************** 07.11.2009 ****************************

bool cut ( const Polyhedron & poly, int n, const Plane3d * plane, int mark, Polyhedron & res )
{
    Body body;
    if ( copy ( poly, body ).sides.size() == 0 ) return false;
    for ( int i = 0; i < n; ++i ) cut ( body, plane[i], mark );
    clearSides ( body ); // Убрать грани которые содержат меньше 3 точек
    copy ( body, res );
    return true;
}
