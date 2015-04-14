#ifndef _SHAIM_3D_EFFECT_H_
#define _SHAIM_3D_EFFECT_H_
#include "main.h"

class Shader3D;
class Shader2D;
class CCamera;
class Shaim3DParts;

class Shaim3DEffect
{
public:
	Shaim3DEffect();
	virtual ~Shaim3DEffect();

	void Release(void);

	static Shaim3DEffect* Create(const D3DXVECTOR3& pos,const D3DXVECTOR3& size,int partsnum=1);
	static void Initialize(void);
	static void Uninit(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);
	static int Num(void){return _Num;}

	D3DXVECTOR3 Pos(void)const{return _Pos;}
	D3DXVECTOR3 Size(void)const{return _Size;}
	D3DXVECTOR3 Speed(void)const{return _Speed;}
	
	void SetPos(const D3DXVECTOR3& pos){_Pos = pos;}
	void SetSize(const D3DXVECTOR3& size){_Size = size;}
	void SetSpeed(const D3DXVECTOR3& speed){_Speed = speed;}

	void AddPos(const D3DXVECTOR3& pos){ _Pos += pos; }
	void AddSize(const D3DXVECTOR3& size){ _Size += size; }
	void AddSpeed(const D3DXVECTOR3& speed){ _Speed += speed; }
	static void SetPause(bool flag){ PauseFlag = flag; }

protected:

	virtual void Init(const D3DXVECTOR2& size = D3DXVECTOR2(30.0f,30.0f));
	virtual void Update(void);
	virtual void Pause(void){};
	void DrawParts(void);
	void Draw(void);

	static bool PauseFlag;
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DSURFACE9 Surface;
	Shaim3DParts** Parts;
	int PartsNum;
	D3DXVECTOR3 _Pos;
	D3DXVECTOR3 _Size;
	D3DXVECTOR3 _Speed;
	D3DXMATRIX WorldMtx;

private:
	void LinkList(void);
	void UnlinkList(void);

	static int _Num;
	static Shader3D* _Shader3D;
	static Shader2D* _Shader2D;
	static CCamera* Camera;
	static Shaim3DEffect* _Top;
	static Shaim3DEffect* _Cur;
	Shaim3DEffect* _Next;
	Shaim3DEffect* _Prev;
	
};

#endif