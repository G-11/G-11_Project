#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_

#include "Stage.h"
typedef enum
{
	STAGE_0 = 0,
	STAGE_1,
	STAGE_2,
	STAGE_3,
	STAGE_MAX
}STAGE;

class Interface;

class StageManager
{
public:
	StageManager(){};
	~StageManager();

	void Init(STAGE mode);
	void Update();

	static void SetStage(STAGE mode);

	static STAGE_MODE stageMode;
	static Stage* _CurrentStage;

	static Interface* Interface(){ return _CurrentStage->GetInterface(); }

private:

};


#endif