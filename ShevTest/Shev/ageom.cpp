
#include "ageom.h"

//**************************** 20.08.2026 *********************************//
//
//               Отсечение положительной части многоугольника
//
//**************************** 16.10.2026 *********************************//

static void makePolygons ( const nat n, CCArrRef<Set2<nat> > & vi, ArrRef<Set2<nat> > & vo, ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & minus )
{
    const nat m = vo.size();
// Пересечение с границей - это один отрезок
    if ( m == 1 )
    {
        Suite<nat> & s = minus.inc();
        s.resize();
        s.inc() = vi[0].b;
        for ( nat i = vi[0].a;; )
        {
            s.inc() = i;
            if ( i == vo[0].a ) break;
            if ( ++i == n ) i = 0;
        }
        s.inc() = vo[0].b;
        return;
    }
// Пересечение с границей - это несколько отрезков
    if ( vo[0].a > vo[1].a ) vo <<= 1;
    if ( vo[0].a < vi[0].a ) vo <<= 1;
    nat i;
    DynArray<nat> arr2 ( 3 * m );
    ArrRef<nat> si ( arr2, 0, m ), so ( arr2, m, m ), outPos ( arr2, m+m, m );
    for ( i = 0; i < m; ++i )
    {
        si[i] = vi[i].b;
        so[i] = vo[i].b;
    }
    guru.arrange ( si, so );
    for ( i = 0; i < m; ++i )
    {
        outPos[so[i]] = i; // завести массив outPos подсказал ИИ
    }
    for ( nat j = 0; j < m; ++j )
    {
        if ( si[j] == m ) continue;
        Suite<nat> & s = minus.inc();
        s.resize();
        for ( nat k = j;; )
        {
            const nat c = si[k];
            const Set2<nat> & vik = vi[c];
            const Set2<nat> & vok = vo[c];
            si[k] = m;
            s.inc() = vik.b;
            for ( i = vik.a;; )
            {
                s.inc() = i;
                if ( i == vok.a ) break;
                if ( ++i == n ) i = 0;
            }
            s.inc() = vok.b;
            k = outPos[c];
            if ( k == j ) break;
        }
    }
}

bool cutPolygon ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & minus )
{
    minus.resize();
    CCArrRef<int> & status = guru.getStatus();
    const nat n = status.size();
    if ( n < 3 )
        return false;
// Найдём пересечения рёбер с границей
    nat ia = n - 1;
    const nat n2 = n / 2;
    DynArray<Set2<nat> > arr ( n );
    LtdSuiteRef<Set2<nat> > vi ( arr, 0, n2 ), vo ( arr, n2, n2 ); // Входящие и выходящие вершины (b), (a) - соседние отрицательные вершины
    int sum = 0; // Сумма всех статусов
// Считаем, что все вершины с положительным и нулевым статусом лежат за границей
// за исключением одиночных вершин с нулевым статусом
    for ( nat ib = 0; ib < n; ++ib )
    {
        const int va = status[ia];
        const int vb = status[ib];
        sum += vb;
// vp | va | vb | vn |  r  |
//    | -1 |  1 |    | a-b | o
//    | -1 |  0 | >= |  b  | o
//    |  1 | -1 |    | a-b | i
// >= |  0 | -1 |    |  a  | i
        if ( va < 0 )
        {
            if ( vb > 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = ia;
                si.b = guru.newVert ( ia, ib );
            }
            else
            if ( !vb && status.cnext(ib) >= 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = ia;
                si.b = ib;
            }
        }
        else
        if ( vb < 0 )
        {
            if ( va > 0 )
            {
                Set2<nat> & si = vi.inc();
                si.a = ib;
                si.b = guru.newVert ( ia, ib );
            }
            else
            if ( !va && status.cprev(ia) >= 0 )
            {
                Set2<nat> & si = vi.inc();
                si.a = ib;
                si.b = ia;
            }
        }
        ia = ib;
    }
    if ( vi.size() != vo.size() )
        return false;
    if ( vo.size() == 0 )
    {
        // Нет пересечения с границей
        if ( sum < 0 )
        {
            Suite<nat> & poly = minus.inc();
            poly.resize(n);
            for ( nat i = 0; i < n; ++i ) poly[i] = i;
        }
    }
    else
    {
        // Есть пересечение с границей
        makePolygons ( n, vi, vo, guru, minus );
    }
    return true;
}

//**************************** 20.08.2026 *********************************//
//
//               Разрезание многоугольника на две части
//
//**************************** 16.10.2026 *********************************//

bool cutPolygon ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & plus, SuiteRef< Suite<nat> > & minus )
{
    plus.resize();
    minus.resize();
    CCArrRef<int> & status = guru.getStatus();
    const nat n = status.size();
    if ( n < 3 )
        return false;
// Найдём пересечения рёбер с границей
    nat ia = n - 1;
    const nat n2 = n / 2;
    DynArray<Set2<nat> > arr ( n+n );
    LtdSuiteRef<Set2<nat> > mi ( arr, 0, n2 ), mo ( arr, n2, n2 ), pi ( arr, n, n2 ), po ( arr, n+n2, n2 );
    int sum = 0; // Сумма всех статусов
    for ( nat ib = 0; ib < n; ++ib )
    {
        const int va = status[ia];
        const int vb = status[ib];
        sum += vb;
// minus
// vp | va | vb | vn |  r  |
//    | -1 |  1 |    | a-b | o
//    | -1 |  0 | >= |  b  | o
//    |  1 | -1 |    | a-b | i
// >= |  0 | -1 |    |  a  | i
// plus
// vp | va | vb | vn |  r  |
//    |  1 | -1 |    | a-b | o
//    |  1 |  0 | <= |  b  | o
//    | -1 |  1 |    | a-b | i
// <= |  0 |  1 |    |  a  | i
        if ( va < 0 )
        {
            if ( vb > 0 )
            {
                const nat iv = guru.newVert ( ia, ib );
                Set2<nat> & ms = mo.inc();
                ms.a = ia;
                ms.b = iv;
                Set2<nat> & ps = pi.inc();
                ps.a = ib;
                ps.b = iv;
            }
            else
            if ( !vb && status.cnext(ib) >= 0 )
            {
                Set2<nat> & ms = mo.inc();
                ms.a = ia;
                ms.b = ib;
            }
        }
        else
        if ( va > 0 )
        {
            if ( vb < 0 )
            {
                const nat iv = guru.newVert ( ia, ib );
                Set2<nat> & ms = mi.inc();
                ms.a = ib;
                ms.b = iv;
                Set2<nat> & ps = po.inc();
                ps.a = ia;
                ps.b = iv;
            }
            else
            if ( !vb && status.cnext(ib) <= 0 )
            {
                Set2<nat> & ps = po.inc();
                ps.a = ia;
                ps.b = ib;
            }
        }
        else
        {
            if ( vb < 0 && status.cprev(ia) >= 0 )
            {
                Set2<nat> & ms = mi.inc();
                ms.a = ib;
                ms.b = ia;
            }
            else
            if ( vb > 0 && status.cprev(ia) <= 0 )
            {
                Set2<nat> & ps = pi.inc();
                ps.a = ib;
                ps.b = ia;
            }
        }
        ia = ib;
    }
    if ( mi.size() != mo.size() )
        return false;
    if ( pi.size() != po.size() )
        return false;
    if ( mo.size() == 0 )
    {
        // Нет пересечения с границей
        if ( sum < 0 )
        {
            Suite<nat> & poly = minus.inc();
            poly.resize(n);
            for ( nat i = 0; i < n; ++i ) poly[i] = i;
        }
    }
    else
    {
        // Есть пересечение с границей
        makePolygons ( n, mi, mo, guru, minus );
    }
    if ( po.size() == 0 )
    {
        // Нет пересечения с границей
        if ( sum > 0 )
        {
            Suite<nat> & poly = plus.inc();
            poly.resize(n);
            for ( nat i = 0; i < n; ++i ) poly[i] = i;
        }
    }
    else
    {
        // Есть пересечение с границей
        makePolygons ( n, pi, po, guru, plus );
    }
    return true;
}