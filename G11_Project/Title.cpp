//
//Title.cpp
//

#include "Title.h"
#include "Sprite.h"
#include "Renderer.h"
#include "Manager.h"
#include "Shader2D.h"
#include "Input/Keyboard.h"

void Title::Init(void)
{
	Window* window = Window::Instance();

	//背景
	Sprite* Bg = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f,0), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), WHITE(1.0f), Sprite::LAYER_4);
	Bg->SetTexture(GetTexture(TEX_BG));

	//点滅用ロゴ
	Sprite* Logo = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f,0), D3DXVECTOR2(100.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_4);
	Logo->SetTexture(GetTexture(TEX_EFFECT_GEAR));
	m_logo = Logo;
	flahing = 0;
	add_flahing_num = 0.01f;

}

void Title::Uninit(void)
{
	CManager::ReleaseObject();
}

void Title::Update(void)
{
	//キー操作用（確認テスト）
	Keyboard *key = Keyboard::Instance();
	
	/*点滅処理（仮）*/
	if (flahing > 1.0f || flahing < 0)
	{
		add_flahing_num = -add_flahing_num;
	}
	flahing += add_flahing_num;
	m_logo->SetColor(WHITE(flahing));
	//ここまで

	//キー操作用（確認テスト：中身なし）
	if (key->Trigger(DIK_L))
	{
	
	}
}

void Title::Draw(void)
{


}
