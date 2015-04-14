#ifndef _LOADING_H_
#define _LOADING_H_

#include "main.h"
#include "ScreenRender.h"

class Mutex;

class Loading
{
public:

	void Init(void);
	void Uninit(void);
	void Update(void);

	static void Finalize(void);

	static Loading* Instance(void){ return Self; }
	static void SetRenderer(ScreenRender* sr);

private:
	Loading();
	~Loading();
	Loading(const Loading& other){};
	Loading& operator= (const Loading& other){};

	void CastMatrix(void);

	static Loading* Self;

	LPDIRECT3DTEXTURE9 Texture;
	static ScreenRender* _ScreenRender;
	Mutex* mutex;

	D3DXMATRIX WorldMatrix[2];
	D3DXVECTOR3 _Pos[2];
	float _Rot[2];
	D3DXVECTOR3 _Size[2];
};

#endif