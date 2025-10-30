//=====================================================================
//
// �G�f�B�^�p�u���b�N [editorblock.cpp]
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
#include "block.h"
#include "editorblock.h"

#include "player.h"
#include "baseScene.h"
#include "Game.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_POS						D3DXVECTOR3_ZERO
#define INIT_SIZE						D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE, 0.0f)
#define INIT_COLOR						D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEditorBlock = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEditorBlock[BLOCK_TYPE_MAX] = {};
EDITORBLOCK g_aEditorBlock[NUM_BLOCK_Y][NUM_BLOCK_X] = {};

const char* g_aEditorBlockFileName[BLOCK_TYPE_MAX] = {
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
	"data\\TEXTURE\\flag000.png",
	"data\\TEXTURE\\player.png",
	"data\\TEXTURE\\wood000.png",
	"data\\TEXTURE\\wood001.png",
	"data\\TEXTURE\\wood002.png",
	"data\\TEXTURE\\wood003.png",
};

//=====================================================================
// ����������
//=====================================================================
void InitEditorBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X
	EDITORBLOCK* pEditorBlock = &g_aEditorBlock[0][0];
	MAPINFO* pMap = GetMap();

	// �\���̂̏�����
	memset(pEditorBlock, 0, sizeof(EDITORBLOCK) * MAX_BLOCK);
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++, pEditorBlock++, pMap++)
		{
			pEditorBlock->obj.pos = D3DXVECTOR3(x * BLOCK_SIZE, y * BLOCK_SIZE, 0.0f);
			pEditorBlock->obj.size = INIT_SIZE;
			pEditorBlock->obj.color = GetBlockInfo()[pMap->type].color;
			pEditorBlock->obj.bVisible = true;
			pEditorBlock->type = pMap->type;

			for (int i = 0; i < MAX_BLOCK_PARAM; i++)
			{
				pEditorBlock->nParam[i] = pMap->nParam[i];
			}
		}
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCount = 0; nCount < BLOCK_TYPE_MAX; nCount++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_aEditorBlockFileName[nCount],
			&g_pTexBuffEditorBlock[nCount]
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEditorBlock,
		NULL
	);
}

//=====================================================================
// �I������
//=====================================================================
void UninitEditorBlock(void)
{
	if (g_pVtxBuffEditorBlock != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffEditorBlock->Release();
		g_pVtxBuffEditorBlock = NULL;
	}

	for (int nCount = 0; nCount < BLOCK_TYPE_MAX; nCount++)
	{
		if (g_pTexBuffEditorBlock[nCount] != NULL)
		{// �e�N�X�`���o�b�t�@�̔j��
			g_pTexBuffEditorBlock[nCount]->Release();
			g_pTexBuffEditorBlock[nCount] = NULL;
		}
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateEditorBlock(void)
{

}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawEditorBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	EDITORBLOCK* pEditorBlock = &g_aEditorBlock[0][0];

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffEditorBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pEditorBlock++, pVtx += 4)
	{
		// ���_����ݒ�
		SetVertexPos(pVtx,
			D3DXVECTOR3(pEditorBlock->obj.pos.x, pEditorBlock->obj.pos.y, 0.0f),
			D3DXVECTOR3(pEditorBlock->obj.pos.x + pEditorBlock->obj.size.x, pEditorBlock->obj.pos.y, 0.0f),
			D3DXVECTOR3(pEditorBlock->obj.pos.x, pEditorBlock->obj.pos.y + pEditorBlock->obj.size.y, 0.0f),
			D3DXVECTOR3(pEditorBlock->obj.pos.x + pEditorBlock->obj.size.x, pEditorBlock->obj.pos.y + pEditorBlock->obj.size.y, 0.0f)
			);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pEditorBlock->obj.color);
		SetVertexTexturePos(pVtx,
			D3DXVECTOR2(0.01f, 0.01f),
			D3DXVECTOR2(1.0f, 0.01f),
			D3DXVECTOR2(0.01f, 1.0f),
			D3DXVECTOR2(1.0f, 1.0f)
		);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEditorBlock->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEditorBlock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pEditorBlock = &g_aEditorBlock[0][0];
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pEditorBlock++)
	{
		if (pEditorBlock->obj.bVisible == true)
		{// �|���S���`��
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexBuffEditorBlock[pEditorBlock->type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// �I�u�W�F�N�g�擾����
//=====================================================================
EDITORBLOCK* GetEditorBlock(void)
{
	return &g_aEditorBlock[0][0];
}

//=====================================================================
// �u���b�N�̐ݒ菈��
//=====================================================================
EDITORBLOCK* SetEditorBlock(BLOCK_TYPE type, int x, int y)
{
	EDITORBLOCK* pEditorBlock = &g_aEditorBlock[y][x];

	pEditorBlock->type = type;
	pEditorBlock->obj.color = GetBlockInfo()[type].color;
	for (int i = 0; i < MAX_BLOCK_PARAM; i++)
	{
		pEditorBlock->nParam[i] = 0;
	}

	return pEditorBlock;
}

bool SaveEditorBlock(const char* pFileName)
{
	MAPINFO MapInfo[NUM_BLOCK_Y][NUM_BLOCK_X];
	memset(&MapInfo[0][0], 0, sizeof(MAPINFO) * MAX_BLOCK);

	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			MapInfo[y][x].type = g_aEditorBlock[y][x].type;

			for (int i = 0; i < MAX_BLOCK_PARAM; i++)
			{
				MapInfo[y][x].nParam[i] = g_aEditorBlock[y][x].nParam[i];
			}
		}
	}

	return SaveBin(pFileName, &MapInfo[0][0], sizeof(MAPINFO), MAX_BLOCK);
}
