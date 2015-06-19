//================================================================================
//	ゴールオブジェクト
//	Author: Toshiyuki Tsukagoshi
//================================================================================
#pragma once
#ifndef _GOAL_H_
#define _GOAL_H_

#include "Sprite.h"

class Player;

class Goal :public Sprite
{
public:
	static Goal* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority = 0);

	Goal(int priority = 0);
	~Goal(){}

	void Update();
	void Draw();

	static void SetPlayer(Player* p){ player = p; }

private:
	static Player* player;
};


#endif