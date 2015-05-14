#pragma once
#ifndef _LIGHT2D_H_
#define _LIGHT2D_H_
#include "main.h"

class CShader2D;

//===================================================
//2Dライトクラス
// シングルトン
//===================================================

class LightScreen
{
public:
	bool Init(void);
	void Finalize(void);
	LPDIRECT3DTEXTURE9 Render(LPDIRECT3DTEXTURE9 tex);
	static LightScreen* Instance(void){ return Self; }

private:
	LightScreen();
	LightScreen(const LightScreen& other){};
	~LightScreen(){}
	LightScreen &operator= (const LightScreen& other);

	CShader2D* _Shader;
	
	LPDIRECT3DSURFACE9 Surface;
	LPDIRECT3DSURFACE9 ResultSurface;

	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DTEXTURE9 Result;

	static LightScreen* Self;

	D3DXMATRIX WorldMtx;

};

#endif