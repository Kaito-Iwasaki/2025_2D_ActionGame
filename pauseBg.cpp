//=====================================================================
// 
// pauseBg [PauseBg.cpp]
// Author:
//
//=====================================================================
#include "pauseBg.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************


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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseBg = NULL;
BASEOBJECT g_pauseBg;

//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitPauseBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	memset(&g_pauseBg, 0, sizeof(BASEOBJECT));
	g_pauseBg.pos = D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0);
	g_pauseBg.size = D3DXVECTOR3(700, 500, 0);
	g_pauseBg.color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	g_pauseBg.bVisible = true;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseBg,
		NULL
	);
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitPauseBg(void)
{
	if (g_pVtxBuffPauseBg != NULL)
	{
		g_pVtxBuffPauseBg->Release();
		g_pVtxBuffPauseBg = NULL;
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdatePauseBg(void)
{

}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawPauseBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffPauseBg->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	SetVertexPos(pVtx, g_pauseBg);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_pauseBg.color);
	SetVertexTexturePos(pVtx);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPauseBg->Unlock();

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPauseBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

BASEOBJECT* GetPauseBg(void)
{
	return &g_pauseBg;
}