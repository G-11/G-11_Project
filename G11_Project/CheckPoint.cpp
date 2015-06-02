#include "CheckPoint.h"
#include "CheckEffect.h"
const D3DXVECTOR2 CheckPoint::DefaultSize = D3DXVECTOR2(170.0f,128.0f);
List<CheckPoint> CheckPoint::_HitList;
CheckPoint::CheckPoint(int priority):Sprite(priority)
{
	SelfIterator = nullptr;
	SelfIterator = _HitList.Add(this);
}

CheckPoint::~CheckPoint()
{

}

CheckPoint* CheckPoint::Create(const D3DXVECTOR2& pos)
{
	CheckPoint* point = new CheckPoint;
	if (point == nullptr){ return nullptr; }

	point->_Pos = Vector3(pos,0);
	point->_Size = Vector3(DefaultSize,1.0f);
	point->Texture = GetTexture(TEX_EFFECT_CHECKPOINT);
	point->SetUV(0,0,2,30);

	return point;
}

void CheckPoint::Update(void)
{
	if (frame % 4 == 0)
	{
		uv.y += uv.w;
		if (uv.y >= 1.0f)
		{
			uv.y = uv.y-1.0f;
		}
	}
	_Offset.y = cosf(DEG2RAD(frame))*0.2f;
	frame++;
	if (frame >= 360)
	{
		frame = 0;
	}
}

void CheckPoint::SetActive(bool flag)
{
	_Active = flag;
	(!_Active) ? uv.x = 0 : uv.x = uv.z;
	if (_Active)
	{
		CheckEffect::Creates(_Pos,&_Pos,240.0f,60,5);
	}
	
}