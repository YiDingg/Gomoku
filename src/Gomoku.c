#include "Gomoku.h"
#include <stdio.h>
#include <stdlib.h>  // rand()
#include <windows.h> // 获取并输出时间

#define debug 0

/* 全局变量（在 .data 段，初值默认 0） */
struct COORDINATE WinCoordinate[5] = {0}; // 获胜坐标
struct GAMEMODE GameMode = {.BlackPlayer = Blank, .WhitePlayer = Blank};
enum COLOR CurrentPlayer = Black; // 当前执棋方
int ChessBoard[COLUMN][ROW];
struct COORDINATE CuurentCoordinate, LastCoordinate = {.raw = -1, .column = -65};
char CurrentTurn = 0; // 当前轮数

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
        "\nRun time: %04d.%02d.%02d %02d:%02d:%02d\n",
        time.wYear,
        time.wMonth,
        time.wDay,
        time.wHour,
        time.wMinute,
        time.wSecond);

    /* 模式选择 */
    ChooseMode(&GameMode);
    fprintf(
        f_GomokuData,
        "Game mode: BlackPlayer-%s, WhitePlayer-%s.\n",
        GameMode.BlackPlayer == Human ? "Human" : "Computer",
        GameMode.WhitePlayer == Human ? "Human" : "Computer");

    /* 开始游戏 */
    CurrentTurn = 1;
    while (VictoryJudgment(ChessBoard) == Blank) {
        printf("\n输入 %d%c 以记录数据并退出游戏\n", 15 + 1, 15 + 65);
        DrawBoard();
        ShowStatu();
        GetChess();
        ChessHandler();
        /* 落子检查通过 */
        ChessBoard[CuurentCoordinate.raw][CuurentCoordinate.column] = CurrentPlayer;
        fprintf(f_GomokuData, "%d%c ", CuurentCoordinate.raw + 1, CuurentCoordinate.column + 65);
        CurrentTurn += CurrentPlayer == White ? 1 : 0;
        CurrentPlayer = -CurrentPlayer;
        LastCoordinate = CuurentCoordinate;
    }

    /* 游戏结果 */
    ShowStatu();
    fprintf(f_GomokuData, "\nWinner: %s\n", (VictoryJudgment(ChessBoard) == Black) ? "Black" : "White");
}

/**
 * @brief 选择游戏模式
 * @param p_gamemode 游戏模式结构体指针
 * @retval none
 */
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
    if (p_gamemode->BlackPlayer == Computer) {
        CuurentCoordinate.raw = 6;
        CuurentCoordinate.column = 6;
    }
}

/**
 * @brief 绘制棋盘
 * @param none
 * @retval none
 */
void DrawBoard() {
    // printf("\n\n");
    for (char i = 0; i < ROW; i++) {
        printf("%-2d ", i + 1);
        for (char j = 0; j < COLUMN; j++) { DrawPoint(i, j, ChessBoard[i][j]); }
    }
    printf("   ");
    for (char i = 0; i < COLUMN; i++) { printf("%-3c", i + 65); }
    printf("\n\n");
}

/**
 * @brief 根据棋盘数据绘制当前棋盘
 * @param i 横坐标
 * @param j 纵坐标
 * @param type 绘制的符号类型
 * @retval none
 */
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
            printf(" %s\n", marker);
        } else {
            printf(" %s ", marker);
        }
    }
}

/**
 * @brief 显示游戏当前状态
 * @param none
 * @retval none
 */
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
        printf("当前回合: %d, 等待 %s 落子：\n", CurrentTurn, (CurrentPlayer == Black) ? "黑方" : "白方");
    } else {
        DrawBoard();
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
}

/**
 * @brief 获取棋子坐标
 * @param none
 * @retval none
 */
void GetChess() {
    switch (CurrentPlayer) {
    case Black:
        if (GameMode.BlackPlayer == Human) { // Human 下黑旗
            scanf("%d%c", &CuurentCoordinate.raw, &CuurentCoordinate.column);
        } else { // Computer 下黑旗
            GetChess_AI_random();
        }
        break;
    case White:
        if (GameMode.WhitePlayer == Human) { // Human 下白旗
            scanf("%d%c", &CuurentCoordinate.raw, &CuurentCoordinate.column);
        } else { // Computer 下白旗
            GetChess_AI_random();
        }
        break;
    default: break;
    }
}

/**
 * @brief 处理（通过 Human 或 AI）获得的棋子坐标
 * @param none
 * @retval none
 */
void ChessHandler() {
    /* 获取输入 */
    CuurentCoordinate.raw -= 1;     // 坐标转换为 0 起始
    CuurentCoordinate.column -= 65; // 坐标转换为 0 起始

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
        printf("非法输入 (%d%c) 请重新输入: \n", CuurentCoordinate.raw, CuurentCoordinate.column);
        GetChess();
        CuurentCoordinate.raw -= 1;                                                     // 坐标转换为 0 起始
        CuurentCoordinate.column -= 65;                                                 // 坐标转换为 0 起始
        if (CuurentCoordinate.raw == 15 && CuurentCoordinate.column == 15) { exit(0); } // 退出程序
    }

    /* 检查此处是否被占 */
    while (ChessBoard[CuurentCoordinate.raw][CuurentCoordinate.column] != Blank) {
        printf(
            "此处 (%d%c) 已有%s棋! 请重新输入: \n",
            CuurentCoordinate.raw + 1,
            CuurentCoordinate.column + 65,
            (ChessBoard[CuurentCoordinate.raw][CuurentCoordinate.column] == Black ? "黑" : "白"));
        GetChess();
        CuurentCoordinate.raw -= 1;                                                     // 坐标转换为 0 起始
        CuurentCoordinate.column -= 65;                                                 // 坐标转换为 0 起始
        if (CuurentCoordinate.raw == 15 && CuurentCoordinate.column == 15) { exit(0); } // 退出程序
    }
}

/**
 * @brief 判断胜负
 * @param none
 * @retval none
 */
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

/**
 * @brief GetChess: AI 随机落子
 * @param none
 * @retval none
 */
void GetChess_AI_random() {
    CuurentCoordinate.raw = rand() % ROW + 1;
    CuurentCoordinate.column = rand() % COLUMN + 65;
}
