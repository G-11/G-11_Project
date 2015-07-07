#pragma once
#ifndef _BOUND_OBJECT_H_
#define _BOUND_OBJECT_H_

#include "Wall.h"

class BoundObject;
class Player;

typedef void(*BOAction)(BoundObject*);

class BoundObject :public Wall
{
public:
	BoundObject(int priority) :Wall(priority){};
	static BoundObject* Create(const D3DXVECTOR2& pos, const D3DXVECTOR2& size,TEX texID,int count,int priority = Sprite::LAYER_3);
	virtual void Update(void);
	virtual bool HitAffect(void);
	
	void SetAction(BOAction action){ TrigAction = action; }

	void CastMatrix(void);

	static void SetPlayer(Player* player){ _Player = player; }

private:
	static Player* _Player;
	int Count;

	BOAction TrigAction;//‹N“®‚µ‚½Œã‚Ì‹““®
	bool Active;
	int _Frame;
};


#endif