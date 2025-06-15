@echo off
color 0A
title 华东师范大学校园导航系统

echo 校园之旅正式开始...

echo ...
echo ...
echo ...
pause
color 06

call navigate\Scripts\activate

cd Algorithm

python setup.py build

python setup.py install

pause
