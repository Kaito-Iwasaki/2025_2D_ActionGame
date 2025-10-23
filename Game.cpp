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
#include "font.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_LEVEL	(10)

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
int g_nCounterGameState = 0;
bool g_bIsPause = false;
FONT* g_pFontInfo = NULL;
DWORD g_dwTimer = timeGetTime();
DWORD g_dwStart = timeGetTime();

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	char aStageFileName[MAX_PATH] = {};

	// 各オブジェクトの初期化処理
	InitDecal();
	InitFont();
	InitBlock();
	InitPlayer();
	InitEnemy();
	InitItem();
	InitEffect();
	InitParticle();
	InitPause();
	InitFuelBar();

	// 背景画像の設置
	SetDecal(
		DECAL_LABEL_BG000,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

	g_pFontInfo = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		30,
		"",
		DT_CENTER
	);

	// 値の初期化
	g_nCounterGameState = 0;
	g_bIsPause = false;
	g_dwStart = timeGetTime();
	SetGameState(GAMESTATE_NORMAL);
	sprintf(&g_pFontInfo->aText[0], "Level %02d", g_nCurrentStage  + 1);

	// 現在のステージ情報のファイル名を取得
	GetStageName(g_nCurrentStage, &aStageFileName[0]);

	// ステージ情報のロード
	memset(&g_map[0][0], 0, sizeof(g_map));
	LoadBin(&aStageFileName[0], &g_map[0][0], sizeof(MAPINFO), MAX_BLOCK);

	// ステージ情報に基づいてブロックを配置
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
	// 各オブジェクトの終了処理
	UninitDecal();
	UninitFont();
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
	g_dwTimer = timeGetTime();

	if (INPUT_TRIGGER_PAUSE)
	{// ポーズメニュー
		TogglePause(!g_bIsPause);
	}

	if (g_bIsPause)
	{// ポーズ処理
		UpdatePause();
	}
	else
	{// ゲーム処理
		// 各オブジェクトの更新処理
		UpdateBlock();
		UpdatePlayer();
		UpdateEnemy();
		UpdateItem();
		UpdateParticle();
		UpdateEffect();
		UpdateFuelBar();

		// 現在のゲーム状態別の処理
		g_nCounterGameState++;
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:	// 通常
			sprintf(&g_pFontInfo->aText[0], "Level %02d\nTime %.2f", g_nCurrentStage + 1, (float)(g_dwTimer - g_dwStart) / 1000.0f);
			break;

		case GAMESTATE_CLEAR:	// クリア
			// プレイヤーを終了状態に移行
			SetPlayerState(PLAYERSTATE_END);

			if (g_nCounterGameState > 60)
			{
				// 次のステージに設定
				SetStage(g_nCurrentStage + 1);

				// ゲームを終了状態に移行
				SetGameState(GAMESTATE_END);
			}

			break;

		case GAMESTATE_END:		// 終了
			// 画面遷移処理
			if (g_nCurrentStage == MAX_LEVEL)
			{// 次のレベルがなければリザルト画面へ遷移
				SetFade(SCENE_RESULT, false);
			}
			else
			{
				// ゲーム画面へ遷移（ステージ情報はゲームの初期化で新しく読み込む）
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
	// 各オブジェクトの描画処理
	DrawDecal();
	DrawBlock();
	DrawItem();
	DrawEnemy();
	DrawEffect();
	DrawPlayer();
	DrawFuelBar();
	DrawFont();

	if (g_bIsPause)
	{// ポーズメニュー描画
		DrawPause();
	}
}

//=====================================================================
// ゲーム状態の設定処理
//=====================================================================
void SetGameState(GAMESTATE newState)
{
	g_gameState = newState;
	g_nCounterGameState = 0;
}

//=====================================================================
// ゲーム状態の取得処理
//=====================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//=====================================================================
// ポーズ状態の切り替え処理
//=====================================================================
void TogglePause(bool bIsPause)
{
	if (GetFade() != FADE_NONE) return;

	SetPauseMenuCursor(0);
	g_bIsPause = bIsPause;

	if (bIsPause)
	{
		PausePlayer();
	}
	else
	{
		UnPausePlayer();
	}
}

//=====================================================================
// マップの設定処理
//=====================================================================
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

//=====================================================================
// 現在のマップ情報の取得処理
//=====================================================================
MAPINFO* GetMap(void)
{
	return &g_map[0][0];
}

//=====================================================================
// ステージファイル名の取得処理
//=====================================================================
void GetStageName(int nStage, char* pBuffer)
{
	sprintf(pBuffer, "data\\MAP\\level%02d.bin", nStage);
}

//=====================================================================
// ステージ番号の設定処理
//=====================================================================
void SetStage(int nStage)
{
	g_nCurrentStage = nStage;
}

//=====================================================================
// ステージ番号の取得処理
//=====================================================================
int GetStage(void)
{
	return g_nCurrentStage;
}