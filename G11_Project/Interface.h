#pragma once
#ifndef _INTERFACE_H_
#define _INTERFACE_H_
#include "main.h"

class Gauge;
class Sprite;

class Interface
{
public:
	Interface();
	~Interface();

	void Init(int timeLimit,int gaugeMax);
	void Update(void);

	float Percent(void){ return _Percent; }

	static float Score(void){ return _Score; }

	static void AddScore(float score){ _Score += score; AddedScore = true; }

private:
	float TimeLimit;
	float Time;
	float _Percent;

	Gauge* GlowGauge;
	Sprite* GaugeFrame;
	Sprite* Clock;
	Sprite* ClockNeedle;

	static float _Score;
	static bool AddedScore;
};

#endif