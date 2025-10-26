//=====================================================================
//
// Title [Title.h]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "Title.h"
#include "decal.h"
#include "input.h"
#include "fade.h"
#include "Game.h"
#include "sound.h"
#include "background.h"
#include "util.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define TIME_STATE_INTRO	(60)

#define LOGO_POS_X			SCREEN_CENTER
#define LOGO_POS_Y			SCREEN_VCENTER - 200
#define LOGO_POS			D3DXVECTOR3(LOGO_POS_X, LOGO_POS_Y, 0)
#define LOGO_SIZE			D3DXVECTOR3(450, 200, 0)
#define LOGO_COLOR			D3DXCOLOR_WHITE
#define LOGO_INIT_SCALE		(1.5f)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	TITLESTATE_INTRO = 0,
	TITLESTATE_NORMAL,
	TITLESTATE_START,
	TITLESTATE_MAX
}TITLESTATE;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
TITLESTATE g_stateTitle = TITLESTATE_INTRO;
int g_nCounterStateTitle = 0;
int g_nElapsedTimeTitle = 0;
DECAL* g_pDecalLogo = NULL;

//=====================================================================
// ����������
//=====================================================================
void InitTitle(void)
{
	InitDecal();
	InitBackground();

	g_stateTitle = TITLESTATE_INTRO;
	g_nCounterStateTitle = 0;
	g_nElapsedTimeTitle = 0;

	g_pDecalLogo = SetDecal(
		DECAL_LABEL_LOGO,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER - 200, 0),
		D3DXVECTOR3(450, 200, 0),
		D3DXVECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)
	);

	StopSound();
	PlaySound(SOUND_LABEL_BGM_TITLE00);
}

//=====================================================================
// �I������
//=====================================================================
void UninitTitle(void)
{
	UninitDecal();
	UninitBackground();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateTitle(void)
{
	UpdateBackground();
	
	g_nElapsedTimeTitle++;
	g_nCounterStateTitle++;
	switch (g_stateTitle)
	{
	case TITLESTATE_INTRO:
	{
		float fProgress = (float)g_nCounterStateTitle / (float)TIME_STATE_INTRO;

		g_pDecalLogo->obj.color.a = fProgress;
		g_pDecalLogo->obj.size = LOGO_SIZE / Clampf(fProgress, 0.5f, 1.0f);

		if (fProgress >= 1.0f)
		{
			g_pDecalLogo->obj.color.a = 1.0f;
			g_pDecalLogo->obj.size = LOGO_SIZE;

			g_stateTitle = TITLESTATE_NORMAL;
			g_nCounterStateTitle = 0;
		}

		break;
	}

	case TITLESTATE_NORMAL:
		if (INPUT_TRIGGER_ACCEPT)
		{// �Q�[���X�^�[�g
			ResetGame();
			SetFade(SCENE_GAME);
		}
		break;

	case TITLESTATE_START:

		break;
	}

	g_pDecalLogo->obj.pos = D3DXVECTOR3(LOGO_POS_X + sinf(g_nElapsedTimeTitle * 0.025f) * 10.0f, LOGO_POS_Y + cosf(g_nElapsedTimeTitle * 0.05f) * 10.0f, 0);
	g_pDecalLogo->obj.rot.z = sinf(g_nElapsedTimeTitle * 0.01f) * 0.075f;
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawTitle(void)
{
	DrawBackground();
	DrawDecal();
}