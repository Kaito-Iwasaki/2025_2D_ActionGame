//=====================================================================
//
// �G�e�I�u�W�F�N�g [enemybullet.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "input.h"
#include "sound.h"
#include "util.h"
#include "collision.h"
#include "block.h"
#include "block_act.h"

#include "player.h"
#include "baseScene.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_POS						D3DXVECTOR3_ZERO
#define INIT_SIZE						D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE, 0.0f)
#define INIT_COLOR						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBlock[BLOCK_TYPE_MAX] = {};
BLOCK g_aBlock[NUM_BLOCK_Y][NUM_BLOCK_X] = {};

const char* g_aBlockFileName[BLOCK_TYPE_MAX] = {
	NULL,
	NULL,
	"data\\TEXTURE\\grass000.png",
	"data\\TEXTURE\\grass001.png",
	"data\\TEXTURE\\grass002.png",
	"data\\TEXTURE\\grass003.png",
	"data\\TEXTURE\\grass004.png",
	"data\\TEXTURE\\grass005.png",
	"data\\TEXTURE\\grass006.png",
	"data\\TEXTURE\\grass007.png",
	"data\\TEXTURE\\grass008.png",
	"data\\TEXTURE\\grass009.png",
	"data\\TEXTURE\\grass010.png",
	"data\\TEXTURE\\grass011.png",
	"data\\TEXTURE\\grass012.png",
	"data\\TEXTURE\\needle000.png",
	"data\\TEXTURE\\needle001.png",
	"data\\TEXTURE\\needle002.png",
	"data\\TEXTURE\\needle003.png",
	"data\\TEXTURE\\grass012.png",
	"data\\TEXTURE\\item000.png",
	"data\\TEXTURE\\item001.png",
	"data\\TEXTURE\\item002.png",
	"data\\TEXTURE\\item003.png",
	"data\\TEXTURE\\flag001.png",
	"data\\TEXTURE\\player.png",
	"data\\TEXTURE\\wood000.png",
	"data\\TEXTURE\\wood001.png",
	"data\\TEXTURE\\wood002.png",
	"data\\TEXTURE\\wood003.png"
};

BLOCK_INFO g_aBlockInfo[BLOCK_TYPE_MAX] = {
	{D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), BLOCK_HIT_NONE},
	{D3DXCOLOR(0.4f, 0.2f, 0.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL, BLOCK_Platform},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL, BLOCK_Platform},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL, BLOCK_Platform},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL, BLOCK_Platform},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_Needle},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_Needle},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_Needle},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_Needle},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_ALL, BLOCK_Platform},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_Coin},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_RedCoin},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_Gas},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_Energy},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_Goal, 4, 2},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_NONE, BLOCK_StartBlock},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_TOP, BLOCK_Platform},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_TOP, BLOCK_Platform},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_TOP, BLOCK_Platform},
	{D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BLOCK_HIT_TOP, BLOCK_Platform},
};

//=====================================================================
// ����������
//=====================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	BLOCK* pBlock = &g_aBlock[0][0];

	// �\���̂̏�����
	memset(pBlock, 0, sizeof(BLOCK) * MAX_BLOCK);
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		pBlock->obj.pos = D3DXVECTOR3_ZERO;
		pBlock->obj.size = D3DXVECTOR3_ZERO;
		pBlock->obj.color = INIT_COLOR;
	}

	for (int nCount = 0; nCount < BLOCK_TYPE_MAX; nCount++)
	{
		Clamp(&g_aBlockInfo[nCount].nMaxPatternX, 1, g_aBlockInfo[nCount].nMaxPatternX + 1);
		Clamp(&g_aBlockInfo[nCount].nMaxPatternY, 1, g_aBlockInfo[nCount].nMaxPatternY + 1);
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCount = 0; nCount < BLOCK_TYPE_MAX; nCount++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_aBlockFileName[nCount],
			&g_pTexBuffBlock[nCount]
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitBlock(void)
{
	if (g_pVtxBuffBlock != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}

	for (int nCount = 0; nCount < BLOCK_TYPE_MAX; nCount++)
	{
		if (g_pTexBuffBlock[nCount] != NULL)
		{// �e�N�X�`���o�b�t�@�̔j��
			g_pTexBuffBlock[nCount]->Release();
			g_pTexBuffBlock[nCount] = NULL;
		}
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateBlock(void)
{
	BLOCK* pBlock = &g_aBlock[0][0];
	PLAYER* pPlayer = GetPlayer();

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		if (pBlock->bUsed == false) continue;

		D3DXVECTOR3 posBlockCenter = pBlock->obj.pos + D3DXVECTOR3(BLOCK_SIZE / 2, BLOCK_SIZE / 2, 0);

		pBlock->posOld = pBlock->obj.pos;

		if (pBlock->Update != NULL)
		{
			pBlock->Update(pBlock);
		}

		if (BoxCollision(
			posBlockCenter,
			D3DXVECTOR3(BLOCK_SIZE * 0.25f, BLOCK_SIZE * 0.25f, 0.0f),
			pPlayer->obj.pos + D3DXVECTOR3(0, -pPlayer->obj.size.y / 2, 0),
			pPlayer->hitBoxSize
		))
		{
			if (pBlock->dwCollisionType == BLOCK_HIT_ALL && pPlayer->state)
			{
				KillPlayer();
			}
		}
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	BLOCK* pBlock = &g_aBlock[0][0];

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx,
			D3DXVECTOR3(pBlock->obj.pos.x, pBlock->obj.pos.y, 0.0f),
			D3DXVECTOR3(pBlock->obj.pos.x + pBlock->obj.size.x, pBlock->obj.pos.y, 0.0f),
			D3DXVECTOR3(pBlock->obj.pos.x, pBlock->obj.pos.y + pBlock->obj.size.y, 0.0f),
			D3DXVECTOR3(pBlock->obj.pos.x + pBlock->obj.size.x, pBlock->obj.pos.y + pBlock->obj.size.y, 0.0f)
			);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pBlock->obj.color);
		SetVertexTexturePos(pVtx,
			D3DXVECTOR2(0.01f, 0.01f),
			D3DXVECTOR2(1.0f, 0.01f),
			D3DXVECTOR2(0.01f, 1.0f),
			D3DXVECTOR2(1.0f, 1.0f)
		);
		SetVertexTexturePos(pVtx, 
			pBlock->nPatternAnimX,
			pBlock->nPatternAnimY, 
			g_aBlockInfo[pBlock->type].nMaxPatternX, 
			g_aBlockInfo[pBlock->type].nMaxPatternY,
			pBlock->obj.bInversed);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBlock = &g_aBlock[0][0];
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		if (pBlock->bUsed == true && pBlock->obj.bVisible == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffBlock[pBlock->type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
BLOCK* GetBlock(void)
{
	return &g_aBlock[0][0];
}

//=====================================================================
// �u���b�N�̐ݒ菈��
//=====================================================================
BLOCK* SetBlock(BLOCK_TYPE type, int x, int y)
{
	BLOCK* pBlock = &g_aBlock[y][x];

	memset(pBlock, 0, sizeof(BLOCK));
	pBlock->bUsed = true;
	pBlock->obj.pos = D3DXVECTOR3(x * BLOCK_SIZE, y * BLOCK_SIZE, 0.0f);
	pBlock->obj.size = INIT_SIZE;
	pBlock->obj.color = g_aBlockInfo[type].color;
	pBlock->type = type;
	pBlock->obj.bVisible = true;
	pBlock->dwCollisionType = g_aBlockInfo[type].dwCollisionType;
	pBlock->Update = g_aBlockInfo[type].Update;
	pBlock->startPos = pBlock->obj.pos;

	if (type == BLOCK_TYPE_AIR)
	{
		pBlock->dwCollisionType = BLOCK_HIT_NONE;
		pBlock->obj.bVisible = false;
	}

	return pBlock;
}

//=====================================================================
// �u���b�N�̏Փ˔��菈��
//=====================================================================
DWORD CollisionBlock(
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3 size,
	BLOCK** dpBlock
){
	BLOCK* pBlock = &g_aBlock[0][0];
	DWORD dwHit = BLOCK_HIT_NONE;

	if (dpBlock != NULL)
	{
		*dpBlock = NULL;
	}

	// �㉺�̏Փ˔���
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		if (pBlock->bUsed == false) continue;

		if (pBlock->dwCollisionType & BLOCK_HIT_TOP
			&& pPosOld->y <= pBlock->posOld.y
			&& pPos->y > pBlock->obj.pos.y
			&& pPosOld->x + size.x / 2 > pBlock->obj.pos.x
			&& pPosOld->x - size.x / 2 < pBlock->obj.pos.x + pBlock->obj.size.x
			)
		{// �ォ��̏Փ˔���
			dwHit |= BLOCK_HIT_TOP;
			pPos->y = pBlock->obj.pos.y;
			pMove->y = 0;

			if (dpBlock != NULL)
			{
				*dpBlock = pBlock;
			}
		}
		if (pBlock->dwCollisionType & BLOCK_HIT_BOTTOM
			&& pPosOld->y - size.y >= pBlock->posOld.y + pBlock->obj.size.y
			&& pPos->y - size.y < pBlock->obj.pos.y + pBlock->obj.size.y
			&& pPosOld->x + size.x / 2 > pBlock->obj.pos.x
			&& pPosOld->x - size.x / 2 < pBlock->obj.pos.x + pBlock->obj.size.x
			)
		{// ������̏Փ˔���
			dwHit |= BLOCK_HIT_BOTTOM;
			pPos->y = pBlock->obj.pos.y + pBlock->obj.size.y + size.y;
			pMove->y = 0;
		}
	}

	// ���E�̏Փ˔���
	pBlock = &g_aBlock[0][0];
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		if (pBlock->bUsed == false) continue;

		if (
			pBlock->dwCollisionType & BLOCK_HIT_LEFT
			&& pPosOld->x + size.x / 2 <= pBlock->posOld.x
			&& pPos->x + size.x / 2 > pBlock->obj.pos.x
			&& pPos->y > pBlock->obj.pos.y
			&& pPos->y - size.y < pBlock->obj.pos.y + pBlock->obj.size.y
			)
		{// ������̏Փ˔���
			dwHit |= BLOCK_HIT_LEFT;
			pPos->x = pBlock->obj.pos.x - size.x / 2;
		}
		if (pBlock->dwCollisionType & BLOCK_HIT_RIGHT
			&& pPosOld->x - size.x / 2 >= pBlock->posOld.x + pBlock->obj.size.x
			&& pPos->x - size.x / 2 < pBlock->obj.pos.x + pBlock->obj.size.x
			&& pPos->y > pBlock->obj.pos.y
			&& pPos->y - size.y < pBlock->obj.pos.y + pBlock->obj.size.y
			)
		{// �E����̏Փ˔���
			dwHit |= BLOCK_HIT_RIGHT;
			pPos->x = pBlock->obj.pos.x + pBlock->obj.size.x + size.x / 2;
		}
	}


	return dwHit;
}

BLOCK_INFO* GetBlockInfo(void)
{
	return &g_aBlockInfo[0];
}