
#include "math.h"

#include "../Shev/rand.h"
#include "../Shev/tune.h"
#include "../Shev/LinAlg.h"
#include "../Shev/mathem.h"
#include "../Shev/Vector4d.h"
#include "../Shev/ShevList.h"
#include "../Shev/ShevArray.h"
#include "../Shev/DoubleN.h"
#include "../Shev/lists.h"
#include "../Shev/heap.h"
#include "../draw.h"

#include "display.h"

double timeInSec();

class TetraFacet
{
public:
    Vector4d vertex[4];
    /*Plane4d plane;

    bool initPlane ()
    {
        const Vector4d & v0 = vertex[0];
        FixArray2<double, 3, 4> data;
        FixArray<nat, 4> index;
        for ( nat i = 0; i < 3; ++i )
        {
            const Vector4d & v = vertex[i+1];
            ArrRef<double> & r = data[i];
            r[0] = v.x1 - v0.x1;
            r[1] = v.x2 - v0.x2;
            r[2] = v.x3 - v0.x3;
            r[3] = v.x4 - v0.x4;
        }
        if ( ! sluGaussRow ( data, 3, 4, index(), 3, 4 ) ) return false;
        const nat ii = index[3];
        double sum = 1;
        for ( nat j = 0; j < 3; ++j ) sum += _pow2 ( data[j][ii] );
        sum = 1 / sqrt ( sum );
        double * p = & plane.norm.x1;
        for ( nat k = 0; k < 3; ++k ) p[index[k]] = data[k][ii] * sum;
        p[index[3]] = - sum;
        const double d = plane.norm * v0;
        if ( d < 0 )
        {
            plane.norm = - plane.norm;
            plane.dist = d;
        }
        else
            plane.dist = - d;
        return true;
    }*/
};

double volume ( const TetraFacet & t )
{
    const Vector4d a = t.vertex[1] - t.vertex[0];
    const Vector4d b = t.vertex[2] - t.vertex[0];
    const Vector4d c = t.vertex[3] - t.vertex[0];
    Matrix3<double> m;
    m.aa = a * a; m.ab = a * b; m.ac = a * c;
    m.ba = m.ab;  m.bb = b * b; m.bc = b * c;
    m.ca = m.ac;  m.cb = m.bc;  m.cc = c * c;
    return sqrt ( fabs ( m.determinant() ) ) / 6;
}

double volume ( CCArrRef<TetraFacet> & arr )
{
    double sum = 0;
    for ( nat k = 0; k < arr.size(); ++k ) sum += volume ( arr[k] );
    return sum;
}

namespace
{

bool convexHull ( CArrRef<Vector4d> point, nat & nv, ArrRef<nat> & iv, nat & nf, ArrRef<TetraFacet> & facet )
{
    const nat n = point.size();
    if ( n < 5 ) return false;
    nat i;
    for ( i = 0; i < n; ++i ) iv[i] = i;
// Находим первую начальную точку
    nat im = 0;
    double max = qmod ( point[0] );
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[i] );
        if ( max < q ) max = q, im = i;
    }
    const double eps = 1e-12 * sqrt ( max );
    _swap ( iv[im], iv[0] );
// Находим вторую начальную точку
    im = 1;
    max = 0.;
    for ( i = 1; i < n; ++i )
    {
        const double q = qmod ( point[iv[i]] - point[iv[0]] );
        if ( max < q ) max = q, im = i;
    }
    if ( !max ) return false;
    _swap ( iv[im], iv[1] );
// Находим третью начальную точку
    im = 2;
    max = 0.;
    const Vector4d dir = point[iv[1]] - point[iv[0]];
    const double dd = dir * dir;
    for ( i = 2; i < n; ++i )
    {
        const Vector4d v = point[iv[i]] - point[iv[0]];
        const double q = dd * qmod ( v ) - _pow2 ( dir * v );
        if ( max < q ) max = q, im = i;
    }
    if ( max < eps * eps ) return false;
    _swap ( iv[im], iv[2] );
// Находим четвёртую начальную точку
/*    Vector3d norm = dir % ( point[iv[2]] - point[iv[0]] );
    if ( !norm ) return false;
    norm.setNorm2();
    const Plane3d plane ( norm, - ( norm * point[iv[0]] ) );
    im = 3;
    max = 0.;
    for ( i = 3; i < n; ++i )
    {
        const double q = fabs ( plane % point[iv[i]] );
        if ( max < q ) max = q, im = i;
    }
    if ( max < eps ) return false;
    _swap ( iv[im], iv[3] );
    if ( plane % point[iv[3]] > 0 ) _swap ( iv[1], iv[2] );*/
    return false;
}

void test ( const TetraFacet & t )
{
    nat i;
    double max = 0, max2 = 0;
    for ( i = 0; i < 4; ++i )
    {
        double /*d = fabs ( t.plane % t.vertex[i] );
        if ( max < d ) max = d;*/
        d = norm2 ( t.vertex[i] ) - 1;
        if ( max2 < d ) max2 = d;
    }
    //if ( max > 1e-12 )
    //    display << "plane =" << max << NL;
    if ( max2 > 1e-12 )
        display << "norm - 1 =" << max2 << NL;
    double min = 9;
    max = 0;
    for ( i = 0; i < 3; ++i )
    {
        for ( nat j = i+1; j < 4; ++j )
        {
            double d = norm2 ( t.vertex[j] - t.vertex[i] );
            if ( max < d ) max = d;
            if ( min > d ) min = d;
        }
    }
    display << "edge" << max / min << NL;
}

void test ( CCArrRef<TetraFacet> & arr )
{
    if ( ! arr.size() ) return;
    double sum = 0, minv = 99, maxv = 0;
    for ( nat k = 0; k < arr.size(); ++k )
    {
        const TetraFacet & t = arr[k];
        double max = 0;
        double min = 9;
        for ( nat i = 0; i < 3; ++i )
        {
            for ( nat j = i+1; j < 4; ++j )
            {
                double d = norm2 ( t.vertex[j] - t.vertex[i] );
                if ( max < d ) max = d;
                if ( min > d ) min = d;
            }
        }
        sum += max / min;
        const double v = volume ( t );
        if ( maxv < v ) maxv = v;
        if ( minv > v ) minv = v;
    }
    display << "edge" << arr.size() << sum / arr.size() << NL;
    display << "volume" << maxv / minv << NL;
//    display << volume ( arr ) << NL;
}

void test2 ( CCArrRef<TetraFacet> & arr )
{
    if ( ! arr.size() ) return;
    static nat c = 0;
    for ( nat k = 0; k < arr.size(); ++k )
    {
        const TetraFacet & t = arr[k];
        if ( k == c ) 
        for ( nat i = 0; i < 4; ++i )
        {
            const Vector4d & v = t.vertex[i];
            Spin3d spin ( v.x1, v.x2, v.x3, v.x4 );
            Ortho3d ortho ( spin );
            drawPoint ( ortho ( Vector3d ( 1,0,0 ) ), 0, 1, 1 );
            drawPoint ( ortho ( Vector3d ( 0,1,0 ) ), 1, 0, 0 );
            drawPoint ( ortho ( Vector3d ( 0,0,1 ) ), 0, 1, 0 );
        }
    }
    if ( ++c == arr.size() ) c = 0;
    display << arr.size() << NL;
}

Vector4d getVector4d ( double a, double b, double c )
{
    if ( a < 0 ) a = 0; else
    if ( a > 1 ) a = 1;
    const double s = sqrt (   a   );
    const double q = sqrt ( 1 - a );
    b *= M_PI;
    c *= M_PI;
    return Vector4d ( s*sin(b+c), s*cos(b+c), q*cos(b-c), q*sin(b-c) );
}

void divide ( const TetraFacet & t, Suite<TetraFacet> & arr )
{
    Suite<Vector4d> vertex ( 6 );
    for ( nat i = 0; i < 3; ++i )
    {
        for ( nat j = i+1; j < 4; ++j )
        {
            vertex.inc() = ( t.vertex[j] + t.vertex[i] ).setNorm2();
        }
    }
    TetraFacet & t0 = arr.inc();
    t0.vertex[0] = t.vertex[0];
    t0.vertex[1] = vertex[0];
    t0.vertex[2] = vertex[1];
    t0.vertex[3] = vertex[2];
    TetraFacet & t1 = arr.inc();
    t1.vertex[0] = t.vertex[1];
    t1.vertex[1] = vertex[4];
    t1.vertex[2] = vertex[3];
    t1.vertex[3] = vertex[0];
    TetraFacet & t2 = arr.inc();
    t2.vertex[0] = t.vertex[2];
    t2.vertex[1] = vertex[1];
    t2.vertex[2] = vertex[3];
    t2.vertex[3] = vertex[5];
    TetraFacet & t3 = arr.inc();
    t3.vertex[0] = t.vertex[3];
    t3.vertex[1] = vertex[5];
    t3.vertex[2] = vertex[4];
    t3.vertex[3] = vertex[2];
//return;
    const double q0 = qmod ( vertex[0] - vertex[5] );
    const double q1 = qmod ( vertex[1] - vertex[4] );
    const double q2 = qmod ( vertex[2] - vertex[3] );
    if ( q0 <= q1 && q0 <= q2 )
    {
        TetraFacet & t0 = arr.inc();
        t0.vertex[0] = vertex[0];
        t0.vertex[1] = vertex[5];
        t0.vertex[2] = vertex[1];
        t0.vertex[3] = vertex[2];
        TetraFacet & t1 = arr.inc();
        t1.vertex[0] = vertex[0];
        t1.vertex[1] = vertex[5];
        t1.vertex[2] = vertex[2];
        t1.vertex[3] = vertex[4];
        TetraFacet & t2 = arr.inc();
        t2.vertex[0] = vertex[0];
        t2.vertex[1] = vertex[5];
        t2.vertex[2] = vertex[4];
        t2.vertex[3] = vertex[3];
        TetraFacet & t3 = arr.inc();
        t3.vertex[0] = vertex[0];
        t3.vertex[1] = vertex[5];
        t3.vertex[2] = vertex[3];
        t3.vertex[3] = vertex[1];
    }
    else
    if ( q1 <= q2 )
    {
        TetraFacet & t0 = arr.inc();
        t0.vertex[0] = vertex[1];
        t0.vertex[1] = vertex[4];
        t0.vertex[2] = vertex[0];
        t0.vertex[3] = vertex[2];
        TetraFacet & t1 = arr.inc();
        t1.vertex[0] = vertex[1];
        t1.vertex[1] = vertex[4];
        t1.vertex[2] = vertex[2];
        t1.vertex[3] = vertex[5];
        TetraFacet & t2 = arr.inc();
        t2.vertex[0] = vertex[1];
        t2.vertex[1] = vertex[4];
        t2.vertex[2] = vertex[5];
        t2.vertex[3] = vertex[3];
        TetraFacet & t3 = arr.inc();
        t3.vertex[0] = vertex[1];
        t3.vertex[1] = vertex[4];
        t3.vertex[2] = vertex[3];
        t3.vertex[3] = vertex[0];
    }
    else
    {
        TetraFacet & t0 = arr.inc();
        t0.vertex[0] = vertex[2];
        t0.vertex[1] = vertex[3];
        t0.vertex[2] = vertex[0];
        t0.vertex[3] = vertex[1];
        TetraFacet & t1 = arr.inc();
        t1.vertex[0] = vertex[2];
        t1.vertex[1] = vertex[3];
        t1.vertex[2] = vertex[1];
        t1.vertex[3] = vertex[5];
        TetraFacet & t2 = arr.inc();
        t2.vertex[0] = vertex[2];
        t2.vertex[1] = vertex[3];
        t2.vertex[2] = vertex[5];
        t2.vertex[3] = vertex[4];
        TetraFacet & t3 = arr.inc();
        t3.vertex[0] = vertex[2];
        t3.vertex[1] = vertex[3];
        t3.vertex[2] = vertex[4];
        t3.vertex[3] = vertex[0];
    }
}

void convexHull_test()
{
    nat i;
    const nat n = 5;
    Vector4d vert[n];
    /*QRand q5(5);
    double b = 0;
    for ( nat k = 0; k < n; ++k )
    {
        const double a = ( 0.5 + k ) / n;
        if ( ( b += 0.61803398874989484820 ) > 1 ) b -= 1;
        const double c = q5();
        vert[k] = getVector4d ( a, b, c );
    }*/
    draw ( Sphere3d ( 1, null3d ), 0.3f, 0.3f, 0.3f, 1, VM_WIRE );
    const double a = 0.25;
    const double b = a * sqrt(5.);
    vert[0] = Vector4d ( a, b, b, b );
    vert[1] = Vector4d ( a, b,-b,-b );
    vert[2] = Vector4d ( a,-b, b,-b );
    vert[3] = Vector4d ( a,-b,-b, b );
    vert[4] = Vector4d (-1, 0, 0, 0 );
    Suite<TetraFacet> facet1 ( n, n );
    for ( i = 0; i < n; ++i )
    {
        TetraFacet & t = facet1[i];
        for ( nat k = 0, j = 0; k < n; ++k )
        {
            if ( k == i ) continue;
            t.vertex[j++] = vert[k];
        }
    }
    //test2 ( facet1 );
    Suite<TetraFacet> facet2 ( 8*n );
    for ( i = 0; i < facet1.size(); ++i )
    {
        divide ( facet1[i], facet2 );
    }
    facet1.swap ( facet2 );
    facet2.resize(0);
    //test2 ( facet1 );
    for ( i = 0; i < facet1.size(); ++i )
    {
        divide ( facet1[i], facet2 );
    }
    facet1.swap ( facet2 );
    facet2.resize(0);
    //test2 ( facet1 );
    for ( i = 0; i < facet1.size(); ++i )
    {
        divide ( facet1[i], facet2 );
    }
    facet1.swap ( facet2 );
    facet2.resize(0);
    //test2 ( facet1 );
    for ( i = 0; i < facet1.size(); ++i )
    {
        divide ( facet1[i], facet2 );
    }
    facet1.swap ( facet2 );
    facet2.resize(0);
    test2 ( facet1 );
display << "end" << NL;
}

void intersection_test ()
{
    static PRand rand1;
    static PRandVector4d rand4;
    Plane4d plane1, plane2, plane3, plane4;
    plane1.norm = rand4();
    plane1.dist = 2*rand1() - 1;
    plane2.norm = rand4();
    plane2.dist = 2*rand1() - 1;
    plane3.norm = rand4();
    plane3.dist = 2*rand1() - 1;
    plane4.norm = rand4();
    plane4.dist = 2*rand1() - 1;
    Def<Vector4d> p = intersection ( plane1, plane2, plane3, plane4 );
    if ( p.isDef )
    {
        display << plane1 % p << plane2 % p << plane3 % p << plane4 % p << NL;
    }
}

inline double round ( double a ) { return fabs ( a ) < 1e-15 ? 0. : a; }

Def<Vector4d> getNearPointU1 ( CCArrRef<Plane4d> & plane )
{
    Def<Vector4d> res;
    const nat nn = plane.size();
    if ( nn < 4 ) return res;
    nat i1 = 0, i2 = 1, i3 = 2, i4 = 3, i5 = 4;
    Def<Vector4d> p5 = intersection ( plane[i1], plane[i2], plane[i3], plane[i4] );
    if ( ! p5.isDef || nn == 4 ) return p5;
    if ( nn > 5 )
    {
        double max = fabs ( plane[i5] % p5 );
        for ( nat i = 5; i < nn; ++i )
        {
            const double t = fabs ( plane[i] % res );
            if ( max < t ) max = t, i5 = i;
        }
    }
    Def<Vector4d> p1 = intersection ( plane[i5], plane[i2], plane[i3], plane[i4] );
    if ( ! p1.isDef ) return p1;
    Def<Vector4d> p2 = intersection ( plane[i1], plane[i5], plane[i3], plane[i4] );
    if ( ! p2.isDef ) return p2;
    Def<Vector4d> p3 = intersection ( plane[i1], plane[i2], plane[i5], plane[i4] );
    if ( ! p3.isDef ) return p3;
    Def<Vector4d> p4 = intersection ( plane[i1], plane[i2], plane[i3], plane[i5] );
    if ( ! p4.isDef ) return p4;
    const Plane4d plane1 = plane[i1] % p1 > 0 ? plane[i1] : - plane[i1];
    const Plane4d plane2 = plane[i2] % p2 > 0 ? plane[i2] : - plane[i2];
    const Plane4d plane3 = plane[i3] % p3 > 0 ? plane[i3] : - plane[i3];
    const Plane4d plane4 = plane[i4] % p4 > 0 ? plane[i4] : - plane[i4];
    const Plane4d plane5 = plane[i5] % p5 > 0 ? plane[i5] : - plane[i5];
    SLU4<double> slu;
    slu.aa = plane1.norm.x1 - plane5.norm.x1;
    slu.ab = plane1.norm.x2 - plane5.norm.x2;
    slu.ac = plane1.norm.x3 - plane5.norm.x3;
    slu.ad = plane1.norm.x4 - plane5.norm.x4;
    slu.ae = plane5.dist - plane1.dist;
    slu.ba = plane2.norm.x1 - plane5.norm.x1;
    slu.bb = plane2.norm.x2 - plane5.norm.x2;
    slu.bc = plane2.norm.x3 - plane5.norm.x3;
    slu.bd = plane2.norm.x4 - plane5.norm.x4;
    slu.be = plane5.dist - plane2.dist;
    slu.ca = plane3.norm.x1 - plane5.norm.x1;
    slu.cb = plane3.norm.x2 - plane5.norm.x2;
    slu.cc = plane3.norm.x3 - plane5.norm.x3;
    slu.cd = plane3.norm.x4 - plane5.norm.x4;
    slu.ce = plane5.dist - plane3.dist;
    slu.da = plane4.norm.x1 - plane5.norm.x1;
    slu.db = plane4.norm.x2 - plane5.norm.x2;
    slu.dc = plane4.norm.x3 - plane5.norm.x3;
    slu.dd = plane4.norm.x4 - plane5.norm.x4;
    slu.de = plane5.dist - plane4.dist;
    res.isDef = slu.gauss ( res.x1, res.x2, res.x3, res.x4 );
    if ( ! res.isDef || nn == 5 )
        return res;
    double dist = plane5 % res;
    if ( dist <= 0 )
        return res;
    Double<5> arr[6];
    arr[0].d0 = arr[1].d0 = arr[2].d0 = arr[3].d0 = arr[4].d0 = arr[5].d0 = dist;
    arr[0].d1 = res.x1;
    arr[0].d2 = res.x2;
    arr[0].d3 = res.x3;
    arr[0].d4 = res.x4;
    arr[1].d1 = res.x1 - p1.x1;
    arr[1].d2 = res.x2 - p1.x2;
    arr[1].d3 = res.x3 - p1.x3;
    arr[1].d4 = res.x4 - p1.x4;
    arr[2].d1 = res.x1 - p2.x1;
    arr[2].d2 = res.x2 - p2.x2;
    arr[2].d3 = res.x3 - p2.x3;
    arr[2].d4 = res.x4 - p2.x4;
    arr[3].d1 = res.x1 - p3.x1;
    arr[3].d2 = res.x2 - p3.x2;
    arr[3].d3 = res.x3 - p3.x3;
    arr[3].d4 = res.x4 - p3.x4;
    arr[4].d1 = res.x1 - p4.x1;
    arr[4].d2 = res.x2 - p4.x2;
    arr[4].d3 = res.x3 - p4.x3;
    arr[4].d4 = res.x4 - p4.x4;
    arr[5].d1 = res.x1 - p5.x1;
    arr[5].d2 = res.x2 - p5.x2;
    arr[5].d3 = res.x3 - p5.x3;
    arr[5].d4 = res.x4 - p5.x4;
    // Нормализация векторов
    for ( nat i = 1; i < 6; ++i )
    {
        Double<5> & d = arr[i];
        d *= 1 / sqrt ( d * d );
    }
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
/*
Def<Vector4d> getNearPointU2 ( CCArrRef<Plane4d> & plane )
{
    Def<Vector4d> res;
    const nat nn = plane.size();
    if ( nn < 4 ) return res;
    if ( nn == 4 ) return intersection ( plane[0], plane[1], plane[2], plane[3] );
    // Найдём 4 независимые гиперплоскости
    CmbArray2<double, 36> data ( 4, nn );
    CmbArray<nat, 9> col ( nn );
    nat i;
    for ( i = 0; i < nn; ++i )
    {
        const Plane4d & p = plane[i];
        data[0][i] = p.norm.x1;
        data[1][i] = p.norm.x2;
        data[2][i] = p.norm.x3;
        data[3][i] = p.norm.x4;
    }
    if ( ! slu_gauss ( data, nn, col ) ) return res;
    // Вычислим пересечение найденных гиперплоскостей
    Plane4d temp[5];
    for ( i = 0; i < 4; ++i ) temp[i] = plane[col[i]];
    res = intersection ( temp[0], temp[1], temp[2], temp[3] );
    if ( ! res.isDef ) return res;
    // Найдём самую удалённую гиперплоскость от пересечения
    nat im = 0;
    double dist = plane[col[0]] % res;
    double max = fabs ( dist );
    for ( i = 1; i < nn; ++i )
    {
        const double d = plane[col[i]] % res;
        const double t = fabs ( d );
        if ( max < t ) max = t, dist = d, im = i;
    }
    if ( im < 4 ) return res;
    // Развернём гиперплоскости нормалью во внутрь области
    const nat ni = col[im];
    temp[4] = dist < 0 ?  - plane[ni] : plane[ni];
    for ( i = 0; i < 4; ++i )
    {
        if ( temp[i].norm * temp[4].norm > 0 ) temp[i] = - temp[i]; // это неправильно
    }
    // Найдём самую удалённую точку от 5 гиперплоскостей
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
    dist = temp[4] % res;
    if ( dist <= 0 )
        return res;
    // Построим конус ограничений в 5-мерном пространстве
    Double<5> arr[6];
    arr[0].init ( dist, res.x1, res.x2, res.x3, res.x4 );
    arr[1] = vec_pro5 ( temp[1].norm, temp[2].norm, temp[3].norm, temp[4].norm );
    arr[2] = vec_pro5 ( temp[0].norm, temp[2].norm, temp[3].norm, temp[4].norm );
    arr[3] = vec_pro5 ( temp[0].norm, temp[1].norm, temp[3].norm, temp[4].norm );
    arr[4] = vec_pro5 ( temp[0].norm, temp[1].norm, temp[2].norm, temp[4].norm );
    arr[5] = vec_pro5 ( temp[0].norm, temp[1].norm, temp[2].norm, temp[3].norm );
    // Нормализация образующих векторов
    for ( i = 1; i < 6; ++i )
    {
        Double<5> & d = arr[i];
        const double c = temp[i-1].norm * Vector4d ( d.d1, d.d2, d.d3, d.d4 ) > 0 ? -1 : 1;
        d *= c / sqrt ( d * d );
    }
    // Поиск оптимума
    for ( nat k = 0; k < nn; ++k )
    {
        im = 0;
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
*/
void getNearPointU_test()
{/*
    Double<5> p0, p1, p2, p3, p4;
    p0.init ( 1, 0, 0, 0, 0 );
    p1.init ( 0, 1, 0, 0, 0 );
    p2.init ( 0, 0, 1, 0, 0 );
    p3.init ( 0, 0, 0, 1, 0 );
    p4.init ( 0, 0, 0, 0, 1 );
    Double<5> r0 = vec_pro ( p1, p2, p3, p4 );
    Double<5> r1 = vec_pro ( p2, p3, p4, p0 );
    Double<5> r2 = vec_pro ( p3, p4, p0, p1 );
    Double<5> r3 = vec_pro ( p4, p0, p1, p2 );
    Double<5> r4 = vec_pro ( p0, p1, p2, p3 );
    display << p0 * r0 << p1 * r1 << p2 * r2 << p3 * r3 << p4 * r4 << NL;
/*/
	nat i;
    static PRand rand1;
    static PRandVector4d rand4;
    FixArray<Plane4d, 9> plane;
    for ( i = 0; i < plane.size(); ++i )
    {
        plane[i].norm = rand4() * ( rand1() + 1 );
        plane[i].dist = 2*rand1() - 1;
    }
    double t0 = timeInSec();
    Def<Vector4d> p = getNearPointU1 ( plane );
    double t1 = timeInSec();
    Def<Vector4d> p2 = getNearPointU ( plane );
    double t2 = timeInSec();
    if ( p.isDef && p2.isDef )
    {
        for ( i = 0; i < plane.size(); ++i )
            display << fabs ( plane[i] % p );
            display << NL;
        for ( i = 0; i < plane.size(); ++i )
            display << fabs ( plane[i] % p2 );
            display << NL;
    }
    display << (t2-t1)/(t1-t0) << NL;
    display << "end" << NL;
}

Conform3d maxPointsInConvexPolyhedron1 ( CCArrRef<Vector3d> & inner, CCArrRef<Plane3d> & outer, const Vector4d & s )
{
    const nat N = 7;
    Double<N> con[N+1];
    Double<N> func;
    func.d0 = 
    func.d1 = 
    func.d2 = 0;
    func.d3 = s.x1;
    func.d4 = s.x2;
    func.d5 = s.x3;
    func.d6 = s.x4;
    const Vector3d axt (  s.x1, -s.x4,  s.x3 );
    const Vector3d ayt (  s.x4,  s.x1, -s.x1 );
    const Vector3d azt ( -s.x3,  s.x1,  s.x1 );
    const Vector3d axx (  s.x1,  s.x3,  s.x4 );
    const Vector3d ayx (  s.x3, -s.x1, -s.x1 );
    const Vector3d azx (  s.x4,  s.x1, -s.x1 );
    const Vector3d axy ( -s.x3,  s.x1,  s.x1 );
    const Vector3d ayy (  s.x1,  s.x3,  s.x4 );
    const Vector3d azy ( -s.x1,  s.x4, -s.x3 );
    const Vector3d axz ( -s.x4, -s.x1,  s.x1 );
    const Vector3d ayz (  s.x1, -s.x4,  s.x3 );
    const Vector3d azz (  s.x1,  s.x3,  s.x4 );
    Conform3d conf ( 0 );
    Double<N> & res = con[0];
    for ( nat k = 0; k < 2; ++k )
    {
        Double<N> b;
        b.fill(1);
        const Vector3d vx = axt * b.d3 + axx * b.d4 + axy * b.d5 + axz * b.d6;
        const Vector3d vy = ayt * b.d3 + ayx * b.d4 + ayy * b.d5 + ayz * b.d6;
        const Vector3d vz = azt * b.d3 + azx * b.d4 + azy * b.d5 + azz * b.d6;
        nat i, im = 0, jm = 0;
        double max = 0;
        for ( i = 0; i < inner.size(); ++i )
        {
            const Vector3d & v = inner[i];
            const Vector3d u ( vx * v + b.d0, vy * v + b.d1, vz * v + b.d2 );
            for ( nat j = 0; j < outer.size(); ++j )
            {
                const double t = outer[j] % u;
                if ( max < t ) max = t, im = i, jm = j;
            }
        }
        const double dist = max;
        const Vector3d & u = inner[im];
        const Plane3d & p = outer[jm];
        Double<N> norm;
        norm.d0 = p.norm.x;
        norm.d1 = p.norm.y;
        norm.d2 = p.norm.z;
        norm.d3 = Vector3d ( axt * u, ayt * u, azt * u ) * p.norm;
        norm.d4 = Vector3d ( axx * u, ayx * u, azx * u ) * p.norm;
        norm.d5 = Vector3d ( axy * u, ayy * u, azy * u ) * p.norm;
        norm.d6 = Vector3d ( axz * u, ayz * u, azz * u ) * p.norm;
        const double lvl = -1e-8 * sqrt ( norm * norm );
        nat ib = 0;
        double sm = 0; // для оптимизатора
        for ( i = 1; i <= N; ++i )
        {
            const Double<N> & v = con[i];
            double t = norm * v;
            if ( t > lvl ) continue;
            t = -1./ t;
            if ( !ib )
            {
                max = ( func * v ) * t;
                ib = i;
                sm = t;
            }
            else
            {
                const double s = ( func * v ) * t;
                if ( max < s ) max = s, ib = i, sm = t;
            }
        }
        if ( !ib )
        {
            return conf;
        }
        const Double<N> & v = con[ib];
        res += v * ( dist * sm );
        for ( i = 1; i <= N; ++i )
        {
            if ( i == ib ) continue;
            Double<N> & ai = con[i];
            ai += v * ( ( norm * ai ) * sm );
            ai *= ( 1./ sqrt ( ai * ai ) );
        }
    }
    return conf;
}


} // namespace

void func4d_test ()
{
    drawNewList2d();
//    intersection_test();
//    getNearPointU_test();
    convexHull_test();
    endNewList ();
}