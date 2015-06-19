#pragma once
#ifndef _TAIZOU_H_
#define _TAIZOU_H_
#include "Creature.h"

class Taizou :public Creature
{
public:
	Taizou(int priority = Sprite::LAYER_0) :Creature(priority){};
	static Taizou* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size = D3DXVECTOR2(158.0f,158.0f));

	void Update(void);
	void Capture(void);
private:
	int frame;
};

#endif