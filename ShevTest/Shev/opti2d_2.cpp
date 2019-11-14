
#include "math.h"

#include "tune.h"
#include "func2d.h"
#include "Vector2d.h"
#include "ShevArray2.h"
#include "LinAlg.h"
#include "Mathem.h"
#include "opti1d.h"
#include "opti2d.h"
#include "intersect2d.h"
#include "intersect3d.h"
#include "func1t.h"
#include "heap.h"
#include "WireModel.h"

bool points2lines ( CCArrRef<Vector2d> & poly, ArrRef<Line2d> & line );
bool points2lines ( CCArrRef<Vector2d> & poly, Conform2d & conf, ArrRef<Line2d> & line );

//**************************** 19.06.2004 *********************************//
//
//      Максимальная окружность вписанная в выпуклый многоугольник
//                       за время O ( n*log(n) )
//
//**************************** 05.07.2011 *********************************//

namespace
{

struct Circle
{
    Vector2d o;              // центр окружности
    double   r;              // её радиус
    nat i1, i2, i3; // номера касательных сторон
    nat info;       // дополнительная информация
};

bool calc ( nat i1, nat i2, nat i3, const Line2d * line, Circle & circle )
{
    const Vector2d v1 = line[i1].norm - line[i2].norm;
    const Vector2d v2 = line[i3].norm - line[i2].norm;
    const double p = v2 % v1;
    if ( p == 0 ) return false;
    const double d1 = line[i1].dist - line[i2].dist;
    const double d2 = line[i3].dist - line[i2].dist;
    circle.o.x = ( v2.y * d1 - v1.y * d2 ) / p;
    circle.o.y = ( v1.x * d2 - v2.x * d1 ) / p;
    circle.r = - ( line[i2] % circle.o );
    circle.i1 = i1;
    circle.i2 = i2;
    circle.i3 = i3;
    return true;
}

} // end of namespace

Def<Circle2d> maxCircle_NlogN ( CCArrRef<Line2d> & line )
{
    Def<Circle2d> res;
    const nat n = line.size();
    if ( n < 3 ) return res;
// Проверяем многоугольник на выпуклость
    nat i;
    for ( i = 0; i < n; ++i )
    {
        if ( line[i>0?i-1:n-1].norm % line[i].norm <= 0 ) return res;
    }
// Для каждой стороны строим окружность
    DynArray<Circle> circle ( n );
    for ( i = 0; i < n; ++i )
    {
        if ( ! calc ( i > 0 ? i-1 : n-1, i, i < n-1 ? i+1 : 0, &line[0], circle[i] ) )
        {
            return res;
        }
        circle[i].info = i + 1;
    }
// Строим пирамиду
    DynArray<Circle*> heap ( n+1 );
    for ( i = 0; i < n; ++i )
    {
        nat j = i + 1;
        heap[j] = &circle[i];
        for ( ; j > 1; )
        {
            const nat k = j / 2;
            Circle * ck = heap[k];
            Circle * cj = heap[j];
            if ( cj->r > ck->r ) break;
            heap[j] = ck;
            ck->info = j;
            heap[k] = cj;
            cj->info = k;
            j = k;
        }
    }
    nat m = n;
    while ( m > 3 )
    {
        Circle & c2 = *heap[1];
// Убираем окружность с минимальным радиусом
        heap[1] = heap[m];
        --m;
// Упорядочиваем пирамиду
        i = 1;
        for(;;)
        {
            const nat i1 = i + i;
            if ( i1 > m ) break;
            const nat i2 = i1 + 1;
            const nat j = i2 > m ? i1 : heap[i1]->r < heap[i2]->r ? i1 : i2;
            Circle * ci = heap[i];
            Circle * cj = heap[j];
            if ( ci->r <= cj->r ) break;
            heap[j] = ci;
            ci->info = j;
            heap[i] = cj;
            cj->info = i;
            i = j;
        }
// Пересчитываем соседние окружности
        Circle & c1 = circle[c2.i1];
        calc ( c1.i1, c1.i2, c2.i3, &line[0], c1 );
        nat j = c1.info;
        for ( ; j > 1; )
        {
            const nat k = j / 2;
            Circle * ck = heap[k];
            Circle * cj = heap[j];
            if ( cj->r > ck->r ) break;
            heap[j] = ck;
            ck->info = j;
            heap[k] = cj;
            cj->info = k;
            j = k;
        }
        Circle & c3 = circle[c2.i3];
        calc ( c2.i1, c3.i2, c3.i3, &line[0], c3 );
        j = c3.info;
        for ( ; j > 1; )
        {
            const nat k = j / 2;
            Circle * ck = heap[k];
            Circle * cj = heap[j];
            if ( cj->r > ck->r ) break;
            heap[j] = ck;
            ck->info = j;
            heap[k] = cj;
            cj->info = k;
            j = k;
        }
    }
    res.r = heap[3]->r;
    res.o = heap[3]->o;
    res.isDef = true;
    return res;
}

Def<Circle2d> maxCircle_NlogN ( CCArrRef<Vector2d> & vert )
{
    DynArray<Line2d> line ( vert.size() );
    if ( ! points2lines ( vert, line ) ) return Def<Circle2d>();
    return maxCircle_NlogN ( line );
}


//**************************** 22.09.2006 *********************************//
//
//      Максимальная окружность вписанная в выпуклый многоугольник
//
//**************************** 22.02.2011 *********************************//

Def<Circle2d> maxCircleInConvexPolygon ( CCArrRef< Line2d > & line, nat & i1, nat & i2, nat & i3 )
{
    Def<Circle2d> res;
    const nat n = line.size();
    if ( n < 3 ) return res;
// Проверяем многоугольник на выпуклость
    nat i;
    for ( i = 0; i < n; ++i )
    {
        if ( line.cprev(i).norm % line[i].norm < 1e-12 ) return res;
    }
    i1 = 0;
    i2 = n - 1;
    while ( i2 > i1 + 2 )
    {
// Поиск нового ограничения
        const Vector2d v1 = line[i2].norm - line[i1].norm;
        const double   d1 = line[i2].dist - line[i1].dist;
        double max = 0;
        i3 = n;
        for ( i = i1 + 1; i < i2; ++i )
        {
            const Vector2d v2 = line[i].norm - line[i1].norm;
            const double   d2 = line[i].dist - line[i1].dist;
            const double p = v2 % v1;
            const Vector2d c ( v2.y * d1 - v1.y * d2,
                               v1.x * d2 - v2.x * d1 );
            const double t = ( line[i1].norm * c ) / p;
            if ( i3 == n || max < t )
            {
                i3 = i;
                max = t;
            }
        }
        if ( line[i1].norm % line[i3].norm < 0 )
        {
            i2 = i3;
            continue;
        }
        if ( line[i3].norm % line[i2].norm < 0 )
        {
            i1 = i3;
            continue;
        }
// Вычисление параметров найденной окружности
        res.r = - max - line[i1].dist;
        const Vector2d v2 = line[i3].norm - line[i1].norm;
        const double   d2 = line[i3].dist - line[i1].dist;
        const double p = v2 % v1;
        res.o.x = ( v2.y * d1 - v1.y * d2 ) / p;
        res.o.y = ( v1.x * d2 - v2.x * d1 ) / p;
        res.isDef = true;
        return res;
    }
// Строим окружность по 3 ограничениям
    i3 = i1 + 1;
    const Vector2d v1 = line[i3].norm - line[i1].norm;
    const Vector2d v2 = line[i3].norm - line[i2].norm;
    const double   d1 = line[i3].dist - line[i1].dist;
    const double   d2 = line[i3].dist - line[i2].dist;
    const double p = v2 % v1;
    res.o.x = ( v2.y * d1 - v1.y * d2 ) / p;
    res.o.y = ( v1.x * d2 - v2.x * d1 ) / p;
    res.r = - line[i3] % res.o;
    res.isDef = true;
    return res;
}

Def<Circle2d> maxCircleInConvexPolygon ( CCArrRef< Line2d > & line )
{
    nat i1, i2, i3;
    return maxCircleInConvexPolygon ( line, i1, i2, i3 );
}

Def<Circle2d> maxCircleInConvexPolygon ( CCArrRef<Vector2d> & vert )
{
    const nat n = vert.size();
    if ( n < 3 ) return Def<Circle2d>();
    const nat n1 = n - 1;
// Поиск перспективной вершины
    nat i, shift = n1;
    Vector2d v1 = vert[0] - vert[n1];
    double max = ( vert[n1] - vert[n1-1] ) * v1;
    for ( i = 0; i < n1; ++i )
    {
        const Vector2d v2 = vert[i+1] - vert[i];
        const double q = v1 * v2;
        if ( max < q ) shift = i, max = q;
        v1 = v2;
    }
// Для каждой стороны вычислим параметры уравнения прямой
// Уравнение прямой имеет вид: norm.x * x + norm.y * y + dist = 0
    DynArray<Line2d> line ( n );
    for ( i = 0; i < n; ++i )
    {
        nat k = i + shift;
        if ( k >= n ) k -= n;
        const Vector2d & v2 = vert.cnext(k);
        const Vector2d & v1 = vert[k];
        Vector2d & norm = line[i].norm;
        norm.x = v2.y - v1.y;
        norm.y = v1.x - v2.x;
        const double q = norm * norm;
        if ( q == 0 ) return Def<Circle2d>();
        norm /= sqrt ( q );
        line[i].dist = - ( norm * v1 );
    }
// Находим окружность с максимальным радиусом
    return maxCircleInConvexPolygon ( line );
}

//**************************** 19.03.2008 *********************************//
//
//      Максимальная окружность вписанная в невыпуклый многоугольник
//
//**************************** 31.05.2014 *********************************//

namespace
{

// Проверка попадания проекции центра круга за пределы отрезка

bool isOut ( const Vector2d & a, const Vector2d & b, const Vector2d & c )
{
    const Vector2d v ( b - a );
    return ( c - a ) * v < 0 || ( c - b ) * v > 0;
}

Def<Circle2d> circleVLLL ( const Vector2d & o, const Line2d & line0, const Line2d & line1, const Line2d & line2 )
{
    Def<Circle2d> res;
    const Vector2d v = line1.norm + line0.norm;
    const Vector2d w = line2.norm - line0.norm;
    const double p = v * w;
    if ( fabs ( p ) < 1e-6 ) 
        return res;
    res.o = o - v * ( ( line2 % o ) / p );
    res.r = - ( line2 % res.o );
    res.isDef = true;
    return res;
}

Def<Circle2d> maxCircleInPolygon ( const Set3<Circle2d, nat> & p3, CArrRef<Set2<nat, bool> > object, 
                                   CCArrRef<Vector2d> & vert, CArrRef<Line2d> line )
{
    nat im = 0;
    double r[2];
    Vector2d o[2];
    Def<Circle2d> res;
    res.r = 0;
    Suite<Set3<Circle2d, nat> > stack;
    stack.inc() = p3;
    while ( stack.size() > 0 )
    {
        double min = 1e300;
        Def<Circle2d> cc;
        const Set3<Circle2d, nat> & s3 = stack.las();
        const Circle2d start = s3.a;
        const nat i1 = s3.b;
        const nat i2 = s3.c;
        stack.dec();
        const Set2<nat, bool> & o1 = object[i1];
        const Set2<nat, bool> & o2 = object[i2];
        if ( o1.b )
        {
            const Vector2d & p1 = vert[o1.a];
            if ( o2.b )
            {
                Def<Circle2d> c;
                const Vector2d & p2 = vert[o2.a];
                const Vector2d v = p2 - p1;
                for ( nat i = i1+1; i < i2; ++i )
                {
                    const Set2<nat, bool> & o3 = object[i];
                    if ( o3.b )
                    {
                        const Vector2d & p3 = vert[o3.a];
                        if ( ( p3 - p1 ) % v <= 0 ) continue;
                        c = circlePPP ( p1, p2, p3 );
                        if ( ! c.isDef ) continue;
                    }
                    else
                    {
                        const Line2d & l3 = line[o3.a];
                        if ( i == i1 + 1 )
                        {
                            const double s = - ( v * l3.norm );
                            if ( s <= 0 ) continue;
                            c.r = ( v * v ) / ( s + s );
                            c.o = p1 - c.r * l3.norm;
                        }
                        else
                        if ( i + 1 == i2 )
                        {
                            const double s = v * l3.norm;
                            if ( s <= 0 ) continue;
                            c.r = ( v * v ) / ( s + s );
                            c.o = p2 - c.r * l3.norm;
                        }
                        else
                        {
                            const nat n = circlePPL ( p1, p2, l3, o, r );
                            if ( n == 0 ) continue;
                            if ( n > 1 )
                            {
                                if ( ( o[1] - o[0] ) % v > 0 )
                                     o[0] = o[1], r[0] = r[1];
                            }
                            if ( isOut ( vert[o3.a], vert.cnext(o3.a), o[0] ) ) continue;
                            c.r = r[0];
                            c.o = o[0];
                        }
                        c.isDef = true;
                    }
                    const double t = ( c.o - start.o ) % v;
                    if ( t < -1e-12 * c.r )
                        continue;
                    if ( min > t ) min = t, cc = c, im = i;
                }
            }
            else
            {
                const Line2d & l2 = line[o2.a];
                if ( i1 + 2 == i2 )
                {
                    const Line2d & l3 = line[object[i1+1].a];
                    const double s = 1 - l2.norm * l3.norm;
                    if ( s < 8e-16 ) continue;
                    cc.r = - ( l2 % p1 ) / s;
                    cc.o = p1 - cc.r * l3.norm;
                    cc.isDef = true;
                    if ( res.r < cc.r ) res = cc;
                    continue;
                }
                const Vector2d & p3 = vert[o2.a];
                const Vector2d v ( p3 - p1 );
                const double s = v * l2.norm;
                if ( s <= 0 ) 
                    continue;
                cc.r = ( v * v ) / ( s + s );
                cc.o = p3 - cc.r * l2.norm;
                cc.isDef = true;
                const Line2d & l1 = line[o1.a];
                im = i2 - 1;
                nat k2 = object[im].b ? im : i2;
                nat k1 = i1;
                if ( l1.norm % ( cc.o - p1 ) > 0 ) 
                {
                    const double s = l1.norm * l2.norm - 1;
                    if ( s < 0 )
                    {
                        cc.r = ( l2 % p1 ) / s;
                        cc.o = p1 - cc.r * l1.norm;
                        im = ++k1;
                    }
                }
                min = ( cc.o - start.o ) % l2.norm;
                Circle2d c;
                const Vector2d v2 = l2.project ( start.o );
                for ( nat i = k1+1; i < k2; ++i )
                {
                    const Set2<nat, bool> & o3 = object[i];
                    if ( o3.b )
                    {
                        const Vector2d & p3 = vert[o3.a];
                        if ( qmod ( p3 - cc.o ) >= cc.r * cc.r )
                        {
                            if ( l2 % p3 > 0 ) continue;
                            const Vector2d v ( p3 - p1 );
                            if ( v % ( p1 - v2 ) > 0 || 
                                 v % ( p1 - l2.project ( cc.o ) ) < 0 ) continue;
                        }
                        const nat n = circlePPL ( p1, p3, l2, o, r );
                        if ( n == 0 ) continue;
                        if ( n > 1 )
                        {
                            const Vector2d u0 = l2.project ( o[0] );
                            const Vector2d u1 = l2.project ( o[1] );
                            if ( ( u1 - u0 ) % ( p1 - p3 ) > 0 ) o[0] = o[1], r[0] = r[1];
                        }
                        c.r = r[0];
                        c.o = o[0];
                    }
                    else
                    {
                        const Line2d & l3 = line[o3.a];
                        if ( l3 % cc.o + cc.r < 0 )
                        {
                            const Vector2d p3 ( 0.5 * ( vert[o3.a] + vert.cnext(o3.a) ) );
                            if ( l2 % p3 > 0 ) continue;
                            const Vector2d v ( p3 - p1 );
                            if ( v % ( p1 - v2 ) > 0 || 
                                 v % ( p1 - l2.project ( cc.o ) ) < 0 ) continue;
                        }
                        if ( i == i1 + 1 )
                        {
                            const double s = 1 - l2.norm * l3.norm;
                            if ( s < 8e-16 ) continue;
                            c.r = - ( l2 % p1 ) / s;
                            c.o = p1 - c.r * l3.norm;
                            if ( ( c.o - start.o ) % l2.norm < 0 ) continue;
                        }
                        else
                        {
                            const nat n = circlePLL ( p1, l2, l3, o, r );
                            if ( n == 0 ) continue;
                            if ( n > 1 && l2.norm % ( o[1] - o[0] ) < 0 ) o[0] = o[1], r[0] = r[1];
                            if ( isOut ( vert[o3.a], vert.cnext(o3.a), o[0] ) ) continue;
                            c.r = r[0];
                            c.o = o[0];
                            if ( isOut ( vert[o2.a], vert.cnext(o2.a), c.o ) ) continue;
                        }
                    }
                    const double t = ( c.o - start.o ) % l2.norm;
                    if ( t < -1e-12 * c.r )
                        continue;
                    if ( min > t ) min = t, cc = c, im = i;
                }
            }
        }
        else
        {
            const Line2d & l1 = line[o1.a];
            const Vector2d v1 = l1.project ( start.o );
            if ( o2.b )
            {
                const Vector2d & p2 = vert[o2.a];
                if ( i1 + 2 == i2 )
                {
                    const Line2d & l3 = line[object[i1+1].a];
                    const double s = 1 - l1.norm * l3.norm;
                    if ( s < 8e-16 ) continue;
                    cc.r = - ( l1 % p2 ) / s;
                    cc.o = p2 - cc.r * l3.norm;
                    cc.isDef = true;
                    if ( res.r < cc.r ) res = cc;
                    continue;
                }
                const Set2<nat, bool> & o3 = object[i1+1];
                const Vector2d & p3 = vert[o3.a];
                const Vector2d v ( p3 - p2 );
                const double s = v * l1.norm;
                if ( s <= 0 ) continue;
                cc.r = ( v * v ) / ( s + s );
                cc.o = p3 - cc.r * l1.norm;
                cc.isDef = true;
                const Line2d & l2 = line.cprev ( o2.a );
                im = i1 + 1;
                nat k1 = o3.b ? im : i1;
                nat k2 = i2;
                if ( l2.norm % ( cc.o - p2 ) < 0 ) 
                {
                    const double s = l1.norm * l2.norm - 1;
                    if ( s < 0 )
                    {
                        cc.r = ( l1 % p2 ) / s;
                        cc.o = p2 - cc.r * l2.norm;
                        im = --k2;
                    }
                }
                min = l1.norm % ( cc.o - start.o );
                Circle2d c;
                for ( nat i = k1+1; i < k2; ++i )
                {
                    const Set2<nat, bool> & o3 = object[i];
                    if ( o3.b )
                    {
                        const Vector2d & p3 = vert[o3.a];
                        if ( qmod ( p3 - cc.o ) >= cc.r * cc.r )
                        {
                            if ( l1 % p3 > 0 ) continue;
                            const Vector2d v ( p2 - p3 );
                            if ( v % ( p2 - v1 ) > 0 || 
                                 v % ( p2 - l1.project ( cc.o ) ) < 0 ) continue;
                        }
                        const nat n = circlePPL ( p2, p3, l1, o, r );
                        if ( n == 0 ) continue;
                        if ( n > 1 )
                        {
                            const Vector2d u0 = l1.project ( o[0] );
                            const Vector2d u1 = l1.project ( o[1] );
                            if ( ( u1 - u0 ) % ( p2 - p3 ) < 0 ) o[0] = o[1], r[0] = r[1];
                        }
                        c.r = r[0];
                        c.o = o[0];
                    }
                    else
                    {
                        const Line2d & l3 = line[o3.a];
                        if ( l3 % cc.o + cc.r < 0 )
                        {
                            const Vector2d p3 ( 0.5 * ( vert[o3.a] + vert.cnext(o3.a) ) );
                            if ( l1 % p3 > 0 ) continue;
                            const Vector2d v ( p2 - p3 );
                            if ( v % ( p2 - v1 ) > 0 || 
                                 v % ( p2 - l1.project ( cc.o ) ) < 0 ) continue;
                        }
                        if ( i + 1 == i2 )
                        {
                            const double s = 1 - l1.norm * l3.norm;
                            if ( s < 8e-16 ) continue;
                            c.r = - ( l1 % p2 ) / s;
                            c.o = p2 - c.r * l3.norm;
                            if ( l1.norm % ( c.o - start.o ) < 0 ) continue;
                        }
                        else
                        {
                            const nat n = circlePLL ( p2, l1, l3, o, r );
                            if ( n == 0 ) continue;
                            if ( n > 1 && l1.norm % ( o[1] - o[0] ) > 0 ) o[0] = o[1], r[0] = r[1];
                            if ( isOut ( vert[o3.a], vert.cnext(o3.a), o[0] ) ) continue;
                            c.r = r[0];
                            c.o = o[0];
                        }
                        if ( isOut ( vert[o1.a], vert.cnext(o1.a), c.o ) ) continue;
                    }
                    const double t = l1.norm % ( c.o - start.o );
                    if ( t < -1e-12 * c.r )
                        continue;
                    if ( min > t ) min = t, cc = c, im = i;
                }
            }
            else
            {
                const Line2d & l2 = line[o2.a];
                const Vector2d v2 = l2.project ( start.o );
                const Vector2d dir = l1.norm * l2.norm > 0 ?
                    l1.norm % l2.norm < 0 ? - ( l1.norm + l2.norm ) : l1.norm + l2.norm
                    :
                    ( l1.norm - l2.norm ).leftPerpendicular();
                for ( nat i = i1+1; i < i2; ++i )
                {
                    Def<Circle2d> c;
                    const Set2<nat, bool> & o3 = object[i];
                    if ( o3.b )
                    {
                        const Vector2d & p3 = vert[o3.a];
                        if ( i == i1 + 1 )
                        {
                            const double s = 1 - l1.norm * l2.norm;
                            if ( s < 8e-16 ) continue;
                            c.r = - ( l2 % p3 ) / s;
                            c.o = p3 - c.r * l1.norm;
                        }
                        else
                        if ( i + 1 == i2 )
                        {
                            const double s = 1 - l1.norm * l2.norm;
                            if ( s < 8e-16 ) continue;
                            c.r = - ( l1 % p3 ) / s;
                            c.o = p3 - c.r * l2.norm;
                        }
                        else
                        {
                            if ( ( p3 - v1 ) * dir <= 0 ) continue;
                            const nat n = circlePLL ( vert[o3.a], l1, l2, o, r );
                            if ( n == 0 ) continue;
                            if ( n > 1 )
                            {
                                const double t = dir * ( o[1] - start.o );
                                if ( t > -1e-12 * r[1] )
                                {
                                     if ( min > t ) min = t, cc.o = o[1], cc.r = r[1], cc.isDef = true, im = i;
                                }
                            }
                            c.r = r[0];
                            c.o = o[0];
                        }
                        c.isDef = true;
                    }
                    else
                    {
                        const Line2d & l3 = line[o3.a];
                        if ( l3 % v1 >= 0 || l3 % v2 >= 0 ) continue;
                        if ( l3.norm % dir > 0 )
                        {
                            if ( l3.norm % l1.norm >= 0 ) continue;
                        }
                        else
                        {
                            if ( l3.norm % l2.norm <= 0 ) continue;
                        }
                        if ( i1 + 1 == i )
                            c = circleVLLL ( vert[o3.a], l1, l3, l2 );
                        if ( ! c.isDef && i + 1 == i2 )
                            c = circleVLLL ( vert[o2.a], l2, l3, l1 );
                        if ( ! c.isDef )
                            c = circleLLL ( l1, l2, l3 );
                        if ( ! c.isDef || isOut ( vert[o3.a], vert.cnext(o3.a), c.o ) ) continue;
                    }
                    const double t = dir * ( c.o - start.o );
                    if ( t < -1e-12 * c.r )
                        continue;
                    if ( min > t ) min = t, cc = c, im = i;
                }
            }
        }
        if ( ! cc.isDef ) continue;
        if ( res.r < cc.r ) res = cc;
        if ( i1 + 1 < im )
        {
            stack.inc() = Set3<Circle2d, nat> ( cc, i1, im );
        }
        if ( im + 1 < i2 )
        {
            stack.inc() = Set3<Circle2d, nat> ( cc, im, i2 );
        }
    }
    return res;
}

} // namespace

Def<Circle2d> maxCircleInPolygon ( CCArrRef<Vector2d> & vert )
{
    Def<Circle2d> res;
    const nat nv = vert.size();
    if ( nv < 3 ) return res;
// Площадь многоугольника
    const double a = area ( vert );
    if ( a <= 0 ) return res;
// Определение габаритов многоугольника
    const Segment2d seg = dimensions ( vert );
// Проверка на малость площади по сравнению с габаритами многоугольника
    if ( a < 1e-9 * ( seg.b.x - seg.a.x ) * ( seg.b.y - seg.a.y ) ) return res;
    DynArray<Line2d> line ( nv );
// Для каждой стороны вычислим параметры уравнения прямой
// Уравнение прямой имеет вид: norm.x * x + norm.y * y + dist = 0
    nat i;
    for ( i = 0; i < nv; ++i )
    {
        const Vector2d & v2 = vert.cnext(i);
        const Vector2d & v1 = vert[i];
        Line2d & linei = line[i];
        Vector2d & norm = linei.norm;
        norm.x = v2.y - v1.y;
        norm.y = v1.x - v2.x;
        const double q = norm * norm;
        if ( q == 0 ) return res;
        norm /= sqrt ( q );
        const double d1 = norm * v1;
        const double d2 = norm * v2;
        linei.dist = d1 > d2 ? - d1 : - d2;
    }
// Заполнение массива объектов ( отрезков и невыпуклых вершин )
    Suite<Set2<nat, bool> > object ( nv+nv );
    nat im = 0;
    double max = 0;
    for ( i = 0; i < nv; ++i )
    {
        const double p = line.cprev(i).norm % line[i].norm;
        if ( p <= 0 )
        {
            Set2<nat, bool> & b = object.inc();
            b.a = i;
            b.b = true; // невыпуклая вершина 
        }
        if ( max < p ) max = p, im = object.size();
        Set2<nat, bool> & a = object.inc();
        a.a = i;
        a.b = false; // отрезок
    }
// Первая вершина должна быть выпуклой
    object <<= im;
// Поиск максимального вписанного круга
    res = maxCircleInPolygon ( Set3<Circle2d, nat> ( Circle2d ( vert[object[0].a], 0 ), 0, object.size()-1 ), object, vert, line );
// На всякий случай проверим решение
    if ( res.isDef )
    {
        if ( res.r <= 0 || ! isIntersect ( res.o, vert ) )
        {
            res.isDef = false;
        }
        else
        {
            const double r2 = ( 1 - 1e-9 ) * res.r * res.r;
            for ( i = 0; i < nv; ++i )
            {
                const Segment2d s ( vert[i], vert.cnext(i) );
                if ( s.qdis ( res.o ) < r2 )
                {
                    res.isDef = false;
                    break;
                }
            }
        }
    }
    return res;
}

//**************************** 13.12.2008 *********************************//
//
//   Максимальный фиксированный эллипс вписанный в выпуклый многоугольник
//
//**************************** 22.12.2010 *********************************//

class InnerObject
{
public:
    virtual Vector2d getPoint ( const Vector2d & dir ) const = 0;
    virtual nat symmetry () const = 0;
};

bool maxObjectInConvexPolygon ( InnerObject & inner, CArrRef<Vector2d> poly, Conform2d & res );

class InnerFixEllipse : public InnerObject
{
    double a, b;
public:
    InnerFixEllipse ( double pa, double pb ) : a(pa), b(pb) {}
    Vector2d getPoint ( const Vector2d & dir ) const
    {
        const double ax = a * dir.x;
        const double by = b * dir.y;
        const double ab = sqrt ( ax * ax + by * by );
        return Vector2d ( ax * a / ab, by * b / ab );
    }
    nat symmetry () const { return 2; }
};

Def<Ellipse2d> maxFixEllipseInConvexPolygon ( double ext, CCArrRef<Vector2d> & poly )
{
    Def<Ellipse2d> res;
    if ( ext < 1e-6 || ext > 0.99 ) return res;
    Conform2d conf;
    if ( ! maxObjectInConvexPolygon ( InnerFixEllipse ( ext, 1. ), poly, conf ) ) return res;
    res.a = ext * conf.magn;
    res.b = conf.magn;
    res.o = conf.trans;
    res.spin = conf.spin;
    res.isDef = true;
    return res;
}

//**************************** 17.09.2018 *********************************//
//
//            Максимальный фиксированный эллипс вписанный 
//               в выпуклый многоугольник без вращения
//                         1e-6 <= ext <= 1e6
//
//**************************** 17.09.2018 *********************************//

Def<Ellipse2d> maxFixEllipseInConvexPolygonNR ( double ext, CCArrRef<Vector2d> & poly )
{
    Def<Ellipse2d> res;
    if ( ext < 1e-6 || ext > 1e6 ) return res;
    DynArray<Vector2d> temp;
    temp = poly;
    for ( nat i = 0; i < poly.size(); ++i )
    {
        const Vector2d & v = poly[i];
        Vector2d & u = temp[i];
        u.x = v.x / ext;
        u.y = v.y;
    }
    Def<Circle2d> cir = maxCircleInConvexPolygon ( temp );
    if ( ! cir.isDef ) return res;
    res.a = cir.r * ext;
    res.b = cir.r;
    res.o.x = cir.o.x * ext;
    res.o.y = cir.o.y;
    res.isDef = true;
    return res;
}

//**************************** 02.06.2014 *********************************//
//
//  Максимальный фиксированный эллипс вписанный в невыпуклый многоугольник
//                        1/64 <= ext <= 64
//
//**************************** 02.06.2014 *********************************//

class MaxFixEllipseInPolygon : public MathFunc1
{
    mutable ArrRef<Vector2d> vert;
    CArrRef<Vector2d> poly;
    const double ext;
public:
    Def<Ellipse2d> & res;

    MaxFixEllipseInPolygon ( double e, CCArrRef<Vector2d> & p, ArrRef<Vector2d> v, Def<Ellipse2d> & r ) : 
    ext(e), poly(p), vert(v), res(r)
    {
        res.b = 0;
    }
    virtual double operator () ( double p ) const
    {
        const Spin2d spin ( p );
        LinTran2d lt ( ~spin );
        lt.x /= ext;
        vert = poly;
        vert *= lt;
        Def<Circle2d> cir = maxCircleInPolygon ( vert );
        if ( cir.isDef && res.b < cir.r )
        {
            res.b = cir.r;
            res.o = cir.o;
            res.spin = spin;
            res.isDef = true;
        }
        return cir.isDef ? cir.r : 0;
    }
};

Def<Ellipse2d> maxFixEllipseInPolygon ( double ext, CCArrRef<Vector2d> & poly, ArrRef<Vector2d> vert )
{
    Def<Ellipse2d> res;
    const double step = M_PI / 16;
    Circle2d c;
    c.r = 0;
    nat im = 16;
    for ( nat i = 0; i < 16; ++i )
    {
        const Spin2d spin ( i * step );
        LinTran2d lt ( ~spin );
        lt.x /= ext;
        vert = poly;
        vert *= lt;
        Def<Circle2d> cir = maxCircleInPolygon ( vert );
        if ( ! cir.isDef ) return res;
        if ( c.r < cir.r ) c = cir, im = i;
    }
    if ( im == 16 ) return res;
    MaxFixEllipseInPolygon func ( ext, poly, vert, res );
    fmax ( (im-1) * step, (im+1) * step, func, 0.001 );
    if ( ! res.isDef ) return res;
    res.o.x *= ext;
    res.a = ext * res.b;
    res.o = res.spin ( res.o );
    return res;
}

Def<Ellipse2d> maxFixEllipseInPolygon ( double ext, CCArrRef<Vector2d> & poly )
{
    Def<Ellipse2d> res;
    if ( ext == 1 )
    {
        const Def<Circle2d> cir = maxCircleInPolygon ( poly );
        if ( ! cir.isDef ) return res;
        res.isDef = true;
        res.a = res.b = cir.r;
        res.o = cir.o;
        return res;
    }
    if ( poly.size() < 3 || ext < 1/64. || ext > 64 ) return res;
    DynArray<Vector2d> vert ( poly.size() );
    return maxFixEllipseInPolygon ( ext, poly, vert );
}

//**************************** 19.09.2018 *********************************//
//
//            Максимальный фиксированный эллипс вписанный 
//              в невыпуклый многоугольник без вращения
//                         1/64 <= ext <= 64
//
//**************************** 19.09.2018 *********************************//

Def<Ellipse2d> maxFixEllipseInPolygonNR ( double ext, CCArrRef<Vector2d> & poly )
{
    Def<Ellipse2d> res;
    if ( ext < 1e-6 || ext > 1e6 ) return res;
    DynArray<Vector2d> temp;
    temp = poly;
    for ( nat i = 0; i < poly.size(); ++i )
    {
        const Vector2d & v = poly[i];
        Vector2d & u = temp[i];
        u.x = v.x / ext;
        u.y = v.y;
    }
    Def<Circle2d> cir = maxCircleInPolygon ( temp );
    if ( ! cir.isDef ) return res;
    res.a = cir.r * ext;
    res.b = cir.r;
    res.o.x = cir.o.x * ext;
    res.o.y = cir.o.y;
    res.isDef = true;
    return res;
}

//**************************** 29.09.2019 *********************************//
//
//      Максимальный свободный эллипс вписанный в треугольник
//                      A - максимум площади 
//
//**************************** 29.09.2019 *********************************//

Def<Ellipse2d> maxEllipseInTriangleA ( Vector2d v1, Vector2d v2, Vector2d v3 )
{
    Def<Ellipse2d> res;
// Вычисление параметров эллипса
    res.o = ( v1 + v2 + v3 ) / 3;
    v1 -= res.o;
    v2 -= res.o;
    const double D = v1 % v2;
    if ( D == 0 ) return res;
    const double a = ( v2.y - v1.y ) / D;
    const double b = ( v1.x - v2.x ) / D;
    const double c = ( v2.y + v1.y ) / D;
    const double d = ( v1.x + v2.x ) / D;
    const double A = a * a + 3 * c * c;
    const double B = a * b - 3 * c * d;
    const double C = b * b + 3 * d * d;
// Заполняем результат
    res.spin = Spin2d ( 0.5 * atan2 ( B + B, A - C ) );
    Vector2d ax, ay;
    res.spin.getReper ( ax, ay );
    res.a = ay.x * ( C * ay.x + B * ay.y ) + ay.y * ( B * ay.x + A * ay.y );
    res.b = ax.x * ( C * ax.x + B * ax.y ) + ax.y * ( B * ax.x + A * ax.y );
    if ( res.a <= 0 || res.b <= 0 ) return res;
    res.a = 1 / sqrt ( res.a );
    res.b = 1 / sqrt ( res.b );
    res.isDef = true;
    return res;
}

//**************************** 07.02.2009 *********************************//
//
//   Максимальный свободный эллипс вписанный в выпуклый многоугольник
//                      A - максимум площади 
//
//**************************** 16.04.2011 *********************************//

struct Edge
{
    Line2d line;
    nat prev, next, index;
};

inline void _swap ( SortItem<double, Edge *> & p1, SortItem<double, Edge *> & p2 )
{
    const SortItem<double, Edge *> p ( p1 );
    p1 = p2;
    p2 = p;
    _swap ( p1.tail->index, p2.tail->index );
}

namespace
{

class Ellipse
{
public:
    double a, b, c; // параметры
    Vector2d o; // центр

    double operator () ( const Line2d & line ) const
    {
        const Vector2d & n = line.norm;
        const double w = n.x * ( c * n.x - b * n.y ) + n.y * ( a * n.y - b * n.x );
        return _pow2 ( line % o ) / w;
    }
};

} // namespace

Def<Ellipse> maxEllipseIn3gon ( const Line2d & line1, const Line2d & line2, const Line2d & line3 )
{
    Def<Ellipse> res;
    const Vector2d & n1 = line1.norm;
    const Vector2d & n2 = line2.norm;
    const Vector2d & n3 = line3.norm;
// Вычисление вершин треугольника
    Vector2d v1, v2, v3;
    SLU2<double> slu;
    slu.aa = n1.x;   slu.ab = n1.y;   slu.ac = -line1.dist;
    slu.ba = n2.x;   slu.bb = n2.y;   slu.bc = -line2.dist;
    if ( ! slu.gauss ( v1.x, v1.y ) ) return res;
    slu.aa = n3.x;   slu.ab = n3.y;   slu.ac = -line3.dist;
    slu.ba = n2.x;   slu.bb = n2.y;   slu.bc = -line2.dist;
    if ( ! slu.gauss ( v2.x, v2.y ) ) return res;
    slu.aa = n3.x;   slu.ab = n3.y;   slu.ac = -line3.dist;
    slu.ba = n1.x;   slu.bb = n1.y;   slu.bc = -line1.dist;
    if ( ! slu.gauss ( v3.x, v3.y ) ) return res;
// Вычисление параметров эллипса
    res.o = ( v1 + v2 + v3 ) / 3;
    v1 -= res.o;
    v2 -= res.o;
    const double D = v1 % v2;
    if ( D == 0 ) return res;
    const double a = ( v2.y - v1.y ) / D;
    const double b = ( v1.x - v2.x ) / D;
    const double c = ( v2.y + v1.y ) / D;
    const double d = ( v1.x + v2.x ) / D;
    res.a = a * a + 3 * c * c;
    res.b = a * b - 3 * c * d;
    res.c = b * b + 3 * d * d;
    res.isDef = true;
    return res;
}

Def<Ellipse> maxEllipseIn4gon ( const Line2d & line1, const Line2d & line2,
                                const Line2d & line3, const Line2d & line4, ArrRef2<double> & data )
{
    Def<Ellipse> res;
    const Vector2d & n1 = line1.norm;
    const Vector2d & n2 = line2.norm;
    const Vector2d & n3 = line3.norm;
    const Vector2d & n4 = line4.norm;
// Заполнение матрицы 4*6
    data[0][0] = n1.x * n1.x; data[0][1] = n1.x * n1.y; data[0][2] = n1.y * n1.y; 
    data[0][3] = n1.x * line1.dist; data[0][4] = n1.y * line1.dist; data[0][5] = line1.dist * line1.dist;
    data[1][0] = n2.x * n2.x; data[1][1] = n2.x * n2.y; data[1][2] = n2.y * n2.y; 
    data[1][3] = n2.x * line2.dist; data[1][4] = n2.y * line2.dist; data[1][5] = line2.dist * line2.dist;
    data[2][0] = n3.x * n3.x; data[2][1] = n3.x * n3.y; data[2][2] = n3.y * n3.y; 
    data[2][3] = n3.x * line3.dist; data[2][4] = n3.y * line3.dist; data[2][5] = line3.dist * line3.dist;
    data[3][0] = n4.x * n4.x; data[3][1] = n4.x * n4.y; data[3][2] = n4.y * n4.y; 
    data[3][3] = n4.x * line4.dist; data[3][4] = n4.y * line4.dist; data[3][5] = line4.dist * line4.dist;
// Метод исключения Гаусса с полным выбором ведущего элемента
    FixArray<nat, 6> col;
    if ( ! slu_gauss ( data, 5, col ) ) return res;
    nat k;
    double ca[5], cb[5];
    for ( k = 0; k < 4; ++k )
    {
        ca[col[k]] = data[k][col[4]];
        cb[col[k]] = data[k][col[5]];
    }
    ca[col[4]] = -1;
    cb[col[4]] = 0;
    const double a0 = cb[2];
    const double a1 = -ca[2];
    const double b0 = cb[1] * -0.5;
    const double b1 = ca[1] * 0.5;
    const double c0 = cb[0];
    const double c1 = -ca[0];
    const double x0 = cb[3] * -0.5;
    const double x1 = ca[3] * 0.5;
    const double y0 = cb[4] * -0.5;
    const double y1 = ca[4] * 0.5;
    const double x0x0 = x0 * x0;
    const double x0x1 = x0 * x1;
    const double x0y0 = x0 * y0;
    const double x0y1 = x0 * y1;
    const double x1x1 = x1 * x1;
    const double x1y0 = x1 * y0;
    const double x1y1 = x1 * y1;
    const double y0y0 = y0 * y0;
    const double y0y1 = y0 * y1;
    const double y1y1 = y1 * y1;
    double a = a1 * x1x1 + 2 * b1 * x1y1 + c1 * y1y1;
    double b = a0 * x1x1 + 2 * ( a1 * x0x1 + b0 * x1y1 + b1 * x0y1 + b1 * x1y0 + c1 * y0y1 ) + c0 * y1y1
        + a1 * c1 - b1 * b1;
    double c = a1 * x0x0 + 2 * ( a0 * x0x1 + b1 * x0y0 + b0 * x0y1 + b0 * x1y0 + c0 * y0y1 ) + c1 * y0y0
        + a1 * c0 + a0 * c1 - 2 * b0 * b1;
    double d = a0 * x0x0 + 2 * b0 * x0y0 + c0 * y0y0
        + a0 * c0 - b0 * b0;
    a *= 3;
    double D = b * b - a * c;
    if ( D < 0 ) return res;
    double t;
    if ( a * a <= D * 1e-22 )
    {
        if ( b == 0 ) return res;
        t = -0.5 * c / b;
    }
    else
    {
        t = ( -b - sqrt(D) ) / a;
    }
    a = a0 + a1 * t;
    b = b0 + b1 * t;
    c = c0 + c1 * t;
    const double x = x0 + x1 * t;
    const double y = y0 + y1 * t;
    c += x * x;
    b -= x * y;
    a += y * y;
    d = a * c - b * b;
    res.a = a / d;
    res.b = b / d;
    res.c = c / d;
    res.o.x = x;
    res.o.y = y;
    res.isDef = true;
    return res;
}

Def<Ellipse> maxEllipseIn5gon ( const Line2d & line1, const Line2d & line2, const Line2d & line3, 
                                const Line2d & line4, const Line2d & line5, ArrRef2<double> & data )
{
    Def<Ellipse> res;
    const Vector2d & n1 = line1.norm;
    const Vector2d & n2 = line2.norm;
    const Vector2d & n3 = line3.norm;
    const Vector2d & n4 = line4.norm;
    const Vector2d & n5 = line5.norm;
// Заполнение матрицы 5*6
    data[0][0] = n1.x * n1.x; data[0][1] = n1.x * n1.y; data[0][2] = n1.y * n1.y; 
    data[0][3] = n1.x * line1.dist; data[0][4] = n1.y * line1.dist; data[0][5] = line1.dist * line1.dist;
    data[1][0] = n2.x * n2.x; data[1][1] = n2.x * n2.y; data[1][2] = n2.y * n2.y; 
    data[1][3] = n2.x * line2.dist; data[1][4] = n2.y * line2.dist; data[1][5] = line2.dist * line2.dist;
    data[2][0] = n3.x * n3.x; data[2][1] = n3.x * n3.y; data[2][2] = n3.y * n3.y; 
    data[2][3] = n3.x * line3.dist; data[2][4] = n3.y * line3.dist; data[2][5] = line3.dist * line3.dist;
    data[3][0] = n4.x * n4.x; data[3][1] = n4.x * n4.y; data[3][2] = n4.y * n4.y; 
    data[3][3] = n4.x * line4.dist; data[3][4] = n4.y * line4.dist; data[3][5] = line4.dist * line4.dist;
    data[4][0] = n5.x * n5.x; data[4][1] = n5.x * n5.y; data[4][2] = n5.y * n5.y; 
    data[4][3] = n5.x * line5.dist; data[4][4] = n5.y * line5.dist; data[4][5] = line5.dist * line5.dist;
// Метод исключения Гаусса с частичным выбором ведущего элемента
    FixArray<double, 5> p;
    if ( ! slu_gauss ( data, p ) ) return res;
    double a = p[2];
    double b = p[1] * -0.5;
    double c = p[0];
    const double x = p[3] * -0.5;
    const double y = p[4] * -0.5;
    c += x * x;
    b -= x * y;
    a += y * y;
    const double d = a * c - b * b;
    if ( d <= 0 ) return res;
    res.a = a / d;
    res.b = b / d;
    res.c = c / d;
    res.o.x = x;
    res.o.y = y;
    res.isDef = true;
    return res;
}
static
bool check ( CCArrRef<Edge> & edge, CCArrRef<nat> & support )
{
    nat j = support.size() - 1;
    for ( nat i = 0; i < support.size(); ++i )
    {
        const Vector2d & n1 = edge[support[j]].line.norm;
        const Vector2d & n2 = edge[support[i]].line.norm;
        if ( n1 % n2 < ( n1 * n2 < 0 ? 1e-6 : 0 ) ) return false;
        j = i;
    }
    return true;
}
inline
double value ( const Edge & e, CCArrRef<Edge> & edge )
{
    const double c = edge[e.prev].line.norm * edge[e.next].line.norm;
    const double s = edge[e.prev].line.norm % edge[e.next].line.norm;
    return c > 0 ? ( s < 0 ? 4 + s : s ) : 2 - s;
}
static
Def<Ellipse> maxEllipseInConvexPolygon ( ArrRef<Edge> & edge )
{
    Def<Ellipse> res;
    const nat n = edge.size();
// Находим простейший замкнутый  трёх- или чётырёхугольник
    MinHeap< SortItem<double, Edge*> > heap ( n );
    nat i;
    for ( i = 0; i < n; ++i )
    {
        Edge & e = edge[i];
        const double a = value ( e, edge );
        heap << SortItem<double, Edge*> ( a, & e );
    }
    while ( heap[0]->head < 2 - 1e-6 )
    {
        if ( heap.size() <= 3 ) return res;
        SortItem<double, Edge*> t;
        heap >> t;
        Edge & prev = edge[t.tail->prev];
        Edge & next = edge[t.tail->next];
        prev.next = t.tail->next;
        next.prev = t.tail->prev;
        heap[prev.index]->head = value ( prev, edge );
        heap.down ( prev.index );
        heap[next.index]->head = value ( next, edge );
        heap.down ( next.index );
    }
    Def<Ellipse> e;
    FixArray<double, 30> data;
    ArrRef2<double> data5 ( data, 0, 5, 6 );
    ArrRef2<double> data4 ( data, 0, 4, 6 );
    CmbArray<nat, 5> support1, support2, support3;
    nat i1, i2, i3;
    i1 = heap[0]->tail->next;
    i2 = edge[i1].next;
    i3 = edge[i2].next;
    if ( heap.size() == 3 )
    {
        e = maxEllipseIn3gon ( edge[i1].line, edge[i2].line, edge[i3].line );
        if ( ! e.isDef ) return res;
        support1.resize ( 3 );
    }
    else
    {
        nat i4 = edge[i3].next;
        e = maxEllipseIn4gon ( edge[i1].line, edge[i2].line, edge[i3].line, edge[i4].line, data4 );
        if ( ! e.isDef ) return res;
        support1.resize ( 4 );
        support1[3] = i4;
    }
    support1[0] = i1;
    support1[1] = i2;
    support1[2] = i3;
    insertSort123 ( support1 );
    double q = e.a * e.c - e.b * e.b;
    if ( q <= 0 ) return res;
    for (;;)
    {
// Ищем наиболее близкую прямую
        nat im;
        double min = 1e300;
        for ( i = 0; i < n; ++i )
        {
            const double t = e ( edge[i].line );
            if ( min > t ) min = t, im = i;
        }
        if ( min * q > 1 - 1e-14 || hasEqu ( support1, im ) ) break;
// Находим максимальный эллипс ограниченный текущими опорными прямыми и прямой line[im]
        const Line2d & line0 = edge[im].line;
        Def<Ellipse> ee;
        double qq = 0;
        nat i1, i2, i3, i4;
        for ( i1 = 1; i1 < support1.size(); ++i1 )
        {
            const Line2d & line1 = edge[support1[i1]].line;
            if ( ee.isDef && ee ( line1 ) * qq > 1 - 1e-14 ) continue;
            ee.isDef = false;
            for ( i2 = 0; i2 < i1; ++i2 )
            {
                const Line2d & line2 = edge[support1[i2]].line;
                if ( ee.isDef && ee ( line2 ) * qq > 1 - 1e-14 ) continue;
                support3.resize(3);
                support3[0] = support1[i2];
                support3[1] = support1[i1];
                support3[2] = im;
                insertSort123 ( support3 );
                if ( check ( edge, support3 ) )
                {
                    ee = maxEllipseIn3gon ( line0, line1, line2 );
                    if ( ! ee.isDef ) return res;
                    qq = ee.a * ee.c - ee.b * ee.b;
                    if ( qq <= 0 ) return res;
                    support2 = support3;
                }
                else
                {
                    ee.isDef = false;
                }
                for ( i3 = 0; i3 < i2; ++i3 )
                {
                    const Line2d & line3 = edge[support1[i3]].line;
                    if ( ee.isDef && ee ( line3 ) * qq > 1 - 1e-14 ) continue;
                    support3.resize(4);
                    support3[0] = support1[i3];
                    support3[1] = support1[i2];
                    support3[2] = support1[i1];
                    support3[3] = im;
                    insertSort123 ( support3 );
                    if ( check ( edge, support3 ) )
                    {
                        ee = maxEllipseIn4gon ( line0, line1, line2, line3, data4 );
                        if ( ! ee.isDef ) return res;
                        qq = ee.a * ee.c - ee.b * ee.b;
                        if ( qq <= 0 ) return res;
                        support2 = support3;
                    }
                    else
                    {
                        ee.isDef = false;
                    }
                    for ( i4 = 0; i4 < i3; ++i4 )
                    {
                        const Line2d & line4 = edge[support1[i4]].line;
                        if ( ee.isDef && ee ( line4 ) * qq > 1 - 1e-14 ) continue;
                        support3.resize(5);
                        support3[0] = support1[i4];
                        support3[1] = support1[i3];
                        support3[2] = support1[i2];
                        support3[3] = support1[i1];
                        support3[4] = im;
                        insertSort123 ( support3 );
                        if ( check ( edge, support3 ) )
                        {
                            ee = maxEllipseIn5gon ( line0, line1, line2, line3, line4, data5 );
                            if ( ! ee.isDef ) return res;
                            qq = ee.a * ee.c - ee.b * ee.b;
                            if ( qq <= 0 ) return res;
                            support2 = support3;
                        }
                        else
                        {
                            ee.isDef = false;
                        }
                    }
                }
            }
        }
        if ( ! ee.isDef ) return res;
// Если площадь эллипса не уменьшилась, то выходим из цикла
        if ( q >= qq ) break;
        q = qq;
        e = ee;
        support1 = support2;
    }
    return e;
}

Def<Ellipse2d> maxEllipseInConvexPolygonA ( CCArrRef<Vector2d> & vert )
{
    Def<Ellipse2d> res;
    const nat n = vert.size();
    if ( n < 3 ) return res;
    if ( n == 3 ) return maxEllipseInTriangleA ( vert[0], vert[1], vert[2] );
    const nat n1 = n - 1;
// Для каждой стороны вычислим параметры уравнения прямой
// Уравнение прямой имеет вид: norm.x * x + norm.y * y + dist = 0
    DynArray<Edge> edge ( n );
    for ( nat i = 0; i < n; ++i )
    {
        Edge & e = edge[i];
        const Vector2d & v2 = vert[i<n1?i+1:0];
        const Vector2d & v1 = vert[i];
        e.line.norm.x = v2.y - v1.y;
        e.line.norm.y = v1.x - v2.x;
        const double q = e.line.norm * e.line.norm;
        if ( q == 0 ) return res;
        e.line.norm /= sqrt ( q );
        e.line.dist = - ( e.line.norm * v1 );
        e.index = i;
        e.prev = i > 0 ? i-1 : n1;
        e.next = i < n1 ? i+1 : 0;
    }
// Находим эллипс с максимальной площадью
    const Def<Ellipse> e = maxEllipseInConvexPolygon ( edge );
    if ( ! e.isDef ) return res;
// Заполняем результат
    res.spin = Spin2d ( 0.5 * atan2 ( e.b + e.b, e.a - e.c ) );
    Vector2d ax, ay;
    res.spin.getReper ( ax, ay );
    res.a = ay.x * ( e.c * ay.x + e.b * ay.y ) + ay.y * ( e.b * ay.x + e.a * ay.y );
    res.b = ax.x * ( e.c * ax.x + e.b * ax.y ) + ax.y * ( e.b * ax.x + e.a * ax.y );
    if ( res.a <= 0 || res.b <= 0 ) return res;
    res.a = 1 / sqrt ( res.a );
    res.b = 1 / sqrt ( res.b );
    res.o = e.o;
    res.isDef = true;
    return res;
}

//**************************** 26.09.2018 *********************************//
//
//              Максимальный свободный эллипс вписанный 
//               в выпуклый многоугольник без вращения
//                      A - максимум площади 
//
//**************************** 26.09.2018 *********************************//

Def<Ellipse2d> maxEllipseInConvexPolygonANR ( CCArrRef<Vector2d> & outer )
{
    Def<Ellipse2d> res;
    Affin2d aff;
// Получение границ многоугольника в виде прямых линий
    Conform2d conf;
    DynArray<Line2d> line ( outer.size() );
    if ( ! points2lines ( outer, conf, line ) ) return res;
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
            return res;
// Поиск максимального нарушения ограничений для выбранного решения
        const double a = best.d0;
        const double b = best.d1;
        const Vector2d v ( best.d2, best.d3 );
        nat k, km = 0;
        max = 0.;
        for ( k = 0; k < line.size(); ++k )
        {
            const Line2d & lk = line[k];
            const Vector2d dir ( a * lk.norm.x, b * lk.norm.y );
            const double t = sqrt ( dir * dir ) + lk % v;
            if ( max < t ) max = t, km = k;
        }
// Если нарушение мало, то завершаем программу
        if ( max < eps )
        {
            aff = Affin2d ( ~conf ) * Affin2d ( Vector2d ( a, 0 ), Vector2d ( 0, b ), v );
            res.a = aff.t.x.x;
            res.b = aff.t.y.y;
            res.o = aff.s;
            res.isDef = true;
            return res;
        }
// Применение ограничения к области допустимых преобразований
        const double x = line[km].norm.x;
        const double y = line[km].norm.y;
        const Vector2d dir ( a * x, b * y );
        const double s = sqrt ( dir * dir );
        Double<5> plane;
        plane.d0 = x * x * a / s;
        plane.d1 = y * y * b / s;
        plane.d2 = x;
        plane.d3 = y;
        plane.d4 = line[km].dist;
        model.cut ( plane, stor );
    }
    return res;
}

//**************************** 04.06.2014 *********************************//
//
//   Максимальный свободный эллипс вписанный в невыпуклый многоугольник
//                      A - максимум площади 
//
//**************************** 04.06.2014 *********************************//

class MaxEllipseInPolygon : public MathFunc1
{
    mutable ArrRef<Vector2d> vert;
    CArrRef<Vector2d> poly;
public:
    Def<Ellipse2d> & res;

    MaxEllipseInPolygon ( CCArrRef<Vector2d> & p, ArrRef<Vector2d> v, Def<Ellipse2d> & r ) : 
    poly(p), vert(v), res(r)
    {
        res.b = 0;
    }
    virtual double operator () ( double p ) const
    {
        const Def<Ellipse2d> e = maxFixEllipseInPolygon ( p, poly, vert );
        if ( ! e.isDef ) return 0;
        const double area = e.a * e.b;
        if ( res.a * res.b < area ) res = e;
        return area;
    }
};

Def<Ellipse2d> maxEllipseInPolygonA ( CCArrRef<Vector2d> & poly )
{
    Def<Ellipse2d> res;
    if ( poly.size() < 3 ) return res;
    if ( poly.size() == 3 ) return maxEllipseInTriangleA ( poly[0], poly[1], poly[2] );
    DynArray<Vector2d> vert ( poly.size() );
    const Def<Circle2d> cir = maxCircleInPolygon ( poly );
    if ( ! cir.isDef ) return res;
    res.a = res.b = cir.r;
    res.o = cir.o;
    res.isDef = true;
    nat im = 9;
    for ( nat i = 0; i < 9; ++i )
    {
        const double ext = ( 1 + i * 7 ) / 64.;
        Def<Ellipse2d> e = maxFixEllipseInPolygon ( ext, poly, vert );
        if ( ! e.isDef ) return e;
        if ( res.a * res.b < e.a * e.b ) res = e, im = i;
    }
    MaxEllipseInPolygon func ( poly, vert, res );
    const double a = im == 0 ? 1. / 64 : ( im * 7 - 6 ) / 64.;
    const double b = im == 9 ? 1.      : ( im * 7 + 8 ) / 64.;
    fmax ( a, b, func, 0.001 );
    return res;
}

//**************************** 22.09.2018 *********************************//
//
//              Максимальный свободный эллипс вписанный 
//              в невыпуклый многоугольник без вращения
//                      A - максимум площади 
//
//**************************** 22.09.2018 *********************************//

class MaxEllipseInPolygonNR : public MathFunc1
{
    CArrRef<Vector2d> poly;
public:
    Def<Ellipse2d> & res;

    MaxEllipseInPolygonNR ( CCArrRef<Vector2d> & p, Def<Ellipse2d> & r ) : poly(p), res(r)
    {
        res.b = 0;
    }
    virtual double operator () ( double p ) const
    {
        const Def<Ellipse2d> e = maxFixEllipseInPolygonNR ( p, poly );
        if ( ! e.isDef ) return 0;
        const double area = e.a * e.b;
        if ( res.a * res.b < area ) res = e;
        return area;
    }
};

Def<Ellipse2d> maxEllipseInPolygonANR ( CCArrRef<Vector2d> & poly )
{
    Def<Ellipse2d> res;
    if ( poly.size() < 3 ) return res;
    MaxEllipseInPolygonNR func ( poly, res );
    fmax ( 1./64, 64, func, 0.001 );
    return res;
}
