//===========================================================
//2Dポリゴンの処理
//Creater:Arai Yuhki
//===========================================================
#ifndef _SPRITE_H_
#define _SPRITE_H_
//===========================================================
//インクルード
//===========================================================
#include "main.h"

class Shader2D;
//===========================================================
//2Dポリゴンクラス
//===========================================================
class Sprite
{
protected:
	D3DXVECTOR3 _Pos;		//ポリゴンの位置
	D3DXVECTOR3 _Size;		//ポリゴンの大きさ
	D3DXVECTOR3 _Speed;		//ポリゴンの移動速度
	D3DXVECTOR3 _Rot;		//ポリゴンの角度
	D3DXVECTOR4 uv;			//x=left,y=top,z=width,w=heightとして扱う
	D3DXVECTOR4 MaskUV;
	D3DXCOLOR _Color;
	D3DXCOLOR _MaskColor;
	D3DXMATRIX WorldMtx;
	D3DXVECTOR3 _Quad[4];
	const static D3DXVECTOR3 QuadBase[4];
	int frame;			//フレームカウント
	bool ReleaseFlag;
	static int CPolygonNum;
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DTEXTURE9 Mask;
	static bool PauseFlag;
	static Shader2D* _Shader;

	void LinkList(void);		//自身をリストに追加
	void UnlinkList(void);		//自身をリストから削除
	virtual void Update(void);
	virtual void Pause(void);
	virtual void Draw(void);
	virtual void CastMatrix(void);
	int Priority;
	int _Pass;

public:

	typedef enum
	{
		LAYER_0 = 0,
		LAYER_1,
		LAYER_2,
		LAYER_3,
		LAYER_INTERFACE,
		LAYER_MAX
	}LAYER;

	Sprite(int priority = 0);
	virtual ~Sprite(){}
	static Sprite* Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color,int priority = 0);

	static void UpdateAll(void);

	static void DrawAll(int priority);

	virtual void Release(void);			//自分を削除
	static void ReleaseAll(void);

	static void Delete(int index);
	static void Clear(void);
	static void SetShader(Shader2D* shader){ _Shader = shader; }

	//Getter&Stter
	static Sprite* Top(int priority){ return Sprite::_Top[priority]; }
	static Sprite* Cur(int priority){ return Sprite::_Cur[priority]; }
	Sprite* Prev(void)const{ return _Prev; }
	Sprite* Next(void)const{ return _Next; }

	D3DXCOLOR Color(void)const{ return _Color; }
	D3DXCOLOR MaskColor(void)const{ return _MaskColor; }
	D3DXVECTOR3 Pos(void)const{return _Pos;}
	D3DXVECTOR3 Rot(void)const{return _Rot;}
	D3DXVECTOR3 Speed(void)const{return _Speed;}
	D3DXVECTOR3 Size(void)const{return _Size;}
	D3DXVECTOR4 UV(void)const{ return uv; }
	D3DXVECTOR3* Quad(void) {return _Quad;}
	static int GetCPolygonNum(void){return Sprite::CPolygonNum;}

	static void SetTop(Sprite* top,int priority=0){_Top[priority] = top;}
	static void SetCur(Sprite* cur,int priority=0){_Cur[priority] = cur;}
	static void SetPause(bool flag){PauseFlag = flag;}
	void SetPos		(const D3DXVECTOR3 &pos){ _Pos = pos; }
	void SetPosX	(const float pos){ _Pos.x = pos; }
	void SetPosY	(const float pos){ _Pos.y = pos; }
	void SetRot		(const D3DXVECTOR3 &rot){_Rot = rot;}
	void SetRot		(const float &rot){ _Rot.z = rot; }
	void SetColor	(const D3DXCOLOR &color){ _Color = color; }
	void SetMaskColor(const D3DXCOLOR& color){ _MaskColor = color; }
	void SetSize	(const D3DXVECTOR3 &size){ _Size = size; }
	void SetSpeed	(const float base,float radian);
	void SetTexture(LPDIRECT3DTEXTURE9 tex){Texture = tex;}
	void SetMask(LPDIRECT3DTEXTURE9 tex){ Mask = tex; }
	void SetUV(int UNum = 0,int VNum = 0,int DivideUNum = 1,int DivideVNum = 1);
	void SetUV(const D3DXVECTOR4& UV){ uv = UV; }
	void SetMaskUV(int UNum = 0,int VNum = 0,int DivideUNum = 1,int DivideVNum = 1);
	void SetMaskUV(const D3DXVECTOR4& UV){ MaskUV = UV; }
	void SetNext(Sprite* next){_Next = next;}
	void SetPrev(Sprite* prev){ _Prev = prev; }
	void SetRelease(void){ReleaseFlag = true;}
	void SetPass(int pass){_Pass = pass;}
	
	void AddPos		(const D3DXVECTOR3 &pos){ _Pos += pos; }
	void AddPosX	(const float pos){ _Pos.x += pos; }
	void AddPosY	(const float pos){ _Pos.y += pos; }
	void AddRot		(const D3DXVECTOR3 &rot){ _Rot += rot; }
	void AddRot		(const float &rot){ _Rot.z += rot; }
	void AddSize	(const D3DXVECTOR3 &size){ _Size += size; }
	void AddColor	(const D3DXCOLOR &color){_Color+=color;}
	void AddMaskColor(const D3DXCOLOR& color){_MaskColor += color;}	
	
	void SetRightPos(float right);
	void SetLeftPos(float left);
	void SetTopPos(float top);
	void SetBottomPos(float bottom);
	//---------------------------

private:
	Sprite* _Prev;
	Sprite* _Next;

	static Sprite* _Top[LAYER_MAX];
	static Sprite* _Cur[LAYER_MAX];

};


#endif