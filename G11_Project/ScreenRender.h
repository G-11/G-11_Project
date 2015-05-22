#ifndef _SCREEN_RENDER_H_
#define _SCREEN_RENDER_H_
#include "main.h"

class CScreenRender
{
public:
	enum
	{
		MODE_NORMAL = 0,
		MODE_MEGA,
		MODE_MONO,
		MODE_SEPIA,
		MODE_SKY,
		MODE_MAX
	};
	CScreenRender();
	~CScreenRender();

	virtual LPDIRECT3DTEXTURE9 RenderScreen(LPDIRECT3DTEXTURE9 tex,int draeNum = 1,bool clear = true);
	virtual LPDIRECT3DTEXTURE9 RenderWindow(LPDIRECT3DTEXTURE9 tex);
	virtual void Init(void);
	int Mode(void)const{return ColorMode_;}
	void SetMode(int mode){ColorMode_ = mode;}
	void SetMask(LPDIRECT3DTEXTURE9 tex){ Effect->SetTexture("MaskMap",tex); }
	void SetValue(float value){ Effect->SetFloat("value",value); }

protected:

	D3DXMATRIX Projection2D;
	LPD3DXEFFECT Effect;
	D3DXHANDLE DiffuseMap;
	int ColorMode_;
};

#endif