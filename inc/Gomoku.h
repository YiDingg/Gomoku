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
void Gomoku_Run();
void ShowInfor();
void ChooseMode(struct GAMEMODE* p_gamemode);
void DrawBoard();
void DrawPoint(char i, char j, int type);
void ShowStatu();
void GetChess();
void ChessHandler();
int VictoryJudgment(int chessboard[][ROW], struct COORDINATE wincoordinate[5]);
void GetChess_AI_random();
#endif // __Gomoku_H