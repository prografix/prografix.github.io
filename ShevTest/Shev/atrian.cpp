
#include "atrian.h"
#include "func1t.h"

void initRibDiag ( CCArrRef<Set3<nat>> & trian, DynArray<Set2<nat>> & rib, Suite<nat> & diag )
{
    const nat nt = trian.size();
    const nat nr = 3 * nt;
    const nat nu = 5 * nt;
    rib.resize ( nr );
    // Запишем массив полурёбер rib.
    // Причём рёбра принадлежащие к одному треугольнику должны располагаться последовательно.
    nat i, k;
    DynArray<SortItem<Set2<nat>, nat> > sar ( nr );
    for ( k = 0; k < nt; ++k )
    {
        const Set3<nat> & t = trian[k];
        const nat na = 3 * k;
        const nat nb = na + 1;
        const nat nc = nb + 1;
        Set2<nat> & ra = rib[na];
        ra.a = t.a;
        ra.b = nu;
        Set2<nat> & rb = rib[nb];
        rb.a = t.b;
        rb.b = nu;
        Set2<nat> & rc = rib[nc];
        rc.a = t.c;
        rc.b = nu;
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
    // Запишем массив диагоналей diag
    diag.resize();
    for ( i = 1; i < nr; ++i )
    {
        SortItem<Set2<nat>, nat> & sa = sar[i];
        SortItem<Set2<nat>, nat> & sb = sar[i-1];
        if ( sa == sb )
        {
            nat j = diag.size();
            diag.inc(3);
            diag[j] = j + 3;
            diag[rib[sa.tail].b = j+1] = sa.tail;
            diag[rib[sb.tail].b = j+2] = sb.tail;
            ++i;
        }
    }
}

void optiL ( Func4a<bool,nat> & change, ArrRef<Set2<nat>> & rib, ArrRef<nat> & diag )
{
    const nat nr = rib.size();
    if ( nr < 6 || nr % 3 != 0 ) return;
    const nat nd = diag.size();
    if ( nd < 3 || nd % 3 != 0) return;
    const nat m = nd + 1; // Метка для диагоналей вне списка проверок
    nat list = 0;
    // Проверка диагоналей
    while ( list < nd )
    {
        nat a1 = diag[list+1], b1, c1;
        switch ( a1 % 3 )
        {
        case 0:
            b1 = a1 + 1;
            c1 = b1 + 1; break;
        case 1:
            b1 = a1 + 1;
            c1 = b1 - 2; break;
        case 2:
            b1 = a1 - 2;
            c1 = b1 + 1; break;
        }
        nat a2 = diag[list+2], b2, c2;
        switch ( a2 % 3 )
        {
        case 0:
            b2 = a2 + 1;
            c2 = b2 + 1; break;
        case 1:
            b2 = a2 + 1;
            c2 = b2 - 2; break;
        case 2:
            b2 = a2 - 2;
            c2 = b2 + 1; break;
        }
        // Вывод диагонали из списка проверок
        nat & d = diag[list];
        list = d;
        d = m;
        // Проверка на изменение диагонали
        if ( ! change ( b1, c1, b2, c2 ) ) continue;
        //
        //     b1 a2              a2
        //    / | | \            /  \
        //   /  | |  \        b2/    \c2
        // c1   | |   c2  ->    ======
        //   \  | |  /        c1\    /b1
        //    \ | | /            \  /
        //     a1 b2              a1
        //
        Set2<nat> & ra1 = rib[a1];
        Set2<nat> & rb1 = rib[b1];
        Set2<nat> & rc1 = rib[c1];
        Set2<nat> & ra2 = rib[a2];
        Set2<nat> & rb2 = rib[b2];
        Set2<nat> & rc2 = rib[c2];
        rb1.a = rc2.a;
        rb2.a = rc1.a;
        const nat db1 = rb1.b;
        rb1.b = ra1.b;
        diag[rb1.b] = b1;
        const nat db2 = rb2.b;
        rb2.b = ra2.b;
        diag[rb2.b] = b2;
        ra1.b = db2;
        if ( ra1.b < nd ) diag[ra1.b] = a1;
        ra2.b = db1;
        if ( ra2.b < nd ) diag[ra2.b] = a2;
        // Добавим соседние диагонали в список проверок, если их там нет
        if ( ra1.b < nd )
        {
            const nat da1 = ra1.b - ra1.b % 3;
            if ( diag[da1] == m )
            {
                diag[da1] = list;
                list = da1;
            }
        }
        if ( rc1.b < nd )
        {
            const nat dc1 = rc1.b - rc1.b % 3;
            if ( diag[dc1] == m )
            {
                diag[dc1] = list;
                list = dc1;
            }
        }
        if ( ra2.b < nd )
        {
            const nat da2 = ra2.b - ra2.b % 3;
            if ( diag[da2] == m )
            {
                diag[da2] = list;
                list = da2;
            }
        }
        if ( rc2.b < nd )
        {
            const nat dc2 = rc2.b - rc2.b % 3;
            if ( diag[dc2] == m )
            {
                diag[dc2] = list;
                list = dc2;
            }
        }
    }
}