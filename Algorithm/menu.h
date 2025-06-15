
#include <windows.h>
#include <conio.h>
#include <limits>
#ifndef MENU_H
#define MENU_H
#include <filesystem> // C++17
#include<string>
#include<vector>
namespace fs=std::filesystem;
// 定义常用颜色（前景色）
enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,// 青色
    Red = 4,
    Magenta = 5,// 紫色
    Yellow = 6,
    White = 7,
    Gray = 8,
    BrightBlue = 9,
    BrightGreen = 10,
    BrightCyan = 11,
    BrightRed = 12,
    BrightMagenta = 13,// 亮紫色
    BrightYellow = 14,
    BrightWhite = 15
};
// 设置控制台文本颜色（前景色和背景色）
void SetColor(int foreground, int background = Black) ;
// 重置控制台颜色（恢复默认颜色）
void ResetColor() ;
// 设置控制台光标位置
void setCursorPos(int x, int y);
// 获取控制台当前光标位置
COORD getCursorPos() ;
// 获取目录下所有.csv文件
std::vector<std::string> getCSVFiles(const std::string &dirPath);

int showMenu(const std::vector<std::string> &options);

int getValidInt() ;

void iscom(int op=-1);

#endif // MENU_H