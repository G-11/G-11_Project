#pragma once
#ifndef _STAR_H_
#define _STAR_H_

#include "Sprite.h"

class Star :public Sprite
{
public:
	Star(int priority = Sprite::LAYER_4) :Sprite(priority){};
	static Star* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,float angle,int frame = 30);

	void Update(void);

private:
	int LifeTime;
	float SpinAngle;

};

#endif