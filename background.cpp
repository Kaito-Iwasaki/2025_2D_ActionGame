//=====================================================================
//
// background [background.h]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "background.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define NUM_BG						(4)
#define INIT_COLOR					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define TEXTURE_LOOP_SIZE			(3)
#define BACKGROUND_SPEED			(-0.1f)
#define BACKGROUND_SPEED_OFFSET		(-0.1f)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBG[NUM_BG] = {};
BACKGROUND g_bg[NUM_BG];

const char* g_aTexFileNameBG[NUM_BG] = {
	"data\\TEXTURE\\game_bg000.png",
	"data\\TEXTURE\\game_bg001.png",
	"data\\TEXTURE\\game_bg002.png",
	"data\\TEXTURE\\game_bg003.png"
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	BACKGROUND* pBG = &g_bg[0];
	int nPosStartX = RandRange(0, SCREEN_WIDTH);

	memset(&g_bg, 0, sizeof(BASEOBJECT));
	for (int i = 0; i < NUM_BG; i++, pBG++)
	{
		pBG->obj.pos = D3DXVECTOR3(nPosStartX, SCREEN_VCENTER, 0);
		pBG->obj.size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0) * TEXTURE_LOOP_SIZE;
		pBG->obj.color = INIT_COLOR;
		pBG->fSpeed = BACKGROUND_SPEED + (i * BACKGROUND_SPEED_OFFSET);
		pBG->obj.bVisible = true;

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			g_aTexFileNameBG[i],
			&g_pTexBuffBG[i]
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBackground(void)
{
	if (g_pVtxBuffBG != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

	for (int i = 0; i < NUM_BG; i++)
	{
		if (g_pTexBuffBG[i] != NULL)
		{// テクスチャバッファの破棄
			g_pTexBuffBG[i]->Release();
			g_pTexBuffBG[i] = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBackground(void)
{
	BACKGROUND* pBG = &g_bg[0];

	for (int i = 0; i < NUM_BG; i++, pBG++)
	{
		if (pBG->obj.pos.x <= 0)
		{
			pBG->obj.pos.x = SCREEN_WIDTH;
		}

		pBG->obj.pos.x += pBG->fSpeed;
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;
	VERTEX_2D* pVtx;
	BACKGROUND* pBG = &g_bg[0];

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < NUM_BG; nCount++, pBG++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pBG->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pBG->obj.color);
		SetVertexTexturePos(pVtx,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(1.0f * TEXTURE_LOOP_SIZE, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f * TEXTURE_LOOP_SIZE),
			D3DXVECTOR2(1.0f * TEXTURE_LOOP_SIZE, 1.0f * TEXTURE_LOOP_SIZE)
		);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBG->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBG = &g_bg[0];
	for (int nCount = 0; nCount < NUM_BG; nCount++, pBG++)
	{
		if (pBG->obj.bVisible == true )
		{// ポリゴン描画
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffBG[nCount]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}