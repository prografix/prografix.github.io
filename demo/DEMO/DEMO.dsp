# Microsoft Developer Studio Project File - Name="DEMO" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DEMO - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DEMO.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DEMO.mak" CFG="DEMO - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DEMO - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DEMO - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/sad/DEMO", OJCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DEMO - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /profile /map /machine:I386

!ELSEIF  "$(CFG)" == "DEMO - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DEMO - Win32 Release"
# Name "DEMO - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\approx.cpp
# End Source File
# Begin Source File

SOURCE=.\approx2d.cpp
# End Source File
# Begin Source File

SOURCE=.\approx3d.cpp
# End Source File
# Begin Source File

SOURCE=.\ApproxCircle.cpp
# End Source File
# Begin Source File

SOURCE=.\CTrian2d.cpp
# End Source File
# Begin Source File

SOURCE=.\CTrian3d.cpp
# End Source File
# Begin Source File

SOURCE=.\DEMO.cpp
# End Source File
# Begin Source File

SOURCE=.\DEMO.rc
# End Source File
# Begin Source File

SOURCE=.\DemoApprox3d.cpp
# End Source File
# Begin Source File

SOURCE=.\DEMODoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DemoIntersect2d.cpp
# End Source File
# Begin Source File

SOURCE=.\DemoIntersect3d.cpp
# End Source File
# Begin Source File

SOURCE=.\DemoOther.cpp
# End Source File
# Begin Source File

SOURCE=.\DEMOView.cpp
# End Source File
# Begin Source File

SOURCE=.\draw.cpp
# End Source File
# Begin Source File

SOURCE=.\File.cpp
# End Source File
# Begin Source File

SOURCE=.\fileOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\fileOBJplh.cpp
# End Source File
# Begin Source File

SOURCE=.\filePYH.cpp
# End Source File
# Begin Source File

SOURCE=.\fileSTL.cpp
# End Source File
# Begin Source File

SOURCE=.\func1d.cpp
# End Source File
# Begin Source File

SOURCE=.\func2d.cpp
# End Source File
# Begin Source File

SOURCE=.\func3d.cpp
# End Source File
# Begin Source File

SOURCE=.\intersect2d.cpp
# End Source File
# Begin Source File

SOURCE=.\intersect3d.cpp
# End Source File
# Begin Source File

SOURCE=.\ioOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\ioPYH.cpp
# End Source File
# Begin Source File

SOURCE=.\ioSTL.cpp
# End Source File
# Begin Source File

SOURCE=.\LineX2d.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Mathem.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Max2d3d.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxCircle.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Min2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Min3d.cpp
# End Source File
# Begin Source File

SOURCE=.\moment2d.cpp
# End Source File
# Begin Source File

SOURCE=.\moment3d.cpp
# End Source File
# Begin Source File

SOURCE=.\opti1d.cpp
# End Source File
# Begin Source File

SOURCE=.\opti2d.cpp
# End Source File
# Begin Source File

SOURCE=.\opti2d_2.cpp
# End Source File
# Begin Source File

SOURCE=.\opti2d_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\opti2d_4.cpp
# End Source File
# Begin Source File

SOURCE=.\opti3d.cpp
# End Source File
# Begin Source File

SOURCE=.\opti3d_2.cpp
# End Source File
# Begin Source File

SOURCE=.\Polyhedron.cpp
# End Source File
# Begin Source File

SOURCE=.\printf.cpp
# End Source File
# Begin Source File

SOURCE=.\Rand.cpp
# End Source File
# Begin Source File

SOURCE=.\RandomVec.cpp
# End Source File
# Begin Source File

SOURCE=.\RealFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenePar.cpp
# End Source File
# Begin Source File

SOURCE=.\ShevList.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TRANS.CPP
# End Source File
# Begin Source File

SOURCE=.\Trian2d.cpp
# End Source File
# Begin Source File

SOURCE=.\trian3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector3f.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\approx.h
# End Source File
# Begin Source File

SOURCE=.\approx2d.h
# End Source File
# Begin Source File

SOURCE=.\approx3d.h
# End Source File
# Begin Source File

SOURCE=.\ApproxCircle.h
# End Source File
# Begin Source File

SOURCE=.\atrian.h
# End Source File
# Begin Source File

SOURCE=.\CTrian2d.h
# End Source File
# Begin Source File

SOURCE=.\CTrian3d.h
# End Source File
# Begin Source File

SOURCE=.\DEMO.h
# End Source File
# Begin Source File

SOURCE=.\DemoApprox3d.h
# End Source File
# Begin Source File

SOURCE=.\DEMODoc.h
# End Source File
# Begin Source File

SOURCE=.\DemoIntersect2d.h
# End Source File
# Begin Source File

SOURCE=.\DemoIntersect3d.h
# End Source File
# Begin Source File

SOURCE=.\DemoOther.h
# End Source File
# Begin Source File

SOURCE=.\DEMOView.h
# End Source File
# Begin Source File

SOURCE=.\DoubleN.h
# End Source File
# Begin Source File

SOURCE=.\draw.h
# End Source File
# Begin Source File

SOURCE=.\File.h
# End Source File
# Begin Source File

SOURCE=.\fileOBJ.h
# End Source File
# Begin Source File

SOURCE=.\fileOBJplh.h
# End Source File
# Begin Source File

SOURCE=.\filePYH.h
# End Source File
# Begin Source File

SOURCE=.\fileSTL.h
# End Source File
# Begin Source File

SOURCE=.\func1d.h
# End Source File
# Begin Source File

SOURCE=.\func1t.h
# End Source File
# Begin Source File

SOURCE=.\func2d.h
# End Source File
# Begin Source File

SOURCE=.\func2t.h
# End Source File
# Begin Source File

SOURCE=.\func3d.h
# End Source File
# Begin Source File

SOURCE=.\heap.h
# End Source File
# Begin Source File

SOURCE=.\intersect2d.h
# End Source File
# Begin Source File

SOURCE=.\intersect3d.h
# End Source File
# Begin Source File

SOURCE=.\ioOBJ.h
# End Source File
# Begin Source File

SOURCE=.\LinAlg.h
# End Source File
# Begin Source File

SOURCE=.\LineX2d.h
# End Source File
# Begin Source File

SOURCE=.\lists.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Mathem.h
# End Source File
# Begin Source File

SOURCE=.\MathFunc.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Max2d3d.h
# End Source File
# Begin Source File

SOURCE=.\MaxCircle.h
# End Source File
# Begin Source File

SOURCE=.\MaxSphere.h
# End Source File
# Begin Source File

SOURCE=.\median.h
# End Source File
# Begin Source File

SOURCE=.\Min2d.h
# End Source File
# Begin Source File

SOURCE=.\Min3d.h
# End Source File
# Begin Source File

SOURCE=.\moment2d.h
# End Source File
# Begin Source File

SOURCE=.\moment3d.h
# End Source File
# Begin Source File

SOURCE=.\opti1d.h
# End Source File
# Begin Source File

SOURCE=.\opti2d.h
# End Source File
# Begin Source File

SOURCE=.\opti2d_3d.h
# End Source File
# Begin Source File

SOURCE=.\opti3d.h
# End Source File
# Begin Source File

SOURCE=.\Poly3gon.h
# End Source File
# Begin Source File

SOURCE=.\Polyhedron.h
# End Source File
# Begin Source File

SOURCE=.\Polyhedron_Poly3gon.h
# End Source File
# Begin Source File

SOURCE=.\Rand.h
# End Source File
# Begin Source File

SOURCE=.\RandomVec.h
# End Source File
# Begin Source File

SOURCE=.\RealFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScenePar.h
# End Source File
# Begin Source File

SOURCE=.\SemiRib.h
# End Source File
# Begin Source File

SOURCE=.\ShevArray.h
# End Source File
# Begin Source File

SOURCE=.\ShevArray2.h
# End Source File
# Begin Source File

SOURCE=.\ShevList.h
# End Source File
# Begin Source File

SOURCE=.\sorting.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Template.h
# End Source File
# Begin Source File

SOURCE=.\Trans.h
# End Source File
# Begin Source File

SOURCE=.\Trian2d.h
# End Source File
# Begin Source File

SOURCE=.\trian2t.h
# End Source File
# Begin Source File

SOURCE=.\trian3d.h
# End Source File
# Begin Source File

SOURCE=.\TUNE.H
# End Source File
# Begin Source File

SOURCE=.\typedef.h
# End Source File
# Begin Source File

SOURCE=.\Vector2d.h
# End Source File
# Begin Source File

SOURCE=.\Vector3d.h
# End Source File
# Begin Source File

SOURCE=.\Vector3f.h
# End Source File
# Begin Source File

SOURCE=.\Vector4d.h
# End Source File
# Begin Source File

SOURCE=.\WireModel.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DEMO.ico
# End Source File
# Begin Source File

SOURCE=.\res\DEMO.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DEMODoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# End Target
# End Project
