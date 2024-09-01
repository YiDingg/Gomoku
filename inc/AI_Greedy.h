#include "Gomoku.h"
#include <stdbool.h> // bool 类型

#if !defined(__AI_GREADY_H)
#define __AI_GREADY_H

/* ------------------------------------------------ */
/* >> ----------------- 宏定义 ----------------- << */
/*                                                  */
/*                                                  */
/* >> ----------------- 宏定义 ----------------- << */
/* ------------------------------------------------ */

/* ----------------------------------------------- */
/* >> ---------------- 数据类型 ---------------- << */
/*                                                 */

/* score 评分函数的评分规则 */
typedef enum {
    score_BothBM = 0,
    score_B1 = +3,
    score_0BB00_00BB0_0B0B0 = +30,
    score_B2 = +20,
    score_0BBB0 = +2000,
    score_B3 = +1000,
    score_B4 = +100000,
    score_B5 = +1000000,
    score_W1 = -score_B1,
    score_0WW00_00WW0_0W0W0 = -score_0BB00_00BB0_0B0B0,
    score_W2 = -score_B2,
    score_0WWW0 = -score_0BBB0,
    score_W3 = -score_B3,
    score_W4 = -score_B4,
    score_W5 = -score_B5,
} Enum_ScoreRule;
/* 带 value 值的棋盘位置结构体 */
typedef struct {
    Struct_Location location;
    int value;
} Struct_LocationWithValue;
/*                                                 */
/* >> ---------------- 数据类型 ---------------- << */
/* ----------------------------------------------- */

/* ------------------------------------------------- */
/* >> --------------- 全局变量声明 --------------- << */
/*                                                   */

const char DefultValuesOfChessboard[ROW][COLUMN];
/*                                                   */
/* >> --------------- 全局变量声明 --------------- << */
/* ------------------------------------------------- */

/* ------------------------------------------------- */
/* >> ----------------- 函数声明 ----------------- << */
/*                                                   */
void GetChess_AI_Greedy(
    Struct_Location* p_location,
    Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me);
int EvaluateChessboard(const Enum_Color chessboard[ROW][COLUMN], const Enum_Color me);
int GetScoreOfThisLocation(
    const Enum_Color chessboard[ROW][COLUMN],
    const Struct_Location location,
    const Enum_Color me);
int GetScoreOfFiveChess(const Enum_Color FiveChess[5], const Enum_Color me);
bool IsFourEnemyInFive(
    Struct_Location five_locations[5],
    Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me,
    Struct_Location* p_location);
void ShellSort_LocationWithValue(Struct_LocationWithValue* locs_with_values, int size);
void ExchangeTwoInt(int* a, int* b);
/*                                                   */
/* >> ----------------- 函数声明 ----------------- << */
/* ------------------------------------------------- */
#endif // __AI_GREADY_H
