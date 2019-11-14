
#include "math.h"

#include "func2d.h"
#include "Vector2d.h"
#include "WireModel.h"
#include "Mathem.h"
#include "opti1d.h"
#include "opti2d.h"
#include "intersect3d.h"
#include "heap.h"

// Вспомогательные функции

bool points2lines ( CCArrRef<Vector2d> & poly, ArrRef<Line2d> & line )
{
// Для каждой стороны вычислим параметры уравнения прямой
// Уравнение прямой имеет вид: norm.x * x + norm.y * y + dist = 0
    for ( nat i = 0; i < poly.size(); ++i )
    {
        const Vector2d & v2 = poly[i+1<poly.size()?i+1:0];
        const Vector2d & v1 = poly[i];
        Line2d & linei = line[i];
        Vector2d & norm = linei.norm;
        norm.x = v2.y - v1.y;
        norm.y = v1.x - v2.x;
        const double q = norm * norm;
        if ( q == 0 ) return false;
        norm /= sqrt ( q );
        const double d1 = norm * v1;
        const double d2 = norm * v2;
        linei.dist = d1 > d2 ? - d1 : - d2;
    }
    return true;
}

bool points2lines ( CCArrRef<Vector2d> & poly, Conform2d & conf, ArrRef<Line2d> & line )
{
// Определение габаритов многоугольника
    const Def<Segment2d> seg = dimensions ( poly );
    if ( ! seg.isDef ) return false;
// Проверка на малость площади по сравнению с габаритами многоугольника
    if ( area ( poly ) < 1e-9 * ( seg.b.x - seg.a.x ) * ( seg.b.y - seg.a.y ) ) return false;
// Преобразование вершин многоугольника к стандартному виду
    const Def<Conform2d> c = standardPosition ( seg );
    if ( ! c.isDef ) return false;
    conf = c;
    DynArray<Vector2d> temp ( poly );
    temp *= Affin2d ( conf );
// Для каждой стороны вычислим параметры уравнения прямой
    return points2lines ( temp, line );
}

bool maxStretchPolygonInConvexPolygonANR ( CCArrRef<Vector2d> & inner, CCArrRef<Vector2d> & outer, Affin2d & res )
{
// Получение границ многоугольника в виде прямых линий
    Conform2d conf;
    DynArray<Line2d> line ( outer.size() );
    if ( ! points2lines ( outer, conf, line ) ) return false;
// Инициализация области допустимых преобразований
    List< Vertex<4> > stor;
    WireModel<4> model;
    model.simplex ( 8., stor );
    Double<4> d4;
    d4.d0 = d4.d1 = 0.;
    d4.d2 = d4.d3 = 2.;
    model.vlist -= d4;
    const double eps = 1e-7;
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        Show< Vertex<4> > show ( model.vlist );
        Double<4> best;
        double max = -1.;
        if ( show.top() )
        do
        {
            const Vertex<4> * p = show.cur();
            const Double<4> & pc = p->coor;
            const double t = pc.d0 * pc.d1;
            if ( max < t ) max = t, best = pc;
            for ( nat k = 0; k < 4; ++k )
            {
                const Vertex<4> * v = p->vertex[k];
                if ( v < p ) continue;
                const Double<4> & vc = v->coor;
                const double a0 = vc.d0 - pc.d0;
                const double a1 = vc.d1 - pc.d1;
                const double a = a0 * a1;
                if ( a == 0 ) continue;
                const double t = -0.5 * ( a1 * pc.d0 + a0 * pc.d1 ) / a;
                if ( t > 0 && t < 1 )
                {
                    const double e0 = pc.d0 + a0 * t;
                    const double e1 = pc.d1 + a1 * t;
                    const double p = e0 * e1;
                    if ( p > max )
                    {
                        max = p;
                        const double s = 1 - t;
                        best.d0 = e0;
                        best.d1 = e1;
                        best.d2 = pc.d2 * s + vc.d2 * t;
                        best.d3 = pc.d3 * s + vc.d3 * t;
                    }
                }
            }
        }
        while ( show.next() );
        if ( max < 0 )
            return false;
// Поиск максимального нарушения ограничений для выбранного решения
        const double a = best.d0;
        const double b = best.d1;
        const Vector2d v ( best.d2, best.d3 );
        Vector2d pm;
        nat k, km = 0;
        max = 0.;
        for ( k = 0; k < line.size(); ++k )
        {
            const Line2d & lk = line[k];
            const Vector2d dir ( a * lk.norm.x, b * lk.norm.y );
            const Vector2d p = getFarthestPoint ( inner, dir );
            const double t = dir * p + lk % v;
            if ( max < t ) max = t, pm = p, km = k;
        }
// Если нарушение мало, то завершаем программу
        if ( max < eps )
        {
            res = Affin2d ( ~conf ) * Affin2d ( Vector2d ( a, 0 ), Vector2d ( 0, b ), v );
            return true;
        }
// Применение ограничения к области допустимых преобразований
        const double x = line[km].norm.x;
        const double y = line[km].norm.y;
        Double<5> plane;
        plane.d0 = x * pm.x;
        plane.d1 = y * pm.y;
        plane.d2 = x;
        plane.d3 = y;
        plane.d4 = line[km].dist;
        model.cut ( plane, stor );
    }
    return false;
}

//**************************** 15.09.2018 *********************************//
//
//           Максимальный по площади прямоугольник вписанный
//              в выпуклый многоугольник без вращения
//
//**************************** 15.09.2018 *********************************//

Def<Rectangle2d> maxRectangleInConvexPolygonANR ( CCArrRef<Vector2d> & outer )
{
    Def<Rectangle2d> res;
    FixArray<Vector2d, 4> inner;
    inner[0] = Vector2d ( 1, 1 );
    inner[1] = Vector2d (-1, 1 );
    inner[2] = Vector2d (-1,-1 );
    inner[3] = Vector2d ( 1,-1 );
    Affin2d aff;
    if ( ! maxStretchPolygonInConvexPolygonANR ( inner, outer, aff ) ) return res;
    res.a = aff.t.x.x;
    res.b = aff.t.y.y;
    res.o = aff.s;
    res.isDef = true;
    return res;
}

//**************************** 15.09.2018 *********************************//
//
//              Максимальный по площади ромб вписанный
//              в выпуклый многоугольник без вращения
//
//**************************** 15.09.2018 *********************************//

Def<Rhombus2d> maxRhombusInConvexPolygonANR ( CCArrRef<Vector2d> & outer )
{
    Def<Rhombus2d> res;
    FixArray<Vector2d, 4> inner;
    inner[0] = Vector2d ( 1, 0 );
    inner[1] = Vector2d ( 0, 1 );
    inner[2] = Vector2d (-1, 0 );
    inner[3] = Vector2d ( 0,-1 );
    Affin2d aff;
    if ( ! maxStretchPolygonInConvexPolygonANR ( inner, outer, aff ) ) return res;
    res.a = aff.t.x.x;
    res.b = aff.t.y.y;
    res.o = aff.s;
    res.isDef = true;
    return res;
}

//**************************** 13.12.2008 *********************************//
//
//      Максимальный объект вписанный в выпуклый многоугольник
//
//**************************** 13.12.2008 *********************************//

class InnerObject
{
public:
    virtual Vector2d getPoint ( const Vector2d & dir ) const = 0;
    virtual nat symmetry () const = 0;
};

bool maxObjectInConvexPolygon ( InnerObject & inner, CArrRef<Vector2d> poly, Conform2d & res )
{
// Получение границ многоугольника в виде прямых линий
    Conform2d conf;
    DynArray<Line2d> line ( poly.size() );
    if ( ! points2lines ( poly, conf, line ) ) return false;
// Инициализация области допустимых преобразований
    List< Vertex<4> > stor;
    WireModel<4> model;
    model.simplex ( 10., stor );
    Double<4> d4;
    d4.d1 = d4.d2 = d4.d3 = 2.;
    d4.d0 = inner.symmetry() > 1 ? 0. : d4.d1;
    model.vlist -= d4;
    const double eps = 1e-7;
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
        if ( ptr == 0 )
            return false;
// Поиск максимального нарушения ограничений для выбранного решения
        const Vector2d v01 ( ptr->coor.d0, ptr->coor.d1 );
        const Vector2d v23 ( ptr->coor.d2, ptr->coor.d3 );
        Vector2d pm;
        nat k, km = 0;
        max = 0.;
        for ( k = 0; k < line.size(); ++k )
        {
            const Line2d & lk = line[k];
            const Vector2d v ( v01 * lk.norm, v01 % lk.norm );
            const Vector2d p ( inner.getPoint ( v ) );
            const double t = v * p + lk % v23;
            if ( max < t ) max = t, pm = p, km = k;
        }
// Если нарушение мало, то завершение программы
        if ( max < eps )
        {
            res = ~conf * Conform2d ( Spin2d ( atan2 ( v01.y, v01.x ) ), v23, sqrt ( v01 * v01 ) );
            return true;
        }
// Применение ограничения к области допустимых преобразований
        const double x = line[km].norm.x;
        const double y = line[km].norm.y;
        Double<5> plane;
        plane.d0 = x * pm.x + y * pm.y;
        plane.d1 = y * pm.x - x * pm.y;
        plane.d2 = x;
        plane.d3 = y;
        plane.d4 = line[km].dist;
        model.cut ( plane, stor );
    }
    return false;
}

//**************************** 16.10.2008 *********************************//
//
//      Максимальный многоугольник вписанный в выпуклый многоугольник
//
//**************************** 01.07.2011 *********************************//

class InnerPolygon : public InnerObject
{
    CArrRef<Vector2d> poly;
public:
    explicit InnerPolygon ( CArrRef<Vector2d> p ) : poly(p) {}
    Vector2d getPoint ( const Vector2d & dir ) const
    {
        nat i, im = 0;
        double max = dir * poly[0];
        for ( i = 1; i < poly.size(); ++i )
        {
            const double t = dir * poly[i];
            if ( max < t ) max = t, im = i;
        }
        return poly[im];
    }
    nat symmetry () const { return 1; }
};

Def<Conform2d> maxPolygonInConvexPolygon ( CCArrRef<Vector2d> & inner, CCArrRef<Vector2d> & outer )
{
    Def<Conform2d> res;
// Преобразование вершин внутреннего многоугольника к стандартному виду
    DynArray<Vector2d> temp ( inner );
    const Def<Conform2d> conf = setStandardPosition ( temp );
    if ( ! conf.isDef ) return res;
// Поиск максимального многоугольника вписанного в выпуклый многоугольник
    if ( maxObjectInConvexPolygon ( InnerPolygon ( temp ), outer, res ) )
    {
        res = res * conf;
        return res;
    }
    return res;
}

//**************************** 02.10.2018 *********************************//
//
// Максимальный многоугольник вписанный в выпуклый многоугольник без вращения
//
//**************************** 02.10.2018 *********************************//

Def<Conform2d> maxPolygonInConvexPolygonNR ( CCArrRef<Vector2d> & inner, CCArrRef<Vector2d> & outer )
{
    Def<Conform2d> res;
    const nat nl = outer.size();
    if ( nl < 3 ) return res;
    DynArray<Line2d> line ( nl );
    Conform2d confo;
    if ( ! points2lines ( outer, confo, line ) ) return res;
    Segment2d dimi = dimensions ( inner );
    Segment2d dimo = dimensions ( outer );
    const Def<Conform2d> confi = standardPosition ( dimi );
    if ( ! confi.isDef ) return res;
    dimi *= confi;
    dimo *= confo;
    Vector3d arr[4];
    arr[0] = Vector3d ( 0.5, 0.5, 1 );
    arr[1] = Vector3d ( -1., 0., 0. );
    arr[2] = Vector3d ( 0., -1., 0. );
    arr[3] = Vector3d ( 0., 0., -1. );
    nat k, n2 = nl + nl;
    for ( k = 0; k < n2; ++k )
    {
        Vector3d & a0 = arr[0];
        const double r = a0.z;
        const Vector2d o ( a0.x, a0.y );
        double dist = 0, u;
        nat i, j, im;
        for ( j = 0; j < inner.size(); ++j )
        {
            const Vector2d v = confi ( inner[j] );
            const Vector2d p = v * r + o;
            for ( i = 0; i < nl; ++i )
            {
                const double t = line[i] % p;
                if ( dist < t ) dist = t, im = i, u = line[i].norm * v;
            }
        }
        if ( dist <= 1e-9 )
        {
            return ~confo * Conform2d ( Spin2d(), o, r ) * confi;
        }
        const Vector2d & norm = line[im].norm;
        const Vector3d cor ( norm.x, norm.y, u );
        nat ib = 0;
        double sg, max;
        for ( i = 1; i < 4; ++i )
        {
            const Vector3d & v = arr[i];
            double t = cor * v;
            if ( t > -1e-8 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.z * t;
                ib = i;
                sg = t;
            }
            else
            {
                const double s = v.z * t;
                if ( s < max ) max = s, ib = i, sg = t;
            }
        }
        if ( ib == 0 )
        {
            return res;
        }
        const Vector3d & v = arr[ib];
        a0 -= v * ( dist * sg );
        for ( i = 1; i < 4; ++i )
        {
            if ( i == ib ) continue;
            Vector3d & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sg );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return res;
}

//**************************** 14.01.2012 *********************************//
//
// Максимальный выпуклый многоугольник вписанный в многоугольник без вращения
//
//**************************** 30.05.2014 *********************************//

static
Def<Vector3d> maxZ ( const Set3<Vector3d, nat> & p3, CArrRef<Set3<Plane3d[3], bool, const Vector2d*> > plane )
{
    Def<Vector3d> res;
    const double eps = 1e-9;
    Suite<Set3<Vector3d, nat> > stack;
    stack.inc() = p3;
    res.z = 0;
    while ( stack.size() > 0 )
    {
        const Set3<Vector3d, nat> & s3 = stack.las();
        const Vector3d start = s3.a;
        const nat i1 = s3.b;
        const nat i2 = s3.c;
        stack.dec();
        const Set3<Plane3d[3], bool, const Vector2d*> & o1 = plane[i1];
        const Set3<Plane3d[3], bool, const Vector2d*> & o2 = plane[i2];
        const Plane3d & plane1 = o1.a[0];
        const Plane3d & plane2 = o2.a[0];
        const Vector3d dir = ( plane2.norm % plane1.norm ).setNorm1();
        if ( i1 + 2 == i2 )
        {
            const Plane3d & plane3 = plane[i1+1].a[0];
            if ( plane3.norm * dir < eps ) continue;
            const Def<Vector3d> c = intersection ( plane1, plane2, plane3 );
            if ( c.isDef && res.z < c.z ) res = c;
            continue;
        }
        nat im = i1;
        double min = 1e300;
        Def<Vector3d> cc;
        for ( nat i = i1+1; i < i2; ++i )
        {
            const Set3<Plane3d[3], bool, const Vector2d*> & o3 = plane[i];
            const Plane3d & plane3 = o3.a[0];
            if ( plane3.norm * dir < eps ) continue;
            const Def<Vector3d> c = intersection ( plane1, plane2, plane3 );
            if ( c.z < 0 ) continue;
            if ( i + 1 < i2 && o3.a[2] % c > 0 ) continue;
            if ( i > i1 + 1 && o3.a[1] % c > 0 ) continue;
            double t = dir * ( c - start );
            if ( t < -1e-12 * c.z ) continue;
            if ( min > t ) min = t, cc = c, im = i;
        }
        if ( fabs ( o1.a[2].norm * dir ) > eps )
        {
            const Def<Vector3d> c = intersection ( plane1, plane2, o1.a[2] );
            double t = dir * ( c - start );
            if ( min > t ) min = t, cc = c, im = i1 + 1;
        }
        if ( fabs ( o2.a[1].norm * dir ) > eps )
        {
            const Def<Vector3d> c = intersection ( plane1, plane2, o2.a[1] );
            double t = dir * ( c - start );
            if ( min > t ) min = t, cc = c, im = i2 - 1;
        }
        if ( ! cc.isDef ) continue;
        if ( res.z < cc.z ) res = cc;
        if ( i1 + 1 < im )
        {
            stack.inc() = Set3<Vector3d, nat> ( cc, i1, im );
        }
        if ( im + 1 < i2 )
        {
            stack.inc() = Set3<Vector3d, nat> ( cc, im, i2 );
        }
    }
    return res;
}

Def<Conform2d> maxConvexPolygonInPolygonNR ( CArrRef<Vector2d> inner, CArrRef<Vector2d> outer,
                                             CArrRef<Vector2d> inn,   CArrRef<Vector2d> out )
{
    nat i, j;
    Def<Conform2d> res;
// Заполнение массива плоскостей
    Suite<Set3<Plane3d[3], bool, const Vector2d*> > plane ( 2*out.size() );
    Suite<nat> temp;
    for ( i = 0; i < out.size(); ++i )
    {
        const Vector2d & v1 = out.cprev(i);
        const Vector2d & v2 = out[i];
        const bool isConv = v1.x * v2.y > v1.y * v2.x;
        if ( ! isConv )
        {
            temp.resize(0);
            for ( j = 0; j < inn.size(); ++j )
            {
                const Vector2d & v = inn[j];
                if ( v.x * v1.y >= v.y * v1.x &&
                     v.x * v2.y <= v.y * v2.x )
                {
                    temp.inc() = j;
                }
            }
            if ( temp.size() > 0 )
            {
                if ( temp.size() > 1 )
                {
                    if ( temp.size() == inn.size() ) return res;
                    while ( temp[0] == temp.las() + 1 || temp[0] + inn.size() == temp.las() + 1 ) temp <<= 1;
                    temp.reverse();
                }
                for ( j = 0; j < temp.size(); ++j )
                {
                    const nat k = temp[j];
                    const Vector2d & v = inn[k];
                    Set3<Plane3d[3], bool, const Vector2d*> & p = plane.inc();
                    p.b = false; // невыпуклая вершина 
                    Plane3d & p0 = p.a[0];
                    p0.norm.x = v.x;
                    p0.norm.y = v.y;
                    p0.norm.z =  v * inner[k];
                    p0.dist = - ( v * outer[i] );
                    Plane3d & p1 = p.a[1];
                    p1.norm.x = v.y;
                    p1.norm.y = -v.x;
                    p1.norm.z = inner.cnext(k) % v;
                    p1.dist = v % outer[i];
                    Plane3d & p2 = p.a[2];
                    p2.norm.x = -v.y;
                    p2.norm.y = v.x;
                    p2.norm.z = v % inner[k];
                    p2.dist = - p1.dist;
                    p.c = &outer[i];
                }
            }
        }
        nat jm = 0;
        double max = v2 * inner[0];
        for ( j = 1; j < inner.size(); ++j )
        {
            const double t = v2 * inner[j];
            if ( max < t ) max = t, jm = j;
        }
        Set3<Plane3d[3], bool, const Vector2d*> & p = plane.inc();
        p.b = isConv; // выпуклая ли вершина 
        Plane3d & p0 = p.a[0];
        p0.norm.x = v2.x;
        p0.norm.y = v2.y;
        p0.norm.z = max;
        p0.dist = - ( v2 * outer[i] );
        Plane3d & p1 = p.a[1];
        p1.norm.x = v2.y;
        p1.norm.y = -v2.x;
        p1.norm.z = inner[jm] % v2;
        p1.dist = v2 % outer[i];
        Plane3d & p2 = p.a[2];
        p2.norm = - p1.norm;
        p2.dist = outer.cnext(i) % v2;
        p.c = &outer[i];
    }
// Первая вершина должна быть выпуклой
    if ( ! plane[0].b )
    {
        const nat n = 1 + plane.size() / 2;
        for ( i = 1; i < n; ++i )
        {
            if ( plane[i].b )
            {
                plane <<= i;
                break;
            }
            j = plane.size() - i;
            if ( plane[j].b )
            {
                plane >>= i;
                break;
            }
        }
        if ( i == n ) return res;
    }
// Поиск максимального вписанного многоугольника
    const Vector2d * start = plane[0].c;
    Def<Vector3d> v = maxZ ( Set3<Vector3d, nat> ( Vector3d ( start->x, start->y, 0 ), 0, plane.size()-1 ), plane );
    if ( v.isDef )
    {
        res.magn = v.z;
        res.trans = Vector2d ( v.x, v.y );
        res.isDef = true;
    }
    return res;
}

Def<Conform2d> maxConvexPolygonInPolygonNR ( CCArrRef<Vector2d> & inner, CCArrRef<Vector2d> & outer )
{
    Def<Conform2d> res;
    if ( inner.size() < 2 || outer.size() < 3 ) return res;
// Площадь внешнего многоугольника
    const double a = area ( outer );
    if ( a <= 0 ) return res;
// Определение габаритов внешнего многоугольника
    const Segment2d seg = dimensions ( outer );
// Проверка на малость площади по сравнению с габаритами многоугольника
    if ( a < 1e-9 * ( seg.b.x - seg.a.x ) * ( seg.b.y - seg.a.y ) ) return res;
// Для каждой стороны внешнего и внутреннего многоугольника вычислим нормаль
    CmbArray<Vector2d, 64> out ( outer.size() );
    nat i;
    for ( i = 0; i < outer.size(); ++i )
    {
        Vector2d & vi = out[i];
        const Vector2d & v1 = outer[i];
        const Vector2d & v2 = outer.cnext(i);
        vi.x = v2.y - v1.y;
        vi.y = v1.x - v2.x;
        if ( ! vi.setNorm2() ) return res;
    }
    CmbArray<Vector2d, 64> inn ( inner.size() );
    for ( i = 0; i < inner.size(); ++i )
    {
        Vector2d & vi = inn[i];
        const Vector2d & v1 = inner[i];
        const Vector2d & v2 = inner.cnext(i);
        vi.x = v2.y - v1.y;
        vi.y = v1.x - v2.x;
        if ( ! vi.setNorm2() ) return res;
    }
    // Поиск максимума
    return maxConvexPolygonInPolygonNR ( inner, outer, inn, out );
}

//**************************** 26.04.2014 *********************************//
//
//      Максимальный выпуклый многоугольник вписанный в многоугольник
//
//**************************** 26.04.2014 *********************************//

class MaxConvexPolygonInPolygon : public MathFunc1
{
    mutable DynArray<Vector2d> tmp1, tmp2;
    CArrRef<Vector2d> inner, outer, inn, out;
public:
    mutable Def<Conform2d> res;

    MaxConvexPolygonInPolygon ( CArrRef<Vector2d> pinner, CArrRef<Vector2d> pouter,
                                CArrRef<Vector2d> pinn,   CArrRef<Vector2d> pout ) : 
    inner(pinner), outer(pouter), inn(pinn), out(pout), tmp1(pinn.size()), tmp2(pinn.size())
    {
        res.magn = 0;
    }
    virtual double operator () ( double p ) const
    {
        const Spin2d spin ( p, true );
        for ( nat i = 0; i < inn.size(); ++i )
        {
            tmp1[i] = spin ( inner[i] );
            tmp2[i] = spin ( inn[i] );
        }
        Def<Conform2d> conf = maxConvexPolygonInPolygonNR ( tmp1, outer, tmp2, out );
        if ( conf.isDef && res.magn < conf.magn )
        {
            res.spin = spin;
            res.trans = conf.trans;
            res.magn = conf.magn;
            res.isDef = true;
        }
        return conf.isDef ? conf.magn : 0;
    }
};

Def<Conform2d> maxConvexPolygonInPolygon ( CCArrRef<Vector2d> & inner, CCArrRef<Vector2d> & outer )
{
    Def<Conform2d> res;
    if ( inner.size() < 2 || outer.size() < 3 ) return res;
// Если внешний многоугольник выпуклый, то вызываем другую функцию
    if ( isConvex ( outer ) ) 
        return maxPolygonInConvexPolygon ( inner, outer );
// Площадь внешнего многоугольника
    const double a = area ( outer );
    if ( a <= 0 ) return res;
// Определение габаритов внешнего многоугольника
    const Segment2d seg = dimensions ( outer );
// Проверка на малость площади по сравнению с габаритами многоугольника
    if ( a < 1e-9 * ( seg.b.x - seg.a.x ) * ( seg.b.y - seg.a.y ) ) return res;
// Для каждой стороны внешнего и внутреннего многоугольника вычислим нормаль
    CmbArray<Vector2d, 64> out ( outer.size() );
    nat i;
    for ( i = 0; i < outer.size(); ++i )
    {
        Vector2d & vi = out[i];
        const Vector2d & v1 = outer[i];
        const Vector2d & v2 = outer.cnext(i);
        vi.x = v2.y - v1.y;
        vi.y = v1.x - v2.x;
        if ( ! vi.setNorm2() ) return res;
    }
    CmbArray<Vector2d, 64> inn ( inner.size() );
    for ( i = 0; i < inner.size(); ++i )
    {
        Vector2d & vi = inn[i];
        const Vector2d & v1 = inner[i];
        const Vector2d & v2 = inner.cnext(i);
        vi.x = v2.y - v1.y;
        vi.y = v1.x - v2.x;
        if ( ! vi.setNorm2() ) return res;
    }
    // Поиск максимума
    CmbArray<Vector2d, 64> tmp;
    tmp = inner;
    const nat n = 20;
    const Spin2d spin ( 360. / n, true );
    res.magn = 0;
    int im;
    for ( i = 0; i < n; ++i )
    {
        Def<Conform2d> conf = maxConvexPolygonInPolygonNR ( tmp, outer, inn, out );
        if ( conf.isDef && res.magn < conf.magn )
        {
            res.spin = Spin2d ( i * 360. / n, true );
            res.trans = conf.trans;
            res.magn = conf.magn;
            res.isDef = true;
            im = i;
        }
        tmp *= spin;
        inn *= spin;
    }
    MaxConvexPolygonInPolygon func ( inner, outer, inn, out );
    fmax ( (im-1) * 360. / n, (im+1) * 360. / n, func, 0.1 );
    return func.res;
}

//**************************** 23.04.2009 *********************************//
//
//                  Вспомогательные классы и функции
//
//**************************** 24.10.2015 *********************************//

static double findMax ( const MathFunc1 & func )
{
    nat im = 9;
    double max = func ( 1 );
    for ( nat i = 0; i < 9; ++i )
    {
        const double a = func ( ( 1 + i * 7 ) / 64. );
        if ( max < a ) max = a, im = i;
    }
    const double a = im == 0 ? 0. : ( im * 7 - 6 ) / 64.;
    const double b = im == 9 ? 1. : ( im * 7 + 8 ) / 64.;
    return fmax ( a, b, func, 0.001 );
}

class MaxFigureInPolygon : public MathFunc1
{
protected:
    mutable FixArray<Vector2d, 4> vert;
    mutable Def<Conform2d> conf;
    mutable double max;
    CArrRef<Vector2d> poly;
    Def<Conform2d> (*func) ( CCArrRef<Vector2d> &, CCArrRef<Vector2d> & );
public:

    MaxFigureInPolygon ( CArrRef<Vector2d> p,
                         Def<Conform2d> (*f) ( CCArrRef<Vector2d> &, CCArrRef<Vector2d> & ) ) : 
                         poly(p), max(0), func(f) {}
};

class MaxRectangleInPolygon : public MaxFigureInPolygon
{
public:

    MaxRectangleInPolygon ( CArrRef<Vector2d> p,
                            Def<Conform2d> (*f) ( CCArrRef<Vector2d> &, CCArrRef<Vector2d> & ) ) : 
                            MaxFigureInPolygon ( p, f )
    {
        vert[0].x = vert[3].x = +1;
        vert[1].x = vert[2].x = -1;
    }
    virtual double operator () ( double y ) const
    {
        vert[0].y = vert[1].y = +y;
        vert[2].y = vert[3].y = -y;
        Def<Conform2d> c = func ( vert, poly );
        if ( ! c.isDef ) return 0;
        const double a = c.magn * c.magn * y;
        if ( max < a ) max = a, conf = c;
        return a;
    }
    Def<Rectangle2d> run () const
    {
        Def<Rectangle2d> res;
        const double y = findMax ( *this );
        if ( conf.isDef )
        {
            res.a = conf.magn;
            res.b = conf.magn * y;
            res.spin = conf.spin;
            res.o = conf.trans;
            res.isDef = true;
        }
        return res;
    }
};

class MaxRhombusInPolygon : public MaxFigureInPolygon
{
public:

    MaxRhombusInPolygon ( CArrRef<Vector2d> p,
                        Def<Conform2d> (*f) ( CCArrRef<Vector2d> &, CCArrRef<Vector2d> & ) ) : 
                        MaxFigureInPolygon ( p, f )
    {
        vert[0].x = 1;
        vert[1].x = vert[3].x = 0;
        vert[2].x = -1;
        vert[0].y = vert[2].y = 0;
    }
    Def<Rhombus2d> run () const
    {
        Def<Rhombus2d> res;
        const double y = findMax ( *this );
        if ( conf.isDef )
        {
            res.a = conf.magn;
            res.b = conf.magn * y;
            res.spin = conf.spin;
            res.o = conf.trans;
            res.isDef = true;
        }
        return res;
    }
};

class MaxRhombusInPolygonA : public MaxRhombusInPolygon
{
public:

    MaxRhombusInPolygonA ( CArrRef<Vector2d> p,
                         Def<Conform2d> (*f) ( CCArrRef<Vector2d> &, CCArrRef<Vector2d> & ) ) : 
                         MaxRhombusInPolygon ( p, f ) {}
    virtual double operator () ( double y ) const
    {
        vert[1].y =  y;
        vert[3].y = -y;
        Def<Conform2d> c = func ( vert, poly );
        if ( ! c.isDef ) return 0;
        const double a = c.magn * c.magn * y;
        if ( max < a ) max = a, conf = c;
        return a;
    }
};

class MaxRhombusInPolygonP : public MaxRhombusInPolygon
{
public:

    MaxRhombusInPolygonP ( CArrRef<Vector2d> p,
                         Def<Conform2d> (*f) ( CCArrRef<Vector2d> &, CCArrRef<Vector2d> & ) ) : 
                         MaxRhombusInPolygon ( p, f ) {}
    virtual double operator () ( double y ) const
    {
        vert[1].y =  y;
        vert[3].y = -y;
        Def<Conform2d> c = func ( vert, poly );
        if ( ! c.isDef ) return 0;
        const double a = c.magn * c.magn * ( 1 + y * y );
        if ( max < a ) max = a, conf = c;
        return a;
    }
};

//**************************** 23.04.2009 *********************************//
//
//      Максимальный прямоугольник вписанный в выпуклый многоугольник
//                      A - максимум площади
//
//**************************** 17.10.2015 *********************************//

Def<Rectangle2d> maxRectangleInConvexPolygonA ( CCArrRef<Vector2d> & poly )
{
    return MaxRectangleInPolygon ( poly, maxPolygonInConvexPolygon ).run();
}

//**************************** 07.06.2014 *********************************//
//
//     Максимальный прямоугольник вписанный в невыпуклый многоугольник
//                      A - максимум площади
//
//**************************** 24.10.2015 *********************************//

Def<Rectangle2d> maxRectangleInPolygonA ( CCArrRef<Vector2d> & poly )
{
    return MaxRectangleInPolygon ( poly, maxConvexPolygonInPolygon ).run();
}

//**************************** 02.10.2018 *********************************//
//
//           Максимальный по площади прямоугольник вписанный
//              в невыпуклый многоугольник без вращения
//
//**************************** 02.10.2018 *********************************//

Def<Rectangle2d> maxRectangleInPolygonANR ( CCArrRef<Vector2d> & poly )
{
    return MaxRectangleInPolygon ( poly, maxConvexPolygonInPolygonNR ).run();
}

//**************************** 24.04.2009 *********************************//
//
//     Максимальный по площади ромб вписанный в выпуклый многоугольник
//
//**************************** 24.10.2015 *********************************//

Def<Rhombus2d> maxRhombusInConvexPolygonA ( CCArrRef<Vector2d> & poly )
{
    return MaxRhombusInPolygonA ( poly, maxPolygonInConvexPolygon ).run();
}

//**************************** 26.04.2009 *********************************//
//
//     Максимальный по периметру ромб вписанный в выпуклый многоугольник
//
//**************************** 24.10.2015 *********************************//

Def<Rhombus2d> maxRhombusInConvexPolygonP ( CCArrRef<Vector2d> & poly )
{
    return MaxRhombusInPolygonP ( poly, maxPolygonInConvexPolygon ).run();
}

//**************************** 07.06.2014 *********************************//
//
//     Максимальный по площади ромб вписанный в невыпуклый многоугольник
//
//**************************** 24.10.2015 *********************************//

Def<Rhombus2d> maxRhombusInPolygonA ( CCArrRef<Vector2d> & poly )
{
    return MaxRhombusInPolygonA ( poly, maxConvexPolygonInPolygon ).run();
}

//**************************** 24.10.2015 *********************************//
//
//     Максимальный по периметру ромб вписанный в невыпуклый многоугольник
//
//**************************** 24.10.2015 *********************************//

Def<Rhombus2d> maxRhombusInPolygonP ( CCArrRef<Vector2d> & poly )
{
    return MaxRhombusInPolygonP ( poly, maxConvexPolygonInPolygon ).run();
}

//**************************** 02.10.2018 *********************************//
//
//              Максимальный по площади ромб вписанный
//              в невыпуклый многоугольник без вращения
//
//**************************** 02.10.2018 *********************************//

Def<Rhombus2d> maxRhombusInPolygonANR ( CCArrRef<Vector2d> & poly )
{
    return MaxRhombusInPolygonA ( poly, maxConvexPolygonInPolygonNR ).run();
}

//**************************** 01.05.2009 *********************************//
//
//      Максимальный треугольник вписанный в выпуклый многоугольник
//
//**************************** 18.02.2018 *********************************//

Def<Triangle2d> maxTriangleInConvexPolygonA ( CCArrRef<Vector2d> & poly )
{
    Def<Triangle2d> triangle;
    if ( poly.size() < 3 ) return triangle;
    nat m0(0), m1(1), m2(2);
    double max = ( poly[m1] - poly[m0] ) % ( poly[m2] - poly[m1] );
    for ( nat i0 =    0; i0 < poly.size() - 2; ++i0 )
    for ( nat i1 = i0+1; i1 < poly.size() - 1; ++i1 )
    for ( nat i2 = i1+1; i2 < poly.size()    ; ++i2 )
    {
        const double d = ( poly[i1] - poly[i0] ) % ( poly[i2] - poly[i1] );
        if ( max < d )
        {
            max = d;
            m0 = i0;
            m1 = i1;
            m2 = i2;
        }
    }
    triangle.a = poly[m0];
    triangle.b = poly[m1];
    triangle.c = poly[m2];
    triangle.isDef = true;
    return triangle;
}

//**************************** 01.05.2009 *********************************//
//
//      Максимальный параллелограмм вписанный в выпуклый многоугольник
//
//**************************** 12.02.2018 *********************************//

static bool area2 ( const WireModel<6> & model, Double<6> & best )
{
// Поиск максимального решения на вершинах и рёбрах
    Show< Vertex<6> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Vertex<6> * p = show.cur();
        const Double<6> & pc = p->coor;
        const double t = pc.d0 * pc.d4 - pc.d1 * pc.d3;
        if ( max < t ) max = t, best = pc;
        for ( nat k = 0; k < 6; ++k )
        {
            const Vertex<6> * v = p->vertex[k];
            if ( v < p ) continue;
            const Double<6> & vc = v->coor;
            const double a0 = vc.d0 - pc.d0;
            const double a1 = vc.d1 - pc.d1;
            const double a3 = vc.d3 - pc.d3;
            const double a4 = vc.d4 - pc.d4;
            const double a = a0 * a4 - a1 * a3;
            if ( a == 0 ) continue;
            const double t = 0.5 * ( a1 * pc.d3 + a3 * pc.d1 - a0 * pc.d4 - a4 * pc.d0 ) / a;
            if ( t >= 0 && t <= 1 )
            {
                const double e0 = pc.d0 + a0 * t;
                const double e1 = pc.d1 + a1 * t;
                const double e3 = pc.d3 + a3 * t;
                const double e4 = pc.d4 + a4 * t;
                const double p = e0 * e4 - e1 * e3;
                if ( p > max )
                {
                    max = p;
                    const double s = 1 - t;
                    best.d0 = e0;
                    best.d1 = e1;
                    best.d2 = pc.d2 * s + vc.d2 * t;
                    best.d3 = e3;
                    best.d4 = e4;
                    best.d5 = pc.d5 * s + vc.d5 * t;
                }
            }
        }
    }
    while ( show.next() );
    return max > 0;
}

static bool perimeter ( const WireModel<6> & model, Double<6> & best )
{
// Поиск максимального решения на вершинах
    Show< Vertex<6> > show ( model.vlist );
    double max = 0.;
    if ( show.top() )
    do
    {
        const Vertex<6> * p = show.cur();
        const Double<6> & pc = p->coor;
        const double t = sqrt ( pc.d0 * pc.d0 + pc.d3 * pc.d3 ) + sqrt ( pc.d1 * pc.d1 + pc.d4 * pc.d4 );
        if ( max < t ) max = t, best = pc;
    }
    while ( show.next() );
    return max > 0;
}

static
bool maxPointsInConvexPolygon ( CArrRef<Vector2d> poly1, CCArrRef<Vector2d> & poly2, 
                                WireModel<6> & model, List< Vertex<6> > & stor,
                                Affin2d & res, bool (*func) ( const WireModel<6> &, Double<6> & ) )
{
// Получение границ многоугольника в виде прямых линий
    Conform2d conf2;
    DynArray<Line2d> line ( poly2.size() );
    if ( ! points2lines ( poly2, conf2, line ) ) return false;
    for ( nat i = 0; i < 1000; ++i )
    {
// Поиск максимального решения
        Double<6> best;
        if ( ! func ( model, best ) ) return false;
// Поиск максимального нарушения ограничений для выбранного решения
        nat km;
        Vector2d pm;
        double max = 0.;
        for ( nat j = 0; j < poly1.size(); ++j )
        {
            const Vector2d & p = poly1[j];
            const Vector2d p1 ( best.d0*p.x + best.d1*p.y + best.d2,
                                best.d3*p.x + best.d4*p.y + best.d5 );
            for ( nat k = 0; k < line.size(); ++k )
            {
                const double t = line[k] % p1;
                if ( max < t ) max = t, pm = p, km = k;
            }
        }
// Если нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            const Affin2d at ( Vector2d ( best.d0, best.d1 ), 
                               Vector2d ( best.d3, best.d4 ),
                               Vector2d ( best.d2, best.d5 ) );
            res = Affin2d ( ~conf2 ) * at;
            return true;
        }
// Применение ограничения к области допустимых преобразований
        const double nx = line[km].norm.x;
        const double ny = line[km].norm.y;
        Double<7> plane;
        plane.d0 = nx * pm.x;
        plane.d1 = nx * pm.y;
        plane.d2 = nx;
        plane.d3 = ny * pm.x;
        plane.d4 = ny * pm.y;
        plane.d5 = ny;
        plane.d6 = line[km].dist;
        model.cut ( plane, stor );
    }
    return false;
}

Def<Parallelogram2d> maxParallelogramInConvexPolygon ( CCArrRef<Vector2d> & poly, bool (*func) ( const WireModel<6> &, Double<6> & ) )
{
// Инициализация области допустимых преобразований
    List< Vertex<6> > stor;
    WireModel<6> model;
    model.simplex ( 2*(6+1), stor );
    Double<6> dn;
    dn.fill ( 2 );
    dn.d0 = dn.d1 = 0;
    model.vlist -= dn;
// Поиск оптимального преобразования
    Affin2d at;
    FixArray<Vector2d, 4> vert;
    vert[0] = Vector2d ( 0.5,-0.5 );
    vert[1] = Vector2d ( 0.5, 0.5 );
    vert[2] = Vector2d (-0.5, 0.5 );
    vert[3] = Vector2d (-0.5,-0.5 );
    if ( ! maxPointsInConvexPolygon ( vert, poly, model, stor, at, func ) ) return Def<Parallelogram2d>();
    return Parallelogram2d ( vert[0] *= at, vert[1] *= at, vert[2] *= at );
}

Def<Parallelogram2d> maxParallelogramInConvexPolygonA ( CCArrRef<Vector2d> & poly )
{
    return maxParallelogramInConvexPolygon ( poly, area2 );
}

Def<Parallelogram2d> maxParallelogramInConvexPolygonP ( CCArrRef<Vector2d> & poly )
{
    return maxParallelogramInConvexPolygon ( poly, perimeter );
}

//**************************** 01.05.2009 *********************************//
//
//      Максимальный четырёхугольник вписанный в выпуклый многоугольник
//
//**************************** 01.05.2009 *********************************//

bool maxQuadInConvexPolygonA ( FixArrRef<Vector2d, 4> & quad, CCArrRef<Vector2d> & poly )
{
    if ( poly.size() < 4 ) return false;
    nat m0(0), m1(1), m2(2), m3(3);
    double max = ( poly[m1] - poly[m0] ) % ( poly[m2] - poly[m1] ) +
                 ( poly[m2] - poly[m0] ) % ( poly[m3] - poly[m2] );
    for ( nat i0 =    0; i0 < poly.size() - 3; ++i0 )
    for ( nat i1 = i0+1; i1 < poly.size() - 2; ++i1 )
    for ( nat i2 = i1+1; i2 < poly.size() - 1; ++i2 )
    for ( nat i3 = i2+1; i3 < poly.size()    ; ++i3 )
    {
        const double d = ( poly[i1] - poly[i0] ) % ( poly[i2] - poly[i1] ) +
                         ( poly[i2] - poly[i0] ) % ( poly[i3] - poly[i2] );
        if ( max < d )
        {
            max = d;
            m0 = i0;
            m1 = i1;
            m2 = i2;
            m3 = i3;
        }
    }
    quad[0] = poly[m0];
    quad[1] = poly[m1];
    quad[2] = poly[m2];
    quad[3] = poly[m3];
    return true;
}

//**************************** 04.12.2015 *********************************//
//
//       Максимальный N-угольник вписанный в выпуклый многоугольник
//
//**************************** 04.12.2015 *********************************//

inline void _swap ( _ChainLink<Item1n> & p1, _ChainLink<Item1n> & p2 )
{
    const _ChainLink<Item1n> p ( p1 );
    p1 = p2;
    p2 = p;
    ::_swap ( p1.tail->info, p2.tail->info );
}

class NListMaxArea
{
    List1n & list;
    CArrRef<Vector2d> vert;
public:
    NListMaxArea ( List1n & l, CArrRef<Vector2d> v ) : list(l), vert(v) {}
    double operator() ( const Item1n * cp )
    {
        Show<Item1n> show ( list );
        const Item1n * pp = show.jump ( cp ).cprev();
        const Item1n * np = show.jump ( cp ).cnext();
        return fabs ( ( vert[cp->a] - vert[pp->a] ) % ( vert[cp->a] - vert[np->a] ) );
    }
    void operator() ( Item1n * pp, Item1n * cp, Item1n * np )
    {
        list.del ( cp );
    }
};

bool maxNgonInConvexPolygonA ( ArrRef<Vector2d> & inner, CCArrRef<Vector2d> & outer )
{
    const nat m = outer.size();
    const nat n = inner.size();
    if ( n < 3 || m < n ) return false;
    if ( n == m )
    {
        inner = outer;
        return true;
    }
// Построим N-угольник
    List1n list;
    nat i;
    for ( i = 0; i < m; ++i )
    {
        list.addAftCur ( new Item1n ( i, i ) );
    }
    NListMaxArea ma ( list, outer );
    simplifyChain ( list, ma, ma, n );
// Подвигаем вершины N-угольника
    for ( nat k = 0; k < 4; ++k )
    {
        bool isChange = false;
// Вначале по одной
        nat ip = list.las()->a;
        list.top();
        do
        {
            const nat i1 = list.cur()->a;
            const nat in = list.cnext()->a;
            list.cprev();
            const Vector2d u = outer[in] - outer[ip];
            double t1 = fabs ( u % ( outer[i1] - outer[ip] ) );
            nat i2 = i1 > 0 ? i1 - 1 : m - 1;
            double t2 = fabs ( u % ( outer[i2] - outer[ip] ) );
            if ( t1 < t2 )
            {
                do
                {
                    list.cur()->a = i2;
                    i2 = i2 > 0 ? i2 - 1 : m - 1;
                    t1 = t2;
                    t2 = fabs ( u % ( outer[i2] - outer[ip] ) );
                }
                while ( t1 < t2 );
                isChange = true;
            }
            else
            {
                i2 = i1 + 1;
                if ( i2 == m ) i2 = 0;
                t2 = fabs ( u % ( outer[i2] - outer[ip] ) );
                while ( t1 < t2 )
                {
                    list.cur()->a = i2;
                    if ( ++i2 == m ) i2 = 0;
                    t1 = t2;
                    t2 = fabs ( u % ( outer[i2] - outer[ip] ) );
                    isChange = true;
                }
            }
            ip = list.cur()->a;
        }
        while ( list.next() );
// Потом по две
        ip = list.las()->a;
        list.top();
        do
        {
            const nat a1 = list.cur()->a;
            const nat b1 = list.cnext()->a;
            const nat in = list.cnext()->a;
            list.cprev();
            list.cprev();
            const Vector2d u = outer[in] - outer[ip];
            double t1 = fabs ( u % ( outer[a1] - outer[ip] ) ) + fabs ( ( outer[in] - outer[b1] ) % ( outer[a1] - outer[b1] ) );
            nat a2 = a1 > 0 ? a1 - 1 : m - 1;
            nat b2 = b1 > 0 ? b1 - 1 : m - 1;
            double t2 = fabs ( u % ( outer[a2] - outer[ip] ) ) + fabs ( ( outer[in] - outer[b2] ) % ( outer[a2] - outer[b2] ) );
            if ( t1 < t2 )
            {
                do
                {
                    list.cur()->a = a2;
                    list.cnext()->a = b2;
                    list.cprev();
                    a2 = a1 > 0 ? a1 - 1 : m - 1;
                    b2 = b1 > 0 ? b1 - 1 : m - 1;
                    t1 = t2;
                    t2 = fabs ( u % ( outer[a2] - outer[ip] ) ) + fabs ( ( outer[in] - outer[b2] ) % ( outer[a2] - outer[b2] ) );
                }
                while ( t1 < t2 );
                isChange = true;
            }
            else
            {
                a2 = a1 + 1;
                if ( a2 == m ) a2 = 0;
                b2 = b1 + 1;
                if ( b2 == m ) b2 = 0;
                t2 = fabs ( u % ( outer[a2] - outer[ip] ) ) + fabs ( ( outer[in] - outer[b2] ) % ( outer[a2] - outer[b2] ) );
                while ( t1 < t2 )
                {
                    list.cur()->a = a2;
                    list.cnext()->a = b2;
                    list.cprev();
                    if ( ++a2 == m ) a2 = 0;
                    if ( ++b2 == m ) b2 = 0;
                    t1 = t2;
                    t2 = fabs ( u % ( outer[a2] - outer[ip] ) ) + fabs ( ( outer[in] - outer[b2] ) % ( outer[a2] - outer[b2] ) );
                    isChange = true;
                }
            }
            ip = list.cur()->a;
        }
        while ( list.next() );
        if ( ! isChange ) break;
    }
// Запись результата
    list.top();
    for ( i = 0; i < n; ++i )
    {
        inner[i] = outer[list.cur()->a];
        list.next();
    }
    return true;
}