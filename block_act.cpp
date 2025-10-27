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
#include "effect.h"
#include "particle.h"
#include "sound.h"
#include "input.h"

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


//=====================================================================
// 針ブロック
//=====================================================================
void BLOCK_Needle(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->state == PLAYERSTATE_INIT) return;
	if (pPlayer->state == PLAYERSTATE_DIED) return;

	D3DXVECTOR3 posBlockCenter = pBlock->obj.pos + D3DXVECTOR3(BLOCK_SIZE / 2, BLOCK_SIZE / 2, 0);

	if (BoxCollision(
		posBlockCenter,
		D3DXVECTOR3(BLOCK_SIZE * 0.75f, BLOCK_SIZE * 0.75f, 0.0f),
		pPlayer->obj.pos + D3DXVECTOR3(0, -pPlayer->obj.size.y / 2, 0),
		pPlayer->hitBoxSize
	))
	{
		KillPlayer();
	}
}

//=====================================================================
// 移動床ブロック
//=====================================================================
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

//=====================================================================
// コインブロック
//=====================================================================
void BLOCK_Coin(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->state == PLAYERSTATE_INIT) return;
	if (pPlayer->state == PLAYERSTATE_DIED) return;

	D3DXVECTOR3 posBlockCenter = pBlock->obj.pos + D3DXVECTOR3(BLOCK_SIZE / 2, BLOCK_SIZE / 2, 0);

	if (BoxCollision(
		posBlockCenter,
		D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE, 0.0f),
		pPlayer->obj.pos + D3DXVECTOR3(0, -pPlayer->obj.size.y / 2, 0),
		pPlayer->hitBoxSize
	))
	{
		EFFECTINFO info;
		info.col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
		info.fMaxAlpha = 1.0f;
		info.fMaxScale = 0.3f;
		info.fRotSpeed = 0.5f;
		info.fSpeed = 2.0f;
		info.nMaxLife = 30;

		SetParticle(
			info,
			posBlockCenter,
			0.0f,
			D3DX_PI * 2,
			1,
			10
		);

		PlaySound(SOUND_LABEL_SE_COIN);

		AddScore(200);

		pBlock->bUsed = false;
	}
}

void BLOCK_RedCoin(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->state == PLAYERSTATE_INIT) return;
	if (pPlayer->state == PLAYERSTATE_DIED) return;

	D3DXVECTOR3 posBlockCenter = pBlock->obj.pos + D3DXVECTOR3(BLOCK_SIZE / 2, BLOCK_SIZE / 2, 0);

	if (BoxCollision(
		posBlockCenter,
		D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE, 0.0f),
		pPlayer->obj.pos + D3DXVECTOR3(0, -pPlayer->obj.size.y / 2, 0),
		pPlayer->hitBoxSize
	))
	{
		EFFECTINFO info;
		info.col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
		info.fMaxAlpha = 1.0f;
		info.fMaxScale = 0.5f;
		info.fRotSpeed = 0.5f;
		info.fSpeed = 2.0f;
		info.nMaxLife = 40;

		SetParticle(
			info,
			posBlockCenter,
			0.0f,
			D3DX_PI * 2,
			1,
			10
		);

		PlaySound(SOUND_LABEL_SE_COIN);

		AddScore(400);

		pBlock->bUsed = false;
	}
}

void BLOCK_Energey(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->state == PLAYERSTATE_INIT) return;
	if (pPlayer->state == PLAYERSTATE_DIED) return;

	D3DXVECTOR3 posBlockCenter = pBlock->obj.pos + D3DXVECTOR3(BLOCK_SIZE / 2, BLOCK_SIZE / 2, 0);

	EFFECTINFO info;
	info.col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
	info.fMaxAlpha = 1.0f;
	info.fMaxScale = 0.3f;
	info.fRotSpeed = 0.5f;
	info.fSpeed = 2.0f;
	info.nMaxLife = 30;

	if (pBlock->obj.bVisible == false)
	{
		pBlock->nCounterState++;
		
		if (pBlock->nCounterState > 180)
		{
			pBlock->obj.bVisible = true;
			pBlock->nCounterState = 0;

			SetParticle(
				info,
				posBlockCenter,
				0.0f,
				D3DX_PI * 2,
				1,
				10
			);
		}
	}

	if (BoxCollision(
		posBlockCenter,
		D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE, 0.0f),
		pPlayer->obj.pos + D3DXVECTOR3(0, -pPlayer->obj.size.y / 2, 0),
		pPlayer->hitBoxSize
	))
	{
		if (pBlock->obj.bVisible == false) return;

		SetParticle(
			info,
			posBlockCenter,
			0.0f,
			D3DX_PI * 2,
			1,
			10
		);

		PlaySound(SOUND_LABEL_SE_COIN);

		pPlayer->fCharge = PLAYER_CHARGE_MAX;
		if (GetKeyboardPress(DIK_SPACE) || GetJoypadPress(JOYKEY_A))
		{
			PlaySound(SOUND_LABEL_SE_JET, 0.1f);
			pPlayer->bIsFlying = true;
		}

		pBlock->obj.bVisible = false;
	}
}

//=====================================================================
// ゴールブロック
//=====================================================================
void BLOCK_Goal(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();

	if (GetGameState() == GAMESTATE_CLEAR) return;
	if (pPlayer->state == PLAYERSTATE_INIT) return;
	if (pPlayer->state == PLAYERSTATE_DIED) return;
	if (pPlayer->state == PLAYERSTATE_END) return;

	D3DXVECTOR3 posBlockCenter = pBlock->obj.pos + D3DXVECTOR3(BLOCK_SIZE / 2, BLOCK_SIZE / 2, 0);

	if (BoxCollision(
		posBlockCenter,
		D3DXVECTOR3(BLOCK_SIZE * 0.25f, BLOCK_SIZE * 0.25f, 0.0f),
		pPlayer->obj.pos + D3DXVECTOR3(0, -pPlayer->obj.size.y / 2, 0),
		pPlayer->hitBoxSize
	))
	{
		if (GetGameState() == GAMESTATE_NORMAL)
		{
			EFFECTINFO info;
			info.col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
			info.fMaxAlpha = 1.0f;
			info.fMaxScale = 0.5f;
			info.fRotSpeed = 0.5f;
			info.fSpeed = 1.0f;
			info.nMaxLife = 70;

			SetParticle(
				info,
				pPlayer->obj.pos,
				0.0f,
				D3DX_PI * 2,
				1,
				10
			);

			info.col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);

			SetParticle(
				info,
				pPlayer->obj.pos,
				0.0f,
				D3DX_PI * 2,
				1,
				10
			);

			info.col = D3DXCOLOR(0.9f, 1.0f, 0.9f, 1.0f);

			SetParticle(
				info,
				pPlayer->obj.pos,
				0.0f,
				D3DX_PI * 2,
				1,
				10
			);

			PlaySound(SOUND_LABEL_SE_CLEAR);
			AddScore(1000);

			SetGameState(GAMESTATE_CLEAR);
		}
	}

	pBlock->nCounterState++;
	if (pBlock->nCounterState % 10 == 0)
	{
		pBlock->nPatternAnimX = (pBlock->nPatternAnimX + 1) % 4;
	}
}

//=====================================================================
// 開始ブロック
//=====================================================================
void BLOCK_StartBlock(BLOCK* pBlock)
{
	GetPlayer()->startPos = pBlock->obj.pos + D3DXVECTOR3(BLOCK_SIZE / 2, BLOCK_SIZE, 0);;

	pBlock->bUsed = false;
}