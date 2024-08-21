@echo off

set CPP_INC=%MG_INCLUDE%
set CPP_FLAGS=/W4 /Zi /EHsc /wd4201 /wd4996 /wd4505 /MP /I %CPP_INC% /I ..\inc /wd4238
set CPP_SRC=..\src\main.cpp
set CPP_LIBS=d3d12.lib dxgi.lib d3dcompiler.lib user32.lib imgui_d3d12.lib

mkdir bin
pushd bin\

cl %CPP_FLAGS% %CPP_SRC% /link /LIBPATH:%MG_LIB% %CPP_LIBS%


for %%f in (..\src\shaders\*.vs) do (
    fxc /Zi /nologo /E:main /T:vs_5_0 %%f /Fo:%%~nf_vs.cso
)

for %%f in (..\src\shaders\*.ps) do (
    fxc /Zi /nologo /E:main /T:ps_5_0 %%f /Fo:%%~nf_ps.cso
)

popd


