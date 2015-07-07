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
	TEX_NUMBER_CHOKE,
	TEX_GAGE,
	TEX_GAGE_POINT,
	TEX_CURSOR,

	TEX_HINT1,
	TEX_HINT2,
	TEX_HINT3,

	TEX_SKY_MAP,
	TEX_EFFECT_GROWUP,
	TEX_EFFECT_CHECKPOINT,
	TEX_EFFECT_TAIL1,
	TEX_EFFECT_STAR,
	TEX_EFFECT_CUT,
	TEX_EFFECT_SMORKE,
	TEX_EFFECT_LIGHT,
	TEX_EFFECT_STAR_NUMBER,
	TEX_EFFECT_WIND,
	TEX_MASK_EAT,

	TEX_GLOW_GAUGE,
	TEX_GLOE_GAUGE_FRAME,
	TEX_CLOCK,
	TEX_CLOCK_NEEDLE,

	TEX_EATAN,
	TEX_ITEM_OFFSET,
	TEX_ITEM_USAGI,
	TEX_ITEM_NEKO,
	TEX_ITEM_KUMA,
	TEX_ITEM_BUTA,
	TEX_ITEM_USI,
	TEX_ITEM_SAI,
	TEX_ITEM_TAIZOU,

	TEX_CREATURE_TAIZO,

	TEX_LOG_GIMMICK,
	TEX_LOG_WALL,
	TEX_LOG_BREAK,
	TEX_TUTA,
	TEX_CLOG,
	TEX_BREAKABLE_OBJ,
	TEX_UNBREAKABLE_OBJ,
	TEX_KIRIKABU,

	TEX_INTERFACE_PUSH,
	TEX_INTERFACE_PUSH_START,
//	TEX_INTERFACE_PUSH_TUTORIAL,
	TEX_INTERFACE_PUSH_COLLECTION,
	TEX_INTERFACE_PUSH_OPTION,
	TEX_INTERFACE_MOKUHYOU1,
	TEX_INTERFACE_MOKUHYOU2,

	TEX_OPTION_BACKTO_GAME,
	TEX_OPTION_BACKTO_STAGESELECT,
	TEX_OPTION_OPERATION,
//	TEX_OPTION_GETITEM,

	TEX_TITLE_BG,
	TEX_RESULT_BG,
	TEX_GELFLOGO,

	TEX_OPTION_BG,
	TEX_OPTION_SETTEI,
	TEX_OPTION_ONGAKU,
	TEX_OPTION_KOUKAON,
	TEX_OPTION_MODORU,

	TEX_FIELD1_BG,
	TEX_CILLING1_BG,
	TEX_FLOOR1_BG,

	TEX_GOAL,

	TEX_EATAN_ME,
	TEX_EATAN_TUBOMI,
	TEX_EATAN_HANA,

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