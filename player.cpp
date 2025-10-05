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
#include "block.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"
#define TEXTURE_MAX_X			(4)
#define TEXTURE_MAX_Y			(2)
#define INIT_POS				D3DXVECTOR3(SCREEN_CENTER, SCREEN_HEIGHT - 100, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(64.0f, 64.0f, 64.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

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
	g_player.posOld = g_player.obj.pos;

	if (GetKeyboardPress(DIK_A))
	{
		g_player.move.x -= 1;
		g_player.nPatternAnimY = 1;
	}
	else if (GetKeyboardPress(DIK_D))
	{
		g_player.move.x += 1;
		g_player.nPatternAnimY = 0;
	}

	if (GetKeyboardPress(DIK_SPACE))
	{
		if (g_player.bIsJumping == false)
		{
			g_player.bIsJumping = true;
			g_player.move.y = - 20.0f;
		}
	}


	g_player.obj.pos += g_player.move;

	if (abs((long)g_player.move.x) > 0.01f && Magnitude(g_player.obj.pos, g_player.posOld) > 0.1f)
	{
		if (g_player.nCounterAnim % 10 == 0)
		{
			g_player.nPatternAnimX = (g_player.nPatternAnimX + 1) % 4;
		}
		g_player.nCounterAnim++;
	}
	else
	{
		g_player.nPatternAnimX = 0;
	}

	g_player.move.x += (0 - g_player.move.x) * 0.5f;
	g_player.move.y += GAME_GRAVITY;

	if (CollisionBlock(&g_player.obj.pos, &g_player.posOld, &g_player.move, g_player.obj.size))
	{
		g_player.bIsJumping = false;
	}
	else
	{
		g_player.bIsJumping = true;

	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// デバイスの取得
	pDevice = GetDevice();

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