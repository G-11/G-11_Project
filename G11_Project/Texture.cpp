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
	"data/texture/Gage.png",
	"data/texture/GagePoint.png",
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
