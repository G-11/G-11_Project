#ifndef _SCREEN_RENDER_H_
#define _SCREEN_RENDER_H_
#include "main.h"

class ScreenRender
{
public:
	ScreenRender();
	~ScreenRender();

	virtual LPDIRECT3DTEXTURE9 RenderScreen(LPDIRECT3DTEXTURE9 tex,int draeNum = 1,bool clear = true);
	virtual LPDIRECT3DTEXTURE9 RenderWindow(LPDIRECT3DTEXTURE9 tex);
	virtual void Init(void);
	int Mode(void)const{return ColorMode_;}
	void SetMode(int mode){ColorMode_ = mode;}

protected:

	D3DXMATRIX Projection2D;
	LPD3DXEFFECT Effect;
	D3DXHANDLE DiffuseMap;
	int ColorMode_;
};

#endif