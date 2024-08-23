@echo off

set CPP_FLAGS=/W4 /Zi /EHsc /wd4201 /wd4238 /wd4996 /wd4505 /MP /I ..\inc /Fe:"qjulia"
set CPP_SRC=..\src\*.cpp ..\src\imgui\*.cpp
set CPP_LIBS=d3d12.lib dxgi.lib d3dcompiler.lib user32.lib

rmdir bin
mkdir bin
pushd bin\

cl %CPP_FLAGS% %CPP_SRC% /link %CPP_LIBS%

for %%f in (..\src\shaders\*.vs) do (
    dxc /Zi /nologo /E main /T vs_6_0 /Fo %%~nf_vs.cso /Fd %%~nf_vs.pdb %%f
)

for %%f in (..\src\shaders\*.ps) do (
    dxc /Zi /nologo /E main /T ps_6_0 /Fo %%~nf_ps.cso /Fd %%~nf_ps.pdb %%f
)

for %%f in (..\src\shaders\*.cs) do (
    dxc /Zi /nologo /E main /T cs_6_0 /Fo %%~nf_cs.cso /Fd %%~nf_cs.pdb %%f
)

popd

