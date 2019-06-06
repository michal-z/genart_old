@echo off
setlocal enableextensions
setlocal enabledelayedexpansion

set FINAL=/O2 /DNDEBUG /MT
set RELEASE=/Zi /O2 /DNDEBUG /MT
set DEBUG=/Zi /Od /D_DEBUG /MTd

set CONFIG=%DEBUG%
set CFLAGS=%CONFIG% /EHa- /GR- /Gy /Gw /W3 /nologo /I"external" /Fd"external.pdb.pch"
set BUILD_SINGLE=

if not exist external.pch cl %CFLAGS% /c /Yc"external.h" external.cpp /Fo"external.obj.pch"

set ERROR=0
if "%BUILD_SINGLE%"=="" (
	if exist *.exe del *.exe
	if exist *.pdb del *.pdb
	for /R %%G in (*.cpp) do (
		if not "%%G"=="%cd%\external.cpp" if not "%%G"=="%cd%\library.cpp" (
			cl %CFLAGS% /Yu"external.h" %%G /link /incremental:no /opt:ref external.obj.pch & if ERRORLEVEL 1 set ERROR=1
			if !ERROR! neq 0 goto end
		)
	)
) else (
	if exist %BUILD_SINGLE%.exe del %BUILD_SINGLE%.exe
	if exist %BUILD_SINGLE%.pdb del %BUILD_SINGLE%.pdb
	cl %CFLAGS% /Yu"external.h" %BUILD_SINGLE%.cpp /link /incremental:no /opt:ref external.obj.pch
)

:end
if exist *.obj del *.obj
