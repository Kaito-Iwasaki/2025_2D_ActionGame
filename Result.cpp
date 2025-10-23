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
	RESULTSTATE_COUNTUP = 0,

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
RESULTSTATE g_stateResult = RESULTSTATE_COUNTUP;
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

	g_stateResult = RESULTSTATE_COUNTUP;
	g_nCounterState = 0;
	g_nCounterScore = GetScore();
	g_nTimeLeft = GetGameTimeLeft() / 10;

	//SetDecal(
	//	DECAL_LABEL_RESULT_BG000,
	//	D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
	//	D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
	//	D3DXVECTOR3_ZERO,
	//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	//);

	g_pFontResultScore = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		100,
		"",
		DT_CENTER | DT_VCENTER
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitResult(void)
{
	UninitFont();
	UninitDecal();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateResult(void)
{
	g_nCounterState++;
	switch (g_stateResult)
	{
	case RESULTSTATE_COUNTUP:
		if (INPUT_TRIGGER_ACCEPT)
		{
			g_nCounterScore = GetScore() + (GetGameTimeLeft() / 10 * 10);
			g_nTimeLeft = 0;
		}

		if (g_nTimeLeft < 1 )
		{
			g_stateResult = RESULTSTATE_END;
		}
		else
		{
			g_nCounterScore += 10;
			g_nTimeLeft--;
		}
		PlaySound(SOUND_LABEL_SE_COIN);

		break;

	case RESULTSTATE_END:
		if (INPUT_TRIGGER_ACCEPT)
		{
			SetFade(SCENE_TITLE);
		}
		break;
	}

	sprintf(&g_pFontResultScore->aText[0], "%06d", g_nCounterScore);
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawResult(void)
{
	DrawDecal();
	DrawFont();
}