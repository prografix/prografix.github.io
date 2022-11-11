
#pragma once

#include "Vector2d.h"
#include "ShevArray.h"


//**************************** 29.12.2007 *********************************//
//
//         ������ ���� 2 ������� ( �����, ��������, �������������� )
//
//**************************** 25.09.2010 *********************************//

Def<Vector2d> centerPnt ( CCArrRef<Vector2d> & point );
Def<Vector2d> centerPnt ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass );
Def<Vector2d> centerSgm ( CCArrRef<Segment2d> & segm );
Def<Vector2d> centerPlg ( CCArrRef<Vector2d>  & vert );


//**************************** 01.10.2022 *********************************//
//
//         ������ ���� 4 ������� ( �����, ��������, �������������� )
//
//**************************** 01.10.2022 *********************************//

Def<Vector2d> center4pnt ( CCArrRef<Vector2d> & point );


//**************************** 02.01.2008 *********************************//
//
//     ���������� ����������� ������� 1-�� ������� ������������ ������
//
//**************************** 15.10.2022 *********************************//

double momentum1pnt ( CCArrRef<Vector2d> & point, const Line2d & line );
double momentum1pnt ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass, const Line2d & line );
double momentum1sgm ( CCArrRef<Segment2d> & segm, const Line2d & line );


//**************************** 29.12.2007 *********************************//
//
//       ���������� ������� 2-�� ������� ������������ ������
//
//**************************** 25.09.2010 *********************************//

double momentum2pnt ( CCArrRef<Vector2d> & point, const Line2d & line );
double momentum2pnt ( CCArrRef<Vector2d> & point, CCArrRef<double> & mass, const Line2d & line );
double momentum2sgm ( CCArrRef<Segment2d> & segm, const Line2d & line );
double momentum2plg ( CCArrRef<Vector2d>  & vert, const Line2d & line );


//**************************** 29.12.2007 *********************************//
//
//       ���������� ������������ � ������������� ������� 2-�� �������
//
//**************************** 26.09.2010 *********************************//

struct Mom2d
{
    Vector2d minA, maxA; // ���
    double minM, maxM; // ������� 2-�� �������
    Vector2d o; // ����� ����
};

Def<Mom2d> momentum2pnt ( CArrRef<Vector2d> point );
Def<Mom2d> momentum2pnt ( CArrRef<Vector2d> point, CArrRef<double> mass );
Def<Mom2d> momentum2sgm ( CArrRef<Segment2d> segm );
Def<Mom2d> momentum2plg ( CArrRef<Vector2d> vert );

Mom2d momentum2plg ( CArrRef<Vector2d> vert, const Vector2d & o );


//**************************** 25.09.2009 *********************************//
//
//          ���������� ������� �� �������� 2-�� �������
//
//**************************** 26.09.2010 *********************************//

Def<Ellipse2d> getEllipse ( const Def<Mom2d> & mom );


//**************************** 25.09.2009 *********************************//
//
//          ���������� �������������� �� �������� 2-�� �������
//
//**************************** 26.09.2010 *********************************//

Def<Rectangle2d> getRectangle ( const Def<Mom2d> & mom );
