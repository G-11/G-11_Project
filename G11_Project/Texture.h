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
	TEX_CHAR_NITORI,
	TEX_OPTION,
	TEX_GATLING_OPTION,

	TEX_BULLET_AQUALAY,
	TEX_BULLET_MISSILE,
	TEX_BULLET_VALCAN,
	TEX_BULLET_RAILGUN,
	TEX_BULLET_PILE,
	TEX_BULLET_HLASER,
	TEX_BULLET_WLASER,
	TEX_BULLET_PLAYER_LASER,
	TEX_BULLET_GATLING,
	TEX_BULLET_RIP_CORE,
	TEX_BULLET_RIP,
	TEX_BULLET_CUCUMBERA,
	TEX_SPELL_BLASTER,
	TEX_SPELL_BLASTER_RING,
	TEX_BLASTER_PATTERN,
	TEX_LASER_BLADE,
	TEX_BLADE_EFFECT,

	TEX_BULLET_A,
	TEX_BULLET_B,
	TEX_BULLET_C,
	TEX_BULLET_D,
	TEX_BULLET_E,
	TEX_BULLET_F,
	TEX_BULLET_G,
	TEX_BULLET_H,
	TEX_BULLET_I,
	TEX_BULLET_J,
	TEX_BULLET_K,
	TEX_BULLET_L,
	TEX_BULLET_M,
	TEX_BULLET_N,
	TEX_BULLET_O,
	TEX_BULLET_P,
	TEX_BULLET_LASER,

	TEX_ENEMY_FAIRY_RED,
	TEX_ENEMY_FAIRY_BLUE,
	TEX_ENEMY_FAIRY_GREEN,
	TEX_ENEMY_FAIRY_YELLOW,
	TEX_ENEMY_FAIRY_VIOLET,
	TEX_ENEMY_FAILY_SUN,
	TEX_ENEMY_MACINERY,
	TEX_ENEMY_SEMIBOSS1,
	TEX_ENEMY_SEMIBOSS2,

	TEX_EFFECT_LIGHT,
	TEX_EFFECT_SMOKE,
	TEX_EFFECT_SMOKE_ANIME,
	TEX_EFFECT_SLOW,
	TEX_EFFECT_SPELL_MAGICCIRCLE,
	TEX_EFFECT_BOSS_MAGICCIRCLE,
	TEX_EFFECT_LIGHTNING,
	TEX_EFFECT_BUNKEREX,
	TEX_EFFECT_KILL,
	TEX_EFFECT_HIT,
	TEX_EFFECT_SHOT,
	TEX_EFFECT_ORBIT,
	TEX_EFFECT_LASERTAIL,
	TEX_EFFECT_LASERTAIL2,
	TEX_EFFECT_GEAR,
	TEX_EFFECT_GEAR_SAW,
	TEX_EFFECT_BULLET_KILL,
	TEX_EFFECT_WHIRL,
	TEX_EFFECT_EXPLODE,
	TEX_EFFECT_DESTROY,
	TEX_EFFECT_RINGLIGHT,
	TEX_EFFECT_RAINBOW,

	TEX_ITEM,
	TEX_NITORI,
	TEX_INTERFACE_BG,
	TEX_INTERFACE,
	TEX_LIFEICON,
	TEX_NUMBER,
	TEX_COMMA,
	TEX_GAGE,
	TEX_GAGE_POINT,
	TEX_GAGE_ENEMY,
	TEX_INTERFACE_POWER,
	TEX_BORDERLINE,
	TEX_BORDERMASK,
	TEX_ENEMYMARKER,
	TEX_PAUSEITEM,
	TEX_YESNO,
	TEX_CURSOR,
	TEX_UNDERLINE,
	TEX_BG,
	TEX_STAGE1_BG,
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