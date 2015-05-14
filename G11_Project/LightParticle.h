//===========================================================
// ライトパーティクル
//Creater:Arai Yuhki
//===========================================================
#ifndef _LIGHT_PARTICLE_H_
#define _LIGHT_PARTICLE_H_
//===========================================================
//インクルード
//===========================================================
#include "main.h"

class CShader2D;
//===========================================================
//2Dポリゴンクラス
//===========================================================
class LightParticle
{
protected:
	D3DXVECTOR3 _Pos;		//ポリゴンの位置
	D3DXVECTOR3 _Size;		//ポリゴンの大きさ
	D3DXVECTOR3 _Speed;		//ポリゴンの移動速度
	D3DXVECTOR3 _Rot;		//ポリゴンの角度
	D3DXVECTOR4 uv;			//x=left,y=top,z=width,w=heightとして扱う
	D3DXCOLOR _Color;
	D3DXMATRIX WorldMtx;
	int frame;			//フレームカウント
	bool ReleaseFlag;
	static int _Num;
	LPDIRECT3DTEXTURE9 Texture;
	static bool PauseFlag;
	static CShader2D* _Shader;

	void LinkList(void);		//自身をリストに追加
	void UnlinkList(void);		//自身をリストから削除
	virtual void Update(void);
	virtual void Pause(void);
	virtual void Draw(void);
	int Priority;

public:

	LightParticle();
	virtual ~LightParticle(){}
	static LightParticle* Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color);

	static void UpdateAll(void);

	static void DrawAll(void);

	void Release(void);			//自分を削除
	static void ReleaseAll(void);

	static void Clear(void);


	//Getter&Stter
	static LightParticle* Top(void){ return LightParticle::Top_; }
	static LightParticle* Cur(void){ return LightParticle::Cur_; }
	LightParticle* Prev(void)const{ return Prev_; }
	LightParticle* Next(void)const{ return Next_; }

	D3DXCOLOR Color(void)const{ return _Color; }
	D3DXVECTOR3 Pos(void)const{ return _Pos; }
	D3DXVECTOR3 Rot(void)const{ return _Rot; }
	D3DXVECTOR3 Speed(void)const{ return _Speed; }
	D3DXVECTOR3 Size(void)const{ return _Size; }
	static int Num(void){ return _Num; }

	static void SetTop(LightParticle* top){ Top_ = top; }
	static void SetCur(LightParticle* cur){ Cur_ = cur; }
	static void SetPause(bool flag){ PauseFlag = flag; }
	void SetPos(const D3DXVECTOR3 &pos){ _Pos = pos; }
	void SetRot(const D3DXVECTOR3 &rot){ _Rot = rot; }
	void SetRot(const float &rot){ _Rot.z = rot; }
	void SetColor(const D3DXCOLOR &color){ _Color = color; }
	void SetSize(const D3DXVECTOR3 &size){ _Size = size; }
	void SetSpeed(const float base,float radian);
	void SetTexture(LPDIRECT3DTEXTURE9 tex){ Texture = tex; }
	void SetUV(int UNum = 0,int VNum = 0,int DivideUNum = 1,int DivideVNum = 1);
	void SetNext(LightParticle* next){ Next_ = next; }
	void SetPrev(LightParticle* prev){ Prev_ = prev; }
	void SetRelease(void){ ReleaseFlag = true; }

	void AddPos(const D3DXVECTOR3 &pos){ _Pos += pos; }
	void AddRot(const D3DXVECTOR3 &rot){ _Rot += rot; }
	void AddRot(const float &rot){ _Rot.z += rot; }
	void AddSize(const D3DXVECTOR3 &size){ _Size += size; }
	void AddColor(const D3DXCOLOR &color){ _Color += color; }

	//---------------------------

private:
	LightParticle* Prev_;
	LightParticle* Next_;

	static LightParticle* Top_;
	static LightParticle* Cur_;




};


#endif