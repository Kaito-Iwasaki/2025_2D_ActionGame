//=====================================================================
//
// Game [Game.h]
// Author : 
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
int CurrentBlock = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	InitDecal();
	InitBlock();
	InitPlayer();
	InitEnemy();

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

	for (int i = 0; i < NUM_BLOCK_X; i++)
	{
		SetBlock(
			BLOCK_TYPE_VOID,
			i, 0
		);
	}

	for (int i = 0; i < NUM_BLOCK_X; i++)
	{
		SetBlock(
			BLOCK_TYPE_VOID,
			i, NUM_BLOCK_Y - 1
		);
	}

	for (int i = 0; i < NUM_BLOCK_Y; i++)
	{
		SetBlock(
			BLOCK_TYPE_VOID,
			0, i
		);
	}

	for (int i = 0; i < NUM_BLOCK_Y; i++)
	{
		SetBlock(
			BLOCK_TYPE_VOID,
			NUM_BLOCK_X - 1, i
		);
	}

	SetBlock(
		BLOCK_TYPE_BLOCK000,
		14, NUM_BLOCK_Y - 2
	);

	//SetEnemy(
	//	ENEMY_TYPE_000,
	//	D3DXVECTOR3(100, 100, 0)
	//);
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
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	UpdateBlock();
	UpdatePlayer();
	UpdateEnemy();

	D3DXVECTOR2 posMouse = GetMousePos();
	D3DXVECTOR3 posBlock = D3DXVECTOR3(
		floorf(posMouse.x / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE / 2,
		floorf(posMouse.y / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE / 2,
		0.0f
	);
	g_pCursor->obj.pos = posBlock;

	if (GetMouse().lZ <= -120)
	{
		CurrentBlock++;
	}
	else if (GetMouse().lZ >= 120)
	{
		CurrentBlock--;
	}
	if (CurrentBlock == -1)
	{
		CurrentBlock = BLOCK_TYPE_MAX - 1;
	}
	else
	{
		CurrentBlock %= BLOCK_TYPE_MAX;
	}

	if (GetMousePress(MOUSE_LEFT))
	{
		SetBlock(
			(BLOCK_TYPE)(CurrentBlock + 1),
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
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawDecal();
	DrawBlock();
	DrawEnemy();
	DrawPlayer();
}