//=====================================================================
//
// baseScene.cppのヘッダファイル [baseScene.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BASESCENE_H_
#define _BASESCENE_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
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
// ***** 列挙型 *****
// 
//*********************************************************************
//*********************************************************************
// シーン列挙型
//*********************************************************************
typedef enum
{
	SCENE_TITLE = 0,	// タイトル画面
	SCENE_TUTORIAL,		// チュートリアル画面
	SCENE_GAME,			// ゲーム画面
	SCENE_RESULT,		// リザルト画面
	SCENE_RANKING,		// ランキング画面
	SCENE_EDITOR,		// エディタ画面
	SCENE_MAX
}SCENE;	

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
//*********************************************************************
// シーン処理関数構造体
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
// ***** プロトタイプ宣言 *****
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