#include <stdio.h>
#include "AI_PVS.h"
#include "Gomoku.h"

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */
int NodeNum = 0;                     // 搜索节点数目
int CurrentScore[2] = {4, 0};        // 当前得分
int NextScore[2] = {0, 0};           // 搜索完毕下完一步棋后的得分
int BoardShape[4][30] = {0};         // 四个方向所有棋形
int MaxDepth = MAXDEPTH;             // AI搜索的最大深度
Line LastLocation_bit[LENGTH] = {0}; // 缓存上一走法的比特棋盘
Tree PV_p;                           // 用于指向找到的最佳走法
Node Node_kill;                      // 用于存储杀棋节点
Node LastNode;                       // 存储最后一个节点
int Flag_timeout = 0;                // 超时标志

clock_t Time_gene, Time_sort; // 生成走法时间和排序时间
clock_t Time_begin, Time_end; // 开始和结束时间
clock_t start_t, end_t;       // 记录当前搜索的开始和结束时间

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/* ------------------------------------------------ */

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */

/**
 * @brief 玩家操作函数，获取玩家的移动并更新走法指针
 * @param none
 * @retval none
 */
void Get_PlayerMovement(void) {
    Tree head; // 定义头节点
    head = GetMovement(Row, Col, 0, LastLocation_bit, g_move); // 获取当前移动
    free(g_move);  // 释放之前的走法内存
    g_move = head; // 更新走法指针
}

/**
 * @brief Alpha-Beta 剪枝算法的负极大值实现，使用 PVS 剪枝
 * @param pNode 当前节点
 * @param last_buf 上一步的棋形缓冲区
 * @param last_move 上一步的移动指针
 * @param depth 当前搜索深度
 * @param alpha 当前最优下限
 * @param beta 当前最优上限
 * @retval best: 当前节点的最优值
 */
int AlphaBeta(
    Tree pNode, Line* last_buf, Tree last_move, int depth, int alpha,
    int beta) {
    Tree p, head;                           // 当前遍历的节点和头节点
    Line buf[LENGTH];                       // 棋形缓冲区
    int v, found_PV = 0, best = N_INFINITY; // 当前值，是否找到主变例，最佳值
    int shape_buf[4];                       // 棋形缓冲数组
    int i = pNode->i, j = pNode->j; // 当前节点的位置

    // 1. 如果当前玩家是白棋且得分超过阈值，判断是否禁手
    if (CurrentPlayer == WHITE && pNode->point_score > 3600) {
        if (forbid(i, j))
            return P_INFINITY; // 是禁手，返回正无穷
        else
            ChessBoard[i][j] = BLACK; // 否则放置黑棋
    }

    // 2. 判断是否已经五连，五连则返回负无穷
    if (GetWinner(i, j)) return N_INFINITY;

    // 3. 搜索深度耗尽，返回当前评分
    if (depth <= 0) {
        if (CurrentPlayer == BLACK)
            return CurrentScore[0] - CurrentScore[1];
        else
            return CurrentScore[1] - CurrentScore[0];
    }

    // 4. 获取可行走法并进行剪枝
    head = GetMovement(i, j, depth, last_buf, last_move); // 获取走法
    if (CurrentScore[BoolPlayer ^ 1] > 2000)
        AlphaBeta_cut_Four(head); // 如果敌方得分高，进行杀棋剪枝
    else
        AlphaBeta_cut_common(head); // 否则进行常规剪枝
    buf_move_board(buf);            // 缓存当前棋盘状态

    // 5. 遍历所有走法
    for (p = head; p->point_score != NULL_SCORE; p++) {
        NodeNum++;                  // 增加搜索节点数
        i = p->i;                   // 更新行
        j = p->j;                   // 更新列
        set_bit_board(i, j);        // 设置位棋盘
        SetThisPoint(p, shape_buf); // 更新评分

        if (found_PV) { // 如果找到主变例
            v = -AlphaBeta(
                p, buf, head, depth - 1, -alpha - 1, -alpha); // 进行PVS搜索
            if (v > alpha && v < beta)
                v = -AlphaBeta(
                    p, buf, head, depth - 1, -beta,
                    -alpha); // 如果在窗口内，重新搜索
        } else {
            v = -AlphaBeta(p, buf, head, depth - 1, -beta, -alpha); // 正常搜索
        }

        ResetThisLocation(p, shape_buf); // 重置当前位置
        re_move_board(j, buf);           // 恢复棋盘

        if (v > best) {       // 如果当前值超过最佳值
            best = v;         // 更新最佳值
            if (v > alpha) {  // 如果当前值超过alpha
                alpha = v;    // 更新alpha
                found_PV = 1; // 标记找到主变例
            }
            if (v >= beta) break; // 如果超过beta，剪枝
        }
    }

    // 6. 释放头节点内存并返回最佳值
    free(head);  // 释放内存
    head = NULL; // 重置头节点指针

    return best; // 返回最佳值
}

/**
 * @brief PVS 剪枝算法的入口，进行第一层搜索
 * @param head 当前走法链表的头节点
 * @param depth 当前搜索深度
 * @retval alpha: 当前搜索得到的最优值
 */
int PVS(Tree head, int depth) {
    Tree p;                 // 当前遍历的节点
    Line buf[LENGTH];       // 棋形缓冲区
    int i, j, v;            // 位置和当前值
    int alpha = N_INFINITY; // 初始alpha
    int found_PV = 0;       // 是否找到主变例
    int shape_buf[4];       // 棋形缓冲数组

    buf_move_board(buf); // 缓存当前棋盘状态
    for (p = head; p->point_score != NULL_SCORE; p++) {
        if (clock() - start_t > 14000) { // 检查是否超时
            Flag_timeout = 1;            // 设置超时标志
            break;                       // 退出循环
        }
        if (p->point_score == LOSE_SCORE) continue; // 跳过必败点
        i = p->i;                                   // 更新行
        j = p->j;                                   // 更新列
        set_bit_board(i, j);                        // 设置位棋盘
        SetThisPoint(p, shape_buf);                 // 更新评分

        if (found_PV) { // 如果找到主变例
            v = -AlphaBeta(
                p, buf, g_move, depth - 1, -alpha - 1, -alpha); // 进行PVS搜索
            if (v > alpha)
                v = -AlphaBeta(
                    p, buf, g_move, depth - 1, N_INFINITY, -alpha); // 重搜索
        } else {
            v = -AlphaBeta(
                p, buf, g_move, depth - 1, N_INFINITY, -alpha); // 正常搜索
        }

        if (v > alpha) {                    // 如果当前值超过alpha
            PV_p = p;                       // 更新最佳走法指针
            found_PV = 1;                   // 标记找到主变例
            alpha = v;                      // 更新alpha
            NextScore[0] = CurrentScore[0]; // 更新下一个得分
            NextScore[1] = CurrentScore[1];
        }
        if (v <= -100000) p->point_score = LOSE_SCORE; // 剔除必败点

        ResetThisLocation(p, shape_buf); // 重置当前位置
        re_move_board(j, buf);           // 恢复棋盘
    }

    return alpha; // 返回最优值
}

/**
 * @brief 获取 AI 的下棋位置，执行 PVS 搜索并更新棋盘
 * @param none
 * @retval none
 */
void GetChess_AI(void) {
    Tree head, p, buf_head; // 定义节点指针
    int val, now_depth;     // 当前值和搜索深度

    start_t = clock();         // 记录开始时间
    Time_gene = Time_sort = 0; // 重置生成和排序时间
    Flag_timeout = 0;          // 重置超时标志

    head = GetMovement(Row, Col, 0, LastLocation_bit, g_move); // 获取走法
    free(g_move);  // 释放之前的走法内存
    g_move = head; // 更新走法指针

    int kill_out = 0; // 初始化杀棋输出

    if (head->point_score >= 3000
        && CurrentScore[BoolPlayer ^ 1] < 2000) { // 如果得分高且对方得分低
        AlphaBeta_cut_Four(head);                 // 进行杀棋剪枝
        for (int kill = 1; kill <= KILL_DEPTH; kill += 2) { // 进行杀棋搜索
            if ((kill_out = PVS_kill(head, kill)) >= 100000)
                break;       // 如果成功，退出
            end_t = clock(); // 更新结束时间
        }
    }
    end_t = clock(); // 更新结束时间

    for (p = head; p->j != NULLPOSITION; p++) { // 遍历所有走法
        if (p->point_score == NULL_SCORE || p->point_score == LOSE_SCORE)
            Get_Value(p); // 获取走法的评分
    }

    if (kill_out < 100000) { // 如果杀棋失败
        if (CurrentScore[BoolPlayer ^ 1] > 2000)
            AlphaBeta_cut_Four(head); // 进行杀棋剪枝
        else
            AlphaBeta_cut_common(head); // 进行常规剪枝

        for (now_depth = 1; now_depth <= MaxDepth;
             now_depth += 1) {          // 迭代加深搜索
            val = PVS(head, now_depth); // 执行 PVS 搜索
            LastNode.i = PV_p->i;       // 更新最后节点
            LastNode.j = PV_p->j;
            LastNode.left = PV_p->left;
            LastNode.right = PV_p->right;
            for (int index = 0; index < 4; index++)
                LastNode.shape[index] = PV_p->shape[index];
            if (val >= 100000) break; // 如果找到成功，退出
        }
        if (Flag_timeout) { PV_p = &LastNode; } // 如果超时，使用最后节点
    } else {
        printf("算杀成功\n"); // 打印成功信息
    }

    end_t = clock();                // 更新结束时间
    CurrentScore[0] = NextScore[0]; // 更新当前得分
    CurrentScore[1] = NextScore[1];
    BoardShape[0][PV_p->j] = PV_p->shape[0]; // 更新棋形
    BoardShape[1][PV_p->i] = PV_p->shape[1];
    BoardShape[2][PV_p->left] = PV_p->shape[2];
    BoardShape[3][PV_p->right] = PV_p->shape[3];
    Row = PV_p->i; // 更新行
    Col = PV_p->j; // 更新列
    printf(
        "Current score: Black %d, White %d\n", CurrentScore[0],
        CurrentScore[1]); // 打印当前得分
    printf(
        "Total time spent: %.2f s\n", (end_t - start_t) / 1000.0); // 打印总用时
    NodeNum = 0; // 重置节点计数
}

/**
 * @brief 设置棋盘上的一个点，并更新相关评分和棋形
 * @param p 当前走法节点
 * @param buf 缓冲区数组，用于存储原始棋形
 * @retval none
 */
void SetThisPoint(Tree p, int* buf) {
    int i = p->i;              // 行索引
    int j = p->j;              // 列索引
    int num, index;            // 当前长度和循环变量
    buf[0] = BoardShape[0][j]; // 缓存当前棋形
    buf[1] = BoardShape[1][i];
    buf[2] = BoardShape[2][p->left];
    buf[3] = BoardShape[3][p->right];
    for (index = 0; index < 4; index++) { // 遍历四个方向
        if ((num = Get_Length(i, j, index)) < 5)
            continue; // 如果长度小于5，跳过
        CurrentScore[0] -= ScoreTable[0][num][buf[index]]; // 更新黑棋得分
        CurrentScore[0] += ScoreTable[0][num][p->shape[index]];
        CurrentScore[1] -= ScoreTable[1][num][buf[index]]; // 更新白棋得分
        CurrentScore[1] += ScoreTable[1][num][p->shape[index]];
    }
    BoardShape[0][j] = p->shape[0]; // 更新棋形
    BoardShape[1][i] = p->shape[1];
    BoardShape[2][p->left] = p->shape[2];
    BoardShape[3][p->right] = p->shape[3];
    ChessBoard[i][j] = CurrentPlayer; // 在棋盘上放置当前玩家的棋子
    BitBoard[j] &= BitSet[i];         // 更新位棋盘
    CHANGE_PLAYER;                    // 切换玩家
}

/**
 * @brief 重置当前位置，包括棋盘、棋形和评分的恢复
 * @param p 当前走法节点
 * @param buf 缓冲区数组，用于恢复原始棋形
 * @retval none
 */
void ResetThisLocation(Tree p, int* buf) {
    int i = p->i;              // 行索引
    int j = p->j;              // 列索引
    int num, index;            // 当前长度和循环变量
    CHANGE_PLAYER;             // 切换回来玩家
    ChessBoard[i][j] = 0;      // 从棋盘上移除棋子
    BitBoard[j] |= ~BitSet[i]; // 更新位棋盘
    BoardShape[0][j] = buf[0]; // 恢复棋形
    BoardShape[1][i] = buf[1];
    BoardShape[2][p->left] = buf[2];
    BoardShape[3][p->right] = buf[3];
    for (index = 0; index < 4; index++) { // 遍历四个方向
        if ((num = Get_Length(i, j, index)) < 5)
            continue; // 如果长度小于5，跳过
        CurrentScore[0] += ScoreTable[0][num][buf[index]]; // 恢复黑棋得分
        CurrentScore[0] -= ScoreTable[0][num][p->shape[index]];
        CurrentScore[1] += ScoreTable[1][num][buf[index]]; // 恢复白棋得分
        CurrentScore[1] -= ScoreTable[1][num][p->shape[index]];
    }
}

/**
 * @brief 剪枝算法，当场面上有冲四时，剔除必败走法
 * @param head 当前走法链表的头节点
 * @retval 1: 有必要剔除, 0: 无需剔除
 */
int AlphaBeta_cut_Four(Tree head) {
    Tree p; // 当前遍历的节点
    for (p = head; p->j != NULLPOSITION && (p->point_score >= 3000);
         p++);                   // 遍历高评分走法
    p->point_score = NULL_SCORE; // 标记为无效
    if (p - head > 0)
        return 1; // 如果有可剔除走法，返回1
    else
        return 0; // 否则返回0
}

/**
 * @brief 常规剪枝，当场面平静时，选取前20个评分最高的走法
 * @param head 当前走法链表的头节点
 * @retval none
 */
void AlphaBeta_cut_common(Tree head) {
    for (int index = 0; head->j != NULLPOSITION && index != 20;
         head++, index++);          // 遍历前20个走法
    head->point_score = NULL_SCORE; // 标记为无效
}

/**
 * @brief 简单的快速排序算法，按照点评分降序排序走法
 * @param move_set 要排序的走法集合
 * @param left 排序的起始索引
 * @param right 排序的结束索引
 * @retval none
 */
void QuickSort(Tree move_set, int left, int right) {
    int i, j, std; // 左右指针和标准值

    if (left > right) return; // 如果左指针超过右指针，退出

    i = left;                         // 初始化左指针
    j = right;                        // 初始化右指针
    std = move_set[left].point_score; // 选择第一个点作为标准值

    while (i != j) { // 当左右指针未相遇
        while (move_set[j].point_score <= std && j > i)
            j--; // 从右向左找到第一个大于标准值的点
        while (move_set[i].point_score >= std && j > i)
            i++; // 从左向右找到第一个小于标准值的点
        if (j > i) ExchangeTowNode(move_set, i, j); // 交换两个点的位置
    }
    ExchangeTowNode(move_set, i, left); // 交换标准值到正确的位置

    QuickSort(move_set, left, i - 1);  // 递归排序左半部分
    QuickSort(move_set, i + 1, right); // 递归排序右半部分
}

/**
 * @brief 快速选择算法，选择前k大的走法
 * @param move_set 要选择的走法集合
 * @param left 选择的起始索引
 * @param right 选择的结束索引
 * @retval i: 选择的位置索引
 */
int QuickSelect(Tree move_set, int left, int right) {
    int i, j, std; // 左右指针和标准值

    i = left;                         // 初始化左指针
    j = right;                        // 初始化右指针
    std = move_set[left].point_score; // 选择第一个点作为标准值

    while (i != j) { // 当左右指针未相遇
        while (move_set[j].point_score <= std
               && j > i) // 从右向左找到第一个大于标准值的点
            j--;
        while (move_set[i].point_score >= std
               && j > i) // 从左向右找到第一个小于标准值的点
            i++;
        if (j > i) ExchangeTowNode(move_set, i, j); // 交换两个点的位置
    }

    ExchangeTowNode(move_set, i, left); // 交换标准值到正确的位置

    if (left < right) { // 如果有子区间需要选择
        if (i < 15)     // 如果选择的位置在前15个
            i = QuickSelect(move_set, i + 1, right); // 继续右侧选择
        else if (i > 20) // 如果选择的位置在后20个
            i = QuickSelect(move_set, left, i - 1); // 继续左侧选择
        else
            return i; // 返回选择的位置
    }
}

/**
 * @brief 交换两个节点的位置
 * @param move_set 走法集合
 * @param i 要交换的第一个节点索引
 * @param j 要交换的第二个节点索引
 * @retval none
 */
void ExchangeTowNode(Tree move_set, int i, int j) {
    Node temp;                 // 临时节点用于交换
    temp = move_set[i];        // 交换第一个节点
    move_set[i] = move_set[j]; // 交换第二个节点
    move_set[j] = temp;        // 完成交换
}

/**
 * @brief 只搜索冲棋走法的 alpha-beta 搜索，用于判断能否胜利
 * @param pNode 当前节点
 * @param last_buf 上一步的棋形缓冲区
 * @param last_move 上一步的移动指针
 * @param depth 当前搜索深度
 * @param alpha 当前最优下限
 * @param beta 当前最优上限
 * @retval best: 当前节点的最优值
 */
int AlphaBeta_kill(
    Tree pNode, Line* last_buf, Tree last_move, int depth, int alpha,
    int beta) {
    Tree p, head;                   // 当前遍历的节点和头节点
    Line buf[LENGTH];               // 棋形缓冲区
    int v, score;                   // 当前值和评分
    int found_PV = 0;               // 是否找到主变例
    int best = N_INFINITY;          // 最佳值
    int i = pNode->i, j = pNode->j; // 当前节点的位置
    int shape_buf[4];               // 棋形缓冲数组

    // 得分够高的情况下判断禁手
    if (CurrentPlayer == WHITE && pNode->point_score > 3600) {
        if (forbid(i, j))
            return P_INFINITY; // 是禁手，返回正无穷
        else
            ChessBoard[i][j] = BLACK; // 否则放置黑棋
    }

    // 判断是否已经五连，五连则返回成功
    if (GetWinner(i, j)) return SUCCESS;

    // 搜索深度耗尽，返回失败标志
    if (depth == 0) { return -1; }

    // 获取走法
    head = GetMovement(i, j, depth, last_buf, last_move); // 获取走法
    if (!AlphaBeta_cut_Four(head)) { // 如果没有冲棋走法
        free(head);                  // 释放内存
        head = NULL;                 // 重置头节点指针
        return 0;                    // 返回0
    }
    buf_move_board(buf); // 缓存棋盘状态

    // 遍历所有走法
    for (p = head; p->point_score != NULL_SCORE; p++) {
        NodeNum++; // 增加节点计数
        i = p->i;  // 更新行
        j = p->j;  // 更新列

        set_bit_board(i, j);        // 设置位棋盘
        SetThisPoint(p, shape_buf); // 更新评分

        if (found_PV) { // 如果找到主变例
            v = -AlphaBeta_kill(
                p, buf, head, depth - 1, -alpha - 1, -alpha); // PVS搜索
            if (v > alpha && v < beta)
                v = -AlphaBeta_kill(
                    p, buf, head, depth - 1, -beta, -alpha); // 重搜索
        } else {
            v = -AlphaBeta_kill(
                p, buf, head, depth - 1, -beta, -alpha); // 正常搜索
        }

        if (v > best) {       // 如果当前值超过最佳值
            best = v;         // 更新最佳值
            if (v > alpha) {  // 如果当前值超过alpha
                alpha = v;    // 更新alpha
                found_PV = 1; // 标记找到主变例
            }
            if (v >= beta) {                     // 如果超过beta，剪枝
                ResetThisLocation(p, shape_buf); // 重置当前位置
                re_move_board(j, buf);           // 恢复棋盘
                break;                           // 退出循环
            }
        }
        ResetThisLocation(p, shape_buf); // 重置当前位置
        re_move_board(j, buf);           // 恢复棋盘
    }
    free(head);  // 释放内存
    head = NULL; // 重置头节点指针

    return best; // 返回最佳值
}

/**
 * @brief PVS 剪枝算法的杀棋搜索，判断是否可以立即获胜
 * @param head 当前走法链表的头节点
 * @param depth 当前搜索深度
 * @retval alpha: 当前搜索得到的最优值
 */
int PVS_kill(Tree head, int depth) {
    Tree p;           // 当前遍历的节点
    Line buf[LENGTH]; // 棋形缓冲区
    int i, j, v;      // 位置和当前值
    int alpha = 0;    // 初始alpha
    int found_PV = 0; // 是否找到主变例
    int shape_buf[4]; // 棋形缓冲数组

    buf_move_board(buf); // 缓存棋盘状态
    for (p = head; p->point_score != NULL_SCORE; p++) {
        if (clock() - start_t > 7500) break;        // 检查是否超时
        if (p->point_score == LOSE_SCORE) continue; // 跳过必败点
        i = p->i;                                   // 更新行
        j = p->j;                                   // 更新列
        set_bit_board(i, j);                        // 设置位棋盘
        SetThisPoint(p, shape_buf);                 // 更新评分

        if (found_PV) { // 如果找到主变例
            v = -AlphaBeta_kill(
                p, buf, g_move, depth - 1, -alpha - 1, -alpha); // PVS搜索
            if (v > alpha)
                v = -AlphaBeta_kill(
                    p, buf, g_move, depth - 1, N_INFINITY, -alpha); // 重搜索
        } else {
            v = -AlphaBeta_kill(
                p, buf, g_move, depth - 1, N_INFINITY, -alpha); // 正常搜索
        }

        if (v > alpha) {        // 如果当前值超过alpha
            Node_kill.i = p->i; // 更新杀棋节点
            Node_kill.j = p->j;
            Node_kill.left = p->left;
            Node_kill.right = p->right;
            for (int index = 0; index < 4; index++)
                Node_kill.shape[index] = p->shape[index];
            PV_p = &Node_kill;              // 更新最佳走法指针
            NextScore[0] = CurrentScore[0]; // 更新下一个得分
            NextScore[1] = CurrentScore[1];
            found_PV = 1; // 标记找到主变例
            alpha = v;    // 更新alpha
        }
        if (v == 0) p->point_score = LOSE_SCORE; // 剔除必败点

        ResetThisLocation(p, shape_buf); // 重置当前位置
        re_move_board(j, buf);           // 恢复棋盘
    }

    return alpha; // 返回最优值
}

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */

/* 按照 Gomoku 禁手规则
 * 活三关键位置为两边不能为黑棋，否则长连。
 * 并且活四关键位置为中间不能是禁手(长连)。
 * 哈希匹配：将棋盘映射到三进制数组，规定长度一定时可以直接一一对应 */