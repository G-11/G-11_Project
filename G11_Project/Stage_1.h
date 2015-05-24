#pragma once
#ifndef _STAGE_1_H_
#define _STAGE_1_H_

#include "Stage.h"

class Stage_1 : public Stage
{
public:
	Stage_1();
	~Stage_1();

	void Uninit(void);
	void Update(void);

	static Stage_1 *Create();

private:

};

#endif