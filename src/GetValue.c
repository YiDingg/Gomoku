#include <stdio.h>
#include <math.h>
#include "GetValue.h"

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */

/* 白棋黑棋评分权值
 * 最初的思想是将黑棋白棋相同棋形采用不同分
 * 不过最后还是成了相同的，这俩其实可以用一个数组 */
const int black_point[8] = {1, 10, 100, 2200, 10000, 100000}; // 黑棋评分权值
const int white_point[8] = {1, 10, 100, 2200, 10000, 100000}; // 白棋评分权值

/* 各长度评分表 */
int ScoreTable_5_w[243] = {0};       // 白棋长度5评分表
int ScoreTable_6_w[729] = {0};       // 白棋长度6评分表
int ScoreTable_7_w[2187] = {0};      // 白棋长度7评分表
int ScoreTable_8_w[6561] = {0};      // 白棋长度8评分表
int ScoreTable_9_w[19683] = {0};     // 白棋长度9评分表
int ScoreTable_10_w[59049] = {0};    // 白棋长度10评分表
int ScoreTable_11_w[177147] = {0};   // 白棋长度11评分表
int ScoreTable_12_w[531441] = {0};   // 白棋长度12评分表
int ScoreTable_13_w[1594323] = {0};  // 白棋长度13评分表
int ScoreTable_14_w[4782969] = {0};  // 白棋长度14评分表
int ScoreTable_15_w[14348907] = {0}; // 白棋长度15评分表
int ScoreTable_5_b[243] = {0};       // 黑棋长度5评分表
int ScoreTable_6_b[729] = {0};       // 黑棋长度6评分表
int ScoreTable_7_b[2187] = {0};      // 黑棋长度7评分表
int ScoreTable_8_b[6561] = {0};      // 黑棋长度8评分表
int ScoreTable_9_b[19683] = {0};     // 黑棋长度9评分表
int ScoreTable_10_b[59049] = {0};    // 黑棋长度10评分表
int ScoreTable_11_b[177147] = {0};   // 黑棋长度11评分表
int ScoreTable_12_b[531441] = {0};   // 黑棋长度12评分表
int ScoreTable_13_b[1594323] = {0};  // 黑棋长度13评分表
int ScoreTable_14_b[4782969] = {0};  // 黑棋长度14评分表
int ScoreTable_15_b[14348907] = {0}; // 黑棋长度15评分表

/* 存所有长度的哈希表的指针*/
int* ScoreTable[2][16] = {
    0,
    0,
    0,
    0,
    0,
    ScoreTable_5_b,
    ScoreTable_6_b,
    ScoreTable_7_b,
    ScoreTable_8_b,
    ScoreTable_9_b,
    ScoreTable_10_b,
    ScoreTable_11_b,
    ScoreTable_12_b,
    ScoreTable_13_b,
    ScoreTable_14_b,
    ScoreTable_15_b,
    0,
    0,
    0,
    0,
    0,
    ScoreTable_5_w,
    ScoreTable_6_w,
    ScoreTable_7_w,
    ScoreTable_8_w,
    ScoreTable_9_w,
    ScoreTable_10_w,
    ScoreTable_11_w,
    ScoreTable_12_w,
    ScoreTable_13_w,
    ScoreTable_14_w,
    ScoreTable_15_w}; // 哈希表指针数组

/* 四个函数指针存四个坐标变换函数 */
int (*transX[4])(int i, int j) = {
    Par_i, Ver_i, Left_i, Right_i}; // 行方向变换函数指针
int (*transY[4])(int i, int j) = {
    Par_j, Ver_j, Left_j, Right_j}; // 列方向变换函数指针

/*                                                  */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/* ------------------------------------------------ */

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/*                                                  */

/**
 * @brief 初始化所有长度的评分哈希表
 * @retval 无
 */
void Init_Hash(void) {
    int len;                          // 定义变量len
    for (len = 6; len <= 15; len++) { // 遍历长度从6到15
        Init_ScoreTable(len);         // 初始化对应长度的评分表
    }
    Init_ScoreTable_5(); // 初始化长度为5的评分表
}

/**
 * @brief 初始化指定长度的评分哈希表
 * @param len 评分表的长度
 * @retval 无
 */
void Init_ScoreTable(int len) {
    int* board = (int*)malloc(sizeof(int) * len); // 分配长度为len的board数组
    int i, j, temp;                               // 定义变量i, j, temp
    for (i = 0; i < pow(3, len); i++) {    // 遍历所有可能的棋盘状态
        temp = i;                          // 赋值临时变量temp
        for (j = 0; j < len; j++) {        // 遍历每一位
            board[len - 1 - j] = temp % 3; // 计算当前位的值
            temp /= 3;                     // 更新temp
        }
        Init_GetValue(board, i, len); // 初始化当前board状态的评分
    }
    free(board);  // 释放board内存
    board = NULL; // 将board指针设为NULL
}

/**
 * @brief 获取当前行棋子数目
 * @param i 当前行索引
 * @param j 当前列索引
 * @param index 方向索引
 * @retval num: 连续相同棋子的数量
 */
int Get_Length(int i, int j, int index) {
    int num;         // 定义变量num
    switch (index) { // 根据方向索引进行判断
    case 0:
    case 1: return 15; // 水平和垂直方向返回固定长度15
    case 2:
        if (i + j < LENGTH)
            return i + j + 1; // 斜向1的长度计算
        else
            return 29 - i - j; // 斜向1的长度计算
    case 3:
        if (i > j)
            return 15 + j - i; // 斜向2的长度计算
        else
            return 15 + i - j; // 斜向2的长度计算
    }
}

/**
 * @brief 获得下棋后节点p四个方向的shape以及该走法的评价(己方收益+对方收益)
 * @param p 指向Tree结构体的指针
 * @retval 无
 */
void Get_Value(Tree p) {
    int i = p->i;                  // 获取节点的行索引
    int j = p->j;                  // 获取节点的列索引
    int x, y, num;                 // 定义变量x, y, num
    int score, index, oppo;        // 定义变量score, index, oppo
    int b_player = BoolPlayer ^ 1; // 计算对手玩家标识

    score = 0;                            // 初始化评分
    for (index = 0; index < 4; index++) { // 遍历四个方向
        x = (transX[index])(i, j);        // 计算x方向的变换索引
        y = (transY[index])(i, j);        // 计算y方向的变换索引
        p->shape[index] =
            BoardShape[index][x] + CurrentPlayer * base[y]; // 更新节点的shape
        if ((num = Get_Length(i, j, index)) < 5)
            continue; // 获取长度，若小于5则跳过
        oppo = BoardShape[index][x]
               + (CurrentPlayer ^ 0b11) * base[y]; // 计算对手的shape
        // 计算攻击和防御评分
        score += (int)(1.5
                       * (ScoreTable[BoolPlayer][num][p->shape[index]]
                          - ScoreTable[BoolPlayer][num]
                                      [BoardShape[index][x]])); // 攻击评分
        score +=
            (ScoreTable[b_player][num][oppo]
             - ScoreTable[b_player][num][BoardShape[index][x]]); // 防御评分
    }
    p->point_score = score; // 更新节点的总评分
}

/**
 * @brief 初始化长度为5的评分哈希表
 * @retval 无
 */
void Init_ScoreTable_5(void) {
    int i;                      // 定义变量i
    for (i = 0; i < 243; i++) { // 遍历所有可能的状态
        switch (i) {            // 根据状态索引进行评分
        case 0: break;          // 状态0不评分
        case One_2w:
        case One_3w:
        case One_4w:
        case One_5w:
        case One_6w:
            ScoreTable_5_w[i] = 1; // 白棋特定状态评分设为1
            break;
        case One_2b:
        case One_3b:
        case One_4b:
        case One_5b:
        case One_6b:
            ScoreTable_5_b[i] = 1; // 黑棋特定状态评分设为1
            break;
        case 216:
        case 180:
        case 168:
        case 164:
        case 72:
        case 60:
        case 56:
        case 24:
        case 20:
        case 8:
            ScoreTable_5_w[i] = 10; // 白棋特定状态评分设为10
            break;
        case 108:
        case 90:
        case 84:
        case 82:
        case 36:
        case 30:
        case 28:
        case 12:
        case 10:
        case 4:
            ScoreTable_5_b[i] = 10; // 黑棋特定状态评分设为10
            break;
        case 234:
        case 222:
        case 218:
        case 186:
        case 182:
        case 170:
        case 78:
        case 74:
        case 62:
        case 26:
            ScoreTable_5_w[i] = 100; // 白棋特定状态评分设为100
            break;
        case 117:
        case 111:
        case 109:
        case 93:
        case 91:
        case 85:
        case 39:
        case 37:
        case 31:
        case 13:
            ScoreTable_5_b[i] = 100; // 黑棋特定状态评分设为100
            break;
        case 240:
        case 236:
        case 224:
        case 188:
        case 80:
            ScoreTable_5_w[i] = 2000; // 白棋特定状态评分设为2000
            break;
        case 120:
        case 118:
        case 112:
        case 94:
        case 40:
            ScoreTable_5_b[i] = 2000; // 黑棋特定状态评分设为2000
            break;
        case 242:
            ScoreTable_5_w[i] = 100000; // 白棋特定状态评分设为100000
            break;
        case 121:
            ScoreTable_5_b[i] = 100000; // 黑棋特定状态评分设为100000
            break;
        }
    }
}

/**
 * @brief 初始化并计算评分哈希表
 * @param board 棋盘状态数组
 * @param i 当前状态索引
 * @param len 棋盘长度
 * @retval int: 初始化结果
 */
int Init_GetValue(int* board, int i, int len) {
    int index, now_i;             // 定义变量index, now_i
    int value;                    // 定义变量value
    int black_evaluate[10] = {0}; // 黑棋评估数组初始化
    int white_evaluate[10] = {0}; // 白棋评估数组初始化
    int score_w = 0;              // 白棋总评分初始化
    int score_b = 0;              // 黑棋总评分初始化

    for (now_i = 0; now_i < len - 5; now_i++) { // 遍历棋盘
        value = 0;                              // 初始化当前值
        for (index = 0; index < 6; index++)     // 遍历连续6个位置
            value += board[now_i + index] * base[index]; // 计算当前评分值
        switch (value) { // 根据评分值进行评估
        case 0: break;   // 空状态不评分
        case One_1w:
        case One_2w:
        case One_3w:
        case One_4w:
        case One_bw:
        case One_cw:
        case One_dw:
            if (now_i + index > len - 1) white_evaluate[0]++; // 白棋评估
            break;
        case One_7w:
        case One_8w:
        case One_9w:
        case One_aw:
        case One_6w:
            white_evaluate[0]++; // 白棋评估
            break;
        case One_5w:
        case One_ew:
            white_evaluate[0]++; // 白棋评估
            now_i++;             // 更新索引
            break;
        case Two_1w:
        case Two_2w:
        case Two_3w:
        case Two_4w:
        case Two_jw:
        case Two_kw:
        case Two_lw:
        case Two_mw:
        case Two_nw:
        case Two_ow:
        case Two_pw:
        case Two_qw:
        case Two_rw:
        case Two_sw:
            if (now_i + index > len - 1) white_evaluate[1]++; // 白棋评估
            break;
        case Two_5w:
        case Two_6w:
        case Two_7w:
        case Two_8w:
        case Two_9w:
        case Two_aw:
        case Two_bw:
        case Two_cw:
        case Two_dw:
        case Two_ew:
        case Two_fw:
        case Two_gw:
        case Two_hw:
        case Two_iw:
            white_evaluate[1]++; // 白棋评估
            break;
        case HuoTwo_1w:
        case HuoTwo_2w:
        case HuoTwo_4w:
            if (now_i + index > len - 1 || board[now_i + index] == 1)
                white_evaluate[2]++; // 白棋活二评估
            break;
        case HuoTwo_3w:
        case HuoTwo_5w:
        case HuoTwo_6w:
            white_evaluate[2]++; // 白棋活二评估
            break;
        case Three_1w:
        case Three_2w:
        case Three_3w:
        case Three_4w:
        case Three_5w:
        case Three_6w:
        case Three_nw:
        case Three_ow:
        case Three_pw:
        case Three_qw:
        case Three_rw:
        case Three_sw:
        case Three_tw:
        case Three_uw:
        case Three_vw:
        case Three_ww:
            if (now_i + index > len - 1) white_evaluate[2]++; // 白棋活三评估
            break;
        case Three_7w:
        case Three_8w:
        case Three_9w:
        case Three_aw:
        case Three_bw:
        case Three_cw:
        case Three_dw:
        case Three_ew:
        case Three_fw:
        case Three_gw:
        case Three_hw:
        case Three_iw:
        case Three_jw:
        case Three_kw:
        case Three_lw:
        case Three_mw:
            white_evaluate[2]++; // 白棋活三评估
            break;
        case HuoThree_1w:
            if (now_i + index > len - 1 || board[now_i + index] == 1)
                white_evaluate[3]++; // 白棋活三评估
            break;
        case HuoThree_2w:
        case HuoThree_3w:
        case HuoThree_4w:
            white_evaluate[3]++; // 白棋活三评估
            break;
        case Four_1w:
        case Four_2w:
        case Four_3w:
        case Four_4w:
        case Four_dw:
        case Four_ew:
        case Four_fw:
        case Four_gw:
            if (now_i + index > len - 1) white_evaluate[3]++; // 白棋活四评估
            break;
        case Four_5w:
        case Four_6w:
        case Four_7w:
        case Four_8w:
        case Four_9w:
        case Four_aw:
        case Four_bw:
        case Four_cw:
            white_evaluate[3]++; // 白棋活四评估
            break;
        case HuoFour_w:
            white_evaluate[4]++; // 白棋活四评估
            break;
        case Five_w1:
        case Five_w2:
            if (now_i + index > len - 1) white_evaluate[5]++; // 白棋五连评估
            break;
        case Five_w3:
        case Five_w4:
        case Five_w5:
            white_evaluate[5]++; // 白棋五连评估
            break;
        case One_1b:
        case One_2b:
        case One_3b:
        case One_4b:
        case One_bb:
        case One_cb:
        case One_db:
            if (now_i + index > len - 1) black_evaluate[0]++; // 黑棋评估
            break;
        case One_7b:
        case One_8b:
        case One_9b:
        case One_ab:
        case One_6b:
            black_evaluate[0]++; // 黑棋评估
            break;
        case One_5b:
        case One_eb:
            black_evaluate[0]++; // 黑棋评估
            now_i++;             // 更新索引
            break;

        case Two_1b:
        case Two_2b:
        case Two_3b:
        case Two_4b:
        case Two_jb:
        case Two_kb:
        case Two_lb:
        case Two_mb:
        case Two_nb:
        case Two_ob:
        case Two_pb:
        case Two_qb:
        case Two_rb:
        case Two_sb:
            if (now_i + index > len - 1) black_evaluate[1]++; // 黑棋评估
            break;
        case Two_5b:
        case Two_6b:
        case Two_7b:
        case Two_8b:
        case Two_9b:
        case Two_ab:
        case Two_bb:
        case Two_cb:
        case Two_db:
        case Two_eb:
        case Two_fb:
        case Two_gb:
        case Two_hb:
        case Two_ib:
            black_evaluate[1]++; // 黑棋评估
            break;
        case HuoTwo_1b:
        case HuoTwo_2b:
        case HuoTwo_4b:
            if (now_i + index > len - 1 || board[now_i + index] == 1)
                black_evaluate[2]++; // 黑棋活二评估
            break;
        case HuoTwo_3b:
        case HuoTwo_5b:
        case HuoTwo_6b:
            black_evaluate[2]++; // 黑棋活二评估
            break;
        case Three_1b:
        case Three_2b:
        case Three_3b:
        case Three_4b:
        case Three_5b:
        case Three_6b:
        case Three_nb:
        case Three_ob:
        case Three_pb:
        case Three_qb:
        case Three_rb:
        case Three_sb:
        case Three_tb:
        case Three_ub:
        case Three_vb:
        case Three_wb:
            if (now_i + index > len - 1) black_evaluate[2]++; // 黑棋活三评估
            break;
        case Three_7b:
        case Three_8b:
        case Three_9b:
        case Three_ab:
        case Three_bb:
        case Three_cb:
        case Three_db:
        case Three_eb:
        case Three_fb:
        case Three_gb:
        case Three_hb:
        case Three_ib:
        case Three_jb:
        case Three_kb:
        case Three_lb:
        case Three_mb:
            black_evaluate[2]++; // 黑棋活三评估
            break;
        case HuoThree_1b:
            if (now_i + index > len - 1 || board[now_i + index] == 1)
                black_evaluate[3]++; // 黑棋活三评估
            break;
        case HuoThree_2b:
        case HuoThree_3b:
        case HuoThree_4b:
            black_evaluate[3]++; // 黑棋活三评估
            break;
        case Four_1b:
        case Four_2b:
        case Four_3b:
        case Four_4b:
        case Four_db:
        case Four_eb:
        case Four_fb:
        case Four_gb:
            if (now_i + index > len - 1) black_evaluate[3]++; // 黑棋活四评估
            break;
        case Four_5b:
        case Four_6b:
        case Four_7b:
        case Four_8b:
        case Four_9b:
        case Four_ab:
        case Four_bb:
        case Four_cb:
            black_evaluate[3]++; // 黑棋活四评估
            break;
        case HuoFour_b:
            black_evaluate[4]++; // 黑棋活四评估
            break;
        case Five_b1:
        case Five_b2:
            if (now_i + index > len - 1) black_evaluate[5]++; // 黑棋五连评估
            break;
        case Five_b3:
        case Five_b4:
            black_evaluate[5]++; // 黑棋五连评估
            break;
        case Five_b5:
            black_evaluate[6]++; // 黑棋特殊五连评估
            break;
        }
    }
    for (int j = 0; j < 6; j++) {                      // 遍历评估数组
        score_b += black_evaluate[j] * black_point[j]; // 计算黑棋总评分
        score_w += white_evaluate[j] * white_point[j]; // 计算白棋总评分
    }
    ScoreTable[0][len][i] += score_b; // 更新黑棋评分表
    ScoreTable[1][len][i] += score_w; // 更新白棋评分表
    if (black_evaluate[6])
        ScoreTable[0][len][i] = -2000000; // 特殊五连赋予高负分
}

/**
 * @brief 坐标变换函数 - 行方向
 * @param i 行索引
 * @param j 列索引
 * @retval 变换后的行索引
 */
int Par_i(int i, int j) {
    return PAR_I(i, j); // 调用宏或其他函数进行变换
}

/**
 * @brief 坐标变换函数 - 列方向
 * @param i 行索引
 * @param j 列索引
 * @retval 变换后的列索引
 */
int Par_j(int i, int j) {
    return PAR_J(i, j); // 调用宏或其他函数进行变换
}

/**
 * @brief 坐标变换函数 - 垂直行方向
 * @param i 行索引
 * @param j 列索引
 * @retval 变换后的垂直行索引
 */
int Ver_i(int i, int j) {
    return VER_I(i, j); // 调用宏或其他函数进行变换
}

/**
 * @brief 坐标变换函数 - 垂直列方向
 * @param i 行索引
 * @param j 列索引
 * @retval 变换后的垂直列索引
 */
int Ver_j(int i, int j) {
    return VER_J(i, j); // 调用宏或其他函数进行变换
}

/**
 * @brief 坐标变换函数 - 左斜行方向
 * @param i 行索引
 * @param j 列索引
 * @retval 变换后的左斜行索引
 */
int Left_i(int i, int j) {
    return LEFT_I(i, j); // 调用宏或其他函数进行变换
}

/**
 * @brief 坐标变换函数 - 左斜列方向
 * @param i 行索引
 * @param j 列索引
 * @retval 变换后的左斜列索引
 */
int Left_j(int i, int j) {
    return LEFT_J(i, j); // 调用宏或其他函数进行变换
}

/**
 * @brief 坐标变换函数 - 右斜行方向
 * @param i 行索引
 * @param j 列索引
 * @retval 变换后的右斜行索引
 */
int Right_i(int i, int j) {
    return RIGHT_I(i, j); // 调用宏或其他函数进行变换
}

/**
 * @brief 坐标变换函数 - 右斜列方向
 * @param i 行索引
 * @param j 列索引
 * @retval 变换后的右斜列索引
 */
int Right_j(int i, int j) {
    return RIGHT_J(i, j); // 调用宏或其他函数进行变换
}

/**
 * @brief 评分思路：
 * 评分以行为单位，只需72个int即可知道全局评分采用哈希表预先缓存5-15长度的所有可能的行的得分
 * 长度为5的直接穷举，长度6-15程序生成 总共约40mb内存 内存大就是任性
 * 缓存过程在开始游戏前，因此可以不做太多速度上的优化
 * 每次下棋只需要更新4个方向的int即可
 */