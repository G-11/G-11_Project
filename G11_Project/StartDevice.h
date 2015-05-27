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
	typedef enum
	{
		TRIGGER = 0,
		ON,
		TIMER,
		SWITCH,
		TRIGGERTYPE_MAX
	}TRIGGER_TYPE;

	StartDevice(int priority = Sprite::LAYER_1);
	~StartDevice();
	static StartDevice* CreateTrigger(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_1);
	static StartDevice* CreateON(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_1);
	static StartDevice* CreateTimer(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int timeLimit,int priority = Sprite::LAYER_1);
	static StartDevice* CreateSwitch(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_1);

	void Update(void);

	static void SetPlayer(Player* player){ _Player = player; }
	
	bool Active(void){ return _Active; }

	void SetActive(bool flag){ _Active = flag; }

	static List<StartDevice>& StartDeviceList(void){ return _StartDeviceList; }
	static void InitList(void){ _StartDeviceList.Clear(); }

protected:
	static Player* _Player;

	bool _Active;
	TRIGGER_TYPE TriggerType;
	static List<StartDevice> _StartDeviceList;
	VALUE<StartDevice>* SelfIterator;
	int TimeLimit;
	bool OldActive;

};

#endif