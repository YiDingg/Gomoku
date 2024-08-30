#include <windows.h> // 解决输出乱码
#include <stdio.h>
#include "Gomoku.h"

#define debug 0

int main() {
    SetConsoleOutputCP(65001); // 设置编码为 UTF-8，解决输出乱码
    Gomoku_Run();
    return 0;
}
