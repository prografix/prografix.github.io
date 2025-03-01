
#pragma once

#include "template.h"
#include "ShevList.h"
#include "ShevArray.h"

class Vector2d;

//**************************** 18.07.2007 *********************************//
//
//          ������������ ��������������� ( ��������� ����������� )
//
//**************************** 27.08.2011 *********************************//

SuiteRef<Set3<nat> > & trianNat1L1MaxMinArea    ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );

SuiteRef<Set3<nat> > & trianNat1L1MaxMinAdivR2  ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );

SuiteRef<Set3<nat> > & trianNat1L1MaxSumAdivR2  ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );

SuiteRef<Set3<nat> > & trianNat1L1MaxMinTan     ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );

SuiteRef<Set3<nat> > & trianNat1L1MaxSumMinAngle( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );


//**************************** 18.07.2007 *********************************//
//
//          ������������ ��������������� ( ���������� ����������� )
//
//**************************** 27.08.2011 *********************************//

SuiteRef<Set3<nat> > & trianG1MaxMinArea    ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );

SuiteRef<Set3<nat> > & trianG1MaxMinAdivR2  ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );

SuiteRef<Set3<nat> > & trianG1MaxSumAdivR2  ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );

SuiteRef<Set3<nat> > & trianG1MaxMinTan     ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );

SuiteRef<Set3<nat> > & trianG1MaxSumMinAngle( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );


//**************************** 25.11.2015 *********************************//
//
//            ������������ ��������������� ( ���������� ������ )
//
//**************************** 19.08.2018 *********************************//

SuiteRef<Set3<nat> > & trianSweepLine ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );
SuiteRef<Set3<nat> > & trianSweepLine ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res );


//**************************** 24.08.2018 *********************************//
//
//      ����������� ������������ �� ������������ ������
//
//**************************** 24.08.2018 *********************************//

ArrRef<Set3<nat> > & rebuildDelauney ( CCArrRef<Vector2d> & vert, ArrRef<Set3<nat> > & res );


//**************************** 01.08.2007 *********************************//
//
//      �������� �� ������� ������������ � �������������� ��������������
//
//**************************** 27.08.2011 *********************************//

bool trianTestNat1L1Area   ( CCArrRef<Vector2d> & vert );

bool trianTestNat1L1AdivR2 ( CCArrRef<Vector2d> & vert );

bool trianTestNat1L1MinTan ( CCArrRef<Vector2d> & vert );


//**************************** 07.08.2008 *********************************//
//
//              ��������� �������������� �� �������� �����
//
//**************************** 04.02.2023 *********************************//

void convexParts ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vert, Suite<nat> & cntr2, Suite<nat> & index );
void convexParts ( CCArrRef<Vector2d> & vert, Suite<nat> & cntr, Suite<nat> & index );


//**************************** 16.01.2023 *********************************//
//
//              ���������� �������������� � ������� �� �����
//
//**************************** 16.01.2023 *********************************//

bool splitPolygon ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vert, Suite<nat> & cntr2, Suite<nat> & index );
