#include "GrowupEffect.h"
#include "Orbit2D.h"
#include "Shader2D.h"

GrowupEffect* GrowupEffect::Create(const D3DXVECTOR3& pos,D3DXVECTOR3* parentPos,float r,int frame,short pass)
{
	GrowupEffect* effect = new GrowupEffect;
	if (effect == nullptr){ return nullptr; }

	effect->_Pos = pos;
	effect->_Size = D3DXVECTOR3(18.0f,18.0f,1.0f);
	effect->_Pass = pass;
	effect->Texture = GetTexture(TEX_EFFECT_GROWUP);
	effect->Radius = r;
	effect->RequireFrame = (float)frame;
	effect->ParentPos = parentPos;
	effect->Init();
	return effect;

}

void GrowupEffect::Creates(const D3DXVECTOR3& pos,D3DXVECTOR3* parentPos,float r,int frame,int num,short pass)
{
	for (int cnt = 0;cnt < num;cnt++)
	{
		if (Create(pos,parentPos,r,frame,pass) == nullptr)
		{
			return;
		}
	}
}

void GrowupEffect::Init(void)
{
	StartPos = D3DXVECTOR2(_Pos.x,_Pos.y);
	for (int cnt = 0;cnt < 2;cnt++)
	{
		float angle = Randf(0.0f,PI*2.0f);
		float r = Radius + (-Radius / 2.0f,Radius/2.0f);
		RelayPoint[cnt].x = sinf(angle)*r+StartPos.x;
		RelayPoint[cnt].y = cosf(angle)*r+StartPos.y;
	}
	frame = 0;


}
void GrowupEffect::Update(void)
{
	float per = (float)(frame / RequireFrame);

	if (per > 1.0f)
	{
		SetRelease();
		return;
	}
	D3DXVECTOR2 destPos(ParentPos->x,ParentPos->y+30.0f);
	_Pos = Vector3(Bezier(StartPos,destPos,RelayPoint[0],RelayPoint[1],per),0);

	frame ++;

}