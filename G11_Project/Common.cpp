#include "Common.h"

D3DXVECTOR3 Vector3(const D3DXVECTOR2& vector2,float z)
{
	return D3DXVECTOR3(vector2.x,vector2.y,z);
}

D3DXVECTOR3 Vector3Zero(void)
{
	return D3DXVECTOR3(0,0,0);
}