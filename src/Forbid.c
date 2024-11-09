#include <stdio.h>
#include "Forbid.h"

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */
const int base[15] = {1,    3,     9,     27,     81,     243,     729,    2187,
                      6561, 19683, 59049, 177147, 531441, 1594323, 4782969};
/*                                                  */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/* ------------------------------------------------ */

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/*                                                  */

/**
 * @brief 判断在位置(i, j)是否构成禁手
 * @param i 行索引
 * @param j 列索引
 * @retval 1: 是禁手，0: 不是禁手
 */
int forbid(int i, int j) {
    ChessBoard[i][j] = BLACK;               /* 在棋盘上放置黑棋 */
    int num_four = 0;                       /* 活四的数量 */
    int num_three = 0;                      /* 活三的数量 */
    int max_length = 1;                     /* 当前最大连珠长度 */
    int line1 = Get_MaxLength(i, j, 1, 0);  /* 横向连珠长度 */
    int line2 = Get_MaxLength(i, j, 0, 1);  /* 纵向连珠长度 */
    int line3 = Get_MaxLength(i, j, 1, 1);  /* 主对角线连珠长度 */
    int line4 = Get_MaxLength(i, j, 1, -1); /* 副对角线连珠长度 */

    if (max_length < line1) max_length = line1; /* 更新最大连珠长度 */
    if (max_length < line2) max_length = line2; /* 更新最大连珠长度 */
    if (max_length < line3) max_length = line3; /* 更新最大连珠长度 */
    if (max_length < line4) max_length = line4; /* 更新最大连珠长度 */

    /* 先判断最简单的长连情况 */
    if (max_length > 5) {     /* 如果有超过5连 */
        ChessBoard[i][j] = 0; /* 撤销落子 */
        if (line1 == 5 || line2 == 5 || line3 == 5
            || line4 == 5) { /* 如果某个方向正好5连，不是禁手 */
            return 0;
        }

        else { /* 否则是禁手 */
            return 1;
        }
    }

    /* 四个方向判断活四 */
    if (isHuoFour(i, j, 1, 0)) num_four++; /* 检查横向是否有活四 */
    if (isHuoFour(i, j, 0, 1)) num_four++; /* 检查纵向是否有活四 */
    if (isHuoFour(i, j, 1, 1)) num_four++; /* 检查主对角线是否有活四 */
    if (isHuoFour(i, j, 1, -1)) num_four++; /* 检查副对角线是否有活四 */

    num_four += isChongFour(i, j, 1, 0) + isChongFour(i, j, 0, 1)
                + isChongFour(i, j, 1, 1)
                + isChongFour(i, j, 1, -1); /* 检查是否有冲四 */
    num_three += isHuoThree(i, j, 1, 0) + isHuoThree(i, j, 0, 1)
                 + isHuoThree(i, j, 1, 1)
                 + isHuoThree(i, j, 1, -1); /* 检查是否有活三 */

    if (num_four >= 2 || num_three >= 2) { /* 如果有两个活四或两个活三 */
        ChessBoard[i][j] = 0;              /* 撤销落子 */
        return 1;                          /* 是禁手 */
    } else {
        ChessBoard[i][j] = 0; /* 撤销落子 */
        return 0;             /* 不是禁手 */
    }
}

/**
 * @brief 判断是否形成活四
 * @param i 行索引
 * @param j 列索引
 * @param dx 行方向增量
 * @param dy 列方向增量
 * @retval 1: 是活四，0: 不是活四
 */
int isHuoFour(int i, int j, int dx, int dy) {
    int num;                /* 连续相同棋子的数量 */
    int now_i, now_j;       /* 当前检查的位置 */
    int l_i, l_j, r_i, r_j; /* 左边和右边的位置 */
    int l_edge = 1;         /* 左边边界标志 */
    int r_edge = 1;         /* 右边边界标志 */

    for (now_i = i, now_j = j, num = 0;
         ChessBoard[now_i][now_j] == BLACK && BothInRange_0_14(now_i, now_j);
         now_i += dx, now_j += dy, num++); /* 向一个方向数连续黑棋 */

    if (now_i == LENGTH || now_j == -1 || now_j == LENGTH) {
        l_edge = 0; /* 判断是否到达边界，如果到达则左边无边界 */
    } else {
        l_i = now_i; /* 左边的位置i */
        l_j = now_j; /* 左边的位置j */
    }

    for (now_i = i - dx, now_j = j - dy;
         BothInRange_0_14(now_i, now_j) && ChessBoard[now_i][now_j] == BLACK;
         now_i -= dx, now_j -= dy, num++); /* 向相反方向数连续黑棋 */

    if (now_i == -1 || now_j == -1 || now_j == LENGTH) {
        r_edge = 0; /* 判断是否到达边界，如果到达则右边无边界 */
    } else {
        r_i = now_i; /* 右边的位置i */
        r_j = now_j; /* 右边的位置j */
    }

    if (num != 4 || (!l_edge && !r_edge)) {
        /* 如果不是四连或两边均被堵，返回0 */
        return 0;
    }

    if ((r_edge) || (l_edge)) { /* 有一边开放，返回1 */
        return 1;
    } else { /* 两边都堵，返回0 */
        return 0;
    }
}

/**
 * @brief 判断是否形成冲四
 * @param i 行索引
 * @param j 列索引
 * @param dx 行方向增量
 * @param dy 列方向增量
 * @retval 冲四的数量
 */
int isChongFour(int i, int j, int dx, int dy) {
    int value, key_i, key_j, now_i, now_j, search_i, search_j; /* 变量定义 */
    int num = 0;        /* 冲四的数量 */
    int index1, index2; /* 循环变量 */
    index2 = 0;         /* 初始化循环计数 */

    for (now_i = i, now_j = j; index2 < 5 && BothInRange_0_14(now_i, now_j);
         now_i -= dx, now_j -= dy, index2++) { /* 检查五个方向 */
        value = 0;                             /* 初始化值 */
        search_i = now_i;                      /* 搜索起始点i */
        search_j = now_j;                      /* 搜索起始点j */
        for (index1 = 0; index1 < 5 && BothInRange_0_14(search_i, search_j);
             index1++, search_i += dx, search_j += dy) { /* 检查连续五个位置 */
            if (ChessBoard[search_i][search_j] == WHITE) { /* 遇到白棋，停止 */
                break;
            }

            else { /* 累积值 */
                value += ChessBoard[search_i][search_j] * base[index1];
            }
        }
        if (index1 == 5) {                      /* 如果五个位置连续 */
            if (value == chong_four_1) {        /* 特定的冲四模式1 */
                key_i = now_i + 2 * dx;         /* 冲四关键位置i */
                key_j = now_j + 2 * dy;         /* 冲四关键位置j */
                num++;                          /* 计数 */
            } else if (value == chong_four_2) { /* 特定的冲四模式2 */
                key_i = now_i + 3 * dx;         /* 冲四关键位置i */
                key_j = now_j + 3 * dy;         /* 冲四关键位置j */
                num++;                          /* 计数 */
            } else if (value == chong_four_3) { /* 特定的冲四模式3 */
                key_i = now_i + 1 * dx;         /* 冲四关键位置i */
                key_j = now_j + 1 * dy;         /* 冲四关键位置j */
                num++;                          /* 计数 */
            }
        }
    }
    return num; /* 返回冲四的数量 */
}

/**
 * @brief 判断是否形成活三
 * @param i 行索引
 * @param j 列索引
 * @param dx 行方向增量
 * @param dy 列方向增量
 * @retval 1: 是活三，0: 不是活三
 */
int isHuoThree(int i, int j, int dx, int dy) {
    int value, key_i, key_j, now_i, now_j, end_i, end_j, search_i,
        search_j;   /* 变量定义 */
    int index1;     /* 循环变量 */
    int index2 = 0; /* 初始化循环计数 */

    for (now_i = i - dx, now_j = j - dy;
         index2 < 4 && BothInRange_0_14(now_i, now_j);
         now_i -= dx, now_j -= dy, index2++) { /* 向一方向检查四个位置 */
        value = 0;                             /* 初始化值 */
        search_i = now_i - dx;                 /* 搜索位置i */
        search_j = now_j - dy;                 /* 搜索位置j */
        if (BothInRange_0_14(search_i, search_j)
            && ChessBoard[search_i][search_j] == BLACK)
            continue; /* 边上有黑棋，会因长连禁手无法构成活三，跳过 */

        search_i = now_i; /* 更新搜索位置i */
        search_j = now_j; /* 更新搜索位置j */
        for (index1 = 0; index1 < 6 && BothInRange_0_14(search_i, search_j);
             index1++, search_i += dx, search_j += dy) { /* 检查连续六个位置 */
            if (ChessBoard[search_i][search_j] == WHITE) { /* 遇到白棋，停止 */
                break;
            }

            else { /* 累积值 */
                value += ChessBoard[search_i][search_j] * base[index1];
            }
        }

        end_i = now_i + index1 * dx; /* 结束位置i */
        end_j = now_j + index1 * dy; /* 结束位置j */

        if (index1 < 6)
            continue; /* 如果未达到六个位置，跳过 */
        else if (
            !BothInRange_0_14(end_i, end_j)
            || ChessBoard[end_i][end_j]
                   != BLACK) { /* 如果结束位置不在范围或不是黑棋 */
            switch (value) {
            case huo_three1:
                key_i = now_i + dx * 1; /* 特定活三模式1的关键位置i */
                key_j = now_j + dy * 1; /* 特定活三模式1的关键位置j */
                if (!forbid(key_i, key_j))
                    return 1; /* 三三禁手没有同一条线上的情况，发现即可返回1 */
                break;
            case huo_three2:
                key_i = now_i + dx * 4; /* 特定活三模式2的关键位置i */
                key_j = now_j + dy * 4; /* 特定活三模式2的关键位置j */
                if (!forbid(key_i, key_j))
                    return 1; /* 三三禁手没有同一条线上的情况，发现即可返回1 */
                break;
            case huo_three3:
                key_i = now_i + dx * 2; /* 特定活三模式3的关键位置i */
                key_j = now_j + dy * 2; /* 特定活三模式3的关键位置j */
                if (!forbid(key_i, key_j))
                    return 1; /* 三三禁手没有同一条线上的情况，发现即可返回1 */
                break;
            case huo_three4:
                key_i = now_i + dx * 3; /* 特定活三模式4的关键位置i */
                key_j = now_j + dy * 3; /* 特定活三模式4的关键位置j */
                if (!forbid(key_i, key_j))
                    return 1; /* 三三禁手没有同一条线上的情况，发现即可返回1 */
                break;
            }
        }
    }
    return 0; /* 不是活三 */
}

/*                                                  */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */

/* 按照 Gomoku 禁手规则
 * 活三关键位置为两边不能为黑棋，否则长连。
 * 并且活四关键位置为中间不能是禁手(长连)。
 * 哈希匹配：将棋盘映射到三进制数组，规定长度一定时可以直接一一对应 */