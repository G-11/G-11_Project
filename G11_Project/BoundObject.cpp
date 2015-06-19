#include "BoundObject.h"

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