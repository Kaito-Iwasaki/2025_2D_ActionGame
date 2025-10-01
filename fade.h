//=====================================================================
// 
// fade.cppのヘッダファイル [fade.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _FADE_H_
#define _FADE_H_		// 二重インクルード防止のマクロ

#include "main.h"
#include "baseScene.h"

typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

// プロトタイプ宣言
void InitFade(SCENE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(SCENE modeNext, bool bStopSound = true);
FADE GetFade(void);
float GetFadeAlpha(void);

#endif