#ifndef _LOADING_H_
#define _LOADING_H_

#include "main.h"
#include "ScreenRender.h"

class Mutex;

class CLoading
{
public:

	void Init(void);
	void Uninit(void);
	void Update(void);

	static void Finalize(void);

	static CLoading* Instance(void)
	{
		if (Self == nullptr)
		{
			Self = new CLoading;
		}
		return Self; 
	}
	static void SetRenderer(CScreenRender* sr);

private:
	CLoading();
	~CLoading();
	CLoading(const CLoading& other){};
	CLoading& operator= (const CLoading& other){};

	void CastMatrix(void);

	static const int PolygonNum = 3;
	static CLoading* Self;

	LPDIRECT3DTEXTURE9 Texture;
	static CScreenRender* ScreenRender;
	Mutex* mutex;
	float frame;

	D3DXMATRIX WorldMatrix[PolygonNum];
	D3DXVECTOR3 _Pos[PolygonNum];
	float _Rot[PolygonNum];
	D3DXVECTOR3 _Size[PolygonNum];
	D3DXVECTOR4 uv[PolygonNum];
	LARGE_INTEGER sys,start,now;
};

#endif