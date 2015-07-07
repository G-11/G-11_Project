#include "FixedCamera.h"
#include "Camera2D.h"
#include "Collision.h"
#include "Player.h"


FixedArea* FixedArea::Create(const vector2& pos,const vector2& size)
{
	FixedArea* device = new FixedArea;
	if (device == nullptr){ return nullptr; }

	device->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	device->_Rot = D3DXVECTOR3(0,0,0);
	device->_Size = D3DXVECTOR3(size.x,size.y,1.0f);
	device->TriggerType = ON;
	device->_Color.a = 0;

	return device;
}

void FixedArea::Update(void)
{
	if (Collision::CircleQuad(_Player->Pos(),_Player->Size().x*0.25f,_Quad,4))
	{
		_Active = true;
	}
	else
	{
		_Active = false;
	}

}

FixedCamera* FixedCamera::Create(const vector2& pos,const vector2& fixedPos)
{
	FixedCamera* gimmick = new FixedCamera;
	if (gimmick == nullptr){ return nullptr; }

	gimmick->_Pos = Vector3(pos,0);
	gimmick->FixedPos = fixedPos;
	gimmick->_Size = vector3(1.0f,1.0f,1.0f);
	gimmick->_Color.a = 0;
	
	return gimmick;
}

void FixedCamera::Update(void)
{
	CheckDevice();
	Camera2D* camera = Camera2D::GetCamera(0);
	if (_Active)
	{
		camera->SetDestPos(Vector3(FixedPos,0));
		camera->SetFiexed(true);
		_Frame++;
	}
	else
	{
		camera->SetFiexed(false);
	}

	Wall::Update();
}