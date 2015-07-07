//===========================================================
//2Dポリゴンの処理
//Creater:Arai Yuhki
//===========================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_
//===========================================================
//インクルード
//===========================================================
#include "main.h"

class CShader2D;
class Sprite;
//===========================================================
//2Dポリゴンクラス
//===========================================================
class Sprite
{
protected:
	D3DXVECTOR3 _Pos;		//ポリゴンの位置
	D3DXVECTOR2 _Offset;	//中心点のオフセット(xが0.5fだと、左端が中心になる)
	D3DXVECTOR3 _Size;		//ポリゴンの大きさ
	D3DXVECTOR3 _Speed;		//ポリゴンの移動速度
	D3DXVECTOR3 _Rot;		//ポリゴンの角度
	D3DXVECTOR4 uv;			//x=left,y=top,z=width,w=heightとして扱う
	D3DXVECTOR4 MaskUV;
	D3DXCOLOR _Color;
	D3DXCOLOR _MaskColor;
	D3DXMATRIX WorldMtx;
	D3DXVECTOR3 _Quad[4];
	D3DXVECTOR3 _LocalQuadBase[4];
	const static D3DXVECTOR3 QuadBase[4];
	int frame;			//フレームカウント
	bool ReleaseFlag;
	static int PolygonNum;
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DTEXTURE9 Mask;
	static bool PauseFlag;
	static CShader2D* _Shader;
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
		LAYER_BACKGROUND = 0,
		LAYER_0,
		LAYER_1,
		LAYER_2,
		LAYER_3,
		LAYER_4,
		LAYER_5,
		LAYER_INTERFACE,
		LAYER_MAX
	}LAYER;

	Sprite(int priority = 0);
	virtual ~Sprite(){}
	static Sprite* Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color,int priority = LAYER_0);

	static void UpdateAll(void);

	static void DrawAll(int priority);

	virtual void Release(void);			//自分を削除
	static void ReleaseAll(void);

	static void Delete(int index);
	static void Clear(void);
	static void SetShader(CShader2D* shader){ _Shader = shader; }

	//Getter&Stter
	static Sprite* Top(int priority){ return Sprite::Top_[priority]; }
	static Sprite* Cur(int priority){ return Sprite::Cur_[priority]; }
	Sprite* Prev(void)const{ return Prev_; }
	Sprite* Next(void)const{ return Next_; }

	D3DXCOLOR Color(void)const		{ return _Color; }
	D3DXCOLOR MaskColor(void)const	{ return _MaskColor; }
	D3DXVECTOR3 Pos(void)const		{ return _Pos; }
	D3DXVECTOR2 Offset(void)const	{ return _Offset; }
	D3DXVECTOR3 Rot(void)const		{ return _Rot; }
	D3DXVECTOR3 Speed(void)const	{ return _Speed; }
	D3DXVECTOR3 Size(void)const		{ return _Size; }
	D3DXVECTOR4 UV(void)const		{ return uv; }
	D3DXVECTOR3* Quad(void)			{ return _Quad; }
	D3DXVECTOR3* LocalQuadBase(void)	{ return _LocalQuadBase; }
	static int Num(void){return Sprite::PolygonNum;}

	static void SetTop(Sprite* top,int priority=0){Top_[priority] = top;}
	static void SetCur(Sprite* cur,int priority=0){Cur_[priority] = cur;}
	static void SetPause(bool flag){PauseFlag = flag;}
	
	void SetPos		(const D3DXVECTOR3 &pos){ _Pos = pos; }
	void SetPosX	(const float pos){ _Pos.x = pos; }
	void SetPosY	(const float pos){ _Pos.y = pos; }

	void SetSpeed(const D3DXVECTOR3 &pos){ _Speed = pos; }
	void SetSpeedX(const float pos){ _Speed.x = pos; }
	void SetSpeedY(const float pos){ _Speed.y = pos; }
	
	void SetOffset	(const D3DXVECTOR2& offset){ _Offset = offset; }
	void SetOffsetX	(const float x){ _Offset.x = x; }
	void SetOffsetY	(const float y){ _Offset.y = y; }
	void SetRot		(const D3DXVECTOR3 &rot){_Rot = rot;}
	void SetRot		(const float &rot){ _Rot.z = rot; }
	
	void SetColor	(const D3DXCOLOR &color){ _Color = color; }
	void SetRed		(float red){ _Color.r = red; }
	void SetGreen	(float green){ _Color.g = green; }
	void SetBlue	(float blue){ _Color.b = blue; }
	void SetAlpha	(float alpha){ _Color.a = alpha; }
	
	void SetMaskColor(const D3DXCOLOR& color){ _MaskColor = color; }
	void SetMaskRed		(float red){ _MaskColor.r = red; }
	void SetMaskGreen	(float green){ _MaskColor.g = green; }
	void SetMaskBlue	(float blue){ _MaskColor.b = blue; }
	void SetMaskAlpha	(float alpha){ _MaskColor.a = alpha; }
	
	void SetSize	(const D3DXVECTOR3 &size){ _Size = size; }
	void SetSizeX	(float x){ _Size.x = x; }
	void SetSizeY	(float y){ _Size.y = y; }
	void SetSpeed	(const float base,float radian);
	
	void SetTexture(LPDIRECT3DTEXTURE9 tex){Texture = tex;}
	void SetMask(LPDIRECT3DTEXTURE9 tex){ Mask = tex; }
	
	void SetUV(int UNum = 0,int VNum = 0,int DivideUNum = 1,int DivideVNum = 1);
	void SetUV(const D3DXVECTOR4& UV){ uv = UV; }
	void SetUVX(float x){ uv.x = x; }
	void SetUVY(float y){ uv.y = y; }
	void SetUVWidth(float width){ uv.z = width; }
	void SetUVHeight(float height){ uv.w = height; }

	void SetMaskUV(int UNum = 0,int VNum = 0,int DivideUNum = 1,int DivideVNum = 1);
	void SetMaskUV(const D3DXVECTOR4& UV){ MaskUV = UV; }
	void SetMaskUVX(float x){ MaskUV.x = x; }
	void SetMaskUVY(float y){ MaskUV.y = y; }
	void SetMaskUVWidth(float width){ MaskUV.z = width; }
	void SetMaskUVHeight(float height){ MaskUV.w = height; }

	void SetLocalQuadBase(D3DXVECTOR3 quad[]){
		for (int cnt = 0;cnt < 4;cnt++)
		{
			_LocalQuadBase[cnt] = quad[cnt];
		}
	}
	
	void SetNext(Sprite* next){Next_ = next;}
	void SetPrev(Sprite* prev){ Prev_ = prev; }
	
	void SetRelease(void){ReleaseFlag = true;}
	void SetPass(int pass){_Pass = pass;}
	
	void AddPos		(const D3DXVECTOR3 &pos){ _Pos += pos; }
	void AddPosX	(const float pos){ _Pos.x += pos; }
	void AddPosY	(const float pos){ _Pos.y += pos; }

	void AddSpeed(const D3DXVECTOR3 &pos){ _Speed += pos; }
	void AddSpeedX(const float pos){ _Speed.x += pos; }
	void AddSpeedY(const float pos){ _Speed.y += pos; }
	
	void AddOffset	(const D3DXVECTOR2& offset){ _Offset += offset; }
	void AddOffsetX	(const float x){ _Offset.x += x; }
	void AddOffsetY	(const float y){ _Offset.y += y; }
	
	void AddRot		(const D3DXVECTOR3 &rot){ _Rot += rot; }
	void AddRot		(const float &rot){ _Rot.z += rot; }
	
	void AddSize	(const D3DXVECTOR3 &size){ _Size += size; }
	void AddSizeX	(float x){ _Size.x += x; }
	void AddSizeY	(float y){ _Size.y += y; }
	
	void AddColor	(const D3DXCOLOR &color){_Color+=color;}
	void AddRed		(float red){ _Color.r += red; }
	void AddGreen	(float green){ _Color.g += green; }
	void AddBlue	(float blue){ _Color.b += blue; }
	void AddAlpha	(float alpha){ _Color.a += alpha; }

	void AddUV(const D3DXVECTOR4& UV){ uv += uv; }
	void AddUVX(float x){ uv.x += x; }
	void AddUVY(float y){ uv.y += y; }
	void AddUVWidth(float width){ uv.z += width; }
	void AddUVHeight(float height){ uv.w += height; }

	void AddMaskColor	(const D3DXCOLOR& color){_MaskColor += color;}	
	void AddMaskRed		(float red){ _MaskColor.r += red; }
	void AddMaskGreen	(float green){ _MaskColor.g += green; }
	void AddMaskBlue	(float blue){ _MaskColor.b += blue; }
	void AddMaskAlpha	(float alpha){ _MaskColor.a += alpha; }

	void AddMaskUVX(float x){ MaskUV.x += x; }
	void AddMaskUVY(float y){ MaskUV.y += y; }
	void AddMaskUVWidth(float width){ MaskUV.z += width; }
	void AddMaskUVHeight(float height){ MaskUV.w += height; }
	//---------------------------

private:
	Sprite* Prev_;
	Sprite* Next_;

	static Sprite* Top_[LAYER_MAX];
	static Sprite* Cur_[LAYER_MAX];

#ifdef _DEBUG
	static int DrawNum;
#endif
};


#endif