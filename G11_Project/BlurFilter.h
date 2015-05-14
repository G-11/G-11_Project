#ifndef _BLURFILTER_H_
#define _BLURFILTER_H_

#include "ScreenRender.h"
//===================================================
//ブラーフィルタークラス
// シングルトン
//===================================================
class CBlurFilter:public CScreenRender
{
public:
	void Init(void);
	static void Finalize(void);
	LPDIRECT3DTEXTURE9 Render(LPDIRECT3DTEXTURE9 tex);

	static CBlurFilter* Instance(void){ return Self; }

private:

	CBlurFilter();
	CBlurFilter(const CBlurFilter& other){};
	CBlurFilter &operator=(const CBlurFilter& other){};
	~CBlurFilter();

	static CBlurFilter* Self;

	LPDIRECT3DTEXTURE9 Texture[2];
	LPDIRECT3DSURFACE9 Surface[2];
	D3DXHANDLE DiffuseMap;

};

#endif