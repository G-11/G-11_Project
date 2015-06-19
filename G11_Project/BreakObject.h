#pragma once
#ifndef _BREAK_OBJECT_H_
#define _BREAK_OBJECT_H_
#include "Wall.h"

class Player;

class BreakObject :public Wall
{
public:
	BreakObject(int priority) :Wall(priority){};
	static BreakObject* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,TEX texID = TEX_NONE,bool player = true,int priority = Sprite::LAYER_2);
	static void SetPlayer(Player* player){ _Player = player; }

	bool Active(void){ return _Active; }
	bool HitAffect(void);

private:
	bool HitPlayer(void);
	bool HitClog(void);
	static Player* _Player;
	bool _Active;
	bool _HitPlayer;
	void Update(void);

};

#endif