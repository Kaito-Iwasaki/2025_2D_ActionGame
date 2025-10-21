//=====================================================================
//
// �u���b�N�������� [block_act.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
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

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************


//=====================================================================
// �j�u���b�N
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
		SetPlayerState(PLAYERSTATE_DIED);
	}
}

//=====================================================================
// �ړ����u���b�N
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
// �R�C���u���b�N
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

		pBlock->bUsed = false;
	}
}

//=====================================================================
// �S�[���u���b�N
//=====================================================================
void BLOCK_Goal(BLOCK* pBlock)
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
		if (GetGameState() == GAMESTATE_NORMAL)
		{
			SetGameState(GAMESTATE_CLEAR);
		}
	}
}