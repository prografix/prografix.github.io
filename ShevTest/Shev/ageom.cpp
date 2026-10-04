
#include "ageom.h"

//**************************** 20.08.2026 *********************************//
//
//      Если в многоугольнике три точки подряд имеют нулевой статус, 
//      то среднюю из них удаляем.
// 
//**************************** 20.08.2026 *********************************//

static void checkPolygon ( Suite<nat> & poly, CCArrRef<int> & status )
{
    if ( poly.size() >= 3 )
    {
        const nat ns = status.size();
        for ( nat ic = poly.size(); ic-- > 0; )
        {
            const nat sc = poly[ic];
            if ( sc < ns && status[sc] ) continue;
            nat ip = ic + 1;
            if ( ip == poly.size() ) ip = 0;
            const nat sp = poly[ip];
            if ( sp < ns && status[sp] ) continue;
            const nat in = ic > 0 ? ic - 1 : poly.size() - 1;
            const nat sn = poly[in];
            if ( sn < ns && status[sn] ) continue;
            poly.delAndShift ( ic );
            if ( poly.size() < 3 ) break;
        }
    }
    if ( poly.size() < 3 ) poly.resize();
}

//**************************** 20.08.2026 *********************************//
//
//               Отсечение положительной части многоугольника
//
//**************************** 26.08.2026 *********************************//

bool cutPolygon ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & minus )
{
    minus.resize();
    CCArrRef<int> & status = guru.getStatus();
    const nat n = status.size();
    if ( n < 3 )
        return false;
    nat ia = n - 1;
    const nat n2 = n / 2;
    DynArray<Set2<nat> > arr ( n );
    LtdSuiteRef<Set2<nat> > vi ( arr, 0, n2 ), vo ( arr, n2, n2 ); // Входящие и выходящие вершины (b), (a) - соседние отрицательные вершины
    nat null = 0; // Счётчик нулевых статусов
    int sum = 0; // Сумма всех статусов
// Найдём пересечения рёбер с границей
    for ( nat ib = 0; ib < n; ++ib )
    {
        const int va = status[ia];
        const int vb = status[ib];
        sum += vb;
        if ( vb > 0 )
        {
            if ( va < 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = ia;
                si.b = guru.newVert ( ia, ib );
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
        }
        else
            ++null;
        ia = ib;
    }
    if ( null > 0 ) // Редкое событие, когда какие-то вершины лежат на границе
    {
        if ( null == n ) // Все вершины лежат на границе
        {
            /*Suite<nat> & poly = minus.inc();
            poly.resize(n);
            for ( nat i = 0; i < n; ++i ) poly[i] = i;*/
            return true;
        }
        nat i1 = 0; // Найдём первый ненулевой статус
        while ( ! status[i1] ) ++i1;
        // Теперь ищем группы нулевых статусов
        nat f0 = n, l0; // индексы первого и последнего члена группы
        for ( nat i = 1; i <= n; ++i )
        {
            nat j = i1 + i;
            if ( j >= n ) j -= n;
            const int vb = status[j];
            if ( ! vb )
            {
                // Строим группу нулевых статусов
                if ( f0 == n ) f0 = j;
                l0 = j;
                continue;
            }
            if ( f0 == n ) // Группы ещё нет
                continue;
            // Рассматриваем построенную группу
            const nat a = f0 > 0 ? f0 - 1 : n - 1;
            const int va = status[a];
            if ( vb > 0 )
            {
                if ( va < 0 )
                {
                    Set2<nat> & si = vo.inc();
                    si.a = a;
                    si.b = f0;
                }
            }
            else
            {
                if ( va > 0 )
                {
                    Set2<nat> & si = vi.inc();
                    si.a = j;
                    si.b = l0;
                }
            }
            f0 = n; // начинаем поиск новой группы нулевых статусов
        }
    }
    if ( vi.size() != vo.size() )
        return false;
    const nat m = vo.size();
// Нет пересечения с границей
    if ( m == 0 )
    {
        if ( sum < 0 )
        {
            Suite<nat> & poly = minus.inc();
            poly.resize(n);
            for ( nat i = 0; i < n; ++i ) poly[i] = i;
        }
        return true;
    }
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
        return true;
    }
// Пересечение с границей - это несколько отрезков
    nat i;
    DynArray<nat> arr2 ( 3 * m );
    ArrRef<nat> si ( arr2, 0, m ), so ( arr2, m, m ), outPos ( arr2, m+m, m );
    if ( vo[0].a < vi[0].a ) vo <<= 1;
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
            si[k] = m;
            const Set2<nat> & vok = vo[so[k]];
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
    return true;
}

bool cutPolygon1 ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & minus )
{
    minus.resize();
    CCArrRef<int> & status = guru.getStatus();
    const nat n = status.size();
    if ( n < 3 )
        return false;
// Найдём пересечения многоугольника с гиперплоскостью
    nat i, ia = n - 1;
    const nat n2 = n / 2;
    DynArray<Set2<nat> > arr ( n );
    LtdSuiteRef<Set2<nat> > vi ( arr, 0, n2 ), vo ( arr, n2, n2 );
    int sum = 0;
    for ( i = 0; i < n; ++i )
    {
        const int va = status[ia];
        const int vb = status[i];
        sum += vb;
// vp | va | vb | vn |  r  |
//    | -1 |  1 |    | a-b | o
// <= |  0 |  1 |    |  a  | o
//    |  1 | -1 |    | a-b | i
//    |  1 |  0 | <= |  b  | i
        if ( vb > 0 )
        {
            if ( va < 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = i;
                si.b = guru.newVert ( ia, i );
            }
            else
            if ( va == 0 && status.cprev(ia) <= 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = i;
                si.b = ia;
            }
        }
        else
        if ( va > 0 )
        {
            if ( vb < 0 )
            {
                Set2<nat> & si = vi.inc();
                si.a = i;
                si.b = guru.newVert ( i, ia );
            }
            else
            if ( status.cnext(i) <= 0 )
            {
                Set2<nat> & si = vi.inc();
                si.a = i;
                si.b = i;
            }
        }
        ia = i;
    }
    if ( vi.size() != vo.size() )
        return false;
    const nat m = vo.size();
// Нет пересечения с гиперплоскостью
    if ( m == 0 )
    {
        if ( sum < 0 )
        {
            Suite<nat> & poly = minus.inc();
            poly.resize(n);
            for ( i = 0; i < n; ++i ) poly[i] = i;
        }
        return true;
    }
// Пересечение с гиперплоскостью - это один отрезок
    if ( m == 1 )
    {
        Suite<nat> & s = minus.inc();
        s.resize();
        s.inc() = vi[0].b;
        for ( i = vi[0].a;; )
        {
            if ( i == vo[0].a ) break;
            if ( i != s.las() ) s.inc() = i;
            if ( ++i == n ) i = 0;
        }
        const nat v = vo[0].b;
        if ( s[0] != v && s.las() != v ) s.inc() = v;
        checkPolygon ( s, status );
        if ( s.size() < 3 ) minus.dec();
        return true;
    }
// Пересечение с гиперплоскостью - это несколько отрезков
    if ( vo[0].a < vi[0].a ) vo <<= 1;
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
            si[k] = m;
            const nat i1 = vo[c].a;
            s.inc() = vi[c].b;
            for ( i = vi[c].a;; )
            {
                if ( i == i1 ) break;
                if ( i != s.las() ) s.inc() = i;
                if ( ++i == n ) i = 0;
            }
            const nat v = vo[c].b;
            if ( s.las() != v && s[0] != v ) s.inc() = v;
            k = outPos[c];
            if ( k == j ) break;
        }
        checkPolygon ( s, status );
        if ( s.size() < 3 ) minus.dec();
    }
    return true;
}

//**************************** 20.08.2026 *********************************//
//
//               Разрезание многоугольника на две части
//
//**************************** 26.08.2026 *********************************//

bool cutPolygon ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & plus, SuiteRef< Suite<nat> > & minus )
{
    plus.resize();
    minus.resize();
    CCArrRef<int> & status = guru.getStatus();
    const nat n = status.size();
    if ( n < 3 )
        return false;
// Найдём пересечения многоугольника с гиперплоскостью
    nat i, ia = n - 1;
    const nat n2 = n / 2;
    DynArray<Set2<nat> > arr ( n );
    LtdSuiteRef<Set2<nat> > vi ( arr, 0, n2 ), vo ( arr, n2, n2 );
    int sum = 0;
    for ( i = 0; i < n; ++i )
    {
        const int va = status[ia];
        const int vb = status[i];
        sum += vb;
// vp | va | vb | vn |  r  |
//    | -1 |  1 |    | a-b | o
// <= |  0 |  1 |    |  a  | o
//    |  1 | -1 |    | a-b | i
//    |  1 |  0 | <= |  b  | i
        if ( vb > 0 )
        {
            if ( va < 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = i;
                si.b = guru.newVert ( ia, i );
            }
            else
            if ( va == 0 && status.cprev(ia) <= 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = i;
                si.b = ia;
            }
        }
        else
        if ( va > 0 )
        {
            if ( vb < 0 )
            {
                Set2<nat> & si = vi.inc();
                si.a = i;
                si.b = guru.newVert ( i, ia );
            }
            else
            if ( status.cnext(i) <= 0 )
            {
                Set2<nat> & si = vi.inc();
                si.a = i;
                si.b = i;
            }
        }
        ia = i;
    }
    if ( vi.size() != vo.size() )
        return false;
    const nat m = vo.size();
// Нет пересечения с гиперплоскостью
    if ( m == 0 )
    {
        Suite<nat> & poly = sum < 0 ? minus.inc() : plus.inc();
        poly.resize(n);
        for ( i = 0; i < n; ++i ) poly[i] = i;
        return true;
    }
// Пересечение с гиперплоскостью - это один отрезок
    if ( m == 1 )
    {
        Suite<nat> & sp = plus.inc();
        sp.resize();
        sp.inc() = vo[0].b;
        for ( i = vo[0].a;; )
        {
            if ( i == vi[0].a ) break;
            if ( i != sp.las() ) sp.inc() = i;
            if ( ++i == n ) i = 0;
        }
        nat v = vi[0].b;
        if ( sp[0] != v && sp.las() != v ) sp.inc() = v;
        checkPolygon ( sp, status );
        if ( sp.size() < 3 ) plus.dec();
        Suite<nat> & sm = minus.inc();
        sm.resize();
        sm.inc() = vi[0].b;
        for ( i = vi[0].a;; )
        {
            if ( i == vo[0].a ) break;
            if ( i != sm.las() ) sm.inc() = i;
            if ( ++i == n ) i = 0;
        }
        v = vo[0].b;
        if ( sm[0] != v && sm.las() != v ) sm.inc() = v;
        checkPolygon ( sm, status );
        if ( sm.size() < 3 ) minus.dec();
        return true;
    }
// Пересечение с гиперплоскостью - это несколько отрезков
    if ( vo[0].a < vi[0].a ) vo <<= 1;
    CmbArray<nat, 16> arr2 ( 4 * m );
    ArrRef<nat> si ( arr2, 0, m ), so ( arr2, m, m ), inPos ( arr2, 2*m, m ), outPos ( arr2, 3*m, m );
    for ( i = 0; i < m; ++i )
    {
        si[i] = vi[i].b;
        so[i] = vo[i].b;
    }
    guru.arrange ( si, so );
    for ( i = 0; i < m; ++i )
    {
        inPos[si[i]] = outPos[so[i]] = i; // завести массивы inPos и outPos подсказал ИИ
    }
    CmbArray<bool, 4> used ( m, false );
    for ( nat j = 0; j < m; ++j )
    {
        if ( used[j] ) continue;
        Suite<nat> & s = plus.inc();
        s.resize();
        for ( nat k = j;; )
        {
            used[k] = true;
            const nat c = so[k];
            const nat c1 = c + 1 < m ? c + 1 : 0;
            const nat i1 = vi[c1].a;
            s.inc() = vo[c].b;
            for ( i = vo[c].a;; )
            {
                if ( i == i1 ) break;
                if ( i != s.las() ) s.inc() = i;
                if ( ++i == n ) i = 0;
            }
            const nat v = vi[c1].b;
            if ( s.las() != v && s[0] != v ) s.inc() = v;
            k = inPos[c1];
            if ( k == j ) break;
        }
        checkPolygon ( s, status );
        if ( s.size() < 3 ) plus.dec();
    }
    used.fill ( false );
    for ( nat j = 0; j < m; ++j )
    {
        if ( used[j] ) continue;
        Suite<nat> & s = minus.inc();
        s.resize();
        for ( nat k = j;; )
        {
            used[k] = true;
            const nat c = si[k];
            const nat i1 = vo[c].a;
            s.inc() = vi[c].b;
            for ( i = vi[c].a;; )
            {
                if ( i == i1 ) break;
                if ( i != s.las() ) s.inc() = i;
                if ( ++i == n ) i = 0;
            }
            const nat v = vo[c].b;
            if ( s.las() != v && s[0] != v ) s.inc() = v;
            k = outPos[c];
            if ( k == j ) break;
        }
        checkPolygon ( s, status );
        if ( s.size() < 3 ) minus.dec();
    }
    return true;
}