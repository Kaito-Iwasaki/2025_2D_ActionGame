//=====================================================================
//
// Tutorial [Tutorial.h]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "Tutorial.h"
#include "background.h"
#include "font.h"
#include "decal.h"
#include "block.h"
#include "player.h"
#include "item.h"
#include "effect.h"
#include "particle.h"
#include "pause.h"
#include "fuelbar.h"
#include "editorblock.h"
#include "util.h"
#include "fade.h"
#include "Game.h"
#include "sound.h"
#include "baseScene.h"
#include "input.h"
#include "pauseBg.h"

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


//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
MAPINFO g_mapTutorial[NUM_BLOCK_Y][NUM_BLOCK_X] = {};
TUTORIALSTATE g_stateTutorial = TUTORIALSTATE_NORMAL;
int g_nCounterStateTutorial = 0;
bool g_bIsPausedTutorial = false;
FONT* g_pFontPauseMenuTitleTutorial = NULL;
DECAL* g_pDecalTutorial[3] = {};

//=====================================================================
// ����������
//=====================================================================
void InitTutorial(void)
{
	// �e�I�u�W�F�N�g�̏���������
	InitDecal();
	InitFont();
	InitBlock();
	InitPlayer();
	InitItem();
	InitEffect();
	InitParticle();
	InitPause();
	InitPauseBg();
	InitFuelBar();
	InitBackground();

	g_pFontPauseMenuTitleTutorial = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 130, 0),
		D3DXVECTOR3(SCREEN_WIDTH, 100, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		60,
		"- PAUSE -",
		DT_CENTER
	);
	g_pFontPauseMenuTitleTutorial->obj.bVisible = false;

	g_pDecalTutorial[0] = SetDecal(
		DECAL_LABEL_TUTORIAL000,
		D3DXVECTOR3(250, 400, 0),
		D3DXVECTOR3(250, 100, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR_WHITE
	);
	g_pDecalTutorial[1] = SetDecal(
		DECAL_LABEL_TUTORIAL001,
		D3DXVECTOR3(500, 400, 0),
		D3DXVECTOR3(250, 100, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR_WHITE
	);
	g_pDecalTutorial[2] = SetDecal(
		DECAL_LABEL_TUTORIAL002,
		D3DXVECTOR3(900, 300, 0),
		D3DXVECTOR3(250, 100, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR_WHITE
	);

	SetTutorialState(TUTORIALSTATE_NORMAL);
	g_bIsPausedTutorial = false;

	if (GetPreviousScene() == SCENE_TITLE)
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME00);
	}

	// �X�e�[�W���̃��[�h
	memset(&g_mapTutorial[0][0], 0, sizeof(g_mapTutorial));
	LoadBin("data\\MAP\\tutorial.bin", &g_mapTutorial[0][0], sizeof(MAPINFO), MAX_BLOCK);

	// �X�e�[�W���Ɋ�Â��ău���b�N��z�u
	BLOCK* pBlock;
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			pBlock = SetBlock(g_mapTutorial[y][x].type, x, y);

			for (int i = 0; i < MAX_BLOCK_PARAM; i++)
			{
				pBlock->nParam[i] = g_mapTutorial[y][x].nParam[i];
			}
		}
	}
}

//=====================================================================
// �I������
//=====================================================================
void UninitTutorial(void)
{
	UninitDecal();
	UninitFont();
	UninitBlock();
	UninitPlayer();
	UninitItem();
	UninitEffect();
	UninitParticle();
	UninitPause();
	UninitPauseBg();
	UninitFuelBar();
	UninitBackground();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateTutorial(void)
{
	if (INPUT_TRIGGER_PAUSE)
	{// �|�[�Y���j���[
		TogglePauseTutorial(!g_bIsPausedTutorial);
	}

	if (g_bIsPausedTutorial)
	{
		UpdatePause();
	}
	else
	{
		UpdateBlock();
		UpdatePlayer();
		UpdateItem();
		UpdateEffect();
		UpdateParticle();
		UpdatePause();
		UpdateFuelBar();
		UpdateBackground();

		g_nCounterStateTutorial++;
		switch (g_stateTutorial)
		{
		case TUTORIALSTATE_NORMAL:
			break;

		case TUTORIALSTATE_CLEAR:
			SetPlayerState(PLAYERSTATE_END);

			if (g_nCounterStateTutorial > 60)
			{
				ResetGame();
				SetFade(SCENE_GAME);
				SetTutorialState(TUTORIALSTATE_END);
			}

			break;

		case TUTORIALSTATE_END:
			break;
		}
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawTutorial(void)
{
	DrawBackground();
	DrawDecal();
	DrawBlock();
	DrawItem();
	DrawEffect();
	DrawPlayer();
	DrawFuelBar();

	if (g_bIsPausedTutorial)
	{
		DrawPauseBg();
		DrawPause();
	}
	DrawFont();
}

void SetTutorialState(TUTORIALSTATE state)
{
	g_stateTutorial = state;
	g_nCounterStateTutorial = 0;
}

void TogglePauseTutorial(bool bIsPause)
{
	if (GetFade() != FADE_NONE) return;
	if (GetCurrentScene() != SCENE_TUTORIAL) return;
	if (g_stateTutorial == TUTORIALSTATE_CLEAR) return;
	if (g_stateTutorial == TUTORIALSTATE_END) return;

	g_bIsPausedTutorial = bIsPause;
	g_pFontPauseMenuTitleTutorial->obj.bVisible = bIsPause;
}
