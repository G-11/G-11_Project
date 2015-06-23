#pragma once
#ifndef _CREATURE_H_
#define _CREATURE_H_

#include "Sprite.h"

class Creature :public Sprite
{
public:
	Creature(int priority = Sprite::LAYER_0);
	virtual ~Creature();
	virtual void Update(void);
	virtual void Capture(void);
	virtual bool HitWall(void);

	static List<Creature>* HitList(void){ return &_HitList; }

protected:
	static List<Creature> _HitList;
	VALUE<Creature>* SelfIterator;

	vector3 ReflectVec;
	vector3 OldPos;
};

#endif