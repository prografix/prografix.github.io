
// 10.04.2006, 18.04.2006, 20.10.2012

#ifndef CMATRIX_H
#define CMATRIX_H

#include "matrix.h"
#include "complex.h"

// Транспонирование

bool trans  ( const IMatrix<Complex> & a, IMatrix<Complex> & b ); // b = aT

//*********************** 18.04.2006 **************************//
//
//           Решение задачи наименьших квадратов 
//          при помощи преобразований Хаусхолдера.
//
//*********************** 20.10.2012 **************************//

class CLSS_H
{
    IMatrix<Complex> & a;
    nat * pivot1, * pivot2;
    double * beta;
    Complex * alpha;
// Запрет конструктора копии и оператора присваивания:
    CLSS_H ( CLSS_H & );
    void operator = ( CLSS_H & );
public:
    CLSS_H ( IMatrix<Complex> & a );
    bool solve ( const Complex * b, Complex * x ) const; // b[a.nRow], x[a.nCol]
    ~CLSS_H ();
};

bool lss_h ( IMatrix<Complex> & a, const Complex * b, Complex * x ); // b[a.nRow], x[a.nCol]

#endif