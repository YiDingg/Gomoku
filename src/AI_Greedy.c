#include <stdio.h>
#include "Gomoku.h"
#include "AI_Greedy.h"
#include <stdlib.h>  // exit()
#include <stdbool.h> // bool 类型

#define DEBUG 0

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */
#if 0 
const char DefultValuesOfChessboard[ROW][COLUMN] = {
    // 棋盘不同位置的默认 value 值
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
#endif

/*                                                  */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/* ------------------------------------------------ */

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/*                                                  */

/**
 * @brief 根据给定棋局，计算我方最佳落子位置
 * @param p_best_location 用于记录最佳位置
 * @param chessboard 给定的棋局
 * @param me 我方的颜色
 * @retval none
 */
void GetBestLocation(
    Struct_Location* p_best_location,
    const Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me) {
    bool islegal_chessboard[ROW][COLUMN]; /* 用于记录棋盘上哪些位置可以落子 */
    Enum_LegalOrIllegal islegal;
    Struct_Location location = {0, 0};
    /* 遍历棋盘上哪些位置可以落子 */
    for (char i = 0; i < ROW; i++) {
        for (char j = 0; j < COLUMN; j++) {
            location.row = i;
            location.column = j;
            CheckThisLocation(&islegal, chessboard, &location, me);
            islegal_chessboard[i][j] = (islegal == Legal) ? true : false;
        }
    }
}

/**
 * @brief 评估给定棋局的价值 value
 * @param chessboard 给定的棋局
 * @retval value: 此棋局的价值
 */
int EvaluateChessboard(const Enum_Color chessboard[ROW][COLUMN], const Enum_Color me) {
    int value = 0;
    Struct_Location location = {0, 0};
    for (char i = 0; i < ROW; i++) {
        location.row = i;
        for (char j = 0; j < COLUMN; j++) {
            location.column = j;
            value += GetScoreOfThisLocation(chessboard, location, me);
            if (DEBUG == 2) {
                if (GetScoreOfThisLocation(chessboard, location, me) != 0) {
                    printf(
                        "location: (%d, %d), score: %d\n",
                        location.row,
                        location.column,
                        GetScoreOfThisLocation(chessboard, location, me));
                }
            }
            if (DEBUG == 1) { printf("value: %d\n", value); }
        }
    }
    return value;
}

/**
 * @brief 获取给定棋盘位置的 score_sum，由四部分构成，分别对应四个方向的 score
 * @param chessboard 给定的棋局
 * @param location 给定的棋盘位置
 * @retval score_sum: 给定棋盘位置的总分
 */
int GetScoreOfThisLocation(
    const Enum_Color chessboard[ROW][COLUMN],
    const Struct_Location location,
    const Enum_Color me) {
    int score_sum = 0;
    char i; /* 循环变量 */
    Enum_Color FiveChess[5];

    /* Right */
    if (location.column <= COLUMN - 5) {
        for (i = 0; i < 5; i++) { FiveChess[i] = chessboard[location.row][location.column + i]; }
        score_sum += GetScoreOfFiveChess(FiveChess, me);
    }
    /* Down */
    if (location.row <= ROW - 5) {
        for (i = 0; i < 5; i++) { FiveChess[i] = chessboard[location.row + i][location.column]; }
        score_sum += GetScoreOfFiveChess(FiveChess, me);
    }

    /* RightDown */
    if (location.row <= ROW - 5 && location.column <= COLUMN - 5) {
        for (i = 0; i < 5; i++) {
            FiveChess[i] = chessboard[location.row + i][location.column + i];
        }
        score_sum += GetScoreOfFiveChess(FiveChess, me);
    }

    /* LeftDown */
    if (location.row <= ROW - 5 && location.column >= 4) { /* 注意这里是 >= 4 而不是 >= 5*/
        for (i = 0; i < 5; i++) {
            FiveChess[i] = chessboard[location.row + i][location.column - i];
        }
        score_sum += GetScoreOfFiveChess(FiveChess, me);
    }
    return score_sum;
}

/**
 * @brief 给定连续的五元组，依据 Enum_ScoreRule 计算此五元组的得分 score
 * @param FiveChess 给定的连续的五元组
 * @retval score: 给定五元组的得分
 */
int GetScoreOfFiveChess(const Enum_Color FiveChess[5], const Enum_Color me) {
    int num_me = 0, num_enemy = 0; /* 使用了函数 ExchangeTwoInt, 因此声明为 int 而非 char */
    for (char i = 0; i < 5; i++) { /* 统计黑白棋子数量（默认我方为黑棋） */
        switch (FiveChess[i]) {
        case Black: num_me++; break;
        case White: num_enemy++; break;
        default: break;
        }
    }
    if (me != Black) { /* 如果我方为白棋，交换棋子数量，使得 num_me 代表白棋数量 */
        ExchangeTwoInt(num_me, num_enemy);
    }

    /* 返回匹配的 score 值 */
    if (num_me > 0 && num_enemy > 0) {
        return score_BothBM;
    } else if (num_me == 0 && num_enemy == 0) { /* 双方都没有棋子 */
        return 0;
    } else if (num_me > 0) { /* 我方有棋子 */
        switch (num_me) {
        case 1: return score_B1;
        case 2:
            if (FiveChess[0] == Blank && FiveChess[4] == Blank) {
                return score_0BB00_00BB0_0B0B0;
            } else {
                return score_B2;
            }
        case 3:
            if (FiveChess[0] == Blank && FiveChess[4] == Blank) {
                return score_0BBB0;
            } else {
                return score_B3;
            }
        case 4: return score_B4;
        case 5: return score_B5;
        default:
            printf("Error: num_me > 5\n");
            exit(1);
            break;
        }
    } else if (num_enemy > 0) { /* 敌方有棋子 */
        switch (num_enemy) {
        case 1: return score_W1;
        case 2:
            if (FiveChess[0] == Blank && FiveChess[4] == Blank) {
                return score_0WW00_00WW0_0W0W0;
            } else {
                return score_W2;
            }
        case 3:
            if (FiveChess[0] == Blank && FiveChess[4] == Blank) {
                return score_0WWW0;
            } else {
                return score_W3;
            }
        case 4: return score_W4;
        case 5: return score_W5;
        default:
            printf("Error: num_enemy > 5\n");
            exit(1);
            break;
        }
    }
}

/**
 * @brief 交换 int a 和 int b 的值
 * @retval none
 */
void ExchangeTwoInt(int a, int b) {
    a = a + b;
    b = a - b;
    a = a - b;
}
/*                                                  */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */