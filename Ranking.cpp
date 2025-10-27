//=====================================================================
//
// Ranking [Ranking.h]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseScene.h"
#include "ranking.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "font.h"
#include "player.h"
#include "decal.h"
#include "util.h"
#include "background.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define FILEPATH_RANKING		"data\\ranking.bin"
#define MAX_PLACE				(5)

#define INIT_RANKING			{ 20000, 18000, 17000, 15000, 13000 }

#define FADE_START				(600)

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
int compare(const void* arg1, const void* arg2);

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
int g_aRanking[MAX_PLACE] = INIT_RANKING;
int g_nCountStateRanking = 0;
int g_nHighlight = -1;
FONT* g_apFontNum[MAX_PLACE] = {};
FONT* g_apFontNumEasy[MAX_PLACE] = {};

//=====================================================================
// ����������
//=====================================================================
void InitRanking(void)
{
	char aString[MAX_PATH] = {};
	g_nCountStateRanking = 0;
	memset(g_apFontNum, 0, sizeof(g_apFontNum));

	InitBackground();
	InitFont();

	SetBackgroundColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

	LoadBin(FILEPATH_RANKING, &g_aRanking[0], sizeof(int), MAX_PLACE);

	qsort(&g_aRanking[0], MAX_PLACE, sizeof(int), compare);

	SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 50.0f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		80,
		"RANKING",
		DT_CENTER
	);

	for (int nCount = 0; nCount < MAX_PLACE; nCount++)
	{
		sprintf(&aString[0], "%2d, %06d", nCount + 1, g_aRanking[nCount]);

		g_apFontNum[nCount] = SetFont(
			FONT_LABEL_DONGURI,
			D3DXVECTOR3(0, 150.0f + (nCount * 100.0f), 0.0f),
			D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			100,
			&aString[0],
			DT_CENTER
		);
	}

	if (GetPreviousScene() == SCENE_TITLE)
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_RESULT00);
	}
}

//=====================================================================
// �I������
//=====================================================================
void UninitRanking(void)
{
	g_nHighlight = -1;
	memset(g_apFontNum, 0, sizeof(g_apFontNum));

	UninitBackground();
	UninitFont();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateRanking(void)
{
	UpdateBackground();

	if (g_nHighlight != -1 && g_nCountStateRanking % 5 == 0)
	{
		if (g_nCountStateRanking % 10 == 0)
		{
			g_apFontNum[g_nHighlight]->obj.color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			g_apFontNum[g_nHighlight]->obj.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	g_nCountStateRanking++;

	if (INPUT_TRIGGER_ACCEPT || g_nCountStateRanking > FADE_START)
	{
		SetFade(SCENE_TITLE);
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawRanking(void)
{
	DrawBackground();
	DrawFont();
}

void SaveScore(int nScore)
{
	int* pRanking;
	char pFilename[MAX_PATH];

	pRanking = &g_aRanking[0];
	strcpy(pFilename, FILEPATH_RANKING);

	// �t�@�C�����烉���L���O��ǂݍ���
	LoadBin(pFilename, pRanking, sizeof(int), MAX_PLACE);

	// ���ѕς���
	qsort(pRanking, MAX_PLACE, sizeof(int), compare);

	if (pRanking[MAX_PLACE - 1] < nScore)
	{// ��ԒႢ�X�R�A��茻�݂̃X�R�A�̕�������������u��������
		pRanking[MAX_PLACE - 1] = nScore;
	}
	else
	{// �����łȂ���Εۑ����Ȃ�
		return;
	}

	// ���ѕς���
	qsort(pRanking, MAX_PLACE, sizeof(int), compare);

	// �_�ŏ��̐ݒ�
	for (int nCount = MAX_PLACE - 1; nCount >= 0; nCount--)
	{// �����L���O��������m�F���Ă���
		if (pRanking[nCount] == nScore)
		{
			// ������������_�ŏ���ݒ�
			g_nHighlight = nCount;
			break;
		}
	}

	// �t�@�C���Ƀ����L���O�������o��
	SaveBin(pFilename, pRanking, sizeof(int), MAX_PLACE);
}

//=====================================================================
// qsort�֐��̔��菈��
//=====================================================================
int compare(const void* arg1, const void* arg2)
{
	int va = *(const int*)arg1;
	int vb = *(const int*)arg2;

	if (va > vb) return -1;
	if (va < vb) return 1;
	return 0;
}