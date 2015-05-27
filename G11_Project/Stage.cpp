#include "Stage.h"
#include "main.h"

#include "Stage_1.h"
//#include "Stage_2.h"
//#include "Stage_3.h"

STAGE_MODE Stage::stageMode = STAGE_MODE1;
Stage *Stage::_stage = nullptr;

//=============================================================================
//コンストラクタ
//=============================================================================
Stage::Stage()
{
}

//=============================================================================
//どのステージ（ギミック）か
//=============================================================================
void Stage::SetStage(STAGE_MODE mode)
{
	if (stageMode == mode)
		return;

	if (_stage != nullptr)
		SafeRelease(_stage);

	switch (mode)
	{
	case STAGE_MODE1:
		_stage = Stage_1::Create();
		break;

	//case STAGE_MODE2:
	//	_stage = Stage_2::Create();
	//	break;

	//case STAGE_MODE3:
	//	_stage = Stage_2::Create();
	//	break;

	}
}