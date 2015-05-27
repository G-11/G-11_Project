//=============================================================================
//テクスチャの処理
//Creater:arai Yuhki
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_
//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
//=============================================================================
//列挙
//=============================================================================
typedef enum
{
	TEX_NONE = 0,

	TEX_NUMBER,
	TEX_GAGE,
	TEX_GAGE_POINT,
	TEX_SKY_MAP,
	TEX_GLOW_GAUGE,
	TEX_GLOE_GAUGE_FRAME,
	TEX_CLOCK,
	TEX_CLOCK_NEEDLE,

	TEX_EATAN,
	TEX_ITEM_OFFSET,
	TEX_ITEM_USAGI,
	TEX_ITEM_NEKO,
	TEX_ITEM_KUMA,

	TEX_INTERFACE_PUSH,

	TEX_TITLE_BG,
	TEX_RESULT_BG,
	TEX_GELFLOGO,

	TEX_FIELD1_BG,
	TEX_CILLING1_BG,
	TEX_FLOOR1_BG,

	TEX_GOAL,

	TEXTURE_MAX

}TEX;
//=============================================================================
//プロトタイプ
//=============================================================================
void InitTexture(void);
void UninitTexture(void);
//テクスチャの取得
LPDIRECT3DTEXTURE9 GetTexture(TEX id);

D3DXVECTOR2 GetImageSize(TEX texture);
D3DXIMAGE_INFO GetImageInfo(TEX texture);


#endif