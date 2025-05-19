@echo off

REM Install raylib with vcpkg with
REM vcpkg install raylib
REM
REM Copy dlls to build folder
REM cp .\vcpkg_installed\x64-windows\bin\raylib.dll ..\..\build
REM cp .\vcpkg_installed\x64-windows\bin\glfw3.dll ..\..\build

set CommonCompilerFlags=-MT -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 -FC -Z7
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib

REM TODO - can we just build both with one exe?

IF NOT EXIST ..\..\build mkdir ..\..\build

pushd ..\..\build



REM 32-bit build

REM cl %CommonCompilerFlags% ..\silentbutdeadly\code\win32_silentbutdeadly.cpp /link -subsystem:windows,5.1 %CommonLinkerFlags%

set RaylibPath=..\silentbutdeadly\code\vcpkg_installed\x64-windows\include
set RaylibInclude=-I%RaylibPath%
set RaylibLibPath=..\silentbutdeadly\code\vcpkg_installed\x64-windows\lib


REM Add raylib.lib to linker flags
set RaylibLinkerFlags=..\silentbutdeadly\code\vcpkg_installed\x64-windows\lib\raylib.lib /LIBPATH:%RaylibLibPath%

REM 64-bit build
del *.pdb > NUL 2> NUL




cl %CommonCompilerFlags% %RaylibInclude% ..\silentbutdeadly\code\silentbutdeadly.c -Fmsilentbutdeadly.map -LD /link -incremental:no -opt:ref -PDB:silentbutdeadly_%random%.pdb -IMPLIB:silentbutdeadly.lib

cl %CommonCompilerFlags% %RaylibInclude% ..\silentbutdeadly\code\win32_silentbutdeadly.c -Fmwin32_silentbutdeadly.map /link %CommonLinkerFlags% silentbutdeadly.lib %RaylibLinkerFlags%

popd
