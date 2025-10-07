//=====================================================================
//
// block.cpp�̃w�b�_�t�@�C�� [block.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define BLOCK_SIZE		(20)
#define NUM_BLOCK_X		(SCREEN_WIDTH / BLOCK_SIZE)
#define NUM_BLOCK_Y		(SCREEN_HEIGHT / BLOCK_SIZE)
#define MAX_BLOCK		(NUM_BLOCK_X * NUM_BLOCK_Y)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	BLOCK_TYPE_AIR = 0,
	BLOCK_TYPE_000,
	BLOCK_TYPE_001,
	BLOCK_TYPE_002,
	BLOCK_TYPE_003,
	BLOCK_TYPE_MAX
}BLOCK_TYPE;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct BLOCK
{
	BASEOBJECT obj;
	D3DXVECTOR3 posOld;
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
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
BLOCK* GetBlock(void);
BLOCK* SetBlock(BLOCK_TYPE type, int x, int y);
bool CollisionBlock(
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3 size,
	BLOCK** dpBlock
);

#endif