//=====================================================================
// 
// プレイヤー処理 [enemy.cpp]
// Author : 
//
//=====================================================================
#include "item.h"
#include "input.h"
#include "util.h"
#include "Game.h"
#include "collision.h"
#include "player.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\item000.png"
#define TEXTURE_MAX_X			(1)
#define TEXTURE_MAX_Y			(1)
#define INIT_POS				D3DXVECTOR3(SCREEN_CENTER, SCREEN_HEIGHT - 100, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(30.0f, 30.0f, 0.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)
#define INIT_HITBOX				INIT_SIZE

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffItem = NULL;
ITEM g_aItem[MAX_ITEM];

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	ITEM* pItem = &g_aItem[0];

	// 構造体の初期化
	memset(pItem, 0, sizeof(ITEM) * MAX_ITEM);
	for (int i = 0; i < MAX_ITEM; i++, pItem++)
	{
		pItem->obj.pos = INIT_POS;
		pItem->obj.size = INIT_SIZE;
		pItem->obj.color = INIT_COLOR;
		pItem->obj.bVisible = false;
		pItem->bUsed = false;
		pItem->hitBoxSize = INIT_HITBOX;
	}

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffItem
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitItem(void)
{
	if (g_pVtxBuffItem != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}

	if (g_pTexBuffItem != NULL)
	{// テクスチャバッファの破棄
		g_pTexBuffItem->Release();
		g_pTexBuffItem = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateItem(void)
{
	ITEM* pItem = &g_aItem[0];
	PLAYER* pPlayer = GetPlayer();

	for (int i = 0; i < MAX_ITEM; i++, pItem++)
	{
		if (pItem->bUsed == false) continue;

		if (pItem->nCounterAnim % 10 == 0)
		{
			pItem->nPatternAnimX = (pItem->nPatternAnimX + 1) % 4;
		}
		pItem->nCounterAnim++;

		if (BoxCollision(
			pItem->obj.pos - D3DXVECTOR3(0, pItem->obj.size.y, 0), pItem->hitBoxSize,
			pPlayer->obj.pos - D3DXVECTOR3(0, pPlayer->obj.size.y, 0), pPlayer->hitBoxSize
		))
		{// プレイヤーとの衝突判定
			pItem->bUsed = false;
		}
	}

}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;
	VERTEX_2D* pVtx;
	ITEM* pItem = &g_aItem[0];

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++, pItem++, pVtx += 4)
	{// 頂点情報を設定
		SetVertexPos(pVtx,
			D3DXVECTOR3(pItem->obj.pos.x - pItem->obj.size.x / 2.0f, pItem->obj.pos.y - pItem->obj.size.y, 0.0f),
			D3DXVECTOR3(pItem->obj.pos.x + pItem->obj.size.x / 2.0f, pItem->obj.pos.y - pItem->obj.size.y, 0.0f),
			D3DXVECTOR3(pItem->obj.pos.x - pItem->obj.size.x / 2.0f, pItem->obj.pos.y, 0.0f),
			D3DXVECTOR3(pItem->obj.pos.x + pItem->obj.size.x / 2.0f, pItem->obj.pos.y, 0.0f)
		);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pItem->obj.color);
		SetVertexTexturePos(pVtx, pItem->nPatternAnimX, pItem->nPatternAnimY, TEXTURE_MAX_X, TEXTURE_MAX_Y, pItem->obj.bInversed);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pItem = &g_aItem[0];
	for (int i = 0; i < MAX_ITEM; i++, pItem++)
	{
		if (pItem->obj.bVisible == true && pItem->bUsed == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffItem);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}

}

ITEM* SetItem(ITEM_TYPE type, D3DXVECTOR3 pos)
{
	ITEM* pItem = &g_aItem[0];
	for (int i = 0; i < MAX_ITEM; i++, pItem++)
	{
		if (pItem->bUsed == true) continue;

		pItem->bUsed = true;
		pItem->obj.pos = pos;
		pItem->obj.size = INIT_SIZE;
		pItem->obj.color = INIT_COLOR;
		pItem->obj.bVisible = true;
		pItem->hitBoxSize = INIT_HITBOX;
		pItem->move = { 2, 0 ,0};

		return pItem;
	}

	return NULL;
}