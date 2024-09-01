#include <stdio.h>
#include "Gomoku.h"
#include "AI_Greedy.h"
#include <stdlib.h>  // exit()
#include <stdbool.h> // bool 类型
#include <time.h>    // time()

#define DEBUG 1

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */

/* 棋盘各位置的默认 value 值 */
const char DefultValuesOfChessboard[ROW][COLUMN] = {
    /* 棋盘不同位置的默认 value 值 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0},
    {0, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 0},
    {0, 2, 4, 4, 6, 6, 6, 6, 6, 6, 6, 4, 4, 2, 0},
    {0, 2, 4, 6, 6, 8, 8, 8, 8, 8, 6, 6, 4, 2, 0},
    {0, 2, 4, 6, 8, 8, 16, 16, 16, 8, 8, 6, 4, 2, 0},
    {0, 2, 4, 6, 8, 16, 20, 24, 20, 16, 8, 6, 4, 2, 0},
    {0, 2, 4, 6, 8, 16, 24, 30, 24, 16, 8, 6, 4, 2, 0},
    {0, 2, 4, 6, 8, 16, 20, 24, 20, 16, 8, 6, 4, 2, 0},
    {0, 2, 4, 6, 8, 8, 16, 16, 16, 8, 8, 6, 4, 2, 0},
    {0, 2, 4, 6, 6, 8, 8, 8, 8, 8, 6, 6, 4, 2, 0},
    {0, 2, 4, 4, 6, 6, 6, 6, 6, 6, 6, 4, 4, 2, 0},
    {0, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 0},
    {0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

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
void GetChess_AI_Greedy(
    Struct_Location* p_best_location,
    Enum_Color chessboard[ROW][COLUMN], /* EvaluateChessboard() 时会修改棋盘, 因此无 const */
    const Enum_Color me) {
    bool islegal_chessboard[ROW][COLUMN] = {false}; /* 用于记录棋盘上哪些位置可以落子 */
    int num_legal = 0; /* 统计可行位置数量（若无禁手则代表空位个数） */
    Enum_LegalOrIllegal islegal;
    Struct_Location location = {0, 0};
    Struct_Location p_five_locations[5]; /* 用于强制围堵检测 */

    /* 检查对方是否成三成四, 进行强制围堵 */
    for (char i = 0; i < ROW; i++) {
        for (char j = 0; j < COLUMN; j++) {
            // if (chessboard[i][j] == Blank) continue; /* 要进行成三检验, 因此不能跳过空位 */

            /* - 向右方 */
            if (j <= COLUMN - 5) {
                for (char k = 0; k < 5; k++) {
                    p_five_locations[k].row = i;
                    p_five_locations[k].column = j + k;
                }
                if (IsThreeOrFourEnemyInFive(p_five_locations, chessboard, me, p_best_location)) {
                    return;
                }
            }

            /* | 向下方 */
            if (i <= ROW - 5) {
                for (char k = 0; k < 5; k++) {
                    p_five_locations[k].row = i + k;
                    p_five_locations[k].column = j;
                }
                if (IsThreeOrFourEnemyInFive(p_five_locations, chessboard, me, p_best_location)) {
                    return;
                }
            }

            /* \ 向右下方 */
            if (i <= ROW - 5 && j <= COLUMN - 5) {
                for (char k = 0; k < 5; k++) {
                    p_five_locations[k].row = i + k;
                    p_five_locations[k].column = j + k;
                }
                if (IsThreeOrFourEnemyInFive(p_five_locations, chessboard, me, p_best_location)) {
                    return;
                }
            }

            /* / 向左下方 */
            if (i <= ROW - 5 && j >= 4) {
                for (char k = 0; k < 5; k++) {
                    p_five_locations[k].row = i + k;
                    p_five_locations[k].column = j - k;
                }
                if (IsThreeOrFourEnemyInFive(p_five_locations, chessboard, me, p_best_location)) {
                    return;
                }
            }
        }
    }

    /* 统计棋盘可行域 */
    for (char i = 0; i < ROW; i++) {
        for (char j = 0; j < COLUMN; j++) {
            location.row = i;
            location.column = j;
            CheckThisLocation(&islegal, chessboard, &location, me);
            if (islegal == Legal) {
                islegal_chessboard[i][j] = true;
                num_legal++;
            }
        }
    }
    if (DEBUG) {
        printf("统计棋盘可行域完成, num_legal = %d，开始申请内容并记录可行域信息\n", num_legal);
    }

    /* 申请内存 */
    if (DEBUG) {
        printf(
            "准备申请内存, num_legal = %d, sizeof(Struct_LocationWithValue) = %d",
            num_legal,
            sizeof(Struct_LocationWithValue));
        if (DEBUG == 2) { system("pause"); }
    }
    Struct_LocationWithValue* p_legal_locs_with_value = malloc(
        num_legal * sizeof(Struct_LocationWithValue)); /* malloc() 不会对所分配的内存进行初始化 */
    if (p_legal_locs_with_value == NULL) {             /* 空指针检测 */
        puts("Error: p_legal_locs_with_value malloc failed!");
        exit(-1);
    }

    /* 遍历可行域 */
    if (num_legal == 0) {
        /* 棋盘上无可行位置, 平局 */
        puts("棋盘上无可行位置, 双方平局");
        exit(0);
    } else if (num_legal <= 2) { /* num_legal 较少的情形 */
        /* num_legal = 0 时满足第 1 条后立即跳出, 因此这里是 num_legal = 1,2 */
    } else {
        num_legal = 0; /* 用于 p_legal_locs_with_value 索引 */
        for (char i = 0; i < ROW; i++) {
            for (char j = 0; j < COLUMN; j++) {
                if (islegal_chessboard[i][j]) {
                    p_legal_locs_with_value[num_legal].location.row = i;
                    p_legal_locs_with_value[num_legal].location.column = j;
                    num_legal++;
                }
            }
        }
        if (DEBUG) {
            printf("可行域信息遍历完成, num_legal = %d, 开始计算 value\n", num_legal);
            if (DEBUG == 2) { system("pause"); }
        }

        /* 计算所有可行域的 value */
        for (int i = 0; i < num_legal; i++) {
            chessboard[p_legal_locs_with_value[i].location.row]
                      [p_legal_locs_with_value[i].location.column] = me;
            if (DEBUG) { printf("%d.", i); }
            if (DEBUG) {
                printf(
                    " (%d, %d) ",
                    p_legal_locs_with_value[i].location.row,
                    p_legal_locs_with_value[i].location.column);
            }
            p_legal_locs_with_value[i].value =
                EvaluateChessboard(chessboard, me)
                + DefultValuesOfChessboard[p_legal_locs_with_value[i].location.row]
                                          [p_legal_locs_with_value[i].location.column];

            chessboard[p_legal_locs_with_value[i].location.row]
                      [p_legal_locs_with_value[i].location.column] = Blank;
        }

        if (DEBUG) { printf("value 计算完成, 开始希尔排序, num_legal = %d \n", num_legal); }
        /* value 从大至小排序 */
        ShellSort_LocationWithValue(p_legal_locs_with_value, num_legal);
        if (DEBUG) {
            for (char i = 0; i < 5; i++) {
                printf(
                    "best %d. (%d, %d) value: %d\n",
                    i,
                    p_legal_locs_with_value[i].location.row,
                    p_legal_locs_with_value[i].location.column,
                    p_legal_locs_with_value[i].value);
            }
        }
    }

    /* 选择最终的落子位置 */
    srand((unsigned)time(NULL) + rand()); /* 随机数种子 */
    double rand_num = (double)rand() / RAND_MAX;
    /* 一半概率选第一, 一半概率选第三 */
    *p_best_location = p_legal_locs_with_value[(rand_num > 0.5) ? 0 : 2].location;

    /* 释放内存 */
    if (DEBUG) {
        printf("全部计算完成, 释放内存\n");
        if (DEBUG == 2) { system("pause"); }
    }
    free(p_legal_locs_with_value);

    return;
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
            if (DEBUG == 3) {
                if (GetScoreOfThisLocation(chessboard, location, me) != 0) {
                    printf(
                        "location: (%d, %d), score: %d\n",
                        location.row,
                        location.column,
                        GetScoreOfThisLocation(chessboard, location, me));
                }
            }
        }
    }
    if (DEBUG == 1) { printf("value: %d\n", value); }
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
        ExchangeTwoInt(&num_me, &num_enemy);
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

bool IsThreeOrFourEnemyInFive(
    Struct_Location five_locations[5],
    Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me,
    Struct_Location* p_location) {
    Enum_Color enemy = -me;
    char num_me = 0, num_enemy = 0;

    /* 统计对手棋子数 */
    for (char k = 0; k < 5; k++) {
        chessboard[five_locations[k].row][five_locations[k].column] == enemy ? num_enemy++ : 0;
    }
    if (num_enemy == 3) {
        if (DEBUG) {
            puts("num_enemy = 3");
            printf(
                "five_locations[0]= (%d, %c) \n",
                five_locations[0].row + 1,
                five_locations[0].column + 'A');
            printf(
                "chessboard[five_locations[0].row][five_locations[0].column] = %d\n",
                chessboard[five_locations[0].row][five_locations[0].column]);
            printf(
                "chessboard[five_locations[4].row][five_locations[4].column] = %d\n",
                chessboard[five_locations[4].row][five_locations[4].column]);
        }

        if (chessboard[five_locations[0].row][five_locations[0].column] == Blank
            && chessboard[five_locations[4].row][five_locations[4].column] == Blank) {
            if (DEBUG) { printf("成三而需要围堵 \n"); }
            Enum_LegalOrIllegal islegal_1, islegal_2;
            CheckThisLocation(&islegal_1, chessboard, &five_locations[0], me);
            CheckThisLocation(&islegal_2, chessboard, &five_locations[0], me);
            if (islegal_1 == Illegal && islegal_2 == Illegal) {
                printf("成三而无法围堵, %s 胜利! \n", (me == Black) ? "白棋" : "黑棋");
                exit(0);
            } else {
                if (DEBUG) { printf("islegal_1 = %d, islegal_2 = %d\n", islegal_1, islegal_2); }
                if (islegal_1 == Legal && islegal_2 == Legal) {
                    if (DEBUG) { puts("进入两种围堵的选择"); }
                    int value_1, value_2;
                    /* 计算两种围堵方式的优劣 */
                    chessboard[five_locations[0].row][five_locations[0].column] = me;
                    value_1 = EvaluateChessboard(chessboard, me);
                    chessboard[five_locations[0].row][five_locations[0].column] = Blank;
                    chessboard[five_locations[4].row][five_locations[4].column] = me;
                    value_2 = EvaluateChessboard(chessboard, me);
                    chessboard[five_locations[0].row][five_locations[0].column] = Blank;
                    /* 选择最佳方式 */
                    if (value_1 > value_2) {
                        if (DEBUG) {
                            printf(
                                "five_locations[0].row = %d, five_locations[0].column = %d \n",
                                five_locations[0].row,
                                five_locations[0].column);
                        }
                        p_location->row = five_locations[0].row;
                        p_location->column = five_locations[0].column;
                    } else {
                        if (DEBUG) {
                            printf(
                                "five_locations[4].row = %d, five_locations[4].column = %d \n",
                                five_locations[4].row,
                                five_locations[4].column);
                        }
                        p_location->row = five_locations[4].row;
                        p_location->column = five_locations[4].column;
                    }
                } else if (islegal_1 == Legal) {
                    p_location->row = five_locations[0].row;
                    p_location->column = five_locations[0].column;
                } else if (islegal_2 == Legal) {
                    p_location->row = five_locations[4].row;
                    p_location->column = five_locations[4].column;
                }
            }
            return true;
        }

        return false;
    } else if (num_enemy == 4) { /* 剩下一个是否为 Blank */
        for (char k = 0; k < 5; k++) {
            if (chessboard[five_locations[k].row][five_locations[k].column] == Blank) {
                p_location->row = five_locations[k].row;
                p_location->column = five_locations[k].column;
                if (DEBUG) {
                    printf(
                        "five_locations[k].row = %d, five_locations[k].column = %d \n",
                        five_locations[k].row,
                        five_locations[k].column);
                }
                return true;
            }
        }
    } else {
        return false;
    }
}

/**
 * @brief 对 locs_with_values 进行希尔排序 (倒序, 最大的在最前面)
 * @param locs_with_values 待排序的 Struct_LocationWithValue 数组
 * @param size 数组大小
 * @retval none
 */
void ShellSort_LocationWithValue(Struct_LocationWithValue* locs_with_values, int size) {
    if (DEBUG) {
        printf("开始希尔排序, size = %d\n", size);
        if (DEBUG == 2) { system("pause"); }
    }
    int step = size / 2;
    while (step > 0) {
        //!
        for (int i = step; i < size; ++i) {
            int tmp = locs_with_values[i].value;
            Struct_Location tmp_loc = locs_with_values[i].location;
            int j = i - step;
            while (j >= 0 && tmp > locs_with_values[j].value) {
                locs_with_values[j + step].value = locs_with_values[j].value;
                locs_with_values[j + step].location = locs_with_values[j].location;
                j -= step;
            }
            locs_with_values[j + step].value = tmp;
            locs_with_values[j + step].location = tmp_loc;
        }
        step = step / 2;
    }
}

/**
 * @brief 交换 int a 和 int b 的值
 * @retval none
 */
void ExchangeTwoInt(int* p_a, int* p_b) {
    *p_a = *p_a + *p_b;
    *p_b = *p_a - *p_b;
    *p_a = *p_a - *p_b;
}
/*                                                  */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */