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
DECAL* g_pCursor = NULL;
int CurrentBlock = 0;

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

	g_pCursor = SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f)
	);

	for (int i = 0; i < NUM_BLOCK_X; i++)
	{
		SetBlock(
			BLOCK_TYPE_000,
			i, NUM_BLOCK_Y - 1
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
	UpdateBlock();
	UpdatePlayer();

	D3DXVECTOR2 posMouse = GetMousePos();
	D3DXVECTOR3 posBlock = D3DXVECTOR3(
		floorf(posMouse.x / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE / 2,
		floorf(posMouse.y / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE / 2,
		0.0f
	);
	g_pCursor->obj.pos = posBlock;

	if (GetMouse().lZ >= 120)
	{
		CurrentBlock++;
	}
	else if (GetMouse().lZ <= -120)
	{
		CurrentBlock--;
	}
	if (CurrentBlock == -1)
	{
		CurrentBlock = 3 - 1;
	}
	else
	{
		CurrentBlock %= 3;
	}

	if (GetMousePress(MOUSE_LEFT))
	{
		PlaySound(SOUND_LABEL_SE_CURSOR);
		SetBlock(
			(BLOCK_TYPE)(CurrentBlock + 1),
			(int)posMouse.x / BLOCK_SIZE,
			(int)posMouse.y / BLOCK_SIZE
		);
	}
	else if (GetMousePress(MOUSE_RIGHT))
	{
		PlaySound(SOUND_LABEL_SE_CURSOR);
		SetBlock(
			BLOCK_TYPE_AIR,
			(int)posMouse.x / BLOCK_SIZE,
			(int)posMouse.y / BLOCK_SIZE
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