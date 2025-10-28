//=====================================================================
//
// editorblock.cpp�̃w�b�_�t�@�C�� [editorblock.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _EDITORBLOCK_H_
#define _EDITORBLOCK_H_

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "baseObject.h"
#include "main.h"
#include "block.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	BLOCK_TYPE type;
	int nParam[MAX_BLOCK_PARAM];
}EDITORBLOCK;

typedef struct
{
	BLOCK_TYPE type;				// �u���b�N�̎��
	int nParam[MAX_BLOCK_PARAM];	// �u���b�N�̃p�����[�^
}MAPINFO;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitEditorBlock(void);
void UninitEditorBlock(void);
void UpdateEditorBlock(void);
void DrawEditorBlock(void);
EDITORBLOCK* GetEditorBlock(void);
EDITORBLOCK* SetEditorBlock(BLOCK_TYPE type, int x, int y);
bool SaveEditorBlock(const char* aFileName);


#endif