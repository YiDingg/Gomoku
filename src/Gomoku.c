#include <stdio.h>
#include "Gomoku.h"
#include "GameMode.h"

/* ------------------------------------------------ */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/*                                                  */
int ChessBoard[LENGTH][LENGTH] = {0}; // 棋盘数组
/*                                                  */
/* >> ---- 全局变量 (在 .data 段, 初值默认 0) ---- << */
/* ------------------------------------------------ */

/* ------------------------------------------------ */
/* >> ------------------ 函数 ------------------ << */
/*                                                  */

/**
 * @brief 运行五子棋游戏，根据选择的模式启动相应的对战
 * @retval 无
 */
void Gomoku_Run(void) {
    Print_Infor();        // 打印游戏信息
    ChooseYourMode();     // 选择游戏模式
    GameMode = getchar(); // 获取用户输入的游戏模式
    while (!isdigit(GameMode) || (GameMode -= '0') > 2) { // 检查输入是否为1,2,3
        printf("Illegal input! Input again: "); // 输入非法，提示重新输入
        while (getchar() != '\n');              // 清空输入缓冲区
        GameMode = getchar();                   // 重新获取输入
    }

    if (GameMode == 0) {        // 如果选择人人对战
        Human_VS_Human();       // 启动人人对战模式
    } else if (GameMode == 1) { // 如果选择人机对战
        Human_VS_Computer();    // 启动人机对战模式
    } else if (GameMode == 2) { // 如果选择机机对战
        Computer_VS_Computer(); // 启动机机对战模式
    } else {                    // 输入其他非法模式
        printf("Erorr! No illegal game mode detected!\n"); // 打印错误信息
    }
}

/**
 * @brief 逐行打印棋盘，显示当前棋局状态
 * @retval 无
 */
void Print_ChessBoard(void) {
    int i, j; // 定义行变量i和列变量j
    /*自上而下打印，因此 i 从 LENGTH - 1 开始递减
     *每行第一个符号分三种边框情况和四种棋子情况特殊处理*/
    if (GameMode == 0)                                // 如果是人人对战
        printf("            Human VS Human\n");       // 打印模式名称
    else if (GameMode == 1)                           // 如果是人机对战
        printf("            Human VS Computer\n");    // 打印模式名称
    else                                              // 如果是机机对战
        printf("            Computer VS Computer\n"); // 打印模式名称

    // draw 列号 (ABC), \033[34m 为蓝色
    printf("  "); // 打印列号前的空格
    for (i = 'A'; i < 'A' + LENGTH; i++)
        printf(" \033[34m%c\033[0m ", i); // 打印列标
    printf("\n");                         // 换行

    // draw Chess Board
    for (i = LENGTH - 1; i >= 0; i--) {       // 从上到下遍历棋盘行
        printf("\033[34m%2d\033[0m ", i + 1); // 打印行号，蓝色字体
        if (ChessBoard[i][0] == EMPTY) {      // 如果第一个位置为空
            if (i == LENGTH - 1)
                printf("\033[43;30m┌ \033[0m"); // 打印左上角边框
            else if (i == 0)
                printf("\033[43;30m└ \033[0m"); // 打印左下角边框
            else
                printf("\033[43;30m├ \033[0m"); // 打印左边中间边框
        } else
            Print_OneChess(i, 0); // 打印棋子
        /*每行第二个符号到倒数第二个符号可以批量处理
         *同样分为三种边框情况和四种棋子情况*/
        for (j = 1; j < LENGTH - 1; j++) {   // 遍历每行中间的位置
            if (ChessBoard[i][j] == EMPTY) { // 如果位置为空
                if (i == LENGTH - 1)
                    printf("\033[43;30m ┬ \033[0m"); // 打印上边框
                else if (i == 0)
                    printf("\033[43;30m ┴ \033[0m"); // 打印下边框
                else
                    printf("\033[43;30m ┼ \033[0m"); // 打印中间边框
            } else
                Print_OneChess(i, j); // 打印棋子
        }
        // 每行最后一个符号分三种边框情况和四种棋子情况
        if (ChessBoard[i][LENGTH - 1] == EMPTY) { // 如果最后一个位置为空
            if (i == LENGTH - 1)
                printf("\033[43;30m ┐\033[0m"); // 打印右上角边框
            else if (i == 0)
                printf("\033[43;30m ┘\033[0m"); // 打印右下角边框
            else
                printf("\033[43;30m ┤\033[0m"); // 打印右边中间边框
        } else
            Print_OneChess(i, LENGTH - 1);    // 打印棋子
        printf(" \033[34m%2d\033[0m", i + 1); // 打印行号，蓝色字体
        printf("\n");                         // 换行
    }

    // draw 列号 (ABC), \033[34m 为蓝色
    printf("  "); // 打印列号前的空格
    for (i = 'A'; i < 'A' + LENGTH; i++)
        printf(" \033[34m%c\033[0m ", i); // 打印列标
    printf("\n\n");                       // 打印换行
}

/**
 * @brief 根据棋盘状态打印对应的棋子或边框符号
 * @param i 棋子所在行
 * @param j 棋子所在列
 * @retval 无
 */
void Print_OneChess(int i, int j) {
    if (j == 0) {                   // 如果是每行的第一个棋子
        switch (ChessBoard[i][j]) { // 根据棋盘状态选择打印内容
        case BLACK:
            printf("\033[43;30m● \033[0m"); // 打印黑棋
            break;
        case WHITE:
            printf("\033[43;30m○ \033[0m"); // 打印白棋
            break;
        case LASTBLACK:
            printf("\033[43;30m▲ \033[0m"); // 打印最后一个下的黑棋
            ChessBoard[i][j] = BLACK;       // 更新棋盘状态
            break;
        default:
            printf("\033[43;30m△ \033[0m"); // 打印最后一个下的白棋
            ChessBoard[i][j] = WHITE;       // 更新棋盘状态
            break;
        }
    } else if (j == LENGTH - 1) {   // 如果是每行的最后一个棋子
        switch (ChessBoard[i][j]) { // 根据棋盘状态选择打印内容
        case BLACK:
            printf("\033[43;30m ●\033[0m"); // 打印黑棋
            break;
        case WHITE:
            printf("\033[43;30m ○\033[0m"); // 打印白棋
            break;
        case LASTBLACK:
            printf("\033[43;30m ▲\033[0m"); // 打印最后一个下的黑棋
            ChessBoard[i][j] = BLACK;       // 更新棋盘状态
            break;
        default:
            printf("\033[43;30m △\033[0m"); // 打印最后一个下的白棋
            ChessBoard[i][j] = WHITE;       // 更新棋盘状态
            break;
        }
    } else {                        // 如果是中间的棋子
        switch (ChessBoard[i][j]) { // 根据棋盘状态选择打印内容
        case BLACK:
            printf("\033[43;30m ● \033[0m"); // 打印黑棋
            break;
        case WHITE:
            printf("\033[43;30m ○ \033[0m"); // 打印白棋
            break;
        case LASTBLACK:
            printf("\033[43;30m ▲ \033[0m"); // 打印最后一个下的黑棋
            ChessBoard[i][j] = BLACK;        // 更新棋盘状态
            break;
        default:
            printf("\033[43;30m △ \033[0m"); // 打印最后一个下的白棋
            ChessBoard[i][j] = WHITE;        // 更新棋盘状态
            break;
        }
    }
}

/**
 * @brief 更新玩家的下棋位置并刷新相关参数
 * @retval 无
 */
void Update_Player(void) {
    Node pt;                              // 定义一个Node结构体变量pt
    int index, num, line;                 // 定义整型变量index, num, line
    Get_PlayerMovement();                 // 执行玩家操作
    GetChess_Human();                     // 获取玩家输入
    Update_ChessBoard(Row, Col);          // 设置新的行(Row)和列(Col)
    pt.i = Row;                           // 将Row赋值给pt的i成员
    pt.j = Col;                           // 将Col赋值给pt的j成员
    pt.left = LEFT_I(Row, Col);           // 计算并赋值pt的left成员
    pt.right = RIGHT_I(Row, Col);         // 计算并赋值pt的right成员
    Get_Value(&pt);                       // 获取pt的相关值
    for (index = 0; index < 4; index++) { // 遍历四个方向
        if ((num = Get_Length(Row, Col, index)) < 5)
            continue;                   // 获取长度，若小于5则继续
        line = transX[index](Row, Col); // 转换索引并赋值给line
        CurrentScore[0] -=
            ScoreTable[0][num][BoardShape[index][line]]; // 更新当前分数
        CurrentScore[0] += ScoreTable[0][num][pt.shape[index]]; // 更新当前分数
        CurrentScore[1] -=
            ScoreTable[1][num][BoardShape[index][line]]; // 更新当前分数
        CurrentScore[1] += ScoreTable[1][num][pt.shape[index]]; // 更新当前分数
    }
    BoardShape[0][pt.j] = pt.shape[0];     // 更新BoardShape的第0行
    BoardShape[1][pt.i] = pt.shape[1];     // 更新BoardShape的第1行
    BoardShape[2][pt.left] = pt.shape[2];  // 更新BoardShape的第2行
    BoardShape[3][pt.right] = pt.shape[3]; // 更新BoardShape的第3行
}

/**
 * @brief AI执行下棋操作并设置新的位置
 * @retval 无
 */
void Update_AI(void) {
    GetChess_AI();               // 执行AI操作
    Update_ChessBoard(Row, Col); // 设置新的行(Row)和列(Col)
}

/**
 * @brief 简单设置棋子位置，适用于人人对战
 * @retval 无
 */
void Update_Human(void) {
    GetChess_Human();                         // 获取输入
    ChessBoard[Row][Col] = CurrentPlayer + 2; // 设置棋盘位置为当前玩家
}

/**
 * @brief 返回对应方向上的最长连子数
 * @param i 起始行
 * @param j 起始列
 * @param dx 行方向增量
 * @param dy 列方向增量
 * @retval num: 连续相同棋子的数量
 */
int Get_MaxLength(int i, int j, int dx, int dy) {
    int k, l;                           // 定义变量k和l
    int num = 1;                        // 初始化连子数量为1
    int last_player = ChessBoard[i][j]; // 获取最后下棋的玩家

    if (!last_player) return 0; // 如果位置为空，返回0
    for (k = i + dx, l = j + dy;
         BothInRange_0_14(k, l) && ChessBoard[k][l] == last_player;
         k += dx, l += dy, num++); // 向一个方向计数
    for (k = i - dx, l = j - dy;
         BothInRange_0_14(k, l) && ChessBoard[k][l] == last_player;
         k -= dx, l -= dy, num++); // 向相反方向计数

    return num; // 返回总连子数量
}

/**
 * @brief 获取玩家输入并进行合法性检查
 * @retval 无
 */
void GetChess_Human(void) { // GetChess_Human, 经过修改
    int correct_input = 0;  // 输入是否正确
    char temp_j;            // 临时存储列坐标
    char num;               // scanf 返回值

    rewind(stdin); // 清空缓冲区，fflush(stdin); 貌似不起作用
    num = scanf("\n%c%d", &temp_j, &Row); // 字母在前
    rewind(stdin);                        // 清空缓冲区

    /* 检查输入格式与接收参数个数 */
    if (num != 2) { // 如果没有正确读取两个参数
        printf("scanf() 扫描到的输入有格式错误，请重新输入\n"); // 打印错误信息
        rewind(stdin);    // 清空缓冲区
        GetChess_Human(); // 递归调用获取输入
        return;           // 返回
    }

    /* 1~15 到 0~14 转换*/
    Row--; // 将行号转换为0基

    /* 坐标范围合法检查 */
    if ((!IsInRange_0_14(temp_j - 'a') && !IsInRange_0_14(temp_j - 'A'))
        || !IsInRange_0_14(Row)) { // 检查列和行是否在合法范围内
        printf(
            "[Warning] GetChess_Human() 获取到 '%c%d', 也即 '%c' 和 '%d', 为非法数据, 请重新输入 :\n",
            temp_j, Row + 1, temp_j, Row + 1); // 打印警告信息
        GetChess_Human();                      // 递归调用获取输入
        return;                                // 返回
    }

    /* 大小写转换 */
    Col = (temp_j > 'Z') ? temp_j - 'a' : temp_j - 'A'; // 转换列字母为索引

    /* 落子位置合法性检查 */
    if (ChessBoard[Row][Col]) { // 如果位置已有棋子
        printf("[Warning] 此处已有棋子, 请重新输入: \n"); // 打印警告
        GetChess_Human(); // 递归调用获取输入
        return;           // 返回
    } else if (
        CurrentPlayer == WHITE
        || !forbid(Row, Col)) { // 检查是否为白棋或不违反禁手
        correct_input = 1;      // 标记输入正确
    } else {                    // 如果违反禁手
        printf("该位置为禁手, 不可落子, 请重新输入: \n"); // 打印警告
        GetChess_Human(); // 递归调用获取输入
        return;           // 返回
    }
}

/**
 * @brief 新下一步棋后对位棋盘进行相关操作
 * @param i 棋子所在行
 * @param j 棋子所在列
 * @retval 无
 */
void Update_ChessBoard(int i, int j) {
    ChessBoard[i][j] = CurrentPlayer + 2; // 设置棋盘位置为当前玩家
    BitBoard[j] &= BitSet[i];             // 更新位棋盘
    buf_move_board(LastLocation_bit);     // 缓存移动
    set_bit_board(i, j);                  // 设置位棋盘
}

/**
 * @brief 获取当前下棋者的胜利状态
 * @param i 棋子所在行
 * @param j 棋子所在列
 * @retval int 胜利者标识，0表示无胜利
 */
int GetWinner(int i, int j) {
    if (Get_MaxLength(i, j, 1, 0) >= 5 || Get_MaxLength(i, j, 1, 1) >= 5
        || Get_MaxLength(i, j, 1, -1) >= 5
        || Get_MaxLength(i, j, 0, 1) >= 5) { // 检查四个方向是否有五子连珠
        if (CurrentPlayer == BLACK)          // 如果当前玩家为黑棋
            return WHITE;                    // 返回白棋为胜者
        else                                 // 如果当前玩家为白棋
            return BLACK;                    // 返回黑棋为胜者
    } else
        return 0; // 无胜利
}

/**
 * @brief 判断一个整数是否在0到LENGTH（不包含LENGTH）之间
 * @param i 要判断的整数
 * @retval int 1表示在范围内，0表示不在范围内
 */
int IsInRange_0_14(int i) {
    if (i >= 0 && i < LENGTH) // 检查i是否大于等于0且小于LENGTH
        return 1;             // 在范围内，返回1
    else
        return 0; // 不在范围内，返回0
}

/**
 * @brief 判断两个整数是否都在0到LENGTH（不包含LENGTH）之间
 * @param i 第一个要判断的整数
 * @param j 第二个要判断的整数
 * @retval int 1表示两个都在范围内，0表示至少一个不在范围内
 */
int BothInRange_0_14(int i, int j) {
    if (IsInRange_0_14(i) && IsInRange_0_14(j)) // 调用within_range分别检查i和j
        return 1; // 两个都在范围内，返回1
    else
        return 0; // 至少一个不在范围内，返回0
}

/**
 * @brief 判断棋盘是否已满
 * @retval int 1表示棋盘已满，0表示棋盘未满
 */
int IsChessBoardFull(void) {
    int i, j; // 定义循环变量i和j

    for (i = 0; i < LENGTH; i++) {               // 遍历棋盘的每一行
        for (j = 0; j < LENGTH; j++) {           // 遍历棋盘的每一列
            if (!ChessBoard[i][j]) { return 0; } // 如果发现有空位，返回0
        } // 结束内层循环
    } // 结束外层循环
    return 1; // 所有位置都被占用，返回1
}

/*                                                  */
/* >> ------------------ 函数 ------------------ << */
/* ------------------------------------------------ */