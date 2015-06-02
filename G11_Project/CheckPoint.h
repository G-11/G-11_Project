#pragma once
#ifndef _CHECK_POINT_H_
#define _CHECK_POINT_H_
#include "Sprite.h"

class CheckPoint :public Sprite
{
public:
	CheckPoint(int priority = Sprite::LAYER_4);
	virtual ~CheckPoint();
	static CheckPoint* Create(const D3DXVECTOR2& pos);

	void Update(void);

	bool Active(void)const { return _Active; }
	static List<CheckPoint>* HitList(void){ return &_HitList; }

	void SetActive(bool flag);

private:
	static List<CheckPoint> _HitList;
	VALUE<CheckPoint>* SelfIterator;
	bool _Active;

	const static D3DXVECTOR2 DefaultSize;

};

#endif