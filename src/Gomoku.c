#include <stdio.h>
#include <stdlib.h>  // rand()
#include <time.h>    // srand(), time()
#include <windows.h> // 获取并输出时间
#include "Gomoku.h"

#define DEBUG 0

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */
int ChessBoard[COLUMN][ROW];
char CurrentTurn = 0;                    // 当前轮数
Struct_Location WinCoordinates[5] = {0}; // 获胜坐标
Enum_Color CurrentPlayer = Black;        // 当前执棋方
Struct_GameMode GameMode = {.BlackPlayer = Blank, .WhitePlayer = Blank};
Struct_Location CuurentCoordinate, LastCoordinate = {.raw = -1, .column = -65};

/*                                                  */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/* ------------------------------------------------ */

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/*                                                  */
/**
 * @brief 五子棋主函数
 * @param none
 * @retval none
 */
void Gomoku_Run() {
    ShowInfor();

    /* 数据文件初始化 */
    FILE* f_GomokuData = fopen("GomokuData.txt", "a+");
    if (f_GomokuData == NULL) {
        printf("Failed to open file GomokuData.txt!\n");
        exit(1);
    }
    SYSTEMTIME time;
    GetLocalTime(&time);
    if (DEBUG) { printf("%04d", time.wSecond); }
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
    while (VictoryJudgment(ChessBoard, WinCoordinates) == Blank) {
        printf("\n输入 %d%c 以记录数据并退出游戏\n", 15 + 1, 15 + 65);
        DrawBoard();
        ShowStatu();
        GetChess();
        while (Illegal == CheckThisLocation(ChessBoard, CuurentCoordinate, CurrentPlayer)) {
            printf(
                "你刚刚输入了 %d%c，输入范围错误，或者此处已有棋子，请重新输入: \n",
                CuurentCoordinate.raw + 1,
                CuurentCoordinate.column + 65);
            GetChess();
        }
        // ChessHandler();
        /* 落子检查通过 */
        ChessBoard[CuurentCoordinate.raw][CuurentCoordinate.column] = CurrentPlayer;
        fprintf(f_GomokuData, "%d%c ", CuurentCoordinate.raw + 1, CuurentCoordinate.column + 65);
        CurrentTurn += CurrentPlayer == White ? 1 : 0;
        CurrentPlayer = -CurrentPlayer;
        LastCoordinate = CuurentCoordinate;
    }

    /* 输出游戏结果并记录数据 */
    ShowStatu();
    fprintf(f_GomokuData, "\nWinner: %s\n", (VictoryJudgment(ChessBoard, WinCoordinates) == Black) ? "Black" : "White");
    for (char i = 0; i < 5; i++) {
        if (i == 0) {
            fprintf(f_GomokuData, "Win Corrdinates: ");
            printf("Win Corrdinates: ");
        }
        fprintf(f_GomokuData, "%d%c ", WinCoordinates[i].raw + 1, WinCoordinates[i].column + 65);
        printf(
            "\033[31m%d%c\033[0m ",
            WinCoordinates[i].raw + 1,
            WinCoordinates[i].column + 65); // \033[31m 设置字体为红色
        if (i == 4) {
            fprintf(f_GomokuData, "\n");
            printf("\n");
        }
    }

    /* 暂停以显示 */
    system("pause");
}

/**
 * @brief 显示游戏相关信息
 * @param none
 * @retval none
 */
void ShowInfor() {
    puts("------------------------------------------------");
    /* \033[31m 设置文本颜色为红色, \033[0m 重置文本颜色 */
    puts(">> ----------- Welcome to Gomoku ! ---------- <<");
    puts("   Author: Yi Ding");
    puts("   Version: 1.0");
    puts("   Date: 2024.8.30");
    puts("   Email: dingyi233@mails.ucas.ac.cn");
    puts("   GitHub: https://github.com/YiDingg/Gomoku");
    puts(">> ------------------------------------------- <<");
    puts("-------------------------------------------------\n");
}

/**
 * @brief 选择游戏模式
 * @param p_gamemode 游戏模式结构体指针
 * @retval none
 */
void ChooseMode(Struct_GameMode* p_gamemode) {
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
    if (DEBUG) {
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
    printf("   ");
    for (char i = 0; i < COLUMN; i++) { printf("\033[34m%-3c\033[0m", i + 65); } // 列号, \033[34m 修改字体为蓝色
    printf("\n");
    for (char i = 0; i < ROW; i++) {
        printf("\033[34m%-2d \033[0m", i + 1); // 行号, \033[34m 修改字体为蓝色
        for (char j = 0; j < COLUMN; j++) { DrawPoint(i, j, ChessBoard[i][j]); }
        printf("\033[34m%-2d\n\033[0m", i + 1); // 行号, \033[34m 修改字体为蓝色
    }
    printf("   ");
    for (char i = 0; i < COLUMN; i++) { printf("\033[34m%-3c\033[0m", i + 65); } // 列号, \033[34m 修改字体为蓝色
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
            case 0: line = "\033[43;30m┌ \033[0m"; break;
            case COLUMN - 1: line = "\033[43;30m ┐\033[0m "; break;
            default: line = "\033[43;30m ┬ \033[0m"; break;
            }
            break;
        case ROW - 1:
            switch (j) {
            case 0: line = "\033[43;30m└ \033[0m"; break;
            case COLUMN - 1: line = "\033[43;30m ┘\033[0m "; break;
            default: line = "\033[43;30m ┴ \033[0m"; break;
            }
            break;
        default:
            switch (j) {
            case 0: line = "\033[43;30m├ \033[0m"; break;
            case COLUMN - 1: line = "\033[43;30m ┤\033[0m "; break;
            default: line = "\033[43;30m ┼ \033[0m"; break;
            }
            break;
        }
        printf("%s", line);
    } else {
        char* marker = (type == White) ? "●" : "○";
        // char* marker = (type == White) ? "■" : "□";
        if (j == 0) {
            printf("\033[43;30m%s \033[0m", marker);
        } else if (j == COLUMN - 1) {
            printf("\033[43;30m %s\033[0m ", marker);
        } else {
            printf("\033[43;30m %s \033[0m", marker);
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
    if (DEBUG) {
        puts(str_Black);
        puts(str_White);
    }
    printf("黑棋由 %s 操控，白棋由 %s 操控\n", str_Black, str_White);

    /* 执棋方或胜负状态 */
    if (VictoryJudgment(ChessBoard, WinCoordinates) == Blank) {
        printf("上一步位置：%d%c\n", LastCoordinate.raw + 1, LastCoordinate.column + 65);
        printf("当前回合: %d, 等待 %s 落子：\n", CurrentTurn, (CurrentPlayer == Black) ? "黑方" : "白方");
    } else {
        DrawBoard();
        switch (VictoryJudgment(ChessBoard, WinCoordinates)) {
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
    /* 将输入的字符转换为 location 结构体坐标 */
    CuurentCoordinate.raw -= 1;
    CuurentCoordinate.column -= 65;
}

/**
 * @brief 检查（通过 Human 或 AI）获得的棋子坐标是否合法
 * @param chessboard 棋盘数据
 * @param location 棋子坐标
 * @param me 当前执棋方
 * @retval none
 */
Enum_LegalOrIllegal
CheckThisLocation(const int chessboard[ROW][COLUMN], const Struct_Location location, const Enum_Color me) {
    /* 退出程序 */
    if (location.raw == 15 && location.column == 15) { exit(0); }
    if (DEBUG) { printf("location.raw: %d, location.column: %d\n", location.raw, location.column); }
    /* 检查非法输入 */
    if (0 > location.raw || location.raw > 14 || 0 > location.column || location.column > 14) { return Illegal; }
    /* 检查此处是否被占 */
    if (ChessBoard[CuurentCoordinate.raw][CuurentCoordinate.column] != Blank) { return Illegal; }
    return Legal;
}

/**
 * @brief 检查（通过 Human 或 AI）获得的棋子坐标是否合法
 * @param none
 * @note 此函数已弃用
 * @retval none
 */
void ChessHandler() {
    /* 获取输入 */
    CuurentCoordinate.raw -= 1;     // 坐标转换为 0 起始
    CuurentCoordinate.column -= 65; // 坐标转换为 0 起始

    if (CuurentCoordinate.raw == 15 && CuurentCoordinate.column == 15) { exit(0); } // 退出程序

    if (DEBUG) {
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
int VictoryJudgment(int chessboard[][ROW], Struct_Location win_coordinate[5]) {
    char i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            if (chessboard[i][j] == Blank) continue;
            /* - 横着连成五子 */
            if (j <= COLUMN - 5)
                if (chessboard[i][j] == chessboard[i][j + 1] && chessboard[i][j] == chessboard[i][j + 2]
                    && chessboard[i][j] == chessboard[i][j + 3] && chessboard[i][j] == chessboard[i][j + 4]) {
                    for (char k = 0; k < 5; k++) { win_coordinate[k] = (Struct_Location){.raw = i, .column = j + k}; }
                    return chessboard[i][j];
                }

            /* | 竖着连成五子 */
            if (i <= ROW - 5)
                if (chessboard[i][j] == chessboard[i + 1][j] && chessboard[i][j] == chessboard[i + 2][j]
                    && chessboard[i][j] == chessboard[i + 3][j] && chessboard[i][j] == chessboard[i + 4][j]) {
                    for (char k = 0; k < 5; k++) { win_coordinate[k] = (Struct_Location){.raw = i + k, .column = j}; }
                    return chessboard[i][j];
                }

            /*  \ 向右下方连成五子 */
            if (i <= ROW - 5 && j <= COLUMN - 5)
                if (chessboard[i][j] == chessboard[i + 1][j + 1] && chessboard[i][j] == chessboard[i + 2][j + 2]
                    && chessboard[i][j] == chessboard[i + 3][j + 3] && chessboard[i][j] == chessboard[i + 4][j + 4]) {
                    for (char k = 0; k < 5; k++) {
                        win_coordinate[k] = (Struct_Location){.raw = i + k, .column = j + k};
                    }
                    return chessboard[i][j];
                }

            /* / 向左下方连成五子 */
            if (i <= ROW - 5 && j >= 5)
                if (chessboard[i][j] == chessboard[i + 1][j - 1] && chessboard[i][j] == chessboard[i + 2][j - 2]
                    && chessboard[i][j] == chessboard[i + 3][j - 3] && chessboard[i][j] == chessboard[i + 4][j - 4]) {
                    for (char k = 0; k < 5; k++) {
                        win_coordinate[k] = (Struct_Location){.raw = i + k, .column = j - k};
                    }
                    return chessboard[i][j];
                }
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
    srand((unsigned)(time(NULL) + rand())); /* 设置随机数种子 */
    CuurentCoordinate.raw = ((rand() % ROW + 1) * 14 + rand()) % ROW + 1;
    srand((unsigned)(time(NULL) + rand())); /* 设置随机数种子 */
    CuurentCoordinate.column = ((rand() % COLUMN + 2) * 8 + 2 * rand()) % COLUMN + 65;
}
/*                                                  */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */
