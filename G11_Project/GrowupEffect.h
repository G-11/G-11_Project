#pragma once
#ifndef _GROWUP_EFFECT_H_
#define _GROWUP_EFFECT_H_

#include "Sprite.h"
#include "Shader2D.h"
class GrowupEffect :public Sprite
{
public:
	GrowupEffect(int priority = Sprite::LAYER_3) :Sprite(priority){};
	static GrowupEffect* Create(const D3DXVECTOR3& pos,D3DXVECTOR3* parentPos,float r,int frame,short pass = CShader2D::NORMAL);
	static void Creates(const D3DXVECTOR3& pos,D3DXVECTOR3* parentPos,float r,int frame,int num,short pass = CShader2D::NORMAL);

	void Update(void);

private:
	void Init(void);

	D3DXVECTOR3* ParentPos;
	float Radius;
	float RequireFrame;
	D3DXVECTOR2 StartPos;
	D3DXVECTOR2 RelayPoint[2];

};


#endif