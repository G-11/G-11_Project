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

	"data/texture/Interface/numbarseat.png",
	"data/texture/Interface/CGauge.png",
	"data/texture/Interface/GaugePoint.png",
	"data/texture/Interface/Cursor_000.png",

	"data/texture/Effect/SkyMap.png",
	"data/texture/Effect/GrowUp.png",
	"data/texture/Effect/CheckPoint.png",
	"data/texture/Effect/Tail1.png",
	"data/texture/Effect/Star.png",
	"data/texture/Effect/turueffect.png",
	"data/texture/Effect/kaziru.png",

	"data/texture/Interface/gauge.png",
	"data/texture/Interface/gaugeFrame.png",
	"data/texture/Interface/clock.png",
	"data/texture/Interface/needle.png",

	"data/texture/Char/eatan.png",
	"",
	"data/texture/Item/usagi.png",
	"data/texture/Item/neko.png",
	"data/texture/Item/kuma.png",
	"data/texture/Item/buta.png",
	"data/texture/Item/usi.png",
	"data/texture/Item/sai.png",
	"data/texture/Item/taizoustump.png",

	"data/texture/Creature/taizou.png",

	"data/texture/Stage/Log_000.png",
	"data/texture/Stage/Ivy_000.png",
	"data/texture/Stage/rock.png",
	"data/texture/Stage/Block_Breakable_000.png",
	"data/texture/Stage/Block_Indestructible_000.png",
	"data/texture/Stage/kirikabu.png",

	"data/texture/Interface/botun.png",
	"data/texture/Interface/mokuhyou1.png",
	"data/texture/Interface/mokuhyou2.png",

	"data/texture/field1.jpg",
	"data/texture/BG_Options_000.png",
	"data/texture/result.jpg",
	"data/texture/gelfロゴ.png",

	"data/texture/Stage/field1.jpg",
	"data/texture/Stage/tenzyou1.png",
	"data/texture/Stage/yuka1.png",

	"data/texture/Stage/ueki.png"
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
