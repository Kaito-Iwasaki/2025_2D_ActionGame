//=====================================================================
//
// Editor [Editor.h]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "sound.h"
#include "input.h"
#include "Editor.h"
#include "decal.h"
#include "block.h"
#include "editorblock.h"
#include "fade.h"
#include "font.h"
#include "Game.h"
#include "background.h"
#include "util.h"

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


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
DECAL* g_pCursor = NULL;
FONT* g_pFont = NULL;
int g_nCurrentBlock = 1;
int g_nOutputLevel = 0;
int g_nSelectedParam = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitEditor(void)
{
	InitBackground();
	InitDecal();
	InitFont();
	InitEditorBlock();

	g_nCurrentBlock = 1;
	g_nOutputLevel = GetStage();

	g_pCursor = SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f)
	);
	g_pCursor->obj.bVisible = false;

	g_pFont = SetFont(
		FONT_LABEL_TERMINAL,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		30,
		"",
		DT_LEFT
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitEditor(void)
{
	UninitBackground();
	UninitDecal();
	UninitFont();
	UninitEditorBlock();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEditor(void)
{
	UpdateEditorBlock();

	char aFileName[MAX_PATH] = {};
	EDITORBLOCK* pEditorBlock = GetEditorBlock();

	D3DXVECTOR2 posMouse = GetMousePos();
	D3DXVECTOR3 posBlock = D3DXVECTOR3(
		floorf(posMouse.x / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE / 2,
		floorf(posMouse.y / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE / 2,
		0.0f
	);
	int nBlockX = Clamp((int)posMouse.x / BLOCK_SIZE, 0, NUM_BLOCK_X - 1);
	int nBlockY = Clamp((int)posMouse.y / BLOCK_SIZE, 0, NUM_BLOCK_Y - 1);
	g_pCursor->obj.pos = posBlock;
	g_pCursor->obj.bVisible = true;

	if (GetMouse().lZ <= -120)
	{
		g_nCurrentBlock++;
	}
	else if (GetMouse().lZ >= 120)
	{
		g_nCurrentBlock--;
	}

	g_nCurrentBlock %= (BLOCK_TYPE_MAX - 1);
	if (g_nCurrentBlock == 0)
	{
		g_nCurrentBlock = (BLOCK_TYPE_MAX - 1);
	}

	if (GetKeyboardRepeat(DIK_UP))
	{
		g_nOutputLevel++;
	}
	else if (GetKeyboardRepeat(DIK_DOWN))
	{
		g_nOutputLevel--;
	}
	g_nOutputLevel = (g_nOutputLevel + MAX_LEVEL) % MAX_LEVEL;

	if (GetKeyboardRepeat(DIK_A))
	{
		g_nSelectedParam--;
	}
	if (GetKeyboardRepeat(DIK_D))
	{
		g_nSelectedParam++;
	}
	g_nSelectedParam = (g_nSelectedParam + MAX_BLOCK_PARAM) % MAX_BLOCK_PARAM;

	if (GetKeyboardRepeat(DIK_W, 1))
	{
		pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[g_nSelectedParam]++;
	}
	if (GetKeyboardRepeat(DIK_S, 1))
	{
		pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[g_nSelectedParam]--;
	}

	if (pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[g_nSelectedParam] > MAX_BLOCK_PARAM_VALUE)
	{
		pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[g_nSelectedParam] = -MAX_BLOCK_PARAM_VALUE;
	}
	else if (pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[g_nSelectedParam] < -MAX_BLOCK_PARAM_VALUE)
	{
		pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[g_nSelectedParam] = MAX_BLOCK_PARAM_VALUE;
	}

	if (GetMousePress(MOUSE_LEFT))
	{
		SetEditorBlock(
			(BLOCK_TYPE)(g_nCurrentBlock),
			nBlockX,
			nBlockY
		);
	}
	else if (GetMousePress(MOUSE_RIGHT))
	{
		SetEditorBlock(
			BLOCK_TYPE_AIR,
			nBlockX,
			nBlockY
		);
	}

	if (GetKeyboardPress(DIK_R))
	{
		for (int i = 0; i < MAX_BLOCK_PARAM; i++)
		{
			pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[i] = 0;
		}
	}

	static bool bKeyPressed = false;
	if (GetKeyboardTrigger(DIK_F2) && !bKeyPressed)
	{
		bKeyPressed = true;
		MessageBeep(MB_ICONASTERISK);
		int nID = MessageBox(GetMainWindow(), "ファイル出力する？", "レベルデータ出力", MB_YESNO);

		if (nID == IDYES)
		{
			sprintf(&aFileName[0], "data\\MAP\\level%02d.bin", g_nOutputLevel);

			if (SaveEditorBlock(&aFileName[0]))
			{
				MessageBox(GetMainWindow(), "出力したよー", "成功", MB_OK);
			}
			else
			{
				MessageBox(GetMainWindow(), "失敗したよー", "失敗", MB_OK | MB_ICONERROR);
			}

		}
	}
	if (GetKeyboardPress(DIK_F2) == false)
	{
		bKeyPressed = false;
	}

	sprintf(&g_pFont->aText[0],
		"[ X: %d | Y: %d ] \n"
		"ブロック:%d\n"
		"パラメータ(%d) [ %d | %d | %d | %d ]\n"
		"出力ファイル名:data\\MAP\\level%02d.bin\n"
		"ファイル出力:[F2]",
		nBlockX, nBlockY,
		g_nCurrentBlock,
		g_nSelectedParam,
		pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[0],
		pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[1],
		pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[2],
		pEditorBlock[nBlockY * NUM_BLOCK_X + nBlockX].nParam[3],
		g_nOutputLevel
	);

	if (GetKeyboardTrigger(DIK_F5))
	{
		SetStage(g_nOutputLevel);
		SetScene(SCENE_GAME);
	}
	else if (GetKeyboardTrigger(DIK_F6))
	{
		SetStage(0);
		SetScene(SCENE_GAME);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawEditor(void)
{
	DrawBackground();
	DrawDecal();
	DrawEditorBlock();
	DrawFont();
}