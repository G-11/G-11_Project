#include "Gate.h"

Gate* Gate::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,TEX texID,int priority)
{
	Gate* gate = new Gate(priority);
	if (gate == nullptr){ return gate; }

	gate->_Pos = Vector3(pos,0);
	gate->_Size = Vector3(size,1.0f);
	gate->Texture = GetTexture(texID);
	gate->OriginalPos = gate->_Pos;

	return gate;
}

void Gate::Update(void)
{
	Gimmick::Update();

	if (!_Active)
	{
		_Pos += (OriginalPos - _Pos)*0.1f;
	}
}