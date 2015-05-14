//=============================================================================
//インクルード
//=============================================================================
#include "LightParticle.h"
#include "Renderer.h"
#include "Shader2D.h"
//=============================================================================
//グローバル変数
//=============================================================================
int LightParticle::_Num = 0;
LightParticle* LightParticle::Top_ = NULL;
LightParticle* LightParticle::Cur_ = NULL;
bool LightParticle::PauseFlag = false;
CShader2D* LightParticle::_Shader = nullptr;
//=============================================================================
//コンストラクタ
//=============================================================================
LightParticle::LightParticle(void)
{

	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	//バッファの作成

	//ステータスの初期化
	_Pos = D3DXVECTOR3(0,0,0);
	_Size = D3DXVECTOR3(1.0f,1.0f,1.0f);
	_Speed = D3DXVECTOR3(0,0,0);
	_Rot = D3DXVECTOR3(0,0,0);
	uv = D3DXVECTOR4(0,0,1.0f,1.0f);
	Texture = GetTexture(TEX_NONE);
	ReleaseFlag = false;
	frame = 0;
	_Num++;
	LinkList();
}
//=============================================================================
//自身をリストに追加
//=============================================================================
void LightParticle::LinkList(void)
{
	if (Top_ != NULL)//二つ目以降の処理
	{
		LightParticle* Polygon = Cur_;
		Polygon->Next_ = this;
		Prev_ = Polygon;
		Next_ = NULL;
		Cur_ = this;
	}
	else//最初の一つの時の処理
	{
		Top_ = this;
		Cur_ = this;
		Prev_ = NULL;
		Next_ = NULL;
	}
}
//=============================================================================
//自身をリストから削除
//=============================================================================
void LightParticle::UnlinkList(void)
{
	if (Prev_ == NULL)//先頭
	{
		if (Next_ != NULL)//次がある
		{
			Next_->Prev_ = NULL;
			Top_ = Next_;
		}
		else//最後の一つだった
		{
			Top_ = NULL;
			Cur_ = NULL;
		}
	}
	else if (Next_ == NULL)//終端
	{
		if (Prev_ != NULL)//前がある
		{
			Prev_->Next_ = NULL;
			Cur_ = Prev_;
		}
		else//最後の一つだった
		{
			Top_ = NULL;
			Cur_ = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		Prev_->Next_ = Next_;
		Next_->Prev_ = Prev_;
	}

	Prev_ = NULL;
	Next_ = NULL;
	_Num--;
}
//=============================================================================
//自殺関数
//=============================================================================
void LightParticle::Release(void)
{
	UnlinkList();
	_Pos = D3DXVECTOR3(0,0,0);
	_Size = D3DXVECTOR3(0,0,0);
	_Speed = D3DXVECTOR3(0,0,0);
	_Rot = D3DXVECTOR3(0,0,0);
	frame = 0;

	delete this;

}
//=============================================================================
//全消去
//=============================================================================
void LightParticle::ReleaseAll(void)
{
	LightParticle* Polygon = Top_;
	LightParticle* PolygonNext;
	while (Polygon)
	{
		PolygonNext = Polygon->Next_;
		Polygon->Release();
		Polygon = PolygonNext;
	}
	LightParticle::Clear();

}
//=============================================================================
//先頭と終端をNULLに
//=============================================================================
void LightParticle::Clear(void)
{
	Top_ = nullptr;
	Cur_ = nullptr;
}

//=============================================================================
//ポリゴンを作成
//=============================================================================
LightParticle* LightParticle::Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color)
{
	LightParticle* Polygon = new LightParticle;
	Polygon->_Pos = pos;
	Polygon->_Size = D3DXVECTOR3(size.x,size.y,0);
	Polygon->_Color = color;

	return Polygon;
}
//=============================================================================
//更新
//=============================================================================
void LightParticle::Update(void)
{
	if (ReleaseFlag)
	{
		Release();
		return;
	}

	_Pos.y = sinf(DEG2RAD(frame))*300.0f + SCREEN_HEIGHT/2.0f;
	frame++;
	frame %= 360;
	_Pos += _Speed;

}
//=============================================================================
//ポーズ処理
//=============================================================================
void LightParticle::Pause(void)
{
	if (ReleaseFlag)
	{
		Release();
		return;
	}

}
//=============================================================================
//全体更新
//=============================================================================
void LightParticle::UpdateAll(void)
{
	LightParticle* Polygon = Top_;
	LightParticle* Next = NULL;
	while (Polygon)
	{
		Next = Polygon->Next_;
		if (PauseFlag)
		{
			Polygon->Pause();
		}
		else
		{
			Polygon->Update();
		}

		Polygon = Next;
	}
}
//=============================================================================
//ポリゴンを描画
//=============================================================================
void LightParticle::Draw(void)
{
	if (_Color.a > 0.01f)
	{
		D3DXMATRIX MtxScl,MtxRot,MtxTrans;
		WorldMtx = _Shader->Identity();


		D3DXMatrixScaling(&MtxScl,_Size.x,_Size.y,_Size.z);
		D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxScl);
		//回転を反映
		D3DXMatrixRotationYawPitchRoll(&MtxRot,_Rot.y,_Rot.x,_Rot.z);
		D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxRot);


		//位置を反映
		D3DXMatrixTranslation(&MtxTrans,_Pos.x,_Pos.y,_Pos.z);
		D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxTrans);

		_Shader->SetMatrix(CShader2D::WORLD_MTX,WorldMtx);
		/*
		_Shader->SetFloatArray(CShader2D::SIZE,_Size,3);
		_Shader->SetMatrix(CShader2D::ROT_MTX,MtxRot);
		_Shader->SetMatrix(CShader2D::POS_MTX,MtxTrans);
		*/

		_Shader->SetFloatArray(CShader2D::DIFFUSE,_Color,4);
		_Shader->SetFloatArray(CShader2D::UV,uv,4);
		//テクスチャの設定
		_Shader->SetTexture(Texture);

		//ポリゴンを描画
		_Shader->Draw(CShader2D::ADD,D3DPT_TRIANGLESTRIP);
	}
}
//=============================================================================
//存在してるポリゴンをすべて描画
//=============================================================================
void LightParticle::DrawAll(void)
{
	if (_Shader == nullptr){ _Shader = CShader2D::Instance(); }

	LightParticle* Polygon = Top_;

	_Shader->DrawBegin();
	while (Polygon)
	{
		Polygon->Draw();
		Polygon = Polygon->Next_;
	}

	_Shader->DrawEnd();
}

void LightParticle::SetSpeed(const float base,float radian)
{
	_Speed.x = Sin_Rad(-radian)*base;
	_Speed.y = Cos_Rad(-radian)*base;
}

//=============================================================================
//UV値の設定
//	UNum	左から何番目か？
//	VNum	上から何番目か？
//	DivideUNum	横の分割数(最低1)
//	DivideVNum	縦の分割数(最低1);
//=============================================================================
void LightParticle::SetUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
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
