//================================================================================
//	いーたんクラス
//	Author:Tsukagoshi Toshiyuki
//================================================================================
#pragma once 

#ifndef _EATAN_H_
#define _EATAN_H_

#include "Sprite.h"

class Eatan :public Sprite
{
public:
	typedef enum
	{
		EATAN_STATE_STOP = 0,		//何もしてない
		EATAN_STATE_MOVE,		//移動
		EATAN_STATE_EAT,		//食べる
		EATAN_STATE_GLAD,		//喜ぶ
		EATAN_STATE_ATTACK,	//攻撃
		EATAN_STATE_REVERSE,	//吐く
		EATAN_STATE_MAX,
		EATAN_STATE_NON,
	}EATAN_STATE;

	static Eatan* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority = 0);

	Eatan(int priority = 0);
	~Eatan(){}

	virtual void Update();
	virtual void Draw();

	void SetState(EATAN_STATE State);	//すぐにステートを変更する
	void SetNextState(EATAN_STATE State){ NextState = State; }	//現在のモーションが終わり次第ステートを変更する
	void SetSwayFlag(bool Flag){ SwayFlag = Flag; }
	EATAN_STATE State(){ return _State; }

protected :
	EATAN_STATE NextState;
	EATAN_STATE _State;
	EATAN_STATE OldState;

	float AnimationCount;			//アニメーション用カウンター
	short AnimationPartition[EATAN_STATE_MAX];	//アニメーション分割数
	float AnimationSpeed[EATAN_STATE_MAX];		//アニメーションスピード
	int   MaxPartition;				//アニメーション枚数の最大値

	bool SwayFlag;					//trueで上下に揺れる
	float SwayCount;				//上下に揺れる用カウンター
};

#endif