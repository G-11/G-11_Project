#pragma once
#ifndef _GAUGE_H_
#define _GAUGE_H_
#include "Sprite.h"

typedef enum
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	DIRECTION_MAX
}DIRECTION;

class Gauge :public Sprite
{
public:
	Gauge(int priority);

	//=====================================================
	// 引数
	//
	// 起点となる位置
	// 大きさ
	// 最小値
	// 最大値
	// 初期値
	// 伸びる方向
	// プライオリティ
	//=====================================================
	static Gauge* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,float min,float max,float current,DIRECTION direction = UP,int priority = Sprite::LAYER_INTERFACE);
	virtual void Init(void);
	void Update(void);

	float Min(void)const { return _Min; }
	float Max(void)const { return _Max; }
	float Current(void)const { return _Current; }
	float MaxSize(void)const { return _MaxSize; }
	float Per(void)const { return (_Current-_Min)/(_Max-_Min); }
	float Delay(void)const { return _RequireTime; }
	bool Active(void)const { return _Active; }

	void SetMin(float min){ _Min = min; }
	void SetMax(float max){ _Max = max; }
	void SetRequireTime(float delay){ _RequireTime = delay; }

	void SetCurrent(float current){
		frame = 0;
		Original = _Current;
		_Active = true;
		if (current > _Max){
			DestCurrent = _Max;
		}
		else if (current < _Min){
			DestCurrent = _Min;
		}
		else{
			DestCurrent = current;
		}
	}

	void SetMaxSize(float size){ _MaxSize += size; }

	void AddMin(float min){ _Min += min; }
	void AddMax(float max){ _Max += max; }
	void AddRequireTime(float time){ _RequireTime += time; }

	void AddCurrent(float num){
		float current = DestCurrent + num;
		frame = 0;
		_Active = true;
		Original = _Current;
		if (current > _Max){
			DestCurrent = _Max;
		}
		else if (current < _Min){
			DestCurrent = _Min;
		}
		else{
			DestCurrent = current;
		}
	}

	void AddMaxSize(float size){ _MaxSize += size; }

protected:
	float _Min;
	float _Max;
	float _MaxSize;//
	float Original;
	float _Current;
	float DestCurrent;
	float _RequireTime;	//0～1.0f 1に近いほど早く現在の値まで変動する
	float frame;
	bool _Active;
	DIRECTION Direction;

};

#endif