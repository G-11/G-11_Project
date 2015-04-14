//=============================================================================
//インクルード
//=============================================================================
#include "Sprite.h"
#include "Renderer.h"
#include "Shader2D.h"
//=============================================================================
//グローバル変数
//=============================================================================
int Sprite::CPolygonNum=0;
Sprite* Sprite::_Top[LAYER_MAX] = {NULL};
Sprite* Sprite::_Cur[LAYER_MAX] = {NULL};
bool Sprite::PauseFlag = false;
Shader2D* Sprite::_Shader = nullptr;

const D3DXVECTOR3 Sprite::QuadBase[4] = {
	D3DXVECTOR3(-0.5f,-0.5f,0),
	D3DXVECTOR3(0.5f,-0.5f,0),
	D3DXVECTOR3(0.5f,0.5f,0),
	D3DXVECTOR3(-0.5f,0.5f,0),
};
//=============================================================================
//コンストラクタ
//=============================================================================
Sprite::Sprite(int priority)
{

	//ステータスの初期化
	_Color =
	_MaskColor = WHITE(1.0f);
	_Pos	= D3DXVECTOR3(0,0,0);
	_Size	= D3DXVECTOR3(1.0f,1.0f,1.0f);
	_Speed	= D3DXVECTOR3(0,0,0);
	_Rot		= D3DXVECTOR3(0,0,0);
	uv		= 
	MaskUV	= D3DXVECTOR4(0,0,1.0f,1.0f);
	Texture = 
	Mask = GetTexture(TEX_NONE);
	ReleaseFlag = false;
	Priority = priority;
	_Pass = Shader2D::NORMAL;
	memcpy(_Quad,QuadBase,sizeof(D3DXVECTOR3)*4);

	CPolygonNum++;
	LinkList();
}
//=============================================================================
//自身をリストに追加
//=============================================================================
void Sprite::LinkList(void)
{
	if(_Top[Priority] != NULL)//二つ目以降の処理
	{
		Sprite* CPolygon = _Cur[Priority];
		CPolygon->_Next = this;
		_Prev = CPolygon;
		_Next = NULL;
		_Cur[Priority] = this;
	}
	else//最初の一つの時の処理
	{
		_Top[Priority] = this;
		_Cur[Priority] = this;
		_Prev = NULL;
		_Next = NULL;
	}
}
//=============================================================================
//自身をリストから削除
//=============================================================================
void Sprite::UnlinkList(void)
{
	if(_Prev == NULL)//先頭
	{
		if(_Next != NULL)//次がある
		{
			_Next->_Prev = NULL;
			_Top[Priority] = _Next;
		}
		else//最後の一つだった
		{
			_Top[Priority] = NULL;
			_Cur[Priority] = NULL;
		}
	}
	else if(_Next == NULL)//終端
	{
		if(_Prev != NULL)//前がある
		{
			_Prev->_Next = NULL;
			_Cur[Priority] = _Prev;
		}
		else//最後の一つだった
		{
			_Top[Priority] = NULL;
			_Cur[Priority] = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		_Prev->_Next = _Next;
		_Next->_Prev = _Prev;
	}

	_Prev = NULL;
	_Next = NULL;
	CPolygonNum--;
}
//=============================================================================
//自殺関数
//=============================================================================
void Sprite::Release(void)
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
void Sprite::ReleaseAll(void)
{
	for (int cnt = 0;cnt < LAYER_MAX;cnt++)
	{
		Sprite* CPolygon = _Top[cnt];
		Sprite* CPolygonNext;
		while (CPolygon)
		{
			CPolygonNext = CPolygon->_Next;
			CPolygon->Release();
			CPolygon = CPolygonNext;
		}
	}
	Sprite::Clear();
	
}
//=============================================================================
//先頭と終端をNULLに
//=============================================================================
void Sprite::Clear(void)
{
	_Top[0] = nullptr;
	_Top[1] = nullptr;
	_Cur[0] = nullptr;
	_Cur[1] = nullptr;
}

//=============================================================================
//ポリゴンを作成
//=============================================================================
Sprite* Sprite::Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color,int priority)
{
	Sprite* CPolygon = new Sprite(priority);
	CPolygon->_Pos = pos;
	CPolygon->_Size = D3DXVECTOR3(size.x,size.y,0);
	CPolygon->_Color = color;

	return CPolygon;
}
//=============================================================================
//更新
//=============================================================================
void Sprite::Update(void)
{


	_Pos += _Speed;

}
//=============================================================================
//ポーズ処理
//=============================================================================
void Sprite::Pause(void)
{


}
//=============================================================================
//全体更新
//=============================================================================
void Sprite::UpdateAll(void)
{
	for (int cnt = 0;cnt < LAYER_MAX;cnt++)
	{
		Sprite* CPolygon = _Top[cnt];
		Sprite* Next = NULL;
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
			if (CPolygon->ReleaseFlag == true)
			{
				CPolygon->Release();
			}

			CPolygon = Next;
		}
	}
}
//=============================================================================
//ポリゴンを描画
//=============================================================================
void Sprite::Draw(void)
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

		CastMatrix();

		_Shader->SetFloatArray(Shader2D::DIFFUSE,_Color,4);
		_Shader->SetFloatArray(Shader2D::MASK_COLOR,_MaskColor,4);
		_Shader->SetFloatArray(Shader2D::UV,uv,4);
		_Shader->SetFloatArray(Shader2D::MASK_UV,MaskUV,4);
		//テクスチャの設定
		_Shader->SetTexture(Texture);
		_Shader->SetMask(Mask);

		//ポリゴンを描画
		_Shader->Draw((Shader2D::PASS)_Pass,D3DPT_TRIANGLESTRIP);
	}
}
//=============================================================================
//存在してるポリゴンをすべて描画
//=============================================================================
void Sprite::DrawAll(int priority)
{
	CRenderer::SetStream2D();
	Sprite* CPolygon = _Top[priority];
	if (_Shader == nullptr){ _Shader = Shader2D::Instance(); }
	_Shader->DrawBegin();
	while (CPolygon)
	{
		CPolygon->Draw();
		CPolygon = CPolygon->_Next;
	}
	_Shader->DrawEnd();
	
}
//=============================================================================
//特定のポリゴンを消去 正直あまり使わない
//=============================================================================
void Sprite::Delete(int index)
{
	if(index>-1&&index<CPolygonNum)
	{
		Sprite* CPolygon = _Top[0];
		for(int cnt=0;cnt<index;cnt++)
		{
			CPolygon = CPolygon->_Next;
		}
		if(CPolygon != NULL)
		{
			CPolygon->Release();
		}
	}
}
//=============================================================================
//ポリゴンの右の辺の位置を設定
//=============================================================================
void Sprite::SetRightPos(float right)
{
	
}
//=============================================================================
//ポリゴンの左の辺の位置を設定
//=============================================================================
void Sprite::SetLeftPos(float left)
{
	
}

void Sprite::SetTopPos(float top)
{
	
}

void Sprite::SetBottomPos(float bottom)
{
	
}

void Sprite::SetSpeed(const float base,float radian)
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
void Sprite::SetUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
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
//=============================================================================
//UV値の設定
//	UNum	左から何番目か？
//	VNum	上から何番目か？
//	DivideUNum	横の分割数(最低1)
//	DivideVNum	縦の分割数(最低1);
//=============================================================================
void Sprite::SetMaskUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
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

	MaskUV.x = DivideU*UNum;
	MaskUV.y = DivideV*VNum;
	MaskUV.z = DivideU;
	MaskUV.w = DivideV;
}

void Sprite::CastMatrix(void)
{
	for (int cnt = 0;cnt < 4;cnt++)
	{
		D3DXVec3TransformCoord(&_Quad[cnt],&QuadBase[cnt],&WorldMtx);
	}
}