#include "BoundObject.h"
#include "Star.h"
#include "Player.h"

Player* BoundObject::_Player = nullptr;

BoundObject* BoundObject::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,TEX texID,int count,int priority)
{
	BoundObject* object = new BoundObject(priority);
	if (object == nullptr){ return nullptr; }

	object->_Pos = Vector3(pos,0);
	object->_Size = Vector3(size,1.0f);
	object->Texture = GetTexture(texID);
	object->Count = count;
	object->TrigAction = nullptr;
	object->Active = nullptr;

	return object;
}

void BoundObject::Update(void)
{
	if (Active)
	{
		TrigAction(this);
	}
}

bool BoundObject::HitAffect(void)
{
	if (Count > 0)
	{
		Count --;
		float angle = atan2(_Player->Speed().y,_Player->Speed().x) + PI + DEG2RAD(Randf(-30.0f,30.0f));
		Star::CreateNumber(vector2(_Player->Pos().x,_Player->Pos().y),100.0f,angle,Count,60);
	}
	if (Count <= 0)
	{
		Active = true;
	}

	return false;
}

void BoundObject::CastMatrix(void)
{
	Sprite::CastMatrix();

}