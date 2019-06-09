@echo off
setlocal enableextensions
setlocal enabledelayedexpansion

set FINAL=/O2 /DNDEBUG /MT
set RELEASE=/Zi /O2 /DNDEBUG /MT
set DEBUG=/Zi /Od /D_DEBUG /MTd
set CONFIG=%RELEASE%
set CFLAGS=%CONFIG% /EHa- /GR- /Gy /Gw /W3 /nologo /I"external" /wd4651
set DXC=dxc.exe /Ges /O3 /WX /nologo
set BUILD_SINGLE=e3bff54e
set SHADER_BEGIN=0
set SHADER_END=-1


for /L %%G in (%SHADER_BEGIN%,1,%SHADER_END%) do (
::for %%G in (0, 1, 13) do (
    if exist data\shaders\%%G.vs.cso del data\shaders\%%G.vs.cso
    if exist data\shaders\%%G.ps.cso del data\shaders\%%G.ps.cso
    %DXC% /D"VS_%%G" /E"VS%%G_Main" /Fo"data\shaders\%%G.vs.cso" /T vs_6_0 master.hlsl & if !ERRORLEVEL! neq 0 goto end
    %DXC% /D"PS_%%G" /E"PS%%G_Main" /Fo"data\shaders\%%G.ps.cso" /T ps_6_0 master.hlsl & if !ERRORLEVEL! neq 0 goto end
)


if not exist library.pch (
    if exist library.lib del library.lib
    cl %CFLAGS% /c /Yc"library.h" library.cpp /Fp"library.pch" /Fo"library.obj" & if !ERRORLEVEL! neq 0 goto end
    lib /nologo library.obj
)
if not exist library_dx12.pch (
    if exist library_dx12.lib del library_dx12.lib
    cl %CFLAGS% /c /D"USE_DX12" /Yc"library.h" library.cpp /Fp"library_dx12.pch" /Fo"library_dx12.obj" & if !ERRORLEVEL! neq 0 goto end
    lib /nologo library_dx12.obj
)


if "%BUILD_SINGLE%"=="" (
    if exist *.exe del *.exe
    for /R %%G in (*.pdb) do if not "%%~nxG"=="vc140.pdb" del %%G
    for /R %%G in (*.cpp) do (
        if not "%%~nxG"=="library.cpp" (
            cl %CFLAGS% /Yu %%G /link /incremental:no /opt:ref & if !ERRORLEVEL! neq 0 goto end
        )
    )
) else (
    if exist %BUILD_SINGLE%.exe del %BUILD_SINGLE%.exe
    if exist %BUILD_SINGLE%.pdb del %BUILD_SINGLE%.pdb
    cl %CFLAGS% /Yu %BUILD_SINGLE%.cpp /link /incremental:no /opt:ref
)

:end
if exist *.obj del *.obj
