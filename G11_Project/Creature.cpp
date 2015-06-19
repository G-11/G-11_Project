#include "Creature.h"
#include "Wall.h"
#include "Collision.h"

List<Creature> Creature::_HitList;

Creature::Creature(int priority) :Sprite(priority)
{
	SelfIterator = nullptr;
	SelfIterator = _HitList.Add(this);
}

Creature::~Creature()
{
	_HitList.Delete(SelfIterator);
	SelfIterator = nullptr;
}

void Creature::Update(void)
{
	_Speed.y += 0.48f;
	if (HitWall())
	{
		_Speed.y = 0;
	}
	OldPos = _Pos;
}

bool Creature::HitWall(void)
{
	VALUE<Wall>* wall = Wall::HitList()->Begin();
	bool hit = false;
	vector3 speed(0,0,0);
	while (wall)
	{
		if (Collision::CircleQuad(_Pos,_Size.y/2.0f,wall->Data->Quad(),4,_Speed,&ReflectVec))
		{
			D3DXVECTOR3 wallSpeed = wall->Data->Speed();
			//壁の移動ベクトルからプレイヤー方向の成分のみを加算
			D3DXVECTOR3 playerwall = _Pos - wall->Data->Pos();
			D3DXVec3Normalize(&playerwall,&playerwall);
			float wallReflect = D3DXVec3Dot(&playerwall,&wallSpeed);

			//反射ベクトルをランダム変化させる

			D3DXVECTOR3 speed = _Speed;
			float angle = atan2(ReflectVec.x,ReflectVec.y);
			float sp = D3DXVec3Length(&(speed + playerwall));
			if (ReflectVec.y < 0.0f)
			{
				_Speed.y = 0;
				_Pos.y = OldPos.y;
			}
			if (ReflectVec.x > 1.0f || ReflectVec.x < -1.0f)
			{
				_Speed.x *= -1.0f;
				uv.z *= -1.0f;
			}
			
			angle = RAD2DEG(angle);
			
			hit = true;
		}
		wall = wall->_Next;
	}
	if (hit)
	{
		//_Speed.x -= speed.x;
		//_Speed.y = 0;
	}
	return hit;
}

void Creature::Capture(void)
{

}