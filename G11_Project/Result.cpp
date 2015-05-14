//
//Result.cpp
//

#include "Result.h"
#include "Sprite.h"
#include "Renderer.h"
#include "Manager.h"

void Result::Init(void)
{
	Window* window = Window::Instance();
	Sprite* Bg = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), WHITE(1.0f), Sprite::LAYER_4);
	Bg->SetTexture(GetTexture(TEX_BG));

	Sprite* Logo = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0), D3DXVECTOR2(100.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_4);
	Logo->SetTexture(GetTexture(TEX_EFFECT_GEAR));

}

void Result::Uninit(void)
{
	CManager::ReleaseObject();
}

void Result::Update(void)
{

}

void Result::Draw(void)
{


}
