//======================================================
//カメラクラス
//Creater:Arai Yuhki
//======================================================
//======================================================
//インクルード
//======================================================
#include "Camera2D.h"
#include "Renderer.h"
#include "Shader2D.h"
#include "Wall.h"
#include "Player.h"
#define CAMERA_SPEED (2.0f)
//======================================================
//グローバル変数
//======================================================
int Camera2D::CameraNum = 0;
Camera2D* Camera2D::Top = NULL;
Camera2D* Camera2D::Cur = NULL;
CShader2D* Camera2D::_Shader = nullptr;
Player* Camera2D::player = nullptr;
//======================================================
//コンストラクタ
//引数：LUPos 描画開始点(左上の座標)
//		Size (描画する大きさ)
//======================================================
Camera2D::Camera2D(const D3DXVECTOR2 &LUPos,D3DXVECTOR2 &size)
{
	if (size.x <0)
	{
		size.x *= -1;
	}
	if (size.y<0)
	{
		size.y *= -1;
	}
	ViewPort.X = (DWORD)LUPos.x;
	ViewPort.Y = (DWORD)LUPos.y;
	ViewPort.Width = (DWORD)size.x;
	ViewPort.Height = (DWORD)size.y;
	ViewPort.MaxZ = 1.0f;
	ViewPort.MinZ = 0.0f;

	_LeftLimit = -200.0f;
	_RightLimit = SCREEN_WIDTH + 200;

	CameraNum++;

	LinkList();

}
//======================================================
//デストラクタ
//======================================================
Camera2D::~Camera2D()
{
	CameraNum--;
}
//=============================================================================
//自身をリストに追加
//=============================================================================
void Camera2D::LinkList(void)
{
	if (Top != NULL)//二つ目以降の処理
	{
		Camera2D* pCamera = Cur;
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
void Camera2D::UnlinkList(void)
{
	if (Prev == NULL)//先頭
	{
		if (Next != NULL)//次がある
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
	else if (Next == NULL)//終端
	{
		if (Prev != NULL)//前がある
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
void Camera2D::ReleaseAll(void)
{
	Camera2D* pCamera = Top;
	Camera2D* CameraNext;
	while (pCamera)
	{
		CameraNext = pCamera->Next;
		delete pCamera;
		pCamera = CameraNext;
	}
	Camera2D::Clear();

}
//=============================================================================
//先頭と終端をNULLに
//=============================================================================
void Camera2D::Clear(void)
{
	Top = NULL;
	Cur = NULL;
}

//=============================================================================
//自殺関数
//=============================================================================
void Camera2D::Destroy(void)
{
	UnlinkList();


	Pos = D3DXVECTOR3(0,0,0);
	Rot = D3DXVECTOR3(0,0,0);
	ForcusDisXZ = 1;
	ForcusDisXY = 1;
	delete this;

}
//=============================================================================
//作成
//=============================================================================
Camera2D* Camera2D::Create(const D3DXVECTOR3 &PPos)
{
	Camera2D* pCamera = NULL;
	pCamera = new Camera2D();
	pCamera->Pos = PPos;
	pCamera->DestPos = PPos;
	pCamera->Projection2D = D3DXMATRIX(
		2.0f / SCREEN_WIDTH,0,0,0,
		0,-2.0f / SCREEN_HEIGHT,0,0,
		0,0,1.0f,0,
		-1.0f,1.0f,0,1.0f);
	return pCamera;
}
//=============================================================================
//作成(ビューポートも設定)
//=============================================================================
Camera2D* Camera2D::Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR2 &LUPos,D3DXVECTOR2 &size)
{
	Camera2D* pCamera = NULL;
	pCamera = new Camera2D(LUPos,size);
	pCamera->Pos = PPos;
	pCamera->DestPos = PPos;
	pCamera->Speed.x = 0;
	pCamera->Speed.y = 0;
	pCamera->Speed.z = 0;
	pCamera->Projection2D = D3DXMATRIX(
		2.0f / SCREEN_WIDTH,0,0,0,
		0,-2.0f / SCREEN_HEIGHT,0,0,
		0,0,1.0f,0,
		-1.0f,1.0f,0,1.0f);
	return pCamera;
}
//======================================================
//更新
//======================================================
void Camera2D::Update(void)
{
	if (player != nullptr)
	{
		D3DXVECTOR3 playerPos = player->Pos();

		//プレイヤーの動きに追従させる(プレイヤーの座標から画面の半分ずらす)
		DestPos.x = playerPos.x - SCREEN_WIDTH*0.5f;
		DestPos.y = playerPos.y - SCREEN_HEIGHT*0.5f;

		//プレイヤーが上下のリミットを超えたらYをもとに戻す
		if (playerPos.y < _TopLimit || playerPos.y > _BottomLimit)
		{
			DestPos.y = OldPos.y;
		}

		//プレイヤーが左右のリミットを超えたらXをもとに戻す	
		if (playerPos.x < _LeftLimit || playerPos.x > _RightLimit)
		{
			DestPos.x = OldPos.x;
		}
		OldPos = DestPos;

		//プレイヤーの移動方向にカメラを少しずらす
		DestPos += player->Speed()*5.0f;

		Pos += (DestPos - Pos)*0.03f;
	}

	
}
//======================================================
//適用
//======================================================
void Camera2D::Set(void)
{
	
	if (_Shader == nullptr){ _Shader = CShader2D::Instance(); }

	Projection2D._41 = -1.0f - Pos.x / (SCREEN_WIDTH*0.5f);
	Projection2D._42 = 1.0f + Pos.y / (SCREEN_HEIGHT*0.5f);
	
	_Shader->SetMatrix(CShader2D::PROJECTION,Projection2D);
}

void Camera2D::SetNoMove(void)
{
	if (_Shader == nullptr){ _Shader = CShader2D::Instance(); }

	Projection2D._41 = -1.0f;
	Projection2D._42 = 1.0f;

	_Shader->SetMatrix(CShader2D::PROJECTION,Projection2D);
}
//======================================================
//全部更新
//======================================================
void Camera2D::UpdateAll(void)
{
	Camera2D* pCamera = Top;
	Camera2D* Next = NULL;
	while (pCamera)
	{
		Next = pCamera->Next;
		pCamera->Update();
		pCamera = Next;
	}
}
//======================================================
//Index番目のカメラを適用
//======================================================
void Camera2D::Set(int Index)
{

	if (Index < CameraNum && Index >= 0)
	{

		Camera2D* pCamera = Top;
		Camera2D* Next = NULL;
		int num = 0;
		while (pCamera)
		{
			Next = pCamera->Next;
			if (num == Index)
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
Camera2D* Camera2D::GetCamera(int Index)
{
	Camera2D* pCamera = Top;
	Camera2D* Next = NULL;
	int num = 0;
	while (pCamera)
	{
		Next = pCamera->Next;
		if (Index == num)
		{
			return pCamera;
		}
		num++;
		pCamera = Next;
	}
	return NULL;
}


//======================================================
//天井セット
//======================================================
void Camera2D::SetCilling(Wall* cilling)
{
	Cilling = cilling;
	D3DXVECTOR3 CillingPos = cilling->Pos();
	D3DXVECTOR2 CillingSize = cilling->Size();
	
	_TopLimit = CillingPos.y - CillingSize.y*0.5f + SCREEN_HEIGHT*0.5f;

}

//======================================================
//床セット
//======================================================
void Camera2D::SetFloor(Wall* floor)
{
	Floor = floor;
	D3DXVECTOR3 FloorPos = Floor->Pos();
	D3DXVECTOR2 FloorSize = Floor->Size();

	_BottomLimit = FloorPos.y + FloorSize.y*0.5f - SCREEN_HEIGHT*0.5f;

}