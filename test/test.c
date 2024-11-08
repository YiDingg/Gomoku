#include <stdio.h>
#include <stdlib.h>  // rand(), exit()
#include <time.h>    // srand(), time()
#include <windows.h> // 获取并输出时间 (用于数据记录)
#include "Gomoku.h"
#include "AI_Greedy.h"

int main() {
    char column;
    int row;
    int num;

    char k_min = 1;
    char k_max = 2;
    for (char k = k_min; k <= k_max; k++) { printf("%d", k); }

    return 0;
}