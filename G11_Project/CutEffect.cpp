#include "CutEffect.h"

CutEffect* CutEffect::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,bool right,int priority)
{
	CutEffect* effect = new CutEffect(priority);
	if (effect == nullptr){ return nullptr; }

	effect->_Pos = Vector3(pos,0);
	effect->_Speed.x = cosf(DEG2RAD(-40.0f))*10.0f;
	effect->_Speed.y = sinf(DEG2RAD(-40.0f))*10.0f;
	if (!right)
	{
		effect->_Speed.x *= -1;
		effect->uv.z = -1.0f;
	}
	effect->_Size = Vector3(size,1.0f);
	effect->Texture = GetTexture(TEX_EFFECT_CUT);
	effect->frame = 0;

	return effect;
}

void CutEffect::Update(void)
{
	_Pos += _Speed;
	_Speed *= 0.9f;
	_Color.a -= 0.02f;

	frame++;
	if (frame > 50)
	{
		SetRelease();
	}
}