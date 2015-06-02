//=============================================================================
// カメラクラス
//Creater:Arai Yuhki
//=============================================================================
#ifndef _CAMERA2D_H_
#define _CAMERA2D_H_
//======================================================
// インクルード
//======================================================
#include "main.h"

class CShader2D;
class Wall;
class Player;

//======================================================
//カメラクラス
//======================================================
class Camera2D
{
protected:
	static Camera2D* Top;
	static Camera2D* Cur;
	Camera2D* Prev;
	Camera2D* Next;
	virtual void Update(void);
	virtual void Set(void);
	void LinkList(void);
	void UnlinkList(void);
	void CreateViewPortMtx(D3DXMATRIX* pViewPort,UINT width,UINT height);

	static CShader2D* _Shader;
	D3DXVECTOR3 Pos;		//カメラの位置
	D3DXVECTOR3 DestPos;	//移動先
	D3DXVECTOR3 Speed;			//速度
	D3DXVECTOR3 Rot;			//角度
	D3DXMATRIX Projection2D;		//プロジェクションマトリクス
	D3DVIEWPORT9 ViewPort;		//ビューポート
	float ForcusDisXZ;			//XZ座標の距離
	float ForcusDisXY;			//XY座標の距離
	D3DXVECTOR3 ClickPos;
	static int CameraNum;		//現在のカメラ数
	int frame;

	Wall* Cilling;
	Wall* Floor;

	static Player* player;
	D3DXVECTOR3 OldPos;

	float _TopLimit;
	float _BottomLimit;
	float _LeftLimit;
	float _RightLimit;


public:
	Camera2D(const D3DXVECTOR2 &LUPos = D3DXVECTOR2(0,0),D3DXVECTOR2 &size = D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));//コストラクタ
	virtual ~Camera2D();//デストラクタ
	static Camera2D* Create(const D3DXVECTOR3 &PPos);//作成(メインカメラ用、複数も可)
	static Camera2D* Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR2 &LUPos,D3DXVECTOR2 &size);//作成(ビューポートも同時に設定)
	static void UpdateAll(void);//全部更新
	/*
	DirectXDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	for(int cnt=0;cnt<Camera3D::GetCameraNum();cnt++)
	{
	Camera3D::Set(cnt);
	描画
	}
	で描ける。
	*/
	static void Set(int Index);//Index番目のカメラを使用
	static void ReleaseAll(void);//全部解放
	void Destroy(void);			//削除
	static void Clear(void);	//TopとCurをクリア
	
	void SetNoMove(void);

	static int GetCameraNum(void)			{ return CameraNum; }//カメラ数を取得
	D3DXVECTOR3 GetPosP(void)const			{ return Pos; }//カメラ位置を取得
	D3DXVECTOR3 GetRot(void)const			{ return Rot; }
	D3DXVECTOR3 GetSpeed(void)const			{ return Speed; }
	D3DXVECTOR2 GetViewPortPos(void)const	{ return D3DXVECTOR2((float)ViewPort.X,(float)ViewPort.Y); }
	D3DXVECTOR2 GetViewPortSize(void)const	{ return D3DXVECTOR2((float)ViewPort.Width,(float)ViewPort.Height); }
	D3DXMATRIX Projection(void){ return Projection2D; }

	void SetPosP(const D3DXVECTOR3 &pos){ Pos = pos; }
	void SetSpeed	(const D3DXVECTOR3 &speed)		{ Speed = speed; }
	void SetRot		(const D3DXVECTOR3 &rot)			{ Rot = rot; }
	void SetViewPortPos(const D3DXVECTOR2 &Pos)	{ ViewPort.X = (DWORD)Pos.x; ViewPort.Y = (DWORD)Pos.y; }
	void SetViewPortSize(const D3DXVECTOR2 &Size)	{ ViewPort.Width = (DWORD)Size.x;ViewPort.Height = (DWORD)Size.y; }
	static void SetShader(CShader2D* shader){_Shader = shader;}
	void AddPosP(const D3DXVECTOR3 &pos)			{ Pos += pos; }
	void AddRot	(const D3DXVECTOR3 &rot)			{ Rot += rot; }
	void AddViewPortPos(const D3DXVECTOR2 &Pos)	{ ViewPort.X += (DWORD)Pos.x; ViewPort.Y += (DWORD)Pos.y; }
	void AddViewPortSize(const D3DXVECTOR2 &Size)	{ ViewPort.Width += (DWORD)Size.x;ViewPort.Height += (DWORD)Size.y; }

	static Camera2D* GetCamera(int Index);				//Index番目のカメラのポインタを取得

	void SetPlayer(Player* p){ player = p; }
	void SetCilling(Wall* cilling);
	void SetFloor(Wall* floor);

	void SetTopLimit(float limit){ _TopLimit = limit; }
	void SetBottomLimit(float limit){ _BottomLimit = limit; }
	void SetLeftLimit(float limit){ _LeftLimit = limit; }
	void SetRightLimit(float limit){ _RightLimit = limit; }

	float TopLimit(){ return _TopLimit; }
	float BottomLimit(){ return _BottomLimit; }
	float LeftLimit(){ return _LeftLimit; }
	float RightLimit(){ return _RightLimit; }
};

#endif