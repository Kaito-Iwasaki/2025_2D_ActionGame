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
#include "item.h"

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

BLOCK g_aMap[NUM_BLOCK_Y][NUM_BLOCK_X] = {};

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	InitDecal();
	InitBlock();
	InitPlayer();
	InitEnemy();
	InitItem();

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

	LoadBin("data\\MAP\\map.bin", &g_aMap[0][0], sizeof(BLOCK), MAX_BLOCK);

	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			SetBlock(g_aMap[y][x].type, x, y);
		}
	}

	SetEnemy(
		ENEMY_TYPE_000,
		D3DXVECTOR3(900, 600, 0)
	);
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
	UninitItem();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	UpdateBlock();
	UpdatePlayer();
	UpdateEnemy();
	UpdateItem();

#ifdef _DEBUG
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

	if (GetKeyboardTrigger(DIK_F1))
	{
		SaveBin("data\\MAP\\map.bin", GetBlock(), sizeof(BLOCK), MAX_BLOCK);
	}
#endif
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawDecal();
	DrawBlock();
	DrawItem();
	DrawEnemy();
	DrawPlayer();
}