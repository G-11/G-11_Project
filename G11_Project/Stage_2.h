#pragma once
#ifndef _STAGE_2_H_
#define _STAGE_2_H_

#include "Stage.h"

#define ITEM_OFSET (80.0f)

class Stage_2 : public Stage
{
public:
	Stage_2();
	~Stage_2();

	void Init(void);

	void Release(void);
	void Update(void);

	static Stage_2 *Create();

	static void HitGimmick(bool hit){ hitChk = hit; }
	static bool hitChk;

private:
};

#endif