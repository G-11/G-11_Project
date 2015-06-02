#include "Gimmick.h"
#include "StartDevice.h"

List<Gimmick> Gimmick::_GimmickList;

Gimmick::Gimmick(int priority) :Wall(priority)
{
	TrigAction = nullptr;
	_Active = false;
	_Frame = 0;
	DeviceList.clear();
	SelfIterator = nullptr;
	SelfIterator = _GimmickList.Add(this);
}

Gimmick::~Gimmick()
{
	DeviceList.clear();
	_GimmickList.Delete(SelfIterator);
	SelfIterator = nullptr;
}

Gimmick* Gimmick::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	Gimmick* gimmick = new Gimmick(priority);
	if (gimmick == nullptr) { return nullptr; }
	gimmick->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	gimmick->_Size = D3DXVECTOR3(size.x,size.y,1.0f);

	return gimmick;
}

void Gimmick::Update(void)
{
	CheckDevice();
	if (_Active)
	{
		if (TrigAction != nullptr)
		{
			TrigAction(this);
		}
		_Frame++;
	}
	
	Wall::Update();
}

void Gimmick::CheckDevice(void)
{
	bool active = true;
	int num = DeviceList.size();
	for (int cnt=0;cnt<num;cnt++)
	{
		if (!DeviceList[cnt]->Active())
		{
			active = false;
			break;
		}
	}
	_Active = active;
}

void Gimmick::AddStartDevice(StartDevice* device)
{
	DeviceList.push_back(device);
}