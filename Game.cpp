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
#include "background.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define MAX_LEVEL	(10)
#define INIT_TIMER	(60 * 120)

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
DWORD g_dwStart = timeGetTime();
DWORD g_dwLast = timeGetTime();
int g_nTimer = INIT_TIMER;
int g_nScore;

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
	InitBackground();

	g_pFontInfo = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
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

	if (GetPreviousScene() == SCENE_TITLE)
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME00);
	}

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
	UninitBackground();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateGame(void)
{
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
		UpdateBackground();

		// ���݂̃Q�[����ԕʂ̏���
		g_nCounterGameState++;
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:	// �ʏ�
			if (GetFade() == FADE_NONE && GetPlayer()->state != PLAYERSTATE_END)
			{
				g_nTimer--;
			}

			sprintf(&g_pFontInfo->aText[0], "Level %02d | Time %03d | Score | %06d", g_nCurrentStage + 1, (g_nTimer) / 10, g_nScore);

			if (g_nTimer <= 0)
			{
				KillPlayer();
				SetFade(SCENE_RESULT);
			}

			break;

		case GAMESTATE_CLEAR:	// �N���A
			// �v���C���[���I����ԂɈڍs
			SetPlayerState(PLAYERSTATE_END);

			sprintf(&g_pFontInfo->aText[0], "Level %02d | Time %03d | Score | %06d", g_nCurrentStage + 1, (g_nTimer) / 10, g_nScore);

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
				SetFade(SCENE_RESULT);
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
	DrawBackground();
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
// �Q�[�����̏���������
//=====================================================================
void ResetGame(void)
{
	SetStage(0);
	g_nTimer = INIT_TIMER;
	g_nScore = 0;
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

void AddScore(int nScore)
{
	g_nScore += nScore;
}

int GetScore(void)
{
	return g_nScore;
}

int GetGameTimeLeft(void)
{
	return g_nTimer;
}
