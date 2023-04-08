
#pragma once

#include "ShevArray.h"


class Line2d;
class Circle2d;
class Vector2d;
class Rhombus2d;
class Ellipse2d;
class Segment2d;
class Conform2d;
class Triangle2d;
class Rectangle2d;
class Parallelogram2d;

////////////////////////////////// ������� ////////////////////////////////////

double getArea ( const Circle2d        & );
double getArea ( const Ellipse2d       & );
double getArea ( const Triangle2d      & );
double getArea ( const Rectangle2d     & );
double getArea ( const Rhombus2d       & );
double getArea ( const Parallelogram2d & );


///////////////////////////////// ��������� ///////////////////////////////////

double getPerimeter ( const Circle2d        & );
double getPerimeter ( const Ellipse2d       & );
double getPerimeter ( const Triangle2d      & );
double getPerimeter ( const Rectangle2d     & );
double getPerimeter ( const Rhombus2d       & );
double getPerimeter ( const Parallelogram2d & );


//**************************** 19.09.2021 *********************************//
//
//            ���������� ������ �� �������� ��������������
//
//**************************** 19.09.2021 *********************************//

bool points2lines ( CCArrRef<Vector2d> & poly, ArrRef<Line2d> & line );


//**************************** 17.10.2008 *********************************//
//
//            ����������� ��������� ��������� �����
//
//**************************** 21.08.2011 *********************************//

Def<Segment2d> dimensions ( CCArrRef<Vector2d> & point );


//**************************** 25.02.2018 *********************************//
//
//            ����� ���� ��������� ����� � ����� ���������
//
//**************************** 25.02.2018 *********************************//

double closestPoints ( CCArrRef<Vector2d> & point, Set2<nat> & pp );


//**************************** 17.10.2008 *********************************//
//
//        �������������� �������������� � ������������ ���������
//
//**************************** 21.08.2011 *********************************//

Def<Conform2d> standardPosition ( const Def<Segment2d> & seg );

Def<Conform2d> setStandardPosition ( ArrRef<Vector2d> & points );


//**************************** 08.10.2018 *********************************//
//
//          ����� ������� ����� ����� ��������� �����������
//
//**************************** 08.10.2018 *********************************//

Def<Vector2d> getFarthestPoint ( CCArrRef<Vector2d> & ar, const Vector2d & dir );


//**************************** 22.10.2008 *********************************//
//
//          ������� ��������� ����� ����� ��������� �����������
//
//**************************** 03.12.2008 *********************************//

double diameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, nat & imin, nat & imax );

double diameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir );


//**************************** 07.02.2009 *********************************//
//
//  ����������� ������� ��������� ����� ����� ��������� ������� �����������.
//  ������ ������� ������� ������������ dir � ���������� ����� � �������� angle.
//  ����� �������� � ���� ������������� ��������, ������������ ����������� res
//  � ���� �������� �������� ����� imin � imax.
//
//**************************** 07.02.2009 *********************************//

double minDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps, Vector2d & res, nat & imin, nat & imax );

double minDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps, Vector2d & res );

double minDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps );


//**************************** 03.03.2009 *********************************//
//
//  ������������ ������� ��������� ����� ����� ��������� ������� �����������.
//  ������ ������� ������� ������������ dir � ���������� ����� � �������� angle.
//  ����� �������� � ���� ������������� ��������, ������������ ����������� res
//  � ���� �������� �������� ����� imin � imax.
//
//**************************** 03.03.2009 *********************************//

double maxDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps, Vector2d & res, nat & imin, nat & imax );

double maxDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps, Vector2d & res );

double maxDiameterPnt ( CArrRef<Vector2d> point, const Vector2d & dir, double angle,
                        double eps );


//*************************************************************************//
//
//                      ������� �������������� 
//
//**************************** 25.09.2010 *********************************//

double area ( CCArrRef<Vector2d> & vert );


//*************************************************************************//
//
//                      �������� ��������������
//
//**************************** 25.09.2010 *********************************//

double perimeter ( CCArrRef<Vector2d> & vert );


//**************************** 14.08.2006 *********************************//
//
//      ������������ ������� ��������� �������������� �� ����� O ( n )
//
//**************************** 02.03.2012 *********************************//

double maxConvexPolygonDiameter ( CArrRef<Vector2d> vert, nat & ix1, nat & ix2 );

double maxConvexPolygonDiameter ( CArrRef<Vector2d> vert );


//**************************** 29.03.2008 *********************************//
//
//      ����������� ������� ��������� �������������� �� ����� O ( n ).
//
//**************************** 02.03.2012 *********************************//

double minConvexPolygonDiameter ( CArrRef<Vector2d> vert, Vector2d & dir, nat & i1, nat & i2 );

double minConvexPolygonDiameter ( CArrRef<Vector2d> vert, Vector2d & dir );

double minConvexPolygonDiameter ( CArrRef<Vector2d> vert );


//**************************** 04.06.2008 *********************************//
//
//              �������� �������������� �� ����������
//
//**************************** 25.09.2010 *********************************//

bool isConvex ( CArrRef<Vector2d> vert );


//**************************** 03.11.2007 *********************************//
//
//              ���������� �������� ��� ������ �������
//
//**************************** 25.09.2010 *********************************//

int loopNumber ( CArrRef<Vector2d> vert );


//**************************** 29.04.2017 *********************************//
//
//          ���������� �� ����� point �� ���������� circle
//          ( �������������, ���� ����� ������ ���������� )
//
//**************************** 29.04.2017 *********************************//

double getDistance ( const Circle2d & circle, const Vector2d & point );


//**************************** 31.10.2008 *********************************//
//
//    ���������� �� ����� p �� ������� ( x/a )^2 + ( y/b )^2 = 1
//    ( �������������, ���� ����� ������ ������� )
//    r - ��� ��������� ����� �� ������� �������
//
//**************************** 29.04.2017 *********************************//

double getDistanceElp ( double a, double b, const Vector2d & p, Vector2d & r );

double getDistanceElp ( double a, double b, const Vector2d & p );


//**************************** 18.08.2004 *********************************//
//
//              ���������� �� ����� �� ��������������
//              index - ��� ����� ��������� �������
//              ��� ����� � ����������� �� isBone
//
//**************************** 02.03.2012 *********************************//

bool distance ( CArrRef<Vector2d> vert, const Vector2d & point,
                double & dist, nat & index, bool & isBone );

bool distance ( CArrRef<Vector2d> vert, const Vector2d & point,
                double & dist, nat & index );

bool distance ( CArrRef<Vector2d> vert, const Vector2d & point, double & dist );


//**************************** 09.10.2004 *********************************//
//
//                  �������������� ����� ����� ��� �.
//
//**************************** 14.08.2006 *********************************//

void mergeSort123 ( nat n, const Vector2d ** a, const Vector2d ** b );


//**************************** 09.10.2004 *********************************//
//
//      �������� �������� ����� �� ��������� �� ����� O ( n log n ).
//
//**************************** 21.08.2006 *********************************//

SuiteRef<  nat   > & convexNlogN ( CArrRef<Vector2d> vert, SuiteRef<  nat   > & res );
SuiteRef<Vector2d> & convexNlogN ( CArrRef<Vector2d> vert, SuiteRef<Vector2d> & res );


//**************************** 19.07.2003 *********************************//
//
//      �������� �������� ����� �� ��������� �� ����� O ( m * n ).
//
//**************************** 17.12.2008 *********************************//

SuiteRef<Vector2d> & convexMN ( SuiteRef<Vector2d> & point );


//**************************** 14.08.2006 *********************************//
//
//      �������� �������� ������ �������������� �� ����� O ( n ).
//      ����� ������ ������ ������� �������, ����� ���������� convexNlogN.
//      vert � res ����� ���������.
//
//**************************** 12.11.2020 *********************************//

SuiteRef<nat> & convexPolygon ( CCArrRef<Vector2d> & vert, SuiteRef<nat> & index );

SuiteRef<Vector2d> & convexPolygon ( CCArrRef<Vector2d> & vert, SuiteRef<Vector2d> & res );


//**************************** 23.08.2003 *********************************//
//
//      ���������� ����������� �� ��� ������.
//      o - ����� ����������, r2 - ������� �������.
//
//**************************** 02.02.2008 *********************************//

bool circlePPP ( const Vector2d & a, const Vector2d & b, const Vector2d & c, Vector2d & o, double & r2 );

Def<Circle2d> circlePPP ( const Vector2d & a, const Vector2d & b, const Vector2d & c );


//**************************** 02.02.2008 *********************************//
//
//      ���������� ����������� �� ���� ������ � ����� ������.
//      ������������ �������� - ���������� �����������.
//      o[2] - ������ ����������, r[2] - �������.
//
//**************************** 25.05.2008 *********************************//

nat circlePPL ( const Vector2d & point1, const Vector2d & point2, const Line2d & line, Vector2d o[2], double r[2] );


//**************************** 02.02.2008 *********************************//
//
//      ���������� ����������� �� ����� ����� � ���� ������.
//      ������������ �������� - ���������� �����������.
//      o[2] - ������ ����������, r[2] - �������.
//
//**************************** 04.01.2012 *********************************//

nat circlePLL ( const Vector2d & point, const Line2d & line1, const Line2d & line2, Vector2d o[2], double r[2] );


//**************************** 02.02.2008 *********************************//
//
//      ���������� ����������� �� ��� ������.
//      o - ����� ����������, r - ������.
//
//**************************** 02.02.2008 *********************************//

bool circleLLL ( const Line2d & line0, const Line2d & line1, const Line2d & line2, Vector2d & o, double & r );

Def<Circle2d> circleLLL ( const Line2d & line0, const Line2d & line1, const Line2d & line2 );


//**************************** 03.10.2009 *********************************//
//
//                  ���������� ������ ��������������
//
//**************************** 26.09.2010 *********************************//

FixArrRef<Vector2d, 4> & makePolygon ( const Rectangle2d & fig, FixArrRef<Vector2d, 4> & vert );


//**************************** 20.10.2015 *********************************//
//
//                      ���������� ������ �����
//
//**************************** 20.10.2015 *********************************//

FixArrRef<Vector2d, 4> & makePolygon ( const Rhombus2d & fig, FixArrRef<Vector2d, 4> & vert );


//**************************** 14.02.2018 *********************************//
//
//                   ���������� ������ ���������������
//
//**************************** 214.02.2018 *********************************//

FixArrRef<Vector2d, 4> & makePolygon ( const Parallelogram2d & fig, FixArrRef<Vector2d, 4> & vert );


//**************************** 17.12.2008 *********************************//
//
//              ��������� �������� �������������
//
//**************************** 05.10.2009 *********************************//

SuiteRef<Vector2d> & randConvexPolygon ( SuiteRef<Vector2d> & poly );


//**************************** 17.12.2008 *********************************//
//
//                      ��������� �������������
//
//**************************** 05.10.2009 *********************************//

ArrRef<Vector2d> randPolygon ( ArrRef<Vector2d> poly );


//**************************** 13.11.2011 *********************************//
//
//       ���������� ������������� ( r - ������ ��������� ���������� )
//
//**************************** 13.11.2011 *********************************//

ArrRef<Vector2d> regularPolygon ( ArrRef<Vector2d> poly, double r = 1 );


//**************************** 25.06.2013 *********************************//
//
//                      ��������� ��������������
//                  poly � res ������ ���� �������
//
//**************************** 06.07.2013 *********************************//

DynArrRef<  nat   > & simplify ( CCArrRef<Vector2d> & poly, double eps, bool closed, DynArrRef<  nat   > & res );
DynArrRef<Vector2d> & simplify ( CCArrRef<Vector2d> & poly, double eps, bool closed, DynArrRef<Vector2d> & res );


//**************************** 05.10.2020 *********************************//
//
//           ��������� �������������� �� ��������� �-�� ������
//                  poly � res ������ ���� �������
//
//**************************** 05.10.2020 *********************************//

DynArrRef<  nat   > & simplifyNV ( CCArrRef<Vector2d> & poly, nat nv, bool closed, DynArrRef<  nat   > & res );
DynArrRef<Vector2d> & simplifyNV ( CCArrRef<Vector2d> & poly, nat nv, bool closed, DynArrRef<Vector2d> & res );


//**************************** 07.04.2023 *********************************//
//
//           ���������� �������������� �� ������ �����������
//
//**************************** 07.04.2023 *********************************//

DynArrRef<Vector2d> & makePolygon ( CCArrRef<Line2d> & line, const double eps, DynArrRef<Vector2d> & poly );
