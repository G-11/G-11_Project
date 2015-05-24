#pragma once
#ifndef _STAGE_H_
#define _STAGE_H_

class Gimmick;

class Stage
{
protected:
	
	virtual void Uninit(void){};
	virtual void Update(void){};

	Gimmick *_gimmick;
	int gmkNum;

public:

	enum
	{
		STAGE_MODE0 = 0,
		STAGE_MODE1,
		STAGE_MODE2,
		STAGE_MODE3,
		STAGE_MODEMAX
	};

	Stage();
	~Stage(){};

	//static Stage *Create();
};

#endif