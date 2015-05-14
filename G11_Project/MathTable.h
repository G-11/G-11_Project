#ifndef _MATH_TABLE_H_
#define _MATH_TABLE_H_

#include <stdio.h>
#include <math.h>
#include <random>

// 角度の正規化(PI～-PI)
#define REVISE_PI(radian) {if((radian) > PI){(radian) -= (2.0f * PI);}\
		else if((radian) < -PI){(radian) += (2.0f * PI);}}


// 角関連
#define PI     (3.141592654f) // 円周率
#define RAD_TO_DEG   (57.29577951f) // 1ラジアン→度
#define DEG_TO_RAD   (0.017453293f) // 1度→ラジアン

// 度→ラジアン
#define DEG2RAD(degree)  ((degree) * DEG_TO_RAD)
// ラジアン→度
#define RAD2DEG(radian)  ((radian) * RAD_TO_DEG)

class CMath
{
private:
	float _tsin[360];//サインテーブル
	float _tcos[360];//コサインテーブル
	float _ttan[360];//タンジェントテーブル
	float _tatan[513];

	//最適化関数
	int Optimize(int deg);
	int RadOptimize(float rad);

	CMath(const CMath &src);

	static std::mt19937 MTRand;

public:

	CMath();//コンストラクタ


	//テーブルからの取得
	//ディグリー
	float tSin_deg(int deg);
	float tCos_deg(int deg);
	float tTan_deg(int deg);
	//ラジアン
	float tSin_rad(float rad);
	float tCos_rad(float rad);
	float tTan_rad(float rad);
	float tAtan2(float index){ return _tatan[(int)(index)]; }

	static float RandomF(float min,float max);
	static int RandomI(int min,int max);

};

//引数が(PI/180)をかけてある場合はこっち
float Sin_Rad(float rad);
float Cos_Rad(float rad);
float Tan_Rad(float rad);

//引数が30°や45°のような整数ならこっち
float Sin_Deg(int deg);
float Cos_Deg(int deg);
float Tan_Deg(int deg);
float Atan2(float y,float x);
float t_sqrtF(const float& x);

template <class type>
type Random(type min,type max)
{
	if (typeid(type) == typeid(int))
	{
		return CMath::RandomI(min,max);
	}
	else
	{
		return CMath::RandomF(min,max);
	}
}


#endif