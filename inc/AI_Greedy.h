#include "Gomoku.h"

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

int EvaluateChessboard(const Enum_Color chessboard[ROW][COLUMN], const Enum_Color me);
int GetScoreOfThisLocation(
    const Enum_Color chessboard[ROW][COLUMN],
    const Struct_Location location,
    const Enum_Color me);
int GetScoreOfFiveChess(const Enum_Color FiveChess[5], const Enum_Color me);
void ExchangeTwoInt(int a, int b);
/*                                                   */
/* >> ----------------- 函数声明 ----------------- << */
/* ------------------------------------------------- */
#endif // __AI_GREADY_H