#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"

class Shader2D;

class CFade
{
public:
	
	~CFade(){ Uninit(); }
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
	void Draw(void);
	void Start(float frame,FADE mode,const D3DXCOLOR &color = D3DXCOLOR(0,0,0,0.0f));
	FADE Mode(void){ return _Mode; }
	bool Active(void){ return Fading; }

	static CFade* Instance(void){return Self;}
	//static void Start(int frame,FADE mode,D3DXCOLOR color = D3DXCOLOR(0,0,0,1.0f));
private:
	CFade(){ Init(); }
	CFade(const CFade& other){};
	CFade &operator= (const CFade& other){};
	static CFade* Self;
	void SetVertex(void);
	D3DXVECTOR3 Size;
	D3DXCOLOR Color;
	D3DXCOLOR DestColor;
	D3DXCOLOR SubColor;
	D3DXMATRIX WorldMtx;
	D3DXVECTOR4 uv;//x=left,y=top,z=width,w=heightとして扱う
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;	//頂点バッファ
	LPDIRECT3DTEXTURE9 Texture;
	FADE _Mode;
	bool Fading;
	Shader2D* _Shader;

};

#endif