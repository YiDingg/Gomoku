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
void ShowInfor(void);
void ChooseMode(Struct_GameMode* p_game_mode);
void DrawBoard(const Enum_Color chessboard[ROW][COLUMN]);
void DrawPoint(const char row, const char column, const Enum_Color color);
void ShowStatu(
    const Struct_GameMode gamemode,
    const Enum_Color chessboard[COLUMN][ROW],
    const char currentturn,
    const Enum_Color currentplayer,
    Struct_Location win_coordinates[5],
    Struct_Location lastlocation);
void ChessHandler(void);
Enum_Color
VictoryJudgment(const Enum_Color chessboard[ROW][COLUMN], Struct_Location win_coordinate[5]);
void GetChess(
    Struct_Location* p_location,
    const Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me);
void GetChess_Human(
    Struct_Location* p_location,
    const Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me);
void GetChess_AI_random(
    Struct_Location* p_location,
    const Enum_Color chessboard[ROW][COLUMN],
    const Enum_Color me);
void CheckThisLocation(
    Enum_LegalOrIllegal* p_islegal,
    const Enum_Color chessboard[ROW][COLUMN],
    const Struct_Location* p_location,
    const Enum_Color me);
/*                                                   */
/* >> ----------------- 函数声明 ----------------- << */
/* ------------------------------------------------- */
#endif // __GOMOKU_H