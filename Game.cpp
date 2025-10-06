//=====================================================================
//
// Game [Game.h]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "Game.h"
#include "player.h"
#include "block.h"
#include "decal.h"
#include "input.h"
#include "sound.h"
#include "util.h"

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
BLOCK* g_pBlockPreview = NULL;

//=====================================================================
// ����������
//=====================================================================
void InitGame(void)
{
	InitPlayer();
	InitBlock();
	InitDecal();

	SetDecal(
		DECAL_LABEL_BG000,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

	g_pBlockPreview = SetBlock(
		BLOCK_TYPE_000,
		D3DXVECTOR3_ZERO
	);
	g_pBlockPreview->obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
	g_pBlockPreview->bCollidable = false;

	SetBlock(
		BLOCK_TYPE_000,
		D3DXVECTOR3(SCREEN_CENTER + 200, 600, 0)
	);

	SetBlock(
		BLOCK_TYPE_000,
		D3DXVECTOR3(SCREEN_CENTER + 100, 500, 0)
	);

	SetBlock(
		BLOCK_TYPE_000,
		D3DXVECTOR3(SCREEN_CENTER, 400, 0)
	);

	SetBlock(
		BLOCK_TYPE_000,
		D3DXVECTOR3(SCREEN_CENTER - 100, 500, 0)
	);

	SetBlock(
		BLOCK_TYPE_000,
		D3DXVECTOR3(SCREEN_CENTER - 200, 600, 0)
	);

	for (int i = 0; i < SCREEN_WIDTH / BLOCK_SIZE + 1; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			D3DXVECTOR3(i * BLOCK_SIZE, 100, 0)
		);
	}

	for (int i = 0; i < SCREEN_WIDTH / BLOCK_SIZE + 1; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			D3DXVECTOR3(i * BLOCK_SIZE, SCREEN_HEIGHT, 0)
		);
	}
	for (int i = 0; i < SCREEN_HEIGHT / BLOCK_SIZE + 1; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			D3DXVECTOR3(0 - BLOCK_SIZE, SCREEN_HEIGHT - BLOCK_SIZE * (i + 1), 0)
		);
	}
	for (int i = 0; i < SCREEN_HEIGHT / BLOCK_SIZE + 1; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - BLOCK_SIZE * (i + 1), 0)
		);
	}
}

//=====================================================================
// �I������
//=====================================================================
void UninitGame(void)
{
	UninitPlayer();
	UninitBlock();
	UninitDecal();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateGame(void)
{
	UpdatePlayer();
	UpdateBlock();

	D3DXVECTOR2 posMouse = GetMousePos();
	D3DXVECTOR3 posBlock = D3DXVECTOR3(
		floorf((float)posMouse.x / BLOCK_SIZE) * BLOCK_SIZE,
		floorf((float)posMouse.y / BLOCK_SIZE) * BLOCK_SIZE,
		0.0f
	);
	g_pBlockPreview->obj.pos = posBlock;

	if (GetMouseTrigger(MOUSE_LEFT))
	{
		PlaySound(SOUND_LABEL_SE_CURSOR);
		SetBlock(
			BLOCK_TYPE_000,
			posBlock
		);
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawGame(void)
{
	DrawDecal();
	DrawBlock();
	DrawPlayer();
}