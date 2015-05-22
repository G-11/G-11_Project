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

	BG = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), WHITE(1.0f), Sprite::LAYER_4);
	BG->SetTexture(GetTexture(TEX_RESULT_BG));

	Push = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH /1.2f, SCREEN_HEIGHT / 1.3f, 0), D3DXVECTOR2(250.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_4);
	Push->SetTexture(GetTexture(TEX_INTERFACE_PUSH));

	flahing = 0;
	add_flahing_num = 0.01f;
}

void Result::Uninit(void)
{
	Manager::ReleaseObject();
}

void Result::Update(void)
{
	/*“_–Åˆ—i‰¼j*/
	if (flahing > 1.0f || flahing < 0)
	{
		add_flahing_num = -add_flahing_num;
	}
	flahing += add_flahing_num;
	Push->SetColor(WHITE(flahing));
	//‚±‚±‚Ü‚Å

	VC* vc = VC::Instance();
	if (vc->Trigger(COMMAND_OK) && Fade::Instance()->Mode() == Fade::FADE_NONE)
	{
		Manager::Instance()->SetScene(Manager::SCENE_TITLE);
	}

}

void Result::Draw(void)
{


}
