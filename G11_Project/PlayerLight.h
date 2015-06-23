#pragma once
#ifndef _PLAYER_LIGHT_H_
#define _PLAYER_LIGHT_H_

#include "LightParticle.h"

class Player;

class PlayerLight :public LightParticle
{
public:
	PlayerLight(){ _Player = nullptr; }
	static PlayerLight* Create(Player* player,float size);
	void Update(void);

private:
	Player* _Player;
};

#endif