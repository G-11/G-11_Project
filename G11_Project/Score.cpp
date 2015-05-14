//=============================================================================
//スコアクラス
//=============================================================================
//=============================================================================
//インクルード
//=============================================================================
#include "Score.h"
#include "Number.h"

//=============================================================================
//生成
//=============================================================================
CScore* CScore::Create(D3DXVECTOR3 pos,D3DXVECTOR2 size,int CountPlace,bool camma)
{
	if(CountPlace > 20)
	{
		return nullptr;
	}

	CScore* Score = new CScore;

	if(Score == nullptr)
	{
		return nullptr;
	}
	Score->MaxScore = 1;
	Score->Pos = Score->DestPos = pos;
	Score->CurrentScore = 0;
	Score->CountPlace = CountPlace;
	Score->Size = size;
	Score->MoveFrame = 1.0f;
	Score->Color = WHITE(1.0f);
	float PosX = pos.x + (size.x*(CountPlace -1));
	for(int cnt = CountPlace - 1,Loop = 0; cnt >= 0; cnt--,Loop++)
	{
		Score->Number[Loop] = CNumber::Create(D3DXVECTOR3(PosX,pos.y,0),size,0,CPolygon::LAYER_INTERFACE);
		if (camma == true && Loop % 3 == 2 && Loop != CountPlace - 1)
		{
			CPolygon* comma = CPolygon::Create(D3DXVECTOR3(PosX - size.x / 2.0f,pos.y + size.y*0.1f,0),size,WHITE(1.0f),CPolygon::LAYER_INTERFACE);
			comma->SetTexture(GetTexture(TEX_COMMA));
		}
		PosX -= size.x;
		Score->MaxScore *= 10;//桁数に合わせて最大値を計算する
	}
	return Score;

}
//=============================================================================
//更新
//=============================================================================
void CScore::Update(void)
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
		Number[cnt]->SetUV(num % 10,0,10,1);
		num /= 10;
	}

	Pos += (DestPos - Pos)*MoveFrame;


	float posX;
	//位置を修正
	for (int cnt = CountPlace - 1,Loop = 0; cnt >= 0; cnt--,Loop++)
	{
		posX = Pos.x + Size.x*cnt;
		Number[Loop]->SetPos(D3DXVECTOR3(posX,Pos.y,Pos.z));
	}
}

//=============================================================================
//開放
//=============================================================================
void CScore::Release(void)
{
	for(int cnt = 0; cnt<CountPlace; cnt++)
	{
		Number[cnt]->Release();
		//Number[cnt] = nullptr;
	}

	delete this;
}

void CScore::SetColor(D3DXCOLOR color)
{
	Color = color;
	for (int cnt = 0;cnt < CountPlace;cnt++)
	{
		Number[cnt]->SetColor(Color);
	}

}

void CScore::SetSize(D3DXVECTOR2 size)
{
	Size = size;

	D3DXVECTOR3 _size(Size.x,Size.y,0);
	for (int cnt = 0;cnt < CountPlace;cnt++)
	{
		Number[cnt]->SetSize(_size);
	}
}

void CScore::AddColor(D3DXCOLOR color)
{
	Color += color;
	for (int cnt = 0;cnt < CountPlace;cnt++)
	{
		Number[cnt]->SetColor(Color);
	}

}

void CScore::AddSize(D3DXVECTOR2 size)
{
	Size += size;

	D3DXVECTOR3 _size(Size.x,Size.y,0);
	for (int cnt = 0;cnt < CountPlace;cnt++)
	{
		Number[cnt]->SetSize(_size);
	}
}