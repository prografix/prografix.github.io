
#include "atrian.h"
#include "func1t.h"

void optiL ( IDiagFunc & change, ArrRef<SemiRib> & rib )
{
    const nat nr = rib.size();
    if ( nr < 6 || nr % 3 != 0 ) return;
    const nat nf = nr / 3;
    nat i;
    DynArray<nat> dinx ( nr, nr );
    Suite<TriDiag<void> > diag ( nr/2 ); // Массив диагоналей
    for ( i = 0; i < nf; ++i )
    {
        const nat ia = 3 * i;
        const SemiRib & a = rib[ia];
        const nat ib = a.next;
        const SemiRib & b = rib[ib];
        const nat ic = b.next;
        const SemiRib & c = rib[ic];
        if ( a.twin < nr )
        {
            dinx[ia] = dinx[a.twin] = diag.size();
            TriDiag<void> & d = diag.inc();
            d.facet1 = i;
            d.facet2 = a.twin/3;
            d.rib = ia;
        }
        if ( b.twin < nr )
        {
            dinx[ib] = dinx[b.twin] = diag.size();
            TriDiag<void> & d = diag.inc();
            d.facet1 = i;
            d.facet2 = b.twin/3;
            d.rib = ib;
        }
        if ( c.twin < nr )
        {
            dinx[ic] = dinx[c.twin] = diag.size();
            TriDiag<void> & d = diag.inc();
            d.facet1 = i;
            d.facet2 = c.twin/3;
            d.rib = ic;
        }
    }
    const nat nd = diag.size();
    Suite<nat> stack ( nd, nd ); // Очередь на проверку оптимальности диагоналей
    for ( i = 0; i < nd; ++i ) stack[i] = i;
    while ( stack.size() > 0 )
    {
        TriDiag<void> & d = diag[stack.las()];
        stack.dec();
        const nat a1 = d.rib;
        const nat b1 = rib[a1].next;
        const nat c1 = rib[b1].next;
        const nat a2 = rib[a1].twin;
        const nat b2 = rib[a2].next;
        const nat c2 = rib[b2].next;
        if ( change ( d.rib, d.facet1, d.facet2 ) )
        {
            rib[a1].vert = rib[c1].vert;
            rib[a1].next = c2;
            rib[c2].next = b1;
            rib[b1].next = a1;
            rib[a2].vert = rib[c2].vert;
            rib[a2].next = c1;
            rib[c1].next = b2;
            rib[b2].next = a2;
            i = dinx[c1];
            if ( i < nr )
            {
                TriDiag<void> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
                ( di.facet1 == d.facet1 ? di.facet1 : di.facet2 ) = d.facet2;
            }
            i = dinx[b1];
            if ( i < nr )
            {
                TriDiag<void> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
            }
            i = dinx[c2];
            if ( i < nr )
            {
                TriDiag<void> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
                ( di.facet1 == d.facet2 ? di.facet1 : di.facet2 ) = d.facet1;
            }
            i = dinx[b2];
            if ( i < nr )
            {
                TriDiag<void> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
            }
        }
        d.rib += nr;
    }
}

void optiL ( IDiagFunc & change, CCArrRef<Set3<nat> > & trian, DynArray<SemiRib> & rib )
{
    const nat nt = trian.size();
    if ( nt < 2 ) return;
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
    optiL ( change.link ( rib ), rib );
}