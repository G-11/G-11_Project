//=============================================================================
//スコアクラス
//=============================================================================
//=============================================================================
//インクルード
//=============================================================================
#include "Counter.h"
#include "Number.h"

//=============================================================================
//生成
//=============================================================================
Counter* Counter::Create(const D3DXVECTOR3& pos,const D3DXVECTOR2& size,int CountPlace,bool camma)
{
	if(CountPlace > 20)
	{
		return nullptr;
	}

	Counter* Score = new Counter;

	if(Score == nullptr)
	{
		return nullptr;
	}
	Score->MaxScore = 1;
	Score->_Pos = Score->DestPos = pos;
	Score->CurrentScore = 0;
	Score->CountPlace = CountPlace;
	Score->Size = size;
	Score->MoveFrame = 1.0f;
	Score->_Color = WHITE(1.0f);
	float PosX = pos.x + (size.x*(CountPlace -1));
	for(int cnt = CountPlace - 1,Loop = 0; cnt >= 0; cnt--,Loop++)
	{
		Score->number[Loop] = Number::Create(D3DXVECTOR3(PosX,pos.y,0),size,0,Sprite::LAYER_INTERFACE);
		PosX -= size.x;
		Score->MaxScore *= 10;//桁数に合わせて最大値を計算する
	}
	return Score;

}
//=============================================================================
//更新
//=============================================================================
void Counter::Update(void)
{
	if(TargetScore > MaxScore-1)
	{
		TargetScore = MaxScore-1;
	}
	else if(TargetScore < 0)
	{
		TargetScore = 0;
	}
	if(TargetScore>CurrentScore)
	{//変動する桁数に合わせて変化量を変える
		if (TargetScore - CurrentScore > 100000)
		{
			CurrentScore += 11111;
		}
		else if (TargetScore - CurrentScore > 10000)
		{
			CurrentScore += 1111;
		}
		else if (TargetScore - CurrentScore > 1000)
		{
			CurrentScore+= 111;
		}
		else if(TargetScore - CurrentScore > 100)
		{
			CurrentScore+=11;
		}
		else
		{
			CurrentScore++;
		}
	}
	else if(TargetScore<CurrentScore)
	{
		if (CurrentScore - TargetScore > 100000)
		{
			CurrentScore -= 11111;
		}
		else if (CurrentScore - TargetScore > 10000)
		{
			CurrentScore -= 1111;
		}
		else if (CurrentScore - TargetScore > 1000)
		{
			CurrentScore -= 111;
		}
		else if (CurrentScore - TargetScore > 100)
		{
			CurrentScore -= 11;
		}
		else
		{
			CurrentScore--;
		}
	}

	int num = CurrentScore;
	for(int cnt = 0; cnt<CountPlace; cnt++)
	{//数字クラスに値をセット
		number[cnt]->SetNum(num%10);
		num /= 10;
	}

	_Pos += (DestPos - _Pos)*MoveFrame;


	float posX;
	//位置を修正
	for (int cnt = CountPlace - 1,Loop = 0; cnt >= 0; cnt--,Loop++)
	{
		posX = _Pos.x + Size.x*cnt;
		number[Loop]->SetPos(D3DXVECTOR3(posX,_Pos.y,_Pos.z));
	}
}

//=============================================================================
//開放
//=============================================================================
void Counter::Release(void)
{
	for(int cnt = 0; cnt<CountPlace; cnt++)
	{
		number[cnt]->Release();
		//Number[cnt] = nullptr;
	}

	delete this;
}

void Counter::SetColor(const D3DXCOLOR& color)
{
	_Color = color;
	for (int cnt = 0;cnt < CountPlace;cnt++)
	{
		number[cnt]->SetColor(_Color);
	}

}

void Counter::SetSize(const D3DXVECTOR2& size)
{
	Size = size;

	D3DXVECTOR3 _size(Size.x,Size.y,0);
	for (int cnt = 0;cnt < CountPlace;cnt++)
	{
		number[cnt]->SetSize(_size);
	}
}

void Counter::AddColor(const D3DXCOLOR& color)
{
	_Color += color;
	for (int cnt = 0;cnt < CountPlace;cnt++)
	{
		number[cnt]->SetColor(_Color);
	}

}

void Counter::AddSize(const D3DXVECTOR2& size)
{
	Size += size;

	D3DXVECTOR3 _size(Size.x,Size.y,0);
	for (int cnt = 0;cnt < CountPlace;cnt++)
	{
		number[cnt]->SetSize(_size);
	}
}