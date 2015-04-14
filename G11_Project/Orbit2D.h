//=============================================================================
//ポリラインエフェクト2Dクラス
//=============================================================================
#ifndef _ORBIT2D_H_
#define _ORBIT2D_H_

//=============================================================================
//インクルード
//=============================================================================
#include "main.h"

class Shader2D;
//クラス定義
class Orbit2D
{
public:
	Orbit2D();
	virtual ~Orbit2D();

	void LinkList(void);
	void UnlinkList(void);

	static Orbit2D* Top(void){ return _Top; }
	static Orbit2D* Cur(void){ return _Cur; }

	Orbit2D* Next(void){return _Next;}
	Orbit2D* Prev(void){return _Prev;}

	static void SetShader(Shader2D* shader){_Shader = shader;}
	static Orbit2D* Create(const D3DXVECTOR3& pos,int divide,float width,D3DXVECTOR3* parent = nullptr,D3DXVECTOR3* parent2 = nullptr);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);

	static int Num(void){ return _Num; }
	static void SetPause(bool flag){ _PauseFlag = flag; }

	void SetHeadPos(const D3DXVECTOR3& pos){Pos = pos;}
	void SetSpeed(const D3DXVECTOR3& speed){_Speed = speed;}
	void SetColor(const D3DXCOLOR& color){_Color = color;}
	void SetTexture(LPDIRECT3DTEXTURE9 tex){Texture = tex;}
	void SetRelease(bool flag = true){_ReleaseFlag = flag;}

	D3DXVECTOR3 HeadPos(void)const{return Pos;}
	D3DXVECTOR3 Speed(void)const{return _Speed;}
	D3DXCOLOR Color(void)const{return _Color;}
	bool ReleaseFlag(void)const{return _ReleaseFlag;}

protected:

	void Init(const D3DXVECTOR3& pos);
	virtual void Update(void);
	virtual void Pause(void);
	virtual void Draw(void);
	virtual void Release(void);
	
	static Orbit2D* _Top;
	static Orbit2D* _Cur;
	Orbit2D* _Next;
	Orbit2D* _Prev;

	static int _Num;
	static Shader2D* _Shader;
	static bool _PauseFlag;

	D3DXVECTOR3* Parent_Pos;
	D3DXVECTOR3* Parent_Pos2[2];
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 _Speed;
	D3DXCOLOR _Color;
	int DivideNum;
	float _Width;

	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	LPDIRECT3DTEXTURE9 Texture;

	bool _ReleaseFlag;
	int frame;
	int CountDown;
};

#endif