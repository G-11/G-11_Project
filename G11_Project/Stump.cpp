#include "Stump.h"

Stump* Stump::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	Stump* device = new Stump(priority);
	if (device == nullptr){ return nullptr; }

	device->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	device->_Rot = D3DXVECTOR3(0,0,0);
	device->OriginalSize =
	device->_Size = D3DXVECTOR3(size.x,size.y,1.0f);
	device->_Offset.y = -0.5f;
	device->uv.y = 0.01f;
	device->uv.w =  0.98f;
	device->TriggerType = ON;
	device->Texture = GetTexture(TEX_KIRIKABU);

	return device;
}

void Stump::Update(void)
{
	StartDevice::Update();

	if (_Active)
	{
		_Size.y = OriginalSize.y/2.0f;
	}
	else
	{
		_Size.y = OriginalSize.y;
	}
}