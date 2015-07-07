#include "StartDevice.h"
#include "Collision.h"
#include "Player.h"
#include "Game.h"
#include "Clog.h"

Player* StartDevice::_Player = nullptr;
List<StartDevice> StartDevice::_StartDeviceList;

StartDevice::StartDevice(int priority) :Sprite(priority)
{
	_Active = false;
	SelfIterator = nullptr;
	SelfIterator = _StartDeviceList.Add(this);
	frame = 0;
	TriggerType = TRIGGER;
	_HitClog = true;
}

StartDevice::~StartDevice()
{
	_StartDeviceList.Delete(SelfIterator);
	SelfIterator = nullptr;
}

StartDevice* StartDevice::CreateTrigger(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	StartDevice* device = new StartDevice(priority);
	if (device == nullptr){ return nullptr; }

	device->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	device->_Rot = D3DXVECTOR3(0,0,0);
	device->_Size = D3DXVECTOR3(size.x,size.y,1.0f);
	device->TriggerType = TRIGGER;

	return device;
}

StartDevice* StartDevice::CreateON(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	StartDevice* device = new StartDevice(priority);
	if (device == nullptr){ return nullptr; }

	device->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	device->_Rot = D3DXVECTOR3(0,0,0);
	device->_Size = D3DXVECTOR3(size.x,size.y,1.0f);
	device->TriggerType = ON;

	return device;
}

StartDevice* StartDevice::CreateTimer(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int timeLimit,int priority)
{
	StartDevice* device = new StartDevice(priority);
	if (device == nullptr){ return nullptr; }

	device->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	device->_Rot = D3DXVECTOR3(0,0,0);
	device->_Size = D3DXVECTOR3(size.x,size.y,1.0f);
	device->TriggerType = TIMER;
	device->TimeLimit = timeLimit;

	return device;

}

StartDevice* StartDevice::CreateSwitch(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	StartDevice* device = new StartDevice(priority);
	if (device == nullptr){ return nullptr; }

	device->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	device->_Rot = D3DXVECTOR3(0,0,0);
	device->_Size = D3DXVECTOR3(size.x,size.y,1.0f);
	device->TriggerType = SWITCH;

	return device;
}

StartDevice* StartDevice::CreateCommand(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	StartDevice* device = new StartDevice(priority);
	if (device == nullptr){ return nullptr; }

	device->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	device->_Rot = D3DXVECTOR3(0,0,0);
	device->_Size = D3DXVECTOR3(size.x,size.y,1.0f);
	device->TriggerType = COMMAND;

	return device;
}

void StartDevice::Update(void)
{
	//ƒvƒŒƒCƒ„[‚Æ‚Ì“–‚½‚è”»’è‚ðŒ©‚Ä_Active‚ðtrue‚É‚·‚é
	bool buffActive;

	if (Collision::Circle(_Player->Pos(), _Player->Size().x*0.25f, _Pos, _Size.x*0.5f))
	{
		buffActive = true;
		if (TriggerType != SWITCH)
		{
			if (TriggerType == COMMAND)
			{
				if (VC::Instance()->Trigger(COMMAND_A)){ _Active = true; }
			}
			else
			{
				_Active = true;
			}
		}
		if (TriggerType == TIMER)
		{
			frame = TimeLimit;
		}
	}
	else
	{
		buffActive = false;
		
		if (TriggerType == ON)
		{
			_Active = false;
		}
	}
	if (TriggerType == SWITCH)
	{
		if ((OldActive^buffActive)& buffActive)
		{
			(_Active)?_Active = false: _Active = true;
		}
	}
	if (TriggerType == TIMER && _Active)
	{
		if (frame > 0)
		{
			frame--;
		}
		if (frame <= 0)
		{
			_Active = false;
		}
	}
	if (_Active == false)
	{
		if (_HitClog)
		{
			buffActive = HitClog();
		}
	}

	OldActive = buffActive;
}

bool StartDevice::HitClog(void)
{
	VALUE<Clog>* clog = Clog::HitList()->Begin();

	bool buffActive = false;
	while (clog)
	{
		if (Collision::Circle(clog->Data->Pos(),clog->Data->Size().x*0.5f,_Pos,_Size.x*0.5f))
		{
			buffActive = true;
			if (TriggerType != SWITCH)
			{
				_Active = true;
			}
			if (TriggerType == TIMER)
			{
				frame = TimeLimit;
			}
		}
		else
		{
			buffActive = false;

			if (TriggerType == ON)
			{
				_Active = false;
			}
		}
		if (TriggerType == SWITCH)
		{
			if ((OldActive^buffActive)& buffActive)
			{
				(_Active) ? _Active = false : _Active = true;
			}
		}
		if (TriggerType == TIMER && _Active)
		{
			if (frame > 0)
			{
				frame--;
			}
			if (frame <= 0)
			{
				_Active = false;
			}
		}
		if (buffActive)
		{
			break;
		}
		clog = clog->_Next;
	}

	return buffActive;
}