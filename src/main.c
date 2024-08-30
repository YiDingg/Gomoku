#include <windows.h> // 解决输出乱码
#include <stdio.h>
#include "Gomoku.h"

#define debug 0

int main() {
    SetConsoleOutputCP(65001); // 设置编码为 UTF-8，解决输出乱码

    if (debug) printf("┼ 所占字节：%d\n", sizeof("┼"));
    DrawBoard(ChessBoard);
    ShowStatu();
    if (debug) getchar();
    system("pause");
    for (char i = 0; i < 5; i++) {
        ChessBoard[2][i] = White;
        ChessBoard[3][2 * i] = Black;
    }
    DrawBoard(ChessBoard);
    ShowStatu();
    system("pause");
    return 0;
}
