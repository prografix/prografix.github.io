
#include "math.h"

#include "Rand.h"
#include "func1t.h"
#include "atrian.h"
#include "trian2t.h"
#include "trian2d.h"
#include "Vector2d.h"
#include "ShevList.h"
#include "ShevArray.h"
#include "lists.h"

//**************************** 18.07.2007 *********************************//
//
//          Триангуляция многоугольников ( локальная оптимизация )
//
//**************************** 27.08.2011 *********************************//

SuiteRef<Set3<nat> > & trianNat1L1MaxMinArea ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_Area<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

SuiteRef<Set3<nat> > & trianNat1L1MaxMinAdivR2 ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

SuiteRef<Set3<nat> > & trianNat1L1MaxSumAdivR2 ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const PosSum2a<double> merge;
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

SuiteRef<Set3<nat> > & trianNat1L1MaxMinTan ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_MinTan<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

SuiteRef<Set3<nat> > & trianNat1L1MaxSumMinAngle ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const PosSum2a<double> merge;
    const TQ_MinAngle<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

//**************************** 18.07.2007 *********************************//
//
//          Триангуляция многоугольников ( глобальная оптимизация )
//
//**************************** 27.08.2011 *********************************//

SuiteRef<Set3<nat> > & trianG1MaxMinArea ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_Area<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

SuiteRef<Set3<nat> > & trianG1MaxMinAdivR2 ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

SuiteRef<Set3<nat> > & trianG1MaxSumAdivR2 ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const PosSum2a<double> merge;
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

SuiteRef<Set3<nat> > & trianG1MaxMinTan ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_MinTan<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

SuiteRef<Set3<nat> > & trianG1MaxSumMinAngle ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const PosSum2a<double> merge;
    const TQ_MinAngle<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}


//**************************** 25.11.2015 *********************************//
//
//            Триангуляция многоугольников ( заметающая прямая )
//
//**************************** 19.08.2018 *********************************//

class ChainNode
{
public:
    nat index, prev, next;
    Item3n * segm;
};

class Polygon
{
public:
    nat size, vert;
};

void triangulate_monotone_polygons ( CCArrRef<Polygon> & monpolys, CCArrRef<ChainNode> & node, CCArrRef<Vector2d> & vertex, SuiteRef<Set3<nat> > & res )
{
    Suite<nat> buf;
    for ( nat i = 0; i < monpolys.size(); ++i )
    {
        const Polygon & poly = monpolys[i];
        if ( poly.size == 3 )
	    {
            const ChainNode & v = node[poly.vert];
            Set3<nat> & s = res.inc();
	        s.a = v.index;
	        s.b = node[v.next].index;
	        s.c = node[v.prev].index;
	    }
        else
	    {
            bool isPrev = true;
            buf.resize();
            buf.inc() = poly.vert;
            nat prev = node[poly.vert].prev;
            nat next = node[poly.vert].next;
            if ( vertex[node[prev].index].x > vertex[node[next].index].x )
            {
                buf.inc() = next;
                next = node[next].next;
                isPrev = false;
            }
            else
            {
                buf.inc() = prev;
                prev = node[prev].prev;
            }
            nat v;
            for ( nat i = 3; i < poly.size; ++i )
            {
                if ( isPrev )
                {
                    if ( vertex[node[next].index].x <= vertex[node[prev].index].x )
                    {
                        v = next;
                        next = node[next].next;
                        const nat a = buf.las();
                        while ( buf.size() > 1 )
                        {
                            const nat i0 = buf.las(0);
                            const nat i1 = buf.las(1);
                            Set3<nat> & s = res.inc();
	                        s.a = node[i0].index;
	                        s.b = node[i1].index;
	                        s.c = node[v].index;
                            buf.dec();
                        }
                        buf[0] = a;
                        isPrev = false;
                    }
                    else
                    {
                        v = prev;
                        prev = node[prev].prev;
                        const Vector2d & v2 = vertex[node[v].index];
                        while ( buf.size() > 1 )
                        {
                            const nat i0 = buf.las(0);
                            const nat i1 = buf.las(1);
                            const Vector2d & v0 = vertex[node[i0].index];
                            const Vector2d & v1 = vertex[node[i1].index];
                            const double t = ( v1 - v0 ) % ( v2 - v1 );
                            if ( t < 0 ) break;
                            Set3<nat> & s = res.inc();
	                        s.a = node[i0].index;
	                        s.b = node[i1].index;
	                        s.c = node[v].index;
                            buf.dec();
                        }
                    }
                }
                else
                {
                    if ( vertex[node[prev].index].x <= vertex[node[next].index].x )
                    {
                        v = prev;
                        prev = node[prev].prev;
                        const nat a = buf.las();
                        while ( buf.size() > 1 )
                        {
                            const nat i0 = buf.las(0);
                            const nat i1 = buf.las(1);
                            Set3<nat> & s = res.inc();
	                        s.a = node[i1].index;
	                        s.b = node[i0].index;
	                        s.c = node[v].index;
                            buf.dec();
                        }
                        buf[0] = a;
                        isPrev = true;
                    }
                    else
                    {
                        v = next;
                        next = node[next].next;
                        const Vector2d & v2 = vertex[node[v].index];
                        const bool order = node[v].next == buf.las();
                        while ( buf.size() > 1 )
                        {
                            const nat i0 = buf.las(0);
                            const nat i1 = buf.las(1);
                            const Vector2d & v0 = vertex[node[i0].index];
                            const Vector2d & v1 = vertex[node[i1].index];
                            const double t = ( v1 - v0 ) % ( v0 - v2 );
                            if ( t < 0 ) break;
                            Set3<nat> & s = res.inc();
	                        s.a = node[i1].index;
	                        s.b = node[i0].index;
	                        s.c = node[v].index;
                            buf.dec();
                        }
                    }
                }
                buf.inc() = v;
            }
            if ( isPrev )
            {
                while ( buf.size() > 1 )
                {
                    const nat i0 = buf.las(0);
                    const nat i1 = buf.las(1);
                    Set3<nat> & s = res.inc();
	                s.a = node[i0].index;
	                s.b = node[i1].index;
	                s.c = node[next].index;
                    buf.dec();
                }
            }
            else
            {
                while ( buf.size() > 1 )
                {
                    const nat i0 = buf.las(0);
                    const nat i1 = buf.las(1);
                    Set3<nat> & s = res.inc();
	                s.a = node[i1].index;
	                s.b = node[i0].index;
	                s.c = node[prev].index;
                    buf.dec();
                }
            }
        }
    }
}

enum TypeEven { NONE_BEFORE, PREV_BEFORE, NEXT_BEFORE, BOTH_BEFORE };

inline TypeEven typeEven ( CCArrRef<ChainNode> & node, nat v )
{
    const ChainNode & b = node[v];
    if ( node[b.prev].segm )
        return b.segm ? BOTH_BEFORE : PREV_BEFORE;
    else
        return b.segm ? NEXT_BEFORE : NONE_BEFORE;
}

void link ( Suite<ChainNode> & node, nat a, nat b )
{
    const nat a1 = node.size();
    const nat b1 = a1 + 1;
    node.inc(2);
    ChainNode & na = node[a];
    ChainNode & nb = node[b];
    ChainNode & na1 = node[a1];
    ChainNode & nb1 = node[b1];
    na1.segm = nb1.segm = 0;
    na1.index = na.index;
    nb1.index = nb.index;
    na1.prev = na.prev;
    nb1.prev = nb.prev;
    na1.next = b;
    nb1.next = a;
    node[na1.prev].next = nb.prev = a1;
    node[nb1.prev].next = na.prev = b1;
}

bool isConvex ( CCArrRef<ChainNode> & node, nat v, CCArrRef<Vector2d> & vertex )
{
    const ChainNode & n = node[v];
    const Vector2d & b = vertex[n.index];
    const Vector2d & a = vertex[node[n.prev].index];
    const Vector2d & c = vertex[node[n.next].index];
    if ( a.y >= b.y && b.y >= c.y ) return true;
    if ( a.y <= b.y && b.y <= c.y ) return false;
    return ( b - a ) % ( c - b ) >= 0;
}

bool isInto ( CCArrRef<ChainNode> & node, nat v, CCArrRef<Vector2d> & vertex, const Item3n * p )
{
    const Vector2d & c = vertex[node[v].index];
    const ChainNode & a = node[p->a];
    const Vector2d & a1 = vertex[a.index];
    const Vector2d & b1 = vertex[node[a.next].index];
    if ( a1.x == b1.x )
    {
        if ( c.y < b1.y ) return false;
    }
    else
    {
        if ( ( b1 - a1 ) % ( c - b1 ) < 0 ) return false;
    }
    const ChainNode & b = node[p->b];
    const Vector2d & a2 = vertex[b.index];
    const Vector2d & b2 = vertex[node[b.next].index];
    if ( a2.x == b2.x )
    {
        if ( c.y > a2.y ) return false;
    }
    else
    {
        if ( ( b2 - a2 ) % ( c - b2 ) < 0 ) return false;
    }
    return true;
}

void noneBefore ( Suite<ChainNode> & node, nat v, List3n & sweepLine, List3n & stor3n, CCArrRef<Vector2d> & vertex )
{
    if ( ! isConvex ( node, v, vertex ) )
    {
        if ( sweepLine.top() )
        do
        {
            Item3n * t = sweepLine.cur();
            if ( ! isInto ( node, v, vertex, t ) ) continue;
            link ( node, t->c, v );
            Item3n * p = sweepLine.addNewAftLas ( stor3n );
            p->c = node.size() - 1;
            p->b = node[p->c].prev;
            p->a = t->a;
            node[p->a].segm = node[p->b].segm = p;
            t->a = t->c = v;
            node[t->a].segm = t;
            return;
        }
        while ( sweepLine.next() );
    }
    Item3n * p = sweepLine.addNewAftLas ( stor3n );
    p->a = p->c = v;
    p->b = node[v].prev;
    node[p->a].segm = node[p->b].segm = p;
}

void prevBefore ( Suite<ChainNode> & node, nat v )
{
    ChainNode & b = node[v];
    ChainNode & a = node[b.prev];
    const nat pre = a.segm->c;
    ( b.segm = a.segm )->a = v;
    b.segm->c = v;
    a.segm = 0;
    if ( pre != b.prev ) link ( node, pre, v );
}

void nextBefore ( Suite<ChainNode> & node, nat v )
{
    ChainNode & b = node[v];
    ChainNode & a = node[b.prev];
    const nat pre = b.segm->c;
    ( a.segm = b.segm )->b = b.prev;
    a.segm->c = v;
    b.segm = 0;
    if ( pre != b.next )
    {
        a.segm->c = node.size() + 1;
        link ( node, pre, v );
    }
}

void bothBefore ( Suite<ChainNode> & node, nat v, List3n & sweepLine, List3n & stor3n )
{
    ChainNode & b = node[v];
    ChainNode & a = node[b.prev];
    if ( a.segm == b.segm )
    {
        if  ( ! a.segm ) return;
        const nat pre = a.segm->c;
        sweepLine.jump ( a.segm ).movCurAftLas ( stor3n );
        a.segm = b.segm = 0;
        if ( pre != b.prev && pre != b.next ) link ( node, pre, v );
    }
    else
    {
        const nat pa = a.segm->c;
        const nat pb = b.segm->c;
        b.segm->b = a.segm->b;
        Item3n * segm = node[b.segm->b].segm = b.segm;
        b.segm->c = v;
        sweepLine.jump ( a.segm ).movCurAftLas ( stor3n );
        a.segm = b.segm = 0;
        const bool linka = pa != node[v].prev;
        const bool linkb = pb != node[v].next;
        if ( linkb )
        {
            segm->c = node.size() + 1;
            link ( node, pb, v );
        }
        if ( linka ) link ( node, pa, segm->c );
    }
}

SuiteRef<Set3<nat> > & trianSweepLine ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vertex, SuiteRef<Set3<nat> > & res )
{
    nat i, nv = 0;
    res.resize();
    if ( vertex.size() < 3 ) return res;
    for ( i = 0; i < cntr.size(); ++i )
    {
        if ( cntr[i] < 3 ) return res;
        nv += cntr[i];
    }
    if ( nv < 3 || nv > vertex.size() ) return res;
// Подготовка данных
    Suite<ChainNode> node ( nv );
    for ( i = 0; i < cntr.size(); ++i )
    {
        const nat n = cntr[i];
        const nat m = node.size();
        node.inc ( n );
        for ( nat j = 0; j < n; ++j )
        {
            const nat k = m + j;
            ChainNode & vi = node[k];
            vi.next = k + 1;
            vi.prev = k - 1;
            vi.index = k;
            vi.segm = 0;
        }
        node[m].prev = node.size() - 1;
        node.las().next = m;
    }
// Построение монотонных многоугольников
    {
        DynArray<SortItem<double, nat> > schedule ( node.size() );
        for ( i = 0; i < node.size(); ++i )
        {
            SortItem<double, nat> & si = schedule[i];
            si.head = vertex[node[i].index].x;
            si.tail = i;
        }
        quickSort123 ( schedule );
        List3n sweepLine, stor3n;
        for ( i = 0; i < schedule.size(); ++i )
        {
            const nat v = schedule[i].tail;
            const TypeEven e = typeEven ( node, v );
            switch ( e )
            {
            case NONE_BEFORE: noneBefore ( node, v, sweepLine, stor3n, vertex ); break;
            case PREV_BEFORE: prevBefore ( node, v ); break;
            case NEXT_BEFORE: nextBefore ( node, v ); break;
            case BOTH_BEFORE: bothBefore ( node, v, sweepLine, stor3n ); break;
            }
        }
    }
// Выделение монотонных многоугольников
    Item3n dummy;
    Suite<Polygon> temp;
    for ( i = 0; i < node.size(); ++i )
    {
        ChainNode & ni = node[i];
        if ( ni.segm ) continue;
        ni.segm = & dummy;
        Polygon & poly = temp.inc();
        poly.vert = i;
        poly.size = 1;
        nat j = node[i].next;
        while ( j != i )
        {
            ChainNode & nj = node[j];
            nj.segm = & dummy;
            if ( vertex[node[poly.vert].index].x > vertex[nj.index].x ) poly.vert = j;
            j = nj.next;
            ++poly.size;
        }
    }
// Триангуляция монотонных многоугольников
    triangulate_monotone_polygons ( temp, node, vertex, res );
    return res;
}

SuiteRef<Set3<nat> > & trianSweepLine ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    FixArray<nat, 1> cntr;
    cntr[0] = vert.size();
    return trianSweepLine ( cntr, vert, res );
}

//**************************** 24.08.2018 *********************************//
//
//      Перестройка триангуляции на триангуляцию Делоне
//
//**************************** 24.08.2018 *********************************//

bool rebuildDelauney ( CCArrRef<Vector2d> & vert, CCArrRef<Set3<nat> > & trian, DynArray<SemiRib> & rib )
{
    const nat nt = trian.size();
    const nat nv = vert.size();
    if ( nt < 1 || nv < 3 ) return false;
    const nat nr = 3 * nt;
    rib.resize ( nr );
    // Запишем массив полурёбер SemiRib.
    // Причём рёбра принадлежащие к одному треугольнику должны находится последовательно, 
    // а поле twin должно быть меньше количества рёбер только у одного ребра из пары.
    nat i, k;
    DynArray<SortItem<Set2<nat>, nat> > sar ( nr );
    for ( k = 0; k < nt; ++k )
    {
        const Set3<nat> & t = trian[k];
        const nat na = 3 * k;
        const nat nb = na + 1;
        const nat nc = nb + 1;
        SemiRib & ra = rib[na];
        ra.next = nb;
        ra.twin = nr;
        ra.vert = t.a;
        SemiRib & rb = rib[nb];
        rb.next = nc;
        rb.twin = nr;
        rb.vert = t.b;
        SemiRib & rc = rib[nc];
        rc.next = na;
        rc.twin = nr;
        rc.vert = t.c;
        SortItem<Set2<nat>, nat> & sa = sar[na];
        sa.head = t.a < t.b ? Set2<nat> ( t.a, t.b ) : Set2<nat> ( t.b, t.a );
        sa.tail = na;
        SortItem<Set2<nat>, nat> & sb = sar[nb];
        sb.head = t.b < t.c ? Set2<nat> ( t.b, t.c ) : Set2<nat> ( t.c, t.b );
        sb.tail = nb;
        SortItem<Set2<nat>, nat> & sc = sar[nc];
        sc.head = t.c < t.a ? Set2<nat> ( t.c, t.a ) : Set2<nat> ( t.a, t.c );
        sc.tail = nc;
    }
    quickSort123 ( sar );
    for ( i = 1; i < nr; ++i )
    {
        SortItem<Set2<nat>, nat> & sa = sar[i];
        SortItem<Set2<nat>, nat> & sb = sar[i-1];
        if ( sa == sb )
        {
            rib[sa.tail].twin = sb.tail;
            ++i;
        }
    }
    const Min2a<double> merge;
    const TQ_MinTan<double, Vector2d> quality ( vert );
    maxL1<double> ( quality, merge, rib );
    return true;
}

ArrRef<Set3<nat> > & rebuildDelauney ( CCArrRef<Vector2d> & vert, ArrRef<Set3<nat> > & res )
{
    DynArray<SemiRib> rib;
    rebuildDelauney ( vert, res, rib );
    const nat nv = vert.size();
    for ( nat i = 0, k = 0; i < rib.size(); ++i )
    {
        SemiRib & a = rib[i];
        if ( a.vert >= nv ) continue;
        SemiRib & b = rib[a.next];
        SemiRib & c = rib[b.next];
        Set3<nat> & t = res[k++];
        t.a = a.vert;
        t.b = b.vert;
        t.c = c.vert;
        a.vert = b.vert = c.vert = nv;
    }
    return res;
}

//**************************** 01.08.2007 *********************************//
//
//      Проверка на наличие триангуляции с положительными треугольниками
//
//**************************** 27.08.2011 *********************************//

bool trianTestNat1L1Area ( CCArrRef<Vector2d> & vert )
{
    const TQ_Area<double, Vector2d> quality ( vert );
    return tempTrianTestNat1L1<double, Vector2d, double> ( quality, vert );
}

bool trianTestNat1L1AdivR2 ( CCArrRef<Vector2d> & vert )
{
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return tempTrianTestNat1L1<double, Vector2d, double> ( quality, vert );
}

bool trianTestNat1L1MinTan ( CCArrRef<Vector2d> & vert )
{
    const TQ_MinTan<double, Vector2d> quality ( vert );
    return tempTrianTestNat1L1<double, Vector2d, double> ( quality, vert );
}

//**************************** 07.08.2008 *********************************//
//
//              Разбиение многоугольника на выпуклые части
//
//**************************** 27.08.2018 *********************************//

bool convexParts ( CCArrRef<Vector2d> & vert, Suite<nat> & cntr, Suite<nat> & index )
{
    cntr.resize();
    index.resize();
    DynArray<SemiRib> rib;
    Suite<Set3<nat> > trian;
// Делаем триангуляцию
    trianSweepLine ( vert, trian );
    if ( ! rebuildDelauney ( vert, trian, rib ) ) return false;
// Находим диагонали
    nat i;
    const nat nv = vert.size();
    const nat nr = rib.size();
    DynArray<nat> prev ( nr );
    Suite<SortItem<double, nat> > diag;
    for ( i = 0; i < nr; ++i )
    {
        const SemiRib & r = rib[i];
        prev[i] = rib[r.next].next;
        if ( r.twin >= nr ) continue;
        SortItem<double, nat> & si = diag.inc();
        si.head = qmod ( vert[r.vert] - vert[rib[r.next].vert] );
        si.tail = i;
    }
    quickSort321 ( diag );
// Делаем объединие многоугольников, если оно будет выпуклым
    for ( i = 0; i < diag.size(); ++i )
    {
        const nat i1 = diag[i].tail;
        SemiRib & r1 = rib[i1];
        const nat i2 = r1.twin;
        SemiRib & r2 = rib[i2];
        const Vector2d & v1 = vert[rib[prev[i1]].vert];
        const Vector2d & v2 = vert[r1.vert];
        const Vector2d & v3 = vert[rib[rib[r2.next].next].vert];
        if ( ( v2 - v1 ) % ( v3 - v2 ) < 0 ) continue;
        const Vector2d & u1 = vert[rib[prev[i2]].vert];
        const Vector2d & u2 = vert[r2.vert];
        const Vector2d & u3 = vert[rib[rib[r1.next].next].vert];
        if ( ( u2 - u1 ) % ( u3 - u2 ) < 0 ) continue;
        rib[prev[i1]].next = r2.next;
        rib[prev[i2]].next = r1.next;
        prev[r1.next] = prev[i2];
        prev[r2.next] = prev[i1];
        r1.vert = r2.vert = nv;
    }
// Запись результата
    for ( i = 0; i < nr; ++i )
    {
        if ( rib[i].vert == nv ) continue;
        nat & n = cntr.inc();
        n = 0;
        nat j = i;
        do
        {
            ++n;
            SemiRib & rj = rib[j];
            index.inc() = rj.vert;
            rj.vert = nv;
            j = rj.next;
        }
        while ( j != i );
    }
    return true;
}

//**************************** 16.01.2023 *********************************//
//
//              Разделение многоугольника с дырками на части
//
//**************************** 16.01.2023 *********************************//

bool splitPolygon ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vert, Suite<nat> & cntr2, Suite<nat> & index )
{
    cntr2.resize();
    index.resize();
    DynArray<SemiRib> rib;
    Suite<Set3<nat> > trian;
// Делаем триангуляцию
    trianSweepLine ( cntr, vert, trian );
    if ( ! rebuildDelauney ( vert, trian, rib ) ) return false;
// Находим диагонали
    nat i;
    const nat nv = vert.size();
    const nat nr = rib.size();
    DynArray<nat> prev ( nr );
    Suite<SortItem<double, nat> > diag;
    for ( i = 0; i < nr; ++i )
    {
        const SemiRib & r = rib[i];
        prev[i] = rib[r.next].next;
        if ( r.twin >= nr ) continue;
        SortItem<double, nat> & si = diag.inc();
        si.head = qmod ( vert[r.vert] - vert[rib[r.next].vert] );
        si.tail = i;
    }
    quickSort321 ( diag );
// Делаем объединие многоугольников, если у него не будет одинаковых вершин
    Suite<nat> buf;
    for ( i = 0; i < diag.size(); ++i )
    {
        const nat i1 = diag[i].tail;
        SemiRib & r1 = rib[i1];
        const nat i2 = r1.twin;
        SemiRib & r2 = rib[i2];
        buf.resize();
        nat j = r1.next;
        while ( j != i1 )
        {
            const SemiRib & rj = rib[j];
            buf.inc() = rj.vert;
if ( buf.size() > nr ) // этого не должно быть
    return false;
            j = rj.next;
        }
        j = r2.next;
        while ( j != i2 )
        {
            const SemiRib & rj = rib[j];
            buf.inc() = rj.vert;
if ( buf.size() > nr ) // этого не должно быть
    return false;
            j = rj.next;
        }
        quickSort123 ( buf );
        for ( nat k = 1; k < buf.size(); ++k )
        {
            if ( buf[k-1] == buf[k] ) goto m1;
        }
        rib[prev[i1]].next = r2.next;
        rib[prev[i2]].next = r1.next;
        prev[r1.next] = prev[i2];
        prev[r2.next] = prev[i1];
        r1.vert = r2.vert = nv;
m1:;}
// Запись результата
    for ( i = 0; i < nr; ++i )
    {
        if ( rib[i].vert == nv ) continue;
        nat & n = cntr2.inc();
        n = 0;
        nat j = i;
        do
        {
            ++n;
            SemiRib & rj = rib[j];
            index.inc() = rj.vert;
            rj.vert = nv;
            j = rj.next;
        }
        while ( j != i );
    }
    return true;
}
