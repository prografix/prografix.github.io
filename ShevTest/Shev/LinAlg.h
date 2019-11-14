
#pragma once

#include "typedef.h"

inline float qmod ( const float & x ) { return x * x; }

inline double qmod ( const double & x ) { return x * x; }

//************************ 20.11.2002 *************************//
//
// Решение систем линейных уравнений 2-го порядка методом Гаусса
//         Выбор ведущего элемента по столбцам
//
//************************ 10.04.2015 *************************//

template <class T1, class T2 = T1> class SLU2
{
public:
    T1 aa, ab, // aa * x + ab * y = ac
       ba, bb; // ba * x + bb * y = bc
    T2 ac, bc;

// Решение системы линейных уравнений 2-го порядка методом Гаусса
    bool gauss ( T2 & x, T2 & y ) const
    {
        T2 v1, v2;
        const double ma = qmod ( aa );
        const double mb = qmod ( ba );
        if ( ma >= mb )
        {
            if ( ! ma ) return false;
            const T1 c = ab / aa;
            const T1 b = bb - c * ba;
            if ( ! b ) return false;
            ( v1 = ac ) /= aa;
            ( v2 = v1 ) *= ba;
            ( ( y = bc ) -= v2 ) /= b;
            ( v2 = y ) *= c;
        }
        else
        {
            const T1 c = bb / ba;
            const T1 a = ab - c * aa;
            if ( a == 0 ) return false;
            ( v1 = bc ) /= ba;
            ( v2 = v1 ) *= aa;
            ( ( y = ac ) -= v2 ) /= a;
            ( v2 = y ) *= c;
        }
        ( x = v1 ) -= v2;
        return true;
    }

    SLU2 & fill ( const T1 & v1, const T2 & v2 )
    {
        aa = ab =
        ba = bb = v1;
        ac = bc = v2;
        return *this;
    }
};

//************************ 20.11.2002 *************************//
//
// Решение систем линейных уравнений 3-го порядка методом Гаусса
//         Выбор ведущего элемента по столбцам
//
//************************ 24.04.2019 *************************//

template <class T> class Matrix3
{
public:
    T aa, ab, ac,
      ba, bb, bc,
      ca, cb, cc;

    T determinant() const
    {
        return  ( ab * bc - ac * bb ) * ca +
                ( ac * ba - aa * bc ) * cb +
                ( aa * bb - ab * ba ) * cc;
    }
};

template <class T1, class T2 = T1> class SLU3 : public Matrix3<T1>
{
public:
    T2 ad, bd, cd;

// Решение системы линейных уравнений 3-го порядка методом Гаусса
// aa * x + ab * y + ac * z = ad
// ba * x + bb * y + bc * z = bd
// ca * x + cb * y + cc * z = cd
    bool gauss ( T2 & x, T2 & y, T2 & z ) const
    {
        SLU2<T1, T2> slu;
        T1 a, b;
        T2 d, e;
        const double ma = qmod ( ac );
        const double mb = qmod ( bc );
        const double mc = qmod ( cc );
        if ( mc >= mb && mc >= ma )
        {
            if ( ! mc ) return false;
            a = ca / cc;
            b = cb / cc;
            ( d = cd ) /= cc;
            slu.aa = aa - a * ac;   slu.ab = ab - b * ac;   ( e = d ) *= ac;   ( slu.ac = ad ) -= e;
            slu.ba = ba - a * bc;   slu.bb = bb - b * bc;   ( e = d ) *= bc;   ( slu.bc = bd ) -= e;
        }
        else
        if ( mb >= ma )
        {
            a = ba / bc;
            b = bb / bc;
            ( d = bd ) /= bc;
            slu.aa = aa - a * ac;   slu.ab = ab - b * ac;   ( e = d ) *= ac;    ( slu.ac = ad ) -= e;
            slu.ba = ca - a * cc;   slu.bb = cb - b * cc;   ( e = d ) *= cc;    ( slu.bc = cd ) -= e;
        }
        else
        {
            a = aa / ac;
            b = ab / ac;
            ( d = ad ) /= ac;
            slu.aa = ba - a * bc;   slu.ab = bb - b * bc;   ( e = d ) *= bc;    ( slu.ac = bd ) -= e;
            slu.ba = ca - a * cc;   slu.bb = cb - b * cc;   ( e = d ) *= cc;    ( slu.bc = cd ) -= e;
        }
        if ( ! slu.gauss ( x, y ) ) return false;
        z = d;
        ( e = x ) *= a; z -= e;
        ( e = y ) *= b; z -= e;
        return true;
    }

    SLU3 & fill ( const T1 & v1, const T2 & v2 )
    {
        aa = ab = ac = 
        ba = bb = bc = 
        ca = cb = cc = v1;
        ad = bd = cd = v2;
        return *this;
    }
};

//************************ 12.05.2012 *************************//
//
// Решение систем линейных уравнений 4-го порядка методом Гаусса
//         Выбор ведущего элемента по столбцам
//
//************************ 24.04.2019 *************************//

template <class T> class Matrix4
{
public:
    T aa, ab, ac, ad,
      ba, bb, bc, bd,
      ca, cb, cc, cd,
      da, db, dc, dd;

    T determinant() const
    {
        Matrix3<T> m;
        T a, b, c;
        const double ma = qmod ( ad );
        const double mb = qmod ( bd );
        const double mc = qmod ( cd );
        const double md = qmod ( dd );
        if ( md >= mc && md >= mb && md >= ma )
        {
            if ( dd == 0 ) return 0;
            a = da / dd;
            b = db / dd;
            c = dc / dd;
            m.aa = aa - a * ad;   m.ab = ab - b * ad;   m.ac = ac - c * ad;
            m.ba = ba - a * bd;   m.bb = bb - b * bd;   m.bc = bc - c * bd;
            m.ca = ca - a * cd;   m.cb = cb - b * cd;   m.cc = cc - c * cd;
            return dd * m.determinant();
        }
        if ( mc >= mb && mc >= ma )
        {
            a = ca / cd;
            b = cb / cd;
            c = cc / cd;
            m.aa = aa - a * ad;   m.ab = ab - b * ad;   m.ac = ac - c * ad;
            m.ba = ba - a * bd;   m.bb = bb - b * bd;   m.bc = bc - c * bd;
            m.ca = da - a * dd;   m.cb = db - b * dd;   m.cc = dc - c * dd;
            return -cd * m.determinant();
        }
        if ( mb >= ma )
        {
            a = ba / bd;
            b = bb / bd;
            c = bc / bd;
            m.aa = aa - a * ad;   m.ab = ab - b * ad;   m.ac = ac - c * ad;
            m.ba = ca - a * cd;   m.bb = cb - b * cd;   m.bc = cc - c * cd;
            m.ca = da - a * dd;   m.cb = db - b * dd;   m.cc = dc - c * dd;
            return bd * m.determinant();
        }
        else
        {
            a = aa / ad;
            b = ab / ad;
            c = ac / ad;
            m.aa = ba - a * bd;   m.ab = bb - b * bd;   m.ac = bc - c * bd;
            m.ba = ca - a * cd;   m.bb = cb - b * cd;   m.bc = cc - c * cd;
            m.ca = da - a * dd;   m.cb = db - b * dd;   m.cc = dc - c * dd;
            return -ad * m.determinant();
        }
    }
};

template <class T1, class T2 = T1> class SLU4 : public Matrix4<T1>
{
public:
    T2 ae, be, ce, de;

// Решение системы линейных уравнений 4-го порядка методом Гаусса
// aa * xa + ab * xb + ac * xc + ad * xd = ae
// ba * xa + bb * xb + bc * xc + bd * xd = be
// ca * xa + cb * xb + cc * xc + cd * xd = ce
// da * xa + db * xb + dc * xc + dd * xd = de
    bool gauss ( T2 & xa, T2 & xb, T2 & xc, T2 & xd ) const
    {
        SLU3<T1, T2> slu;
        T1 a, b, c;
        T2 e, f;
        const double ma = qmod ( ad );
        const double mb = qmod ( bd );
        const double mc = qmod ( cd );
        const double md = qmod ( dd );
        if ( md >= mc && md >= mb && md >= ma )
        {
            if ( dd == 0 ) return false;
            a = da / dd;
            b = db / dd;
            c = dc / dd;
            e = de; e /= dd;
            f = e;  f *= ad;
            slu.aa = aa - a * ad;   slu.ab = ab - b * ad;   slu.ac = ac - c * ad;   slu.ad = ae;    slu.ad -= f;
            f = e;  f *= bd;
            slu.ba = ba - a * bd;   slu.bb = bb - b * bd;   slu.bc = bc - c * bd;   slu.bd = be;    slu.bd -= f;
            f = e;  f *= cd;
            slu.ca = ca - a * cd;   slu.cb = cb - b * cd;   slu.cc = cc - c * cd;   slu.cd = ce;    slu.cd -= f;
        }
        else
        if ( mc >= mb && mc >= ma )
        {
            a = ca / cd;
            b = cb / cd;
            c = cc / cd;
            e = ce; e /= cd;
            f = e;  f *= ad;
            slu.aa = aa - a * ad;   slu.ab = ab - b * ad;   slu.ac = ac - c * ad;   slu.ad = ae;    slu.ad -= f;
            f = e;  f *= bd;
            slu.ba = ba - a * bd;   slu.bb = bb - b * bd;   slu.bc = bc - c * bd;   slu.bd = be;    slu.bd -= f;
            f = e;  f *= dd;
            slu.ca = da - a * dd;   slu.cb = db - b * dd;   slu.cc = dc - c * dd;   slu.cd = de;    slu.cd -= f;
        }
        else
        if ( mb >= ma )
        {
            a = ba / bd;
            b = bb / bd;
            c = bc / bd;
            e = be; e /= bd;
            f = e;  f *= ad;
            slu.aa = aa - a * ad;   slu.ab = ab - b * ad;   slu.ac = ac - c * ad;   slu.ad = ae;    slu.ad -= f;
            f = e;  f *= cd;
            slu.ba = ca - a * cd;   slu.bb = cb - b * cd;   slu.bc = cc - c * cd;   slu.bd = ce;    slu.bd -= f;
            f = e;  f *= dd;
            slu.ca = da - a * dd;   slu.cb = db - b * dd;   slu.cc = dc - c * dd;   slu.cd = de;    slu.cd -= f;
        }
        else
        {
            a = aa / ad;
            b = ab / ad;
            c = ac / ad;
            e = ae; e /= ad;
            f = e;  f *= bd;
            slu.aa = ba - a * bd;   slu.ab = bb - b * bd;   slu.ac = bc - c * bd;   slu.ad = be;    slu.ad -= f;
            f = e;  f *= cd;
            slu.ba = ca - a * cd;   slu.bb = cb - b * cd;   slu.bc = cc - c * cd;   slu.bd = ce;    slu.bd -= f;
            f = e;  f *= dd;
            slu.ca = da - a * dd;   slu.cb = db - b * dd;   slu.cc = dc - c * dd;   slu.cd = de;    slu.cd -= f;
        }
        if ( ! slu.gauss ( xa, xb, xc ) ) return false;
        xd = e;
        f = xa; f *= a; xd -= f;
        f = xb; f *= b; xd -= f;
        f = xc; f *= c; xd -= f;
        return true;
    }

    SLU4 & fill ( const T1 & v1, const T2 & v2 )
    {
        aa = ab = ac = ad =
        ba = bb = bc = bd =
        ca = cb = cc = cd =
        da = db = dc = dd = v1;
        ae = be = ce = de = v2;
        return *this;
    }
};

//*************************** 19.02.2017 ******************************//
//
//      Метод исключений Гаусса.
//      Выбор ведущего элемента по строкам.
//      nRow, nCol - размеры матрицы.
//      index - массив для индексов выбранных столбцов (nCol).
//      mCol - размер подматрицы, где выбираются ведущие элементы.
//
//*************************** 02.06.2019 ******************************//

template <class Matrix>
bool _sluGaussRow ( Matrix & data, nat nRow, nat nCol, nat index[], nat mCol )
{
    if ( nRow == 0 || nCol <= nRow || mCol < nRow ) return false;
    if ( mCol > nCol ) mCol = nCol;
// Прямой ход
    nat i, j, k;
    for ( i = 0; i < nCol; ++i ) index[i] = i;
    for ( k = 0; k < nRow; ++k )
    {
// Поиск максимального по модулю члена в k-ой строке
        nat im = k;
        double * rk = & data[k][0];
        double max = fabs ( rk[index[k]] );
        for ( i = k+1; i < mCol; ++i )
        {
            const double t = fabs ( rk[index[i]] );
            if ( max < t ) max = t, im = i;
        }
        if ( max == 0 ) return false;
        _swap ( index[k], index[im] );
        const nat ik = index[k];
// Нормализация строки
        const double p = 1. / rk[ik];
        for ( i = k+1; i < nCol; ++i ) rk[index[i]] *= p;
// Вычитание строк
        for ( j = k+1; j < nRow; ++j )
        {
            double * rj = & data[j][0];
            const double t = rj[ik];
            for ( i = k+1; i < nCol; ++i )
            {
                const nat ii = index[i];
                if ( fabs ( rj[ii] -= rk[ii] * t ) < 1e-290 ) rj[ii] = 0;
            }
        }
    }
// Обратная подстановка
    for ( j = nRow; --j > 0; )
    {
        const double * rj = & data[j][0];
        for ( i = 0; i < j; ++i )
        {
            double * ri = & data[i][0];
            const double t = ri[index[j]];
            for ( k = nRow; k < nCol; ++k )
            {
                const nat ik = index[k];
                ri[ik] -= rj[ik] * t;
            }
        }
    }
    return true;
}

template <class T> class IMatrix;

template <class T> inline
bool sluGaussRow ( IMatrix<T> & data, nat nRow, nat nCol, unsigned * index, nat mCol )
{
    return _sluGaussRow ( data, nRow, nCol, index, mCol );
}

template <class T> class ArrRef2;

template <class T> inline
bool sluGaussRow ( ArrRef2<T> & data, nat nRow, nat nCol, unsigned * index, nat mCol )
{
    return _sluGaussRow ( data, nRow, nCol, index, mCol );
}
