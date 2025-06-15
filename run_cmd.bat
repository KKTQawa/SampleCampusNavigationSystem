@echo off
color 0A
title 华东师范大学校园导航系统

echo 校园之旅正式开始...

echo ...
echo ...
echo ...
pause
color 06

cd Algorithm

cmake ^
    -S . ^
    -B build_cmd ^
    -G Ninja ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER=A:/minc++/mingw64/bin/gcc.exe ^
    -DCMAKE_CXX_COMPILER=A:/minc++/mingw64/bin/g++.exe

cmake --build build_cmd --parallel 12

cd ../

call Algorithm\build_cmd\ecnuNavigation.exe

pause
