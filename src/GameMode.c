#include "GameMode.h"
#include <stdio.h>

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */
/* 三种游戏模式入口 */

char player_color = 0; // 玩家选择的颜色
int CurrentPlayer = 1; // 当前玩家，1 为黑棋，2 为白棋
int BoolPlayer = 0;    // 当前玩家标识，0 为黑棋，1 为白棋
Tree g_move = NULL;    // 指向上一步走法的指针，用于更新走法
int Row = 7;           // 当前行号，存储当前要落子的位置
int Col = 7;           // 当前列号，存储当前要落子的位置
extern int MaxDepth;   // AI搜索的最大深度
/*                                                  */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/* ------------------------------------------------ */

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/*                                                  */

/**
 * @brief 人人对战模式的主循环
 * @param none
 * @retval none
 */
void Human_VS_Human(void) {
    int result; // 记录游戏结果

    // system("cls"); // 清屏命令，暂时注释
    Print_ChessBoard();                                              // 打印当前棋盘
    while (!(result = GetWinner(Row, Col)) && !IsChessBoardFull()) { // 检查是否有赢家或棋盘满
        Update_Human();                                              // 更新玩家落子
        // system("cls"); // 清屏命令，暂时注释
        Print_ChessBoard();   // 打印更新后的棋盘
        Print_LastLocation(); // 打印最后落子的位置
        CHANGE_PLAYER;        // 切换到下一个玩家
    }
    Print_ChessBoard(); // 打印最终棋盘
    CHANGE_PLAYER;      // 切换回最终赢家
    Print_Winner();     // 打印赢家信息
}

/**
 * @brief 悔棋机制，确认是否悔棋 (Human VS Computer 专属)
 * @param none
 * @retval none
 */
void IsUndo() {
    if (CurrentPlayer == WHITE) { // 如果当前玩家是白棋
        printf(
            "[白方] %s 落子在 [%c%d] (上方棋盘), 是否确认 ? (input 'y' or 'n')\n",
            CurrentPlayer == player_color ? "Human (对手 AI)" : "Computer",
            Col + 'A',
            Row + 1);
    } else { // 如果当前玩家是黑旗
        printf(
            "[黑方] %s 落子在 [%c%d] (上方棋盘), 是否确认 ? (input 'y' or 'n')\n",
            CurrentPlayer == player_color ? "Human (对手 AI)" : "Computer",
            Col + 'A',
            Row + 1);
    }
}

/**
 * @brief 人机对战模式的主循环
 * @param none
 * @retval none
 */
void Human_VS_Computer(void) {
    int result;      // 记录游戏结果
    int time_AI = 0; // AI思考时间

    /* 初始化AI相关的哈希表等 */
    // printf("Loading...\n");
    Init_Hash();      // 初始化评分哈希表
    Init_BitBoard();  // 初始化位棋盘
    Init_MoveTable(); // 初始化走法表

    while (getchar() != '\n'); // 清空输入缓冲区
    puts("选择 Human (或对手 AI) 颜色");
    puts("  1. 黑棋 (先手)");
    puts("  2. 白棋 (后手)");
    player_color = getchar(); // 获取玩家颜色选择
    while (!isdigit(player_color)
           || ((player_color -= '0') != 1 && player_color != 2)) { // 检查输入是否合法
        printf("[Warning] Illegal input, try again:\n"); // 输入非法，提示重新输入
        while (getchar() != '\n');                       // 清空输入缓冲区
        player_color = getchar();                        // 重新获取玩家颜色选择
    }
    // system("cls"); // 清屏命令，暂时注释

    /* 选择白棋则在天元先下 */
    if (player_color == WHITE) {     // 如果玩家选择白棋
        Update_ChessBoard(Row, Col); // 更新棋盘
        BoardShape[0][7] = 2187;     // 更新棋盘形状
        BoardShape[1][7] = 2187;
        BoardShape[2][14] = 2187;
        BoardShape[3][14] = 2187;
        Print_ChessBoard();   // 打印棋盘
        Print_LastLocation(); // 打印最后落子位置
        CHANGE_PLAYER;        // 切换到AI
    } else {
        MaxDepth = 3;       // 设置AI搜索深度
        Print_ChessBoard(); // 打印初始棋盘
        printf("等待 [黑方] Human (或对手AI) 落子：\n");
    }

    while (!(result = GetWinner(Row, Col)) && !IsChessBoardFull()) { // 检查是否有赢家或棋盘满
        if (CurrentPlayer == player_color) {                         // 如果当前玩家是人类
            Update_Player();                                         // 人类落子
            // IsUndo();
            //  system("cls"); // 清屏命令，暂时注释
        } else {         // 如果当前玩家是AI
            Update_AI(); // AI落子
            // system("cls"); // 清屏命令，暂时注释
            if (MaxDepth == 3) { MaxDepth = 9; } // 动态调整AI搜索深度
        }
        Print_ChessBoard();   // 打印更新后的棋盘
        Print_LastLocation(); // 打印最后落子的位置
        CHANGE_PLAYER;        // 切换到下一个玩家
    }
    free(g_move);       // 释放走法指针内存
    g_move = NULL;      // 重置走法指针
    Print_ChessBoard(); // 打印最终棋盘
    CHANGE_PLAYER;      // 切换回最终赢家
    Print_Winner();     // 打印赢家信息
}

/**
 * @brief 机机对战模式的主循环
 * @param none
 * @retval none
 */
void Computer_VS_Computer(void) {
    int result, step;       // 记录游戏结果和步数
    int time_b, time_w;     // 记录黑棋和白棋的思考时间
    clock_t start_t, end_t; // 记录开始和结束时间

    step = time_b = time_w = 0; // 初始化步数和思考时间

    // printf("Loading...\n");
    Init_Hash();      // 初始化评分哈希表
    Init_BitBoard();  // 初始化位棋盘
    Init_MoveTable(); // 初始化走法表

    ChessBoard[Row][Col] = CurrentPlayer + 2; // 在棋盘上设置初始位置
    BitBoard[Col] &= BitSet[Row];             // 更新位棋盘
    set_bit_board(Row, Col);                  // 设置位棋盘
    BoardShape[0][7] = 2187;                  // 更新棋盘形状
    BoardShape[1][7] = 2187;
    BoardShape[2][14] = 2187;
    BoardShape[3][14] = 2187;

    Print_ChessBoard();   // 打印初始棋盘
    Print_LastLocation(); // 打印最后落子的位置
    CHANGE_PLAYER;        // 切换到下一个玩家

    while (!(result = GetWinner(Row, Col)) && !IsChessBoardFull()) { // 检查是否有赢家或棋盘满
        start_t = clock();                                           // 记录AI开始思考时间
        Update_AI();                                                 // AI落子
        end_t = clock();                                             // 记录AI结束思考时间
        Print_ChessBoard();                                          // 打印更新后的棋盘
        Print_LastLocation();                                        // 打印最后落子的位置
        CHANGE_PLAYER;                                               // 切换到下一个玩家
    }
    free(g_move);       // 释放走法指针内存
    g_move = NULL;      // 重置走法指针
    Print_ChessBoard(); // 打印最终棋盘
    CHANGE_PLAYER;      // 切换回最终赢家
    Print_Winner();     // 打印赢家信息
}

/**
 * @brief 选择游戏模式
 * @param none
 * @retval none
 */
void ChooseYourMode(void) {
    puts("----------------------------------------------------"); // 分割线
    puts(" 共三种游戏模式：");                                    // 显示游戏模式选项
    puts("     0.   Human  VS  Human   \t(人人对战, 有禁手)");    // 人人对战模式
    puts("     1. Human VS Computer \t(人机对战, 有禁手)");       // 人机对战模式
    puts("     2. Computer VS Computer \t(机机对战, 有禁手)");    // 机机对战模式
    puts(" Choose your game mode: ");                             // 提示用户选择模式
    puts("----------------------------------------------------"); // 分割线
}

/*                                                  */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */