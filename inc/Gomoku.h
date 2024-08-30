#if !defined(__Gomoku_H)
#define __Gomoku_H

/* 宏定义 */
#define ROW 15
#define COLUMN 15

enum COLOR { White = -1, Black = 1, Blank = 0 };
enum HUMANorCOMPUTER { None = 0, Human = 1, Computer = 2 };

/* 变量、数据声明 */
int ChessBoard[COLUMN][ROW];
struct GAMEMODE {
    char BlackPlayer;
    char WhitePlayer;
} GameMode;

/* 函数声明 */
void DrawPoint(char i, char j, int type);
void DrawBoard(int chessboard[][ROW]);
int VictoryJudgment(int chessboard[][ROW]);
void ShowStatu(void);
void ChooseMode(struct GAMEMODE* p_gamemode);

#endif // __Gomoku_H