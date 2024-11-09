#include <stdio.h>
#include "BitBoard.h"
#include "Gomoku.h"

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */
unsigned long long move_table[MOVE_NUM]; // 走法缓存表

/* 每次下棋都将对应的模板在相应的5x5区域内作或操作*/
const Line bit_move_set[LENGTH][3] = {
    0b0111000000000000, 0b0110000000000000, 0b0101000000000000,
    0b0111100000000000, 0b0111000000000000, 0b0010100000000000,
    0b0111110000000000, 0b0011100000000000, 0b0101010000000000,
    0b0011111000000000, 0b0001110000000000, 0b0010101000000000,
    0b0001111100000000, 0b0000111000000000, 0b0001010100000000,
    0b0000111110000000, 0b0000011100000000, 0b0000101010000000,
    0b0000011111000000, 0b0000001110000000, 0b0000010101000000,
    0b0000001111100000, 0b0000000111000000, 0b0000001010100000,
    0b0000000111110000, 0b0000000011100000, 0b0000000101010000,
    0b0000000011111000, 0b0000000001110000, 0b0000000010101000,
    0b0000000001111100, 0b0000000000111000, 0b0000000001010100,
    0b0000000000111110, 0b0000000000011100, 0b0000000000101010,
    0b0000000000011111, 0b0000000000001110, 0b0000000000010101,
    0b0000000000001111, 0b0000000000000111, 0b0000000000001010,
    0b0000000000000111, 0b0000000000000011, 0b0000000000000101,
};

const Line BitSet[LENGTH] = {
    0b1011111111111111, 0b1101111111111111, 0b1110111111111111,
    0b1111011111111111, 0b1111101111111111, 0b1111110111111111,
    0b1111111011111111, 0b1111111101111111, 0b1111111110111111,
    0b1111111111011111, 0b1111111111101111, 0b1111111111110111,
    0b1111111111111011, 0b1111111111111101, 0b1111111111111110};

Line BitBoard[LENGTH] = {0};
Line bit_move_board[LENGTH] = {0};

/*                                                  */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/* ------------------------------------------------ */

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/*                                                  */

int QuickSelect(Tree move_set, int left, int right);

/**
 * @brief 初始化 BitBoard, 全部置零
 */
void Init_BitBoard(void) {
    for (int i = 0; i < LENGTH; i++) // 遍历 BitBoard 的每一行
        BitBoard[i] = ~0;            // 将每一行置为全 1
}

/**
 * @brief 初始化 move_table
 */
void Init_MoveTable(void) {
    int index1, index2, temp; // 定义循环变量和临时变量
    unsigned long long val;   // 定义一个 64 位无符号整数

    g_move = (Tree)malloc(sizeof(Node) * 2); // 分配两个 Node 的内存
    g_move->i = 7;                           // 设置初始位置 i
    g_move->j = 7;                           // 设置初始位置 j
    g_move->left = LEFT_I(7, 7);             // 设置初始 left 值
    g_move->right = RIGHT_I(7, 7);           // 设置初始 right 值
    for (index1 = 0; index1 < 4; index1++)   // 初始化 shape 数组
        g_move->shape[index1] = 0;
    (g_move + 1)->j = NULLPOSITION; // 设置第二个 Node 的 j 值为 NULLPOSITION
    g_move->point_score = 4;        // 设置初始分数

    for (index1 = 0; index1 < MOVE_NUM; index1++) { // 遍历所有可能的走法
        temp = index1; // 将当前索引赋值给 temp
        val = 0;       // 初始化 val
        for (index2 = LENGTH - 1; index2 >= 0; index2--) { // 遍历每一行
            if (temp & 0x1) {   // 如果 temp 的最低位为 1
                val = val << 4; // 将 val 左移 4 位
                val += index2;  // 将当前行号加到 val 中
            }
            temp = temp >> 1; // 将 temp 右移 1 位
        }
        move_table[index1] = val; // 将计算结果存入 move_table
    }
}

/**
 * @brief 获取走法
 * @param i 当前行
 * @param j 当前列
 * @param depth 当前深度
 * @param last_buf 上一步的缓冲区
 * @param last_move 上一步的走法
 * @retval 返回新的走法集合
 */
Tree GetMovement(int i, int j, int depth, Line* last_buf, Tree last_move) {
    Time_begin = clock(); // 记录开始时间
    Tree move_set, p, q;  // 定义走法集合和两个指针
    int index;            // 定义索引变量

    move_set = (Tree)malloc(sizeof(Node) * 200); // 分配足够大的内存
    index = get_new_move(move_set, last_buf, j, depth); // 获取新走法
    NodeNum += index;                                   // 更新节点数

    for (p = move_set + index, q = last_move;
         (q->i != i || q->j != j) && q->j != NULLPOSITION; p++, q++, index++) {
        p->i = q->i;         // 复制 i 值
        p->j = q->j;         // 复制 j 值
        p->left = q->left;   // 复制 left 值
        p->right = q->right; // 复制 right 值
        Get_Value(p);        // 获取分数
    }
    if (q->j != NULLPOSITION) { // 如果 q 还没有到达末尾
        q++;
        for (; q->j != NULLPOSITION; p++, q++, index++) {
            p->i = q->i;         // 复制 i 值
            p->j = q->j;         // 复制 j 值
            p->left = q->left;   // 复制 left 值
            p->right = q->right; // 复制 right 值
            Get_Value(p);        // 获取分数
        }
    }

    p->j = NULLPOSITION;                // 设置末尾标志
    Time_end = clock();                 // 记录结束时间
    Time_gene += Time_end - Time_begin; // 更新生成时间
    Time_begin = clock();               // 记录开始时间
    if (index > 15) {                   // 快速排序
        QuickSort(move_set, 0, QuickSelect(move_set, 0, index - 1));
    } else {
        QuickSort(move_set, 0, index - 1);
    }

    Time_end = clock();                 // 记录结束时间
    Time_sort += Time_end - Time_begin; // 更新排序时间

    return move_set; // 返回走法集合
}

/**
 * @brief 根据缓存的 buf 和当前位置 5*5 取余内取异或获取新走法, 返回新走法数目
 * @param move_set 走法集合
 * @param last_buf 上一步的缓冲区
 * @param j 当前列
 * @param depth 当前深度
 * @retval 返回新走法数目
 */
int get_new_move(Tree move_set, Line* last_buf, int j, int depth) {
    int start_j, end_j, i, index; // 定义变量
    unsigned long long val;       // 定义 64 位无符号整数
    Line new_move;                // 定义新走法

    index = 0;                      // 初始化索引
    get_range(&start_j, &end_j, j); // 获取范围
    for (; start_j != end_j; start_j++) {
        new_move = (bit_move_board[start_j] ^ last_buf[start_j])
                   & BitBoard[start_j]; // 计算新走法
        val = move_table[new_move];     // 获取走法
        while (val) {
            i = val & 0b1111;                            // 获取最低 4 位
            move_set[index].i = i;                       // 设置 i 值
            move_set[index].j = start_j;                 // 设置 j 值
            move_set[index].left = LEFT_I(i, start_j);   // 设置 left 值
            move_set[index].right = RIGHT_I(i, start_j); // 设置 right 值
            Get_Value(move_set + index);                 // 获取分数
            val = val >> 4;                              // 右移 4 位
            index++;                                     // 更新索引
        }
    }

    return index; // 返回新走法数目
}

/**
 * @brief ij 位置下棋后对位棋盘和走法棋盘进行改动
 * @param i 当前行
 * @param j 当前列
 */
void set_bit_board(int i, int j) {
    int start_j, end_j;  // 定义范围变量
    int temp = j, index; // 定义临时变量和索引

    get_range(&start_j, &end_j, j); // 获取范围
    start_j--;
    for (index = 0; temp != end_j; temp++, index++) {
        bit_move_board[temp] |= bit_move_set[i][index]; // 更新走法棋盘
        bit_move_board[temp] &= BitBoard[temp];         // 更新位棋盘
    }
    for (index = 1, j--; j != start_j; j--, index++) {
        bit_move_board[j] |= bit_move_set[i][index]; // 更新走法棋盘
        bit_move_board[j] &= BitBoard[j];            // 更新位棋盘
    }
}

/**
 * @brief 恢复 move_board
 * @param j 当前列
 * @param buf 缓冲区
 */
void re_move_board(int j, Line* buf) {
    int start_j, end_j; // 定义范围变量

    get_range(&start_j, &end_j, j); // 获取范围
    for (; start_j != end_j; start_j++)
        bit_move_board[start_j] = buf[start_j]; // 恢复走法棋盘
}

/**
 * @brief 缓存 move_board
 * @param buf 缓冲区
 */
void buf_move_board(Line* buf) {
    for (int i = 0; i < LENGTH; i++) buf[i] = bit_move_board[i]; // 缓存走法棋盘
}

/**
 * @brief 获取 j 的范围（主要是为了考虑边界的情况）
 * @param pstart_j 起始列指针
 * @param pend_j 结束列指针
 * @param j 当前列
 */
void get_range(int* pstart_j, int* pend_j, int j) {
    switch (j) {
    case 0:
        *pstart_j = 0; // 设置起始列
        *pend_j = 3;   // 设置结束列
        break;
    case 1:
        *pstart_j = 0; // 设置起始列
        *pend_j = 4;   // 设置结束列
        break;
    case 13:
        *pstart_j = 11; // 设置起始列
        *pend_j = 15;   // 设置结束列
        break;
    case 14:
        *pstart_j = 12; // 设置起始列
        *pend_j = 15;   // 设置结束列
        break;
    default:
        *pstart_j = j - 2; // 设置起始列
        *pend_j = j + 3;   // 设置结束列
        break;
    }
}

/**
 * @brief 判断该点是否与上一步在同一行 以此判断是否需要更新点的信息
 * @param i 当前行
 * @param j 当前列
 * @param p 上一步的走法
 * @retval 返回是否需要更新点的信息
 */
int is_change(int i, int j, Tree p) {
    if (p->point_score == NULL_SCORE || p->point_score == LOSE_SCORE)
        return 1; // 如果分数为 NULL_SCORE 或 LOSE_SCORE，则需要更新

    int now_i = p->i; // 获取上一步的 i 值
    int now_j = p->j; // 获取上一步的 j 值

    if ((now_i == i) || (now_j == j))
        return 1; // 如果在同一行或同一列，则需要更新

    if (abs(now_i - i) == abs(now_j - j))
        return 1; // 如果在同一对角线，则需要更新

    return 0; // 否则不需要更新
}
/*                                                  */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */

/*
 * 位棋盘 BitBoard 由 15 个 short（2 字节，16 位）构成，0 表示该位置已放置棋子
 * 在走法位棋盘 bit_move_board 中，1 表示当前有效的走法（两步内存在其他棋子）
 * 每次下棋时，将现有的 bit_move_set 模板与位棋盘进行异或操作以获取新的走法
 * 通过或操作来更新 bit_move_board
 * 预先缓存哈希表 move_table，依据索引直接获取该行可行走的位置
 * 在 move_table 中，每行的位棋盘值作为索引，直接获取可下棋位置的横坐标
 * 64 位 long long 可存储 16 个 16 进制数
 * 因此，可用其中 15 位来表示一行的 15 个位置是否可以下棋
 * 位棋盘速度极快，测试时生成一千万个节点约需 100ms
 */