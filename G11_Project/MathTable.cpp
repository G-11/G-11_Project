//===========================================================
//
// 三角関数テーブル	[MathTable.cpp]
//
//		製作者　新井 裕貴
//
//===========================================================

//===========================================================
//インクルード
//===========================================================
#include "MathTable.h"

std::mt19937 CMath::MTRand;

CMath math;
//===========================================================
//コンストラクタ
//===========================================================
CMath::CMath()
{

	//初期化処理
	int cnt=0;

	while(cnt<360)
	{
		_tsin[cnt] = sinf(cnt*(PI/180.0f));
		if(cnt==0||cnt==180)
		{
			_tsin[cnt] = 0;
		}
		_tcos[cnt] = cosf(cnt*(PI/180.0f));
		if(cnt==90||cnt==270)
		{
			_tcos[cnt] = 0;
		}
		_ttan[cnt] = tanf(cnt*(PI/180.0f));
		cnt++;
	}
	float num = 256.0f;
	for (int cnt = 0;cnt <= 512;cnt++,num--)
	{
		_tatan[cnt] = (float)atan2(num,256);
	}
	std::random_device randDevice;
	MTRand = std::mt19937(randDevice());
}


//===========================================================
//最適化
//===========================================================
int CMath::Optimize(int deg)
{
	deg = deg%360;//360°内に収めさせる

	if(deg<0)//0より小さかったら正位置に最適化
	{
		 deg+=360;
	}

	return deg;
}
//===========================================================
//ラジアンをディグリーに最適化
//===========================================================
int CMath::RadOptimize(float rad)
{
	int radiation = (int)(rad/(PI/180));//ラジアン化の逆
	radiation = radiation%360;

	if(radiation < 0)//0より小さかったら正位置に最適化
	{
		radiation += 360;
	}

	return radiation;
}
//===========================================================
//ディグリーからのsin値取得
//===========================================================
float CMath::tSin_deg(int deg)
{	
	int degree = deg;
	degree = Optimize(degree);

	return _tsin[degree];
}
//===========================================================
//ディグリーからのcos値取得
//===========================================================
float CMath::tCos_deg(int deg)
{
	int degree=deg;
	
	degree = Optimize(degree);

	return _tcos[degree];
}
//===========================================================
//ディグリーからのtan値取得
//===========================================================
float CMath::tTan_deg(int deg)
{
	int degree=deg;
	
	degree = Optimize(degree);

	return _ttan[degree];
}
//===========================================================
//ラジアンからのsin値取得
//===========================================================
float CMath::tSin_rad(float rad)
{
	int deg = RadOptimize(rad);

	return _tsin[deg];
}
//===========================================================
//ラジアンからのcos値取得
//===========================================================
float CMath::tCos_rad(float rad)
{
	int deg = RadOptimize(rad);

	return _tcos[deg];
}
//===========================================================
//ラジアンからのtan値取得
//===========================================================
float CMath::tTan_rad(float rad)
{
	int deg = RadOptimize(rad);

	return _ttan[deg];
}

float CMath::RandomF(float min,float max)
{
	std::uniform_real<float> dist(min,max);
	return dist(MTRand);
}

int CMath::RandomI(int min,int max)
{
	std::uniform_int<int> dist(min,max);
	return dist(MTRand);
}

float Sin_Rad(float rad)
{
	return math.tSin_rad(rad);
}

float Cos_Rad(float rad)
{
	return math.tCos_rad(rad);
}

float Tan_Rad(float rad)
{
	return math.tTan_rad(rad);
}

float Sin_Deg(int deg)
{
	return math.tSin_deg(deg);
}

float Cos_Deg(int deg)
{
	return math.tCos_deg(deg);
}

float Tan_Deg(int deg)
{
	return math.tTan_deg(deg);
}

float Atan2(float y,float x)
{
	if (x <= y)
	{
		if (x >= -y)
		{
			return math.tAtan2(x * (1 << 8) / y + (1 << 8)) + 1.57079637f;
		}
		else
		{
			if (y < 0)
			{
				return math.tAtan2(y * (1 << 8) / -x + (1 << 8)) + 3.1503193865f;
			}
			else
			{
				return math.tAtan2(y * (1 << 8) / -x + (1 << 8)) + PI;
			}

		}
	}
	else
	{
		if (x > -y)
		{
			if (y > 0)
			{
				return math.tAtan2(y * (1 << 8) / -x + (1 << 8));
			}
			else
			{
				return math.tAtan2(y * (1 << 8) / -x + (1 << 8))-0.0087266465f;
			}
		}
		else
		{
			return math.tAtan2(x * (1 << 8) / y + (1 << 8)) - 1.5795230165f;
		}
	}
	return 0;
}

float t_sqrtF(const float& x)
{
	float xHalf = 0.5f * x;
	int   tmp = 0x5F3759DF - (*(int*)&x >> 1); //initial guess
	float xRes = *(float*)&tmp;

	xRes *= (1.5f - (xHalf * xRes * xRes));
	//xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );//コメントアウトを外すと精度が上がる
	return xRes * x;
}