#pragma once
#ifndef _OPTION_H_
#define _OPTION_H_

#include "Scene.h"

class Sprite;
class Gauge;

class Option :public Scene
{
public:
	Option();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	Sprite* Cursor;
	Sprite* BGMFrame;
	Sprite* SEFrame;
	Gauge* BGMGauge;
	Gauge* SEGauge;

	short Select;
};

#endif