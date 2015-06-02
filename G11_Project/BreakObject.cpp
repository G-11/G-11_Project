#include "BreakObject.h"
#include "Player.h"
#include "Collision.h"

Player* BreakObject::_Player = nullptr;

BreakObject* BreakObject::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,TEX texID,int priority)
{
	BreakObject* object = new BreakObject(priority);
	if (object == nullptr){ return nullptr; }

	object->_Pos = Vector3(pos,0);
	object->_Size = Vector3(size,0);
	object->SetTexture(GetTexture(texID));
	object->_Color = WHITE(1.0f);
	object->Active = false;
	object->frame = 0;

	return object;
}

void BreakObject::Update(void)
{
	if (Active)
	{
		_Rot.z += DEG2RAD(10.0f);
		frame++;
		if (frame > 120)
		{
			_Color.a -= 0.02f;
			if (_Color.a <= 0)
			{
				ReleaseFlag = true;
			}
		}
	}

	Sprite::Update();

}

void BreakObject::HitAffect(void)
{
	if (_Player != nullptr)
	{
		if (!Active)
		{
			D3DXVECTOR3 vec(0,0,0);
			float speed = 0;
			vec = _Player->Speed();
			speed = D3DXVec3LengthSq(&vec);
			if (speed > 90.0f)
			{
				Active = true;
				float angle = atan2(vec.y,vec.x) + Randf(DEG2RAD(-70.0f),DEG2RAD(70.0f));
				_Speed.x = cosf(angle)*10.0f;
				_Speed.y = sinf(angle)*10.0f;
			}
		}
	}
}