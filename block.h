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
#define MAX_BLOCK		(1024)
#define BLOCK_SIZE		(20.0f)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	BLOCK_TYPE_000 = 0,
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
typedef struct
{
	BASEOBJECT obj;
	bool bUsed;
	BLOCK_TYPE type;
	int nCounterState;
	bool bCollidable;
}BLOCK;

typedef struct
{
	BLOCK_TYPE type;
}MAP;

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
BLOCK* SetBlock(BLOCK_TYPE type, D3DXVECTOR3 pos);
BLOCK* SetBlock(BLOCK_TYPE type, int nX, int nY);
bool CollisionBlock(
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3 size
);

#endif