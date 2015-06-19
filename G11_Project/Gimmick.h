#pragma once
#ifndef _GIMMICK_H_
#define _GIMMICK_H_
#include "Wall.h"
#include "StartDevice.h"
#include<vector>

class Gimmick;

typedef void(*Action)(Gimmick*);
class Gimmick :public Wall
{
public:
	Gimmick(int priority);
	virtual ~Gimmick();
	static Gimmick* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_1);
	void Update(void);
	void SetAction(Action action){ TrigAction = action; }
	void AddStartDevice(StartDevice* device);
	int Frame(void){ return _Frame; }

	static List<Gimmick>& GimmickList(void){ return _GimmickList; }
	static void InitList(void){ _GimmickList.Clear(); }

protected:
	bool _Active;
	void CheckDevice(void);
	int _Frame;
	std::vector<StartDevice*> DeviceList;

	D3DXVECTOR2 _Speed;
	Action TrigAction;//‹N“®‚µ‚½Œã‚Ì‹““®

	static List<Gimmick> _GimmickList;
	VALUE<Gimmick>* SelfIterator;

};
#endif