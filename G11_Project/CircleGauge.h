//=============================================================================
//円形ゲージクラス
//=============================================================================
#ifndef _CIRCLE_GAGE_H_
#define _CIRCLE_GAGE_H_

//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Sprite.h"

//クラス定義
class CircleGauge :public Sprite
{
public:
	CircleGauge(int priority = Sprite::LAYER_INTERFACE);
	~CircleGauge();

	static CircleGauge* Create(const D3DXVECTOR3& pos,float r,int divide,bool point = true,int priority = Sprite::LAYER_INTERFACE);

	bool Init(void);
	void Update(void);
	void Draw(void);

	void SetMax(float max){ Max = max; }//最大値を設定
	void SetCur(float cur){ DestCur = cur; }//現在値を設定

private:

	int Divide;//分割数
	float r;//半径
	float Max;//最大値
	float Current;//現在値
	float DestCur;//徐々に減少させるための目標値
	float BaseAngle;//開始地点の角度
	bool _Point;
	Sprite* Point[2];
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;//頂点バッファ
};

#endif