#include "Game.h"
#include "Sprite.h"
#include "Gauge.h"
#include "Camera2D.h"
#include "Light2D.h"
#include "Renderer.h"
#include "Manager.h"
#include "Pause.h"
#include "Shader2D.h"

#include "Player.h"
#include "Item.h"

#include "Wall.h"
#include "ScreenRender.h"

#include "StartDevice.h"
#include "BreakObject.h"
#include "BoundObject.h"
#include "CheckPoint.h"

#include "StageManager.h"
#include "Stage.h"
#include "Stage_1.h"
#include "Goal.h"

#ifdef _DEBUG
#include "Input/Keyboard.h"
#include "Clog.h"
#include "PlayerLight.h"
bool Game::Hit = false;

#endif

bool Game::_PauseFlag = false;

Player			*Game::_player = nullptr;
Item			*Game::_item = nullptr;
StageManager	*Game::Stage = nullptr;
Pause*			Game::_Pause = nullptr;

float time = 0;
void Game::Init(void)
{
	Camera2D* camera = Camera2D::GetCamera(0);
	
	Window* window = Window::Instance();

	//ステージ
	Stage = new StageManager;
	Stage->Init(STAGE_1);
	
	// プレイヤー
	_player = Player::Create(D3DXVECTOR3(500.0f, 450.0f, 0), D3DXVECTOR2(158.0f, 158.0f), WHITE(1.0f), Sprite::LAYER_1);
	StartDevice::SetPlayer(_player);
	BreakObject::SetPlayer(_player);
	BoundObject::SetPlayer(_player);
	D3DXVECTOR3 pos = _player->Pos();
	pos.x = 0;
	camera->ResetPos(pos);
	camera->SetDestPos(vector3(0,0,0));

	Goal::SetPlayer(_player);
	Item::SetPlayer(_player);

	camera->SetPlayer(_player);
	_Pause = new Pause();
	_Pause->Init();

}

void Game::Uninit(void)
{
	Camera2D* camera = Camera2D::GetCamera(0);
	camera->SetPlayer(nullptr);
	camera->ResetPos(vector3(0,0,0));
	StartDevice::SetPlayer(nullptr);
	BreakObject::SetPlayer(nullptr);
	BoundObject::SetPlayer(nullptr);
	Item::SetPlayer(nullptr);
	SafeDelete(Stage);
	SafeDelete(_Pause);
	Renderer::SetRenderMode(CScreenRender::MODE_NORMAL);
	Sound::Instance()->Fade(100);
	Manager::ReleaseObject();
}

void Game::Update(void)
{
	if (VC::Instance()->Trigger(COMMAND_START))
	{
		_PauseFlag = ((_PauseFlag) ? false : true);
		Manager::SetPause(_PauseFlag);
	}
	if (_PauseFlag)
	{
		_Pause->PauseUpdate();
	}
	else
	{
		_Pause->Update();

		Stage->Update();

#ifdef _DEBUG

		CDebugProc::Print("0 リザルトへ遷移\n");
		if (VC::Instance()->keyboard()->Trigger(DIK_0) &&
			Fade::Instance()->Mode() == Fade::FADE_NONE)
		{
			Manager::Instance()->SetScene(Manager::SCENE_RESULT);
		}

		if (VC::Instance()->keyboard()->Trigger(DIK_8))
		{
			bool hit = _player->HitCheckFlag();
			_player->SetHitCheckFlag(!hit);
		}

		//アイテム生成
		if (VC::Instance()->keyboard()->Trigger(DIK_F1))
		{
			Item::Create(_player->Pos(),D3DXVECTOR2(84.0f,84.0f),WHITE(1.0f),Randi(1,ITEM_ID_MAX - 1),Sprite::LAYER_2);
		}

		//ゲームリセット
		if (VC::Instance()->keyboard()->Trigger(DIK_9) &&
			Fade::Instance()->Mode() == Fade::FADE_NONE)
		{
			Manager::Instance()->SetScene(Manager::SCENE_GAME);
		}

#endif
	}
}

void Game::Draw(void)
{


}