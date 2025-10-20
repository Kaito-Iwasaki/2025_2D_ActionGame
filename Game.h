//=====================================================================
//
// Game.cpp�̃w�b�_�t�@�C�� [Game.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _Game_H_
#define _Game_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"
#include "editorblock.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define GAME_GRAVITY		(1.0f)

//*********************************************************************
// 
// ***** �񋓌^ *****
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
// ***** �v���g�^�C�v�錾 *****
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