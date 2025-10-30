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
#include "pauseBg.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_TIMER			(60 * 180)
#define TIME_STATE_READY	(4)
#define TIMER_DANGER		(60 * 30)

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
GAMESTATE g_gameState = GAMESTATE_READY;
int g_nCurrentStage = 0;
int g_nCounterGameState = 0;
int g_nTimer = INIT_TIMER;
int g_nScore;
bool g_bIsPause = false;
FONT* g_pFontInfo = NULL;
FONT* g_pFontPauseMenuTitle = NULL;
FONT* g_pFontCountDown = NULL;

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
	InitPauseBg();

	g_pFontInfo = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		35,
		"",
		DT_CENTER
	);

	g_pFontPauseMenuTitle = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 130, 0),
		D3DXVECTOR3(SCREEN_WIDTH, 100, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		60,
		"- PAUSE -",
		DT_CENTER
	);
	g_pFontPauseMenuTitle->obj.bVisible = false;

	g_pFontCountDown = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		D3DXCOLOR(1.0f, 0.7f, 0.3f, 0.0f),
		200,
		"",
		DT_CENTER | DT_VCENTER
	);

	// 値の初期化
	g_nCounterGameState = 0;
	g_bIsPause = false;
	sprintf(
		&g_pFontInfo->aText[0],
		"Level %02d/%02d | Time %03d | Score %06d",
		g_nCurrentStage + 1,
		MAX_LEVEL,
		(INIT_TIMER) / 10,
		g_nScore
	);
	GetPlayer()->bIsControlEnabled = false;

	if (g_nCurrentStage == 0)
	{
		g_nTimer = TIME_STATE_READY;
		SetGameState(GAMESTATE_READY);
	}
	else
	{
		SetGameState(GAMESTATE_NORMAL);
	}

	if (GetPreviousScene() == SCENE_TITLE)
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME00);
	}
	
	if (GetPreviousScene() == SCENE_EDITOR)
	{
		SetGameState(GAMESTATE_NORMAL);
		g_nTimer = INIT_TIMER;
		g_nScore = 0;
	}

	// 現在のステージ情報のファイル名を取得
	GetStageName(g_nCurrentStage, &aStageFileName[0]);

	// ステージ情報のロード
	memset(&g_map[0][0], 0, sizeof(g_map));
	LoadBin(&aStageFileName[0], &g_map[0][0], sizeof(MAPINFO), MAX_BLOCK);

	// ステージ情報に基づいてブロックを配置
	BLOCK* pBlock;
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			pBlock = SetBlock(g_map[y][x].type, x, y);

			for (int i = 0; i < MAX_BLOCK_PARAM; i++)
			{
				pBlock->nParam[i] = g_map[y][x].nParam[i];
			}
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
	UninitPauseBg();
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
		case GAMESTATE_READY:
			GetPlayer()->bIsControlEnabled = false;

			if (g_nCounterGameState % 60 == 0)
			{
				g_pFontCountDown->obj.color.a = 1.0f;
				g_nTimer--;
				PlaySound(SOUND_LABEL_SE_CURSOR);
			}

			g_pFontCountDown->obj.color.a = Clampf(g_pFontCountDown->obj.color.a - 0.01f, 0.0f, 1.0f);

			if (g_nTimer < 1)
			{
				sprintf(&g_pFontCountDown->aText[0], "START");
				SetGameState(GAMESTATE_NORMAL);
				g_nTimer = INIT_TIMER;
				PlaySound(SOUND_LABEL_SE_COIN);
			}
			else
			{
				sprintf(&g_pFontCountDown->aText[0], "%d", g_nTimer);
			}

			break;

		case GAMESTATE_NORMAL:	// 通常
			GetPlayer()->bIsControlEnabled = true;

			if (GetFade() == FADE_NONE && GetPlayer()->state != PLAYERSTATE_END)
			{
				g_nTimer--;
			}

			g_pFontCountDown->obj.color.a = Clampf(g_pFontCountDown->obj.color.a - 0.01f, 0.0f, 1.0f);

			sprintf(
				&g_pFontInfo->aText[0],
				"Level %02d/%02d | Time %03d | Score %06d",
				g_nCurrentStage + 1,
				MAX_LEVEL,
				(g_nTimer) / 10,
				g_nScore
			);

			if (g_nTimer <= 0)
			{
				g_nTimer = 0;
				KillPlayer();
				SetFade(SCENE_RESULT);
			}
			else if (g_nTimer < TIMER_DANGER)
			{
				if (g_nTimer % 20 == 0)
				{
					if (g_pFontInfo->obj.color == D3DXCOLOR_WHITE)
					{
						g_pFontInfo->obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					}
					else
					{
						g_pFontInfo->obj.color = D3DXCOLOR_WHITE;
					}
				}
			}
			else
			{
				if (GetPlayer()->state == PLAYERSTATE_DIED)
				{
					g_pFontInfo->obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else
				{
					g_pFontInfo->obj.color = D3DXCOLOR_WHITE;
				}
			}

			break;

		case GAMESTATE_CLEAR:	// クリア
			// プレイヤーを終了状態に移行
			SetPlayerState(PLAYERSTATE_END);

			sprintf(
				&g_pFontInfo->aText[0],
				"Level %02d/%02d | Time %03d | Score %06d",
				g_nCurrentStage + 1,
				MAX_LEVEL,
				(g_nTimer) / 10,
				g_nScore
			);

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

	if (g_bIsPause)
	{// ポーズメニュー描画
		DrawPauseBg();
		DrawPause();
	}

	DrawFont();
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
	g_nTimer = TIME_STATE_READY;
	g_nScore = 0;
}

//=====================================================================
// ポーズ状態の切り替え処理
//=====================================================================
void TogglePause(bool bIsPause)
{
	if (GetFade() != FADE_NONE) return;
	if (GetCurrentScene() != SCENE_GAME) return;
	if (g_gameState == GAMESTATE_READY) return;

	g_bIsPause = bIsPause;
	g_pFontPauseMenuTitle->obj.bVisible = bIsPause;
	g_pFontCountDown->obj.bVisible = !bIsPause;
	GetPauseBg()->bVisible = bIsPause;

	if (bIsPause)
	{
		SetPauseMenuCursor(0);
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
			for (int i = 0; i < MAX_BLOCK_PARAM; i++)
			{
				g_map[y][x].nParam[i] = map->nParam[i];
			}
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

void DecreaseTime(int nTime)
{
	if (g_gameState != GAMESTATE_NORMAL) return;

	g_nTimer -= nTime;
	Clamp(&g_nTimer, 0, g_nTimer);
	g_pFontInfo->obj.color = D3DXCOLOR(1, 0, 0, 1);
}
