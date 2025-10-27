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
#include "background.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_LEVEL	(10)
#define INIT_TIMER	(60 * 120)

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
DWORD g_dwStart = timeGetTime();
DWORD g_dwLast = timeGetTime();
int g_nTimer = INIT_TIMER;
int g_nScore;

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
	InitBackground();

	g_pFontInfo = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
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

	if (GetPreviousScene() == SCENE_TITLE)
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME00);
	}

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
	UninitBackground();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
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
		UpdateBackground();

		// 現在のゲーム状態別の処理
		g_nCounterGameState++;
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:	// 通常
			if (GetFade() == FADE_NONE && GetPlayer()->state != PLAYERSTATE_END)
			{
				g_nTimer--;
			}

			sprintf(&g_pFontInfo->aText[0], "Level %02d | Time %03d | Score | %06d", g_nCurrentStage + 1, (g_nTimer) / 10, g_nScore);

			if (g_nTimer <= 0)
			{
				KillPlayer();
				SetFade(SCENE_RESULT);
			}

			break;

		case GAMESTATE_CLEAR:	// クリア
			// プレイヤーを終了状態に移行
			SetPlayerState(PLAYERSTATE_END);

			sprintf(&g_pFontInfo->aText[0], "Level %02d | Time %03d | Score | %06d", g_nCurrentStage + 1, (g_nTimer) / 10, g_nScore);

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
				SetFade(SCENE_RESULT);
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
	DrawBackground();
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
// ゲーム情報の初期化処理
//=====================================================================
void ResetGame(void)
{
	SetStage(0);
	g_nTimer = INIT_TIMER;
	g_nScore = 0;
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

void AddScore(int nScore)
{
	g_nScore += nScore;
}

int GetScore(void)
{
	return g_nScore;
}

int GetGameTimeLeft(void)
{
	return g_nTimer;
}
