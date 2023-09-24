
#pragma once

#include "template.h"

inline float qmod ( const float & x ) { return x * x; }

inline double qmod ( const double & x ) { return x * x; }


//************************ 15.12.2021 *************************//
//
//              Квадратная матрица 2-го порядка 
//
//************************ 15.12.2021 *************************//

template <class T> class Matrix2
{
public:
    T aa, ab,
      ba, bb;

    Matrix2 & set ( const T & v )
    {
        aa = ab =
        ba = bb = v;
        return *this;
    }

    Def<Matrix2> operator ~ () const
    {
        SLU2<T> slu;
        slu.matrix() = *this;
        slu.ac = 1; slu.bc = 0;
        Def<Matrix2> m;
        if ( ! slu.gauss ( m.aa, m.ba ) ) return m;
        slu.ac = 0; slu.bc = 1; 
        m.isDef = slu.gauss ( m.ab, m.bb );
        return m;
    }

    T determinant() const
    {
        return aa * bb - ab * ba;
    }
};

template <class T> Matrix2<T> operator * ( const Matrix2<T> & l, const Matrix2<T> & r )
{
    Matrix2<T> m;
    m.aa = l.aa * r.aa + l.ab * r.ba; m.ab = l.aa * r.ab + l.ab * r.bb;
    m.ba = l.ba * r.aa + l.bb * r.ba; m.bb = l.ba * r.ab + l.bb * r.bb;
    return m;
}


//*************************************************************//
//
//              Квадратная матрица 3-го порядка 
//
//************************ 15.12.2021 *************************//

template <class T> class Matrix3
{
public:
    T aa, ab, ac,
      ba, bb, bc,
      ca, cb, cc;

    Matrix3 & set ( const T & v )
    {
        aa = ab = ac = 
        ba = bb = bc = 
        ca = cb = cc = v;
        return *this;
    }

    T determinant() const
    {
        return  ( ab * bc - ac * bb ) * ca +
                ( ac * ba - aa * bc ) * cb +
                ( aa * bb - ab * ba ) * cc;
    }
};


//************************ 21.08.2023 *************************//
//
//          Симметричная квадратная матрица 3-го порядка 
//
//************************ 21.08.2023 *************************//

template <class T> class SymMatrix3
{
public:
    T aa, ab, ac,
      bb, bc,
      cc;

    SymMatrix3 & set ( const T & v )
    {
        aa = ab = ac = 
        bb = bc = 
        cc = v;
        return *this;
    }

    T determinant() const
    {
        return  ( ab * bc - ac * bb ) * ac +
                ( ac * ab - aa * bc ) * bc +
                ( aa * bb - ab * ab ) * cc;
    }
};


//*************************************************************//
//
//              Квадратная матрица 4-го порядка 
//
//************************ 15.12.2021 *************************//

template <class T> class Matrix4
{
public:
    T aa, ab, ac, ad,
      ba, bb, bc, bd,
      ca, cb, cc, cd,
      da, db, dc, dd;

    Matrix4 & set ( const T & v )
    {
        aa = ab = ac = ad =
        ba = bb = bc = bd =
        ca = cb = cc = cd =
        da = db = dc = dd = v;
        return *this;
    }

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
            if ( ! dd ) return 0;
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

//************************ 20.11.2002 *************************//
//
// Решение систем линейных уравнений 2-го порядка методом Гаусса
//         Выбор ведущего элемента по столбцам
//
//************************ 10.04.2015 *************************//

template <class T1, class T2 = T1> class SLU2 : public Matrix2<T1>
{
public:
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
            if ( ! a ) return false;
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
        set ( v1 );
        ac = bc = v2;
        return *this;
    }

    Matrix2<T1> & matrix ()
    {
        return (Matrix2<T1> &) *this;
    }

    SLU2 & operator += ( const SLU2 & slu )
    {
        aa += slu.aa; ab += slu.ab; ac += slu.ac;
        ba += slu.ba; bb += slu.bb; bc += slu.bc;
        return *this;
    }
};

//************************ 20.11.2002 *************************//
//
// Решение систем линейных уравнений 3-го порядка методом Гаусса
//         Выбор ведущего элемента по столбцам
//
//************************ 24.04.2019 *************************//

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
        set ( v1 );
        ad = bd = cd = v2;
        return *this;
    }
};

//************************ 21.08.2023 *************************//
//
// Решение систем линейных уравнений 3-го порядка методом Гаусса
//         Выбор ведущего элемента по столбцам
//                Симметричная матрица
//
//************************ 21.08.2023 *************************//

template <class T1, class T2 = T1> class SymSLU3 : public SymMatrix3<T1>
{
public:
    T2 ad, bd, cd;

    SymSLU3 & operator += ( const SymSLU3 & slu )
    {
        aa += slu.aa; ab += slu.ab; ac += slu.ac; ad += slu.ad;
        bb += slu.bb; bc += slu.bc; bd += slu.bd;
        cc += slu.cc; cd += slu.cd;
        return *this;
    }

// Решение системы линейных уравнений 3-го порядка методом Гаусса
// aa * x + ab * y + ac * z = ad
// ab * x + bb * y + bc * z = bd
// ac * x + bc * y + cc * z = cd
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
            a = ac / cc;
            b = bc / cc;
            ( d = cd ) /= cc;
            slu.aa = aa - a * ac;   slu.ab = ab - b * ac;   ( e = d ) *= ac;   ( slu.ac = ad ) -= e;
            slu.ba = ab - a * bc;   slu.bb = bb - b * bc;   ( e = d ) *= bc;   ( slu.bc = bd ) -= e;
        }
        else
        if ( mb >= ma )
        {
            a = ab / bc;
            b = bb / bc;
            ( d = bd ) /= bc;
            slu.aa = aa - a * ac;   slu.ab = ab - b * ac;   ( e = d ) *= ac;    ( slu.ac = ad ) -= e;
            slu.ba = ac - a * cc;   slu.bb = bc - b * cc;   ( e = d ) *= cc;    ( slu.bc = cd ) -= e;
        }
        else
        {
            a = aa / ac;
            b = ab / ac;
            ( d = ad ) /= ac;
            slu.aa = ab - a * bc;   slu.ab = bb - b * bc;   ( e = d ) *= bc;    ( slu.ac = bd ) -= e;
            slu.ba = ac - a * cc;   slu.bb = bc - b * cc;   ( e = d ) *= cc;    ( slu.bc = cd ) -= e;
        }
        if ( ! slu.gauss ( x, y ) ) return false;
        z = d;
        ( e = x ) *= a; z -= e;
        ( e = y ) *= b; z -= e;
        return true;
    }

    SymSLU3 & fill ( const T1 & v1, const T2 & v2 )
    {
        set ( v1 );
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
            if ( ! dd ) return false;
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
        set ( v1 );
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
//      mRow, mCol - размеры подматрицы, где выбираются ведущие элементы.
//
//*************************** 02.06.2019 ******************************//

template <class Matrix>
bool _sluGaussRow ( Matrix & data, nat nRow, nat nCol, nat index[], nat mRow, nat mCol )
{
    if ( mRow > nRow ) mRow = nRow;
    if ( mCol > nCol ) mCol = nCol;
    if ( mRow < 2 || mRow > mCol ) return false;
// Прямой ход
    nat i, j, k;
    for ( i = 0; i < nCol; ++i ) index[i] = i;
    for ( k = 0; k < mRow; ++k )
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
        if ( ! max ) return false;
        _swap ( index[k], index[im] );
        const nat ik = index[k];
// Нормализация строки
        const double p = 1. / rk[ik];
        for ( i = k+1; i < nCol; ++i ) rk[index[i]] *= p;
        rk[ik] = 1;
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
            rj[ik] = 0;
        }
    }
// Обратная подстановка
    for ( j = mRow; --j > 0; )
    {
        const double * rj = & data[j][0];
        const nat ij = index[j];
        for ( i = 0; i < j; ++i )
        {
            double * ri = & data[i][0];
            const double t = ri[ij];
            for ( k = mRow; k < nCol; ++k )
            {
                const nat ik = index[k];
                ri[ik] -= rj[ik] * t;
            }
            ri[ij] = 0;
        }
    }
    return true;
}

template <class T> class IMatrix;

template <class T> inline
bool sluGaussRow ( IMatrix<T> & data, nat nRow, nat nCol, unsigned * index, nat mRow, nat mCol )
{
    return _sluGaussRow ( data, nRow, nCol, index, mRow, mCol );
}

template <class T> class ArrRef2;

template <class T> inline
bool sluGaussRow ( ArrRef2<T> & data, nat nRow, nat nCol, unsigned * index, nat mRow, nat mCol )
{
    return _sluGaussRow ( data, nRow, nCol, index, mRow, mCol );
}
