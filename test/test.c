#include <stdio.h>

int main() {
    char column;
    int row;
    int num;

    printf("请输入坐标（例如 a2, h8, a10）: \n");

    while (1) {
        num = scanf("%c%d", &column, &row);

        // 检查输入格式与接收参数个数
        if (num != 2) {
            // scanf 返回成功获取的参数个数, != 2 时输入格式不正确，清除输入缓冲区并提示重新输入
            while (getchar() != '\n'); // 清除缓冲区直到遇到换行符
            printf("输入格式不正确，请重新输入（例如 a2, h8, a10）：\n");
        } else {
            // 输出读取的字母和数字
            printf("字母: %c, 数字: %d\n", column, row);
            break; // 退出循环
        }
    }

    return 0;
}