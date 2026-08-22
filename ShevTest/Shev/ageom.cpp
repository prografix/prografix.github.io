
#include "ageom.h"

static void insertSort ( ArrRef<Set2<nat> > & a, ICutPolygonGuru & x )
{
    for ( nat j = 1; j < a.size(); ++j )
    {
        const Set2<nat> b = a[j];
        int i = j - 1;
        if ( x.isOrder ( a[i].b, b.b ) ) continue;
        do
        {
            a[i+1] = a[i];
            if ( --i < 0 ) break;
        }
        while ( x.isOrder ( b.b, a[i].b ) );
        a[i+1] = b;
    }
}

// Если три точки подряд имеют нулевой статус, то среднюю из них удаляем

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
//**************************** 20.08.2026 *********************************//

bool cutPolygon ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & minus )
{
    minus.resize();
    CCArrRef<int> & status = guru.getStatus();
    const nat n = status.size();
    if ( n < 3 )
        return false;
// Найдём пересечения многоугольника с гиперплоскостью
    nat i, ip = n - 1;
    const nat n2 = n / 2;
    DynArray<Set2<nat> > arr ( n );
    LtdSuiteRef<Set2<nat> > vi ( arr, 0, n2 ), vo ( arr, n2, n2 );
    int sum = 0;
    for ( i = 0; i < n; ++i )
    {
        const int vp = status[ip];
        const int vc = status[i];
        sum += vc;
        if ( vp < 0 && vc >= 0 )
        {
            if ( vc > 0 || status.cnext(i) >= 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = i;
                si.b = vc > 0 ? guru.newVert ( ip, i ) : i;
            }
        }
        else
        if ( vc < 0 && vp >= 0 )
        {
            if ( vp > 0 || status.cprev(ip) >= 0 )
            {
                Set2<nat> & si = vi.inc();
                si.a = i;
                si.b = vp > 0 ? guru.newVert ( i, ip ) : ip;
            }
        }
        ip = i;
    }
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
    DynArray<Set2<nat> > arr2 ( m + m );
    ArrRef<Set2<nat> > si ( arr2, 0, m ), so ( arr2, m, m );
    for ( i = 0; i < m; ++i )
    {
        si[i].a = so[i].a = i;
        si[i].b = vi[i].b;
        so[i].b = vo[i].b;
    }
    insertSort ( si, guru );
    insertSort ( so, guru );
    CmbArray<nat, 8> outPos ( m );
    for ( i = 0; i < m; ++i )
    {
        outPos[so[i].a] = i; // завести массив outPos подсказал ИИ
    }
    for ( nat j = 0; j < m; ++j )
    {
        if ( si[j].a == m ) continue;
        Suite<nat> & s = minus.inc();
        s.resize();
        for ( nat k = j;; )
        {
            const nat c = si[k].a;
            si[k].a = m;
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
//**************************** 20.08.2026 *********************************//

bool cutPolygon ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & plus, SuiteRef< Suite<nat> > & minus )
{
    plus.resize();
    minus.resize();
    CCArrRef<int> & status = guru.getStatus();
    const nat n = status.size();
    if ( n < 3 )
        return false;
// Найдём пересечения многоугольника с гиперплоскостью
    nat i, ip = n - 1;
    const nat n2 = n / 2;
    DynArray<Set2<nat> > arr ( n );
    LtdSuiteRef<Set2<nat> > vi ( arr, 0, n2 ), vo ( arr, n2, n2 );
    int sum = 0;
    for ( i = 0; i < n; ++i )
    {
        const int vp = status[ip];
        const int vc = status[i];
        sum += vc;
        if ( vp < 0 && vc >= 0 )
        {
            if ( vc > 0 || status.cnext(i) >= 0 )
            {
                Set2<nat> & si = vo.inc();
                si.a = i;
                si.b = vc > 0 ? guru.newVert ( ip, i ) : i;
            }
        }
        else
        if ( vc < 0 && vp >= 0 )
        {
            if ( vp > 0 || status.cprev(ip) >= 0 )
            {
                Set2<nat> & si = vi.inc();
                si.a = i;
                si.b = vp > 0 ? guru.newVert ( i, ip ) : ip;
            }
        }
        ip = i;
    }
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
    CmbArray<Set2<nat>, 8> arr2 ( m + m );
    ArrRef<Set2<nat> > si ( arr2, 0, m ), so ( arr2, m, m );
    for ( i = 0; i < m; ++i )
    {
        si[i].a = so[i].a = i;
        si[i].b = vi[i].b;
        so[i].b = vo[i].b;
    }
    insertSort ( si, guru );
    insertSort ( so, guru );
    CmbArray<nat, 8> arr3 ( m + m );
    ArrRef<nat> inPos ( arr3, 0, m ), outPos ( arr3, m, m );
    for ( i = 0; i < m; ++i )
    {
        inPos[si[i].a] = outPos[so[i].a] = i; // завести массивы inPos и outPos подсказал ИИ
    }
    DynArray<bool> used ( m, false );
    for ( nat j = 0; j < m; ++j )
    {
        if ( used[j] ) continue;
        Suite<nat> & s = plus.inc();
        s.resize();
        for ( nat k = j;; )
        {
            used[k] = true;
            const nat c = so[k].a;
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
            const nat c = si[k].a;
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

static void addVert ( Suite<nat> & s, nat v )
{
    if ( ! s.size() || s.las() != v ) s.inc() = v;
}
// дуга [iBeg, iEnd) по кольцу 0..n-1, затем точка на сечении vEnd
static void addArc ( Suite<nat> & s, nat iBeg, nat iEnd, nat vBeg, nat vEnd, nat n )
{
    s.inc() = vBeg;
    for ( nat i = iBeg; i != iEnd; )
    {
        addVert ( s, i );
        if ( ++i == n ) i = 0;
    }
    if ( s.las() != vEnd && s[0] != vEnd ) s.inc() = vEnd;
}
static void addSide ( SuiteRef< Suite<nat> > & side, Suite<nat> & s )
{
    if ( s.size() < 3 ) side.dec();
}
bool cutPolygon2 ( ICutPolygonGuru & guru, SuiteRef< Suite<nat> > & plus, SuiteRef< Suite<nat> > & minus )
{
    plus.resize();
    minus.resize();
    CCArrRef<int> & status = guru.getStatus();
    const nat n = status.size();
    if ( n < 3 ) return false;
    nat i, ip = n - 1;
    const nat n2 = n / 2;
    DynArray<Set2<nat> > arr ( n );
    LtdSuiteRef<Set2<nat> > vi ( arr, 0, n2 ), vo ( arr, n2, n2 );
    int sum = 0;
    for ( i = 0; i < n; ++i )
    {
        const int vp = status[ip];
        const int vc = status[i];
        sum += vc;
        if ( vp < 0 && vc >= 0 )
        {
            if ( vc > 0 || status.cnext(i) >= 0 )
            {
                Set2<nat> & e = vo.inc();
                e.a = i; // индекс записи выхода (первая неотрицательная)
                e.b = vc > 0 ? guru.newVert ( ip, i ) : i;
            }
        }
        else
        if ( vc < 0 && vp >= 0 )
        {
            if ( vp > 0 || status.cprev(ip) >= 0 )
            {
                Set2<nat> & e = vi.inc();
                e.a = i;
                e.b = vp > 0 ? guru.newVert ( i, ip ) : ip;
            }
        }
        ip = i;
    }
    const nat m = vo.size();
    if ( m == 0 )
    {
        Suite<nat> & poly = sum < 0 ? minus.inc() : plus.inc();
        poly.resize ( n );
        for ( i = 0; i < n; ++i ) poly[i] = i;
        return true;
    }
    if ( m > 1 && vo[0].a < vi[0].a ) vo <<= 1;
    DynArray<Set2<nat> > ord ( m + m );
    ArrRef<Set2<nat> > si ( ord, 0, m ), so ( ord, m, m );
    for ( i = 0; i < m; ++i )
    {
        si[i].a = so[i].a = i;
        si[i].b = vi[i].b;
        so[i].b = vo[i].b;
    }
    insertSort ( si, guru );
    insertSort ( so, guru );
    DynArray<nat> inPos ( m ), outPos ( m );
    for ( i = 0; i < m; ++i )
    {
        inPos[si[i].a] = outPos[so[i].a] = i;
    }
    DynArray<bool> seen ( m, false );
    for ( nat start = 0; start < m; ++start )
    {
        if ( seen[start] ) continue;
        Suite<nat> & s = minus.inc();
        s.resize();
        nat k = start;
        for (;;)
        {
            seen[k] = true;
            const nat c = si[k].a;
            addArc ( s, vi[c].a, vo[c].a, vi[c].b, vo[c].b, n );
            k = outPos[c];          // тот же индекс на прямой -> следующий вход
            if ( k == start ) break;
        }
        addSide ( minus, s );
    }
    seen.fill ( false );
    for ( nat start = 0; start < m; ++start )
    {
        if ( seen[start] ) continue;
        Suite<nat> & s = plus.inc();
        s.resize();
        nat k = start;
        for (;;)
        {
            seen[k] = true;
            const nat c = so[k].a;
            const nat c1 = c + 1 < m ? c + 1 : 0;
            addArc ( s, vo[c].a, vi[c1].a, vo[c].b, vi[c1].b, n );
            k = inPos[c1];
            if ( k == start ) break;
        }
        addSide ( plus, s );
    }
    return true;
}