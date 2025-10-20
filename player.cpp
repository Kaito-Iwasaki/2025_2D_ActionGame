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
#include "fade.h"
#include "particle.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"
#define TEXTURE_MAX_X			(4)
#define TEXTURE_MAX_Y			(2)
#define INIT_POS				D3DXVECTOR3(100, 640, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(75.0f, 75.0f, 0.0f) * 0.7f
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_HITBOX				D3DXVECTOR3(45.0f, 70.0f, 0.0f) * 0.7f
#define INIT_PLAYER_SPEED		(4.0f)
#define INIT_PLAYER_LIFE		(3)
#define INIT_PLAYER_JUMPPOWER	(16.0f)
#define INIT_PLAYER_CHARGE		(1.0f)

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
	g_player.hitBoxSize = INIT_HITBOX;
	g_player.state = PLAYERSTATE_INIT;
	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.nLife = INIT_PLAYER_LIFE;
	g_player.fJumpPower = INIT_PLAYER_JUMPPOWER;
	g_player.fCharge = INIT_PLAYER_CHARGE;
	g_player.bIsFlying = false;

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
	g_player.nCounterState++;
	switch (g_player.state)
	{
	case PLAYERSTATE_INIT:
		g_player.obj.pos = INIT_POS;
		g_player.obj.bVisible = true;
		g_player.move = D3DXVECTOR3_ZERO;
		g_player.fSpeed = INIT_PLAYER_SPEED;
		g_player.nLife = INIT_PLAYER_LIFE;
		g_player.fCharge = INIT_PLAYER_CHARGE;
		SetPlayerState(PLAYERSTATE_APPEAR);

	case PLAYERSTATE_APPEAR:
		g_player.obj.bVisible ^= 1;
		if (g_player.nCounterState > 120)
		{
			g_player.obj.bVisible = true;
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DAMAGED:
		g_player.obj.bVisible ^= 1;
		if (g_player.nCounterState > 120)
		{
			g_player.obj.bVisible ^= 1;
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_DIED:
		g_player.obj.bVisible = false;
		//if (g_player.nCounterState > 30)
		//{
		//	SetPlayerState(PLAYERSTATE_INIT);
		//}
		SetGameState(GAMESTATE_END);
		return;

	case PLAYERSTATE_END:
		return;
	}

	// �O��̈ʒu���L�^
	g_player.posOld = g_player.obj.pos;

	// �v���C���[�̑��쏈��
	g_player.move.x = 0;
	if (INPUT_PRESS_LEFT)
	{// ��
		g_player.move.x -= g_player.fSpeed;
	}
	if (INPUT_PRESS_RIGHT)
	{// �E
		g_player.move.x += g_player.fSpeed;
	}

	if (GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_A))
	{// �W�����v
		if (g_player.bIsJumping == false)
		{// �ڒn���ɃX�y�[�X�L�[�������ꂽ
			g_player.bIsJumping = true;
			g_player.move.y = -g_player.fJumpPower;
		}
		else
		{// �󒆂ŃX�y�[�X�L�[�������ꂽ
			if (g_player.fCharge > 0)
			{
				g_player.bIsFlying = true;
			}
		}
	}
	else if (GetKeyboardRelease(DIK_SPACE))
	{// �󒆂ŃX�y�[�X�L�[�������ꂽ
		g_player.bIsFlying = false;
	}

	// �󒆂ł̋���
	if (g_player.bIsFlying)
	{
		g_player.move.y = 0;
		g_player.fCharge -= PLAYER_CHARGE_DECAY;

		EFFECTINFO infoRed;
		infoRed.col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
		infoRed.fMaxAlpha = 0.5f;
		infoRed.fMaxScale = 0.6f;
		infoRed.fRotSpeed = 0.5f;
		infoRed.fSpeed = 4.0f;
		infoRed.nMaxLife = 30;

		EFFECTINFO infoWhite = infoRed;
		infoWhite.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		infoWhite.fMaxScale = 0.5f;

		SetParticle(
			infoRed,
			g_player.obj.pos + D3DXVECTOR3(0, -2, 0),
			0.0f,
			0.6f,
			1,
			1
		);
		SetParticle(
			infoWhite,
			g_player.obj.pos + D3DXVECTOR3(0, -2, 0),
			0.0f,
			0.6f,
			1,
			1
		);

		if (g_player.fCharge <= 0.0f)
		{
			g_player.bIsFlying = false;
		}
	}

	if (g_player.pBlock != NULL)
	{// �u���b�N�ɏ���Ă���Ƃ��Ƀu���b�N�̈ړ��ʂ𔽉f
		g_player.obj.pos += (g_player.pBlock->obj.pos - g_player.pBlock->posOld);
	}

	// �ʒu���X�V
	if (g_player.bIsFlying == false)
	{
		g_player.move.y += GAME_GRAVITY;	// �d�͂����Z
	}
	g_player.obj.pos += g_player.move;

	// �u���b�N�Ƃ̏Փ˔���
	DWORD dwHit = CollisionBlock(&g_player.obj.pos, &g_player.posOld, &g_player.move, g_player.hitBoxSize, &g_player.pBlock);

	if (dwHit & BLOCK_HIT_TOP)
	{// �n�ʂɐڂ��Ă���
		g_player.bIsJumping = false;
		g_player.fCharge = INIT_PLAYER_CHARGE;
	}
	else
	{// �󒆂ɂ���
		g_player.bIsJumping = true;
	}

	if (g_player.nCounterAnim % 5 == 0)
	{
		g_player.nPatternAnimX = (g_player.nPatternAnimX + 1) % 4;
	}
	g_player.nCounterAnim++;

	// �A�j���[�V�����̍X�V
	if (g_player.bIsJumping)
	{// �W�����v
		g_player.nPatternAnimX = 0;
		g_player.nPatternAnimY = 1;
	}
	else if (abs((long)g_player.move.x) > 0.01f)
	{// �ړ�
		g_player.nPatternAnimY = 1;
	}
	else
	{// ��~
		g_player.nPatternAnimY = 0;
	}

	// �A�j���[�V�����̌�����ݒ�
	if (g_player.move.x < 0)
	{// ��
		g_player.obj.bInversed = true;
	}
	else if (g_player.move.x > 0)
	{// �E
		g_player.obj.bInversed = false;
	}

	if (IsObjectOutOfScreen(g_player.obj))
	{// ��ʊO�ɏo���玀�S
		SetPlayerState(PLAYERSTATE_DIED);
	}
}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;
	VERTEX_2D* pVtx;

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

PLAYER* GetPlayer(void)
{
	return &g_player;
}

void HitPlayer(void)
{
	if (g_player.state == PLAYERSTATE_APPEAR)	return;
	if (g_player.state == PLAYERSTATE_DAMAGED)	return;
	if (g_player.state == PLAYERSTATE_DIED)		return;

	g_player.nLife--;

	if (g_player.nLife < 1)
	{
		SetPlayerState(PLAYERSTATE_DIED);
	}
	else
	{
		SetPlayerState(PLAYERSTATE_DAMAGED);
	}
}

void SetPlayerState(PLAYERSTATE state)
{
	g_player.state = state;
	g_player.nCounterState = 0;
}
