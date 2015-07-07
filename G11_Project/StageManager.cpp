#include "StageManager.h"
#include "main.h"
#include "Stage_1.h"
#include "Stage_2.h"

STAGE_MODE StageManager::stageMode = STAGE_MODEMAX;
Stage *StageManager::_CurrentStage = nullptr;

//=============================================================================
//デストラクタ
//=============================================================================
StageManager::~StageManager(void)
{
	SafeDelete(_CurrentStage);
}

//=============================================================================
//初期化
//=============================================================================
void StageManager::Init(STAGE mode)
{
	SetStage(mode);
}

//=============================================================================
//更新
//=============================================================================
void StageManager::Update(void)
{
	_CurrentStage->Update();
}

//=============================================================================
//ステージ変更
//=============================================================================
void StageManager::SetStage(STAGE mode)
{
	if (stageMode == mode)
		return;

	SafeDelete(_CurrentStage);

	switch (mode)
	{
	case STAGE_MODE1:
		_CurrentStage = Stage_1::Create();
		break;

	case STAGE_MODE2:
		_CurrentStage = Stage_2::Create();
		break;

		//case STAGE_MODE3:
		//_CurrentStage = Stage_3::Create();
		//	break;

	default:
		break;

	}
}