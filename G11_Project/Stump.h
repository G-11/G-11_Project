#pragma once
#ifndef _STUMP_H_
#define _STUMP_H_
#include "StartDevice.h"

class Stump :public StartDevice
{
public:
	Stump(int priority) :StartDevice(priority){};
	static Stump* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_1);

	void Update(void);
private:
	D3DXVECTOR3 OriginalSize;
};


#endif