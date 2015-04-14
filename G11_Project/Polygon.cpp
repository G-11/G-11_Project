#include "Polygon.h"
#include "Renderer.h"
#include "Shader3D.h"

CPolygon* CPolygon::_Top = nullptr;
CPolygon* CPolygon::_Cur = nullptr;
int CPolygon::CPolygon3DNum = 0;
int CPolygon::CPolygon3DMax = 0;
bool CPolygon::PauseFlag = false;
Shader3D* CPolygon::_Shader = nullptr;


CPolygon::CPolygon()
{
	_Pos = D3DXVECTOR3(0,0,0);
	_Scl = D3DXVECTOR3(0,0,0);
	_Speed = D3DXVECTOR3(0,0,0);
	_Rot = D3DXVECTOR3(0,0,0);
	_Scl = D3DXVECTOR3(0,0,0);
	LinkList();
}
CPolygon::~CPolygon()
{
	CPolygon3DNum--;
}
//=============================================================================
//あらかじめ用意するポリゴン数の設定(四角ベース)
//=============================================================================
HRESULT CPolygon::Init(void)
{
	return S_OK;
}

void CPolygon::Uninit(void)
{
	ReleaseAll();
	
}

void CPolygon::Release(void)
{
	UnlinkList();
	_Pos = D3DXVECTOR3(0,0,0);
	_Scl = D3DXVECTOR3(0,0,0);
	_Speed = D3DXVECTOR3(0,0,0);
	_Rot = D3DXVECTOR3(0,0,0);
	_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	delete this;
}

void CPolygon::ReleaseAll(void)
{
	CPolygon* Polygon = _Top;
	CPolygon* Next = NULL;
	while(Polygon)
	{
		Next = Polygon->_Next;
		Polygon->Release();
		Polygon = Next;
	}
}

//=============================================================================
//自身をリストに追加
//=============================================================================
void CPolygon::LinkList(void)
{
	if(_Top != NULL)//二つ目以降の処理
	{
		CPolygon* CPolygon = _Cur;
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
	CPolygon3DNum++;
}
//=============================================================================
//自身をリストから削除
//=============================================================================
void CPolygon::UnlinkList(void)
{
	if(_Prev == NULL)//先頭
	{
		if(_Next != NULL)//次がある
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
	else if(_Next == NULL)//終端
	{
		if(_Prev != NULL)//前がある
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

}
//=============================================================================
//作成
//	D3DXVECTOR3 pos	作成位置(ポリゴンの中心)
//	D3DXVECTOR3 size 大きさ(xzのみ影響)
//	D3DXVECTOR3 rot 傾き	ラジアン値
//	D3DXCOLOR	color ポリゴンの色
//確保済みのバッファを越えなければ作成され、作られたポリゴンのポインタが返る
//オーバーした場合はNULLが返る
//=============================================================================
CPolygon* CPolygon::Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color,const D3DXVECTOR3 &rot)
{

	CPolygon* Polygon = new CPolygon;

	Polygon->_Pos = pos;
	Polygon->_Rot = rot;
	Polygon->_Color = color;
	Polygon->_Scl = D3DXVECTOR3(size.x,size.y,100.0f);
	Polygon->uv.x	= 0.0f;
	Polygon->uv.y	= 0.0f;
	Polygon->uv.z	= 1.0f;
	Polygon->uv.w	= 1.0f;
	Polygon->Texture = GetTexture(TEX_NONE);


	return Polygon;
}
//=============================================================================
//更新
//=============================================================================
void CPolygon::Update(void)
{

}
void CPolygon::Pause(void)
{

}
//=============================================================================
//全て更新
//=============================================================================
void CPolygon::UpdateAll(void)
{
	CPolygon* Polygon = _Top;
	CPolygon* Next = NULL;
	
	while(Polygon)
	{
		Next = Polygon->_Next;
		if(PauseFlag)
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
//描画
//=============================================================================
void CPolygon::Draw(void)
{
	D3DXMATRIX MtxScl,MtxRot,MtxTrans;

	_Shader->SetFloatArray(Shader3D::SIZE,_Scl,3);
	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&MtxRot,_Rot.y,_Rot.x,_Rot.z);
	_Shader->SetMatrix(Shader3D::ROT_MTX,MtxRot);
	//位置を反映
	D3DXMatrixTranslation(&MtxTrans,_Pos.x,_Pos.y,_Pos.z);
	_Shader->SetMatrix(Shader3D::POS_MTX,MtxTrans);
	_Shader->SetFloatArray(Shader3D::DIFFUSE,_Color,4);
	_Shader->SetFloatArray(Shader3D::UV,uv,4);
	//ワールドマトリックスの適用
	_Shader->SetTexture(Texture);

	_Shader->Draw(Shader3D::NORMAL,D3DPT_TRIANGLESTRIP);
}
//=============================================================================
//全て描画
//=============================================================================
void CPolygon::DrawAll(void)
{
	CPolygon* Polygon = _Top;
	CPolygon* Next = NULL;
	if (_Shader == nullptr){ _Shader = Shader3D::Instance(); }

	_Shader->DrawBegin();
	while(Polygon)
	{
		Next = Polygon->_Next;
		Polygon->Draw();
		Polygon = Next;
		
	}
	_Shader->DrawEnd();
}
//=============================================================================
//Index番目のポリゴンのポインタを取得する
//=============================================================================
CPolygon* CPolygon::GetCPolygon3D(int Index)
{
	CPolygon* Polygon = _Top;
	CPolygon* Next = NULL;
	int num=0;
	while(Polygon)
	{
		Next = Polygon->_Next;
		if(num==Index)
		{
			return Polygon;
		}
		Polygon = Next;
		num++;
	}
	return NULL;
}

//=============================================================================
//UV値の設定
//	UNum	左から何番目か？
//	VNum	上から何番目か？
//	DivideUNum	横の分割数(最低1)
//	DivideVNum	縦の分割数(最低1);
//=============================================================================
void CPolygon::SetUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
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
