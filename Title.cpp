//=====================================================================
//
// Title [Title.h]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Title.h"
#include "decal.h"
#include "input.h"
#include "fade.h"
#include "Game.h"
#include "sound.h"
#include "background.h"
#include "util.h"
#include "font.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TIME_STATE_INTRO	(60)
#define TIME_STATE_START	(60)

#define LOGO_POS_X			SCREEN_CENTER
#define LOGO_POS_Y			SCREEN_VCENTER - 200
#define LOGO_POS			D3DXVECTOR3(LOGO_POS_X, LOGO_POS_Y, 0)
#define LOGO_SIZE_X			(450.0f)
#define LOGO_SIZE_Y			(200.0f)
#define LOGO_SIZE			D3DXVECTOR3(LOGO_SIZE_X, LOGO_SIZE_Y, 0)
#define LOGO_COLOR			D3DXCOLOR_WHITE
#define LOGO_INIT_SCALE		(1.5f)

#define COLOR_SELECTED			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_DESELECTED		D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	TITLESTATE_INTRO = 0,
	TITLESTATE_NORMAL,
	TITLESTATE_PRESSED,
	TITLESTATE_MAX
}TITLESTATE;

typedef enum
{
	TITLESELECTION_START = 0,
	TITLESELECTION_RANKING,
	TITLESELECTION_CREDIT,
	TITLESELECTION_QUIT,
	TITLESELECTION_MAX
}TITLESELECTION;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
TITLESTATE g_stateTitle = TITLESTATE_INTRO;
DECAL* g_pDecalLogo = NULL;
FONT* g_pFontTitleSelection[TITLESELECTION_MAX] = {};
int g_nCounterStateTitle = 0;
int g_nElapsedTimeTitle = 0;
int g_nSelectTitle = 0;

const char* g_aTitleSelection[TITLESELECTION_MAX] = {
	"Start",
	"RANKING",
	"Credit",
	"Quit"
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitTitle(void)
{
	InitDecal();
	InitFont();
	InitBackground();

	g_stateTitle = TITLESTATE_INTRO;
	g_nCounterStateTitle = 0;
	g_nElapsedTimeTitle = 0;
	g_nSelectTitle = 0;

	g_pDecalLogo = SetDecal(
		DECAL_LABEL_LOGO,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER - 200, 0),
		D3DXVECTOR3(450, 200, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)
	);

	for (int i = 0; i < TITLESELECTION_MAX; i++)
	{
		g_pFontTitleSelection[i] = SetFont(
			FONT_LABEL_DONGURI,
			D3DXVECTOR3(0, (SCREEN_VCENTER + 100) + i * 50, 0),
			D3DXVECTOR3(SCREEN_WIDTH, 50, 0),
			D3DXCOLOR_BLACK,
			50,
			g_aTitleSelection[i],
			DT_CENTER
		);
		g_pFontTitleSelection[i]->obj.bVisible = false;
	}

	StopSound();
	PlaySound(SOUND_LABEL_BGM_TITLE00);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitTitle(void)
{
	UninitDecal();
	UninitFont();
	UninitBackground();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTitle(void)
{
	UpdateBackground();
	
	g_nElapsedTimeTitle++;
	g_nCounterStateTitle++;
	switch (g_stateTitle)
	{
	case TITLESTATE_INTRO:
	{
		float fProgress = (float)g_nCounterStateTitle / (float)TIME_STATE_INTRO;

		g_pDecalLogo->obj.color.a = fProgress;
		g_pDecalLogo->obj.size = D3DXVECTOR3(
			Lerpf(LOGO_SIZE_X  * 3.0f, LOGO_SIZE_X, fProgress),
			Lerpf(LOGO_SIZE_Y * 3.0f, LOGO_SIZE_Y, fProgress),
			0.0f
		);

		if (fProgress >= 1.0f || INPUT_TRIGGER_ACCEPT)
		{
			g_pDecalLogo->obj.color.a = 1.0f;
			g_pDecalLogo->obj.size = LOGO_SIZE;

			for (int i = 0; i < TITLESELECTION_MAX; i++)
			{
				g_pFontTitleSelection[i]->obj.bVisible = true;
			}

			g_stateTitle = TITLESTATE_NORMAL;
			g_nCounterStateTitle = 0;
		}

		break;
	}

	case TITLESTATE_NORMAL:
		if (INPUT_TRIGGER_ACCEPT)
		{// 
			g_stateTitle = TITLESTATE_PRESSED;
			g_nCounterStateTitle = 0;
			break;
		}

		if (INPUT_REPEAT_UP)
		{
			g_nSelectTitle--;
		}
		else if (INPUT_REPEAT_DOWN)
		{
			g_nSelectTitle++;
		}
		g_nSelectTitle = (g_nSelectTitle + TITLESELECTION_MAX) % TITLESELECTION_MAX;

		for (int i = 0; i < TITLESELECTION_MAX; i++)
		{
			if (i == g_nSelectTitle)
			{
				g_pFontTitleSelection[i]->obj.color = COLOR_SELECTED;
			}
			else
			{
				g_pFontTitleSelection[i]->obj.color = COLOR_DESELECTED;
			}
		}

		break;

	case TITLESTATE_PRESSED:
		if (g_nCounterStateTitle % 5 == 0)
		{
			g_pFontTitleSelection[g_nSelectTitle]->obj.bVisible ^= 1;
		}

		if (g_nCounterStateTitle == TIME_STATE_INTRO)
		{
			switch (g_nSelectTitle)
			{
			case TITLESELECTION_START:
				ResetGame();
				SetFade(SCENE_GAME);
				break;

			case TITLESELECTION_RANKING:
				SetFade(SCENE_RANKING);
				break;

			case TITLESELECTION_CREDIT:
				g_stateTitle = TITLESTATE_NORMAL;
				break;

			case TITLESELECTION_QUIT:
				g_stateTitle = TITLESTATE_NORMAL;
				break;
			}
		}
		break;
	}

	g_pDecalLogo->obj.pos = D3DXVECTOR3(LOGO_POS_X + sinf(g_nElapsedTimeTitle * 0.025f) * 10.0f, LOGO_POS_Y + cosf(g_nElapsedTimeTitle * 0.05f) * 10.0f, 0);
	g_pDecalLogo->obj.rot.z = sinf(g_nElapsedTimeTitle * 0.01f) * 0.075f;

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{
		SetFade(SCENE_RANKING);
	}
#endif

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTitle(void)
{
	DrawBackground();
	DrawDecal();
	DrawFont();
}