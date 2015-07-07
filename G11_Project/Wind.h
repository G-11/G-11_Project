#pragma once
#ifndef _WIND_H_
#define _WIND_H_

#include "Sprite.h"

class Wind:public Sprite
{
public:
	Wind(int priority = Sprite::LAYER_4) :Sprite(priority){};
	static Wind* Create(const vector2& pos,const vector2& size,const float angle);

	void Update(void);

private:
	int Phase;
};

#endif