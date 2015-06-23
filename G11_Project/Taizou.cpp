#include "Taizou.h"
#include "Item.h"
#include "Smoke.h"

Taizou* Taizou::Create(const vector2& pos,const vector2& size)
{
	Taizou* taizou = new Taizou();
	if (taizou == nullptr){ return nullptr; }

	taizou->OldPos = taizou->_Pos = Vector3(pos,0);
	taizou->_Size = Vector3(size,1.0f);
	taizou->Texture = GetTexture(TEX_CREATURE_TAIZO);
	taizou->SetUV(0,0,2,1);
	taizou->_Speed.x = -2.0f;
	
	return taizou;
}

void Taizou::Update(void)
{
	_Speed.y += 0.48f;

	if (HitWall())
	{
		if (ReflectVec.y < -0.5f)
		{
			_Speed.y = 0;
		}
		if (ReflectVec.x < -0.5f || ReflectVec.x > 0.5f)
		{
			//_Speed.x *= -1.0f;
		}
	}
	_Pos += _Speed;
	OldPos = _Pos;
	if (frame % 15 == 0)
	{
		uv.x += uv.z;
	}
	frame++;
}

void Taizou::Capture(void)
{
	Smoke::Create(vector2(_Pos.x,_Pos.y),vector2(300.0f,300.0f));
	Item* item = Item::Create(_Pos,vector2(84.0f,84.0f),WHITE(1.0f),ITEM_ID_TAIZOU);
	item->SetSpeedY(-10.0f);
	SetRelease();
}