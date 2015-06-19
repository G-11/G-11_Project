#pragma once
#ifndef _CLOG_H_
#define _CLOG_H_

#include "Sprite.h"

class Clog :public Sprite
{
public:
	Clog(int priority);
	~Clog();
	static Clog* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,int priority = Sprite::LAYER_2);

	void Update(void);

	void SetHave(bool flag){ _Have = flag; }
	bool Have(void){return _Have; }

	static List<Clog>* HitList(void){ return &_HitList; }
private:
	static List<Clog> _HitList;
	VALUE<Clog>* SelfIterator;
	bool _Have;
};

#endif