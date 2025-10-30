//=====================================================================
//
// Game.cppのヘッダファイル [Game.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _Game_H_
#define _Game_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "editorblock.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define GAME_GRAVITY		(0.35f)
#define MAX_LEVEL			(11)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	GAMESTATE_READY = 0,
	GAMESTATE_NORMAL,
	GAMESTATE_RESET,
	GAMESTATE_CLEAR,
	GAMESTATE_END,
	GAMESTATE_MAX
}GAMESTATE;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE newState);
GAMESTATE GetGameState(void);
void ResetGame(void);
void TogglePause(bool bIsPause);
void SetMap(MAPINFO* map);
MAPINFO* GetMap(void);
void GetStageName(int nStage, char* pBuffer);
void SetStage(int nStage);
int GetStage(void);
void AddScore(int nScore);
int GetScore(void);
int GetGameTimeLeft(void);

#endif