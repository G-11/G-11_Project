#include "BreakObject.h"
#include "Player.h"
#include "Collision.h"
#include "Star.h"
#include "Clog.h"

Player* BreakObject::_Player = nullptr;

BreakObject* BreakObject::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,TEX texID,bool player,int priority)
{
	BreakObject* object = new BreakObject(priority);
	if (object == nullptr){ return nullptr; }

	object->_Pos = Vector3(pos,0);
	object->_Size = Vector3(size,0);
	object->SetTexture(GetTexture(texID));
	object->_Color = WHITE(1.0f);
	object->_Active = false;
	object->frame = 0;
	object->_HitPlayer = player;

	return object;
}

void BreakObject::Update(void)
{
	if (_Active)
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
	else if (!_HitPlayer)
	{
		HitClog();
	}
	Sprite::Update();

}

bool BreakObject::HitAffect(void)
{
	if (_Active)
	{
		return true;
	}
	if (_HitPlayer)
	{
		if (_Player != nullptr)
		{
			return HitPlayer();
		}
	}
	return false;
}

bool BreakObject::HitPlayer(void)
{
	if (!_Active)
	{
		D3DXVECTOR3 vec(0,0,0);
		float speed = 0;
		vec = _Player->Speed();
		speed = D3DXVec3LengthSq(&vec);
		if (speed > 30.0f)
		{
			_Active = true;
			float angle = atan2(vec.y,vec.x) + Randf(DEG2RAD(-70.0f),DEG2RAD(70.0f));
			_Speed.x = cosf(angle)*10.0f;
			_Speed.y = sinf(angle)*10.0f;
			for (int cnt = 0;cnt < 4;cnt++)
			{
				float starAngle = angle + Randf(DEG2RAD(-45.0f),DEG2RAD(45.0f));
				float size = Randf(20.0f,60.0f);
				Star::Create(D3DXVECTOR2(_Player->Pos().x,_Player->Pos().y),D3DXVECTOR2(size,size),starAngle);
			}
			return true;
		}
	}
	else
	{
		return true;
	}

	return false;
}

bool BreakObject::HitClog(void)
{
	VALUE<Clog>* clog = Clog::HitList()->Begin();

	while (clog)
	{
		if (!clog->Data->Have())
		{
			if (Collision::Quad(_Pos,_Quad,4,clog->Data->Pos(),clog->Data->Quad(),4))
			{
				D3DXVECTOR3 vec(0,0,0);
				float speed = 0;
				vec = clog->Data->Speed();
				speed = D3DXVec3LengthSq(&vec);

				_Active = true;
				float angle = -(atan2(vec.y,vec.x) + Randf(DEG2RAD(-70.0f),DEG2RAD(70.0f)));
				_Speed.x = sinf(angle)*10.0f;
				_Speed.y = cosf(angle)*10.0f;
				for (int cnt = 0;cnt < 4;cnt++)
				{
					float starAngle = angle + Randf(DEG2RAD(-45.0f),DEG2RAD(45.0f));
					float size = Randf(20.0f,60.0f);
					Star::Create(D3DXVECTOR2(clog->Data->Pos().x,clog->Data->Pos().y),D3DXVECTOR2(size,size),starAngle);
				}
				return true;
			}
		}
		clog = clog->_Next;
	}

	return false;

}