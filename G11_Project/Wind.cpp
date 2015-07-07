#include "Wind.h"
#include "Shader2D.h"
Wind* Wind::Create(const vector2& pos,const vector2& size,const float angle)
{
	Wind* effect = new Wind;
	if (effect == nullptr){ return nullptr; }
	effect->_Pos = Vector3(pos,0);
	effect->_Size = Vector3(size,1.0f);
	effect->_Rot.z = -angle;
	effect->Texture = GetTexture(TEX_EFFECT_WIND);

	effect->_Speed.x = sinf(angle)*20.0f;
	effect->_Speed.y = cosf(angle)*20.0f;
	effect->_Color.a = 0;
	effect->Phase = 0;
	effect->frame = 0;
	effect->_Pass = CShader2D::ADD;
	return effect;
}

void Wind::Update(void)
{
	switch (Phase)
	{
	case 0:
		_Color.a += 0.05f;
		if (_Color.a >= 1.0f)
		{
			_Color.a = 1.0f;
			Phase++;
		}
		break;
	case 1:
		frame++;
		if (frame > 10)
		{
			Phase++;
		}
		break;
	case 2:
		_Color.a -= 0.05f;
		if (_Color.a <= 0)
		{
			SetRelease();
			Phase++;
		}
		break;
	default:
		break;
	}
	Sprite::Update();
}