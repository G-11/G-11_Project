//=============================================================================
//
// プリミティブ表示処理 [main.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#define _CRT_SECURE_NO_WARNINGS	//警告対策
#define DIRECTINPUT_VERSION (0x0800)// 警告対策用
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include<Windows.h>
#include "d3dx9.h"
#include "dinput.h"
#include "xaudio2.h"
#include "Debug.h"
#include "MathTable.h"
#include "Texture.h"
#include "Input/VC.h"
#include "Window.h"
//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH (1280.0f)
#define SCREEN_HEIGHT (960.0f)
#define WINDOW_WIDTH (1280.0f)
#define WINDOW_HEIGHT (960.0f)
#define FVF_VERTEX_2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//2D用頂点フォーマット
#define FVF_POINT_SPRITE (D3DFVF_XYZW | D3DFVF_PSIZE | D3DFVF_DIFFUSE)
#define FVF_CUSTOM	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

#define SafeRelease(obj) { if(obj!=nullptr){obj->Release();obj=nullptr;} }
#define SafeDelete(obj) {if(obj!=nullptr){delete obj;obj=nullptr;}}
//=============================================================================
//色名(α値)
//=============================================================================
#define WHITE(Alpha) (D3DXCOLOR(1.0f,1.0f,1.0f,Alpha))
#define RED(Alpha) (D3DXCOLOR(1.0f,0.0f,0.0f,Alpha))
#define BLUE(Alpha) (D3DXCOLOR(0.0f,0.0f,1.0f,Alpha))
#define GREEN(Alpha) (D3DXCOLOR(0.0f,1.0f,0.0f,Alpha))
#define YELLOW(Alpha) (D3DXCOLOR(1.0f,1.0f,0.0f,Alpha))
#define PURPLE(Alpha) (D3DXCOLOR(1.0f,0.0f,1.0f,Alpha))
#define CYAN(Alpha)	(D3DXCOLOR(0.0f,1.0f,1.0f,Alpha))
#define BLACK(Alpha) (D3DXCOLOR(0.0f,0.0f,0.0f,Alpha))
#define GRAY(Alpha) (D3DXCOLOR(0.5f,0.5f,0.5f,Alpha))

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 vtx;	//頂点座標
	D3DXVECTOR2 tex;	//テクスチャの座標
}VERTEX_2D;

//ポイントスプライト
typedef struct
{
	D3DXVECTOR3 pos;
	FLOAT size;
	D3DCOLOR diffuse;

}POINT_SPRITE;

typedef struct
{
	D3DXVECTOR3 vtx;
	D3DXVECTOR3 nor;
	D3DXVECTOR2 tex;

}CUSTOM_VTX;



#endif