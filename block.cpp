//=====================================================================
//
// 敵弾オブジェクト [enemybullet.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "input.h"
#include "sound.h"
#include "util.h"
#include "collision.h"
#include "block.h"

#include "player.h"
#include "baseScene.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_POS						D3DXVECTOR3_ZERO
#define INIT_SIZE						D3DXVECTOR3(BLOCK_SIZE, BLOCK_SIZE, 0.0f)
#define INIT_COLOR						D3DXCOLOR(0.5f, 0.3f, 0.0f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBlock[BLOCK_TYPE_MAX] = {};
BLOCK g_aBlock[MAX_BLOCK] = {};

const char* g_aBlockFileName[BLOCK_TYPE_MAX] = {
	"data\\TEXTURE\\block000.png",
	"data\\TEXTURE\\block000.png",
	"data\\TEXTURE\\block000.png",
	"data\\TEXTURE\\block000.png",
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	BLOCK* pBlock = &g_aBlock[0];

	// 構造体の初期化
	memset(pBlock, 0, sizeof(BLOCK) * MAX_BLOCK);
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		pBlock->obj.pos = D3DXVECTOR3_ZERO;
		pBlock->obj.size = D3DXVECTOR3_ZERO;
		pBlock->obj.color = INIT_COLOR;
	}

	// テクスチャの読み込み
	for (int nCount = 0; nCount < BLOCK_TYPE_MAX; nCount++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_aBlockFileName[nCount],
			&g_pTexBuffBlock[nCount]
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBlock(void)
{
	memset(&g_aBlock[0], 0, sizeof(BLOCK) * MAX_BLOCK);

	if (g_pVtxBuffBlock != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}

	for (int nCount = 0; nCount < BLOCK_TYPE_MAX; nCount++)
	{
		if (g_pTexBuffBlock[nCount] != NULL)
		{// テクスチャバッファの破棄
			g_pTexBuffBlock[nCount]->Release();
			g_pTexBuffBlock[nCount] = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBlock(void)
{
	BLOCK* pBlock = &g_aBlock[0];
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	BLOCK* pBlock = &g_aBlock[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx,
			D3DXVECTOR3(pBlock->obj.pos.x, pBlock->obj.pos.y, 0.0f),
			D3DXVECTOR3(pBlock->obj.pos.x + pBlock->obj.size.x, pBlock->obj.pos.y, 0.0f),
			D3DXVECTOR3(pBlock->obj.pos.x, pBlock->obj.pos.y + pBlock->obj.size.y, 0.0f),
			D3DXVECTOR3(pBlock->obj.pos.x + pBlock->obj.size.x, pBlock->obj.pos.y + pBlock->obj.size.y, 0.0f)
			);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pBlock->obj.color);
		SetVertexTexturePos(pVtx, pBlock->obj.bInversed);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBlock = &g_aBlock[0];
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		if (pBlock->bUsed == true && pBlock->obj.bVisible == true)
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffBlock[pBlock->type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
BLOCK* GetBlock(void)
{
	return &g_aBlock[0];
}

//=====================================================================
// ブロックの設定処理
//=====================================================================
BLOCK* SetBlock(BLOCK_TYPE type, D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BLOCK* pBlock = &g_aBlock[0];

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		if (pBlock->bUsed == false)
		{
			memset(pBlock, 0, sizeof(BLOCK));
			pBlock->bUsed = true;
			pBlock->obj.pos = pos;
			pBlock->obj.size = INIT_SIZE;
			pBlock->obj.color = INIT_COLOR;
			pBlock->type = type;
			pBlock->obj.bVisible = true;
			pBlock->bCollidable = true;

			return pBlock; // ブロックの生成に成功
		}
	}
	return NULL;	// ブロックの生成に失敗
}

bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 size)
{
	bool bLand = false;

	BLOCK* pBlock = &g_aBlock[0];
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++, pBlock++)
	{
		if (pBlock->bUsed == false) continue;
		if (pBlock->bCollidable == false) continue;

		if (
			pPosOld->x <= pBlock->obj.pos.x - size.x
			&& pPos->x > pBlock->obj.pos.x - size.x
			&& pPos->y > pBlock->obj.pos.y
			&& pPos->y < pBlock->obj.pos.y + pBlock->obj.size.y + size.y
			)
		{
			pPos->x = pBlock->obj.pos.x - size.x;
			pMove->x = 0;
		}// 左からの衝突判定
		else if (
			pPosOld->x >= pBlock->obj.pos.x + pBlock->obj.size.x + size.x
			&& pPos->x < pBlock->obj.pos.x + pBlock->obj.size.x + size.x
			&& pPos->y > pBlock->obj.pos.y
			&& pPos->y < pBlock->obj.pos.y + pBlock->obj.size.y + size.y
			)
		{// 右からの衝突判定
			pPos->x = pBlock->obj.pos.x + pBlock->obj.size.x + size.x;
			pMove->x = 0;
		}

		if (
			pPosOld->y <= pBlock->obj.pos.y
			&& pPos->y > pBlock->obj.pos.y
			&& pPos->x > pBlock->obj.pos.x - size.x
			&& pPos->x < pBlock->obj.pos.x + pBlock->obj.size.x + size.x
			)
		{// 上からの衝突判定
			bLand = true;
			pPos->y = pBlock->obj.pos.y;
			pMove->y = 0;
		}
		else if (
			pPosOld->y - size.y >= pBlock->obj.pos.y + pBlock->obj.size.y
			&& pPos->y - size.y < pBlock->obj.pos.y + pBlock->obj.size.y
			&& pPos->x > pBlock->obj.pos.x - size.x
			&& pPos->x < pBlock->obj.pos.x + pBlock->obj.size.x + size.x
			)
		{// 下からの衝突判定
			pPos->y = pBlock->obj.pos.y + pBlock->obj.size.y + size.y;
			pMove->y = 0;
		}



	}

	return bLand;
}
