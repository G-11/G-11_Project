//================================================================================
//	いーたんクラス
//	Author:Tsukagoshi Toshiyuki
//================================================================================
#pragma once 

#ifndef _EATAN_H_
#define _EATAN_H_

#include "Sprite.h"
#include <list>

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
		EATAN_STATE_HAVE,	//持つ
		EATAN_STATE_WORRIED,	//悩む
		EATAN_STATE_MASTICATION,	//もぐもぐ
		EATAN_STATE_EARN,	//溜め
		EATAN_STATE_DASH,	//体当たり
		EATAN_STATE_SLEEP,	//眠る
		EATAN_STATE_MAX,
		EATAN_STATE_NON,
	}EATAN_STATE;

	static Eatan* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority = 0);

	Eatan(int priority = 0);
	~Eatan(){}

	virtual void Update();
	virtual void Draw();

	void SetState(EATAN_STATE State);	//すぐにステートを変更する
	void SetSwayFlag(bool Flag){ SwayFlag = Flag; }
	EATAN_STATE State(){ return _State; }

	void AddState(EATAN_STATE state);

protected :
	EATAN_STATE NextState;
	EATAN_STATE _State;
	EATAN_STATE OldState;

	float AnimationCount;			//アニメーション用カウンター
	short AnimationPartition[EATAN_STATE_MAX];	//アニメーション分割数
	float AnimationSpeed[EATAN_STATE_MAX];		//アニメーションスピード
	short AnimationRepeat[EATAN_STATE_MAX];		//アニメーションの繰り返し回数(0で無限,1で一回のみ再生)
	int   MaxPartition;				//アニメーション枚数の最大値

	bool SwayFlag;					//trueで上下に揺れる
	float SwayCount;				//上下に揺れる用カウンター

	int LoopCount;					//アニメーションの繰り返しカウント

	std::list<EATAN_STATE> StateList;
};

#endif