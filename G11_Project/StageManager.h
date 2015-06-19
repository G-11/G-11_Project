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

class Stage;

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

private:

};


#endif