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

//=====================================================================
// 初期化処理
//=====================================================================
void InitEditor(void)
{
	InitDecal();
	InitFont();
	InitEditorBlock();

	SetDecal(
		DECAL_LABEL_BG000,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

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
		D3DXVECTOR3_ZERO,
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

	D3DXVECTOR2 posMouse = GetMousePos();
	D3DXVECTOR3 posBlock = D3DXVECTOR3(
		floorf(posMouse.x / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE / 2,
		floorf(posMouse.y / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE / 2,
		0.0f
	);
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

	sprintf(&g_pFont->aText[0], "ブロック:%d\n出力ファイル名：data\\MAP\\level%02d.bin", g_nCurrentBlock, g_nOutputLevel);

	if (GetMousePress(MOUSE_LEFT))
	{
		SetEditorBlock(
			(BLOCK_TYPE)(g_nCurrentBlock),
			(int)posMouse.x / BLOCK_SIZE,
			(int)posMouse.y / BLOCK_SIZE
		);
	}
	else if (GetMousePress(MOUSE_RIGHT))
	{
		SetEditorBlock(
			BLOCK_TYPE_AIR,
			(int)posMouse.x / BLOCK_SIZE,
			(int)posMouse.y / BLOCK_SIZE
		);
	}

	if (GetKeyboardTrigger(DIK_F2))
	{
		sprintf(&aFileName[0], "data\\MAP\\level%02d.bin", g_nOutputLevel);

		SaveEditorBlock(&aFileName[0]);
	}


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
	DrawDecal();
	DrawEditorBlock();
	DrawFont();
}