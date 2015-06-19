#pragma once
#ifndef _STAGE_1_H_
#define _STAGE_1_H_

#include "Stage.h"

#define ITEM_OFSET (80.0f)

class Stage_1 : public Stage
{
public:
	Stage_1();
	~Stage_1();

	void Init(void);

	void Release(void);
	void Update(void);

	static Stage_1 *Create();

	static void HitGimmick(bool hit){ hitChk = hit; }
	static bool hitChk;

private:
};

#endif