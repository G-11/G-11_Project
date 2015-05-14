//=============================================================================
// カメラクラス
//Creater:Arai Yuhki
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_
//======================================================
// インクルード
//======================================================
#include "main.h"
//======================================================
//カメラクラス
//======================================================
class Camera3D
{
protected:
	static Camera3D* Top;
	static Camera3D* Cur;
	Camera3D* Prev;
	Camera3D* Next;
	virtual void Update(void);
	void LinkList(void);
	void UnlinkList(void);
	void CreateViewPortMtx(D3DXMATRIX* pViewPort,const UINT width,const UINT height);

	D3DXVECTOR3 PosCameraP;		//カメラの位置
	D3DXVECTOR3 DestPosCameraP;	//移動量?
	D3DXVECTOR3 PosCameraR;		//注視点
	D3DXVECTOR3 VecCameraU;		//法線ベクトル
	D3DXVECTOR3 Speed;			//速度
	D3DXVECTOR3 Rot;			//角度
	D3DXMATRIX ViewMatrix;		//ビューマトリクス
	D3DXMATRIX Projection;		//プロジェクションマトリクス
	D3DXMATRIX Projection2D;		//プロジェクションマトリクス
	D3DVIEWPORT9 ViewPort;		//ビューポート
	D3DXVECTOR2 _Size;			//アスペクト比に使用
	float ForcusDisXZ;			//XZ座標の距離
	float ForcusDisXY;			//XY座標の距離
	D3DXVECTOR3 ClickPos;
	static int CameraNum;		//現在のカメラ数
	int frame;

public:
	Camera3D(const D3DXVECTOR2 &LUPos=D3DXVECTOR2(0,0),D3DXVECTOR2 &size=D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));//コストラクタ
	virtual ~Camera3D();//デストラクタ
	static Camera3D* Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR3 &RPos);//作成(メインカメラ用、複数も可)
	static Camera3D* Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR3 &RPos,const D3DXVECTOR2 &LUPos,D3DXVECTOR2 &size);//作成(ビューポートも同時に設定)
	static void UpdateAll(void);//全部更新
	virtual void Set(void);
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

	static int GetCameraNum(void)			{return CameraNum;}//カメラ数を取得
	D3DXVECTOR3 GetPosP(void)const			{return PosCameraP;}//カメラ位置を取得
	D3DXVECTOR3 GetPosR(void)const			{return PosCameraR;}//注視点を取得
	D3DXVECTOR3 GetRot(void)const			{return Rot;}
	D3DXVECTOR2 GetViewPortPos(void)const	{return D3DXVECTOR2((float)ViewPort.X,(float)ViewPort.Y);}
	D3DXVECTOR2 GetViewPortSize(void)const	{return D3DXVECTOR2((float)ViewPort.Width,(float)ViewPort.Height);}
	D3DXMATRIX GetViewMatrix(void)const		{return ViewMatrix;}

	void SetPosP(D3DXVECTOR3 Pos)			{PosCameraP = Pos;}
	void SetPosR(D3DXVECTOR3 Pos)			{PosCameraR = Pos;}
	void SetSpeed(D3DXVECTOR3 speed)		{Speed = speed;}
	void SetRot(D3DXVECTOR3 rot)			{Rot = rot;}
	void SetViewPortPos(D3DXVECTOR2 Pos)	{ViewPort.X = (DWORD)Pos.x; ViewPort.Y = (DWORD)Pos.y;}
	void SetViewPortSize(D3DXVECTOR2 Size)	{ViewPort.Width = (DWORD)Size.x;ViewPort.Height = (DWORD)Size.y;}
	void SetSize(const D3DXVECTOR2& size) { _Size = size; }

	void AddPosP(D3DXVECTOR3 Pos)			{PosCameraP += Pos;}
	void AddPosR(D3DXVECTOR3 Pos)			{PosCameraR += Pos;}
	void AddRot(D3DXVECTOR3 rot)			{Rot += rot;}
	void AddViewPortPos(D3DXVECTOR2 Pos)	{ViewPort.X += (DWORD)Pos.x; ViewPort.Y += (DWORD)Pos.y;}
	void AddViewPortSize(D3DXVECTOR2 Size)	{ViewPort.Width += (DWORD)Size.x;ViewPort.Height += (DWORD)Size.y;}
	
	static Camera3D* GetCamera(int Index);				//Index番目のカメラのポインタを取得

};

#endif