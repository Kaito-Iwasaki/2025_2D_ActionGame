//=====================================================================
// 
// 汎用関数 [util.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _UTIL_H_
#define _UTIL_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
int Clamp(int nNum, int nMin, int nMax);
void Clamp(int* pNum, int nMin, int nMax);
int Clampf(float fNum, float fMin, float fMax);
void Clampf(float* pNum, float fMin, float fMax);
int RandRange(int nMin, int nMax);
float Dot(D3DXVECTOR2 vectorA, D3DXVECTOR2 vectorB);
D3DXVECTOR3 Normalize(D3DXVECTOR3 vector);
D3DXVECTOR3 Direction(D3DXVECTOR3 from, D3DXVECTOR3 to);
D3DXVECTOR3 Direction(float fAngle);
float Angle(D3DXVECTOR3 from, D3DXVECTOR3 to);
float Magnitude(D3DXVECTOR3 vector);
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to);
D3DXVECTOR2 Vector3To2(D3DXVECTOR3 source);
D3DXVECTOR3 Vector2To3(D3DXVECTOR2 source, float fValueZ = 0.0f);
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);

#endif