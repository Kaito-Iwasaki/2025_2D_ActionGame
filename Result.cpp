//=====================================================================
//
// リザルト画面処理 [Result.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Result.h"
#include "input.h"
#include "sound.h"
#include "decal.h"
#include "fade.h"
#include "font.h"
#include "Game.h"
#include "background.h"
#include "Ranking.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	RESULTSTATE_COUNTREADY = 0,
	RESULTSTATE_COUNTUP,
	RESULTSTATE_END,
	RESULTSTATE_MAX
}RESULTSTATE;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
RESULTSTATE g_stateResult = RESULTSTATE_COUNTREADY;
FONT* g_pFontResultInfo = NULL;
FONT* g_pFontResultScore = NULL;
int g_nCounterState = 0;
int g_nCounterScore = 0;
int g_nTimeLeft = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitResult(void)
{
	InitFont();
	InitDecal();
	InitBackground();

	g_stateResult = RESULTSTATE_COUNTREADY;
	g_nCounterState = 0;
	g_nCounterScore = GetScore();
	g_nTimeLeft = GetGameTimeLeft() / 10;

	SetBackgroundColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

	g_pFontResultInfo = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		30,
		"",
		DT_CENTER
	);


	g_pFontResultScore = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		100,
		"",
		DT_CENTER | DT_VCENTER
	);

	SaveScore(GetScore() + GetGameTimeLeft() / 10 * 10);

	StopSound();
	PlaySound(SOUND_LABEL_BGM_RESULT00);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitResult(void)
{
	UninitFont();
	UninitDecal();
	UninitBackground();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateResult(void)
{
	UpdateBackground();

	g_nCounterState++;
	switch (g_stateResult)
	{
	case RESULTSTATE_COUNTREADY:
		if (g_nCounterState > 60)
		{
			g_stateResult = RESULTSTATE_COUNTUP;
		}
		break;

	case RESULTSTATE_COUNTUP:
		if (INPUT_TRIGGER_ACCEPT)
		{
			g_nCounterScore = GetScore() + (GetGameTimeLeft() / 10 * 10);
			g_nTimeLeft = 0;
		}

		if (g_nTimeLeft < 1 )
		{
			g_stateResult = RESULTSTATE_END;
			g_nCounterState = 0;
		}
		else
		{
			g_nCounterScore += 10;
			g_nTimeLeft--;
			PlaySound(SOUND_LABEL_SE_COIN);
		}

		break;

	case RESULTSTATE_END:
		if (INPUT_TRIGGER_ACCEPT  || g_nCounterState > 360)
		{
			SetFade(SCENE_RANKING);
		}
		break;
	}

	sprintf(&g_pFontResultInfo->aText[0], "Level %02d | Time %03d | Score | %06d", GetStage(), g_nTimeLeft, g_nCounterScore);
	sprintf(&g_pFontResultScore->aText[0], "YOUR SCORE\n%06d", g_nCounterScore);
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawResult(void)
{
	DrawBackground();
	DrawDecal();
	DrawFont();
}