#pragma once
#ifndef _IVY_H_
#define _IVY_H_
#include "StartDevice.h"

class Ivy :public StartDevice
{
public:
	Ivy(int priority) :StartDevice(priority){};
	static Ivy* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority);

	void Update(void);

private:
	bool EffectEmited;

};

#endif