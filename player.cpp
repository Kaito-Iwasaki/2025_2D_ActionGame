//=====================================================================
// 
// �v���C���[���� [player.cpp]
// Author : 
//
//=====================================================================
#include "player.h"
#include "input.h"
#include "util.h"
#include "Game.h"
#include "block.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"
#define TEXTURE_MAX_X			(4)
#define TEXTURE_MAX_Y			(2)
#define INIT_POS				D3DXVECTOR3(SCREEN_CENTER, SCREEN_HEIGHT - 100, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(64.0f, 64.0f, 64.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_player;

//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X

	// �\���̂̏�����
	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.pos = INIT_POS;
	g_player.obj.size = INIT_SIZE;
	g_player.obj.color = INIT_COLOR;
	g_player.obj.bVisible = true;

	// �e�N�X�`���̓ǂݍ���
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffPlayer
		);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitPlayer(void)
{
	if (g_pVtxBuffPlayer != NULL)
	{// ���_�o�b�t�@�̔j��
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	if (g_pTexBuffPlayer != NULL)
	{// �e�N�X�`���o�b�t�@�̔j��
		g_pTexBuffPlayer->Release();
		g_pTexBuffPlayer = NULL;
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdatePlayer(void)
{
	g_player.posOld = g_player.obj.pos;

	if (GetKeyboardPress(DIK_A))
	{
		g_player.move.x -= 1;
		g_player.nPatternAnimY = 1;
	}
	else if (GetKeyboardPress(DIK_D))
	{
		g_player.move.x += 1;
		g_player.nPatternAnimY = 0;
	}

	if (GetKeyboardPress(DIK_SPACE))
	{
		if (g_player.bIsJumping == false)
		{
			g_player.bIsJumping = true;
			g_player.move.y = - 20.0f;
		}
	}


	g_player.obj.pos += g_player.move;

	if (abs((long)g_player.move.x) > 0.01f && Magnitude(g_player.obj.pos, g_player.posOld) > 0.1f)
	{
		if (g_player.nCounterAnim % 10 == 0)
		{
			g_player.nPatternAnimX = (g_player.nPatternAnimX + 1) % 4;
		}
		g_player.nCounterAnim++;
	}
	else
	{
		g_player.nPatternAnimX = 0;
	}

	g_player.move.x += (0 - g_player.move.x) * 0.5f;
	g_player.move.y += GAME_GRAVITY;

	if (CollisionBlock(&g_player.obj.pos, &g_player.posOld, &g_player.move, g_player.obj.size))
	{
		g_player.bIsJumping = false;
	}
	else
	{
		g_player.bIsJumping = true;

	}
}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	SetVertexPos(pVtx, 
		D3DXVECTOR3(g_player.obj.pos.x - g_player.obj.size.x / 2.0f, g_player.obj.pos.y - g_player.obj.size.y, 0.0f),
		D3DXVECTOR3(g_player.obj.pos.x + g_player.obj.size.x / 2.0f, g_player.obj.pos.y - g_player.obj.size.y, 0.0f),
		D3DXVECTOR3(g_player.obj.pos.x - g_player.obj.size.x / 2.0f, g_player.obj.pos.y, 0.0f),
		D3DXVECTOR3(g_player.obj.pos.x + g_player.obj.size.x / 2.0f, g_player.obj.pos.y, 0.0f)
	);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_player.obj.color);
	SetVertexTexturePos(pVtx, g_player.nPatternAnimX, g_player.nPatternAnimY, 4, 2, g_player.obj.bInversed);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.obj.bVisible == true)
	{// �|���S���`��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexBuffPlayer);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}