#ifndef _POLYGON_H_
#define _POLYGON_H_
#include "main.h"

class Shader3D;

class CPolygon
{
private:
	static CPolygon* _Top;
	static CPolygon* _Cur;
	
	void LinkList(void);
	void UnlinkList(void);

	static int CPolygon3DNum;	//É|ÉäÉSÉìÇÃëçêî
	static int CPolygon3DMax;	//çÏÇÍÇÈÉ|ÉäÉSÉìÇÃç≈ëÂêî

	void Draw(void);
	D3DXMATRIX WorldMatrix;
	
protected:
	CPolygon* _Next;
	CPolygon* _Prev;

	D3DXVECTOR3 _Pos;
	D3DXVECTOR3 _Rot;
	D3DXVECTOR3 _Speed;
	D3DXVECTOR3 _Scl;
	D3DXCOLOR _Color;
	D3DXVECTOR4 uv;//x=left,y=top,z=width,w=heightÇ∆ÇµÇƒàµÇ§
	LPDIRECT3DTEXTURE9 Texture;
	static Shader3D* _Shader;
	static bool PauseFlag;
	virtual void Update(void);
	virtual void Pause(void);

public:
	CPolygon();
	virtual ~CPolygon();

	HRESULT Init(void);
	static void Uninit(void);
	static CPolygon* Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color,const D3DXVECTOR3 &rot = D3DXVECTOR3(0,0,0));
	static void UpdateAll(void);
	static void DrawAll(void);

	static int GetCPolygon3DNum(void){return CPolygon3DNum;}

	void Release(void);
	static void ReleaseAll(void);

	static CPolygon* Top(void){ return _Top; }
	static CPolygon* Cur(void){ return _Cur; }
	CPolygon* Next(void)const{return _Next;}
	CPolygon* Prev(void)const{return _Prev;}
	D3DXVECTOR3 Pos(void)const{return _Pos;}
	D3DXVECTOR3 Size(void)const{return _Scl;}
	D3DXVECTOR3 Rot(void)const{return _Rot;}
	D3DXVECTOR3 Speed(void)const{return _Speed;}
	D3DXCOLOR Color(void)const{return _Color;}
	static CPolygon* GetCPolygon3D(int Index=0);

	static void SetTop(CPolygon* top){_Top = top;}
	static void SetCur(CPolygon* cur){_Cur = cur;}
	static void SetPause(bool flag){PauseFlag = flag;}

	void SetNext(CPolygon* next){_Next = next;}
	void SetPrev(CPolygon* prev){_Prev = prev;}
	void SetPos		(const D3DXVECTOR3 &pos)	{_Pos = pos;}
	void SetSize	(const D3DXVECTOR3 &size)	{_Scl = size;}
	void SetRot		(const D3DXVECTOR3 &rot)	{_Rot = rot;}
	void SetSpeed	(const D3DXVECTOR3 &speed)	{_Speed = speed;}
	void SetColor	(const D3DXCOLOR &color)	{_Color = color;}
	void SetUV(int UNum=0,int VNum=0,int DivideUNum=1,int DivideVNum=1);
	void SetUV(const D3DXVECTOR4& UV){ uv = UV; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex){Texture = tex;}
	//void FixedTex(TEX tex);
	
	void AddPos		(const D3DXVECTOR3 &pos)	{_Pos  += pos;}
	void AddSize	(const D3DXVECTOR3 &size)	{_Scl += size;}
	void AddRot		(const D3DXVECTOR3 &rot)	{_Rot += rot;}
	void AddSpeed	(const D3DXVECTOR3 &speed)	{_Speed += speed;}
	void AddColor	(const D3DXCOLOR &color)	{_Color += color;}
	void AddUV		(const D3DXVECTOR4& UV)		{ uv += UV;}
	void AddUV_XY	(float x,float y = 0)		{ uv.x += x; uv.y += y; }


};

#endif