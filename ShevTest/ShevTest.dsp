# Microsoft Developer Studio Project File - Name="ShevTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ShevTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ShevTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ShevTest.mak" CFG="ShevTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ShevTest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ShevTest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/sad/ShevTest", YFCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ShevTest - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FD /c
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

!ELSEIF  "$(CFG)" == "ShevTest - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FD /GZ /c
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

# Name "ShevTest - Win32 Release"
# Name "ShevTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DisplayPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\draw.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ShevTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ShevTest.rc
# End Source File
# Begin Source File

SOURCE=.\ShevTestDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ShevTestView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\draw.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShevTest.h
# End Source File
# Begin Source File

SOURCE=.\ShevTestDoc.h
# End Source File
# Begin Source File

SOURCE=.\ShevTestView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ShevTest.ico
# End Source File
# Begin Source File

SOURCE=.\res\ShevTest.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ShevTestDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Shev h"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Shev\acoder.h
# End Source File
# Begin Source File

SOURCE=.\Shev\approx.h
# End Source File
# Begin Source File

SOURCE=.\Shev\approx2d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\approx3d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\atrian.h
# End Source File
# Begin Source File

SOURCE=.\Shev\AVL_Tree.h
# End Source File
# Begin Source File

SOURCE=.\Shev\CMatrix.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Complex.h
# End Source File
# Begin Source File

SOURCE=.\Shev\DoubleN.h
# End Source File
# Begin Source File

SOURCE=.\Shev\file.h
# End Source File
# Begin Source File

SOURCE=.\Shev\fileBMP.h
# End Source File
# Begin Source File

SOURCE=.\Shev\fileOBJ.h
# End Source File
# Begin Source File

SOURCE=.\Shev\filePYH.h
# End Source File
# Begin Source File

SOURCE=.\Shev\fileSTL.h
# End Source File
# Begin Source File

SOURCE=.\Shev\func1d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\func1t.h
# End Source File
# Begin Source File

SOURCE=.\Shev\func2d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\func2t.h
# End Source File
# Begin Source File

SOURCE=.\Shev\func3d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\heap.h
# End Source File
# Begin Source File

SOURCE=.\Shev\HeapAndTree.h
# End Source File
# Begin Source File

SOURCE=.\Shev\intersect2d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\intersect3d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\LinAlg.h
# End Source File
# Begin Source File

SOURCE=.\Shev\lists.h
# End Source File
# Begin Source File

SOURCE=.\Shev\LZH.H
# End Source File
# Begin Source File

SOURCE=.\Shev\MarchingCubes.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Mathem.h
# End Source File
# Begin Source File

SOURCE=.\Shev\MathFunc.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Shev\median.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Micro.h
# End Source File
# Begin Source File

SOURCE=.\Shev\moment2d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\moment3d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\opti1d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\opti2d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\opti2d_3d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\opti3d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Poly3gon.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Polyhedron.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Polyhedron_Poly3gon.h
# End Source File
# Begin Source File

SOURCE=.\Shev\PseudoFile.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Rand.h
# End Source File
# Begin Source File

SOURCE=.\Shev\RealFile.h
# End Source File
# Begin Source File

SOURCE=.\Shev\S2D_Model.h
# End Source File
# Begin Source File

SOURCE=.\Shev\SemiRib.h
# End Source File
# Begin Source File

SOURCE=.\Shev\ShevArray.h
# End Source File
# Begin Source File

SOURCE=.\Shev\ShevArray2.h
# End Source File
# Begin Source File

SOURCE=.\Shev\ShevList.h
# End Source File
# Begin Source File

SOURCE=.\Shev\sorting.h
# End Source File
# Begin Source File

SOURCE=.\Shev\StringFile.h
# End Source File
# Begin Source File

SOURCE=.\Shev\swdict.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Template.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Timer.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Trans.h
# End Source File
# Begin Source File

SOURCE=.\Shev\trian2d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\trian2t.h
# End Source File
# Begin Source File

SOURCE=.\Shev\trian3d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\TUNE.H
# End Source File
# Begin Source File

SOURCE=.\Shev\typedef.h
# End Source File
# Begin Source File

SOURCE=.\Shev\ValueSet.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector2d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector2f.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector2i.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector3d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector3f.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector4d.h
# End Source File
# Begin Source File

SOURCE=.\Shev\Vert.h
# End Source File
# Begin Source File

SOURCE=.\Shev\VertFunc.h
# End Source File
# Begin Source File

SOURCE=.\Shev\WireModel.h
# End Source File
# End Group
# Begin Group "Shev cpp"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Shev\acoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\approx.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\approx2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\approx3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\asc.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\CMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Complex.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\File.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\fileBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\fileOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\filePYH.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\fileSTL.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\func1d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\func2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\func3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\intersect2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\intersect3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\ioSTL.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\LZH.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\MarchingCubes.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Mathem.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Micro.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\moment2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\moment3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\opti1d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\opti2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\opti2d_2.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\opti2d_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\opti2d_4.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\opti3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\opti3d_2.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Poly3gon.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Polyhedron.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Polyhedron_Poly3gon.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\printf.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\PseudoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Rand.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\RealFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\ShevList.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\swdict.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\TRANS.CPP
# End Source File
# Begin Source File

SOURCE=.\Shev\trian2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\trian3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\ValueSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector2f.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector3f.cpp
# End Source File
# Begin Source File

SOURCE=.\Shev\Vector4d.cpp
# End Source File
# End Group
# Begin Group "Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Test\approx2d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\approx3d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\approx_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\array_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\calc_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\complex_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\compress_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\Display.h
# End Source File
# Begin Source File

SOURCE=.\Test\file_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\filePYH_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\func1d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\func2d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\func3d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\func4d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\intersect2d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\intersect3d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\LinAlg_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\math_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\matrix_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\moment2d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\opti1d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\opti2d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\opti3d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\polyhedron_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\PolyhedronErrorRecipient.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\PolyhedronErrorRecipient.h
# End Source File
# Begin Source File

SOURCE=.\Test\rand_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\recon_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\S2D_Model_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\ShevList_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\tree_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\trian2d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\trian3d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\Vector2d_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test\Vector3d_test.cpp
# End Source File
# End Group
# Begin Group "Extra"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Extra\BODY2.CPP
# End Source File
# Begin Source File

SOURCE=.\Extra\Body2.h
# End Source File
# Begin Source File

SOURCE=.\Extra\Body2Func.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\Body2Func.h
# End Source File
# Begin Source File

SOURCE=.\Extra\ex_intersect3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\ex_intersect3d.h
# End Source File
# Begin Source File

SOURCE=.\Extra\ex_opti3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\ExtraBody.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\ExtraBody.h
# End Source File
# Begin Source File

SOURCE=.\Extra\ExtraBodyPolyhedron.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\ExtraBodyPolyhedron.h
# End Source File
# Begin Source File

SOURCE=.\Extra\filePYH_Body.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\filePYH_Body.h
# End Source File
# Begin Source File

SOURCE=.\Extra\filePYH_ExtraBody.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\filePYH_ExtraBody.h
# End Source File
# Begin Source File

SOURCE=.\Extra\KONTUR.CPP
# End Source File
# Begin Source File

SOURCE=.\Extra\maxConvPolyInPolyNR3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\PolyhedronDoctor.cpp
# End Source File
# Begin Source File

SOURCE=.\Extra\PolyhedronDoctor.h
# End Source File
# Begin Source File

SOURCE=.\Extra\TRANS3.CPP
# End Source File
# Begin Source File

SOURCE=.\Extra\Vert.h
# End Source File
# End Group
# End Target
# End Project
