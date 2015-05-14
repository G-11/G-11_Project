#pragma once
#ifndef _GIMMICK_H_
#define _GIMMICK_H_
#include "Sprite.h"
#include "StartDevice.h"
#include<vector>

class Gimmick;

typedef void(*Action)(Gimmick*);
class Gimmick :public Sprite
{
public:
	Gimmick(int priority);
	virtual ~Gimmick();
	static Gimmick* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_1);
	void Update(void);
	void SetAction(Action action){ TrigAction = action; }
	void AddStartDevice(StartDevice* device);
	int Frame(void){ return _Frame; }

protected:
	bool _Active;
	void CheckDevice(void);
	int _Frame;
	std::vector<StartDevice*> List;
	Action TrigAction;//‹N“®‚µ‚½Œã‚Ì‹““®

};
#endif