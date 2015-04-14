//=============================================================================
//インクルード
//=============================================================================
#include "ShimmerParticle2D.h"
#include "Renderer.h"
#include "Shader2D.h"
//=============================================================================
//グローバル変数
//=============================================================================
int ShimmerParticle2D::_Num = 0;
ShimmerParticle2D* ShimmerParticle2D::_Top = NULL;
ShimmerParticle2D* ShimmerParticle2D::_Cur = NULL;
bool ShimmerParticle2D::PauseFlag = false;
Shader2D* ShimmerParticle2D::_Shader = nullptr;
//=============================================================================
//コンストラクタ
//=============================================================================
ShimmerParticle2D::ShimmerParticle2D(void)
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
void ShimmerParticle2D::LinkList(void)
{
	if (_Top != NULL)//二つ目以降の処理
	{
		ShimmerParticle2D* CPolygon = _Cur;
		CPolygon->_Next = this;
		_Prev = CPolygon;
		_Next = NULL;
		_Cur = this;
	}
	else//最初の一つの時の処理
	{
		_Top = this;
		_Cur = this;
		_Prev = NULL;
		_Next = NULL;
	}
}
//=============================================================================
//自身をリストから削除
//=============================================================================
void ShimmerParticle2D::UnlinkList(void)
{
	if (_Prev == NULL)//先頭
	{
		if (_Next != NULL)//次がある
		{
			_Next->_Prev = NULL;
			_Top = _Next;
		}
		else//最後の一つだった
		{
			_Top = NULL;
			_Cur = NULL;
		}
	}
	else if (_Next == NULL)//終端
	{
		if (_Prev != NULL)//前がある
		{
			_Prev->_Next = NULL;
			_Cur = _Prev;
		}
		else//最後の一つだった
		{
			_Top = NULL;
			_Cur = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		_Prev->_Next = _Next;
		_Next->_Prev = _Prev;
	}

	_Prev = NULL;
	_Next = NULL;
	_Num--;
}
//=============================================================================
//自殺関数
//=============================================================================
void ShimmerParticle2D::Release(void)
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
void ShimmerParticle2D::ReleaseAll(void)
{
	ShimmerParticle2D* CPolygon = _Top;
	ShimmerParticle2D* CPolygonNext;
	while (CPolygon)
	{
		CPolygonNext = CPolygon->_Next;
		CPolygon->Release();
		CPolygon = CPolygonNext;
	}
	ShimmerParticle2D::Clear();

}
//=============================================================================
//先頭と終端をNULLに
//=============================================================================
void ShimmerParticle2D::Clear(void)
{
	_Top = nullptr;
	_Cur = nullptr;
}

//=============================================================================
//ポリゴンを作成
//=============================================================================
ShimmerParticle2D* ShimmerParticle2D::Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color)
{
	ShimmerParticle2D* CPolygon = new ShimmerParticle2D;
	CPolygon->_Pos = pos;
	CPolygon->_Size = D3DXVECTOR3(size.x,size.y,0);
	CPolygon->_Color = color;

	return CPolygon;
}
//=============================================================================
//更新
//=============================================================================
void ShimmerParticle2D::Update(void)
{
	if (ReleaseFlag)
	{
		Release();
		return;
	}

	_Pos += _Speed;

}
//=============================================================================
//ポーズ処理
//=============================================================================
void ShimmerParticle2D::Pause(void)
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
void ShimmerParticle2D::UpdateAll(void)
{
	ShimmerParticle2D* CPolygon = _Top;
	ShimmerParticle2D* Next = NULL;
	while (CPolygon)
	{
		Next = CPolygon->_Next;
		if (PauseFlag)
		{
			CPolygon->Pause();
		}
		else
		{
			CPolygon->Update();
		}

		CPolygon = Next;
	}
}
//=============================================================================
//ポリゴンを描画
//=============================================================================
void ShimmerParticle2D::Draw(void)
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

		_Shader->SetMatrix(Shader2D::WORLD_MTX,WorldMtx);
		/*
		_Shader->SetFloatArray(Shader2D::SIZE,_Size,3);
		_Shader->SetMatrix(Shader2D::ROT_MTX,MtxRot);
		_Shader->SetMatrix(Shader2D::POS_MTX,MtxTrans);
		*/

		_Shader->SetFloatArray(Shader2D::DIFFUSE,_Color,4);
		_Shader->SetFloatArray(Shader2D::UV,uv,4);
		//テクスチャの設定
		_Shader->SetTexture(Texture);

		//ポリゴンを描画
		_Shader->Draw(Shader2D::ADD,D3DPT_TRIANGLESTRIP);
	}
}
//=============================================================================
//存在してるポリゴンをすべて描画
//=============================================================================
void ShimmerParticle2D::DrawAll(void)
{
	if (_Shader == nullptr){ _Shader = Shader2D::Instance(); }

	ShimmerParticle2D* CPolygon = _Top;

	_Shader->DrawBegin();
	while (CPolygon)
	{
		CPolygon->Draw();
		CPolygon = CPolygon->_Next;
	}

	_Shader->DrawEnd();
}

void ShimmerParticle2D::SetSpeed(const float base,float radian)
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
void ShimmerParticle2D::SetUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
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
