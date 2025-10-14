//=====================================================================
// 
// プレイヤー処理 [player.cpp]
// Author : 
//
//=====================================================================
#include "player.h"
#include "input.h"
#include "util.h"
#include "Game.h"
#include "fade.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"
#define TEXTURE_MAX_X			(4)
#define TEXTURE_MAX_Y			(2)
#define INIT_POS				D3DXVECTOR3(100, 500, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(75.0f, 75.0f, 0.0f) * 0.7f
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_HITBOX				D3DXVECTOR3(45.0f, 70.0f, 0.0f) * 0.7f
#define INIT_PLAYER_SPEED		(4.0f)
#define INIT_PLAYER_LIFE		(3)
#define INIT_PLAYER_JUMPPOWER	(16.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPlayer = NULL;
PLAYER g_player;

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス

	// 構造体の初期化
	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.pos = INIT_POS;
	g_player.obj.size = INIT_SIZE;
	g_player.obj.color = INIT_COLOR;
	g_player.obj.bVisible = true;
	g_player.hitBoxSize = INIT_HITBOX;
	g_player.state = PLAYERSTATE_INIT;
	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.nLife = INIT_PLAYER_LIFE;
	g_player.fJumpPower = INIT_PLAYER_JUMPPOWER;

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffPlayer
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitPlayer(void)
{
	if (g_pVtxBuffPlayer != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	if (g_pTexBuffPlayer != NULL)
	{// テクスチャバッファの破棄
		g_pTexBuffPlayer->Release();
		g_pTexBuffPlayer = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdatePlayer(void)
{
	g_player.nCounterState++;
	switch (g_player.state)
	{
	case PLAYERSTATE_INIT:
		g_player.obj.pos = INIT_POS;
		g_player.obj.bVisible = true;
		g_player.move = D3DXVECTOR3_ZERO;
		g_player.fSpeed = INIT_PLAYER_SPEED;
		g_player.nLife = INIT_PLAYER_LIFE;
		SetPlayerState(PLAYERSTATE_APPEAR);

	case PLAYERSTATE_APPEAR:
		g_player.obj.bVisible ^= 1;
		if (g_player.nCounterState > 120)
		{
			g_player.obj.bVisible = true;
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DAMAGED:
		g_player.obj.bVisible ^= 1;
		if (g_player.nCounterState > 120)
		{
			g_player.obj.bVisible ^= 1;
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_DIED:
		g_player.obj.bVisible = false;
		if (g_player.nCounterState > 30)
		{
			SetPlayerState(PLAYERSTATE_INIT);
		}
		return;

	case PLAYERSTATE_END:
		SetFade(SCENE_GAME);
		return;
	}

	// 前回の位置を記録
	g_player.posOld = g_player.obj.pos;

	// プレイヤーの操作処理
	g_player.move.x = 0;
	if (GetKeyboardPress(DIK_A))
	{// 左
		g_player.move.x -= g_player.fSpeed;
		/*g_player.nPatternAnimY = 1;*/
	}
	if (GetKeyboardPress(DIK_D))
	{// 右
		g_player.move.x += g_player.fSpeed;
		//g_player.nPatternAnimY = 0;
	}

	if (GetKeyboardPress(DIK_SPACE))
	{// ジャンプ
		if (g_player.bIsJumping == false)
		{
			g_player.bIsJumping = true;
			g_player.move.y = -g_player.fJumpPower;
		}
	}

	if (g_player.pBlock != NULL)
	{// ブロックに乗っているときにブロックの移動量を反映
		g_player.obj.pos += (g_player.pBlock->obj.pos - g_player.pBlock->posOld);
	}

	// 位置を更新
	g_player.move.y += GAME_GRAVITY;	// 重力を加算
	g_player.obj.pos += g_player.move;

	DWORD dwHit = CollisionBlock(&g_player.obj.pos, &g_player.posOld, &g_player.move, g_player.hitBoxSize, &g_player.pBlock);

	// ブロックとの衝突判定
	if (dwHit & BLOCK_HIT_TOP)
	{
		g_player.bIsJumping = false;
	}
	else
	{
		g_player.bIsJumping = true;
	}

	if (g_player.nCounterAnim % 5 == 0)
	{
		g_player.nPatternAnimX = (g_player.nPatternAnimX + 1) % 4;
	}
	g_player.nCounterAnim++;

	// アニメーションの更新
	if (g_player.bIsJumping)
	{// ジャンプ
		g_player.nPatternAnimX = 0;
		g_player.nPatternAnimY = 1;
	}
	else if (abs((long)g_player.move.x) > 0.01f)
	{// 移動
		g_player.nPatternAnimY = 1;
	}
	else
	{// 停止
		g_player.nPatternAnimY = 0;
	}

	// アニメーションの向きを設定
	if (g_player.move.x < 0)
	{// 左
		g_player.obj.bInversed = true;
	}
	else if (g_player.move.x > 0)
	{// 右
		g_player.obj.bInversed = false;
	}

	if (IsObjectOutOfScreen(g_player.obj))
	{// 画面外に出たら死亡
		SetPlayerState(PLAYERSTATE_DIED);
	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, 
		D3DXVECTOR3(g_player.obj.pos.x - g_player.obj.size.x / 2.0f, g_player.obj.pos.y - g_player.obj.size.y, 0.0f),
		D3DXVECTOR3(g_player.obj.pos.x + g_player.obj.size.x / 2.0f, g_player.obj.pos.y - g_player.obj.size.y, 0.0f),
		D3DXVECTOR3(g_player.obj.pos.x - g_player.obj.size.x / 2.0f, g_player.obj.pos.y, 0.0f),
		D3DXVECTOR3(g_player.obj.pos.x + g_player.obj.size.x / 2.0f, g_player.obj.pos.y, 0.0f)
	);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_player.obj.color);
	SetVertexTexturePos(pVtx, g_player.nPatternAnimX, g_player.nPatternAnimY, 4, 2, g_player.obj.bInversed);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.obj.bVisible == true)
	{// ポリゴン描画
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffPlayer);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

PLAYER* GetPlayer(void)
{
	return &g_player;
}

void HitPlayer(void)
{
	if (g_player.state == PLAYERSTATE_APPEAR)	return;
	if (g_player.state == PLAYERSTATE_DAMAGED)	return;
	if (g_player.state == PLAYERSTATE_DIED)		return;

	g_player.nLife--;

	if (g_player.nLife < 1)
	{
		SetPlayerState(PLAYERSTATE_DIED);
	}
	else
	{
		SetPlayerState(PLAYERSTATE_DAMAGED);
	}
}

void SetPlayerState(PLAYERSTATE state)
{
	g_player.state = state;
	g_player.nCounterState = 0;
}
