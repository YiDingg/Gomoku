#include "Gomoku.h"
#include "BitBoard.h"

#if !defined(__AI_PVS_H)
#define __AI_PVS_H

/* ------------------------------------------------ */
/* >> ----------------- 宏定义 ----------------- << */
/*                                                  */
#define MAXDEPTH 10         // 普通搜索深度
#define KILL_DEPTH 16       // 算杀搜索深度
#define P_INFINITY 2000000  // 评分上界
#define N_INFINITY -2000000 // 评分下界

#define FAIL 100000     // 定义失败标志
#define SUCCESS -100000 // 定义成功标志

/*                                                  */
/* >> ----------------- 宏定义 ----------------- << */
/* ------------------------------------------------ */

/* ----------------------------------------------- */
/* >> ---------------- 数据类型 ---------------- << */
/*                                                 */
/*                                                 */
/* >> ---------------- 数据类型 ---------------- << */
/* ----------------------------------------------- */

/* ------------------------------------------------- */
/* >> --------------- 全局变量声明 --------------- << */
/*                                                   */
/*                                                   */
/* >> --------------- 全局变量声明 --------------- << */
/* ------------------------------------------------- */

/* ------------------------------------------------- */
/* >> ----------------- 函数声明 ----------------- << */
/*                                                   */
int AlphaBeta(
    Tree pNode,
    unsigned short* last_buf,
    Tree last_move,
    int depth,
    int alpha,
    int beta); // alpha-beta剪枝算法
int AlphaBeta_kill(
    Tree pNode,
    unsigned short* last_buf,
    Tree last_move,
    int depth,
    int alpha,
    int beta);                      // alpha-beta剪枝算法
void GetChess_AI(void);             // ai操作入口
void Get_PlayerMovement(void);      // 读取对方操作进行一定的初始化修改
void SetThisPoint(Tree, int*);      // 下棋
void ResetThisLocation(Tree, int*); // 悔棋
int PVS_kill(Tree, int);
int PVS(Tree head, int depth);   // 主要变例搜索(PVS, Principal Variation Search)
void AlphaBeta_cut_common(Tree); // 未冲棋状态下走法裁剪
int AlphaBeta_cut_Four(Tree);    // 冲棋状态下只需要搜索封堵点或成四成五点
void ExchangeTowNode(Tree move_set, int i, int j); // 交换两个node的值，用于排序
int Get_Length(int, int, int);                     // 获得某行某方向上棋子数目
int QuickSelect(Tree move_set, int left, int right);
/*                                                   */
/* >> ----------------- 函数声明 ----------------- << */
/* ------------------------------------------------- */
#endif // __AI_PVS_H