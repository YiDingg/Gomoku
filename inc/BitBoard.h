#include "Gomoku.h"
#include "GetValue.h"

#if !defined(__BITBOARD_H)
#define __BITBOARD_H

/* ------------------------------------------------ */
/* >> ----------------- 宏定义 ----------------- << */
/*                                                  */
#define MOVE_NUM 32768 // 2^15,表示一行15个子下棋位点的所有情况
#define NULL_SCORE 33554432 // 着法裁剪的标志
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
/* bit_board由15个16bit的Line构成，每一个bit为1表示为空，为0表示有子*/
extern Line BitBoard[];
/* bit_set是bit_board每一个下棋对应的模板，每次下棋都需要对应进行或操作*/
extern const Line BitSet[];
/* LastLocation_bit是上一步的缓存bit_move_board*/
extern Line LastLocation_bit[LENGTH];
/*                                                   */
/* >> --------------- 全局变量声明 --------------- << */
/* ------------------------------------------------- */

/* ------------------------------------------------- */
/* >> ----------------- 函数声明 ----------------- << */
/*                                                   */
void Init_MoveTable(void); // 初始化走法哈希表
void Init_BitBoard(void); // 初始化位棋盘，0表示已下棋，1表示未下棋
Tree GetMovement(
    int i, int j, int depth, Line* last_buf, Tree last_move); // 获取全部走法
int get_new_move(
    Tree move_set, Line* last_buf, int j, int depth); // 每下一步棋得到的新走法
void set_bit_board(int i, int j);                     // 在bit_board上下棋
void buf_move_board(Line* buf);       // 缓存上一步的bit_move_board
void re_move_board(int j, Line* buf); // 恢复上一步的bit_move_board
void get_range(int* start_j, int* end_j, int j);
int is_change(int, int, Tree);
void QuickSort(Tree move_set, int left, int right); // 快排
/*                                                   */
/* >> ----------------- 函数声明 ----------------- << */
/* ------------------------------------------------- */
#endif // __BITBOARD_H