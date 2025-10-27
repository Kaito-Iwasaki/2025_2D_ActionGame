//=====================================================================
// 
// プレイヤー処理 [player.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================
#include "player.h"
#include "input.h"
#include "util.h"
#include "Game.h"
#include "fade.h"
#include "particle.h"
#include "sound.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"
#define TEXTURE_MAX_X			(4)
#define TEXTURE_MAX_Y			(3)
#define INIT_POS				D3DXVECTOR3(100.0f, 640.0f, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(75.0f, 75.0f, 0.0f) * 0.7f
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define INIT_HITBOX				D3DXVECTOR3(45.0f, 70.0f, 0.0f) * 0.7f
#define INIT_PLAYER_SPEED		(4.0f)
#define INIT_PLAYER_LIFE		(3)
#define INIT_PLAYER_JUMPPOWER	(10.0f)
#define INIT_PLAYER_CHARGE		PLAYER_CHARGE_MAX

#define TIME_STATE_DIED			(60)

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
	g_player.obj.bVisible = false;
	g_player.hitBoxSize = INIT_HITBOX;
	g_player.startPos = INIT_POS;
	g_player.state = PLAYERSTATE_INIT;
	g_player.fSpeed = INIT_PLAYER_SPEED;
	g_player.nLife = INIT_PLAYER_LIFE;
	g_player.fJumpPower = INIT_PLAYER_JUMPPOWER;
	g_player.fCharge = INIT_PLAYER_CHARGE;
	g_player.bIsFlying = false;
	g_player.bIsControlEnabled = true;

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
		g_player.obj.pos = g_player.startPos;
		g_player.obj.bVisible = true;
		g_player.obj.bInversed = false;
		g_player.move = D3DXVECTOR3_ZERO;
		g_player.fSpeed = INIT_PLAYER_SPEED;
		g_player.nLife = INIT_PLAYER_LIFE;
		g_player.fCharge = INIT_PLAYER_CHARGE;
		g_player.bIsControlEnabled = true;
		SetPlayerState(PLAYERSTATE_APPEAR);
		PlaySound(SOUND_LABEL_SE_APPEAR);

		{
			EFFECTINFO info;
			info.col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
			info.fMaxAlpha = 1.0f;
			info.fMaxScale = 0.5f;
			info.fRotSpeed = 0.5f;
			info.fSpeed = 1.0f;
			info.nMaxLife = 70;

			SetParticle(
				info,
				g_player.obj.pos,
				0.0f,
				D3DX_PI * 2,
				1,
				10
			);

			info.col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);

			SetParticle(
				info,
				g_player.obj.pos,
				0.0f,
				D3DX_PI * 2,
				1,
				10
			);

			info.col = D3DXCOLOR(0.9f, 1.0f, 0.9f, 1.0f);

			SetParticle(
				info,
				g_player.obj.pos,
				0.0f,
				D3DX_PI * 2,
				1,
				10
			);
		}

		break;

	case PLAYERSTATE_APPEAR:
		g_player.obj.bVisible ^= 1;

		if (g_player.nCounterState > 90)
		{
			g_player.obj.bVisible = true;
			SetPlayerState(PLAYERSTATE_NORMAL);
		}
		break;

	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DIED:
		if (g_player.nCounterState > TIME_STATE_DIED)
		{
			//SetGameState(GAMESTATE_END);
			SetPlayerState(PLAYERSTATE_INIT);
		}

		{
			EFFECTINFO info = {};
			info.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			info.fMaxAlpha = 0.5f;
			info.fMaxScale = 0.5f;
			info.fRotSpeed = 0.5f;
			info.fSpeed = 4.0f;
			info.nMaxLife = 30;

			SetParticle(
				info,
				g_player.obj.pos + D3DXVECTOR3(0, -2, 0),
				0.0f,
				0.6f,
				1,
				1
			);
		}

		g_player.move.y += GAME_GRAVITY * 2;	// 重力を加算
		g_player.obj.pos += g_player.move;
		return;

	case PLAYERSTATE_END:
		StopSound(SOUND_LABEL_SE_JET);
		g_player.bIsControlEnabled = false;
		g_player.bIsFlying = false;

		g_player.nPatternAnimX = 2;
		g_player.nPatternAnimY = 2;

		break;
	}

	// 前回の位置を記録
	g_player.posOld = g_player.obj.pos;

	// プレイヤーの操作処理
	g_player.move.x = 0;
	if (g_player.bIsControlEnabled && GetFade() == FADE_NONE && GetGameState() != GAMESTATE_READY)
	{
		if (INPUT_PRESS_LEFT)
		{// 左
			g_player.move.x -= g_player.fSpeed;
		}
		if (INPUT_PRESS_RIGHT)
		{// 右
			g_player.move.x += g_player.fSpeed;
		}

		if (GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_A))
		{// ジャンプ
			if (g_player.bIsJumping == false)
			{// 接地中にスペースキーが押された
				EFFECTINFO infoWhite;
				infoWhite.col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
				infoWhite.fMaxAlpha = 0.5f;
				infoWhite.fMaxScale = 0.6f;
				infoWhite.fRotSpeed = 0.01f;
				infoWhite.fSpeed = 1.0f;
				infoWhite.nMaxLife = 60;

				SetParticle(
					infoWhite,
					g_player.obj.pos,
					D3DX_PI,
					D3DX_PI / 2,
					1,
					5
				);

				PlaySound(SOUND_LABEL_SE_JUMP);

				g_player.bIsJumping = true;
				g_player.move.y = -g_player.fJumpPower;
			}
			else
			{// 空中でスペースキーが押された
				if (g_player.fCharge > 0)
				{
					PlaySound(SOUND_LABEL_SE_JET, 0.1f);
					g_player.bIsFlying = true;
				}
			}
		}
		else if (GetKeyboardRelease(DIK_SPACE) || GetJoypadRelease(JOYKEY_A))
		{// 空中でスペースキーが離された
			StopSound(SOUND_LABEL_SE_JET);
			g_player.bIsFlying = false;
		}
	}

	// 空中での挙動
	if (g_player.bIsFlying)
	{
		g_player.move.y = 0;
		g_player.fCharge -= PLAYER_CHARGE_DECAY;

		EFFECTINFO infoRed;
		infoRed.col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
		infoRed.fMaxAlpha = 0.5f;
		infoRed.fMaxScale = 0.6f;
		infoRed.fRotSpeed = 0.5f;
		infoRed.fSpeed = 4.0f;
		infoRed.nMaxLife = 30;

		EFFECTINFO infoWhite = infoRed;
		infoWhite.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		infoWhite.fMaxScale = 0.5f;

		SetParticle(
			infoRed,
			g_player.obj.pos + D3DXVECTOR3(0, -2, 0),
			0.0f,
			0.6f,
			1,
			1
		);

		SetParticle(
			infoWhite,
			g_player.obj.pos + D3DXVECTOR3(0, -2, 0),
			0.0f,
			0.6f,
			1,
			1
		);

		if (g_player.fCharge <= 0.0f)
		{
			StopSound(SOUND_LABEL_SE_JET);
			g_player.bIsFlying = false;
		}
	}

	if (g_player.pBlock != NULL)
	{// ブロックに乗っているときにブロックの移動量を反映
		g_player.obj.pos += (g_player.pBlock->obj.pos - g_player.pBlock->posOld);
	}

	// 位置を更新
	if (g_player.bIsFlying == false)
	{
		g_player.move.y += GAME_GRAVITY;	// 重力を加算
	}
	g_player.obj.pos += g_player.move;

	// ブロックとの衝突判定
	DWORD dwHit = CollisionBlock(&g_player.obj.pos, &g_player.posOld, &g_player.move, g_player.hitBoxSize, &g_player.pBlock);

	if (dwHit & BLOCK_HIT_TOP)
	{// 地面に接している
		if (g_player.bIsJumping == true)
		{
			EFFECTINFO info;
			info.col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
			info.fMaxAlpha = 0.5f;
			info.fMaxScale = 0.4f;
			info.fRotSpeed = 0.5f;
			info.fSpeed = 4.0f;
			info.nMaxLife = 20;

			SetParticle(
				info,
				g_player.obj.pos,
				D3DX_PI,
				D3DX_PI / 2,
				1,
				5
			);
		}
		g_player.bIsJumping = false;
		g_player.bIsFlying = false;
		g_player.fCharge += 0.05f;
		Clampf(&g_player.fCharge, 0.0f, 1.0f);
	}
	else
	{// 空中にいる
		g_player.bIsJumping = true;
	}

	if (g_player.nCounterAnim % 5 == 0)
	{
		g_player.nPatternAnimX = (g_player.nPatternAnimX + 1) % 4;

		if (abs((long)g_player.move.x) > 0.01f && g_player.bIsJumping == false)
		{
			EFFECTINFO info;
			info.col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
			info.fMaxAlpha = 0.5f;
			info.fMaxScale = 0.4f;
			info.fRotSpeed = 0.5f;
			info.fSpeed = 0.5f;
			info.nMaxLife = 60;

			float fAngle = -2.2f;

			if (g_player.obj.bInversed)
			{
				fAngle *= -1;
			}

			SetParticle(
				info,
				g_player.obj.pos,
				fAngle,
				0.5f,
				1,
				3
			);
		}
	}
	g_player.nCounterAnim++;

	// アニメーションの更新
	if (g_player.state != PLAYERSTATE_END)
	{
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
	}

	if (IsObjectOutOfScreen(g_player.obj, OOS_BOTTOM))
	{// 画面外に出たら死亡
		KillPlayer();
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
	SetVertexTexturePos(pVtx, g_player.nPatternAnimX, g_player.nPatternAnimY, TEXTURE_MAX_X, TEXTURE_MAX_Y, g_player.obj.bInversed);

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

void PausePlayer(void)
{
	if (g_player.bIsFlying)
	{
		PauseSound(SOUND_LABEL_SE_JET);
	}
}

void UnPausePlayer(void)
{
	if (g_player.bIsFlying)
	{
		UnPauseSound(SOUND_LABEL_SE_JET);
	}

	if (GetKeyboardPress(DIK_SPACE) == false && GetJoypadRelease(JOYKEY_A) == false)
	{
		StopSound(SOUND_LABEL_SE_JET);
		g_player.bIsFlying = false;
	}
}

void KillPlayer(void)
{
	if (g_player.state == PLAYERSTATE_DIED) return;
	if (g_player.state == PLAYERSTATE_END) return;
	if (GetGameState() == GAMESTATE_CLEAR) return;

	g_player.obj.bVisible = true;
	g_player.nPatternAnimX = 0;
	g_player.nPatternAnimY = 2;
	g_player.bIsFlying = false;
	StopSound(SOUND_LABEL_SE_JET);
	PlaySound(SOUND_LABEL_SE_DEAD);
	g_player.move = D3DXVECTOR3(0.0f, -g_player.fJumpPower, 0.0f);
	SetPlayerState(PLAYERSTATE_DIED);
}
