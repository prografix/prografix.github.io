; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "demo.h"
LastPage=0

ClassCount=20
Class1=ApproxCircle
Class2=Trian2d
Class3=Trian3d
Class4=CDEMOApp
Class5=CAboutDlg
Class6=DemoApprox3d
Class7=CDEMODoc
Class8=DemoIntersect2d
Class9=DemoIntersect3d
Class10=CDEMOView
Class11=LineX2d
Class12=CMainFrame
Class13=MaxCircle
Class14=MaxSphere
Class15=Min2d
Class16=Min3d
Class17=RandomVec
Class18=ScenePar

ResourceCount=18
Resource1=IDD_ABOUTBOX (English (U.S.))
Resource2=IDD_TRIAN_2D
Resource3=IDD_SCENE_PAR (English (U.S.))
Resource4=IDD_INTERSECT_3D
Resource5=IDD_LINES_X_2D
Resource6=IDD_RANDOM_VEC
Resource7=IDD_OTHER
Resource8=IDD_POINT_2D
Resource9=IDD_MAX_2D3D
Resource10=IDD_TRIAN_3D
Resource11=IDD_MIN_3D
Resource12=IDD_MIN_2D
Resource13=IDD_INTERSECT_2D
Resource14=IDD_APPROX_3D
Resource15=IDD_MAX_3D
Resource16=IDD_MAX_2D
Class19=DemoOther
Class20=Max2d3d
Resource17=IDD_APPROX_2D
Resource18=IDR_MAINFRAME (English (U.S.))

[CLS:ApproxCircle]
Type=0
BaseClass=CDialog
HeaderFile=ApproxCircle.h
ImplementationFile=ApproxCircle.cpp
LastObject=ID_DEMO_OTHER

[CLS:Trian2d]
Type=0
BaseClass=CDialog
HeaderFile=CTrian2d.h
ImplementationFile=CTrian2d.cpp

[CLS:Trian3d]
Type=0
BaseClass=CDialog
HeaderFile=CTrian3d.h
ImplementationFile=CTrian3d.cpp

[CLS:CDEMOApp]
Type=0
BaseClass=CWinApp
HeaderFile=DEMO.h
ImplementationFile=DEMO.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=DEMO.cpp
ImplementationFile=DEMO.cpp

[CLS:DemoApprox3d]
Type=0
BaseClass=CDialog
HeaderFile=DemoApprox3d.h
ImplementationFile=DemoApprox3d.cpp

[CLS:CDEMODoc]
Type=0
BaseClass=CDocument
HeaderFile=DEMODoc.h
ImplementationFile=DEMODoc.cpp

[CLS:DemoIntersect2d]
Type=0
BaseClass=CDialog
HeaderFile=DemoIntersect2d.h
ImplementationFile=DemoIntersect2d.cpp

[CLS:DemoIntersect3d]
Type=0
BaseClass=CDialog
HeaderFile=DemoIntersect3d.h
ImplementationFile=DemoIntersect3d.cpp

[CLS:CDEMOView]
Type=0
BaseClass=CView
HeaderFile=DEMOView.h
ImplementationFile=DEMOView.cpp

[CLS:LineX2d]
Type=0
BaseClass=CDialog
HeaderFile=LineX2d.h
ImplementationFile=LineX2d.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=ID_DEMO_MAXI_2D_3D

[CLS:MaxCircle]
Type=0
BaseClass=CDialog
HeaderFile=MaxCircle.h
ImplementationFile=MaxCircle.cpp
LastObject=ID_APP_ABOUT
Filter=D
VirtualFilter=dWC

[CLS:MaxSphere]
Type=0
BaseClass=CDialog
HeaderFile=MaxSphere.h
ImplementationFile=MaxSphere.cpp

[CLS:Min2d]
Type=0
BaseClass=CDialog
HeaderFile=Min2d.h
ImplementationFile=Min2d.cpp

[CLS:Min3d]
Type=0
BaseClass=CDialog
HeaderFile=Min3d.h
ImplementationFile=Min3d.cpp

[CLS:RandomVec]
Type=0
BaseClass=CDialog
HeaderFile=RandomVec.h
ImplementationFile=RandomVec.cpp
LastObject=ID_APP_ABOUT
Filter=D
VirtualFilter=dWC

[CLS:ScenePar]
Type=0
BaseClass=CDialog
HeaderFile=ScenePar.h
ImplementationFile=ScenePar.cpp

[DLG:IDD_APPROX_2D]
Type=1
Class=ApproxCircle
ControlCount=4
Control1=IDC_POINTS_NUM,combobox,1344339971
Control2=IDC_STATIC,static,1342308352
Control3=IDC_NEXT_POINTS,button,1342242816
Control4=IDC_COMBO_VAR,combobox,1344339971

[DLG:IDD_TRIAN_2D]
Type=1
Class=Trian2d
ControlCount=8
Control1=IDC_POLYGON_SIZE,combobox,1344339971
Control2=IDC_STATIC,static,1342308352
Control3=IDC_NEXT_POLYGON,button,1342242816
Control4=IDC_TRIAN_TYPE,combobox,1344339971
Control5=IDC_STATIC,static,1342308352
Control6=IDC_TRIANGULATE,button,1342242816
Control7=IDC_CHECK_GLOBAL,button,1342242851
Control8=IDC_CONVEX_PARTS,button,1342242816

[DLG:IDD_TRIAN_3D]
Type=1
Class=Trian3d
ControlCount=5
Control1=ID_RUN,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_N_VERTEX,edit,1350639744
Control4=65535,static,1342308352
Control5=IDC_COMBO_VAR,combobox,1344339971

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_APPROX_3D]
Type=1
Class=DemoApprox3d
ControlCount=4
Control1=IDC_POINTS_NUM,combobox,1344339971
Control2=IDC_STATIC,static,1342308352
Control3=IDC_NEXT_POINTS,button,1342242816
Control4=IDC_COMBO_VAR,combobox,1344339971

[DLG:IDD_INTERSECT_2D]
Type=1
Class=DemoIntersect2d
ControlCount=10
Control1=ID_NEXT_1,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_1_NV,combobox,1344339971
Control4=IDC_STATIC,button,1342177287
Control5=IDC_COMBO_1,combobox,1344339971
Control6=ID_NEXT_2,button,1342242817
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_2_NV,combobox,1344339971
Control9=IDC_STATIC,button,1342177287
Control10=IDC_COMBO_2,combobox,1344339971

[DLG:IDD_INTERSECT_3D]
Type=1
Class=DemoIntersect3d
ControlCount=6
Control1=ID_NEXT_1,button,1342242817
Control2=65535,button,1342177287
Control3=IDC_COMBO_1,combobox,1344339971
Control4=ID_NEXT_2,button,1342242817
Control5=65535,button,1342177287
Control6=IDC_COMBO_2,combobox,1344339971

[DLG:IDD_LINES_X_2D]
Type=1
Class=LineX2d
ControlCount=7
Control1=IDC_LINES_NUM,combobox,1344339971
Control2=IDC_STATIC,static,1342308352
Control3=IDC_NEXT_LINES,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[DLG:IDD_MAX_2D]
Type=1
Class=MaxCircle
ControlCount=11
Control1=ID_NEXT_OUTER,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_OUTER_NV,combobox,1344339971
Control4=IDC_STATIC,button,1342177287
Control5=IDC_COMBO_OUTER,combobox,1344339971
Control6=ID_NEXT_INNER,button,1342242817
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_INNER_NV,combobox,1344339971
Control9=IDC_STATIC,button,1342177287
Control10=IDC_COMBO_INNER,combobox,1344339971
Control11=IDC_COMBO_ROTATE,combobox,1344339971

[DLG:IDD_MAX_3D]
Type=1
Class=MaxSphere
ControlCount=7
Control1=ID_NEXT_OUTER,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_OUTER_NV,combobox,1344339971
Control4=IDC_STATIC,button,1342177287
Control5=ID_NEXT_INNER,button,1073807361
Control6=IDC_STATIC,button,1342177287
Control7=IDC_COMBO_INNER,combobox,1344339971

[DLG:IDD_MIN_2D]
Type=1
Class=Min2d
ControlCount=10
Control1=ID_NEXT_OUTER,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_OUTER_NV,combobox,1344339971
Control4=IDC_STATIC,button,1342177287
Control5=IDC_COMBO_OUTER,combobox,1344339971
Control6=ID_NEXT_INNER,button,1342242817
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_INNER_NV,combobox,1344339971
Control9=IDC_STATIC,button,1342177287
Control10=IDC_COMBO_INNER,combobox,1344339971

[DLG:IDD_MIN_3D]
Type=1
Class=Min3d
ControlCount=8
Control1=ID_NEXT_OUTER,button,1073741824
Control2=65535,button,1342177287
Control3=IDC_COMBO_OUTER,combobox,1344339971
Control4=ID_NEXT_INNER,button,1342242817
Control5=65535,static,1342308352
Control6=IDC_COMBO_INNER_NV,combobox,1344339971
Control7=65535,button,1342177287
Control8=IDC_COMBO_INNER,combobox,1344339971

[DLG:IDD_RANDOM_VEC]
Type=1
Class=RandomVec
ControlCount=15
Control1=ID_RUN,button,1342242817
Control2=IDC_NVEC,edit,1350639744
Control3=IDC_MAX,edit,1350641792
Control4=IDC_MIN,edit,1350641792
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_RAND,combobox,1344339971
Control9=IDC_AREA_MIN,edit,1350641792
Control10=IDC_AREA_MAX,edit,1350641792
Control11=IDC_EDGE_SIGMA,edit,1350641792
Control12=IDC_AREA_SIGMA,edit,1350641792
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352

[DLG:IDD_SCENE_PAR]
Type=1
Class=ScenePar

[DLG:IDD_OTHER]
Type=1
Class=DemoOther
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBO1,combobox,1344339971

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_SAVE_VIEW
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_DEMO_RANDOM_VEC
Command9=ID_DEMO_TRIAN_2D
Command10=ID_DEMO_APPROX_2D
Command11=ID_DEMO_MAXI_2D
Command12=ID_DEMO_MINI_2D
Command13=ID_DEMO_INTERSECT_2D
Command14=ID_DEMO_LINES_X_2D
Command15=ID_DEMO_MAXI_2D_3D
Command16=ID_DEMO_TRIAN_3D
Command17=ID_DEMO_APPROX_3D
Command18=ID_DEMO_MAXI_3D
Command19=ID_DEMO_MINI_3D
Command20=ID_DEMO_INTERSECT_3D
Command21=ID_DEMO_OTHER
Command22=ID_MODEL_WIRE
Command23=ID_MODEL_FLAT
Command24=ID_MODEL_SMOOTH
Command25=ID_SCENE
Command26=ID_VIEW_TOOLBAR
Command27=ID_APP_ABOUT
CommandCount=27

[DLG:IDD_POINT_2D]
Type=1
Class=?
ControlCount=5
Control1=IDC_POINTS_NUM,combobox,1344339971
Control2=IDC_STATIC,static,1342308352
Control3=IDC_NEXT_POINTS,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=Max2d3d
ControlCount=3
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889

[DLG:IDD_SCENE_PAR (English (U.S.))]
Type=1
Class=?
ControlCount=12
Control1=IDOK,button,1342242816
Control2=65535,static,1342177282
Control3=IDC_SDISTANCE,msctls_trackbar32,1342242816
Control4=IDC_EDISTANCE,edit,1342179458
Control5=65535,static,1342177282
Control6=IDC_SANGLE,msctls_trackbar32,1342242816
Control7=IDC_EANGLE,edit,1342179458
Control8=65535,static,1342177282
Control9=IDC_SMAGNIFY,msctls_trackbar32,1342242817
Control10=IDC_EMAGNIFY,edit,1342179458
Control11=65535,static,1342308352
Control12=IDCANCEL,button,1342242816

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_MOV_XM
Command4=ID_MOV_XP
Command5=ID_ROT_X_CW
Command6=ID_ROT_X_CCW
Command7=ID_MOV_ZP
Command8=ID_MOV_ZM
Command9=ID_ROT_Z_CW
Command10=ID_ROT_Z_CCW
Command11=ID_MOV_YP
Command12=ID_MOV_YM
Command13=ID_ROT_Y_CW
Command14=ID_ROT_Y_CCW
Command15=ID_ZOOM_OUT
Command16=ID_ZOOM_IN
Command17=ID_RESET
Command18=ID_APP_ABOUT
CommandCount=18

[CLS:DemoOther]
Type=0
HeaderFile=DemoOther.h
ImplementationFile=DemoOther.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_DEMO_MAXI_2D
VirtualFilter=dWC

[CLS:Max2d3d]
Type=0
HeaderFile=Max2d3d.h
ImplementationFile=Max2d3d.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_DEMO_MAXI_2D

[DLG:IDD_MAX_2D3D]
Type=1
Class=?
ControlCount=11
Control1=ID_NEXT_OUTER,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_OUTER_NV,combobox,1344339971
Control4=IDC_STATIC,button,1342177287
Control5=IDC_COMBO_OUTER,combobox,1344339971
Control6=ID_NEXT_INNER,button,1073807361
Control7=IDC_STATIC,static,1073872896
Control8=IDC_COMBO_INNER_NV,combobox,1075904515
Control9=IDC_STATIC,button,1342177287
Control10=IDC_COMBO_INNER,combobox,1344339971
Control11=IDC_COMBO_ROTATE,combobox,1075904515

