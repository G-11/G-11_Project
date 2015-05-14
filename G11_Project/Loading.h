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

	static CLoading* Self;

	LPDIRECT3DTEXTURE9 Texture;
	static CScreenRender* ScreenRender;
	Mutex* mutex;

	D3DXMATRIX WorldMatrix[2];
	D3DXVECTOR3 _Pos[2];
	float _Rot[2];
	D3DXVECTOR3 _Size[2];
};

#endif