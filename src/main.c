#include <windows.h> // 解决输出乱码
#include <stdio.h>
#include "Gomoku.h"

#define debug 0

int main() {
    SetConsoleOutputCP(65001); // 设置编码为 UTF-8，解决输出乱码
    Gomoku_Run();
    // FILE* f_GomokuData = fopen("GomokuData.txt", "a+");
    // fprintf(f_GomokuData, "%d%c ", 3 + 1, 0 + 65);
    return 0;
}
