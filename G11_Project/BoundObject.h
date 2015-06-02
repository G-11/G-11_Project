#pragma once
#ifndef _BOUND_OBJECT_H_
#define _BOUND_OBJECT_H_

#include "Wall.h"

class BoundObject;
typedef void(*Action)(BoundObject*);

class BoundObject :public Wall
{
public:
	BoundObject(int priority) :Wall(priority){};
	static BoundObject* Create(const D3DXVECTOR2& pos, const D3DXVECTOR2& size,TEX texID,int count,int priority = Sprite::LAYER_3);
	virtual void Update(void);
	virtual void HitAffect(void);
	
	void SetAction(Action action){ TrigAction = action; }

	void CastMatrix(void);

private:
	int Count;

	Action TrigAction;//‹N“®‚µ‚½Œã‚Ì‹““®
	bool Active;
	int _Frame;
};


#endif