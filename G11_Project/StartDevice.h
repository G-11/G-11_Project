#pragma once
#ifndef _START_DEVICE_H_
#define _START_DEVICE_H_
#include "Sprite.h"
#include <list>

class Player;
class StartDevice;

class StartDevice :public Sprite
{
public:
	StartDevice(int priority = Sprite::LAYER_1);
	~StartDevice();
	static StartDevice* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_1);

	void Update(void);

	static Player* _Player;
	
	bool Active(void){ return _Active; }

	void SetActive(bool flag){ _Active = flag; }

	static List<StartDevice>& StartDeviceList(void){ return _StartDeviceList; }
	static void InitList(void){ _StartDeviceList.Clear(); }

protected:
	bool _Active;

	static List<StartDevice> _StartDeviceList;
	VALUE<StartDevice>* SelfIterator;
	
};

#endif