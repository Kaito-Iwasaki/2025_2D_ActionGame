//=====================================================================
// 
// �v���C���[���� [enemy.cpp]
// Author : 
//
//=====================================================================
#include "item.h"
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
#define TEXTURE_FILENAME		"data\\TEXTURE\\item000.png"
#define TEXTURE_MAX_X			(1)
#define TEXTURE_MAX_Y			(1)
#define INIT_POS				D3DXVECTOR3(SCREEN_CENTER, SCREEN_HEIGHT - 100, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(30.0f, 30.0f, 0.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)
#define INIT_HITBOX				INIT_SIZE

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffItem = NULL;
ITEM g_aItem[MAX_ITEM];

//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	ITEM* pItem = &g_aItem[0];

	// �\���̂̏�����
	memset(pItem, 0, sizeof(ITEM) * MAX_ITEM);
	for (int i = 0; i < MAX_ITEM; i++, pItem++)
	{
		pItem->obj.pos = INIT_POS;
		pItem->obj.size = INIT_SIZE;
		pItem->obj.color = INIT_COLOR;
		pItem->obj.bVisible = false;
		pItem->bUsed = false;
		pItem->hitBoxSize = INIT_HITBOX;
	}

	// �e�N�X�`���̓ǂݍ���
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffItem
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitItem(void)
{
	if (g_pVtxBuffItem != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}

	if (g_pTexBuffItem != NULL)
	{// �e�N�X�`���o�b�t�@�̔j��
		g_pTexBuffItem->Release();
		g_pTexBuffItem = NULL;
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdateItem(void)
{
	ITEM* pItem = &g_aItem[0];
	PLAYER* pPlayer = GetPlayer();

	for (int i = 0; i < MAX_ITEM; i++, pItem++)
	{
		if (pItem->bUsed == false) continue;

		if (pItem->nCounterAnim % 10 == 0)
		{
			pItem->nPatternAnimX = (pItem->nPatternAnimX + 1) % 4;
		}
		pItem->nCounterAnim++;

		if (BoxCollision(
			pItem->obj.pos - D3DXVECTOR3(0, pItem->obj.size.y, 0), pItem->hitBoxSize,
			pPlayer->obj.pos - D3DXVECTOR3(0, pPlayer->obj.size.y, 0), pPlayer->hitBoxSize
		))
		{// �v���C���[�Ƃ̏Փ˔���
			pItem->bUsed = false;
		}
	}

}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;
	VERTEX_2D* pVtx;
	ITEM* pItem = &g_aItem[0];

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++, pItem++, pVtx += 4)
	{// ���_����ݒ�
		SetVertexPos(pVtx,
			D3DXVECTOR3(pItem->obj.pos.x - pItem->obj.size.x / 2.0f, pItem->obj.pos.y - pItem->obj.size.y, 0.0f),
			D3DXVECTOR3(pItem->obj.pos.x + pItem->obj.size.x / 2.0f, pItem->obj.pos.y - pItem->obj.size.y, 0.0f),
			D3DXVECTOR3(pItem->obj.pos.x - pItem->obj.size.x / 2.0f, pItem->obj.pos.y, 0.0f),
			D3DXVECTOR3(pItem->obj.pos.x + pItem->obj.size.x / 2.0f, pItem->obj.pos.y, 0.0f)
		);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pItem->obj.color);
		SetVertexTexturePos(pVtx, pItem->nPatternAnimX, pItem->nPatternAnimY, TEXTURE_MAX_X, TEXTURE_MAX_Y, pItem->obj.bInversed);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pItem = &g_aItem[0];
	for (int i = 0; i < MAX_ITEM; i++, pItem++)
	{
		if (pItem->obj.bVisible == true && pItem->bUsed == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffItem);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}

}

ITEM* SetItem(ITEM_TYPE type, D3DXVECTOR3 pos)
{
	ITEM* pItem = &g_aItem[0];
	for (int i = 0; i < MAX_ITEM; i++, pItem++)
	{
		if (pItem->bUsed == true) continue;

		pItem->bUsed = true;
		pItem->obj.pos = pos;
		pItem->obj.size = INIT_SIZE;
		pItem->obj.color = INIT_COLOR;
		pItem->obj.bVisible = true;
		pItem->hitBoxSize = INIT_HITBOX;
		pItem->move = { 2, 0 ,0};

		return pItem;
	}

	return NULL;
}