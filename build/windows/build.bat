@echo off

cd ./../../
echo Building...
build\premake\premake5.exe vs2022
CHOICE /M "Retry?"

if %errorlevel%==2 goto:EOF
cd build/windows/
call build.bat