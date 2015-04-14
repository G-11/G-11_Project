#ifndef _BLURFILTER_H_
#define _BLURFILTER_H_

#include "ScreenRender.h"

class Blur:public ScreenRender
{
public:
	void Init(void);
	static void Finalize(void);
	LPDIRECT3DTEXTURE9 Render(LPDIRECT3DTEXTURE9 tex);

	static Blur* Instance(void){ return Self; }
private:

	Blur();
	Blur(const Blur& other){};
	Blur &operator=(const Blur& other){};
	~Blur();

	static Blur* Self;

	LPDIRECT3DTEXTURE9 Texture[2];
	LPDIRECT3DSURFACE9 Surface[2];
	D3DXHANDLE DiffuseMap;

};

#endif