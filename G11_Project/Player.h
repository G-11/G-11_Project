//================================================================================
//	プレイヤークラス
//	Author:Tsukagoshi Toshiyuki
//================================================================================
#pragma once 

//================================================================================
//	include
//================================================================================
#include "Sprite.h"

//================================================================================
//	プレイヤークラス
//================================================================================
class Player :public Sprite
{
public:
	typedef enum
	{
		PLAYER_STATE_STOP = 0,		//何もしてない
		PLAYER_STATE_MOVE,		//移動
		PLAYER_STATE_EAT,		//食べる
		PLAYER_STATE_GLAD,		//喜ぶ
		PLAYER_STATE_MAX,
		PLAYER_SATAE_NON,
	}PLAYER_STATE;

	static Player* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority = 0);

	Player(int priority = 0);
	~Player(){}

	void Update();
	void Draw();

	void SetState(PLAYER_STATE State);
	PLAYER_STATE State(){ return _State; }

private:
	PLAYER_STATE _State;
	PLAYER_STATE OldState;

	float AnimationCount;			//アニメーション用カウンター
	short AnimationPartition[PLAYER_STATE_MAX];	//アニメーション分割数
	float AnimationSpeed[PLAYER_STATE_MAX];		//アニメーションスピード
	int   MaxPartition;				//アニメーション枚数の最大値

	float SwayCount;				//上下に揺れる用カウンター

};

