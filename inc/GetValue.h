#include <math.h>
#include "Gomoku.h"

#if !defined(__GetValue_H)
#define __GetValue_H

/* ------------------------------------------------ */
/* >> ----------------- 宏定义 ----------------- << */
/*                                                  */
#define One_1w 486 // 200000
#define One_2w 162 // 020000
#define One_3w 54  // 002000
#define One_4w 18  // 000200
#define One_5w 6   // 000020
#define One_6w 2   // 000002

#define One_7w 405 // 120000
#define One_8w 297 // 102000
#define One_9w 261 // 100200
#define One_aw 249 // 100020
#define One_bw 163 // 020001
#define One_cw 55  // 002001
#define One_dw 19  // 000201
#define One_ew 7   // 000021

#define Two_1w 648 // 220000
#define Two_2w 540 // 202000
#define Two_3w 504 // 200200
#define Two_4w 492 // 200020
#define Two_5w 8   // 000022
#define Two_6w 20  // 000202
#define Two_7w 56  // 002002
#define Two_8w 164 // 020002

#define Two_9w 459 // 122000
#define Two_aw 423 // 120200
#define Two_bw 411 // 120020
#define Two_cw 407 // 120002
#define Two_dw 315 // 102200
#define Two_ew 303 // 102020
#define Two_fw 299 // 102002
#define Two_gw 267 // 100220
#define Two_hw 263 // 100202
#define Two_iw 251 // 100022
#define Two_jw 649 // 220001
#define Two_kw 541 // 202001
#define Two_lw 505 // 200201
#define Two_mw 493 // 200021
#define Two_nw 217 // 022001
#define Two_ow 181 // 020201
#define Two_pw 169 // 020021
#define Two_qw 73  // 002201
#define Two_rw 69  // 002021
#define Two_sw 25  // 000221

#define HuoTwo_1w 216 // 022000
#define HuoTwo_2w 180 // 020200
#define HuoTwo_3w 168 // 020020
#define HuoTwo_4w 72  // 002200
#define HuoTwo_5w 24  // 000220
#define HuoTwo_6w 60  // 002020

#define Three_1w 702 // 222000
#define Three_2w 666 // 220200
#define Three_3w 654 // 220020
#define Three_4w 558 // 202200
#define Three_5w 546 // 202020
#define Three_6w 510 // 200220

#define Three_7w 26  // 000222
#define Three_8w 74  // 002202
#define Three_9w 218 // 022002
#define Three_aw 62  // 002022
#define Three_bw 182 // 020202
#define Three_cw 170 // 020022

#define Three_dw 477 // 122200
#define Three_ew 465 // 122020
#define Three_fw 461 // 122002
#define Three_gw 429 // 120220
#define Three_hw 425 // 120202
#define Three_iw 413 // 120022
#define Three_jw 321 // 102220
#define Three_kw 317 // 102202
#define Three_lw 305 // 102022
#define Three_mw 269 // 100222

#define Three_nw 703 // 222001
#define Three_ow 667 // 220201
#define Three_pw 655 // 220021
#define Three_qw 559 // 202201
#define Three_rw 547 // 202021
#define Three_sw 511 // 200221
#define Three_tw 235 // 022201
#define Three_uw 223 // 022021
#define Three_vw 187 // 020221
#define Three_ww 79  // 002221

#define HuoThree_1w 234 // 022200
#define HuoThree_2w 222 // 022020
#define HuoThree_3w 78  // 002220
#define HuoThree_4w 186 // 020220

#define Four_1w 720 // 222200
#define Four_2w 708 // 222020
#define Four_3w 672 // 220220
#define Four_4w 564 // 202220

#define Four_5w 80  // 002222
#define Four_6w 236 // 022202
#define Four_7w 224 // 022022
#define Four_8w 188 // 020222
#define Four_9w 483 // 122220
#define Four_aw 323 // 102222
#define Four_bw 479 // 122202
#define Four_cw 431 // 120222

#define Four_dw 721 // 222201
#define Four_ew 241 // 022221
#define Four_fw 709 // 222021
#define Four_gw 565 // 202221

#define HuoFour_w 240 // 022220

#define Five_w1 726 // 222220
#define Five_w2 727 // 222221
#define Five_w3 242 // 022222
#define Five_w4 485 // 122222
#define Five_w5 728 // 222222

#define One_1b 243
#define One_2b 81
#define One_3b 27
#define One_4b 9
#define One_5b 3
#define One_6b 1
#define One_7b 567
#define One_8b 513
#define One_9b 495
#define One_ab 489
#define One_bb 83
#define One_cb 29
#define One_db 11
#define One_eb 5

#define Two_1b 324
#define Two_2b 270
#define Two_3b 252
#define Two_4b 246

#define Two_5b 4
#define Two_6b 10
#define Two_7b 28
#define Two_8b 82
#define Two_9b 594
#define Two_ab 576
#define Two_bb 570
#define Two_cb 568
#define Two_db 522
#define Two_eb 516
#define Two_fb 514
#define Two_gb 498
#define Two_hb 496
#define Two_ib 490

#define Two_jb 326
#define Two_kb 272
#define Two_lb 254
#define Two_mb 248
#define Two_nb 110
#define Two_ob 92
#define Two_pb 86
#define Two_qb 38
#define Two_rb 32
#define Two_sb 14

#define HuoTwo_1b 108
#define HuoTwo_2b 90
#define HuoTwo_3b 84
#define HuoTwo_4b 36
#define HuoTwo_5b 12
#define HuoTwo_6b 30

#define Three_1b 351
#define Three_2b 333
#define Three_3b 327
#define Three_4b 279
#define Three_5b 273
#define Three_6b 255
#define Three_7b 13
#define Three_8b 37
#define Three_9b 109
#define Three_ab 31
#define Three_bb 91
#define Three_cb 85

#define Three_db 603
#define Three_eb 597
#define Three_fb 595
#define Three_gb 579
#define Three_hb 577
#define Three_ib 571
#define Three_jb 525
#define Three_kb 523
#define Three_lb 517
#define Three_mb 499

#define Three_nb 353
#define Three_ob 335
#define Three_pb 329
#define Three_qb 281
#define Three_rb 275
#define Three_sb 257
#define Three_tb 119
#define Three_ub 113
#define Three_vb 95
#define Three_wb 41

#define HuoThree_1b 117
#define HuoThree_2b 111
#define HuoThree_3b 39
#define HuoThree_4b 93

#define Four_1b 360
#define Four_2b 354
#define Four_3b 336
#define Four_4b 282

#define Four_5b 40
#define Four_6b 118
#define Four_7b 112
#define Four_8b 94
#define Four_9b 606
#define Four_ab 526
#define Four_bb 604
#define Four_cb 580

#define Four_db 362
#define Four_eb 122
#define Four_fb 356
#define Four_gb 284

#define HuoFour_b 120

#define Five_b1 363
#define Five_b2 365
#define Five_b3 121
#define Five_b4 607
#define Five_b5 364
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
extern int* ScoreTable[][16]; // 黑白两色的棋盘评分表头指针数组, 0-5 空置
extern int (*transX[4])(int i, int j); // 四种坐标变换的函数指针
extern int (*transY[4])(int i, int j);
/*                                                   */
/* >> --------------- 全局变量声明 --------------- << */
/* ------------------------------------------------- */

/* ------------------------------------------------- */
/* >> ----------------- 函数声明 ----------------- << */
/*                                                   */
void Init_Hash(void);         // 初始化所有 ScoreTable
void Init_ScoreTable(int);    // 初始化 6-15 长度某一长度的 ScoreTable
void Init_ScoreTable_5(void); // 初始化长度为 5 的 ScoreTable
int Init_GetValue(int* board, int i, int len); // 分析某一棋形
int Get_Length(int, int, int);                 // 获得某行某方向长度
void Get_Value(Tree p); // 获得某点四个方向上的棋形和位置价值
// void get_point_score_kill(Tree p);

/*四种坐标变换，本身采用宏函数，为了方便采用了函数指针数组存这几个函数*/
int Par_i(int, int);
int Par_j(int, int);
int Ver_i(int, int);
int Ver_j(int, int);
int Left_i(int, int);
int Left_j(int, int);
int Right_i(int, int);
int Right_j(int, int);

/*                                                   */
/* >> ----------------- 函数声明 ----------------- << */
/* ------------------------------------------------- */
#endif // __GetValue_H