#pragma once
#ifndef _STAGE_H_
#define _STAGE_H_

typedef enum
{
	STAGE_MODE0 = 0,
	STAGE_MODE1,
	STAGE_MODE2,
	STAGE_MODE3,
	STAGE_MODEMAX
}STAGE_MODE;

class Gimmick;

class Stage
{
protected:

	Gimmick *GetGimmick(void){ return _gimmick;}

	Gimmick *_gimmick;
	int gmkNum;

public:


	Stage();
	~Stage(){};

	virtual void Update(void){};
	virtual void Release(void){};

	static STAGE_MODE stageMode;
	void SetStage(STAGE_MODE mode);

private:
	static Stage *_stage;
};

#endif