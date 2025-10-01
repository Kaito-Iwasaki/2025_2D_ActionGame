//=====================================================================
//
// font.cppのヘッダファイル [font.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _FONT_H_
#define _FONT_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_FONT		(256)
#define MAX_TEXTCHAR	(256)

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
typedef enum
{
	FONT_LABEL_DONGURI = 0,
	FONT_LABEL_MAX
}FONT_LABEL;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	LPD3DXFONT font;

	BASEOBJECT obj;
	FONT_LABEL type;
	bool bUsed;
	int nID;
	char aText[MAX_TEXTCHAR];
	UINT format;
}FONT;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitFont(void);
void UninitFont(void);
void UpdateFont(void);
void DrawFont(void);
FONT* GetFont(void);
FONT* SetFont(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col, int nScale, const char* aText, UINT format);
void DeleteFont(int nID);
void DeleteFont(void);

#endif