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
#define GAME_GRAVITY		(1.0f)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	GAMESTATE_NORMAL = 0,
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
void TogglePause(bool bIsPause);
void SetMap(MAPINFO* map);
MAPINFO* GetMap(void);
void GetStageName(int nStage, char* pBuffer);
void SetStage(int nStage);

#endif