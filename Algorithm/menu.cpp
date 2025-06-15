#include"menu.h"
#include<iostream>

void SetColor(int foreground, int background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, foreground | (background << 4));
}
// 恢复默认颜色（灰色文字，黑色背景）
void ResetColor() {
    SetColor(Gray, Black);
}

// 设置控制台光标位置
void setCursorPos(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 获取控制台当前光标位置
COORD getCursorPos() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition;
}

// 获取目录下所有.csv文件
std::vector<std::string> getCSVFiles(const std::string &dirPath)
{
    std::cout << "当前工作目录: " << std::filesystem::current_path()<<dirPath << std::endl;
    std::vector<std::string> csvFiles;
    for (const auto &entry : fs::directory_iterator(dirPath))
    {
        if (entry.path().extension() == ".csv")
        {
            csvFiles.push_back(entry.path().filename().string());
        }
    }
    return csvFiles;
}

int showMenu(const std::vector<std::string> &options){
    int selected = 0;
    bool confirmed = false;
    COORD originalPos = getCursorPos(); // 保存原始光标位置

    while (!confirmed)
    {
        // 回到菜单开始位置重绘
        setCursorPos(originalPos.X, originalPos.Y);
        int opsize=options.size();
        // 打印所有选项
        for (int i = 0; i < opsize; i++)
        {
            if (i == selected)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_GREEN | FOREGROUND_INTENSITY); // 绿色高亮选中项
                std::cout << "> " << options[i] << std::endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 恢复默认颜色
            }
            else
            {
                std::cout << "  " << options[i] << std::endl;
            }
        }

        // 获取按键输入
        int ch = _getch();
        if (ch == 0 || ch == 0xE0)
        {                  // 功能键或方向键的第一个字节
            ch = _getch(); // 获取第二个字节
            switch (ch)
            {
            case 72: // 上箭头
                selected = (selected > 0) ? selected - 1 : options.size() - 1;
                break;
            case 80:
                // 下箭头
                int tmp = options.size() - 1;
                selected = (selected < tmp) ? selected + 1 : 0;
                break;
            }
        }
        else if (ch == 13)
        { // Enter键
            confirmed = true;
        }
        else if (ch == 27)
        {              // ESC键
            return -1; // 取消选择
        }
    }
    return selected;
}
int getValidInt() {
    int value;
    while (true) {
        if (std::cin >> value) { // 输入成功
            if(value<0)throw std::invalid_argument("输入类型必须为非负整数！");
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("输入类型必须为非负整数！");
    }
    return value;
}
void iscom(int op)
{
    SetColor(Yellow);
    //system("pause");
    std::cout << "按任意键继续..." << std::endl;
    _getch();
    ResetColor();
    if (op==1)
        system("cls");
}
