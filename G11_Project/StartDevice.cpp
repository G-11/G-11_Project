#include "StartDevice.h"

Player* StartDevice::_Player = nullptr;

StartDevice::StartDevice(int priority) :Sprite(priority)
{
	_Active = false;
}

StartDevice* StartDevice::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	StartDevice* device = new StartDevice(priority);
	if (device == nullptr){ return nullptr; }

	device->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	device->_Rot = D3DXVECTOR3(0,0,0);
	device->_Size = D3DXVECTOR3(size.x,size.y,1.0f);

	return device;
}

void StartDevice::Update(void)
{
	//プレイヤーとの当たり判定を見て_Activeをtrueにする

	if (_Active)//アクティブ状態を仮置き
	{
		_Color = RED(1.0f);
	}
}
