//=====================================================================
//
// block.cppのヘッダファイル [block.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

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
#define BLOCK_SIZE		(40)
#define NUM_BLOCK_X		(SCREEN_WIDTH / BLOCK_SIZE)
#define NUM_BLOCK_Y		(SCREEN_HEIGHT / BLOCK_SIZE)
#define MAX_BLOCK		(NUM_BLOCK_X * NUM_BLOCK_Y)

#define BLOCK_HIT_NONE		(0x0000)
#define BLOCK_HIT_TOP		(0x0001)
#define BLOCK_HIT_BOTTOM	(0x0002)
#define BLOCK_HIT_LEFT		(0x0004)
#define BLOCK_HIT_RIGHT		(0x0008)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	BLOCK_TYPE_AIR = 0,
	BLOCK_TYPE_VOID,
	BLOCK_TYPE_GRASS000,
	BLOCK_TYPE_GRASS001,
	BLOCK_TYPE_GRASS002,
	BLOCK_TYPE_GRASS003,
	BLOCK_TYPE_GRASS004,
	BLOCK_TYPE_GRASS005,
	BLOCK_TYPE_GRASS006,
	BLOCK_TYPE_GRASS007,
	BLOCK_TYPE_GRASS008,
	BLOCK_TYPE_GRASS009,
	BLOCK_TYPE_GRASS010,
	BLOCK_TYPE_GRASS011,
	BLOCK_TYPE_GRASS012,
	BLOCK_TYPE_NEEDLE000,
	BLOCK_TYPE_BLOCK000,
	BLOCK_TYPE_ITEM000,
	BLOCK_TYPE_FLAG000,
	BLOCK_TYPE_MAX
} BLOCK_TYPE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct BLOCK
{
	BASEOBJECT obj;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	bool bUsed;
	BLOCK_TYPE type;
	int nCounterState;
	bool bCollidable;
	void (*Update)(BLOCK* pBlock);
}BLOCK;

typedef struct
{
	D3DXCOLOR color;
	bool bCollidable;
	void (*Update)(BLOCK* pBlock);
}BLOCK_INFO;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
BLOCK* GetBlock(void);
BLOCK* SetBlock(BLOCK_TYPE type, int x, int y);
DWORD CollisionBlock(
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3 size,
	BLOCK** dpBlock
);
BLOCK_INFO* GetBlockInfo(void);

#endif