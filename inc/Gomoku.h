#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#if !defined(__GOMOKU_H)
#define __GOMOKU_H

/* ------------------------------------------------ */
/* >> ----------------- 宏定义 ----------------- << */
/*                                                  */
/* 棋盘基本参数宏 */
#define LINE_NUM 14         // 最大坐标
#define LENGTH 15           // 棋盘长度
#define EMPTY 0             // 空位
#define BLACK 1             // 黑棋
#define WHITE 2             // 白棋
#define NULLPOSITION 255    // 表示非可下棋位置
#define LOSE_SCORE -1000002 // 标记必败点，在迭代加深中剔除

/* 坐标变换宏 PAR即水平 VER为垂直 LEFT为左上45° RIGHT为右上45° */
#define PAR_I(i, j) j
#define PAR_J(i, j) i
#define VER_I(i, j) i
#define VER_J(i, j) j
#define LEFT_I(i, j) (i + j)
#define LEFT_J(i, j) (i + j < LENGTH) ? i : (LINE_NUM - j)
#define RIGHT_I(i, j) (j - i + LINE_NUM)
#define RIGHT_J(i, j) (j <= i) ? j : i

/* 切换玩家 */
#define CHANGE_PLAYER                                                          \
    CurrentPlayer ^= 0b11;                                                     \
    BoolPlayer = BoolPlayer ^ 1

#define LASTBLACK 3 // 刚下的黑棋
#define LASTWHITE 4 // 刚下的白棋

/*                                                  */
/* >> ----------------- 宏定义 ----------------- << */
/* ------------------------------------------------ */

/* ----------------------------------------------- */
/* >> ---------------- 数据类型 ---------------- << */
/*                                                 */
/* 节省内存 坐标只用一个字节 */
typedef unsigned char Chess_point;
/* 在bit棋盘中 一个short16位即可表示一行 所以是Line */
typedef unsigned short Line;
/* 记录一个走法的节点Node */
typedef struct {
    Chess_point i; // 节点横坐标i(映射到显示屏上的棋盘则是纵坐标)
    Chess_point j; // 节点纵坐标j(映射到显示屏上的棋盘则是横坐标)
    Chess_point left;  // 按左上45°排序的行标
    Chess_point right; // 按右上45°排序的航标
    int point_score;   // 位置得分 用于排序
    int shape[4];      // 对应四个方向的棋形
} Node;

typedef Node* Tree; // 博弈树
/*                                                 */
/* >> ---------------- 数据类型 ---------------- << */
/* ----------------------------------------------- */

/* ------------------------------------------------- */
/* >> --------------- 全局变量声明 --------------- << */
/*                                                   */
extern int CurrentScore[];      // 当前局面评分
extern Line LastLocation_bit[]; // 缓存走法数组
extern char GameMode;           // 游戏模式

extern int Row, Col;      // 记录当前下棋位置
extern int CurrentPlayer; // 记录当前棋局的下棋人
extern int BoolPlayer; // 记录下棋人的布尔值，主要用于数组索引
extern int ChessBoard[LENGTH][LENGTH];                     // 棋盘
extern clock_t Time_sort, Time_gene, Time_end, Time_begin; // 调试用时间管理
extern int NodeNum;                                        // 节点总数
extern const int base[15]; // 哈希匹配权值  三进制 改进空间为四进制结合位操作
extern Line g_last_buf[5];   // 缓存变化前的未改变的着法部分
extern Tree g_move;          // 缓存上一步所得的走法
extern int BoardShape[][30]; // 棋形棋盘 3进制
/*                                                   */
/* >> --------------- 全局变量声明 --------------- << */
/* ------------------------------------------------- */

/* ------------------------------------------------- */
/* >> ----------------- 函数声明 ----------------- << */
/*                                                   */

void Gomoku_Run(void);
void Print_ChessBoard(void);       // 打印当前局势的棋盘
void Print_OneChess(int i, int j); // 打印棋子
void Update_Player(void); // 每下一步棋后根据玩家下棋对相应全局参数修改
void Update_AI(void);                            // AI下棋
void Update_Human(void);                         // 玩家下棋
int Get_MaxLength(int i, int j, int dx, int dy); // 返回对应行连子个数
void GetChess_Human(void);                       // 获取玩家输入
void Update_ChessBoard(int i, int j);            // 置点
void Print_LastLocation(void);                   // 报点
int GetWinner(int i, int j);                     // 判断是否赢
void Get_PlayerMovement(void);                   // 玩家操作
void GetChess_AI(void);                          // 从这开始进入AI
int forbid(int i, int j);                        // 判断禁手
int IsInRange_0_14(int i);          // 判断i是否在0-14闭区间内
int BothInRange_0_14(int i, int j); // 判断i, j都在0到14内
int GetWinner(int i, int j);        // 判断是否赢
int IsChessBoardFull(void);         // 判断棋盘有没有下满
/*                                                   */
/* >> ----------------- 函数声明 ----------------- << */
/* ------------------------------------------------- */
#endif // __GOMOKU_H
