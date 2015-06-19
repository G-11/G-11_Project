//=============================================================================
//数字クラス
//=============================================================================
//=============================================================================
//インクルード
//=============================================================================
#include "Number.h"

//=============================================================================
//生成
//=============================================================================
Number* Number::Create(D3DXVECTOR3 pos,D3DXVECTOR2 size,int num,int priority)
{
	Number* number = new Number(priority);
	if (number == nullptr)
	{
		return nullptr;
	}
	number->_Pos = pos;
	number->_Size = D3DXVECTOR3(size.x,size.y,0);
	number->_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	number->SetUV(num,0,1,10);
	number->_Number = num;
	number->Texture = GetTexture(TEX_NUMBER);
	number->DestUV_xy.x = number->uv.x;
	number->DestUV_xy.y = number->uv.y;

	return number;
}
void Number::Update(void)
{
	uv.x += (DestUV_xy.x - uv.x)*0.05f;
	uv.y += (DestUV_xy.y - uv.y)*0.05f;
	if (uv.y < -0.05f)
	{
		uv.y = 1.0f+uv.y;
		DestUV_xy.y = _Number*0.1f;
	}
	if (uv.y >= 0.995f)
	{
		uv.y = -(1.0f - uv.y);
		DestUV_xy.y = _Number*0.1f;
	}
	
}
//=============================================================================
//数字を設定
//=============================================================================
void Number::SetNum(int num)
{
	_Number = num;
	//if (num != 0)
	//{
		DestUV_xy.y = num*0.1f;
	//}
	if (num == 0 && uv.y >= 0.85f)
	{
		DestUV_xy.y = 1.0f;
	}
	if (num == 9 && uv.y < 0.05f)
	{
		DestUV_xy.y = -0.1f;
	}
	
	
}