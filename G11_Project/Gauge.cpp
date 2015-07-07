#include "Gauge.h"

Gauge::Gauge(int priority) :Sprite(priority)
{
	_Min = _Max = _Current = 0;
	_MaxSize = 0;
	_RequireTime = 30.0f;
	Texture = GetTexture(TEX_NONE);

}

Gauge* Gauge::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,float min,float max,float current,DIRECTION direction,int priority)
{
	Gauge* gauge = new Gauge(priority);
	if (gauge == nullptr){ return nullptr; }
	gauge->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	gauge->_Size = D3DXVECTOR3(size.x,size.y,1.0f);
	gauge->_Min = min;
	gauge->_Max = max;
	gauge->Original = 
	gauge->_Current = gauge->DestCurrent = current;
	gauge->Direction = direction;
	gauge->Init();

	return gauge;
}

void Gauge::Init()
{
	switch (Direction)
	{
	case UP:
		_Offset.y = -0.5f;
		break;
	case DOWN:
		_Offset.y = 0.5f;
		break;
	case LEFT:
		_Offset.x = -0.5f;
		break;
	case RIGHT:
		_Offset.x = 0.5f;
		break;
	default:
		break;
	}

	if (Direction == UP || Direction == DOWN)
	{
		_MaxSize = _Size.y;
		_Size.y = 0;
	}
	else if (Direction == LEFT || Direction == RIGHT)
	{
		_MaxSize = _Size.x;
		_Size.x = 0;
	}
}

void Gauge::Update(void)
{
	if (_Active)
	{
		frame += 1.0f / _RequireTime;
		if (frame > 1.0f)
		{
			frame = 1.0f;
			_Active = false;
		}
		_Current = EaseOut(Original,DestCurrent,frame);
	}
	
	float per = Per();

	if (Direction == UP || Direction == DOWN)
	{
		_Size.y = _MaxSize*per;
	}
	else if (Direction == LEFT || Direction == RIGHT)
	{
		_Size.x = _MaxSize*per;
	}

}