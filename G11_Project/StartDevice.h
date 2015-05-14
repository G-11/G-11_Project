#pragma once
#ifndef _START_DEVICE_H_
#define _START_DEVICE_H_
#include "Sprite.h"

class Player;

class StartDevice :public Sprite
{
public:
	StartDevice(int priority = Sprite::LAYER_1);
	static StartDevice* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_1);

	void Update(void);

	static Player* _Player;
	
	bool Active(void){ return _Active; }

	void SetActive(bool flag){ _Active = flag; }
protected:
	bool _Active;
	
};

#endif