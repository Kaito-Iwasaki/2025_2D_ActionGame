//=====================================================================
//
// ゲーム画面処理 [Game.h]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Game.h"
#include "player.h"
#include "block.h"
#include "decal.h"
#include "input.h"
#include "sound.h"
#include "util.h"
#include "enemy.h"
#include "item.h"
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "pause.h"
#include "fuelbar.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_LEVEL	(6)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
MAPINFO g_map[NUM_BLOCK_Y][NUM_BLOCK_X] = {};
int g_nCurrentStage = 0;
GAMESTATE g_gameState = GAMESTATE_NORMAL;
bool g_bIsPause = false;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	InitDecal();
	InitBlock();
	InitPlayer();
	InitEnemy();
	InitItem();
	InitEffect();
	InitParticle();
	InitPause();
	InitFuelBar();

	SetDecal(
		DECAL_LABEL_BG000,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

	g_bIsPause = false;

	char aStageFileName[MAX_PATH] = {};

	GetStageName(g_nCurrentStage, &aStageFileName[0]);

	memset(&g_map[0][0], 0, sizeof(g_map));
	LoadBin(&aStageFileName[0], &g_map[0][0], sizeof(MAPINFO), MAX_BLOCK);

	SetGameState(GAMESTATE_NORMAL);

	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			SetBlock(g_map[y][x].type, x, y);
		}
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	UninitDecal();
	UninitBlock();
	UninitPlayer();
	UninitEnemy();
	UninitItem();
	UninitParticle();
	UninitEffect();
	UninitPause();
	UninitFuelBar();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	if (INPUT_TRIGGER_PAUSE)
	{
		TogglePause(!g_bIsPause);
	}

	if (g_bIsPause)
	{
		UpdatePause();
	}
	else
	{
		UpdateBlock();
		UpdatePlayer();
		UpdateEnemy();
		UpdateItem();
		UpdateParticle();
		UpdateEffect();
		UpdateFuelBar();

		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:
			break;

		case GAMESTATE_CLEAR:
			SetStage(g_nCurrentStage + 1);
			SetPlayerState(PLAYERSTATE_END);
			SetGameState(GAMESTATE_END);
			break;

		case GAMESTATE_END:
			if (g_nCurrentStage == MAX_LEVEL)
			{
				SetFade(SCENE_RESULT, false);
			}
			else
			{
				SetFade(SCENE_GAME, false);
			}

			break;
		}
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1) && GetFade() == FADE_NONE)
	{// デバッグ用クリアボタン
		SetGameState(GAMESTATE_CLEAR);
	}
	if (GetKeyboardTrigger(DIK_F5))
	{// エディタモード遷移
		SetScene(SCENE_EDITOR);
	}
#endif
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawDecal();
	DrawBlock();
	DrawItem();
	DrawEnemy();
	DrawEffect();
	DrawPlayer();
	DrawFuelBar();

	if (g_bIsPause)
	{
		DrawPause();
	}
}

void SetGameState(GAMESTATE newState)
{
	g_gameState = newState;
}

GAMESTATE GetGameState(void)
{
	return g_gameState;
}

void TogglePause(bool bIsPause)
{
	SetPauseMenuCursor(0);
	g_bIsPause = bIsPause;
}

void SetMap(MAPINFO* map)
{
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++, map++)
		{
			g_map[y][x].type = map->type;
		}
	}
}

MAPINFO* GetMap(void)
{
	return &g_map[0][0];
}

void GetStageName(int nStage, char* pBuffer)
{
	sprintf(pBuffer, "data\\MAP\\level%02d.bin", nStage);
}

void SetStage(int nStage)
{
	g_nCurrentStage = nStage;
}

int GetStage(void)
{
	return g_nCurrentStage;
}