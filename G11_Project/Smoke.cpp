#include "Smoke.h"

Smoke* Smoke::Create(const vector2& pos,const vector2& size)
{
	Smoke* smoke = new Smoke;
	if (smoke == nullptr){ return smoke; }

	smoke->_Pos = Vector3(pos,0);
	smoke->_Size = Vector3(size,1.0f);
	smoke->frame = 0;
	smoke->Texture = GetTexture(TEX_EFFECT_SMORKE);
	smoke->SetUV(0,0,8,1);

	return smoke;
}

void Smoke::Update(void)
{
	if (frame % 5 == 0)
	{
		uv.x += uv.z;
	}
	if (uv.x >= 1.0f)
	{
		SetRelease();
	}
	frame++;
}