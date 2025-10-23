//=====================================================================
//
// �Q�[����ʏ��� [Game.h]
// Author : Kaito Iwasaki
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
#include "enemy.h"
#include "item.h"
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "pause.h"
#include "fuelbar.h"
#include "font.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define MAX_LEVEL	(10)

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
MAPINFO g_map[NUM_BLOCK_Y][NUM_BLOCK_X] = {};
int g_nCurrentStage = 0;
GAMESTATE g_gameState = GAMESTATE_NORMAL;
int g_nCounterGameState = 0;
bool g_bIsPause = false;
FONT* g_pFontInfo = NULL;
DWORD g_dwTimer = timeGetTime();
DWORD g_dwStart = timeGetTime();

//=====================================================================
// ����������
//=====================================================================
void InitGame(void)
{
	char aStageFileName[MAX_PATH] = {};

	// �e�I�u�W�F�N�g�̏���������
	InitDecal();
	InitFont();
	InitBlock();
	InitPlayer();
	InitEnemy();
	InitItem();
	InitEffect();
	InitParticle();
	InitPause();
	InitFuelBar();

	// �w�i�摜�̐ݒu
	SetDecal(
		DECAL_LABEL_BG000,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	);

	g_pFontInfo = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		30,
		"",
		DT_CENTER
	);

	// �l�̏�����
	g_nCounterGameState = 0;
	g_bIsPause = false;
	g_dwStart = timeGetTime();
	SetGameState(GAMESTATE_NORMAL);
	sprintf(&g_pFontInfo->aText[0], "Level %02d", g_nCurrentStage  + 1);

	// ���݂̃X�e�[�W���̃t�@�C�������擾
	GetStageName(g_nCurrentStage, &aStageFileName[0]);

	// �X�e�[�W���̃��[�h
	memset(&g_map[0][0], 0, sizeof(g_map));
	LoadBin(&aStageFileName[0], &g_map[0][0], sizeof(MAPINFO), MAX_BLOCK);

	// �X�e�[�W���Ɋ�Â��ău���b�N��z�u
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			SetBlock(g_map[y][x].type, x, y);
		}
	}
}

//=====================================================================
// �I������
//=====================================================================
void UninitGame(void)
{
	// �e�I�u�W�F�N�g�̏I������
	UninitDecal();
	UninitFont();
	UninitBlock();
	UninitPlayer();
	UninitEnemy();
	UninitItem();
	UninitParticle();
	UninitEffect();
	UninitPause();
	UninitFuelBar();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateGame(void)
{
	g_dwTimer = timeGetTime();

	if (INPUT_TRIGGER_PAUSE)
	{// �|�[�Y���j���[
		TogglePause(!g_bIsPause);
	}

	if (g_bIsPause)
	{// �|�[�Y����
		UpdatePause();
	}
	else
	{// �Q�[������
		// �e�I�u�W�F�N�g�̍X�V����
		UpdateBlock();
		UpdatePlayer();
		UpdateEnemy();
		UpdateItem();
		UpdateParticle();
		UpdateEffect();
		UpdateFuelBar();

		// ���݂̃Q�[����ԕʂ̏���
		g_nCounterGameState++;
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:	// �ʏ�
			sprintf(&g_pFontInfo->aText[0], "Level %02d\nTime %.2f", g_nCurrentStage + 1, (float)(g_dwTimer - g_dwStart) / 1000.0f);
			break;

		case GAMESTATE_CLEAR:	// �N���A
			// �v���C���[���I����ԂɈڍs
			SetPlayerState(PLAYERSTATE_END);

			if (g_nCounterGameState > 60)
			{
				// ���̃X�e�[�W�ɐݒ�
				SetStage(g_nCurrentStage + 1);

				// �Q�[�����I����ԂɈڍs
				SetGameState(GAMESTATE_END);
			}

			break;

		case GAMESTATE_END:		// �I��
			// ��ʑJ�ڏ���
			if (g_nCurrentStage == MAX_LEVEL)
			{// ���̃��x�����Ȃ���΃��U���g��ʂ֑J��
				SetFade(SCENE_RESULT, false);
			}
			else
			{
				// �Q�[����ʂ֑J�ځi�X�e�[�W���̓Q�[���̏������ŐV�����ǂݍ��ށj
				SetFade(SCENE_GAME, false);
			}

			break;
		}
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1) && GetFade() == FADE_NONE)
	{// �f�o�b�O�p�N���A�{�^��
		SetGameState(GAMESTATE_CLEAR);
	}
	if (GetKeyboardTrigger(DIK_F5))
	{// �G�f�B�^���[�h�J��
		SetScene(SCENE_EDITOR);
	}
#endif
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawGame(void)
{
	// �e�I�u�W�F�N�g�̕`�揈��
	DrawDecal();
	DrawBlock();
	DrawItem();
	DrawEnemy();
	DrawEffect();
	DrawPlayer();
	DrawFuelBar();
	DrawFont();

	if (g_bIsPause)
	{// �|�[�Y���j���[�`��
		DrawPause();
	}
}

//=====================================================================
// �Q�[����Ԃ̐ݒ菈��
//=====================================================================
void SetGameState(GAMESTATE newState)
{
	g_gameState = newState;
	g_nCounterGameState = 0;
}

//=====================================================================
// �Q�[����Ԃ̎擾����
//=====================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//=====================================================================
// �|�[�Y��Ԃ̐؂�ւ�����
//=====================================================================
void TogglePause(bool bIsPause)
{
	if (GetFade() != FADE_NONE) return;

	SetPauseMenuCursor(0);
	g_bIsPause = bIsPause;

	if (bIsPause)
	{
		PausePlayer();
	}
	else
	{
		UnPausePlayer();
	}
}

//=====================================================================
// �}�b�v�̐ݒ菈��
//=====================================================================
void SetMap(MAPINFO* map)
{
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++, map++)
		{
			g_map[y][x].type = map->type;
		}
	}
}

//=====================================================================
// ���݂̃}�b�v���̎擾����
//=====================================================================
MAPINFO* GetMap(void)
{
	return &g_map[0][0];
}

//=====================================================================
// �X�e�[�W�t�@�C�����̎擾����
//=====================================================================
void GetStageName(int nStage, char* pBuffer)
{
	sprintf(pBuffer, "data\\MAP\\level%02d.bin", nStage);
}

//=====================================================================
// �X�e�[�W�ԍ��̐ݒ菈��
//=====================================================================
void SetStage(int nStage)
{
	g_nCurrentStage = nStage;
}

//=====================================================================
// �X�e�[�W�ԍ��̎擾����
//=====================================================================
int GetStage(void)
{
	return g_nCurrentStage;
}