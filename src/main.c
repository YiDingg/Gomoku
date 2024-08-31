#include <windows.h> // 解决输出乱码
#include <stdio.h>
#include "Gomoku.h"
#include "AI_Greedy.h"

#define DEBUG 0

int main() {
    /* 控制台相关设置 */
    // 设置编码为 UTF-8，解决输出乱码
    SetConsoleOutputCP(65001);
    /*
     颜色属性由两个十六进制数字指定，第一个对应于背景，第二个对应于前景。每个数字可以为以下任何值:
     0-黑色 1-蓝色 2-绿色 3-红色 4-紫色 5-黄色 6-青色 7-白色 8-灰色
     9-淡绿色 10-淡浅绿色 11-淡红色 12-淡紫色 13-淡黄色 14-亮白色
    */

    /*
    #define WINDOW_BLACK 			0		//黑色
    #define WINDOW_COBALTBLUE 		1       //钴蓝
    #define WINDOW_GREEN 			2 		//绿色
    #define WINDOW_BLUE 			3		//蓝色
    #define WINDOW_RED 			    4		//红色
    #define WINDOW_PURPLE 			5		//紫色
    #define WINDOW_DARKYELLOW 		6		//深黄色
    #define WINDOW_WHITE 			7		//白色
    #define WINDOW_GRAY 			8		//灰色
    #define WINDOW_INDIGO 			9       //靛蓝
    #define WINDOW_LIGHTGREEN 		10		//浅绿
    #define	WINDOW_LIGHTBLUE 		11		//浅蓝
    #define WINDOW_LIGHTRED 		12		//浅红
    #define WINDOW_LIGHTPURPLE 	    13		//浅紫
    #define WINDOW_LIGHTYELLOW 	    14		//浅黄
    #define WINDOW_BRIGHTWHITE 	    15		//亮白
    也可以用枚举：
    typedef enum {
    {
    Black, Blue, Green, Lakeblue, Red, Purple, Yellow, White,
    Gray,Lightblue,Lightgreen,Lightsimplegreen, Lightred, Lightpurple, Lightyellow, Brightwhite
    }Enum_Color;
     */
    system("color 07"); // 背景: 0-黑色，字体: 7-白色

    /* AI_Greedy.c 测试 */
    /*
    Enum_Color Chessboard[COLUMN][ROW] = {0};
    // Chessboard[0][0] = Black;
    Chessboard[5][5] = Black;
    printf("Value of this chessboard is: %d\n", EvaluateChessboard(Chessboard, Black));
    */

    /* 运行五子棋 */
    Gomoku_Run();
    return 0;
}
