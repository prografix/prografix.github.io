
#include <math.h>
#include "mathem.h"
#include "LinAlg.h"
#include "Vector4d.h"
#include "DoubleN.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "func2d.h"
#include "TrianFacet.h"

// Задание векторных норм

Vector4d & Vector4d::setNorm1 ( double p ) // единичная норма
{
    const double t = fabs ( x1 ) + fabs ( x2 ) + fabs ( x3 ) + fabs ( x4 );
    if ( t > 0 )
    {
        p /= t;
        x1 *= p;
        x2 *= p;
        x3 *= p;
        x4 *= p;
    }
    return * this;
}

Vector4d & Vector4d::setNorm2 ( double p ) // квадратичная норма
{
    const double t = x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4;
    if ( t > 0 )
    {
        p /= sqrt ( t );
        x1 *= p;
        x2 *= p;
        x3 *= p;
        x4 *= p;
    }
    return * this;
}

Vector4d & Vector4d::setNormU ( double p ) // бесконечная норма
{
    const double t = _max ( fabs ( x1 ), fabs ( x2 ), fabs ( x3 ), fabs ( x4 ) );
    if ( t > 0 )
    {
        p /= t;
        x1 *= p;
        x2 *= p;
        x3 *= p;
        x4 *= p;
    }
    return * this;
}

// Вычисление векторных норм

double norm1 ( const Vector4d & v ) // единичная норма
{
    return fabs ( v.x1 ) + fabs ( v.x2 ) + fabs ( v.x3 ) + fabs ( v.x4 );
}

double norm2 ( const Vector4d & v ) // квадратичная норма
{
    return sqrt ( v.x1 * v.x1 + v.x2 * v.x2 + v.x3 * v.x3 + v.x4 * v.x4 );
}

double normU ( const Vector4d & v ) // бесконечная норма
{
    return _max ( fabs ( v.x1 ), fabs ( v.x2 ), fabs ( v.x3 ), fabs ( v.x4 ) );
}

//**************************** 22.04.2019 *********************************//
//
//                Пересечение четырёх гиперплоскостей
//
//**************************** 22.04.2019 *********************************//

Def<Vector4d> intersection ( const Plane4d & plane1, const Plane4d & plane2, const Plane4d & plane3, const Plane4d & plane4 )
{
    SLU4<double> slu;
    slu.aa = plane1.norm.x1;   slu.ab = plane1.norm.x2;   slu.ac = plane1.norm.x3;   slu.ad = plane1.norm.x4;   slu.ae = -plane1.dist;
    slu.ba = plane2.norm.x1;   slu.bb = plane2.norm.x2;   slu.bc = plane2.norm.x3;   slu.bd = plane2.norm.x4;   slu.be = -plane2.dist;
    slu.ca = plane3.norm.x1;   slu.cb = plane3.norm.x2;   slu.cc = plane3.norm.x3;   slu.cd = plane3.norm.x4;   slu.ce = -plane3.dist;
    slu.da = plane4.norm.x1;   slu.db = plane4.norm.x2;   slu.dc = plane4.norm.x3;   slu.dd = plane4.norm.x4;   slu.de = -plane4.dist;
    Def<Vector4d> point;
    point.isDef = slu.gauss ( point.x1, point.x2, point.x3, point.x4 );
    return point;
}

//************************ 09.05.2019 *******************************//
//
//      Вычисление ближайшей точки к заданным плоскостям
//      Минимум суммы квадратов расстояний ( 2-норма )
//
//************************ 09.05.2019 *******************************//

Def<Vector4d> getNearPoint2 ( CCArrRef<Plane4d> & plane )
{
    Def<Vector4d> res;
    const nat n = plane.size();
    if ( n < 4 ) return res;
    HMatrix<double> a ( n, 4 );
    CmbArray<double, 80> b ( n );
    for ( nat i = 0; i < n; ++i )
    {
        const Plane4d & pi = plane[i];
        b[i] = - pi.dist;
        double * row = a[i];
        row[0] = pi.norm.x1;
        row[1] = pi.norm.x2;
        row[2] = pi.norm.x3;
        row[3] = pi.norm.x4;
    }
    FixArray<double, 4> c;
    if ( res.isDef = ortholin ( a, b, c ) )
    {
        res.x1 = c[0];
        res.x2 = c[1];
        res.x3 = c[2];
        res.x4 = c[3];
    }
    return res;
}

//************************ 28.04.2019 *******************************//
//
//      Вычисление ближайшей точки к заданным плоскостям
//      Минимум максимального расстояния ( бесконечная норма )
//
//************************ 12.06.2019 *******************************//

Def<Vector4d> getNearPointU ( CCArrRef<Plane4d> & plane )
{
    Def<Vector4d> res;
    const nat nn = plane.size();
    if ( nn < 4 ) return res;
    if ( nn == 4 ) return intersection ( plane[0], plane[1], plane[2], plane[3] );
    // Поиск первоначального оптимума
    CmbArray2<double, 36> data ( 4, nn );
    CmbArray<double, 9> w ( nn );
    CmbArray<nat, 10> index ( nn );
    nat i, j;
    for ( i = 0; i < nn; ++i )
    {
        const Plane4d & p = plane[i];
        data[0][i] = p.norm.x1;
        data[1][i] = p.norm.x2;
        data[2][i] = p.norm.x3;
        data[3][i] = p.norm.x4;
    }
    if ( ! sluGaussRow ( data, 4, nn, index(), 4, nn ) ) return res;
    double max = 0, summ = 0;
    nat jm = 4;
    for ( j = 4; j < nn; ++j )
    {
        w.fill ( 0 );
        double norm = 1;
        const nat nj = index[j];
        for ( i = 0; i < 4; ++i )
        {
            norm += fabs ( w[index[i]] = data[i][nj] );
        }
        w[nj] = -1;
        double sum = 0;
        for ( nat k = 0; k < nn; ++k ) sum += plane[k].dist * w[k];
        const double t = fabs ( sum ) / norm;
        if ( max < t ) max = t, jm = j, summ = sum;
    }
    Plane4d temp[5];
    const nat nj = index[jm];
    for ( i = 0; i < 4; ++i )
    {
        const nat ni = index[i];
        temp[i] = summ * data[i][nj] < 0 ? -plane[ni] : plane[ni];
    }
    if ( max < 1e-14 )
        return intersection ( temp[0], temp[1], temp[2], temp[3] );
    {
        temp[4] = summ > 0 ? -plane[nj] : plane[nj];
    }
    SLU4<double> slu;
    slu.aa = temp[0].norm.x1 - temp[4].norm.x1;
    slu.ab = temp[0].norm.x2 - temp[4].norm.x2;
    slu.ac = temp[0].norm.x3 - temp[4].norm.x3;
    slu.ad = temp[0].norm.x4 - temp[4].norm.x4;
    slu.ae = temp[4].dist - temp[0].dist;
    slu.ba = temp[1].norm.x1 - temp[4].norm.x1;
    slu.bb = temp[1].norm.x2 - temp[4].norm.x2;
    slu.bc = temp[1].norm.x3 - temp[4].norm.x3;
    slu.bd = temp[1].norm.x4 - temp[4].norm.x4;
    slu.be = temp[4].dist - temp[1].dist;
    slu.ca = temp[2].norm.x1 - temp[4].norm.x1;
    slu.cb = temp[2].norm.x2 - temp[4].norm.x2;
    slu.cc = temp[2].norm.x3 - temp[4].norm.x3;
    slu.cd = temp[2].norm.x4 - temp[4].norm.x4;
    slu.ce = temp[4].dist - temp[2].dist;
    slu.da = temp[3].norm.x1 - temp[4].norm.x1;
    slu.db = temp[3].norm.x2 - temp[4].norm.x2;
    slu.dc = temp[3].norm.x3 - temp[4].norm.x3;
    slu.dd = temp[3].norm.x4 - temp[4].norm.x4;
    slu.de = temp[4].dist - temp[3].dist;
    res.isDef = slu.gauss ( res.x1, res.x2, res.x3, res.x4 );
    if ( ! res.isDef || nn == 5 )
        return res;
    double dist = temp[4] % res;
    if ( dist <= 0 )
        return res;
    const nat nRow = 5, nCol = 2*nRow;
    CmbArray2<double, nRow*nCol> mat ( nRow, nCol );
    for ( i = 0; i < nRow; ++i )
    {
        mat[i][0] = -1;
        mat[i][1] = temp[i].norm.x1;
        mat[i][2] = temp[i].norm.x2;
        mat[i][3] = temp[i].norm.x3;
        mat[i][4] = temp[i].norm.x4;
        for ( j = 0; j < nRow; ++j ) mat[i][j+nRow] = i == j ? 1 : 0;
    }
    index.resize ( nCol );
    sluGaussRow ( mat, nRow, nCol, index(), nRow, nRow );
    Double<5> arr[6];
    arr[0].init ( dist, res.x1, res.x2, res.x3, res.x4 );
    for ( i = 0; i < nRow; ++i )
    {
        double * d = & arr[i+1].d0;
        for ( j = 0; j < nRow; ++j ) d[index[j]] = mat[j][nRow+i];
    }
    // Нормализация векторов
    for ( i = 1; i < 6; ++i )
    {
        Double<5> & d = arr[i];
        const double c = temp[i-1].norm * Vector4d ( d.d1, d.d2, d.d3, d.d4 ) > 0 ? -1 : 1;
        d *= c / sqrt ( d * d );
    }
    // Поиск оптимума
    for ( nat k = 0; k < nn; ++k )
    {
        nat i, im = 0;
        double max = fabs ( plane[0] % res );
        for ( i = 1; i < nn; ++i )
        {
            const double t = fabs ( plane[i] % res );
            if ( max < t ) max = t, im = i;
        }
        if ( max <= ( 1 + 1e-12 ) * dist )
            return res;
        dist = max - dist;
        Plane4d pm = plane[im];
        if ( pm % res > 0 ) pm = - pm;
        Double<5> cor;
        cor.init ( 1, pm.norm.x1, pm.norm.x2, pm.norm.x3, pm.norm.x4 );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i < 6; ++i )
        {
            const Double<5> & v = arr[i];
            double t = cor * v;
            if ( t < 1e-12 ) continue;
            t = 1./ t;
            if ( ib == 0 )
            {
                max = v.d0 * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double u = v.d0 * t;
                if ( u < max ) max = u, ib = i, sm = t;
            }
        }
        if ( ib == 0 ) 
            break;
        const Double<5> & v = arr[ib];
        Double<5> & a0 = arr[0];
        a0 += v * ( dist * sm );
        res.x1 = a0.d1;
        res.x2 = a0.d2;
        res.x3 = a0.d3;
        res.x4 = a0.d4;
        dist = a0.d0;
        for ( i = 1; i < 6; ++i )
        {
            if ( i == ib ) continue;
            Double<5> & ai = arr[i];
            ai -= v * ( ( cor * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return Def<Vector4d>();
}

//**************************** 21.10.2026 *********************************//
//
//      Выпуклая оболочка множества четырёхмерных точек
//
//**************************** 21.10.2026 *********************************//

namespace
{

const nat NIL = nat ( -1 );

double det3 ( double a11, double a12, double a13,
              double a21, double a22, double a23,
              double a31, double a32, double a33 )
{
    return a11 * ( a22 * a33 - a23 * a32 )
         - a12 * ( a21 * a33 - a23 * a31 )
         + a13 * ( a21 * a32 - a22 * a31 );
}

// Вектор, ортогональный трём данным: его скалярное произведение с x равно det ( u, v, w, x )
Vector4d cross4 ( const Vector4d & u, const Vector4d & v, const Vector4d & w )
{
    return Vector4d
    (
        det3 ( u.x2, v.x2, w.x2, u.x3, v.x3, w.x3, u.x4, v.x4, w.x4 ),
       -det3 ( u.x1, v.x1, w.x1, u.x3, v.x3, w.x3, u.x4, v.x4, w.x4 ),
        det3 ( u.x1, v.x1, w.x1, u.x2, v.x2, w.x2, u.x4, v.x4, w.x4 ),
       -det3 ( u.x1, v.x1, w.x1, u.x2, v.x2, w.x2, u.x3, v.x3, w.x3 )
    );
}

// Гиперплоскость через четыре точки. Внутренняя точка лежит с отрицательной стороны
Plane4d makePlane ( const Vector4d & a, const Vector4d & b, const Vector4d & c, const Vector4d & d, const Vector4d & inner )
{
    Vector4d n = cross4 ( b - a, c - a, d - a );
    if ( n * ( inner - a ) > 0 ) n = - n;
    const double q = qmod ( n );
    if ( q > 0 ) n *= 1 / sqrt ( q );
    return Plane4d ( n, - ( n * a ) );
}

bool axes2 ( const Vector4d & u, const Vector4d & w, Vector4d & e1, Vector4d & e2 )
{
    e1 = u;
    e1.setNorm2();
    if ( ! e1 ) return false;
    e2 = w - e1 * ( w * e1 );
    e2.setNorm2();
    if ( ! e2 ) return false;
    return true;
}

DynArrRef<Vector4d> & hullLine ( CCArrRef<Vector4d> & point, DynArrRef<Vector4d> & vert, nat i0, const Vector4d & dir )
{
    double tmin = 0, tmax = 0;
    nat imn = i0, imx = i0;
    for ( nat i = 0; i < point.size(); ++i )
    {
        const double t = dir * ( point[i] - point[i0] );
        if ( t < tmin ) tmin = t, imn = i;
        if ( t > tmax ) tmax = t, imx = i;
    }
    if ( imn == imx )
    {
        vert.resize ( 1 );
        vert[0] = point[imn];
        return vert;
    }
    vert.resize ( 2 );
    vert[0] = point[imn];
    vert[1] = point[imx];
    return vert;
}

DynArrRef<Vector4d> & hullFlat2 ( CCArrRef<Vector4d> & point, DynArrRef<Vector4d> & vert,
    const Vector4d & origin, const Vector4d & e1, const Vector4d & e2 )
{
    const nat n = point.size();
    DynArray<Vector2d> q ( n );
    for ( nat i = 0; i < n; ++i )
    {
        const Vector4d x = point[i] - origin;
        q[i] = Vector2d ( x * e1, x * e2 );
    }
    Suite<nat> id;
    convexNlogN ( q, id );
    vert.resize ( id.size() );
    for ( nat i = 0; i < id.size(); ++i ) vert[i] = point[id[i]];
    return vert;
}

DynArrRef<Vector4d> & hullFlat3 ( CCArrRef<Vector4d> & point, DynArrRef<Vector4d> & vert,
    const Vector4d & origin, const Vector4d & e1, const Vector4d & e2, const Vector4d & e3 )
{
    const nat n = point.size();
    DynArray<Vector3d> q ( n );
    for ( nat i = 0; i < n; ++i )
    {
        const Vector4d x = point[i] - origin;
        q[i] = Vector3d ( x * e1, x * e2, x * e3 );
    }
    if ( n >= 4 )
    {
        nat nv, nf;
        DynArray<nat> iv ( n );
        DynArray<TrianFacet> tri ( 2 * n - 4 );
        if ( convexHull ( q, nv, iv, nf, tri ) )
        {
            vert.resize ( nv );
            for ( nat i = 0; i < nv; ++i ) vert[i] = point[iv[i]];
            return vert;
        }
    }
    return hullFlat2 ( point, vert, origin, e1, e2 );
}

struct Facet4
{
    nat vertex[4];
    nat nei[4];
    nat side[4];
    Plane4d plane;
    nat head;
    nat mark;
    bool alive;

    Facet4 () : head ( NIL ), mark ( 0 ), alive ( false )
    {
        vertex[0] = vertex[1] = vertex[2] = vertex[3] = 0;
        nei[0] = nei[1] = nei[2] = nei[3] = NIL;
        side[0] = side[1] = side[2] = side[3] = 0;
    }
};

struct Horizon4
{
    nat nei, side, v0, v1, v2;
};

struct Edge4
{
    nat a, b, facet, face;
};

void linkEdge ( Suite<Edge4> & edge, nat a, nat b, nat facetId, nat face, Suite<Facet4> & facet )
{
    if ( a > b ) _swap ( a, b );
    for ( nat i = 0; i < edge.size(); ++i )
    {
        if ( edge[i].a != a || edge[i].b != b ) continue;
        const nat f1 = edge[i].facet;
        const nat s1 = edge[i].face;
        facet[f1].nei[s1] = facetId;
        facet[f1].side[s1] = face;
        facet[facetId].nei[face] = f1;
        facet[facetId].side[face] = s1;
        edge.del ( i );
        return;
    }
    Edge4 & e = edge.inc();
    e.a = a;
    e.b = b;
    e.facet = facetId;
    e.face = face;
}

struct Work
{
    CArrRef<Vector4d> point;
    Suite<Facet4> facet;
    DynArray<nat> next;
    DynArray<bool> on;
    DynArray<bool> skip;
    DynArray<nat> seen;
    Suite<nat> hull;
    Vector4d inner;
    double tol;
    nat gen;
    nat seenId;

    Work ( CCArrRef<Vector4d> & p, double t );
    void pushOut ( nat f, nat p );
    void seed ( const nat s[5] );
    bool addPoint ( nat p, nat seedFacet );
    void run ();
};

Work::Work ( CCArrRef<Vector4d> & p, double t )
    : point ( p ), next ( p.size(), NIL ), on ( p.size(), false ), skip ( p.size(), false ),
      seen ( p.size(), nat(0) ), tol ( t ), gen ( 0 ), seenId ( 0 )
{}

void Work::pushOut ( nat f, nat p )
{
    next[p] = facet[f].head;
    facet[f].head = p;
}

void Work::seed ( const nat s[5] )
{
    nat loc[5][4];
    nat i, k;
    for ( i = 0; i < 5; ++i )
    {
        nat t = 0;
        for ( nat j = 0; j < 5; ++j ) if ( j != i ) loc[i][t++] = j;
    }
    inner = point[s[0]];
    for ( k = 1; k < 5; ++k ) inner += point[s[k]];
    inner *= 0.2;
    for ( i = 0; i < 5; ++i )
    {
        Facet4 & f = facet.inc();
        for ( k = 0; k < 4; ++k ) f.vertex[k] = s[loc[i][k]];
        f.plane = makePlane ( point[f.vertex[0]], point[f.vertex[1]], point[f.vertex[2]], point[f.vertex[3]], point[s[i]] );
        f.alive = true;
        f.head = NIL;
        f.mark = 0;
        for ( k = 0; k < 4; ++k )
        {
            const nat other = loc[i][k];
            f.nei[k] = other;
            nat ns = 0;
            for ( nat t = 0; t < 4; ++t ) if ( loc[other][t] == i ) ns = t;
            f.side[k] = ns;
        }
    }
    for ( k = 0; k < 5; ++k )
    {
        on[s[k]] = true;
        hull.add ( s[k] );
    }
    for ( i = 0; i < point.size(); ++i ) if ( ! on[i] )
    {
        nat best = NIL;
        double mx = tol;
        for ( nat f = 0; f < 5; ++f )
        {
            const double d = facet[f].plane % point[i];
            if ( d > mx ) mx = d, best = f;
        }
        if ( best != NIL ) pushOut ( best, i );
    }
}

bool Work::addPoint ( nat p, nat seedFacet )
{
    ++gen;
    Suite<nat> stack, vis;
    Suite<Horizon4> hor;
    facet[seedFacet].mark = gen;
    stack.add ( seedFacet );
    while ( stack.size() > 0 )
    {
        const nat f = stack.las();
        stack.dec();
        vis.add ( f );
        for ( nat k = 0; k < 4; ++k )
        {
            const nat g = facet[f].nei[k];
            if ( g == NIL || facet[g].mark == gen || ! facet[g].alive ) continue;
            if ( facet[g].plane % point[p] > tol )
            {
                facet[g].mark = gen;
                stack.add ( g );
                continue;
            }
            nat vv[3];
            nat t = 0;
            for ( nat j = 0; j < 4; ++j ) if ( j != k ) vv[t++] = facet[f].vertex[j];
            const nat ns = facet[f].side[k];
            Horizon4 & h = hor.inc();
            h.nei = g;
            h.side = ns;
            h.v0 = vv[0];
            h.v1 = vv[1];
            h.v2 = vv[2];
        }
    }
    if ( hor.size() == 0 ) return false;

    ++seenId;
    Suite<nat> cand;
    nat i;
    for ( i = 0; i < vis.size(); ++i )
    {
        const nat f = vis[i];
        for ( nat s = facet[f].head; s != NIL; s = next[s] )
        {
            if ( s == p || on[s] || seen[s] == seenId ) continue;
            seen[s] = seenId;
            cand.add ( s );
        }
        facet[f].head = NIL;
        facet[f].alive = false;
    }

    const nat nh = hor.size();
    const nat base = facet.size();
    for ( i = 0; i < nh; ++i )
    {
        const nat nb = hor[i].nei;
        const nat ns = hor[i].side;
        const nat v0 = hor[i].v0;
        const nat v1 = hor[i].v1;
        const nat v2 = hor[i].v2;
        Facet4 & fn = facet.inc();
        fn.vertex[0] = v0;
        fn.vertex[1] = v1;
        fn.vertex[2] = v2;
        fn.vertex[3] = p;
        fn.plane = makePlane ( point[v0], point[v1], point[v2], point[p], inner );
        fn.alive = true;
        fn.head = NIL;
        fn.mark = 0;
        fn.nei[0] = fn.nei[1] = fn.nei[2] = NIL;
        fn.nei[3] = nb;
        fn.side[3] = ns;
        facet[nb].nei[ns] = base + i;
        facet[nb].side[ns] = 3;
    }

    Suite<Edge4> edge;
    for ( i = 0; i < nh; ++i )
    {
        const nat id = base + i;
        linkEdge ( edge, facet[id].vertex[1], facet[id].vertex[2], id, 0, facet );
        linkEdge ( edge, facet[id].vertex[0], facet[id].vertex[2], id, 1, facet );
        linkEdge ( edge, facet[id].vertex[0], facet[id].vertex[1], id, 2, facet );
    }

    for ( i = 0; i < cand.size(); ++i )
    {
        const nat c = cand[i];
        if ( c == p || on[c] ) continue;
        nat best = NIL;
        double mx = tol;
        for ( nat f = 0; f < facet.size(); ++f ) if ( facet[f].alive )
        {
            const double d = facet[f].plane % point[c];
            if ( d > mx ) mx = d, best = f;
        }
        if ( best != NIL ) pushOut ( best, c );
    }
    return true;
}

void Work::run ()
{
    while ( true )
    {
        nat bestF = NIL, bestP = NIL;
        double mx = tol;
        nat f;
        for ( f = 0; f < facet.size(); ++f ) if ( facet[f].alive )
        {
            for ( nat s = facet[f].head; s != NIL; s = next[s] )
            {
                if ( on[s] || skip[s] ) continue;
                const double d = facet[f].plane % point[s];
                if ( d > mx ) mx = d, bestF = f, bestP = s;
            }
        }
        if ( bestP == NIL )
        {
            for ( nat p = 0; p < point.size(); ++p ) if ( ! on[p] && ! skip[p] )
            {
                for ( f = 0; f < facet.size(); ++f ) if ( facet[f].alive )
                {
                    const double d = facet[f].plane % point[p];
                    if ( d > mx ) mx = d, bestF = f, bestP = p;
                }
            }
        }
        if ( bestP == NIL ) break;
        if ( addPoint ( bestP, bestF ) )
        {
            on[bestP] = true;
            hull.add ( bestP );
        }
        else skip[bestP] = true;
    }
}

}

DynArrRef<Vector4d> & convexHull ( CCArrRef<Vector4d> & point, DynArrRef<Vector4d> & vert )
{
    const nat n = point.size();
    if ( n == 0 ) return vert.resize();

    nat i, i0 = 0;
    double mq = qmod ( point[0] );
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[i] );
        if ( q > mq ) mq = q, i0 = i;
    }
    const double tol = 1e-9 * sqrt ( mq );

    nat i1 = NIL;
    double best = tol * tol;
    for ( i = 0; i < n; ++i ) if ( i != i0 )
    {
        const double q = qmod ( point[i] - point[i0] );
        if ( q > best ) best = q, i1 = i;
    }
    if ( i1 == NIL )
    {
        vert.resize ( 1 );
        vert[0] = point[i0];
        return vert;
    }

    const Vector4d dir = point[i1] - point[i0];
    const double dd = dir * dir;
    nat i2 = NIL;
    best = tol * tol * dd;
    for ( i = 0; i < n; ++i ) if ( i != i0 && i != i1 )
    {
        const Vector4d x = point[i] - point[i0];
        const double q = dd * qmod ( x ) - _pow2 ( dir * x );
        if ( q > best ) best = q, i2 = i;
    }
    Vector4d e1, e2;
    if ( i2 == NIL || ! axes2 ( dir, point[i2] - point[i0], e1, e2 ) )
        return hullLine ( point, vert, i0, dir );

    const Vector4d u = dir;
    const Vector4d w = point[i2] - point[i0];
    const double uu = u * u;
    const double uw = u * w;
    const double ww = w * w;
    const double g2 = uu * ww - uw * uw;
    nat i3 = NIL;
    best = tol * tol;
    for ( i = 0; i < n; ++i ) if ( i != i0 && i != i1 && i != i2 )
    {
        const Vector4d x = point[i] - point[i0];
        const double ux = u * x;
        const double wx = w * x;
        const double xx = qmod ( x );
        double det = xx * g2 - ux * ( ux * ww - wx * uw ) + wx * ( ux * uw - wx * uu );
        if ( det < 0 ) det = 0;
        const double q = g2 > 0 ? det / g2 : 0;
        if ( q > best ) best = q, i3 = i;
    }
    if ( i3 == NIL ) return hullFlat2 ( point, vert, point[i0], e1, e2 );

    Vector4d e3 = point[i3] - point[i0];
    e3 -= e1 * ( e3 * e1 );
    e3 -= e2 * ( e3 * e2 );
    e3.setNorm2();
    if ( ! e3 ) return hullFlat2 ( point, vert, point[i0], e1, e2 );

    Vector4d n3 = cross4 ( e1, e2, e3 );
    n3.setNorm2();
    if ( ! n3 ) return hullFlat3 ( point, vert, point[i0], e1, e2, e3 );

    nat i4 = NIL;
    best = tol;
    for ( i = 0; i < n; ++i )
    {
        if ( i == i0 || i == i1 || i == i2 || i == i3 ) continue;
        const double d = fabs ( n3 * ( point[i] - point[i0] ) );
        if ( d > best ) best = d, i4 = i;
    }
    if ( i4 == NIL ) return hullFlat3 ( point, vert, point[i0], e1, e2, e3 );

    const nat simp[5] = { i0, i1, i2, i3, i4 };
    Work work ( point, tol );
    work.seed ( simp );
    work.run ();
    vert.resize ( work.hull.size() );
    for ( i = 0; i < work.hull.size(); ++i ) vert[i] = point[work.hull[i]];
    return vert;
}
