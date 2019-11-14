
#include "math.h"
#include "lists.h"
#include "func1t.h"
#include "func2d.h"
#include "opti2d.h"
#include "ShevArray.h"
#include "intersect2d.h"

//**************************** 08.03.2011 *********************************//
//
//               Пересечение точки и многоугольника
//
//**************************** 28.03.2011 *********************************//

bool isIntersect ( const Vector2d & p, CArrRef<Vector2d> poly )
{
    const nat n = poly.size();
    if ( n <= 1 ) return n > 0 ? p == poly[0] : false;
    bool res = false;
    const Vector2d & v1 = poly.las();
    double x1 = p.x - v1.x;
    double y1 = p.y - v1.y;
    for ( nat i = 0; i < n; ++i )
    {
        const Vector2d & v2 = poly[i];
        const double x2 = p.x - v2.x;
        const double y2 = p.y - v2.y;
        if ( y1 == 0 && ( x1 == 0 || ( y2 == 0 && x1 > 0 != x2 > 0 ) ) ) return true;
        if ( y1 > 0 != y2 > 0 )
        {
            const double d = x1 - y1 * ( x1 - x2 ) / ( y1 - y2 );
            if ( d == 0 ) return true;
            if ( d > 0 ) res = ! res;
        }
        x1 = x2;
        y1 = y2;
    }
    return res;
}

//**************************** 08.09.2006 *********************************//
//
//                      Пересечение двух прямых
//
//**************************** 23.05.2010 *********************************//

Def<Vector2d> intersection ( const Line2d & line1, const Line2d & line2 )
{
    Def<Vector2d> point;
    const double d = line1.norm % line2.norm;
    if ( d == 0 ) return point;
    point.x = ( line1.norm.y * line2.dist - line2.norm.y * line1.dist ) / d; 
    point.y = ( line2.norm.x * line1.dist - line1.norm.x * line2.dist ) / d;
    point.isDef = true;
    return point;
}

//**************************** 13.10.2010 *********************************//
//
//                      Пересечение прямой и отрезка
//
//**************************** 13.10.2010 *********************************//

Def<Vector2d> intersection ( const Line2d & line, const Segment2d & seg )
{
    Def<Vector2d> p;
    const double a = line % seg.a;
    const double b = line % seg.b;
    if ( a * b > 0 || a == b ) return p;
    if ( fabs ( line.norm.x ) > fabs ( line.norm.y ) )
    {
        p.y = ( seg.a.y * b - seg.b.y * a ) / ( b - a );
        p.x = - ( line.norm.y * p.y + line.dist ) / line.norm.x;
    }
    else
    {
        p.x = ( seg.a.x * b - seg.b.x * a ) / ( b - a );
        p.y = - ( line.norm.x * p.x + line.dist ) / line.norm.y;
    }
    p.isDef = true;
    return p;
}

//**************************** 16.10.2010 *********************************//
//
//                      Пересечение двух отрезков
//
//**************************** 16.10.2010 *********************************//

Def<Vector2d> intersection ( const Segment2d & s1, const Segment2d & s2 )
{
    Def<Vector2d> p;
    const Vector2d v1 = s1.b - s1.a;
    const Vector2d v2 = s2.b - s2.a;
    const double d = v1 % v2;
    if ( d == 0 ) return p;
    const Vector2d vv = s1.a - s2.a;
    const double t1 = ( v2 % vv ) / d;
    if ( t1 < 0 || t1 > 1 ) return p;
    const double t2 = ( v1 % vv ) / d;
    if ( t2 < 0 || t2 > 1 ) return p;
    p = t1 < t2 ? s1.a + v1 * t1 : s2.a + v2 * t2;
    return p;
}

//**************************** 18.05.2008 *********************************//
//
//                  Отсечение прямой части отрезка
//
//**************************** 18.10.2010 *********************************//

Def<Segment2d> cut ( const Line2d & line, const Segment2d & seg )
{
    Def<Segment2d> res ( seg );
    const double a = line % res.a;
    const double b = line % res.b;
    if ( a < 0 )
    {
        if ( b > 0 )
        {
            const double c = b - a;
            if ( fabs ( line.norm.x ) > fabs ( line.norm.y ) )
            {
                res.b.y = ( res.a.y * b - res.b.y * a ) / c;
                res.b.x = - ( line.norm.y * res.b.y + line.dist ) / line.norm.x;
            }
            else
            {
                res.b.x = ( res.a.x * b - res.b.x * a ) / c;
                res.b.y = - ( line.norm.x * res.b.x + line.dist ) / line.norm.y;
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
        if ( fabs ( line.norm.x ) > fabs ( line.norm.y ) )
        {
            res.a.y = ( res.a.y * b - res.b.y * a ) / c;
            res.a.x = - ( line.norm.y * res.a.y + line.dist ) / line.norm.x;
        }
        else
        {
            res.a.x = ( res.a.x * b - res.b.x * a ) / c;
            res.a.y = - ( line.norm.x * res.a.x + line.dist ) / line.norm.y;
        }
    }
    return res;
}

//**************************** 27.11.2010 *********************************//
//
//                  Пересечение прямой и круга
//
//**************************** 11.12.2010 *********************************//

Def<Segment2d> intersection ( const Line2d & line, const Circle2d & cir )
{
    Def<Segment2d> res;
    const double u = norm2 ( line.norm );
    if ( u == 0 ) return res;
    const double d = ( line % cir.o ) / u;
    if ( fabs ( d ) > cir.r ) return res;
    const Vector2d norm = line.norm / u;
    const Vector2d v = cir.o - d * norm;
    const Vector2d p = norm.leftPerpendicular();
    const double s = sqrt ( cir.r * cir.r - d * d );
    res.a = v - s * p;
    res.b = v + s * p;
    res.isDef = true;
    return res;
}

//**************************** 27.11.2010 *********************************//
//
//                  Пересечение отрезка и круга
//
//**************************** 11.12.2010 *********************************//

Def<Segment2d> intersection ( const Segment2d & seg, const Circle2d & cir )
{
    Def<Segment2d> res;
    const Vector2d u = cir.o - seg.a;
    const Vector2d v = seg.b - seg.a;
    const double a = v * v;
    const double b = v * u;
    const double c = u * u - cir.r * cir.r;
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

//**************************** 11.12.2010 *********************************//
//
//                  Пересечение прямой и эллипса
//
//**************************** 11.12.2010 *********************************//

Def<Segment2d> intersection ( const Line2d & line, const Ellipse2d & e )
{
    Def<Segment2d> res;
    const Affin2d af = e.getAffin2d();
    Line2d tmp;
    tmp.norm.x = line.norm.x * af.t.x.x + line.norm.y * af.t.y.x;
    tmp.norm.y = line.norm.x * af.t.x.y + line.norm.y * af.t.y.y;
    tmp.dist = line % af.s;
    res = intersection ( tmp, Circle2d ( null2d, 1 ) );
    if ( ! res.isDef ) return res;
    res.a = af ( res.a );
    res.b = af ( res.b );
    return res;
}

//**************************** 11.12.2010 *********************************//
//
//                  Пересечение отрезка и эллипса
//
//**************************** 11.12.2010 *********************************//

Def<Segment2d> intersection ( const Segment2d & seg, const Ellipse2d & e )
{
    Def<Segment2d> res;
    const Affin2d af1 = e.getAffin2d();
    const Affin2d af2 = ~af1;
    res = intersection ( Segment2d ( af2 ( seg.a ), af2 ( seg.b ) ), Circle2d ( null2d, 1 ) );
    if ( ! res.isDef ) return res;
    res.a = af1 ( res.a );
    res.b = af1 ( res.b );
    return res;
}

//**************************** 08.01.2011 *********************************//
//
//               Пересечение прямой и многоугольника
//
//**************************** 08.01.2011 *********************************//

DynArrRef<Segment2d> & intersection ( const Line2d & line, CArrRef<Vector2d> poly, DynArrRef<Segment2d> & res )
{
    const nat n = poly.size();
    if ( n < 2 || ! line.norm ) return res.resize();
    const nat n2 = n / 2;
    DynArray<SortItem<double, Vector2d> > arr ( n );
    LtdSuiteRef<SortItem<double, Vector2d> > v1 ( arr,  0, n2 ), v2 ( arr, n2, n2 );
    DynArray<double> d ( n );
    nat i;
    for ( i = 0; i < n; ++i )
    {
        d[i] = line % poly[i];
    }
    for ( i = 0; i < n; ++i )
    {
        const Vector2d & va = poly.cprev(i);
        const Vector2d & vb = poly[i];
        const double a = d.cprev(i);
        const double b = d[i];
        const double c = a * b;
        if ( c > 0 ) continue;
        if ( c < 0 )
        {
            SortItem<double, Vector2d> & si = a < 0 ? v1.inc() : v2.inc();
            si.tail = va + ( vb - va ) * ( a / ( a - b ) );
            si.head = line.norm % si.tail;
        }
        else
        {
            if ( a == 0 )
            {
                if ( b >= 0 || d.cprev(i>0?i-1:n-1) < 0 ) continue;
                SortItem<double, Vector2d> & si = v2.inc();
                si.tail = va;
                si.head = line.norm % si.tail;
            }
            else
            {
                if ( a >= 0 || d.cnext(i) < 0 ) continue;
                SortItem<double, Vector2d> & si = v1.inc();
                si.tail = vb;
                si.head = line.norm % si.tail;
            }
        }
    }
    if ( v1.size() > 1 )
    {
        quickSort123 ( v1 );
        quickSort123 ( v2 );
    }
    res.resize ( v1.size() );
    for ( i = 0; i < v1.size(); ++i )
    {
        res[i].a = v1[i].tail;
        res[i].b = v2[i].tail;
    }
    return res;
}

//**************************** 08.01.2011 *********************************//
//
//               Пересечение отрезка и многоугольника
//
//**************************** 08.01.2011 *********************************//

SuiteRef<Segment2d> & intersection ( const Segment2d & seg, CArrRef<Vector2d> poly, SuiteRef<Segment2d> & res )
{
    res.resize();
    const Line2d line ( seg.a, seg.b );
    const double a = line.norm % seg.a;
    const double b = line.norm % seg.b;
    DynArray<Segment2d> sec;
    intersection ( poly, line, sec );
    for ( nat i = 0; i < sec.size(); ++i )
    {
        double ai = line.norm % sec[i].a;
        double bi = line.norm % sec[i].b;
        if ( ai > bi )
        {
            _swap ( ai, bi );
            _swap ( sec[i].a, sec[i].b );
        }
        if ( bi <= a || ai >= b ) continue;
        Segment2d & s = res.inc();
        s.a = ai > a ? sec[i].a : seg.a;
        s.b = bi < b ? sec[i].b : seg.b;
    }
    return res;
}

//**************************** 28.01.2011 *********************************//
//
//               Отсечение части многоугольника прямой
//
//**************************** 28.01.2011 *********************************//

SuiteRef< Suite<Vector2d> > & cut ( CArrRef<Vector2d> poly, const Line2d & line, 
                                    DynArrRef<Set2<nat, Vector2d> > & arr, DynArrRef<double> & d,
                                    SuiteRef< Suite<Vector2d> > & res )
{
    const nat n = poly.size();
    if ( n == 0 || ! line.norm ) return res;
    const double ax = fabs ( line.norm.x );
    const double ay = fabs ( line.norm.y );
    const bool xgy = ax > ay;
    double kx, ky, kd;
    if ( xgy )
    {
        kx = line.norm.x > 0 ? 1 : -1;
        ky = line.norm.y / ax;
        kd = line.dist / ax;
    }
    else
    {
        kx = line.norm.x / ay;
        ky = line.norm.y > 0 ? 1 : -1;
        kd = line.dist / ay;
    }
    arr.resize ( n );
    d.resize ( n );
    nat i;
    if ( xgy )
    {
        for ( i = 0; i < n; ++i )
        {
            double t = ky * poly[i].y + kd;
            d[i] = kx * poly[i].x + t;
        }
    }
    else
    {
        for ( i = 0; i < n; ++i )
        {
            double t = kx * poly[i].x + kd;
            d[i] = ky * poly[i].y + t;
        }
    }
    const nat n2 = n / 2;
    LtdSuiteRef<Set2<nat, Vector2d> > v1 ( arr, 0, n2 ), v2 ( arr, n2, n2 );
    for ( i = 0; i < n; ++i )
    {
        const Vector2d & va = poly.cprev(i);
        const Vector2d & vb = poly[i];
        const double a = d.cprev(i);
        const double b = d[i];
        const double c = a * b;
        if ( c > 0 ) continue;
        if ( c < 0 )
        {
            Set2<nat, Vector2d> & si = a < 0 ? v1.inc() : v2.inc();
            si.a = i;
            if ( xgy )
            {
                si.b.y = va.y + ( vb.y - va.y ) * ( a / ( a - b ) );
                double t = ky * si.b.y + kd;
                si.b.x = - kx * t;
            }
            else
            {
                si.b.x = va.x + ( vb.x - va.x ) * ( a / ( a - b ) );
                double t = kx * si.b.x + kd;
                si.b.y = - ky * t;
            }
        }
        else
        {
            if ( a == 0 )
            {
                if ( b >= 0 || d.cprev(i>0?i-1:n-1) < 0 ) continue;
                Set2<nat, Vector2d> & si = v2.inc();
                si.a = i;
                si.b = va;
            }
            else
            {
                if ( a > 0 || d.cnext(i) < 0 ) continue;
                Set2<nat, Vector2d> & si = v1.inc();
                si.a = i;
                si.b = vb;
            }
        }
    }
    const nat m = v1.size();
// Нет пересечения с прямой
    if ( m == 0 )
    {
        for ( i = 0; i < n; ++i )
        {
            if ( d[i] < 0 )
            {
                res.inc() = poly;
                break;
            }
        }
        return res;
    }
// Пересечение с прямой - это один отрезок
    if ( m == 1 )
    {
        Suite<Vector2d> & s = res.inc();
        s.resize();
        s.inc() = v2[0].b;
        for ( i = v2[0].a;; )
        {
            if ( poly[i] != s.las() ) s.inc() = poly[i];
            if ( ++i == n ) i = 0;
            if ( i == v1[0].a ) break;
        }
        const Vector2d & v = v1[0].b;
        if ( s[0] != v && s.las() != v ) s.inc() = v;
        if ( s.size() < 3 ) res.dec();
        return res;
    }
// Пересечение с прямой - это несколько отрезков
    if ( v1[0].a < v2[0].a ) v1 <<= 1;
    DynArray<SortItem<double, nat> > arr2 ( m+m );
    ArrRef<SortItem<double, nat> > s1 ( arr2, 0, m ), s2 ( arr2, m, m );
    for ( i = 0; i < m; ++i )
    {
        s1[i].head = line.norm % v1[i].b;
        s1[i].tail = i;
        s2[i].head = line.norm % v2[i].b;
        s2[i].tail = i;
    }
    insertSort123 ( s1 );
    insertSort123 ( s2 );
    for ( nat j = 0; j < m; ++j )
    {
        if ( s2[j].tail == m ) continue;
        Suite<Vector2d> & s = res.inc();
        s.resize();
        for ( nat k = j;; )
        {
            const nat c = s2[k].tail;
            s2[k].tail = m;
            const nat i1 = v1[c].a;
            s.inc() = v2[c].b;
            for ( i = v2[c].a;; )
            {
                if ( poly[i] != s.las() ) s.inc() = poly[i];
                if ( ++i == n ) i = 0;
                if ( i == i1 ) break;
            }
            const Vector2d & v = v1[c].b;
            if ( s.las() != v && s[0] != v ) s.inc() = v;
            for ( k = j; k < m; ++k )
            {
                if ( s1[k].tail == c ) break;
            }
            if ( k == j ) break;
        }
        if ( s.size() < 3 ) res.dec();
    }
    return res;
}

SuiteRef< Suite<Vector2d> > & cut ( CArrRef<Vector2d> poly, const Line2d & line, SuiteRef< Suite<Vector2d> > & res )
{
    DynArray<Set2<nat, Vector2d> > arr;
    DynArray<double> d;
    res.resize();
    return cut ( poly, line, arr, d, res );
}

//**************************** 28.01.2011 *********************************//
//
//            Пересечение выпуклого многоугольника с простым
//
//**************************** 28.01.2011 *********************************//

Suite< Suite<Vector2d> > & 
intersect1c ( CArrRef<Vector2d> conv, CArrRef<Vector2d> poly, 
              Suite< Suite<Vector2d> > & tmp, Suite< Suite<Vector2d> > & res )
{
    res.resize();
    const nat n = conv.size();
    if ( n < 3 ) return res;
    Suite<Set2<nat, Vector2d> > arr;
    Suite<double> d;
    res.inc() = poly;
    bool none = true;
    for ( nat i = 0; i < conv.size(); ++i )
    {
        const Vector2d & a = conv.cprev(i);
        const Vector2d & b = conv[i];
        if ( a == b ) continue;
        none = false;
        const Line2d line ( a, b );
        for ( nat j = 0; j < res.size(); ++j ) cut ( res[j], line, arr, d, tmp );
        _swap ( res, tmp );
        tmp.resize();
    }
    if ( none ) res.dec();
    return res;
}

Suite< Suite<Vector2d> > & 
intersect1c ( CArrRef<Vector2d> conv, CArrRef<Vector2d> poly, Suite< Suite<Vector2d> > & res )
{
    Suite< Suite<Vector2d> > tmp;
    return intersect1c ( conv, poly, tmp, res );
}

//**************************** 23.05.2012 *********************************//
//
//             Пересечение двух простых многоугольников
//
//**************************** 28.05.2014 *********************************//

inline void put ( Suite<Vector2d> & p, const Vector2d & v )
{
    if ( p.size() == 0 || p.las() != v ) p.inc() = v;
}

bool intersection ( CArrRef<Vector2d> poly1, CArrRef<Vector2d> poly2, Suite< Suite<Vector2d> > & res )
{
    res.resize();
    if ( poly1.size() < 3 || poly2.size() < 3 ) return true;
    Suite<Vector2d> p1 ( poly1.size() ), p2 ( poly2.size() );
    nat i, j;
    for ( i = 0; i < poly1.size(); ++i )
    {
        if ( poly1[i] != poly1.cprev(i) ) p1.inc() = poly1[i];
    }
    for ( i = 0; i < poly2.size(); ++i )
    {
        if ( poly2[i] != poly2.cprev(i) ) p2.inc() = poly2[i];
    }
    if ( p1.size() < 3 || p2.size() < 3 ) return true;
// Вычисляем площади многоугольников
    double a1 = area ( poly1 );
    double a2 = area ( poly2 );
    if ( a1 == 0 || a2 == 0 ) return true;
// Если один из многоугольников выпуклый, то применям специальный алгоритм пересечения
    if ( a1 > 0 && a2 > 0 )
    {
        if ( isConvex ( poly1 ) )
        {
            intersect1c ( poly1, poly2, res );
            return true;
        }
        if ( isConvex ( poly2 ) )
        {
            intersect1c ( poly2, poly1, res );
            return true;
        }
    }
// Делаем первым многоугольник с большей площадью по модулю
    if ( fabs ( a1 ) < fabs ( a2 ) )
    {
        _swap ( a1, a2 );
        _swap ( p1, p2 );
    }
// Находим точки пересечения границ многоугольников
    const nat n = p2.size();
    Suite<Set4<Vector2d, nat, nat, bool> > vert;
    SortItem<double, Set4<Vector2d, nat, nat, bool> > v;
    Suite<SortItem<double, Set4<Vector2d, nat, nat, bool> > > arr;
    for ( i = 0; i < p1.size(); ++i )
    {
        const Vector2d & va1 = p1[i];
        const Vector2d & vb1 = p1.cnext(i);
        const Vector2d dir1 ( vb1 - va1 );
        const double sa1 = dir1 * va1;
        const double sb1 = dir1 * vb1;
        arr.resize();
        for ( j = 0; j < p2.size(); ++j )
        {
            const Vector2d & va2 = p2[j];
            const Vector2d & vb2 = p2.cnext(j);
            const Vector2d dir2 ( vb2 - va2 );
            const Vector2d a2a1 ( va2 - va1 );
            const Vector2d b2a1 ( vb2 - va1 );
            const Vector2d a2b1 ( va2 - vb1 );
            const Vector2d b2b1 ( vb2 - vb1 );
            const double a1 = ( a2a1 * a2a1 < a2b1 * a2b1 ? a2a1 : a2b1 ) % dir1;
            const double b1 = ( b2a1 * b2a1 < b2b1 * b2b1 ? b2a1 : b2b1 ) % dir1;
            const double c1 = a1 * b1;
            if ( c1 > 0 ) continue;
            const double a2 = dir2 % ( a2a1 * a2a1 < b2a1 * b2a1 ? a2a1 : b2a1 );
            const double b2 = dir2 % ( a2b1 * a2b1 < b2b1 * b2b1 ? a2b1 : b2b1 );
            const double c2 = a2 * b2;
            if ( c2 > 0  ) continue;
            if ( c1 < 0 )
            {
                if ( c2 < 0 )
                {
                    v.head = a2 / ( a2 - b2 );
                    v.tail.a = va1 + dir1 * v.head;
                }
                else
                {
                    if ( a2 ) continue;
                    const Vector2d dir0 ( va1 - p1.cprev(i) );
                    if ( dir0 % dir1 < 0 )
                    {
                        v.tail.d = b1 > 0 && b2a1 % dir0 > 0;
                        if ( v.tail.d == ( a1 > 0 && a2a1 % dir0 > 0 ) ) continue;
                    }
                    else
                    {
                        v.tail.d = b1 > 0 || b2a1 % dir0 > 0;
                        if ( v.tail.d == ( a1 > 0 || a2a1 % dir0 > 0 ) ) continue;
                    }
                    v.head = 0;
                    v.tail.a = va1;
                }
                v.tail.d = b1 > 0;
            }
            else
            {
                if ( a1 ) continue;
                if ( ! b1 ) // рёбра лежат на одной прямой
                {
                    if ( a2a1 * dir1 < 0 && ( vb2 - va1 ) * dir1 > 0 ) continue;
                }
                const Vector2d ue1 ( p2.cprev(j) - va1 );
                if ( c2 < 0 )
                {
                    if ( ( b1 <= 0 ) == ( ue1 % dir1 <= 0 ) ) continue;
                    v.tail.d = b1 > 0;
                    v.head = a2 / ( a2 - b2 );
                }
                else
                {
                    if ( a2 ) continue;
                    const Vector2d & vc1 = p1.cprev(i);
                    const Vector2d dir0 ( va1 - vc1 );
                    const Vector2d uc1 ( p2.cprev(j) - vc1 );
                    if ( dir0 % dir1 < 0 )
                    {
                        v.tail.d = b1 > 0 && b2a1 % dir0 > 0;
                        if ( v.tail.d == ( ue1 % dir1 > 0 && uc1 % dir0 > 0 ) ) continue;
                    }
                    else
                    {
                        v.tail.d = b1 > 0 || b2a1 % dir0 > 0;
                        if ( v.tail.d == ( ue1 % dir1 > 0 || uc1 % dir0 > 0 ) ) continue;
                    }
                    v.head = 0;
                }
                v.tail.a = va2;
            }
            v.tail.b = i;
            v.tail.c = j;
            arr.inc() = v;
        }
        insertSort123 ( arr );
        for ( j = 0; j < arr.size(); ++j )
        {
            if ( vert.size() > 0 && vert.las().a == arr[j].tail.a )
                vert.dec();
            else
                vert.inc() = arr[j].tail;
        }
    }
    if ( vert.size() > 0 )
    {
        Suite<Vector2d> poly;
        nat pre = vert.size() - 1;
        for ( nat cur = 0; cur < vert.size(); ++cur )
        {
            if ( vert[pre].d == vert[cur].d )
            {
                return false;
            }
            pre = cur;
        }
    }
    if ( vert.size() > 0 && vert.las().a == vert[0].a )
    {
        for ( j = 2; j < vert.size(); ++j ) vert[j-2] = vert[j-1];
        vert.dec ( 2 );
    }
// Случай, когда границы многоугольников не пересекаются
    if ( vert.size() == 0 )
    {
        Def<Vector2d> o;
        if ( a2 > 0 )
        {
            Def<Circle2d> cir = maxCircleInPolygon ( p2 );
            if ( cir.isDef ) o = cir.o;
        }
        else
        {
            DynArray<Vector2d> temp;
            temp = p2;
            Def<Circle2d> cir = maxCircleInPolygon ( temp.reverse() );
            if ( cir.isDef ) o = cir.o;
        }
        if ( ! o.isDef ) 
        {
            o = 0.5 * ( p2[0] + p2[1] );
        }
        if ( a1 > 0 == isIntersect ( o, p1 ) ) res.inc() = p2;
        if ( a2 < 0 ) res.inc() = p1;
        return true;
    }
// Случай, когда границы многоугольников пересекаются
    if ( vert.size() == 2 )
    {
        const Set4<Vector2d, nat, nat, bool> & oa = vert[0].d ? vert[0] : vert[1];
        const Set4<Vector2d, nat, nat, bool> & ob = vert[0].d ? vert[1] : vert[0];
        Suite<Vector2d> & p3 = res.inc();
        p3.resize();
        if ( oa.b == ob.b )
        {
            if ( ( ob.a - oa.a ) * ( p1.cnext(oa.b) - p1[oa.b] ) < 0 )
            {
                for ( i = 1; i <= p1.size(); ++i )
                {
                    j = oa.b + i;
                    if ( j >= p1.size() ) j -= p1.size();
                    put ( p3, p1[j] );
                }
            }
        }
        else
        {
            for ( i = oa.b;; )
            {
                if ( i == ob.b ) break;
                if ( ++i == p1.size() ) i = 0;
                put ( p3, p1[i] );
            }
        }
        put ( p3, ob.a );
        if ( oa.c == ob.c )
        {
            if ( ( ob.a - oa.a ) * ( p2.cnext(oa.c) - p2[oa.c] ) > 0 )
            {
                for ( i = 1; i <= p2.size(); ++i )
                {
                    j = oa.c + i;
                    if ( j >= p2.size() ) j -= p2.size();
                    put ( p3, p2[j] );
                }
            }
        }
        else
        {
            for ( i = ob.c;; )
            {
                if ( i == oa.c ) break;
                if ( ++i == p2.size() ) i = 0;
                put ( p3, p2[i] );
            }
        }
        if ( p3[0] != oa.a && p3.las() != oa.a ) p3.inc() = oa.a;
        if ( p3.size() < 3 ) res.dec();
    }
    else
    {
        List2n list;
        DynArray<SortItem<Set2<nat, double>, nat> > temp ( vert.size() );
        j = vert[0].d ? 0 : 1;
        for ( i = 0; i < vert.size(); i += 2 )
        {
            Item2n * item = new Item2n;
            item->a = i + j;
            item->b = item->a + 1;
            if ( item->b == vert.size() ) item->b = 0;
            list.addAftLas ( item );
        }
        for ( i = 0; i < vert.size(); ++i )
        {
            j = vert[i].c;
            const double t = ( p2.cnext(j) - p2[j] ).setNorm2() * vert[i].a;
            temp[i].head.a = j;
            temp[i].head.b = t;
            temp[i].tail = i;
        }
        insertSort123 ( temp );
        if ( vert[temp[0].tail].d ) temp >>= 1;
        for ( i = 0; i < temp.size(); ++i )
        {
            Item2n * item = new Item2n;
            item->a = temp[i].tail;
            ++i;
            item->b = temp[i].tail;
            list.addAftLas ( item );
        }
        List<ListItem<List2n> > itog;
        makeClosed ( list, itog );
        res.resize ( itog.size() ).resize();
        if ( itog.top() )
        do
        {
            Suite<Vector2d> & p3 = res.inc();
            p3.resize();
            List2n * zol = itog.cur();
            nat ia = zol->top()->a;
            do
            {
                const nat ib = zol->cur()->b;
                const Set4<Vector2d, nat, nat, bool> & oa = vert[ia];
                const Set4<Vector2d, nat, nat, bool> & ob = vert[ib];
                if ( vert[ia].d )
                {
                    if ( oa.b == ob.b )
                    {
                        if ( ( ob.a - oa.a ) * ( p1.cnext(oa.b) - p1[oa.b] ) < 0 )
                        {
                            for ( i = 1; i <= p1.size(); ++i )
                            {
                                j = oa.b + i;
                                if ( j >= p1.size() ) j -= p1.size();
                                put ( p3, p1[j] );
                            }
                        }
                    }
                    else
                    {
                        for ( i = oa.b;; )
                        {
                            if ( i == ob.b ) break;
                            if ( ++i == p1.size() ) i = 0;
                            put ( p3, p1[i] );
                        }
                    }
                    put ( p3, ob.a );
                    ia = ib;
                }
                else
                {
                    if ( oa.c == ob.c )
                    {
                        if ( ( ob.a - oa.a ) * ( p2.cnext(oa.c) - p2[oa.c] ) < 0 )
                        {
                            for ( i = 1; i <= p2.size(); ++i )
                            {
                                j = oa.c + i;
                                if ( j >= p2.size() ) j -= p2.size();
                                put ( p3, p2[j] );
                            }
                        }
                    }
                    else
                    {
                        for ( i = oa.c;; )
                        {
                            if ( i == ob.c ) break;
                            if ( ++i == p2.size() ) i = 0;
                            put ( p3, p2[i] );
                        }
                    }
                    put ( p3, ob.a );
                    ia = ib;
                }
            }
            while ( zol->next() );
            if ( p3.las() == p3[0] ) p3.dec();
            if ( p3.size() < 3 ) res.dec();
        }
        while ( itog.next() );
    }
    return true;
}
