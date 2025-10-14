//=====================================================================
//
// editorblock.cppのヘッダファイル [editorblock.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _EDITORBLOCK_H_
#define _EDITORBLOCK_H_

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
#define MAP_FILENAME	"map"
#define MAP_FILENAME	"data\\MAP\\"

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	EDITORBLOCK_TYPE_AIR = 0,
	EDITORBLOCK_TYPE_VOID,
	EDITORBLOCK_TYPE_GRASS000,
	EDITORBLOCK_TYPE_GRASS001,
	EDITORBLOCK_TYPE_GRASS002,
	EDITORBLOCK_TYPE_GRASS003,
	EDITORBLOCK_TYPE_GRASS004,
	EDITORBLOCK_TYPE_GRASS005,
	EDITORBLOCK_TYPE_GRASS006,
	EDITORBLOCK_TYPE_GRASS007,
	EDITORBLOCK_TYPE_GRASS008,
	EDITORBLOCK_TYPE_GRASS009,
	EDITORBLOCK_TYPE_GRASS010,
	EDITORBLOCK_TYPE_GRASS011,
	EDITORBLOCK_TYPE_GRASS012,
	EDITORBLOCK_TYPE_NEEDLE000,
	EDITORBLOCK_TYPE_EDITORBLOCK000,
	EDITORBLOCK_TYPE_ITEM000,
	EDITORBLOCK_TYPE_FLAG000,
	EDITORBLOCK_TYPE_MAX
} EDITORBLOCK_TYPE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	bool bUsed;
	EDITORBLOCK_TYPE type;
}EDITORBLOCK;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitEditorBlock(void);
void UninitEditorBlock(void);
void UpdateEditorBlock(void);
void DrawEditorBlock(void);
EDITORBLOCK* GetEditorBlock(void);
EDITORBLOCK* SetEditorBlock(EDITORBLOCK_TYPE type, int x, int y);

#endif