#include "StartDevice.h"
#include "Collision.h"
#include "Player.h"
#include "Game.h"

Player* StartDevice::_Player = nullptr;
List<StartDevice> StartDevice::_StartDeviceList;

StartDevice::StartDevice(int priority) :Sprite(priority)
{
	_Active = false;
	SelfIterator = nullptr;
	SelfIterator = _StartDeviceList.Add(this);
}

StartDevice::~StartDevice()
{
	_StartDeviceList.Delete(SelfIterator);
	SelfIterator = nullptr;
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
	VALUE<StartDevice>* itr = _StartDeviceList.Begin();

	while (itr)
	{
		if (Collision::Circle(_Player->Pos(), _Player->Size().x*0.5f, itr->Data->_Pos, itr->Data->_Size.x*0.5f))
		{
			_Active = true;
		}

		itr = itr->_Next;
	}

	if (_Active)//アクティブ状態を仮置き
	{
		_Color = RED(1.0f);
	}
}
