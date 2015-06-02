#pragma once
#ifndef _CHECK_EFFECT_H_
#define _CHECK_EFFECT_H_

#include "Sprite.h"
#include "Shader2D.h"

class Orbit2D;

class CheckEffect :public Sprite
{
public:
	CheckEffect(int priority = Sprite::LAYER_3) :Sprite(priority){};
	static CheckEffect* Create(const D3DXVECTOR3& pos,D3DXVECTOR3* parentPos,float r,int frame,short pass = CShader2D::ADD);
	static void Creates(const D3DXVECTOR3& pos,D3DXVECTOR3* parentPos,float r,int frame,int num,short pass = CShader2D::ADD);

	void Update(void);

private:
	void Init(void);

	D3DXVECTOR3* ParentPos;
	float Radius;
	float RequireFrame;
	D3DXVECTOR2 StartPos;
	D3DXVECTOR2 RelayPoint[2];
	Orbit2D* Tail;

};


#endif