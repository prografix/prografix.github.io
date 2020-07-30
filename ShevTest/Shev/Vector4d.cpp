
#include <math.h>
#include "mathem.h"
#include "LinAlg.h"
#include "Vector4d.h"
#include "DoubleN.h"

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