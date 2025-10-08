//=====================================================================
// 
// �v���C���[���� [enemy.cpp]
// Author : 
//
//=====================================================================
#include "enemy.h"
#include "input.h"
#include "util.h"
#include "Game.h"
#include "collision.h"
#include "player.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		NULL
#define TEXTURE_MAX_X			(4)
#define TEXTURE_MAX_Y			(2)
#define INIT_POS				D3DXVECTOR3(SCREEN_CENTER, SCREEN_HEIGHT - 100, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(50.0f, 75.0f, 0.0f) * 0.7f
#define INIT_COLOR				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define INIT_HITBOX				D3DXVECTOR3(11.0f, 75.0f, 0.0f) * 0.7f

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEnemy = NULL;
ENEMY g_aEnemy[MAX_ENEMY];

//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	ENEMY* pEnemy = &g_aEnemy[0];

	// �\���̂̏�����
	memset(pEnemy, 0, sizeof(ENEMY) * MAX_ENEMY);
	for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
	{
		pEnemy->obj.pos = INIT_POS;
		pEnemy->obj.size = INIT_SIZE;
		pEnemy->obj.color = INIT_COLOR;
		pEnemy->obj.bVisible = false;
		pEnemy->bUsed = false;
		pEnemy->hitBoxSize = INIT_HITBOX;
	}

	// �e�N�X�`���̓ǂݍ���
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffEnemy
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitEnemy(void)
{
	if (g_pVtxBuffEnemy != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	if (g_pTexBuffEnemy != NULL)
	{// �e�N�X�`���o�b�t�@�̔j��
		g_pTexBuffEnemy->Release();
		g_pTexBuffEnemy = NULL;
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdateEnemy(void)
{
	ENEMY* pEnemy = &g_aEnemy[0];
	PLAYER* pPlayer = GetPlayer();

	for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
	{
		if (pEnemy->bUsed == false) continue;

		pEnemy->posOld = pEnemy->obj.pos;

		pEnemy->obj.pos += pEnemy->move;

		if (abs((long)pEnemy->move.x) > 0.01f && Magnitude(pEnemy->obj.pos, pEnemy->posOld) > 0.1f)
		{
			if (pEnemy->nCounterAnim % 10 == 0)
			{
				pEnemy->nPatternAnimX = (pEnemy->nPatternAnimX + 1) % 4;
			}
			pEnemy->nCounterAnim++;
		}
		else
		{
			pEnemy->nPatternAnimX = 0;
		}

		pEnemy->move.y += GAME_GRAVITY;

		if (CollisionBlock(&pEnemy->obj.pos, &pEnemy->posOld, &pEnemy->move, pEnemy->hitBoxSize, &pEnemy->pBlock))
		{
			pEnemy->bIsJumping = false;
		}
		else
		{
			pEnemy->bIsJumping = true;

		}

		if (BoxCollision(
			pEnemy->obj.pos - D3DXVECTOR3(0, pEnemy->obj.size.y, 0), pEnemy->hitBoxSize,
			pPlayer->obj.pos - D3DXVECTOR3(0, pPlayer->obj.size.y, 0), pPlayer->hitBoxSize
		))
		{// �v���C���[�Ƃ̏Փ˔���
			
		}
	}

}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;
	VERTEX_2D* pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ENEMY; i++, pEnemy++, pVtx += 4)
	{// ���_����ݒ�
		SetVertexPos(pVtx,
			D3DXVECTOR3(pEnemy->obj.pos.x - pEnemy->obj.size.x / 2.0f, pEnemy->obj.pos.y - pEnemy->obj.size.y, 0.0f),
			D3DXVECTOR3(pEnemy->obj.pos.x + pEnemy->obj.size.x / 2.0f, pEnemy->obj.pos.y - pEnemy->obj.size.y, 0.0f),
			D3DXVECTOR3(pEnemy->obj.pos.x - pEnemy->obj.size.x / 2.0f, pEnemy->obj.pos.y, 0.0f),
			D3DXVECTOR3(pEnemy->obj.pos.x + pEnemy->obj.size.x / 2.0f, pEnemy->obj.pos.y, 0.0f)
		);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pEnemy->obj.color);
		SetVertexTexturePos(pVtx, pEnemy->nPatternAnimX, pEnemy->nPatternAnimY, 4, 2, pEnemy->obj.bInversed);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pEnemy = &g_aEnemy[0];
	for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
	{
		if (pEnemy->obj.bVisible == true && pEnemy->bUsed == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffEnemy);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}

}

ENEMY* SetEnemy(ENEMY_TYPE type, D3DXVECTOR3 pos)
{
	ENEMY* pEnemy = &g_aEnemy[0];
	for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
	{
		if (pEnemy->bUsed == true) continue;

		pEnemy->bUsed = true;
		pEnemy->obj.pos = pos;
		pEnemy->obj.size = INIT_SIZE;
		pEnemy->obj.color = INIT_COLOR;
		pEnemy->obj.bVisible = true;
		pEnemy->hitBoxSize = INIT_HITBOX;
		pEnemy->move = { 4,0 ,0};

		return pEnemy;
	}

	return NULL;
}