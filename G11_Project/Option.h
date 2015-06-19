#pragma once
#ifndef _OPTION_H_
#define _OPTION_H_

#include "Scene.h"

class Sprite;
class Gauge;
class Counter;

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
	Counter* BGMVolume;
	Counter* SEVolume;

	D3DXVECTOR3 DestPos;
	D3DXVECTOR3 CursolPosition[3];

	const static short ItemMax;
	short Select;
};

#endif