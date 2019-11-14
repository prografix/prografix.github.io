
#include "math.h"
#include "LinAlg.h"
#include "func1t.h"
#include "trans.h"
#include "Vector2d.h"
#include "Polyhedron.h"
#include "intersect2d.h"
#include "intersect3d.h"

//**************************** 01.02.2007 *********************************//
//
//                    Пересечение трёх плоскостей
//
//**************************** 09.04.2011 *********************************//

Def<Vector3d> intersection ( const Plane3d & plane1, const Plane3d & plane2, const Plane3d & plane3 )
{
    SLU3<double> slu;
    slu.aa = plane1.norm.x;   slu.ab = plane1.norm.y;   slu.ac = plane1.norm.z;   slu.ad = -plane1.dist;
    slu.ba = plane2.norm.x;   slu.bb = plane2.norm.y;   slu.bc = plane2.norm.z;   slu.bd = -plane2.dist;
    slu.ca = plane3.norm.x;   slu.cb = plane3.norm.y;   slu.cc = plane3.norm.z;   slu.cd = -plane3.dist;
    Def<Vector3d> point;
    point.isDef = slu.gauss ( point.x, point.y, point.z );
    return point;
}

//**************************** 01.02.2007 *********************************//
//
//                    Пересечение двух плоскостей
//
//**************************** 23.05.2010 *********************************//

Def<Line3d> intersection ( const Plane3d & plane1, const Plane3d & plane2 )
{
    const Vector3d v = plane1.norm % plane2.norm;
    const double s = v * v;
    if ( s < 1e-16 ) return Def<Line3d>();
    Vector3d point;
    const double ax = fabs ( plane1.norm.x );
    const double ay = fabs ( plane1.norm.y );
    const double az = fabs ( plane1.norm.z );
    if ( ax >= ay && ax >= az )
    {
        const double c = plane2.norm.x / plane1.norm.x;
        const double y = plane2.norm.y - c * plane1.norm.y;
        const double z = plane2.norm.z - c * plane1.norm.z;
        const double d = plane2.dist   - c * plane1.dist;
        if ( y * z < 0 )
        {
            point.y = d / ( z - y );
            point.z = - point.y;
        }
        else
        {
            point.y = point.z = - d / ( z + y );
        }
        point.x = - ( point.y * plane1.norm.y + point.z * plane1.norm.z + plane1.dist ) / plane1.norm.x;
    }
    else
    if ( ay >= az )
    {
        const double c = plane2.norm.y / plane1.norm.y;
        const double x = plane2.norm.x - c * plane1.norm.x;
        const double z = plane2.norm.z - c * plane1.norm.z;
        const double d = plane2.dist   - c * plane1.dist;
        if ( x * z < 0 )
        {
            point.x = d / ( z - x );
            point.z = - point.x;
        }
        else
        {
            point.x = point.z = - d / ( z + x );
        }
        point.y = - ( point.x * plane1.norm.x + point.z * plane1.norm.z + plane1.dist ) / plane1.norm.y;
    }
    else
    {
        const double c = plane2.norm.z / plane1.norm.z;
        const double y = plane2.norm.y - c * plane1.norm.y;
        const double x = plane2.norm.x - c * plane1.norm.x;
        const double d = plane2.dist   - c * plane1.dist;
        if ( y * x < 0 )
        {
            point.y = d / ( x - y );
            point.x = - point.y;
        }
        else
        {
            point.y = point.x = - d / ( x + y );
        }
        point.z = - ( point.y * plane1.norm.y + point.x * plane1.norm.x + plane1.dist ) / plane1.norm.z;
    }
    return Line3d ( v / sqrt(s), point );
}

//**************************** 09.02.2007 *********************************//
//
//                    Пересечение плоскости и прямой
//
//**************************** 23.05.2010 *********************************//

Def<Vector3d> intersection ( const Plane3d & plane, const Line3d & line )
{
    const double s = plane.norm * line.dir;
    return s * s < 1e-16 * ( plane.norm * plane.norm ) * ( line.dir * line.dir ) ? Def<Vector3d>() : 
        Def<Vector3d> ( line.point - line.dir * ( ( plane % line.point ) / s ) );
}

//**************************** 25.10.2010 *********************************//
//
//                    Пересечение плоскости и отрезка
//
//**************************** 25.10.2010 *********************************//

Def<Vector3d> intersection ( const Plane3d & plane, const Segment3d & seg )
{
    Def<Vector3d> p;
    const double a = plane % seg.a;
    const double b = plane % seg.b;
    if ( a * b > 0 || a == b ) return p;
    const double c = b - a;
    const double ax = fabs ( plane.norm.x );
    const double ay = fabs ( plane.norm.y );
    const double az = fabs ( plane.norm.z );
    if ( ax >= ay && ax >= az )
    {
        p.y = ( seg.a.y * b - seg.b.y * a ) / c;
        p.z = ( seg.a.z * b - seg.b.z * a ) / c;
        p.x = - ( plane.norm.y * p.y + plane.norm.z * p.z + plane.dist ) / plane.norm.x;
    }
    else
    if ( ay >= ax && ay >= az )
    {
        p.x = ( seg.a.x * b - seg.b.x * a ) / c;
        p.z = ( seg.a.z * b - seg.b.z * a ) / c;
        p.y = - ( plane.norm.x * p.x + plane.norm.z * p.z + plane.dist ) / plane.norm.y;
    }
    else
    {
        p.x = ( seg.a.x * b - seg.b.x * a ) / c;
        p.y = ( seg.a.y * b - seg.b.y * a ) / c;
        p.z = - ( plane.norm.x * p.x + plane.norm.y * p.y + plane.dist ) / plane.norm.z;
    }
    p.isDef = true;
    return p;
}

//**************************** 14.05.2008 *********************************//
//
//                    Отсечение плоскостью части отрезка
//
//**************************** 20.08.2011 *********************************//

Def<Segment3d> cut ( const Plane3d & plane, const Segment3d & seg )
{
    Def<Segment3d> res ( seg );
    const double a = plane % seg.a;
    const double b = plane % seg.b;
    if ( a < 0 )
    {
        if ( b > 0 )
        {
            const double c = b - a;
            const double ax = fabs ( plane.norm.x );
            const double ay = fabs ( plane.norm.y );
            const double az = fabs ( plane.norm.z );
            if ( ax >= ay && ax >= az )
            {
                res.b.y = ( res.a.y * b - res.b.y * a ) / c;
                res.b.z = ( res.a.z * b - res.b.z * a ) / c;
                res.b.x = - ( plane.norm.y * res.b.y + plane.norm.z * res.b.z + plane.dist ) / plane.norm.x;
            }
            else
            if ( ay >= ax && ay >= az )
            {
                res.b.x = ( res.a.x * b - res.b.x * a ) / c;
                res.b.z = ( res.a.z * b - res.b.z * a ) / c;
                res.b.y = - ( plane.norm.x * res.b.x + plane.norm.z * res.b.z + plane.dist ) / plane.norm.y;
            }
            else
            {
                res.b.x = ( res.a.x * b - res.b.x * a ) / c;
                res.b.y = ( res.a.y * b - res.b.y * a ) / c;
                res.b.z = - ( plane.norm.x * res.b.x + plane.norm.y * res.b.y + plane.dist ) / plane.norm.z;
            }
        }
    }
    else
    {
        if ( b >= 0 )
        {
            res.isDef = false;
            return res;
        }
        const double c = b - a;
        const double ax = fabs ( plane.norm.x );
        const double ay = fabs ( plane.norm.y );
        const double az = fabs ( plane.norm.z );
        if ( ax >= ay && ax >= az )
        {
            res.a.y = ( res.a.y * b - res.b.y * a ) / c;
            res.a.z = ( res.a.z * b - res.b.z * a ) / c;
            res.a.x = - ( plane.norm.y * res.a.y + plane.norm.z * res.a.z + plane.dist ) / plane.norm.x;
        }
        else
        if ( ay >= ax && ay >= az )
        {
            res.a.x = ( res.a.x * b - res.b.x * a ) / c;
            res.a.z = ( res.a.z * b - res.b.z * a ) / c;
            res.a.y = - ( plane.norm.x * res.a.x + plane.norm.z * res.a.z + plane.dist ) / plane.norm.y;
        }
        else
        {
            res.a.x = ( res.a.x * b - res.b.x * a ) / c;
            res.a.y = ( res.a.y * b - res.b.y * a ) / c;
            res.a.z = - ( plane.norm.x * res.a.x + plane.norm.y * res.a.y + plane.dist ) / plane.norm.z;
        }
    }
    return res;
}

//**************************** 04.12.2010 *********************************//
//
//                  Пересечение прямой и шара
//
//**************************** 04.12.2010 *********************************//

Def<Segment3d> intersection ( const Line3d & line, const Sphere3d & sphere )
{
    Def<Segment3d> res;
    if ( ! line.dir ) return res;
    const double d2 = line.dir * line.dir;
    const double t = ( ( sphere.o - line.point ) * line.dir ) / d2;
    const Vector3d v = line.point + t * line.dir;
    const double g = sphere.r * sphere.r - qmod ( v - sphere.o );
    if ( g < 0 ) return res;
    const double s = sqrt ( g / d2 );
    res.a = v - s * line.dir;
    res.b = v + s * line.dir;
    res.isDef = true;
    return res;
}

//**************************** 04.12.2010 *********************************//
//
//                  Пересечение отрезка и шара
//
//**************************** 04.12.2010 *********************************//

Def<Segment3d> intersection ( const Segment3d & seg, const Sphere3d & sphere )
{
    Def<Segment3d> res;
    const Vector3d u = sphere.o - seg.a;
    const Vector3d v = seg.b - seg.a;
    const double a = v * v;
    const double b = v * u;
    const double c = u * u - sphere.r * sphere.r;
    if ( a == 0 )
    {
        return c > 0 ? res : seg;
    }
    double d = b * b - a * c;
    if ( d < 0 ) return res;
    d = sqrt ( d );
    const double t1 = ( b - d ) / a;
    if ( t1 > 1 ) return res;
    const double t2 = ( b + d ) / a;
    if ( t2 < 0 ) return res;
    res.a = t1 > 0 ? seg.a + t1 * v : seg.a;
    res.b = t2 < 1 ? seg.a + t2 * v : seg.b;
    res.isDef = true;
    return res;
}

//**************************** 18.12.2010 *********************************//
//
//                  Пересечение прямой и эллипсоида
//
//**************************** 18.12.2010 *********************************//

Def<Segment3d> intersection ( const Line3d & line, const Ellipsoid3d & e )
{
    Def<Segment3d> res;
    const Affin3d af1 = e.getAffin3d();
    const Affin3d af2 = ~af1;
    res = intersection ( Line3d ( af2.t(line.dir), af2(line.point) ), Sphere3d ( 1, Vector3d(0,0,0) ) );
    if ( ! res.isDef ) return res;
    res.a = af1 ( res.a );
    res.b = af1 ( res.b );
    return res;
}

//**************************** 18.12.2010 *********************************//
//
//                  Пересечение отрезка и эллипсоида
//
//**************************** 18.12.2010 *********************************//

Def<Segment3d> intersection ( const Segment3d & seg, const Ellipsoid3d & e )
{
    Def<Segment3d> res;
    const Affin3d af1 = e.getAffin3d();
    const Affin3d af2 = ~af1;
    res = intersection ( Segment3d ( af2 ( seg.a ), af2 ( seg.b ) ), Sphere3d ( 1, Vector3d(0,0,0) ) );
    if ( ! res.isDef ) return res;
    res.a = af1 ( res.a );
    res.b = af1 ( res.b );
    return res;
}

//**************************** 28.01.2012 *********************************//
//
//                Пересечение многогранника и плоскости
//
//**************************** 21.03.2016 *********************************//

bool intersection ( const Polyhedron & poly, const Plane3d & plane, 
                    DynArrRef<DynArray<Vector3d> > & res )
{
    if ( ! poly.vertex.size() || ! plane.norm ) return false;
// Вычисляем расстояния от вершин многогранника до плоскости
    double bx = plane.norm.x;
    double by = plane.norm.y;
    double bz = plane.norm.z;
    double bd = plane.dist;
    const double ax = fabs ( bx );
    const double ay = fabs ( by );
    const double az = fabs ( bz );
    const nat way = ay > az ? ( ay > ax ? 1 : 0 ) : ( az > ax ? 2 : 0 );
    switch ( way )
    {
        case 0: bx /= ax; by /= ax; bz /= ax; bd /= ax; break;
        case 1: bx /= ay; by /= ay; bz /= ay; bd /= ay; break;
        case 2: bx /= az; by /= az; bz /= az; bd /= az; break;
    }
    DynArray<double> dist ( poly.vertex.size() );
    nat i, j;
    for ( i = 0; i < dist.size(); ++i )
    {
        double d = 0; // для оптимизатора 21.01.2014
        const Vector3d & v = poly.vertex[i];
        switch ( way )
        {
            case 0: dist[i] = bx * v.x;
                    d = by * v.y + bz * v.z + bd; break;
            case 1: dist[i] = by * v.y;
                    d = bz * v.z + bx * v.x + bd; break;
            case 2: dist[i] = bz * v.z;
                    d = bx * v.x + by * v.y + bd; break;
        }
        dist[i] += d;
    }
// Находим рёбра многоугольников
    List2n list;
    CmbSuite<SortItem<double, nat>, 9> va, vb;
    Suite<SortItem<Set2<nat>, Vector3d> > vert;
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        va.resize();
        vb.resize();
        const Facet & facet = poly.facet[i];
        if ( facet.nv < 3 ) continue;
        for ( j = 0; j < facet.nv; ++j )
        {
            const nat u0 = facet.index[j];
            const nat u1 = facet.index[j+1];
            const double d0 = dist[u0];
            const double d1 = dist[u1];
            const Vector3d & v0 = poly.vertex[u0];
            const Vector3d & v1 = poly.vertex[u1];
            const double p = d0 * d1;
            if ( p > 0 ) continue;
            if ( p < 0 )
            {
                SortItem<double, nat> & si = d0 < 0 ? va.inc() : vb.inc();
                const nat ii = facet.index[facet.nv+1+j];
                if ( i > ii )
                {
// Вершина многоугольника уже вычислена и находится в vert
                    SortItem<Set2<nat>, Vector3d> v;
                    v.head.a = ii;
                    v.head.b = facet.index[facet.nv+facet.nv+1+j];
                    const nat k = lasEqu123 ( vert, v );
                    if ( k == vert.size() ) return false;
                    si.tail = k;
                }
                else
                {
// Вершину многоугольника надо вычислить
                    si.tail = vert.size();
                    SortItem<Set2<nat>, Vector3d> & v = vert.inc();
                    v.head.a = i;
                    v.head.b = j;
                    const double a = d0 / ( d0 - d1 );
                    const double b = d1 / ( d1 - d0 );
                    switch ( way )
                    {
                    case 0: v.tail.y = a * v1.y + b * v0.y;
                            v.tail.z = a * v1.z + b * v0.z;
                            v.tail.x = by * v.tail.y + bz * v.tail.z + bd;
                            if ( bx > 0 ) v.tail.x = - v.tail.x;
                            break;
                    case 1: v.tail.z = a * v1.z + b * v0.z;
                            v.tail.x = a * v1.x + b * v0.x;
                            v.tail.y = bz * v.tail.z + bx * v.tail.x + bd;
                            if ( by > 0 ) v.tail.y = - v.tail.y;
                            break;
                    case 2: v.tail.x = a * v1.x + b * v0.x;
                            v.tail.y = a * v1.y + b * v0.y;
                            v.tail.z = bx * v.tail.x + by * v.tail.y + bd;
                            if ( bz > 0 ) v.tail.z = - v.tail.z;
                            break;
                    }
                }
            }
            else
            {
                if ( d0 == 0 )
                {
                    if ( d1 >= 0 ) continue;
                    SortItem<double, nat> & si = vb.inc();
                    const nat ii = facet.index[facet.nv+1+j];
                    if ( i > ii )
                    {
 // Вершина многоугольника уже вычислена и находится в vert
                        SortItem<Set2<nat>, Vector3d> v;
                        v.head.a = ii;
                        v.head.b = facet.index[facet.nv+facet.nv+1+j];
                        const nat k = lasEqu123 ( vert, v );
                        if ( k == vert.size() ) return false;
                        si.tail = k;
                    }
                    else
                    {
// Вершину многоугольника надо добавить
                        si.tail = vert.size();
                        SortItem<Set2<nat>, Vector3d> & v = vert.inc();
                        v.head.a = i;
                        v.head.b = j;
                        v.tail = v0;
                    }
                }
                else
                {
                    if ( d0 >= 0 ) continue;
                    SortItem<double, nat> & si = va.inc();
                    const nat ii = facet.index[facet.nv+1+j];
                    if ( i > ii )
                    {
 // Вершина многоугольника уже вычислена и находится в vert
                        SortItem<Set2<nat>, Vector3d> v;
                        v.head.a = ii;
                        v.head.b = facet.index[facet.nv+facet.nv+1+j];
                        const nat k = lasEqu123 ( vert, v );
                        if ( k == vert.size() ) return false;
                        si.tail = k;
                    }
                    else
                    {
// Вершину многоугольника надо добавить
                        si.tail = vert.size();
                        SortItem<Set2<nat>, Vector3d> & v = vert.inc();
                        v.head.a = i;
                        v.head.b = j;
                        v.tail = v1;
                    }
                }
            }
        }
        if ( va.size() == 0 ) continue;
        if ( va.size() == 1 )
        {
            Item2n * item = new Item2n;
            item->b = va[0].tail;
            item->a = vb[0].tail;
            list.addAftLas ( item );
            continue;
        }
        Vector3d dir ( 0, 0, 0 );
        for ( j = 0; j < va.size(); ++j )
        {
            dir += vert[vb[j].tail].tail;
            dir -= vert[va[j].tail].tail;
        }
        if ( ! dir ) return false;
        for ( j = 0; j < va.size(); ++j )
        {
            va[j].head = dir * vert[va[j].tail].tail;
            vb[j].head = dir * vert[vb[j].tail].tail;
        }
        insertSort123 ( va );
        insertSort123 ( vb );
        for ( j = 0; j < va.size(); ++j )
        {
            Item2n * item = new Item2n;
            item->b = va[j].tail;
            item->a = vb[j].tail;
            list.addAftLas ( item );
        }
    }
// Соединяем рёбра в многоугольники
    List<ListItem<List2n> > itog;
    makeClosed ( list, itog );
// Переносим данные из списков в массивы
    res.resize ( itog.size() );
    itog.top();
    for ( i = 0; i < res.size(); ++i )
    {
        List2n * vlist = itog.cur();
        DynArray<Vector3d> & pol = res[i];
        pol.resize ( vlist->size() );
        vlist->top();
        for ( j = 0; j < pol.size(); ++j )
        {
            pol[j] = vert[vlist->cur()->a].tail;
            vlist->next();
        }
        itog.next();
    }
    return true;
}

//**************************** 11.02.2012 *********************************//
//
//              Отсечение части многогранника плоскостью
//
//        Функция возвращает false, если 
//        1) ссылки poly и res указывают на один тот же объект 
//        2) нормаль плоскости равна нулевому вектору
//        3) произошла ошибка в программе
//
//**************************** 11.02.2012 *********************************//

namespace
{

struct LinkInfo
{
    ListItem<List2n> * list;
    Item2n * o1, * o2;
    double d;
    bool used;
    int boneNum;
};

// Найти наименьшее расстояние между многоугольниками и записать результат в info
void findDist ( List2n & plus, CArrRef<Vector3d> & vert, LinkInfo & info )
{
    ListItem<List2n> * list = info.list;
    plus.top();
    do
    {
        const Item2n * b1 = plus.cur();
        const Vector3d & v1 = vert[b1->a];
        list->top();
        do
        {
            const Item2n * b2 = list->cur();
            const Vector3d & v2 = vert[b2->a];
            if ( b1->a == b2->a || v1 == v2 )
            {
                info.d = 0.;
                info.o1 = plus.cur();
                info.o2 = list->cur();
                info.boneNum = 0;
                return;
            }
            const double d1 = Segment3d ( vert[b1->a], vert[b1->b] ).qdis ( v2 );
            const double d2 = Segment3d ( vert[b2->a], vert[b2->b] ).qdis ( v1 );
            if ( d1 < d2 )
            {
                if ( info.d > d1 )
                {
                    info.d = d1;
                    info.o1 = plus.cur();
                    info.o2 = list->cur();
                    info.boneNum = 1;
                }
            }
            else
            {
                if ( info.d > d2 )
                {
                    info.d = d2;
                    info.o1 = plus.cur();
                    info.o2 = list->cur();
                    info.boneNum = 2;
                }
            }
        }
        while ( list->next() );
    }
    while ( plus.next() );
}

} // namespace

inline void addAftLas ( List2n & list, nat a, nat b, CutStor3d & stor )
{
    Item2n * item = list.addNewAftLas ( stor.list2n );
    item->a = a;
    item->b = b;
}

inline void addBefFir ( List2n & list, nat a, nat b, CutStor3d & stor )
{
    Item2n * item = list.addNewBefFir ( stor.list2n );
    item->a = a;
    item->b = b;
}

static
void link ( List2n & plus, List<ListItem<List2n> > & minus, CArrRef<Vector3d> vert, 
            const Vector3d & norm, CutStor3d & stor )
{
    if ( plus.size() == 0 || minus.size() == 0 ) return;
// Находим наименьшие расстояния между положительным и отрицательными многоугольниками
    const nat n = minus.size();
    LinkInfo * info = new LinkInfo[n];
    nat i;
    minus.top();
    for ( i = 0; i < n; ++i )
    {
        ListItem<List2n> * list = minus.cur();
        LinkInfo & cinfo = info[i];
        cinfo.d = 1e300;
        cinfo.list = list;
        cinfo.used = false;
        findDist ( plus, vert, cinfo );
        minus.next();
    }
// Связываем отрицательные многоугольники с положительным
    List2n side;
    for(;;)
    {
        nat imin = n;
        for ( i = 0; i < n; ++i )
        {
            const LinkInfo & cinfo = info[i];
            if ( cinfo.used ) continue;
            if ( imin == n || info[imin].d > cinfo.d ) imin = i;
        }
        if ( imin == n ) break;
        LinkInfo & cinfo = info[imin];
        cinfo.used = true;
        ListItem<List2n> * list = cinfo.list;
        list->jump ( cinfo.o2 );
        list->makeCurFir();
        plus.jump ( cinfo.o1 );
        if ( cinfo.boneNum == 1 )
        {
            const Vector3d & a = vert[cinfo.o1->a];
            const Vector3d & b = vert[cinfo.o1->b];
            const Vector3d & c = vert[cinfo.o2->a];
            if ( ( c - a ) * ( b - a ) < 0 )
            {
                const Vector3d & d = vert[plus.cprev()->a];
                const Vector3d v1 = ( b - a ) % ( c - b );
                const Vector3d v2 = ( a - d ) % ( c - a );
                if ( norm * ( v1 - v2 ) < 0 )
                    cinfo.o1 = plus.cur();
                else
                    plus.jump ( cinfo.o1 );
            }
            else
            if ( ( c - b ) * ( a - b ) < 0 )
            {
                const Vector3d & d = vert[plus.cnext()->b];
                const Vector3d v1 = ( b - a ) % ( c - b );
                const Vector3d v2 = ( d - b ) % ( c - d );
                if ( norm * ( v1 - v2 ) < 0 )
                    cinfo.o1 = plus.cur();
                else
                    plus.jump ( cinfo.o1 );
            }
            plus.makeCurLas().movCurAftLas ( side );
            addAftLas ( side, cinfo.o1->b, cinfo.o2->a, stor );
            addAftLas ( side, cinfo.o2->a, cinfo.o1->a, stor );
            addBefFir ( *list, cinfo.o1->a, cinfo.o2->a, stor );
            addAftLas ( *list, cinfo.o2->a, cinfo.o1->b, stor );
            for ( i = 0; i < n; ++i )
            {
                LinkInfo & ci = info[i];
                if ( ! ci.used && ci.o1 == cinfo.o1 ) ci.d = 1e300;
            }
        }
        else
        if ( cinfo.boneNum == 2 )
        {
            const Vector3d & a = vert[cinfo.o2->a];
            const Vector3d & b = vert[cinfo.o2->b];
            const Vector3d & c = vert[cinfo.o1->a];
            if ( ( c - a ) * ( b - a ) < 0 )
            {
                const Vector3d & d = vert[list->cprev()->a];
                const Vector3d v1 = ( b - a ) % ( c - b );
                const Vector3d v2 = ( a - d ) % ( c - a );
                if ( norm * ( v1 - v2 ) < 0 )
                    cinfo.o2 = list->cur();
                else
                    list->jump ( cinfo.o2 );
            }
            else
            if ( ( c - b ) * ( a - b ) < 0 )
            {
                const Vector3d & d = vert[list->cnext()->b];
                const Vector3d v1 = ( b - a ) % ( c - b );
                const Vector3d v2 = ( d - b ) % ( c - d );
                if ( norm * ( v1 - v2 ) < 0 )
                {
                    cinfo.o2 = list->cur();
                    list->makeCurFir();
                }
                else
                    list->jump ( cinfo.o2 );
            }
            list->movCurAftCur ( side );
            addAftLas ( side, cinfo.o2->b, cinfo.o1->a, stor );
            addAftLas ( side, cinfo.o1->a, cinfo.o2->a, stor );
            addBefFir ( *list, cinfo.o1->a, cinfo.o2->b, stor );
            addAftLas ( *list, cinfo.o2->a, cinfo.o1->a, stor );
        }
// Считаем наименьшие расстояния для новых рёбер
        for ( i = 0; i < n; ++i )
        {
            LinkInfo & ci = info[i];
            if ( ci.used ) continue;
            findDist ( *list, vert, ci );
        }
// Добавляем рёбра в plus
        if ( cinfo.boneNum == 1 )
            list->movAllAftCur ( plus );
        else
            list->movAllBefCur ( plus );
// Меняем список minus
        if ( cinfo.boneNum == 0 )
            minus.del ( list );
        else
            swap ( *list, side );
    }
    delete[] info;
}

inline void addAftLas ( List1n & list, nat a, CutStor3d & stor )
{
    list.addNewAftLas ( stor.list1n )->a = a;
}

bool cut ( const Polyhedron & poly, const Plane3d & plane, Polyhedron & res, CutStor3d & stor, int info )
{
    if ( ! plane.norm || & poly == & res) return false;
// Вычисляем расстояния от вершин многогранника до плоскости
    double bx = plane.norm.x;
    double by = plane.norm.y;
    double bz = plane.norm.z;
    double bd = plane.dist;
    const double ax = fabs ( bx );
    const double ay = fabs ( by );
    const double az = fabs ( bz );
    nat way = ay > az ? ( ay > ax ? 1 : 0 ) : ( az > ax ? 2 : 0 );
    switch ( way )
    {
        case 0: bx /= ax; by /= ax; bz /= ax; bd /= ax; break;
        case 1: bx /= ay; by /= ay; bz /= ay; bd /= ay; break;
        case 2: bx /= az; by /= az; bz /= az; bd /= az; break;
    }
    const nat nvold = poly.vertex.size();
    Suite<double> & dist = stor.dist;
    dist.resize ( nvold );
    nat i, j;
    for ( i = 0; i < nvold; ++i )
    {
        double d = 0;
        const Vector3d & v = poly.vertex[i];
        switch ( way )
        {
            case 0: dist[i] = bx * v.x;
                    d = by * v.y + bz * v.z + bd; break;
            case 1: dist[i] = by * v.y;
                    d = bz * v.z + bx * v.x + bd; break;
            case 2: dist[i] = bz * v.z;
                    d = bx * v.x + by * v.y + bd; break;
        }
        dist[i] += d;
    }
// Находим рёбра многоугольников
    List2n list;
    Suite<nat> & fullFacet = stor.fullFacet;
    List<ListItem<List1n> > partFacet;
    CmbSuite<SortItem<double, Set2<nat> >, 9> va, vb;
    Suite<SortItem<Set2<nat>, Vector3d> > & vert = stor.vert;
    fullFacet.resize();
    vert.resize();
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        va.resize();
        vb.resize();
        const Facet & facet = poly.facet[i];
        if ( facet.nv < 3 ) continue;
        for ( j = 0; j < facet.nv; ++j )
        {
            const nat u0 = facet.index[j];
            const nat u1 = facet.index[j+1];
            const double d0 = dist[u0];
            const double d1 = dist[u1];
            const Vector3d & v0 = poly.vertex[u0];
            const Vector3d & v1 = poly.vertex[u1];
            const double p = d0 * d1;
            if ( p > 0 ) continue;
            if ( p < 0 )
            {
                SortItem<double, Set2<nat> > & si = d0 < 0 ? va.inc() : vb.inc();
                si.tail.a = j;
                const nat ii = facet.index[facet.nv+1+j];
                if ( i > ii )
                {
// Вершина многоугольника уже вычислена и находится в vert
                    SortItem<Set2<nat>, Vector3d> v;
                    v.head.a = ii;
                    v.head.b = facet.index[facet.nv+facet.nv+1+j];
                    const nat k = lasEqu123 ( vert, v );
                    if ( k == vert.size() ) return false;
                    si.tail.b = nvold + k;
                }
                else
                {
// Вершину многоугольника надо вычислить
                    si.tail.b = nvold + vert.size();
                    SortItem<Set2<nat>, Vector3d> & v = vert.inc();
                    v.head.a = i;
                    v.head.b = j;
                    const double a = d0 / ( d0 - d1 );
                    const double b = d1 / ( d1 - d0 );
                    switch ( way )
                    {
                    case 0: v.tail.y = a * v1.y + b * v0.y;
                            v.tail.z = a * v1.z + b * v0.z;
                            v.tail.x = by * v.tail.y + bz * v.tail.z + bd;
                            if ( bx > 0 ) v.tail.x = - v.tail.x;
                            break;
                    case 1: v.tail.z = a * v1.z + b * v0.z;
                            v.tail.x = a * v1.x + b * v0.x;
                            v.tail.y = bz * v.tail.z + bx * v.tail.x + bd;
                            if ( by > 0 ) v.tail.y = - v.tail.y;
                            break;
                    case 2: v.tail.x = a * v1.x + b * v0.x;
                            v.tail.y = a * v1.y + b * v0.y;
                            v.tail.z = bx * v.tail.x + by * v.tail.y + bd;
                            if ( bz > 0 ) v.tail.z = - v.tail.z;
                            break;
                    }
                }
            }
            else
            {
                if ( d0 == 0 )
                {
                    if ( d1 >= 0 ) continue;
                    if ( dist[facet.index[j>0?j-1:facet.nv-1]] < 0 ) continue;
                    SortItem<double, Set2<nat> > & si = vb.inc();
                    si.tail.a = j;
                    si.tail.b = u0;
                }
                else
                {
                    if ( d0 > 0 ) continue;
                    nat j2 = j + 2;
                    if ( j2 > facet.nv ) j2 -= facet.nv;
                    if ( dist[facet.index[j2]] < 0 ) continue;
                    SortItem<double, Set2<nat> > & si = va.inc();
                    si.tail.a = j;
                    si.tail.b = u1;
                }
            }
        }
        const nat m = va.size();
        if ( m == 0 )
        {
// Нет пересечения грани с плоскостью
            for ( j = 0; j < facet.nv; ++j )
            {
                if ( dist[facet.index[j]] < 0 )
                {
                    fullFacet.inc() = i;
                    break;
                }
            }
            continue;
        }
        if ( m == 1 )
        {
// Пересечение грани с плоскостью - это один отрезок
            const Set2<nat> & sa = va[0].tail;
            const Set2<nat> & sb = vb[0].tail;
            addAftLas ( list, sb.b, sa.b, stor );
            ListItem<List1n> * vlist = new ListItem<List1n>;
            vlist->info = i;
            partFacet.addAftLas ( vlist );
            addAftLas ( *vlist, sa.b, stor );
            addAftLas ( *vlist, sb.b, stor );
            for ( j = sb.a;; )
            {
                if ( ++j >= facet.nv ) j -= facet.nv;
                addAftLas ( *vlist, facet.index[j], stor );
                if ( j == sa.a ) break;
            }
            continue;
        }
// Пересечение грани с плоскостью - это несколько отрезков
        Vector3d dir ( 0, 0, 0 );
        for ( j = 0; j < m; ++j )
        {
            const nat ib = vb[j].tail.b;
            dir += ib < nvold ? poly.vertex[ib] : vert[ib-nvold].tail;
            const nat ia = va[j].tail.b;
            dir -= ia < nvold ? poly.vertex[ia] : vert[ia-nvold].tail;
        }
        if ( ! dir ) return false;
        for ( j = 0; j < m; ++j )
        {
            const nat ia = va[j].tail.b;
            va[j].head = dir * ( ia < nvold ? poly.vertex[ia] : vert[ia-nvold].tail );
            const nat ib = vb[j].tail.b;
            vb[j].head = dir * ( ib < nvold ? poly.vertex[ib] : vert[ib-nvold].tail );
        }
        insertSort123 ( va );
        insertSort123 ( vb );
        for ( j = 0; j < m; ++j )
        {
            addAftLas ( list, vb[j].tail.b, va[j].tail.b, stor );
        }
        while ( va.size() > 0 )
        {
            const Set2<nat> & sa = va.las().tail;
            const Set2<nat> & sb = vb.las().tail;
            ListItem<List1n> * vlist = new ListItem<List1n>;
            vlist->info = i;
            partFacet.addAftLas ( vlist );
            addAftLas ( *vlist, sa.b, stor );
            addAftLas ( *vlist, sb.b, stor );
            va.dec();
            vb.dec();
            for ( j = sb.a;; )
            {
                if ( ++j >= facet.nv ) j -= facet.nv;
                const nat u = facet.index[j];
                if ( dist[u] < 0 )
                {
                    addAftLas ( *vlist, u, stor );
                }
                else
                {
                    j = ( j == 0 ? facet.nv : j ) - 1; 
                    nat k, km = va.size();
                    for ( k = 0; k < va.size(); ++k )
                    {
                        if ( va[k].tail.a == j )
                        {
                            km = k;
                            break;
                        }
                    }
                    if ( km == va.size() ) break;
                    const Set2<nat> & sa = va[km].tail;
                    const Set2<nat> & sb = vb[km].tail;
                    addAftLas ( *vlist, sa.b, stor );
                    addAftLas ( *vlist, sb.b, stor );
                    j = sb.a;
                    va.del(km);
                    vb.del(km);
                }
            }
        }
    }
    if ( fullFacet.size() == poly.facet.size() )
    {
        res = poly;
        return true;
    }
// Создаём массив index для перевода старых индексов вершин в новые
    Suite<nat> & index = stor.index;
    index.resize ( nvold );
    nat nv = 0;
    for ( i = 0; i < nvold; ++i )
    {
        index[i] = dist[i] > 0 ? nvold : nv++;
    }
// Соединяем новые рёбра в многоугольники
    List<ListItem<List2n> > newFacet;
    if ( list.size() > 2 ) makeClosed ( list, newFacet );
// Создаём новый список вершин
    res.vertex.resize ( nv + vert.size() );
    for ( i = 0; i < nvold; ++i )
    {
        if ( index[i] < nvold ) res.vertex[index[i]] = poly.vertex[i];
    }
    for ( i = 0; i < vert.size(); ++i )
    {
        res.vertex[nv+i] = vert[i].tail;
    }
// Обновляем индексы вершин для новых граней
    if ( newFacet.top() )
    do
    {
        List2n * vlist = newFacet.cur();
        vlist->top();
        do
        {
            Item2n * item = vlist->cur();
            const nat iv = item->a;
            if ( iv < nvold )
            {
                const nat k = index[iv];
                if ( k >= nvold ) return false;
                item->a = k;
            }
            else
            {
                item->a = nv + iv - nvold;
            }
        }
        while ( vlist->next() );
        nat iv = vlist->fir()->a;
        vlist->end();
        do
        {
            vlist->cur()->b = iv;
            iv = vlist->cur()->a;
        }
        while ( vlist->prev() );
    }
    while ( newFacet.next() );
// Обработка отрицательных граней
    if ( newFacet.top() )
    {
        // Разделяем положительные и отрицательные грани
        CmbSuite<SortItem<double, ListItem<List2n> *>, 9> plus, minus;
        do
        {
            ListItem<List2n> * vlist = newFacet.cur();
            Vector3d v ( 0., 0., 0. );
            if ( vlist->top() )
            do
            {
                v += res.vertex[vlist->cur()->a] % res.vertex[vlist->cur()->b];
            }
            while ( vlist->next() );
            const double a = v * plane.norm;
            if ( a < 0 )
            {
                minus.inc() = SortItem<double, ListItem<List2n> *> ( a, vlist );
            }
            else
            {
                plus.inc() = SortItem<double, ListItem<List2n> *> ( a, vlist );
            }
        }
        while ( newFacet.next() );
        if ( minus.size() > 0 )
        {
            insertSort123 ( plus );
            insertSort321 ( minus );
            // Слияние отрицательных многоугольников с положительными
            CmbSuite<Vector2d, 9> buf2;
            CmbSuite<Vector3d, 9> buf3;
            OrthoFunc3to2 func ( plane.norm );
            for ( i = 0; i < plus.size(); ++i )
            {
                if ( minus.size() == 0 ) break;
                List2n & lp = * plus[i].tail;
                const nat nv = lp.size();
                buf2.resize ( nv );
                buf3.resize ( nv );
                nat count = 0;
                lp.top();
                do
                {
                    buf3[count++] = res.vertex[lp.cur()->a];
                }
                while ( lp.next() );
                ortho_trans ( buf3, plane.norm, buf2 );
                List<ListItem<List2n> > temp;
                for ( j = 0; j < minus.size(); ++j )
                {
                    if ( plus[i].head + minus[j].head < 0 ) break;
                    List2n & mlp = * minus[j].tail;
                    if ( mlp.top() )
                    do
                    {
                        const Vector2d p = func ( res.vertex[mlp.cur()->a] );
                        if ( isIntersect ( buf2, p ) )
                        {
                            newFacet.jump ( minus[j].tail ).movCurAftCur ( temp );
                            minus[j].head = 0;
                            break;
                        }
                    }
                    while ( mlp.next() );
                }
                for ( j = 0; j < minus.size(); )
                {
                    if ( minus[j].head == 0 )
                    {
                        ArrRef<SortItem<double, ListItem<List2n> *> > ref ( minus, j, minus.size() - j );
                        ref <<= 1;
                        minus.dec();
                    }
                    else
                        ++j;
                }
                link ( lp, temp, res.vertex, plane.norm, stor );
                temp.movAllAftLas ( newFacet );
            }
        }
    }
// Перенос полных граней
    res.facet.resize ( fullFacet.size() + partFacet.size() + newFacet.size() );
    for ( i = 0; i < fullFacet.size(); ++i )
    {
        const Facet & facet1 = poly.facet[fullFacet[i]];
        Facet & facet2 = res.facet[i];
        facet2.info = facet1.info;
        facet2.plane = facet1.plane;
        facet2.resize ( facet1.nv );
        for ( j = 0; j < facet1.nv; ++j )
        {
            const nat k = index[facet1.index[j]];
            if ( k >= nvold ) return false;
            facet2.index[j] = k;
        }
    }
    nat n = fullFacet.size();
// Перенос обрезанных граней
    partFacet.top();
    for ( i = 0; i < partFacet.size(); ++i )
    {
        ListItem<List1n> * vlist = partFacet.cur();
        const Facet & facet1 = poly.facet[vlist->info];
        Facet & facet2 = res.facet[n+i];
        facet2.info = facet1.info;
        facet2.plane = facet1.plane;
        facet2.resize ( vlist->size() );
        vlist->top();
        for ( j = 0; j < vlist->size(); ++j )
        {
            const nat iv = vlist->cur()->a;
            if ( iv < nvold )
            {
                const nat k = index[iv];
                if ( k >= nvold ) return false;
                facet2.index[j] = k;
            }
            else
            {
                facet2.index[j] = nv + iv - nvold;
            }
            vlist->next();
        }
        vlist->movAllAftLas ( stor.list1n );
        partFacet.next();
    }
    n += partFacet.size();
// Добавление новых граней
    newFacet.top();
    for ( i = 0; i < newFacet.size(); ++i )
    {
        List2n * vlist = newFacet.cur();
        Facet & facet = res.facet[n+i];
        facet.info = info;
        facet.plane = plane;
        facet.resize ( vlist->size() );
        vlist->top();
        for ( j = 0; j < vlist->size(); ++j )
        {
            facet.index[j] = vlist->cur()->a;
            vlist->next();
        }
        vlist->movAllAftLas ( stor.list2n );
        newFacet.next();
    }
    res.linkFacets();
    return true;
}

//**************************** 16.08.2012 *********************************//
//
//            Пересечение выпуклого многогранника с произвольным
//
//**************************** 16.08.2012 *********************************//

bool intersect1c ( const Polyhedron & conv, const Polyhedron & poly, Polyhedron & res )
{
    if ( conv.facet.size() < 4 || poly.facet.size() < 4 )
    {
        res.makeVoid();
        return true;
    }
    Polyhedron tmp1, tmp2;
    tmp1 = poly;
    for ( nat i = 0; i < conv.facet.size(); ++i )
    {
        const Facet & facet = conv.facet[i];
        if ( ! cut ( tmp1, facet.plane, tmp2, facet.info ) ) return false;
        _swap ( tmp1, tmp2 );
    }
    _swap ( tmp1, res );
    return true;
}

