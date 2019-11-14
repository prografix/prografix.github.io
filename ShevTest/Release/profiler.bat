del prof.txt
rem "C:\Program Files\Microsoft Visual Studio\VC98\Bin\prep.exe" /OM /AT /STACK 2 /SF ?cutTest@@YAXAAV?$S2D_Model@$08@@AAV?$S2D_ModelStor@$08@@@Z ShevTest.exe
"C:\Program Files\Microsoft Visual Studio\VC98\Bin\prep.exe" /OM /FT /SF ?cutTest@@YAXAAV?$S2D_Model@$08@@AAV?$S2D_ModelStor@$08@@@Z ShevTest.exe
"C:\Program Files\Microsoft Visual Studio\VC98\Bin\profile" /I ShevTest.pbi ShevTest.exe
"C:\Program Files\Microsoft Visual Studio\VC98\Bin\prep.exe" /IO ShevTest.pbo /IT ShevTest.pbt /OT ShevTest.pbt
"C:\Program Files\Microsoft Visual Studio\VC98\Bin\plist" ShevTest.pbt >> prof.txt
