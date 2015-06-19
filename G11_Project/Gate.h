#pragma once
#ifndef _GATE_H_
#define _GATE_H_
#include "Gimmick.h"

class Gate :public Gimmick
{
public:
	Gate(int priority) :Gimmick(priority){};
	static Gate* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,TEX texID,int priority = Sprite::LAYER_3);

	void Update(void);

private:
	D3DXVECTOR3 OriginalPos;
};


#endif