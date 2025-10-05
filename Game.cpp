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

	for (int i = 0; i < SCREEN_WIDTH / 30 + 1; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			D3DXVECTOR3(i * 30, 100, 0)
		);
	}

	for (int i = 0; i < SCREEN_WIDTH / 30 + 1; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			D3DXVECTOR3(i * 30, SCREEN_HEIGHT, 0)
		);
	}
	for (int i = 0; i < SCREEN_HEIGHT / 30 + 1; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			D3DXVECTOR3(0 - 30, SCREEN_HEIGHT - 48 * (i + 1), 0)
		);
	}
	for (int i = 0; i < SCREEN_HEIGHT / 30 + 1; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 30 * (i + 1), 0)
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

	if (GetMouseTrigger(MOUSE_LEFT))
	{
		PlaySound(SOUND_LABEL_SE_CURSOR);
		SetBlock(
			BLOCK_TYPE_000,
			Vector2To3(GetMousePos())
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