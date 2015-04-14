#ifndef _SHAIM_3D_PARTS_H_
#define _SHAIM_3D_PARTS_H_
#include "main.h"
#include "Shader3D.h"

class Shaim3DParts
{
public:
	Shaim3DParts();
	virtual ~Shaim3DParts();

	static Shaim3DParts* Create(const D3DXVECTOR3& pos,float size,const D3DXVECTOR3& rot = D3DXVECTOR3(0,0,0));
	static Shaim3DParts* Create(const D3DXVECTOR3& pos,const D3DXVECTOR2& size,const D3DXVECTOR3& rot = D3DXVECTOR3(0,0,0));
	void Update(void);
	void Draw(void);
	void Release(void);

	D3DXVECTOR3 Pos(void)const{return _Pos;}
	D3DXVECTOR3 Rot(void)const{return _Rot;}
	D3DXVECTOR3 Size(void)const{ return _Size; }
	D3DXVECTOR3 Speed(void)const{ return _Speed; }
	D3DXVECTOR4 UV(void)const{ return uv; }
	D3DXCOLOR Color(void)const{ return _Color; }
	int Pass(void)const{ return _Pass; }
	static int Num(void){return _Num;}

	void SetPos(const D3DXVECTOR3& pos){ _Pos = pos; }
	void SetRot(const D3DXVECTOR3& rot){ _Rot = rot; }
	void SetSize(const D3DXVECTOR3& size){ _Size = size; }
	void SetSpeed(const D3DXVECTOR3& speed){ _Speed = speed; }
	void SetUV(const D3DXVECTOR4& UV){ uv = UV; }
	void SetUV(int UNum = 0,int VNum = 0,int DivideUNum = 1,int DivideVNum = 1);
	void SetColor(const D3DXCOLOR color){ _Color = color; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex){ Texture = tex; }
	void SetPass(Shader3D::PASS pass){ _Pass = pass; }
	static void SetShader(Shader3D* shader){ _Shader = shader; }

	void AddPos(const D3DXVECTOR3& pos)		{ _Pos += pos; }
	void AddRot(const D3DXVECTOR3& rot)		{ _Rot += rot; }
	void AddSize(const D3DXVECTOR3& size)	{ _Size += size; }
	void AddSpeed(const D3DXVECTOR3& speed)	{ _Speed += speed; }
	void AddUV(const D3DXVECTOR4& UV)		{ uv += UV; }
	void AddColor(const D3DXCOLOR color)	{ _Color += color; }

	void MultiSize(const D3DXVECTOR3& multi){ _Size.x *= multi.x; _Size.y *= multi.y; _Size.z *= multi.z; }
	void MultiSize(const float multi){_Size *= multi;}

protected:
	D3DXVECTOR3 _Pos;
	D3DXVECTOR3 _Rot;
	D3DXVECTOR3 _Size;
	D3DXVECTOR3 _Speed;
	D3DXVECTOR4 uv;
	D3DXCOLOR _Color;
	LPDIRECT3DTEXTURE9 Texture;
	Shader3D::PASS _Pass;
	int frame;
	static int _Num;
	static Shader3D* _Shader;
};


#endif