// AI 采用了 Alpha-Beta 剪枝的优化 (PVS), 实现约 8 层搜索

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h> // 解决输出乱码
#include "GameMode.h"
#include "Gomoku.h"

int main(void) {
    SetConsoleOutputCP(65001); // 设置编码为 UTF-8 (65001)，解决输出乱码
    // SetConsoleOutputCP(936); // 设置编码为 GBK (936)，解决输出乱码

    Gomoku_Run(); // 运行五子棋主函数

    system("pause");
    system("pause");
}