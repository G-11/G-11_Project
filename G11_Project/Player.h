//================================================================================
//	プレイヤークラス
//	Author:Tsukagoshi Toshiyuki
//================================================================================
#pragma once 

//================================================================================
//	include
//================================================================================
#include "Eatan.h"

class CheckPoint;
class Clog;
//================================================================================
//	プレイヤークラス
//================================================================================
class Player :public Eatan
{
public:
	

	static Player* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority = 0);

	Player(int priority = 0);
	~Player();

	void Update();
	void Draw();

	bool HitCheckFlag(){ return _HitCheckFlag; }

	void SetHitCheckFlag(bool Flag){ _HitCheckFlag = Flag; }
	void SetActionFlag(bool Flag){ ActionFlag = Flag; }
	
	D3DXVECTOR3* PosPtr(void){ return &_Pos; }
private:

	void HitWall(void);
	void HitCheckPoint(void);
	bool HitClog(void);

	D3DXVECTOR3 OldPos;
	D3DXVECTOR2 OffsetPos;
	bool _HitCheckFlag;
	bool ActionFlag;
	CheckPoint* CurrentCheckPoint;
	D3DXVECTOR3 CheckPointPos;
	Clog* HavingObject;
	int Score;		//前回食べたもののスコア
};

