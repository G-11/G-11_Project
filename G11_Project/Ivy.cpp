#include "Ivy.h"
#include "CutEffect.h"

Ivy* Ivy::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority)
{
	Ivy* ivy = new Ivy(priority);
	if (ivy == nullptr){ return nullptr; }

	ivy->_Pos = Vector3(pos,0);
	ivy->_Size = Vector3(size,1.0f);
	ivy->_Rot = D3DXVECTOR3(0,0,0);
	ivy->Texture = GetTexture(TEX_TUTA);
	ivy->TriggerType = COMMAND;
	ivy->EffectEmited = false;

	return ivy;
}

void Ivy::Update(void)
{
	StartDevice::Update();

	if (_Active)
	{
		if (!EffectEmited)
		{
			CutEffect::Create(D3DXVECTOR2(_Pos.x,_Pos.y),D3DXVECTOR2(64.0f,64.0f),true);
			CutEffect::Create(D3DXVECTOR2(_Pos.x,_Pos.y),D3DXVECTOR2(64.0f,64.0f),false);
			EffectEmited = true;
		}
		_Pos.y -= 1000.0f;
	}
}