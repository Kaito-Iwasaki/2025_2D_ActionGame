//=====================================================================
// 
// 汎用関数 [util.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "util.h"

//=====================================================================
// 整数を範囲内に収める処理
//=====================================================================
int Clamp(int nNum, int nMin, int nMax)
{
	if (nNum < nMin)
	{
		return nMin;
	}
	else if (nNum > nMax)
	{
		return nMax;
	}

	return nNum;
}

//=====================================================================
// 整数を範囲内に収める処理（ポインタから直接編集）
//=====================================================================
void Clamp(int* pNum, int nMin, int nMax)
{
	if (*pNum < nMin)
	{
		*pNum = nMin;
	}
	else if (*pNum > nMax)
	{
		*pNum = nMax;
	}
}

//=====================================================================
// 小数を範囲内に収める処理
//=====================================================================
float Clampf(float fNum, float fMin, float fMax)
{
	if (fNum < fMin)
	{
		return fMin;
	}
	else if (fNum > fMax)
	{
		return fMax;
	}

	return fNum;
}

//=====================================================================
// 小数を範囲内に収める処理（ポインタから直接編集）
//=====================================================================
void Clampf(float* pNum, float fMin, float fMax)
{
	if (*pNum < fMin)
	{
		*pNum = fMin;
	}
	else if (*pNum > fMax)
	{
		*pNum = fMax;
	}
}

//=====================================================================
// ベクトルの内積
//=====================================================================
float Dot(D3DXVECTOR2 vectorA, D3DXVECTOR2 vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
}

//=====================================================================
// ベクトルの正規化処理
//=====================================================================
D3DXVECTOR3 Normalize(D3DXVECTOR3 vector)
{
	float fMagnitude = sqrtf(vector.x * vector.x + vector.y * vector.y);

	if (fMagnitude != 0)
	{
		return D3DXVECTOR3(vector.x / fMagnitude, vector.y / fMagnitude, vector.z / fMagnitude);
	}
	else
	{
		return D3DXVECTOR3_ZERO;
	}
}

//=====================================================================
// 方向ベクトル（二つのベクトルから）
//=====================================================================
D3DXVECTOR3 Direction(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return Normalize(to - from);
}

//=====================================================================
// 方向ベクトル（角度から）
//=====================================================================
D3DXVECTOR3 Direction(float fAngle)
{
	return D3DXVECTOR3(sin(fAngle), cos(fAngle), 0.0f);
}

//=====================================================================
// ベクトルの大きさ（距離）を求める処理
//=====================================================================
float Magnitude(D3DXVECTOR3 vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

//=====================================================================
// 二つのベクトルの大きさ（距離）を求める処理
//=====================================================================
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	D3DXVECTOR3 fDistance = to - from;

	return sqrtf(fDistance.x * fDistance.x + fDistance.y * fDistance.y);
}

//=====================================================================
// ベクトルから別のベクトルまでの角度を求める処理
//=====================================================================
float Angle(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return atan2f(to.x - from.x, to.y - from.y);
}

//=====================================================================
// 3次元ベクトルから2次元ベクトルへの変換処理
//=====================================================================
D3DXVECTOR2 Vector3To2(D3DXVECTOR3 source)
{
	return D3DXVECTOR2(source.x, source.y);
}

//=====================================================================
// 2次元ベクトルから3次元ベクトルへの変換処理
//=====================================================================
D3DXVECTOR3 Vector2To3(D3DXVECTOR2 source, float fValueZ)
{
	return D3DXVECTOR3(source.x, source.y, fValueZ);
}

//=====================================================================
// 指定した範囲の乱数（整数）を取得する処理
//=====================================================================
int RandRange(int nMin, int nMax)
{
	return (rand() % (nMax - nMin)) + nMin;
}

//=====================================================================
// バイナリファイル読み込み処理
//=====================================================================
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;

	pFile = fopen(pFilePath, "rb");

	if (pFile != NULL)
	{
		fread(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================================
// バイナリファイル書き出し処理
//=====================================================================
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;

	pFile = fopen(pFilePath, "wb");

	if (pFile != NULL)
	{
		fwrite(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}
