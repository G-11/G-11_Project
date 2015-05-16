//=============================================================================
//テクスチャの処理
//Creater:Arai Yuhki
//=============================================================================
//=============================================================================
//インクルード
//=============================================================================
#include "Texture.h"
#include "Mutex.h"
//=============================================================================
//グローバル変数
//=============================================================================
//ファイルネーム配列
const LPCSTR TextureFileName[] = {
	"data/texture/white.bmp",
	"data/texture/Char/player.png",
	"data/texture/Option.png",
	"data/texture/GatlingOption.png",

	"data/texture/Bullet/AquaLay.png",
	"data/texture/Bullet/Missile.png",
	"data/texture/Bullet/Valcan.png",
	"data/texture/Bullet/RailGun.png",
	"data/texture/Bullet/Pile.png",
	"data/texture/Bullet/HormingLaser.png",
	"data/texture/Bullet/WaveLaser.png",
	"data/texture/Bullet/PlayerLaser.png",
	"data/texture/Bullet/Gatling.png",
	"data/texture/Bullet/RIP_Core.png",
	"data/texture/Bullet/RIP.png",
	"data/texture/Bullet/Cucumbera.png",
	"data/texture/Bullet/HyperBlaster.png",
	"data/texture/Effect/BlasterRing.png",
	"data/texture/Effect/LaserPattern.png",
	"data/texture/Bullet/LaserBlade.png",
	"data/texture/Effect/BladeEffect.png",

	"data/texture/Bullet/BulletA.png",
	"data/texture/Bullet/BulletB.png",
	"data/texture/Bullet/BulletC.png",
	"data/texture/Bullet/BulletD.png",
	"data/texture/Bullet/BulletE.png",
	"data/texture/Bullet/BulletF.png",
	"data/texture/Bullet/BulletG.png",
	"data/texture/Bullet/BulletH.png",
	"data/texture/Bullet/BulletI.png",
	"data/texture/Bullet/BulletJ.png",
	"data/texture/Bullet/BulletK.png",
	"data/texture/Bullet/BulletL.png",
	"data/texture/Bullet/BulletM.png",
	"data/texture/Bullet/BulletN.png",
	"data/texture/Bullet/BulletO.png",
	"data/texture/Bullet/BulletP.png",
	"data/texture/Bullet/Laser.png",

	"data/texture/Enemy/FairyRed.png",
	"data/texture/Enemy/FairyBlue.png",
	"data/texture/Enemy/FairyGreen.png",
	"data/texture/Enemy/FairyYellow.png",
	"data/texture/Enemy/FairyViolet.png",
	"data/texture/Enemy/FairySunFlower.png",
	"data/texture/Enemy/Machinery.png",
	"data/texture/Enemy/SemiBoss1.png",
	"data/texture/Enemy/SemiBoss2.png",

	"data/texture/Effect/Light.png",
	"data/texture/Effect/Smoke.png",
	"data/texture/Effect/SmokeAnim.png",
	"data/texture/Effect/SlowEffect.png",
	"data/texture/Effect/MagicCircle1.png",
	"data/texture/Effect/MagicCircle2.png",
	"data/texture/Effect/Lightning.png",
	"data/texture/Effect/BunkerEx.png",
	"data/texture/Effect/KillEffect.png",
	"data/texture/Effect/HitEffect.png",
	"data/texture/Effect/ShotEffect.png",
	"data/texture/Effect/OrbitEffect.png",
	"data/texture/Effect/LaserTail.png",
	"data/texture/Effect/LaserTail2.png",
	"data/texture/Effect/Gear.png",
	"data/texture/Effect/GearSaw.png",
	"data/texture/Effect/BulletKill.png",
	"data/texture/Effect/WhirlEffect.png",
	"data/texture/Effect/Explode.png",
	"data/texture/Effect/Destroy.png",
	"data/texture/Effect/RingLight.png",
	"data/texture/Effect/RainbowEffect.png",

	"data/texture/Item.png",
	"data/texture/Nitori.png",
	"data/texture/InterfaceBG.png",
	"data/texture/Interface/Interface.png",
	"data/texture/Interface/LifeIcon.png",
	"data/texture/Interface/Number.png",
	"data/texture/Interface/Comma.png",
	"data/texture/Interface/Gauge.png",
	"data/texture/Interface/GaugePoint.png",
	"data/texture/Interface/EnemyGauge.png",
	"data/texture/Interface/PowerInterface.png",
	"data/texture/Interface/BorderLine.png",
	"data/texture/Interface/BorderLineMask.png",
	"data/texture/Interface/EnemyMarker.png",
	"data/texture/Interface/PauseItem.png",
	"data/texture/Interface/Yes_No.png",
	"data/texture/Interface/Cursor.png",
	"data/texture/Interface/CursorUnderline.png",
	"data/texture/BackGround.png",
	"data/texture/Grid.png",

	"data/texture/Char/eatan.png",
	"data/texture/Item/usagi.png",
	"data/texture/Item/neko.png",
	"data/texture/Item/kuma.png",

								};
LPDIRECT3DTEXTURE9 Texture[TEXTURE_MAX];
static D3DXIMAGE_INFO ImageInfo[TEXTURE_MAX];
//=============================================================================
//初期化
//=============================================================================
void InitTexture(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	Mutex* mutex = Mutex::Instance();
	for (int cnt = 0;cnt<TEXTURE_MAX;cnt++)
	{
		//D3DXCreateTextureFromFile(Device,TextureName[cnt],&Texture[cnt]);
		D3DXGetImageInfoFromFile(TextureFileName[cnt],&ImageInfo[cnt]);

		mutex->Enter();
		D3DXCreateTextureFromFileEx(Device,TextureFileName[cnt],ImageInfo[cnt].Width,ImageInfo[cnt].Height,
			1,D3DUSAGE_AUTOGENMIPMAP,ImageInfo[cnt].Format,D3DPOOL_MANAGED,D3DX_FILTER_TRIANGLE,D3DX_FILTER_LINEAR,0,NULL,NULL,&Texture[cnt]);
		mutex->Leave();
	}
}
//=============================================================================
//終了
//=============================================================================
void UninitTexture(void)
{
	for(int cnt=0;cnt<TEXTURE_MAX;cnt++)
	{
		if(Texture[cnt]!= NULL)
		{
			Texture[cnt]->Release();
			Texture[cnt] = NULL;
		}
	}
}
//=============================================================================
//テクスチャの取得
//=============================================================================
LPDIRECT3DTEXTURE9 GetTexture(TEX id)
{
	return Texture[id];

}

D3DXVECTOR2 GetImageSize(TEX texture)
{
	return D3DXVECTOR2((float)ImageInfo[texture].Width,(float)ImageInfo[texture].Height);
}
D3DXIMAGE_INFO GetImageInfo(TEX texture)
{
	return ImageInfo[texture];
}
