#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"

class CShader2D;

class Fade
{
public:
	
	~Fade(){ Uninit(); }
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_INFINIT,
		FADE_MAX
	}FADE;

	static void Finalize(void);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(int layer = 0);
	void Start(float frame,FADE mode,const D3DXCOLOR &color = D3DXCOLOR(0,0,0,0.0f),int layer = 0);
	FADE Mode(int layer = 0){ return _Mode[layer]; }
	bool Active(int layer = 0){ return Fading[layer]; }

	static Fade* Instance(void){ return Self; }
	//static void Start(int frame,FADE mode,D3DXCOLOR color = D3DXCOLOR(0,0,0,1.0f));
private:
	Fade(){ Init(); }
	Fade(const Fade& other){};
	Fade &operator= (const Fade& other){};
	static Fade* Self;
	void SetVertex(void);
	D3DXVECTOR3 Size;
	D3DXCOLOR Color[2];
	D3DXCOLOR DestColor[2];
	D3DXCOLOR SubColor[2];
	D3DXMATRIX WorldMtx;
	D3DXVECTOR4 uv;//x=left,y=top,z=width,w=heightとして扱う
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;	//頂点バッファ
	LPDIRECT3DTEXTURE9 Texture;
	FADE _Mode[2];
	bool Fading[2];
	CShader2D* _Shader;

};

#endif