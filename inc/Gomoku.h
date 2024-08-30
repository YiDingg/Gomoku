#if !defined(__Gomoku_H)
#define __Gomoku_H

/* 宏定义 */
#define ROW 15
#define COLUMN 15

enum COLOR { White = -1, Black = 1, Blank = 0 } CurrentPlayer;
enum HUMANorCOMPUTER { None = 0, Human = 1, Computer = -1 };

/* 变量、数据声明 */
int ChessBoard[COLUMN][ROW];
struct GAMEMODE {
    char BlackPlayer;
    char WhitePlayer;
} GameMode;
struct COORDINATE {
    char raw;
    char column;
} CuurentCoordinate;

/* 函数声明 */
void Gomoku_Run(void);
void ChessHandler(void);
void GetChess(void);
void GetChess_AI_random();
void DrawPoint(char i, char j, int type);
void DrawBoard();
int VictoryJudgment(int chessboard[][ROW]);
void ShowStatu(void);
void ChooseMode(struct GAMEMODE* p_gamemode);

#endif // __Gomoku_H