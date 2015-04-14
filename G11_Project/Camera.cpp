//======================================================
//カメラクラス
//Creater:Arai Yuhki
//======================================================
//======================================================
//インクルード
//======================================================
#include "Camera.h"
#include "Renderer.h"
#include "Shader3D.h"
#define CAMERA_SPEED (2.0f)
//======================================================
//グローバル変数
//======================================================
int CCamera::CameraNum = 0;
CCamera* CCamera::Top = NULL;
CCamera* CCamera::Cur = NULL;

//======================================================
//コンストラクタ
//引数：LUPos 描画開始点(左上の座標)
//		Size (描画する大きさ)
//======================================================
CCamera::CCamera(const D3DXVECTOR2 &LUPos,D3DXVECTOR2 &size)
{
	if(size.x <0)
	{
		size.x *= -1;
	}
	if(size.y<0)
	{
		size.y *= -1;
	}
	ViewPort.X = (DWORD)LUPos.x;
	ViewPort.Y = (DWORD)LUPos.y;
	ViewPort.Width = (DWORD)size.x;
	ViewPort.Height = (DWORD)size.y;
	ViewPort.MaxZ = 1.0f;
	ViewPort.MinZ = 0.0f;
	_Size = Window::Instance()->WindowSize();
	CameraNum++;
	
	LinkList();

}
//======================================================
//デストラクタ
//======================================================
CCamera::~CCamera()
{
	CameraNum--;
}
//=============================================================================
//自身をリストに追加
//=============================================================================
void CCamera::LinkList(void)
{
	if(Top != NULL)//二つ目以降の処理
	{
		CCamera* pCamera = Cur;
		pCamera->Next = this;
		Prev = pCamera;
		Next = NULL;
		Cur = this;
	}
	else//最初の一つの時の処理
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
}

//=============================================================================
//自身をリストから削除
//=============================================================================
void CCamera::UnlinkList(void)
{
	if(Prev == NULL)//先頭
	{
		if(Next != NULL)//次がある
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if(Next == NULL)//終端
	{
		if(Prev != NULL)//前がある
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;
}

//=============================================================================
//全消去
//=============================================================================
void CCamera::ReleaseAll(void)
{
	CCamera* pCamera = Top;
	CCamera* CameraNext;
	while(pCamera)
	{
		CameraNext = pCamera->Next;
		delete pCamera;
		pCamera = CameraNext;
	}
	CCamera::Clear();
	
}
//=============================================================================
//先頭と終端をNULLに
//=============================================================================
void CCamera::Clear(void)
{
	Top = NULL;
	Cur = NULL;
}

//=============================================================================
//自殺関数
//=============================================================================
void CCamera::Destroy(void)
{
	UnlinkList();
	
	
	PosCameraP = D3DXVECTOR3(0,0,0);
	PosCameraR = D3DXVECTOR3(0,0,0);
	VecCameraU = D3DXVECTOR3(0,0,0);
	Rot = D3DXVECTOR3(0,0,0);
	ForcusDisXZ = 1;
	ForcusDisXY = 1;
	delete this;

}
//=============================================================================
//作成
//=============================================================================
CCamera* CCamera::Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR3 &RPos)
{
	CCamera* pCamera = NULL;
	pCamera = new CCamera();
	pCamera->PosCameraP = PPos;
	pCamera->DestPosCameraP = PPos;
	pCamera->PosCameraR = RPos;
	pCamera->VecCameraU = D3DXVECTOR3(0,1.0f,0);
	D3DXVECTOR3 dis = D3DXVECTOR3((PPos.x-RPos.x),(PPos.y-RPos.y),(PPos.z-RPos.z));
	pCamera->ForcusDisXZ = sqrtf(dis.x*dis.x+dis.z*dis.z);
	pCamera->ForcusDisXZ = sqrtf(dis.x*dis.x+dis.y*dis.y);
	pCamera->Rot.y = atan2(dis.x,dis.z);
	pCamera->Rot.x = atan2(dis.x,dis.y);

	return pCamera;
}
//=============================================================================
//作成(ビューポートも設定)
//=============================================================================
CCamera* CCamera::Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR3 &RPos,const D3DXVECTOR2 &LUPos,D3DXVECTOR2 &size)
{
	CCamera* pCamera = NULL;
	pCamera = new CCamera(LUPos,size);
	pCamera->PosCameraP = PPos;
	pCamera->PosCameraR = RPos;
	pCamera->DestPosCameraP = PPos;
	pCamera->VecCameraU = D3DXVECTOR3(0,1.0f,0);
	D3DXVECTOR3 dis = D3DXVECTOR3((PPos.x-RPos.x),(PPos.y-RPos.y),(PPos.z-RPos.z));
	pCamera->ForcusDisXZ = sqrtf(dis.x*dis.x+dis.z*dis.z);
	pCamera->ForcusDisXZ = sqrtf(dis.x*dis.x+dis.y*dis.y);
	pCamera->Rot.y = atan2(dis.x,dis.z);
	pCamera->Rot.x = atan2(dis.x,dis.y);
	pCamera->Speed.x = 0;
	pCamera->Speed.y = 0;
	pCamera->Speed.z = 0;
	return pCamera;
}
//======================================================
//更新
//======================================================
void CCamera::Update(void)
{
}
//======================================================
//適用
//======================================================
void CCamera::Set(void)
{
	Shader3D* shader = Shader3D::Instance();
	

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&ViewMatrix);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&ViewMatrix,&PosCameraP,&PosCameraR,&VecCameraU);
	shader->SetMatrix(Shader3D::VIEW_MTX,ViewMatrix);
	D3DXMATRIX Inverce;
	D3DXMatrixIdentity(&Inverce);
	D3DXMatrixInverse(&Inverce,NULL,&ViewMatrix);
	Inverce._41 = 0;
	Inverce._42 = 0;
	Inverce._43 = 0;
	shader->SetMatrix(Shader3D::INV_VIEW_MTX,Inverce);
	
	D3DXMATRIX view;

	//プロジェクションの初期化
	D3DXMatrixIdentity(&Projection);
	//プロジェクションの作成
	D3DXMatrixPerspectiveFovLH(&Projection,(45.0f*(D3DX_PI/180.0f)),//視野角
													_Size.x/_Size.y,	//アスペクト比
													10.0f,		//視界限界最短距離
													3000.0f);	//視界限界最長距離


	shader->SetMatrix(Shader3D::PROJECTION,Projection);

}
//======================================================
//全部更新
//======================================================
void CCamera::UpdateAll(void)
{
	CCamera* pCamera = Top;
	CCamera* Next = NULL;
	while(pCamera)
	{
		Next = pCamera->Next;
		pCamera->Update();
		pCamera = Next;
	}
}
//======================================================
//Index番目のカメラを適用
//======================================================
void CCamera::Set(int Index)
{
	
	if(Index < CameraNum && Index >= 0)
	{
		
		CCamera* pCamera = Top;
		CCamera* Next = NULL;
		int num=0;
		while(pCamera)
		{
			Next = pCamera->Next;
			if(num == Index)
			{
				pCamera->Set();
				break;
			}
			num++;
			pCamera = Next;
		}
	}
}
//======================================================
//Index番目のカメラのポインタを取得
//======================================================
CCamera* CCamera::GetCamera(int Index)
{
	CCamera* pCamera = Top;
	CCamera* Next = NULL;
	int num=0;
	while(pCamera)
	{
		Next = pCamera->Next;
		if(Index == num)
		{
			return pCamera;
		}
		num++;
		pCamera = Next;
	}
	return NULL;
}

void CCamera::CreateViewPortMtx(D3DXMATRIX* pViewPort,const UINT width,const UINT height)
{
	float Width = (float)width / 2;
	float Height = (float)height / 2;
	pViewPort->_11 = Width;	pViewPort->_12 = 0.0f;		pViewPort->_13 = 0.0f;	pViewPort->_14 = 0.0f;
	pViewPort->_21 = 0.0f;	pViewPort->_22 = -Height;	pViewPort->_23 = 0.0f;	pViewPort->_24 = 0.0f;
	pViewPort->_31 = 0.0f;	pViewPort->_32 = 0.0f;		pViewPort->_33 = 1.0f;	pViewPort->_34 = 0.0f;
	pViewPort->_41 = Width;	pViewPort->_42 = Height;	pViewPort->_43 = 0.0f;	pViewPort->_44 = 1.0f;

}