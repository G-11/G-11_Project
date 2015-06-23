#include "PlayerLight.h"
#include "Player.h"

PlayerLight* PlayerLight::Create(Player* player,float size)
{
	PlayerLight* light = new PlayerLight;
	if (light == nullptr){ return nullptr; }
	light->_Pos = player->Pos();
	light->_Player = player;
	light->_Size = vector3(size,size,1.0f);
	light->Texture = GetTexture(TEX_EFFECT_LIGHT);
	light->_Color = WHITE(1.0f);
	
	return light;
}

void PlayerLight::Update(void)
{
	if (_Player != nullptr)
	{
		_Pos = _Player->Pos();
	}
}