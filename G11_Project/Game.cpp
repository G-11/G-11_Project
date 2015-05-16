#include "Game.h"
#include "Sprite.h"
#include "Gauge.h"
#include "Camera2D.h"
#include "Renderer.h"
#include "Manager.h"
#include "Pause.h"
#include "Shader2D.h"

#include "Player.h"

#include "Input/Keyboard.h"
#include "Gimmick.h"
#include "StartDevice.h"
#include "Collision.h"

#ifdef _DEBUG

bool Game::Hit = false;

#endif

Camera2D* camera=nullptr;
bool Game::_PauseFlag = false;
static StartDevice* device = nullptr;
Player* player = nullptr;
Sprite* sprite = nullptr;
D3DXVECTOR3 player_OldPos(0,0,0);
void Game::Init(void)
{
	Window* window = Window::Instance();
	sprite = Sprite::Create(D3DXVECTOR3(200.0f,200.0f,0),D3DXVECTOR2(100.0f,300.0f),WHITE(1.0f));
	sprite->SetRot(DEG2RAD(45.0f));
	player = Player::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 2.0f, 0), D3DXVECTOR2(100.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_3);
	Sound::Instance()->Play(BGM_TITLE);
}

void Game::Uninit(void)
{
	CManager::ReleaseObject();
}

void Game::Update(void)
{
	D3DXVECTOR3 reflectVec(0,0,0);
	if (Collision::CircleQuad(player->Pos(),10.0f,sprite->Quad(),4,player->Speed(),&reflectVec))
	{
		D3DXVECTOR3 speed = player->Speed();
		float angle = atan2(reflectVec.y,reflectVec.x);
		player->SetSpeed(D3DXVec3Length(&speed),angle);
		player->SetPos(player_OldPos);
	}
	player_OldPos = player->Pos();
	
}

void Game::Draw(void)
{


}