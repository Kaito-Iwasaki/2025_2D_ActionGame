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
#include "pauseBg.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_TIMER			(60 * 180)
#define TIME_STATE_READY	(4)
#define TIMER_DANGER		(60 * 30)

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
GAMESTATE g_gameState = GAMESTATE_READY;
int g_nCurrentStage = 0;
int g_nCounterGameState = 0;
int g_nTimer = INIT_TIMER;
int g_nScore;
bool g_bIsPause = false;
FONT* g_pFontInfo = NULL;
FONT* g_pFontPauseMenuTitle = NULL;
FONT* g_pFontCountDown = NULL;

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
	InitPauseBg();

	g_pFontInfo = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		35,
		"",
		DT_CENTER
	);

	g_pFontPauseMenuTitle = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 130, 0),
		D3DXVECTOR3(SCREEN_WIDTH, 100, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		60,
		"- PAUSE -",
		DT_CENTER
	);
	g_pFontPauseMenuTitle->obj.bVisible = false;

	g_pFontCountDown = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3_ZERO,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		D3DXCOLOR(1.0f, 0.7f, 0.3f, 0.0f),
		200,
		"",
		DT_CENTER | DT_VCENTER
	);

	// �l�̏�����
	g_nCounterGameState = 0;
	g_bIsPause = false;
	sprintf(
		&g_pFontInfo->aText[0],
		"Level %02d/%02d | Time %03d | Score %06d",
		g_nCurrentStage + 1,
		MAX_LEVEL,
		(INIT_TIMER) / 10,
		g_nScore
	);
	GetPlayer()->bIsControlEnabled = false;

	if (g_nCurrentStage == 0)
	{
		g_nTimer = TIME_STATE_READY;
		SetGameState(GAMESTATE_READY);
	}
	else
	{
		SetGameState(GAMESTATE_NORMAL);
	}

	if (GetPreviousScene() == SCENE_TITLE)
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME00);
	}
	
	if (GetPreviousScene() == SCENE_EDITOR)
	{
		SetGameState(GAMESTATE_NORMAL);
		g_nTimer = INIT_TIMER;
		g_nScore = 0;
	}

	// ���݂̃X�e�[�W���̃t�@�C�������擾
	GetStageName(g_nCurrentStage, &aStageFileName[0]);

	// �X�e�[�W���̃��[�h
	memset(&g_map[0][0], 0, sizeof(g_map));
	LoadBin(&aStageFileName[0], &g_map[0][0], sizeof(MAPINFO), MAX_BLOCK);

	// �X�e�[�W���Ɋ�Â��ău���b�N��z�u
	BLOCK* pBlock;
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			pBlock = SetBlock(g_map[y][x].type, x, y);

			for (int i = 0; i < MAX_BLOCK_PARAM; i++)
			{
				pBlock->nParam[i] = g_map[y][x].nParam[i];
			}
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
	UninitPauseBg();
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
		case GAMESTATE_READY:
			GetPlayer()->bIsControlEnabled = false;

			if (g_nCounterGameState % 60 == 0)
			{
				g_pFontCountDown->obj.color.a = 1.0f;
				g_nTimer--;
				PlaySound(SOUND_LABEL_SE_CURSOR);
			}

			g_pFontCountDown->obj.color.a = Clampf(g_pFontCountDown->obj.color.a - 0.01f, 0.0f, 1.0f);

			if (g_nTimer < 1)
			{
				sprintf(&g_pFontCountDown->aText[0], "START");
				SetGameState(GAMESTATE_NORMAL);
				g_nTimer = INIT_TIMER;
				PlaySound(SOUND_LABEL_SE_COIN);
			}
			else
			{
				sprintf(&g_pFontCountDown->aText[0], "%d", g_nTimer);
			}

			break;

		case GAMESTATE_NORMAL:	// �ʏ�
			GetPlayer()->bIsControlEnabled = true;

			if (GetFade() == FADE_NONE && GetPlayer()->state != PLAYERSTATE_END)
			{
				g_nTimer--;
			}

			g_pFontCountDown->obj.color.a = Clampf(g_pFontCountDown->obj.color.a - 0.01f, 0.0f, 1.0f);

			sprintf(
				&g_pFontInfo->aText[0],
				"Level %02d/%02d | Time %03d | Score %06d",
				g_nCurrentStage + 1,
				MAX_LEVEL,
				(g_nTimer) / 10,
				g_nScore
			);

			if (g_nTimer <= 0)
			{
				g_nTimer = 0;
				KillPlayer();
				SetFade(SCENE_RESULT);
			}
			else if (g_nTimer < TIMER_DANGER)
			{
				if (g_nTimer % 20 == 0)
				{
					if (g_pFontInfo->obj.color == D3DXCOLOR_WHITE)
					{
						g_pFontInfo->obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					}
					else
					{
						g_pFontInfo->obj.color = D3DXCOLOR_WHITE;
					}
				}
			}
			else
			{
				if (GetPlayer()->state == PLAYERSTATE_DIED)
				{
					g_pFontInfo->obj.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else
				{
					g_pFontInfo->obj.color = D3DXCOLOR_WHITE;
				}
			}

			break;

		case GAMESTATE_CLEAR:	// �N���A
			// �v���C���[���I����ԂɈڍs
			SetPlayerState(PLAYERSTATE_END);

			sprintf(
				&g_pFontInfo->aText[0],
				"Level %02d/%02d | Time %03d | Score %06d",
				g_nCurrentStage + 1,
				MAX_LEVEL,
				(g_nTimer) / 10,
				g_nScore
			);

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

	if (g_bIsPause)
	{// �|�[�Y���j���[�`��
		DrawPauseBg();
		DrawPause();
	}

	DrawFont();
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
	g_nTimer = TIME_STATE_READY;
	g_nScore = 0;
}

//=====================================================================
// �|�[�Y��Ԃ̐؂�ւ�����
//=====================================================================
void TogglePause(bool bIsPause)
{
	if (GetFade() != FADE_NONE) return;
	if (GetCurrentScene() != SCENE_GAME) return;
	if (g_gameState == GAMESTATE_READY) return;

	g_bIsPause = bIsPause;
	g_pFontPauseMenuTitle->obj.bVisible = bIsPause;
	g_pFontCountDown->obj.bVisible = !bIsPause;
	GetPauseBg()->bVisible = bIsPause;

	if (bIsPause)
	{
		SetPauseMenuCursor(0);
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
			for (int i = 0; i < MAX_BLOCK_PARAM; i++)
			{
				g_map[y][x].nParam[i] = map->nParam[i];
			}
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

void DecreaseTime(int nTime)
{
	if (g_gameState != GAMESTATE_NORMAL) return;

	g_nTimer -= nTime;
	Clamp(&g_nTimer, 0, g_nTimer);
	g_pFontInfo->obj.color = D3DXCOLOR(1, 0, 0, 1);
}
