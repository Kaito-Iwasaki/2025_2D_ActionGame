//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Kaito Iwasaki
// Special Thanks : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_CURSOR = 0,
	SOUND_LABEL_SE_JUMP,
	SOUND_LABEL_SE_JET,
	SOUND_LABEL_SE_COIN,
	SOUND_LABEL_SE_CLEAR,
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label, float fVolume = 1.0f);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
HRESULT UnPauseSound(SOUND_LABEL label);

#endif
