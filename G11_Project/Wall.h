#pragma once
#ifndef _WALL_H_
#define _WALL_H_
#include "Sprite.h"


class Wall :public Sprite
{
public:
	Wall(int priority);
	virtual ~Wall();

	static Wall* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,TEX texId,int priority = Sprite::LAYER_0);

	static List<Wall>* HitList(void){return &_HitList; }
	static void InitHitList(void){ _HitList.Clear(); }

	virtual void HitAffect(void);
	virtual void Update();

protected:
	static List<Wall> _HitList;
	VALUE<Wall>* SelfIterator;

	D3DXVECTOR3 OldPos;
};

#endif