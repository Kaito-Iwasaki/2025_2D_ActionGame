//=====================================================================
//
// baseScene.cpp�̃w�b�_�t�@�C�� [baseScene.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BASESCENE_H_
#define _BASESCENE_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INPUT_TRIGGER_ACCEPT	(GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) || GetJoypadTrigger(JOYKEY_A))
#define INPUT_TRIGGER_PAUSE		(GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
#define INPUT_REPEAT_UP			(GetKeyboardRepeat(DIK_W) || GetJoypadRepeat(JOYKEY_UP) || GetJoystickRepeat(JOYSTICK_L_UP))
#define INPUT_REPEAT_DOWN		(GetKeyboardRepeat(DIK_S) || GetJoypadRepeat(JOYKEY_DOWN) || GetJoystickRepeat(JOYSTICK_L_DOWN))
#define INPUT_PRESS_LEFT		(GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT) || GetJoystickPress(JOYSTICK_L_LEFT))
#define INPUT_PRESS_RIGHT		(GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT) || GetJoystickPress(JOYSTICK_L_RIGHT))

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
//*********************************************************************
// �V�[���񋓌^
//*********************************************************************
typedef enum
{
	SCENE_TITLE = 0,	// �^�C�g�����
	SCENE_TUTORIAL,		// �`���[�g���A�����
	SCENE_GAME,			// �Q�[�����
	SCENE_RESULT,		// ���U���g���
	SCENE_RANKING,		// �����L���O���
	SCENE_EDITOR,		// �G�f�B�^���
	SCENE_MAX
}SCENE;	

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
//*********************************************************************
// �V�[�������֐��\����
//*********************************************************************
typedef struct
{
	void (*Init)(void);
	void (*Uninit)(void);
	void (*Update)(void);
	void (*Draw)(void);
}SCENEDATA;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitScene(void);
void UninitScene(void);
void UpdateScene(void);
void DrawScene(void);
SCENE SetScene(SCENE nextScene, bool bStopSound = true);
SCENE GetCurrentScene(void);
SCENE GetPreviousScene(void);

#endif