//=====================================================================
// 
// player.cpp�̃w�b�_�t�@�C�� [player.h]
// Author:
//
//=====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_		// ��d�C���N���[�h�h�~�̃}�N��

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"
#include "block.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define PLAYER_CHARGE_DECAY		(0.01f)
#define PLAYER_CHARGE_MAX		(1.0f)

//*********************************************************************
// 
// ***** �񋓌^ *****
// 
//*********************************************************************
typedef enum
{
	PLAYERSTATE_INIT = 0,
	PLAYERSTATE_APPEAR,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_DAMAGED,
	PLAYERSTATE_DIED,
	PLAYERSTATE_CLEAR,
	PLAYERSTATE_END,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*********************************************************************
// 
// ***** �\���� *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 hitBoxSize;
	PLAYERSTATE state;
	float fSpeed;
	int nLife;
	float fJumpPower;
	int nCounterState;
	int nCounterAnim;
	int nPatternAnimX;
	int nPatternAnimY;
	bool bIsJumping;
	float fCharge;
	bool bIsFlying;
	bool bIsControlEnabled;
	BLOCK* pBlock;
	D3DXVECTOR3 startPos;
}PLAYER;

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
void HitPlayer(void);
void SetPlayerState(PLAYERSTATE state);
void PausePlayer(void);
void UnPausePlayer(void);
void KillPlayer(void);

#endif