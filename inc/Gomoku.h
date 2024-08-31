#if !defined(__GOMOKU_H)
#define __GOMOKU_H
/* ------------------------------------------------ */
/* >> ----------------- 宏定义 ----------------- << */
/*                                                  */
#define ROW 15
#define COLUMN 15

/*                                                  */
/* >> ----------------- 宏定义 ----------------- << */
/* ------------------------------------------------ */

/* ----------------------------------------------- */
/* >> ---------------- 数据类型 ---------------- << */
/*                                                 */
typedef enum { White = -1, Black = 1, Blank = 0 } Enum_Color;
typedef enum { None = 0, Human = 1, Computer = -1 } Enum_HumanOrComputer;
typedef enum { Legal = 1, Illegal = 2 } Enum_LegalOrIllegal;
typedef struct {
    char BlackPlayer;
    char WhitePlayer;
} Struct_GameMode;
typedef struct {
    char row;
    char column;
} Struct_Location;
/*                                                 */
/* >> ---------------- 数据类型 ---------------- << */
/* ----------------------------------------------- */

/* ------------------------------------------------- */
/* >> --------------- 全局变量声明 --------------- << */
/*                                                   */
/*                                                   */
/* >> --------------- 全局变量声明 --------------- << */
/* ------------------------------------------------- */

/* ------------------------------------------------- */
/* >> ----------------- 函数声明 ----------------- << */
/*                                                   */
void Gomoku_Run();
void ShowInfor();
void ChooseMode(Struct_GameMode* p_game_mode);
void DrawBoard();
void DrawPoint(char i, char j, int type);
void ShowStatu();
void GetChess();
void ChessHandler();
int VictoryJudgment(Enum_Color chessboard[][ROW], Struct_Location win_coordinates[5]);
void GetChess_AI_random();
Enum_LegalOrIllegal CheckThisLocation(
    const Enum_Color chessboard[ROW][COLUMN],
    const Struct_Location location,
    const Enum_Color me);
/*                                                   */
/* >> ----------------- 函数声明 ----------------- << */
/* ------------------------------------------------- */
#endif // __GOMOKU_H