#include "Gomoku.h"
#include <stdio.h>
#include <stdlib.h>  // rand()
#include <windows.h> // 获取并显示时间

#define debug 0

/* 全局变量（在 .data 段，初值默认 0） */
struct GAMEMODE GameMode = {.BlackPlayer = Blank, .WhitePlayer = Blank};
enum COLOR CurrentPlayer = Black; // 当前执棋方
int ChessBoard[COLUMN][ROW];
struct COORDINATE CuurentCoordinate, LastCoordinate = {.raw = -1, .column = -65};

/* 函数 */

/**
 * @brief 五子棋主函数
 * @param none
 * @retval none
 */
void Gomoku_Run() {
    /* 数据文件初始化 */
    FILE* f_GomokuData = fopen("GomokuData.txt", "a+");
    if (f_GomokuData == NULL) {
        printf("Failed to open file GomokuData.txt!\n");
        exit(1);
    }
    SYSTEMTIME time;
    GetLocalTime(&time);
    fprintf(
        f_GomokuData,
        "Run time: %04d.%02d.%02d %02d:%02d:%02d\n",
        time.wYear,
        time.wMonth,
        time.wDay,
        time.wHour,
        time.wMinute,
        time.wSecond);

    /* 模式选择 */
    ChooseMode(&GameMode);

    /* 开始游戏 */
    while (VictoryJudgment(ChessBoard) == Blank) {
        printf("输入 %d%c 以退出游戏，", 15 + 1, 15 + 65);
        DrawBoard(ChessBoard);
        ShowStatu();

        /* 获取输入 */
        scanf("%d%c", &CuurentCoordinate.raw, &CuurentCoordinate.column);
        CuurentCoordinate.raw -= 1;                                                     // 坐标转换为 0 起始
        CuurentCoordinate.column = CuurentCoordinate.column - 65;                       // 坐标转换为 0 起始
        if (CuurentCoordinate.raw == 15 && CuurentCoordinate.column == 15) { exit(0); } // 退出程序

        if (debug) {
            printf(
                "CuurentCoordinate.raw: %d, CuurentCoordinate.column: %d\n",
                CuurentCoordinate.raw,
                CuurentCoordinate.column);
        }

        /* 检查非法输入 */
        while (0 > CuurentCoordinate.raw || CuurentCoordinate.raw > 14 || 0 > CuurentCoordinate.column
               || CuurentCoordinate.column > 14) {
            printf("非法输入! 请重新输入: \n");
            scanf("%d %c", &CuurentCoordinate.raw, &CuurentCoordinate.column);
            CuurentCoordinate.raw -= 1;                                                     // 坐标转换为 0 起始
            CuurentCoordinate.column -= 65;                                                 // 坐标转换为 0 起始
            if (CuurentCoordinate.raw == 15 && CuurentCoordinate.column == 15) { exit(0); } // 退出程序
        }
        /* 检查此处是否被占 */
        while (ChessBoard[CuurentCoordinate.raw][CuurentCoordinate.column] != Blank) {
            printf(
                "此处已有%s棋! 请重新输入: \n",
                (ChessBoard[CuurentCoordinate.raw][CuurentCoordinate.column] == Black ? "黑" : "白"));
            scanf("%d %c", &CuurentCoordinate.raw, &CuurentCoordinate.column);
            CuurentCoordinate.raw -= 1;                                                     // 坐标转换为 0 起始
            CuurentCoordinate.column -= 65;                                                 // 坐标转换为 0 起始
            if (CuurentCoordinate.raw == 15 && CuurentCoordinate.column == 15) { exit(0); } // 退出程序
        }

        ChessBoard[CuurentCoordinate.raw][CuurentCoordinate.column] = CurrentPlayer;
        fprintf(f_GomokuData, "%d%c ", CuurentCoordinate.raw + 1, CuurentCoordinate.column + 65);

        CurrentPlayer = -CurrentPlayer;
        LastCoordinate = CuurentCoordinate;
    }

    /* 游戏结果 */
    ShowStatu();
}

void ShowStatu(void) {
    /* 游戏模式 */
    char* str_Black = (GameMode.BlackPlayer == Human) ? "Human" : "Computer";
    char* str_White = (GameMode.WhitePlayer == Human) ? "Human" : "Computer";
    if (debug) {
        puts(str_Black);
        puts(str_White);
    }
    printf("黑棋由 %s 操控，白棋由 %s 操控\n", str_Black, str_White);

    /* 执棋方或胜负状态 */
    if (VictoryJudgment(ChessBoard) == Blank) {
        printf("上一步位置：%d%c\n", LastCoordinate.raw + 1, LastCoordinate.column + 65);
        printf("等待 %s 落子：\n", (CurrentPlayer == Black) ? "黑方" : "白方");
    } else {
        switch (VictoryJudgment(ChessBoard)) {
        case White:
            puts("---------------");
            puts("！白棋胜利！");
            puts("---------------");
            break;
        case Black:
            puts("---------------");
            puts("！黑旗胜利！");
            puts("---------------");
            break;
        default: break;
        }
    }

    /* 谁下棋，第几手 */
}

void ChooseMode(struct GAMEMODE* p_gamemode) {
    puts("------------------------------");
    puts("选择黑棋由谁操控：");
    printf("%d: Human\n", Human);
    printf("%d: Computer\n", Computer);
    puts("------------------------------");
    scanf("%d", &p_gamemode->BlackPlayer);
    puts("------------------------------");
    puts("选择白棋由谁操控：");
    printf("%d: Human\n", Human);
    printf("%d: Computer\n", Computer);
    puts("------------------------------");
    scanf("%d", &p_gamemode->WhitePlayer);
    puts("------------------------------");
    if (debug) {
        printf("p_gamemode->BlackPlayer: %d\n", p_gamemode->BlackPlayer);
        printf("p_gamemode->WhitePlayer: %d\n", p_gamemode->WhitePlayer);
    }
    int bp = p_gamemode->BlackPlayer * p_gamemode->BlackPlayer;
    int wp = p_gamemode->WhitePlayer * p_gamemode->WhitePlayer;
    if (bp != 1 || wp != 1) {
        puts("模式选择错误，请重新输入！");
        puts("------------------------------");
        ChooseMode(p_gamemode);
    }
}

/**
 * @brief 随机落子
 * @param none
 * @retval none
 */
void AIChess_random(int chessboard[][ROW]) {
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
    printf("\n\n");
    for (char i = 0; i < ROW; i++) {
        printf("%-2d ", i + 1);
        for (char j = 0; j < COLUMN; j++) { DrawPoint(i, j, chessboard[i][j]); }
    }
    printf("   ");
    for (char i = 0; i < COLUMN; i++) { printf("%-3c", i + 65); }
    printf("\n\n");
}

int VictoryJudgment(int chessboard[][ROW]) {
    int i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            if (chessboard[i][j] == Blank) continue;

            /* - 横着连成五子 */
            if (j < COLUMN - 4)
                if (chessboard[i][j] == chessboard[i][j + 1] && chessboard[i][j] == chessboard[i][j + 2]
                    && chessboard[i][j] == chessboard[i][j + 3] && chessboard[i][j] == chessboard[i][j + 4])
                    return chessboard[i][j];

            /* | 竖着连成五子 */
            if (i < ROW - 4)
                if (chessboard[i][j] == chessboard[i + 1][j] && chessboard[i][j] == chessboard[i + 2][j]
                    && chessboard[i][j] == chessboard[i + 3][j] && chessboard[i][j] == chessboard[i + 4][j])
                    return chessboard[i][j];

            /*  \ 向右下方连成五子 */
            if (i < ROW - 4 && j < COLUMN - 4)
                if (chessboard[i][j] == chessboard[i + 1][j + 1] && chessboard[i][j] == chessboard[i + 2][j + 2]
                    && chessboard[i][j] == chessboard[i + 3][j + 3] && chessboard[i][j] == chessboard[i + 4][j + 4])
                    return chessboard[i][j];

            /* / 向左下方连成五子 */
            if (i < ROW - 4 && j > 4)
                if (chessboard[i][j] == chessboard[i + 1][j - 1] && chessboard[i][j] == chessboard[i + 2][j - 2]
                    && chessboard[i][j] == chessboard[i + 3][j - 3] && chessboard[i][j] == chessboard[i + 4][j - 4])
                    return chessboard[i][j];
        }
    }
    return Blank;
}
