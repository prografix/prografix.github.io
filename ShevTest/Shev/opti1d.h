
#pragma once

#include "MathFunc.h"
#include "ShevArray.h"

//************************* 11.06.2008 **************************************//
//
//        ����� �������� ��� ��������� ������� �� �������� ��������� 
//                  ����� "�������� �������"
//
//************************* 26.04.2010 **************************************//

double goldenRatioMin ( double a, double b, const MathFunc1 & func, double eps );
double goldenRatioMax ( double a, double b, const MathFunc1 & func, double eps );


//************************* 26.04.2010 **************************************//
//
//        ����� �������� ��� ��������� ������� �� �������� ��������� 
//                  ��������������� ����� 
//
//************************* 26.04.2010 **************************************//

double fmin ( double a, double b, const MathFunc1 & func, double eps );
double fmax ( double a, double b, const MathFunc1 & func, double eps );


//************************* 02.03.2017 **************************************//
//
//          ����� ��������� �� ��������� ������� ���� x*x + a*x + b
//
//************************* 02.03.2017 **************************************//

Def<double> getMinMaxParabola1d ( CArrRef<Set2<double> > arr );
