#include "Stage.h"
#include "main.h"
#include "Interface.h"
#include "Sprite.h"
#include "Camera2D.h"
#include "Stage_1.h"

#ifdef _DEBUG
#include "Input\Keyboard.h"
#endif
//#include "Stage_2.h"
//#include "Stage_3.h"

Sprite* Stage::_Field = nullptr;

//=============================================================================
//コンストラクタ
//=============================================================================
Stage::Stage()
{
	_Field = nullptr;
	_Interface = nullptr;
	_Camera = Camera2D::GetCamera(0);
}

Stage::~Stage()
{
	SafeRelease(_Field);
	SafeDelete(_Interface);
}

void Stage::Update(void)
{
	if (_Interface != nullptr){

		_Interface->Update();
	}

	if (_Field != nullptr){

		_Field->SetMaskUVX(_Interface->Percent());
		_Field->AddUVX(_Camera->GetSpeed().x*-0.00001f);
	}
#ifdef _DEBUG
	//ゲージリセット
	if (VC::Instance()->keyboard()->Trigger(DIK_F2))
	{
		Interface::AddScore(-Interface::Score());
	}
#endif
}