//=====================================================================
//
// ブロック挙動処理 [block_act.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "block.h"
#include "block_act.h"
#include "collision.h"
#include "player.h"
#include "fade.h"
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
void BLOCK_Needle(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->state == PLAYERSTATE_INIT) return;
	if (pPlayer->state == PLAYERSTATE_DIED) return;

	if (BoxCollision(
		pBlock->obj.pos + D3DXVECTOR3(pBlock->obj.size.x / 2, -pBlock->obj.size.y / 2, 0), D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE * 0.25f, 0.0f),
		pPlayer->obj.pos - D3DXVECTOR3(0, pPlayer->obj.size.y, 0), pPlayer->hitBoxSize
	))
	{
		SetPlayerState(PLAYERSTATE_DIED);
	}
}

void BLOCK_Platform(BLOCK* pBlock)
{
	if (pBlock->nCounterState == 0)
	{
		pBlock->move.x = 1.0f;
	}
	pBlock->obj.pos.x += pBlock->move.x;

	if (pBlock->nCounterState % 180 == 0)
	{
		pBlock->move.x *= -1;
	}

	pBlock->nCounterState++;
}

void BLOCK_Coin(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->state == PLAYERSTATE_INIT) return;
	if (pPlayer->state == PLAYERSTATE_DIED) return;

	if (BoxCollision(
		pBlock->obj.pos + D3DXVECTOR3(pBlock->obj.size.x / 2, -pBlock->obj.size.y / 2, 0), D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE * 0.25f, 0.0f),
		pPlayer->obj.pos - D3DXVECTOR3(0, pPlayer->obj.size.y, 0), pPlayer->hitBoxSize
	))
	{
		pBlock->bUsed = false;
	}
}

void BLOCK_Goal(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();

	if (BoxCollision(
		pBlock->obj.pos + D3DXVECTOR3(pBlock->obj.size.x / 2, -pBlock->obj.size.y / 2, 0), D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE * 0.25f, 0.0f),
		pPlayer->obj.pos - D3DXVECTOR3(0, pPlayer->obj.size.y, 0), pPlayer->hitBoxSize
	))
	{
		if (GetGameState() == GAMESTATE_NORMAL)
		{
			SetGameState(GAMESTATE_CLEAR);
		}
	}
}