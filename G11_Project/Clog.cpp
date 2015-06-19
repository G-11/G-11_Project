#include "Clog.h"
#include "Wall.h"
#include "Collision.h"

List<Clog> Clog::_HitList;

Clog::Clog(int priority) :Sprite(priority)
{
	SelfIterator = nullptr;
	SelfIterator = _HitList.Add(this);
	_Have = false;
}

Clog::~Clog()
{
	_HitList.Delete(SelfIterator);
}

Clog* Clog::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	Clog* clog = new Clog(priority);
	if (clog == nullptr){ return clog; }

	clog->_Pos = Vector3(pos,0);
	clog->_Size = Vector3(size,1.0f);
	clog->Texture = GetTexture(TEX_CLOG);

	return clog;
}

void Clog::Update(void)
{
	if (!_Have)
	{
		_Speed.y += 0.49f;
	}

	VALUE<Wall>* wall = Wall::HitList()->Begin();
	while (wall)
	{
		if (Collision::CircleQuad(_Pos,_Size.y/2.0f,wall->Data->Quad(),4))
		{
			_Speed.y = 0;
			break;
		}
		wall = wall->_Next;
	}
	_Pos += _Speed;
}