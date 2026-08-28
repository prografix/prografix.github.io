
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