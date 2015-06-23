#pragma once
#ifndef _SMORKE_H_
#define _SMORKE_H_
#include "Sprite.h"

class Smoke :public Sprite
{
public:
	Smoke(int priority = Sprite::LAYER_4) :Sprite(priority){};
	static Smoke* Create(const vector2& pos,const vector2& size);

	void Update(void);
};
#endif