#include <windows.h> // 解决输出乱码
#include <stdio.h>
#include "Gomoku.h"
#include "AI_Greedy.h"
#include <time.h> // time()

#define DEBUG 0

int main() {
    /* 控制台相关设置 */
    // 设置编码为 UTF-8，解决输出乱码
    SetConsoleOutputCP(65001);

    /* 运行五子棋 */
    Gomoku_Run();

#if 0 
    /* 黑窗口颜色设置 */
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
    Enum_Color Chessboard[COLUMN][ROW] = {0};
    for (char i = 0; i < ROW; i++) {
        for (char j = 0; j < COLUMN; j++) {
            Chessboard[i][j] = Black;
            printf(
                "Value: %d (row: %d, column: %d)\n", EvaluateChessboard(Chessboard, Black), i, j);
            Chessboard[i][j] = Blank;
        }
    }

    /* 希尔排序测试 */
    int nums[] = {3, 2, 1, 5, 6, 4, 4985, -88922, 94665};
    for (char i = 0; i < sizeof(nums) / sizeof(nums[0]); i++) { printf("%d, ", nums[i]); }
    putchar('\n');
    SortArray_Shell(nums, sizeof(nums) / sizeof(nums[0]));
    for (char i = 0; i < sizeof(nums) / sizeof(nums[0]); i++) { printf("%d, ", nums[i]); }
    putchar('\n');
#endif

#if 0
    /* 修改后的希尔排序测试 */
    Struct_LocationWithValue locs_with_values[] = {
        {{0, 1}, 999},   {{0, 3}, 999},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542},
        {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{0, 1}, 999},   {{0, 3}, 999},
        {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},
        {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542},
        {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542},
        {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},
        {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},
        {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},
        {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},
        {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542},
        {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542},
        {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},
        {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},
        {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},
        {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},
        {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542},
        {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542},
        {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},
        {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},
        {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},
        {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},
        {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542},
        {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542},
        {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},
        {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},
        {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},
        {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},
        {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542},
        {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542},
        {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},
        {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},
        {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},
        {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},
        {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542},
        {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542},
        {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},
        {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},   {{1, 0}, 123},
        {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},   {{1, 1}, -36},
        {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542}, {{2, 0}, 416},
        {{1, 1}, -36},   {{1, 0}, 123},   {{1, 4}, 123},   {{0, 5}, -1542}, {{0, 6}, -1542},
        {{2, 0}, 416},   {{1, 1}, -36},

    };
    puts("----------------------------------");
    for (int i = 0; i < sizeof(locs_with_values) / sizeof(locs_with_values[0]); i++) {
        printf(
            "%d. (%d, %d) --> %d\n",
            i,
            locs_with_values[i].location.row,
            locs_with_values[i].location.column,
            locs_with_values[i].value);
    }
    ShellSort_LocationWithValue(
        locs_with_values, sizeof(locs_with_values) / sizeof(locs_with_values[0]));
    puts("----------------------------------");
    for (int i = 0; i < sizeof(locs_with_values) / sizeof(locs_with_values[0]); i++) {
        printf(
            "%d. (%d, %d) --> %d\n",
            i,
            locs_with_values[i].location.row,
            locs_with_values[i].location.column,
            locs_with_values[i].value);
    }
    puts("----------------------------------");
#endif

#if 0
    /* 随机数测试 */
    double randnum = 0.0;
    int num_da = 0;
    int num_xiao = 0;
    for (int i = 0; i < 100000; i++) {
        srand(time(NULL) + rand());
        randnum = (double)rand() / RAND_MAX;
        (randnum > 0.5) ? num_da++ : num_xiao++;
    }
    printf("num_da: %d, num_xiao: %d\n", num_da, num_xiao);
#endif

    return 0;
}

/* void SortArray_Shell(int* nums, int numsSize) {
    int step = numsSize / 2;
    while (step > 0) {
        //!
        for (int i = step; i < numsSize; ++i) {
            int tmp = nums[i];
            int j = i - step;
            while (j >= 0 && tmp < nums[j]) {
                nums[j + step] = nums[j];
                j -= step;
            }
            nums[j + step] = tmp;
        }
        step = step / 2;
    }
} */