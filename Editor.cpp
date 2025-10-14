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
#include "fade.h"
#include "font.h"

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
int CurrentBlock = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitEditor(void)
{
	InitDecal();
	InitFont();
	InitBlock();

	SetDecal(
		DECAL_LABEL_BG000,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

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
		"選択ブロック：BLOCK_000",
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
	UninitBlock();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEditor(void)
{
	UpdateBlock();

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
		CurrentBlock++;
	}
	else if (GetMouse().lZ >= 120)
	{
		CurrentBlock--;
	}

	CurrentBlock %= (BLOCK_TYPE_MAX - 1);
	if (CurrentBlock == 0)
	{
		CurrentBlock = (BLOCK_TYPE_MAX - 1);
	}

	sprintf(&g_pFont->aText[0], "選択:%d", CurrentBlock);

	if (GetMousePress(MOUSE_LEFT))
	{
		SetBlock(
			(BLOCK_TYPE)(CurrentBlock),
			(int)posMouse.x / BLOCK_SIZE,
			(int)posMouse.y / BLOCK_SIZE
		);
	}
	else if (GetMousePress(MOUSE_RIGHT))
	{
		SetBlock(
			BLOCK_TYPE_AIR,
			(int)posMouse.x / BLOCK_SIZE,
			(int)posMouse.y / BLOCK_SIZE
		);
	}

	if (GetKeyboardTrigger(DIK_F5))
	{
		SetScene(SCENE_GAME);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawEditor(void)
{
	DrawDecal();
	DrawFont();
	DrawBlock();
}