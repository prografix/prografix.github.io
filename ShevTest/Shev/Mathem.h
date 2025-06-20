
#pragma once

#include "Matrix.h"
#include "MathFunc.h"
#include "ShevArray2.h"

//*********************** 13.05.2004 ************************//
//
//                   �������������� �����
//
//*********************** 25.06.2013 ************************//

double root3 ( double x );
double root4 ( double x );
double root5 ( double x );
double root6 ( double x );
double root7 ( double x );
double root8 ( double x );
double root9 ( double x );
double root10( double x );


//*************************************************************//
//
//    ������� �������������� ��������� 2, 3, 4 � 5 �������.
//    ������������ �������� - ���������� ��������� ������.
//
//************************ 10.08.2018 *************************//

// a * x^2 + b * x + c = 0 - ����� ���������� ���������

nat root2 ( double a, double b, double c, double x[2] );


// x^2 + a * x + b = 0 - ���������� ���������� ���������

nat root2 ( double a, double b, double x[2] );


// x^3 + p * x + q = 0

nat root3s ( double p, double q, double * x );


// x^3 + a * x^2 + b * x + c = 0

nat root3 ( double a, double b, double c, double * x );


// x^4 + p * x^2 + q * x + r = 0

nat root4s ( double p, double q, double r, double * x );


// x^4 + a * x^3 + b * x^2 + c * x + d = 0

nat root4 ( double a, double b, double c, double d, double * x );


// x^5 + a * x^4 + b * x^3 + c * x^2 + d * x + e = 0

nat root5 ( double a, double b, double c, double d, double e, double * x );


//************************ 26.10.2006 *************************//
//
//      ������� �������������� ��������� n-�� �������.
//
//************************ 26.10.2006 *************************//

bool rootN ( nat n, const double * p, double * r, double * i );


//*********************************************************************//
//
//      ������� ������ �������� ��������� ������� ������
//
//*************************** 24.04.2007 ******************************//

class SLU_Gauss
{
    const nat n;
    CmbArray<nat, 80> ipvt;
    DynArray2<double> a;
    double cond;
// ������ ������������ ����� � ��������� ������������:
    SLU_Gauss ( SLU_Gauss & );
    void operator = ( SLU_Gauss & );
public:
    SLU_Gauss ( nat k, const double * const * a );
    bool solve ( const double * b, double * x ) const; // b[n], x[n]
    bool solve ( const double * const * a, const double * b, double * x ) const;
    double condition () const { return cond; }
    double determinant () const;
};


//*************************** 31.07.2010 ******************************//
//
//      ������� ������ �������� ��������� ������� ������
//      ����� �������� �������� �� ��������
//
//*************************** 27.04.2016 ******************************//

bool slu_gauss ( ArrRef2<double> & data ); // ��������� ������ ��������
bool slu_gauss ( ArrRef2<double> & data, ArrRef<double> & x ); // ���� ������ �������


//*************************** 24.03.2011 ******************************//
//
//      ������� ������ �������� ��������� ������� ������
//      ����� ����� �������� ��������
//      nc - �-�� �������� � ����� ����� ������� ���������
//      col - ������ �������� ��������
//
//*************************** 24.03.2011 ******************************//

bool slu_gauss ( ArrRef2<double> & data, const nat nc, ArrRef<nat> & col );


//*************************** 31.05.2025 ******************************//
//
//      ������� ������ �������� ��������� ������� ������
//      ��� ����������� �������. ����� �������� �������� �� �������.
//
//*************************** 31.05.2025 ******************************//

bool sluGaussRow ( nat n, Suite<SortItem<nat, double> > * a, double * b, double * x );


//*********************************************************************//
//
//      ������� ������ �������� ��������� �������
//      ��������������� �����-������ ( n <= m )
//
//*************************** 18.01.2007 ******************************//

bool slu_ortho ( nat n, nat m, const double * const * a, const double * b, double * x );


//*************************** 01.06.2007 ******************************//
//
//      ������� ������ �������� ��������� �������
//      ��������������� ��� ������ ��������� ( n <= m )
//
//*************************** 01.06.2007 ******************************//

bool slu_orthoH1 ( nat n, nat m, const double * const * a, const double * b, double * x );

nat slu_orthoH1 ( const IMatrix<double> & a, const double * b, IMatrix<double> & x );


//*************************************************************//
//
//      ������������ ������������ ����������� �������.
//              ���������� ���������.
//
//*************************************************************//

class SM_Chol
{
    const nat n;
    CmbArray<double, 820> g;
// ������ ������������ ����� � ��������� ������������:
    SM_Chol ( SM_Chol & );
    void operator = ( SM_Chol & );
public:
    SM_Chol ( nat k, const double * const * a );
    bool solve ( const double * b, double * x ) const; // b[n], x[n]
    double determinant () const;
};


//*************************************************************//
//
//      ������������ ������������ ����������� �������.
//              LDLt ����������.
//
//*************************************************************//

class SM_LDLt
{
    const nat n;
    CmbArray<double, 820> g;
// ������ ������������ ����� � ��������� ������������:
    SM_LDLt ( SM_LDLt & );
    void operator = ( SM_LDLt & );
public:
    SM_LDLt ( nat k, const double * const * a );
    bool solve ( const double * b, double * x ) const; // b[n], x[n]
    double determinant () const;
};


//*********************** 22.09.2023 **************************//
//
//           ������������ ����������� �������.
//
//*********************** 22.09.2023 **************************//

bool slu_LDLt ( nat n, const nat * m, double * a, const double * b, double * x );
bool slu_LDLt ( nat n, const Suite<SortItem<nat, double> > * data, const double * b, double * x );


//*********************** 04.10.2023 **************************//
//
//           ������������ ����������� �������.
// ������������ �������� � ����� ��� ���������� ���������� �����.
//
//*********************** 04.10.2023 **************************//

bool slu_LDLtO ( nat n, const Suite<SortItem<nat, double> > * data, const double * b, double * x );


//*********************** 01.06.2023 **************************//
//
//      ������������ ������������ ����������� �������.
//              ����� ���������� ����������.
//
//*********************** 16.06.2023 **************************//

bool slu_cg ( nat n, const Suite<SortItem<nat, double> > * data, const double * b, double * x, bool isX = 0 );


//*********************** 08.04.2010 **************************//
//
//      ��������������� ������� �������� ��������� ( n > m ).
//      ������� 1-����� ������� �������.
//
//*********************** 24.12.2021 **************************//

bool minNorm1 ( CArrRef2<double> & data, ArrRef<double> & x, ArrRef<nat> & index );
bool minNorm1 ( CArrRef2<double> & data, ArrRef<double> & x );
bool minNorm1 ( CArrRef2<double> & data, ArrRef<nat> & index );


//*********************** 24.07.2010 **************************//
//
//      ��������������� ������� �������� ��������� ( n > m ).
//      ������� 2-����� ������� �������.
//
//*********************** 12.11.2011 **************************//

bool minNorm2 ( CArrRef2<double> & data, ArrRef<double> & x );


//*********************** 17.07.2010 **************************//
//
//      ��������������� ������� �������� ��������� ( n > m ).
//      ������� ����������� ����� ������� �������.
//
//*********************** 12.11.2011 **************************//

bool minNormU ( CArrRef2<double> & data, ArrRef<double> & x, ArrRef<nat> & index );
bool minNormU ( CArrRef2<double> & data, ArrRef<double> & x );


//****************************** 07.03.2003 *********************************//
//
//      ����� ���������� ��������� ( n >= m ). �������������� �����������.
//
//****************************** 21.07.2012 *********************************//

class LSS_H
{
    IMatrix<double> & a;
    CmbArray<nat, 80> pivot;
    mutable CmbArray<double, 80> alpha;
// ������ ������������ ����� � ��������� ������������:
    LSS_H ( LSS_H & );
    void operator = ( LSS_H & );
public:
    LSS_H ( IMatrix<double> & a );
    bool solve ( CArrRef<double> b, ArrRef<double> x ) const; // b[a.nRow], x[a.nCol]
};

bool lss_h ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x );


//****************************** 07.03.2003 *********************************//
//
//      ����� ���������� ��������� ( n >= m ). O�������������� �����-������.
//
//****************************** 21.07.2012 *********************************//

bool ortholin ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x );
bool ortholin ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x, DynArrRef<double> & p );


//****************************** 19.06.2007 *********************************//
//
//          ��������������� �������� ��� ������� ������ ���������
//
//****************************** 21.10.2012 *********************************//

bool columnScale ( bool (*func) ( IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x ),
                   const IMatrix<double> & a, CArrRef<double> b, ArrRef<double> x );


//****************************** 23.05.2007 *********************************//
//
//          O�������������� �������� ��������.
//          nr - �-�� ����������� ����� �������.
//          ������������ �������� - ���� �������.
//
//****************************** 23.05.2007 *********************************//

nat orthogonalizationH1 ( nat nr, IMatrix<double> & mat );


//***************************************************************************//
// 
//  ����� ����� ���������� ����������� �������� � ����������� ��������
//                       ��� ������������ ������
//    ������� ������:
//        n - ����������� �������
//        a - �������� �������. � �������� ������ ��������������� ��������
//            ����� ��������, �� �� ����� ������������ �� ���������������
//    �������� ������:
//        d - ������ ����������� ��������
//        v - ������ ����������� ��������
//
//***************************************************************************//

void jacobi ( const nat n, double * const * a, double * d, double * const * v );


//************************* 20.02.2007 **************************************//
//
//        ���������� �������������� ������� � ������� ����� �����������
//
//************************* 20.02.2007 **************************************//

void orthes ( nat n, double * const * a );


//************************* 05.08.2006 **************************************//
//
//        ���������� ����������� �������� �������������� �������
//        �������� � ������� ����� �����������
//
//************************* 05.08.2006 **************************************//

bool hqr ( nat n, double * const * h, double * wr, double * wi );


//************************* 03.12.2005 **************************************//
//
//        ����� ���� ������� �� �������� ���������
//        �������� ������� � ax � bx ������ ����� ������ �����
//
//************************* 03.12.2005 **************************************//

bool zeroin ( double ax, double bx, const MathFunc1 & func, double tol, double & res );
