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
#include "Interface.h"

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

Sprite			*Game::_Field = nullptr;
Wall			*Game::_cilling = nullptr;
Wall			*Game::_floor = nullptr;

Player			*Game::_player = nullptr;
Item			*Game::_item = nullptr;
StageManager	*Game::Stage = nullptr;

float time = 0;
void Game::Init(void)
{
	Camera2D* camera = Camera2D::GetCamera(0);
	
	Window* window = Window::Instance();
	Sound::Instance()->Play(BGM_TITLE);

	//ステージ
	Stage = new StageManager;
	Stage->Init(STAGE_1);
	
	// 背景
	_Field = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), WHITE(1.0f), Sprite::LAYER_BACKGROUND);
	_Field->SetTexture(GetTexture(TEX_FIELD1_BG));

	_Field->SetMask(GetTexture(TEX_SKY_MAP));
	_Field->SetPass(CShader2D::SKY);

	// プレイヤー
	_player = Player::Create(D3DXVECTOR3(200.0f, 500.0f, 0), D3DXVECTOR2(158.0f, 158.0f), WHITE(1.0f), Sprite::LAYER_1);
	StartDevice::SetPlayer(_player);
	BreakObject::SetPlayer(_player);
	D3DXVECTOR3 pos = _player->Pos();
	pos.x = 0;
	camera->ResetPos(pos);

	Goal::SetPlayer(_player);
	Item::SetPlayer(_player);

	camera->SetPlayer(_player);

	_Interface = new Interface;
	_Interface->Init(360000,100);

}

void Game::Uninit(void)
{
	Camera2D* camera = Camera2D::GetCamera(0);
	camera->SetPlayer(nullptr);
	
	
	StartDevice::SetPlayer(nullptr);
	BreakObject::SetPlayer(nullptr);
	Item::SetPlayer(nullptr);
	SafeDelete(Stage);
	SafeDelete(_Interface);
	Renderer::SetRenderMode(CScreenRender::MODE_NORMAL);
	Sound::Instance()->Fade(100);
	Manager::ReleaseObject();
}

void Game::Update(void)
{
	Stage->Update();

	_Interface->Update();
	_Field->SetMaskUVX(_Interface->Percent());

	_Field->AddUVX(Camera2D::GetCamera(0)->GetSpeed().x*-0.00001f);

	if (Pause != nullptr)
	{
		Pause->Update();
	}

#ifdef _DEBUG
	CDebugProc::Print("0 リザルトへ遷移\n");
	if (VC::Instance()->keyboard()->Trigger(DIK_0) &&
		Fade::Instance()->Mode() == Fade::FADE_NONE)
	{
		Manager::Instance()->SetScene(Manager::SCENE_RESULT);
	}

	//アイテム生成
	if (VC::Instance()->keyboard()->Trigger(DIK_F1))
	{
		Item::Create(_player->Pos(), D3DXVECTOR2(84.0f, 84.0f), WHITE(1.0f), Randi(1, ITEM_ID_MAX-1), Sprite::LAYER_2);
	}

	//ゲージリセット
	if (VC::Instance()->keyboard()->Trigger(DIK_F2))
	{
		Interface::AddScore(-Interface::Score());
	}

	//ゲームリセット
	if (VC::Instance()->keyboard()->Trigger(DIK_9) &&
		Fade::Instance()->Mode() == Fade::FADE_NONE)
	{
		Manager::Instance()->SetScene(Manager::SCENE_GAME);
	}

#endif

}

void Game::Draw(void)
{


}