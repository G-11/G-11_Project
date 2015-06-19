#pragma once
#ifndef _CUT_EFFECT_H_
#define _CUT_EFFECT_H_
#include "Sprite.h"

class CutEffect :public Sprite
{
public:
	CutEffect(int priority) :Sprite(priority){};
	static CutEffect* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,bool right,int priority = Sprite::LAYER_3);

	void Update(void);
private:
	int frame;
	D3DXVECTOR3 DestPos;
};

#endif