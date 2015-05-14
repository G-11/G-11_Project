#include "Shaim3DParts.h"
#include "Texture.h"
int Shaim3DParts::Num_ = 0;
CShader3D* Shaim3DParts::_Shader = nullptr;

Shaim3DParts::Shaim3DParts()
{
	_Pos = 
	_Rot = 
	_Speed = D3DXVECTOR3(0,0,0);
	_Size = D3DXVECTOR3(1.0f,1.0f,1.0f);
	uv = D3DXVECTOR4(0,0,1.0f,1.0f);
	_Color = WHITE(1.0f);
	Texture = GetTexture(TEX_NONE);
	_Pass = CShader3D::ADD;
	Num_++;
}

Shaim3DParts::~Shaim3DParts()
{
	Num_--;
}

Shaim3DParts* Shaim3DParts::Create(const D3DXVECTOR3& pos,float size,const D3DXVECTOR3& rot)
{
	Shaim3DParts* parts = new Shaim3DParts;
	if (parts == nullptr)
	{
		return nullptr;
	}

	parts->_Pos = pos;
	parts->_Rot = rot;
	parts->_Size.x = size;
	parts->_Size.y = size;

	return parts;
}

Shaim3DParts* Shaim3DParts::Create(const D3DXVECTOR3& pos,const D3DXVECTOR2& size,const D3DXVECTOR3& rot)
{
	Shaim3DParts* parts = new Shaim3DParts;
	if (parts == nullptr)
	{
		return nullptr;
	}

	parts->_Pos = pos;
	parts->_Rot = rot;
	parts->_Size.x = size.x;
	parts->_Size.y = size.y;

	return parts;

}
void Shaim3DParts::Update(void)
{

	frame++;
	_Pos += _Speed;
}

void Shaim3DParts::Draw(void)
{
	_Shader->SetFloatArray(CShader3D::SIZE,_Size,3);
	D3DXMATRIX matrix;

	D3DXMatrixRotationYawPitchRoll(&matrix,_Rot.y,_Rot.x,_Rot.z);
	_Shader->SetMatrix(CShader3D::ROT_MTX,matrix);
	D3DXMatrixTranslation(&matrix,_Pos.x,_Pos.y,_Pos.z);
	_Shader->SetMatrix(CShader3D::POS_MTX,matrix);
	_Shader->SetFloatArray(CShader3D::DIFFUSE,_Color,4);
	_Shader->SetFloatArray(CShader3D::UV,uv,4);

	_Shader->SetTexture(Texture);
	_Shader->Draw(_Pass,D3DPT_TRIANGLESTRIP);
	
}

void Shaim3DParts::Release(void)
{
	delete this;
}

//=============================================================================
//UV値の設定
//	UNum	左から何番目か？
//	VNum	上から何番目か？
//	DivideUNum	横の分割数(最低1)
//	DivideVNum	縦の分割数(最低1);
//=============================================================================
void Shaim3DParts::SetUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
{
	//ゼロディバイド防止
	if (DivideUNum == 0)
	{
		DivideUNum = 1;
	}
	if (DivideVNum == 0)
	{
		DivideVNum = 1;
	}
	float DivideU = 1.0f / DivideUNum;
	float DivideV = 1.0f / DivideVNum;

	uv.x = DivideU*UNum;
	uv.y = DivideV*VNum;
	uv.z = DivideU;
	uv.w = DivideV;
}
