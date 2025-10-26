//=====================================================================
// 
// 体力バー [FuelBar.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "fuelbar.h"
#include "player.h"
#include "util.h"
#include "baseScene.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_SIZE_X				(36.0f)
#define INIT_SIZE_Y				(10.0f)

#define COLOR_REMAIN			D3DXCOLOR(1.0f, 0.75f, 0.0f, 1.0f)
#define COLOR_REMAIN_DANGER		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_EMPTY				D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)

#define BAR_POS_OFFSET		D3DXVECTOR3(-INIT_SIZE_X / 2, -65.0, 0)

//*********************************************************************
// 
// ***** 列挙型定義 *****
// 
//*********************************************************************
typedef enum
{
	FUELBAR_TYPE_EMPTY = 0,
	FUELBAR_TYPE_REMAIN,
	FUELBAR_TYPE_MAX
}FUELBAR_TYPE;


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFuelBar = NULL;		// 頂点バッファへのポインタ
FUELBAR g_aFuelBar[FUELBAR_TYPE_MAX];

D3DXCOLOR g_aColorFuelBar[FUELBAR_TYPE_MAX] = {
	COLOR_EMPTY,
	COLOR_REMAIN
};

int g_nCounterDamaged = 0;
float g_fLastRemainSize = INIT_SIZE_X;

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitFuelBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FUELBAR* pFuelBar = &g_aFuelBar[0];

	g_nCounterDamaged = 0;
	g_fLastRemainSize = INIT_SIZE_X;

	// 構造体の初期化
	memset(pFuelBar, 0, sizeof(FUELBAR) * FUELBAR_TYPE_MAX);
	for (int nCount = 0; nCount < FUELBAR_TYPE_MAX; nCount++, pFuelBar++)
	{
		pFuelBar->obj.pos = D3DXVECTOR3_ZERO;
		pFuelBar->obj.size = { INIT_SIZE_X, INIT_SIZE_Y, 0 };
		pFuelBar->obj.color = g_aColorFuelBar[nCount];
		pFuelBar->obj.bVisible = true;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * FUELBAR_TYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFuelBar,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitFuelBar(void)
{
	if (g_pVtxBuffFuelBar != NULL)
	{
		g_pVtxBuffFuelBar->Release();
		g_pVtxBuffFuelBar = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateFuelBar(void)
{
	FUELBAR* pFuelBar = &g_aFuelBar[0];
	PLAYER* pPlayer = GetPlayer();

	for (int nCount = 0; nCount < FUELBAR_TYPE_MAX; nCount++, pFuelBar++)
	{
		if (pPlayer->fCharge == PLAYER_CHARGE_MAX || pPlayer->obj.bVisible == false)
		{
			pFuelBar->obj.bVisible = false;
		}
		else
		{
			pFuelBar->obj.bVisible = true;
		}

		pFuelBar->obj.pos = pPlayer->obj.pos + BAR_POS_OFFSET;

		switch (nCount)
		{
		case FUELBAR_TYPE_REMAIN:
			pFuelBar->obj.size.x = INIT_SIZE_X * pPlayer->fCharge;
			Clampf(&pFuelBar->obj.size.x, 0.0f, pFuelBar->obj.size.x);

			if (pPlayer->fCharge < 0.4f)
			{
				pFuelBar->obj.color = COLOR_REMAIN_DANGER;
			}
			else
			{
				pFuelBar->obj.color = COLOR_REMAIN;
			}

			break;
		}
	}
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawFuelBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	FUELBAR* pFuelBar = &g_aFuelBar[0];

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffFuelBar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < FUELBAR_TYPE_MAX; nCount++, pFuelBar++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx,
			pFuelBar->obj.pos,
			pFuelBar->obj.pos + D3DXVECTOR3(pFuelBar->obj.size.x, 0, 0),
			pFuelBar->obj.pos + D3DXVECTOR3(0, pFuelBar->obj.size.y, 0),
			pFuelBar->obj.pos + pFuelBar->obj.size
		);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pFuelBar->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffFuelBar->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFuelBar, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pFuelBar = &g_aFuelBar[0];
	for (int nCount = 0; nCount < FUELBAR_TYPE_MAX; nCount++, pFuelBar++)
	{
		if (pFuelBar->obj.bVisible == false) continue;

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}