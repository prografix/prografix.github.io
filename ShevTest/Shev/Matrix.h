
// 13.03.2004, 27.03.2004, 05.06.2004, 26.06.2004, 25.02.2006, 20.10.2012, 02.02.2013

#pragma once

#include "template.h"

template <class Type>
class IMatrix
{
    Type * const * const data;
// Запрет оператора присваивания и конструктора копии
    void operator = ( const IMatrix & );
    IMatrix ( const IMatrix & );
public:
    const nat nRow; // к-во строк
    const nat nCol; // к-во столбцов

    IMatrix ( nat r, nat c, Type * const * const p ) : nRow(r), nCol(c), data(p) {}

    IMatrix & fill ( const Type & p )
    {
        for ( nat i = 0; i < nRow; ++i )
        {
            Type * t = data[i];
            for ( nat j = 0; j < nCol; ++j ) t[j] = p;
        }
        return *this;
    }
    IMatrix & operator *= ( const Type & p )
    {
        for ( nat i = 0; i < nRow; ++i )
        {
            Type * t = data[i];
            for ( nat j = 0; j < nCol; ++j ) t[j] *= p;
        }
        return *this;
    }
    operator       Type * const * ()       { return data; }
    operator const Type * const * () const { return data; }
};

template <class Type, nat R, nat C>
class SMatrix : public IMatrix<Type>
{
    Type * p[R];
    Type stor[R*C];
public:
    SMatrix () : IMatrix<Type> ( R, C, p )
    {
        p[0] = stor;
        for ( nat i = 1; i < R; ++i ) p[i] = p[i-1] + C;
    }

    SMatrix ( const SMatrix & m ) : IMatrix<Type> ( R, C, p )
    {
        nat i;
        p[0] = stor;
        for ( i = 1; i < R; ++i ) p[i] = p[i-1] + C;
        for ( i = 0; i < R*C; ++i ) stor[i] = m.stor[i];
    }

    SMatrix & operator = ( const SMatrix & m )
    {
        for ( nat i = 0; i < R*C; ++i ) stor[i] = m.stor[i];
        return *this;
    }
};

template <class Type>
class HMatrix : public IMatrix<Type>
{
    Type ** p;
// Запрет оператора присваивания
    HMatrix & operator = ( const HMatrix & m );
public:
    HMatrix ( nat r, nat c ) : IMatrix<Type> ( r, c, p = r > 0 ? new Type*[r] : 0 )
    {
        if ( ! p ) return;
        p[0] = new Type[r*c];
        for ( nat i = 1; i < r; ++i ) p[i] = p[i-1] + c;
    }

    explicit HMatrix ( const IMatrix<Type> & m ) : IMatrix<Type> ( m.nRow, m.nCol, p = m.nRow > 0 ? new Type*[m.nRow] : 0 )
    {
        if ( ! p ) return;
        const nat n = m.nRow * m.nCol;
        Type * t = p[0] = new Type[n];
        for ( nat i = 1; i < m.nRow; ++i ) p[i] = p[i-1] + m.nCol;
        const Type * s = m[0];
        for ( nat j = 0; j < n; ++j ) t[j] = s[j];
    }

    explicit HMatrix ( const HMatrix<Type> & m ) : IMatrix<Type> ( m.nRow, m.nCol, p = m.nRow > 0 ? new Type*[m.nRow] : 0 )
    {
        if ( ! p ) return;
        const nat n = m.nRow * m.nCol;
        Type * t = p[0] = new Type[n];
        for ( nat i = 1; i < m.nRow; ++i ) p[i] = p[i-1] + m.nCol;
        const Type * s = m[0];
        for ( nat j = 0; j < n; ++j ) t[j] = s[j];
    }

    ~HMatrix()
    {
        if ( p != 0 )
        {
            delete[] p[0];
            delete[] p;
        }
    }
};

template <class Type, nat R, nat C>
class CmbMatrix : public IMatrix<Type>
{
    Type ** p;
    Type * pa[R];
    Type stor[R*C];
// Запрет оператора присваивания
    CmbMatrix & operator = ( const CmbMatrix & m );
public:
    CmbMatrix ( nat r, nat c ) : IMatrix<Type> ( r, c, p = r > R ? new Type*[r] : pa )
    {
        const nat n = nRow * nCol;
        p[0] = n > R*C ? new Type[n] : stor;
        for ( nat i = 1; i < nRow; ++i ) p[i] = p[i-1] + nCol;
    }

    explicit CmbMatrix ( const IMatrix<Type> & m ) : IMatrix<Type> ( m.nRow, m.nCol, p = m.nRow > R ? new Type*[m.nRow] : pa )
    {
        const nat n = nRow * nCol;
        Type * t = p[0] = n > R*C ? new Type[n] : stor;
        for ( nat i = 1; i < nRow; ++i ) p[i] = p[i-1] + nCol;
        const Type * s = m[0];
        for ( nat j = 0; j < n; ++j ) t[j] = s[j];
    }

    explicit CmbMatrix ( const CmbMatrix<Type, R, C> & m ) : IMatrix<Type> ( m.nRow, m.nCol, p = m.nRow > R ? new Type*[m.nRow] : pa )
    {
        const nat n = nRow * nCol;
        Type * t = p[0] = n > R*C ? new Type[n] : stor;
        for ( nat i = 1; i < nRow; ++i ) p[i] = p[i-1] + nCol;
        const Type * s = m[0];
        for ( nat j = 0; j < n; ++j ) t[j] = s[j];
    }

    ~CmbMatrix()
    {
        if ( p[0] != stor ) delete[] p[0];
        if ( p != pa ) delete[] p;
    }
};

template <class Type>
bool copy ( const IMatrix<Type> & a, IMatrix<Type> & b ) // b = a
{
    if ( a.nCol != b.nCol || a.nRow != b.nRow || a.nCol == 0 ) return false;
    if ( a[0] == b[0] ) return true; // a и b - одна и та же матрица
    const Type * pa = *a;
          Type * pb = *b;
    for ( int i = b.nRow*b.nCol; --i >= 0; ) *pb++ = *pa++;
    return true;
}

template <class Type>
bool plus ( const IMatrix<Type> & a, const IMatrix<Type> & b, IMatrix<Type> & c ) // c = a + b
{
    if ( a.nCol != b.nCol || a.nRow != b.nRow ) return false;
    if ( a.nCol != c.nCol || a.nRow != c.nRow || a.nRow == 0 ) return false;
    const Type * pa = *a;
    const Type * pb = *b;
          Type * pc = *c;
    for ( int i = a.nRow*a.nCol; --i >= 0; ) *pc++ = *pa++ + *pb++;
    return true;
}

template <class Type>
bool minus ( const IMatrix<Type> & a, const IMatrix<Type> & b, IMatrix<Type> & c ) // c = a - b
{
    if ( a.nCol != b.nCol || a.nRow != b.nRow ) return false;
    if ( a.nCol != c.nCol || a.nRow != c.nRow || a.nRow == 0 ) return false;
    const Type * pa = *a;
    const Type * pb = *b;
          Type * pc = *c;
    for ( int i = a.nRow*a.nCol; --i >= 0; ) *pc++ = *pa++ - *pb++;
    return true;
}

template <class Type>
bool multi ( const IMatrix<Type> & a, const IMatrix<Type> & b, IMatrix<Type> & c ) // c = a * b
{
    if ( a.nCol != b.nRow || a.nRow != c.nRow || b.nCol != c.nCol || a.nRow == 0 ) return false;
    if ( a[0] == c[0] || b[0] == c[0] )
    {
        HMatrix<Type> m ( c.nRow, c.nCol );
        return multi ( a, b, m ) && copy ( m, c );
    }
    for ( int i = c.nRow; --i >= 0; )
    {
        const Type * r = a[i];
        for ( int j = c.nCol; --j >= 0; )
        {
            Type s = 0.;
            for ( int k = a.nCol; --k >= 0; ) s += r[k] * b[k][j];
            c[i][j] = s;
        }
    }
    return true;
}

// Разные функции

bool determinant ( const IMatrix<double> & m, double & d );
bool trans  ( const IMatrix<double> & a, IMatrix<double> & b ); // b = aT
bool inverse( const IMatrix<double> & a, IMatrix<double> & b ); // a * b = 1

// Матричные нормы

double norm1 ( const IMatrix<double> & ); // 1-норма
double norm2 ( const IMatrix<double> & ); // 2-норма
double normF ( const IMatrix<double> & ); // норма Фробениуса
double normU ( const IMatrix<double> & ); // бесконечная норма

// Cингулярное разложение: A = U * W * V

bool svd ( const IMatrix<double> & A, IMatrix<double> & U, IMatrix<double> & W, IMatrix<double> & V );


//*********************** 10.02.2024 ************************//
//
//                   Динамическая матрица
//
//*********************** 10.02.2024 ************************//

template <class Type> class DMatrix
{
    Type * data;
    Type ** row;
    nat nRow; // к-во строк
    nat nCol; // к-во столбцов
// Запрет оператора присваивания
    DMatrix & operator = ( const DMatrix & m );
public:
    DMatrix () : nRow(0), nCol(0), row(0), data(0) {}

    DMatrix ( nat r, nat c ) : nRow(r), nCol(c)
    {
        if ( nRow > 0 )
        {
            row = new Type*[nRow];
            if ( nCol > 0 )
            {
                row[0] = data = new Type[nRow*nCol];
                for ( nat i = 1; i < nRow; ++i ) row[i] = row[i-1] + nCol;
            }
            else
            {
                data = 0;
                for ( nat i = 0; i < nRow; ++i ) row[i] = 0;
            }
        }
        else
        {
            row = 0;
            data = 0;
        }
    }

    explicit DMatrix ( const DMatrix<Type> & m ) : nRow ( m.nRow ), nCol ( m.nCol )
    {
        if ( nRow > 0 )
        {
            row = new Type*[nRow];
            if ( nCol > 0 )
            {
                nat i;
                row[0] = data = new Type[nRow*nCol];
                for ( i = 1; i < nRow; ++i ) row[i] = row[i-1] + nCol;
                for ( i = 0; i < nRow; ++i )
                {
                    Type * pi = row[i];
                    const Type * mi = m[i];
                    for ( nat j = 0; j < nCol; ++j ) pi[j] = mi[j];
                }
            }
            else
            {
                data = 0;
                for ( nat i = 0; i < nRow; ++i ) row[i] = 0;
            }
        }
        else
        {
            row = 0;
            data = 0;
        }
    }

    ~DMatrix()
    {
        delete[] data;
        delete[] row;
    }

    nat rowSize () { return nRow; } // к-во строк
    nat colSize () { return nCol; } // к-во столбцов

    DMatrix & resize ( nat r, nat c )
    {
        delete[] data;
        delete[] row;
        nRow = r;
        nCol = c;
        if ( r > 0 )
        {
            row = new Type*[r];
            if ( c > 0 )
            {
                row[0] = data = new Type[r*c];
                for ( nat i = 1; i < r; ++i ) row[i] = row[i-1] + c;
            }
            else
            {
                data = 0;
                for ( nat i = 0; i < r; ++i ) row[i] = 0;
            }
        }
        else
        {
            row = 0;
            data = 0;
        }
        return *this;
    }

          Type * operator [] ( nat i )       { return row[i]; }
    const Type * operator [] ( nat i ) const { return row[i]; }

    bool swapRows ( nat i1, nat i2 )
    {
        if ( i1 >= nRow || i2 >= nRow ) return false;
        _swap ( row[i1], row[i2] );
        return true;
    }

    DMatrix & fill ( const Type & p )
    {
        const nat n = nRow * nCol;
        for ( nat i = 0; i < n; ++i ) data[i] = p;
        return *this;
    }

    DMatrix & operator *= ( const Type & p )
    {
        const nat n = nRow * nCol;
        for ( nat i = 0; i < n; ++i ) data[i] *= p;
        return *this;
    }
};
