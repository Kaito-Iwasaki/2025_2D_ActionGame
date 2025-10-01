//=====================================================================
// 
// フェード処理 [fade.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "fade.h"
#include "input.h"

// マクロ定義
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y			(SCREEN_HEIGHT / 2)

#define FADE_SCALE				(0.025f)

// グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;				// 頂点バッファへのポインタ
FADE g_fade;
SCENE g_sceneNext;
D3DXCOLOR g_colorFade;
float g_fFadeScale;
bool g_bStopSound = true;

//=====================================================================
// 初期化処理
//=====================================================================
void InitFade(SCENE sceneNext)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 変数の初期化
	g_fade = FADE_IN;
	g_sceneNext = sceneNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_fFadeScale = FADE_SCALE;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// 頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();

	// モードの設定
	SetScene(g_sceneNext);
	SetFade(g_sceneNext);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitFade(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateFade(void)
{
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	if (g_fade != FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			if (g_colorFade.a > 0 && g_colorFade.a < 1)
			{// Enterキーを押されたら早くする
				g_fFadeScale = FADE_SCALE * 2;
			}
		}

		if (g_fade == FADE_IN)
		{// フェードイン
			g_colorFade.a -= g_fFadeScale;

			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
				g_fFadeScale = FADE_SCALE;

			}
		}
		else if (g_fade == FADE_OUT)
		{// フェードアウト
			g_colorFade.a += g_fFadeScale;

			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;

				SetScene(g_sceneNext, g_bStopSound);
			}
		}

		// 頂点カラーの設定
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;
	}
	
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================================================
// フェード切り替え処理
//=====================================================================
void SetFade(SCENE sceneNext, bool bStopSound)
{
	if (g_fade == FADE_NONE)
	{
		g_fade = FADE_OUT;
		g_sceneNext = sceneNext;
		g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_fFadeScale = FADE_SCALE;
		g_bStopSound = bStopSound;
	}
}

//=====================================================================
// 現在のフェード状態を取得
//=====================================================================
FADE GetFade(void)
{
	return g_fade;
}

//=====================================================================
// 現在のフェードの透明地を取得
//=====================================================================
float GetFadeAlpha(void)
{
	return (float)g_colorFade.a;
}