#pragma once
#ifndef _FIXED_CAMERA_H_
#define _FIXED_CAMERA_H_

#include "Gimmick.h"
#include "StartDevice.h"

class FixedArea :public StartDevice
{
public:
	FixedArea(int priority = Sprite::LAYER_0) :StartDevice(priority){};
	static FixedArea* Create(const vector2& pos,const vector2& size);

	void Update(void);
private:

};

class FixedCamera :public Gimmick
{
public:
	FixedCamera(int priority = Sprite::LAYER_0) :Gimmick(priority){};
	static FixedCamera* Create(const vector2& pos,const vector2& fixedPos);
	void Update(void);

private:
	vector2 FixedPos;
};


#endif