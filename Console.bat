@echo off

set ASDISK=d:
set ASPATH=%ASDISK%\repository\as

%ASDISK%
cd %ASPATH%

if NOT EXIST "%ASPATH%\Console.bat" goto perror

set PATH=C:\Python27;C:\msys64\usr\bin;C:\msys64\mingw64\bin;%PATH%

cd release/asboot
start cmd

cd ../..
cd release/ascore
start cmd

cd ../..
set PATH=C:\Anaconda3;%PATH%
cd com/as.tool/as.one.py
cmd

:perror
echo Please fix the var "ASDISK" and "WD" to the right path!
pasue