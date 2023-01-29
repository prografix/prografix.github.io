
#include "trian3d.h"

//***************************** 06.07.2003 ********************************//
//
//   Триангуляция замкнутой ломаной в пространстве. Минимум общей площади.
//
//***************************** 26.10.2011 ********************************//

static void trian ( CArrRef<Vector3d> vert, ArrRef<double> a, ArrRef<nat> b )
{
    const nat n = vert.size();
    for ( nat k = 2; k < n; ++k )
    {
        for ( int i1 = n - k; --i1 >= 0; )
        {
            const nat i2 = i1 + k;
            const Vector3d v = vert[i2] - vert[i1];
            const Vector3d u = vert[i2] % vert[i1];
            nat im = i1 + 1;
            double s = norm2 ( vert[im] % v + u );
            const nat j = (i2-1) * (i2-2) / 2 + i1;
            if ( k == 2 )
            {
                b[j] = im;
                a[j] = s;
                continue;
            }
            double sum = s + a[j+1];
            const nat ik = i2 - 1;
            for ( nat i = i1+2; i < ik; ++i )
            {
                double s = a[(i-1)*(i-2)/2+i1] + a[j+i-i1];
                if ( sum <= s ) continue;
                s += norm2 ( vert[i] % v + u );
                if ( sum > s ) sum = s, im = i;
            }
            s = norm2 ( vert[ik] % v + u ) + a[(ik-1)*(ik-2)/2+i1];
            if ( sum > s ) sum = s, im = ik;
            b[j] = im;
            a[j] = sum;
        }
    }
}

DynArrRef< Set3<nat> > & trian ( CArrRef<Vector3d> vert, DynArrRef< Set3<nat> > & res )
{
    const nat n = vert.size();
    if ( n < 3 ) return res.resize(0);
    if ( n == 3 )
    {
        res.resize(1)[0] = Set3<nat> ( 0, 1, 2 );
        return res;
    }

// Заполнение массивов a и b

    const nat n2 = (n-1) * (n-2) / 2;
    CmbArray<double, 820> a ( n2 );
    CmbArray<nat, 820+42> b ( n2 + n );
    trian ( vert, a, b );

// Заполнение массива res

    res.resize ( n - 2 );
    nat k = 0, count = 0;
    ArrRef<nat> c ( b, n2, n );
    c[count++] = 0;
    c[count++] = n - 1;
    while ( count > 0 )
    {
        const nat j2 = c[--count];
        const nat j1 = c[--count];
        const nat j = b[(j2-1)*(j2-2)/2+j1];
        res[k++] = Set3<nat> ( j1, j, j2 );
        if ( j2 - j > 1 )
        {
            c[count++] = j;
            c[count++] = j2;
        }
        if ( j - j1 > 1 )
        {
            c[count++] = j1;
            c[count++] = j;
        }
    }
    return res;
}


//**************************** 09.12.2003 *********************************//
//
//   Триангуляция двух ломаных в пространстве. Минимум общей площади.
//
//   vert1, vert2 - ломаные, заданные своими вершинами. 
//   Последняя вершина не совпадает с первой в любом случае.
//   isCycle = true - ломаные считаются замкнутыми и результирующая поверхность будет замкнута в кольцо.
//   res - массив результирующих треугольников, заданных индексами вершин из
//   ломаных vert1 и vert2. Номер индекса от 0 до n1-1 задает точку из vert1,
//   номер индекса от n1 до n1+n2-1 задает точку из vert2 ( n1 и n2 > 1 ).
//   Размер массива res: n1+n2 (isCycle = true); n1+n2-2 (isCycle = false);
//   Возвращает количество построенных треугольников.
//
//***************************** 26.10.2011 ********************************//

DynArrRef< Set3<nat> > & trian ( CArrRef<Vector3d> vert1, CArrRef<Vector3d> vert2, bool isCycle, DynArrRef< Set3<nat> > & res )
{
    nat n1 = vert1.size();
    nat n2 = vert2.size();
    if ( n1 < 2 || n2 < 2 ) return res.resize(0);
    const nat m1 = n1;
    const nat m2 = n2;
    if ( isCycle )
    {
        ++n1;
        ++n2;
    }
    const nat nn = n1 * n2;
    CmbArray<double, 820> a ( nn );
    CmbArray<bool, 820> b ( nn );
// Заполнение матриц a и b
    a[0] = 0.;
    nat l = 1, i, j;
    for ( ; l < n2; ++l )
    {
        const Vector3d & v0 = vert1[0];
        const Vector3d & v1 = vert2[l-1];
        const Vector3d & v2 = vert2[l < m2 ? l : 0];
        const double s = norm2 ( ( v1 - v0 ) % ( v2 - v0 ) );
        a[l] = a[l-1] + s;
        b[l] = false;
    }
    for ( i = 1; i < n1; ++i )
    {
        const Vector3d & v2 = vert2[0];
        const Vector3d & v1 = vert1[i-1];
        const Vector3d & v0 = vert1[i < m1 ? i : 0];
        const double s = norm2 ( ( v1 - v0 ) % ( v2 - v0 ) );
        a[l] = a[l-n2] + s;
        b[l] = true;
        ++l;
        for ( j = 1; j < n2; ++j )
        {
            const Vector3d & v2 = vert2[j < m2 ? j : 0];
            const double s1 = a[l- 1] + norm2 ( ( v2 - v0 ) % ( vert2[j-1] - v0 ) );
            const double s2 = a[l-n2] + norm2 ( ( v2 - v0 ) % ( vert1[i-1] - v0 ) );
            if ( s1 < s2 )
            {
                a[l] = s1;
                b[l] = false;
            }
            else
            {
                a[l] = s2;
                b[l] = true;
            }
            ++l;
        }
    }
// Запись результата в массив res
    int k = n1 + n2 - 2;
    res.resize ( k );
    --l;
    while ( --k >= 0 )
    {
        i = l / n2;
        j = l - n2 * i;
        if ( b[l] )
        {
            nat ij = i - 1;
            if ( i == m1 ) i = 0; 
            if ( j == m2 ) j = 0;
            res[k] = Set3<nat> ( ij, i, j+m1 );
            l -= n2;
        }
        else
        {
            nat ij = j - 1 + m1;
            if ( i == m1 ) i = 0; 
            if ( j == m2 ) j = 0; 
            res[k] = Set3<nat> ( ij, i, j+m1 );
            --l;
        }
    }
    return res;
}
