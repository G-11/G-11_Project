#include "Star.h"

Star* Star::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,float angle,int life)
{
	Star* effect = new Star;
	if (effect == nullptr){ return nullptr; }

	effect->_Pos = Vector3(pos,0);
	effect->_Size = Vector3(size,1.0f);
	effect->Texture = GetTexture(TEX_EFFECT_STAR);
	effect->SpinAngle = DEG2RAD(Randf(-3.0f,3.0f));
	
	float BaseSpeed = Randf(3.0f,10.0f);
	effect->_Speed.x = cosf(angle)*BaseSpeed;
	effect->_Speed.y = sinf(angle)*BaseSpeed;
	effect->_Speed.z = 0;
	effect->LifeTime = life;
	effect->frame = 0;

	return effect;
}

void Star::Update(void)
{
	if (frame > LifeTime)
	{
		SetRelease();
	}

	_Rot.z += SpinAngle;

	_Speed *= 0.99f;
	if (LifeTime - frame <= 10)
	{
		_Color.a -= 0.1f;
	}
	_Pos += _Speed;
	frame++;
}