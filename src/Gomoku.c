#include <stdio.h>
#include "Gomoku.h"
#include <stdlib.h> // rand()

/* 全局变量（在 .data 段，初值默认 0） */
struct GAMEMODE GameMode = {.BlackPlayer = Blank, .WhitePlayer = Blank};

int ChessBoard[COLUMN][ROW];

/* 函数 */

/**
 * @brief 游戏初始化
 * @param none
 * @retval none
 */
void Gomoku_Init() {
}

/**
 * @brief 五子棋主函数
 * @param none
 * @retval none
 */
void Gomoku_Run() {
    ChooseMode(&GameMode);
}

void ChooseMode(struct GAMEMODE* p_gamemode) {
    puts("选择黑棋由谁操控：");
    printf("%d: Human\n", Human);
    printf("%d: Computer\n", Computer);
    p_gamemode->BlackPlayer = getchar();
    puts("选择白棋由谁操控：");
    printf("%d: Human\n", Human);
    printf("%d: Computer\n", Computer);
    p_gamemode->WhitePlayer = getchar();
    if (p_gamemode->BlackPlayer == None || p_gamemode->WhitePlayer == None) {
        printf("模式选择错误，请重新输入！\n");
        ChooseMode(p_gamemode);
    }
}

/**
 * @brief 人机对战时的随机落子
 * @param none
 * @retval none
 */
void PutRandomChess(int chessboard[][ROW]) {
    rand();
}

void DrawPoint(char i, char j, int type) {
    if (type == Blank) {
        char* line;
        switch (i) {
        case 0:
            switch (j) {
            case 0: line = "┌ "; break;
            case COLUMN - 1: line = " ┐\n"; break;
            default: line = " ┬ "; break;
            }
            break;
        case ROW - 1:
            switch (j) {
            case 0: line = "└ "; break;
            case COLUMN - 1: line = " ┘\n"; break;
            default: line = " ┴ "; break;
            }
            break;
        default:
            switch (j) {
            case 0: line = "├ "; break;
            case COLUMN - 1: line = " ┤\n"; break;
            default: line = " ┼ "; break;
            }
            break;
        }
        printf("%s", line);
    } else {
        char* marker = (type == White) ? "●" : "○";
        if (j == 0) {
            printf("%s ", marker);
        } else if (j == COLUMN - 1) {
            printf(" %s", marker);
        } else {
            printf(" %s ", marker);
        }
    }
}

void DrawBoard(int chessboard[][ROW]) {
    for (int i = 0; i < ROW; i++) {
        for (char j = 0; j < COLUMN; j++) { DrawPoint(i, j, chessboard[i][j]); }
    }
}

int VictoryJudgment(int chessboard[][ROW]) {
    int i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            if (chessboard[i][j] == Blank) continue;

            //  (-)横着连成五子
            if (j < COLUMN - 4)
                if (chessboard[i][j] == chessboard[i][j + 1] && chessboard[i][j] == chessboard[i][j + 2]
                    && chessboard[i][j] == chessboard[i][j + 3] && chessboard[i][j] == chessboard[i][j + 4])
                    return chessboard[i][j];

            //  (|)竖着连成五子
            if (i < ROW - 4)
                if (chessboard[i][j] == chessboard[i + 1][j] && chessboard[i][j] == chessboard[i + 2][j]
                    && chessboard[i][j] == chessboard[i + 3][j] && chessboard[i][j] == chessboard[i + 4][j])
                    return chessboard[i][j];

            //  (\)左斜连成五子
            if (i < ROW - 4 && j < COLUMN - 4)
                if (chessboard[i][j] == chessboard[i + 1][j + 1] && chessboard[i][j] == chessboard[i + 2][j + 2]
                    && chessboard[i][j] == chessboard[i + 3][j + 3] && chessboard[i][j] == chessboard[i + 4][j + 4])
                    return chessboard[i][j];

            //  (/)右斜连成五子
            if (i < ROW - 4 && j > 4)
                if (chessboard[i][j] == chessboard[i + 1][j - 1] && chessboard[i][j] == chessboard[i + 2][j - 2]
                    && chessboard[i][j] == chessboard[i + 3][j - 3] && chessboard[i][j] == chessboard[i + 4][j - 4])
                    return chessboard[i][j];
        }
    }
    return Blank;
}

void ShowStatu(void) {
    /* 游戏模式 */
    char* str_Black = (GameMode.BlackPlayer == Human) ? "Human" : "Computer";
    char* str_White = (GameMode.WhitePlayer == Human) ? "Human" : "Computer";

    printf("黑棋由 %s 操控，白棋由 %s 操控\n", GameMode.BlackPlayer, GameMode.WhitePlayer);

    /* 胜负状态 */
    switch (VictoryJudgment(ChessBoard)) {
    case White:
        puts("---------------");
        puts("胜负状态：！！白旗胜利！！");
        puts("---------------");
        break;
    case Black:
        puts("---------------");
        puts("胜负状态：！！黑旗胜利！！");
        puts("---------------");
        break;
    case Blank: puts("胜负状态：游戏进行中..."); break;
    default: break;
    }

    /* 谁下棋，第几手 */
}