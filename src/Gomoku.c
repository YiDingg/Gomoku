#include <stdio.h>
#include <stdlib.h>  // rand(), exit()
#include <time.h>    // srand(), time()
#include <windows.h> // 获取并输出时间
#include "Gomoku.h"

#define DEBUG 0

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */
Enum_Color Chessboard[COLUMN][ROW];
Struct_GameMode GameMode = {.BlackPlayer = Blank, .WhitePlayer = Blank}; /* 游戏模式 */

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
    char CurrentTurn = 1;                         /* 当前回合数 */
    Enum_LegalOrIllegal islegal;                  /* 用于落子合法性检查 */
    Enum_Color CurrentPlayer = Black;             /* 当前执棋方 */
    Struct_Location WinCoordinates[5] = {0};      /* 获胜坐标 */
    Struct_Location CurentLocation, LastLocation; /* 落子坐标记录 */

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

    while (VictoryJudgment(Chessboard, WinCoordinates) == Blank) {
        printf("\n输入 %d%c 以记录数据并退出游戏\n", 15 + 1, 15 + 65);
        DrawBoard(Chessboard);
        ShowStatu(GameMode, Chessboard, CurrentTurn, CurrentPlayer, WinCoordinates, LastLocation);
        GetChess(&CurentLocation, Chessboard, CurrentPlayer); /* 获取落子坐标 */

#if 0
        /*
        GetChess_Human() 递归获取落子坐标时出现的 bug 还未修复, 暂时将
        GetChess_Human()的合法性检查放在这里
        */
        Enum_LegalOrIllegal islegal;
        CheckThisLocation(&islegal, Chessboard, &CurentLocation, CurrentPlayer);
        while (islegal == Illegal) {
            if (DEBUG) {
                printf(
                    "'%d' '%c' = GetChess_AI_random(), which is illegal, retrying...\n",
                    CurentLocation.row + 1,
                    CurentLocation.column + 'A');
            }
            GetChess(
                &CurentLocation,
                Chessboard,
                CurrentPlayer); /* 这里其实是从 GetChess_Human 获取坐标 */
            CheckThisLocation(&islegal, Chessboard, &CurentLocation, CurrentPlayer);
        }
#endif

        /* 落子检查通过, 执行落子操作 */
        Chessboard[CurentLocation.row][CurentLocation.column] = CurrentPlayer;
        fprintf(f_GomokuData, "%d%c ", CurentLocation.row + 1, CurentLocation.column + 65);
        CurrentTurn += CurrentPlayer == White ? 1 : 0;
        CurrentPlayer = -CurrentPlayer;
        LastLocation = CurentLocation;
    }

    /* 输出游戏结果并记录数据 */
    ShowStatu(GameMode, Chessboard, CurrentTurn, CurrentPlayer, WinCoordinates, LastLocation);
    fprintf(
        f_GomokuData,
        "\nWinner: %s\n",
        (VictoryJudgment(Chessboard, WinCoordinates) == Black) ? "Black" : "White");
    for (char i = 0; i < 5; i++) {
        if (i == 0) {
            fprintf(f_GomokuData, "Win Corrdinates: ");
            printf("Win Corrdinates: ");
        }
        fprintf(f_GomokuData, "%d%c ", WinCoordinates[i].row + 1, WinCoordinates[i].column + 65);
        printf(
            "\033[31m%d%c\033[0m ",
            WinCoordinates[i].row + 1,
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
void ShowInfor(void) {
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
}

/**
 * @brief 绘制棋盘
 * @param none
 * @retval none
 */
void DrawBoard(const Enum_Color chessboard[ROW][COLUMN]) {
    // printf("\n\n");
    printf("   ");
    for (char i = 0; i < COLUMN; i++) {
        printf("\033[34m%-3c\033[0m", i + 65);
    } // 列号, \033[34m 修改字体为蓝色
    printf("\n");
    for (char i = 0; i < ROW; i++) {
        printf("\033[34m%-2d \033[0m", i + 1); // 行号, \033[34m 修改字体为蓝色
        for (char j = 0; j < COLUMN; j++) { DrawPoint(i, j, chessboard[i][j]); }
        printf("\033[34m%-2d\n\033[0m", i + 1); // 行号, \033[34m 修改字体为蓝色
    }
    printf("   ");
    for (char i = 0; i < COLUMN; i++) {
        printf("\033[34m%-3c\033[0m", i + 65);
    } // 列号, \033[34m 修改字体为蓝色
    printf("\n\n");
}

/**
 * @brief 根据棋盘数据绘制当前棋盘
 * @param i 横坐标
 * @param j 纵坐标
 * @param type 绘制的符号类型
 * @retval none
 */
void DrawPoint(const char row, const char column, const int type) {
    if (type == Blank) {
        char* line;
        switch (row) {
        case 0:
            switch (column) {
            case 0: line = "\033[43;30m┌ \033[0m"; break;
            case COLUMN - 1: line = "\033[43;30m ┐\033[0m "; break;
            default: line = "\033[43;30m ┬ \033[0m"; break;
            }
            break;
        case ROW - 1:
            switch (column) {
            case 0: line = "\033[43;30m└ \033[0m"; break;
            case COLUMN - 1: line = "\033[43;30m ┘\033[0m "; break;
            default: line = "\033[43;30m ┴ \033[0m"; break;
            }
            break;
        default:
            switch (column) {
            case 0: line = "\033[43;30m├ \033[0m"; break;
            case COLUMN - 1: line = "\033[43;30m ┤\033[0m "; break;
            default: line = "\033[43;30m ┼ \033[0m"; break;
            }
            break;
        }
        printf("%s", line);
    } else {
        char* marker = (type == Black) ? "●" : "○"; /* 背景色为黄色，因此 "●" 会显示为黑棋 */
        // char* marker = (type == White) ? "■" : "□";
        if (column == 0) {
            printf("\033[43;30m%s \033[0m", marker);
        } else if (column == COLUMN - 1) {
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
void ShowStatu(
    const Struct_GameMode gamemode,
    const Enum_Color chessboard[COLUMN][ROW],
    const char currentturn,
    const Enum_Color currentplayer,
    Struct_Location win_coordinates[5],
    Struct_Location lastlocation) {
    /* 游戏模式 */
    char* str_Black = (gamemode.BlackPlayer == Human) ? "Human" : "Computer";
    char* str_White = (gamemode.WhitePlayer == Human) ? "Human" : "Computer";
    printf("黑棋由 %s 操控，白棋由 %s 操控\n", str_Black, str_White);

    /* 执棋方或胜负状态 */
    if (VictoryJudgment(chessboard, win_coordinates) == Blank) {
        printf("上一步位置：%d%c\n", lastlocation.row + 1, lastlocation.column + 65);
        printf(
            "当前回合: %d, 等待 %s 落子：\n",
            currentturn,
            (currentplayer == Black) ? "黑方" : "白方");
    } else {
        DrawBoard(chessboard);
        switch (VictoryJudgment(chessboard, win_coordinates)) {
        case White:
            puts("------------------");
            puts("    ！白棋胜利！");
            puts("------------------");
            break;
        case Black:
            puts("------------------");
            puts("    ！黑旗胜利！");
            puts("------------------");
            break;
        default: break;
        }
    }
}

/**
 * @brief 检查（通过 Human 或 AI）获得的棋子坐标是否合法
 * @param chessboard 棋盘数据
 * @param location 棋子坐标
 * @param me 当前执棋方
 * @retval none
 */
void CheckThisLocation(
    Enum_LegalOrIllegal* p_islegal,
    const Enum_Color chessboard[ROW][COLUMN],
    const Struct_Location* p_location,
    const Enum_Color me) {
    if (DEBUG) { puts("--> CheckThisLocation()"); }

    /* 退出程序 */
    if (p_location->row == 15 && p_location->column == 15) {
        puts("手动退出程序！");
        exit(0);
    }
    if (DEBUG) {
        printf("location.row: %d, location.column: %d\n", p_location->row, p_location->column);
    }

    /* 检查非法输入 */
    if (p_location->row < 0 || p_location->row > 14 || p_location->column < 0
        || p_location->column > 14) {
        if (DEBUG) {
            printf(
                "Error: 非法输入 '%d' 和 '%c', 请重新输入: \n",
                p_location->row + 1,
                p_location->column + 'A');
            puts("<-- CheckThisLocation()");
        }
        *p_islegal = Illegal;
        return;
    }

    /* 检查此处是否被占 */
    if (chessboard[p_location->row][p_location->column] != Blank) {
        if (DEBUG) {
            printf(
                "Error: 此处已有 %s棋, 请重新输入: \n",
                (chessboard[p_location->row][p_location->column] == Black ? "黑" : "白"));
            puts("<-- CheckThisLocation()");
        }
        *p_islegal = Illegal;
        return;
    }

    if (DEBUG) {
        puts("Success!");
        puts("<-- CheckThisLocation()");
    }
    *p_islegal = Legal;
    return;
}

#if 0
/**
 * @brief 检查（通过 Human 或 AI）获得的棋子坐标是否合法
 * @param none
 * @note 此函数已弃用
 * @retval none
 */
void ChessHandler() {
    /* 获取输入 */
    CurentLocation.row -= 1;     // 坐标转换为 0 起始
    CurentLocation.column -= 65; // 坐标转换为 0 起始

    if (CurentLocation.row == 15 && CurentLocation.column == 15) { exit(0); } // 退出程序

    if (DEBUG) {
        printf(
            "CurentLocation.row: %d, CurentLocation.column: %d\n",
            CurentLocation.row,
            CurentLocation.column);
    }

    /* 检查非法输入 */
    while (0 > CurentLocation.row || CurentLocation.row > 14 || 0 > CurentLocation.column
            || CurentLocation.column > 14) {
        printf("非法输入 (%d%c) 请重新输入: \n", CurentLocation.row, CurentLocation.column);
        GetChess();
        CurentLocation.row -= 1;     // 坐标转换为 0 起始
        CurentLocation.column -= 65; // 坐标转换为 0 起始
        if (CurentLocation.row == 15 && CurentLocation.column == 15) { exit(0); } // 退出程序
    }

    /* 检查此处是否被占 */
    while (Chessboard[CurentLocation.row][CurentLocation.column] != Blank) {
        printf(
            "此处 (%d%c) 已有%s棋! 请重新输入: \n",
            CurentLocation.row + 1,
            CurentLocation.column + 65,
            (Chessboard[CurentLocation.row][CurentLocation.column] == Black ? "黑" : "白"));
        GetChess();
        CurentLocation.row -= 1;     // 坐标转换为 0 起始
        CurentLocation.column -= 65; // 坐标转换为 0 起始
        if (CurentLocation.row == 15 && CurentLocation.column == 15) { exit(0); } // 退出程序
    }
}
#endif

/**
 * @brief 判断胜负
 * @param none
 * @retval none
 */
Enum_Color
VictoryJudgment(const Enum_Color chessboard[ROW][COLUMN], Struct_Location win_coordinate[5]) {
    char i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            if (chessboard[i][j] == Blank) continue;
            /* - 横着连成五子 */
            if (j <= COLUMN - 5)
                if (chessboard[i][j] == chessboard[i][j + 1]
                    && chessboard[i][j] == chessboard[i][j + 2]
                    && chessboard[i][j] == chessboard[i][j + 3]
                    && chessboard[i][j] == chessboard[i][j + 4]) {
                    for (char k = 0; k < 5; k++) {
                        win_coordinate[k] = (Struct_Location){.row = i, .column = j + k};
                    }
                    return chessboard[i][j];
                }

            /* | 竖着连成五子 */
            if (i <= ROW - 5)
                if (chessboard[i][j] == chessboard[i + 1][j]
                    && chessboard[i][j] == chessboard[i + 2][j]
                    && chessboard[i][j] == chessboard[i + 3][j]
                    && chessboard[i][j] == chessboard[i + 4][j]) {
                    for (char k = 0; k < 5; k++) {
                        win_coordinate[k] = (Struct_Location){.row = i + k, .column = j};
                    }
                    return chessboard[i][j];
                }

            /*  \ 向右下方连成五子 */
            if (i <= ROW - 5 && j <= COLUMN - 5)
                if (chessboard[i][j] == chessboard[i + 1][j + 1]
                    && chessboard[i][j] == chessboard[i + 2][j + 2]
                    && chessboard[i][j] == chessboard[i + 3][j + 3]
                    && chessboard[i][j] == chessboard[i + 4][j + 4]) {
                    for (char k = 0; k < 5; k++) {
                        win_coordinate[k] = (Struct_Location){.row = i + k, .column = j + k};
                    }
                    return chessboard[i][j];
                }

            /* / 向左下方连成五子 */
            if (i <= ROW - 5 && j >= 5)
                if (chessboard[i][j] == chessboard[i + 1][j - 1]
                    && chessboard[i][j] == chessboard[i + 2][j - 2]
                    && chessboard[i][j] == chessboard[i + 3][j - 3]
                    && chessboard[i][j] == chessboard[i + 4][j - 4]) {
                    for (char k = 0; k < 5; k++) {
                        win_coordinate[k] = (Struct_Location){.row = i + k, .column = j - k};
                    }
                    return chessboard[i][j];
                }
        }
    }
    return Blank;
}

/**
 * @brief 获取棋子坐标
 * @param none
 * @retval none
 */
void GetChess(
    Struct_Location* p_location,
    const Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me) {
    switch (me) {
    case Black:
        if (GameMode.BlackPlayer == Human) { // Human 下黑旗
            GetChess_Human(p_location, chessboard, me);
        } else { // Computer 下黑旗
            GetChess_AI_random(p_location, chessboard, me);
        }
        break;
    case White:
        if (GameMode.WhitePlayer == Human) { // Human 下白旗
            GetChess_Human(p_location, chessboard, me);
        } else { // Computer 下白旗
            GetChess_AI_random(p_location, chessboard, me);
        }
        break;
    default: break;
    }
}

void GetChess_Human(
    Struct_Location* p_location,
    const Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me) {
    if (DEBUG) { puts("--> GetChess_Human()"); }

    /* 获取落子坐标 */
    int num = scanf("%d%c", &p_location->row, &p_location->column);
    p_location->row -= 1;
    p_location->column -= 'A';

    /* 检查输入格式与接收参数个数 */
    if (num != 2) {
        /* scanf 返回成功获取的参数个数, != 2 时输入格式不正确，清除输入缓冲区并提示重新输入*/
        while (getchar() != '\n'); // 清除缓冲区直到遇到换行符
        printf("scanf() 扫描到的输入有格式错误，请重新输入\n");
        GetChess_Human(p_location, chessboard, me);
    }

    /* 检查落子坐标合法性 */
    Enum_LegalOrIllegal islegal;
    CheckThisLocation(&islegal, chessboard, p_location, me);
    if (islegal == Illegal) {
        if (DEBUG) {
            printf(
                "'%d' '%c' = GetChess_Human(), which is illegal, retrying...\n",
                p_location->row + 1,
                p_location->column + 'A');
        }
        printf(
            "GetChess_Human() 获取到 '%d' 和 '%c'，为非法数据, 请重新输入:\n",
            p_location->row + 1,
            p_location->column + 'A');
        GetChess_Human(p_location, chessboard, me);
    }

#if 0
    /* 检查输入是否合法 */
        while (1) {
            if (Legal == CheckThisLocation(chessboard, loc, me)) {
                puts("退出 while GetChess_Human");
                if (DEBUG) { puts("Exit : GetChess_Human()"); }
                return loc;
            }
            puts("进入 while GetChess_Human");
            loc = GetChess_Human(chessboard, me);
        }

    while (Illegal == CheckThisLocation(islegal, Chessboard, CurentLocation, CurrentPlayer)) {
                printf(
                    "GetChess_Human() 获取到 '%d' 和 '%c'，为非法数据, 请重新输入: ",
                    CurentLocation.row + 1,
                    CurentLocation.column + 'A');
                GetChess(Chessboard, CurrentPlayer);
            }
#endif

    /* 返回落子位置 */
    if (DEBUG) { puts("<-- GetChess_Human()"); }
}

/**
 * @brief GetChess: AI 随机落子
 * @param none
 * @retval none
 */
void GetChess_AI_random(
    Struct_Location* p_location,
    const Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me) {
    if (DEBUG) { puts("--> GetChess_AI_random()"); }

    /* 生成随机落子 */
    srand((unsigned)(time(NULL) + rand())); /* 设置随机数种子 */
    p_location->row = ((rand() % ROW + 1) * 14 + rand()) % ROW;
    srand((unsigned)(time(NULL) + rand())); /* 设置随机数种子 */
    p_location->column = ((rand() % COLUMN + 2) * 8 + 2 * rand()) % COLUMN;

    /* 检查落子坐标合法性 */
    Enum_LegalOrIllegal islegal;
    CheckThisLocation(&islegal, chessboard, p_location, me);
    if (islegal == Illegal) {
        if (DEBUG) {
            printf(
                "'%d' '%c' = GetChess_AI_random(), which is illegal, retrying...\n",
                p_location->row + 1,
                p_location->column + 'A');
        }
        GetChess_AI_random(p_location, chessboard, me);
    }

    /* 返回落子位置 */
    if (DEBUG) { puts("<-- GetChess_AI_random()"); }
}
/*                                                  */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */
