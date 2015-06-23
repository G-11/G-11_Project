#pragma once
#ifndef _SEONSOR_H_
#define _SEONSOR_H_

#include "Gimmick.h"

class Sensor :public Gimmick
{
public:
	Sensor(int priority = Sprite::LAYER_0) :Gimmick(priority){ frame = 0; };
	static Sensor* Create(const vector2& pos,float r);

	bool HitAffect(void);
	int Frame(void){ return frame; }
	void SetFrame(int num = 0){ frame = num; }
	void AddFrame(int num = 1){ frame += num; }

private:
	int frame;
};

#endif