//
//Title.cpp
//

#include "Title.h"
#include "Sprite.h"
#include "Renderer.h"
#include "Manager.h"
#include "Shader2D.h"
#include "Input/Keyboard.h"
#include "Manager.h"
#include "Eatan.h"
#include "MathTable.h"
#include "Title.h"
#include "ItemManager.h"

typedef enum
{
	TMenu_Start=0,
	//TMenu_Tutorial,
	//TMenu_Collection,
	TMenu_Config,
	
	TMenu_Num
}TitleMenu;

void Title::Init(void)
{
	Window* window = Window::Instance();
	RotFream = 0;

	Sound::Instance()->Play(BGM_TITLE);

	//各明滅処理
	MenuNum = TMenu_Start;

	//背景
	Bg = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f,0), D3DXVECTOR2(SCREEN_WIDTH*1.5f, SCREEN_HEIGHT/1.3f), WHITE(1.0f), Sprite::LAYER_BACKGROUND);
	Bg->SetTexture(GetTexture(TEX_TITLE_BG));
	Bg->SetUV(D3DXVECTOR4(0, 0.2f, 1, 0.7f));

	//キャラ
	Usagi = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 1.2f, SCREEN_HEIGHT/1.3f, 0), D3DXVECTOR2(100, 100), WHITE(1.0f), Sprite::LAYER_4);
	Usagi->SetTexture(GetTexture(TEX_ITEM_USAGI));

	Kuma = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH/4.7f, SCREEN_HEIGHT/1.3f, 0), D3DXVECTOR2(100, 100), WHITE(1.0f), Sprite::LAYER_4);
	Kuma->SetTexture(GetTexture(TEX_ITEM_KUMA));

	Neko = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH/1.2f, SCREEN_HEIGHT/1.6f, 0), D3DXVECTOR2(100, 100), WHITE(1.0f), Sprite::LAYER_4);
	Neko->SetTexture(GetTexture(TEX_ITEM_NEKO));

	TEatan = Eatan::Create(D3DXVECTOR3(SCREEN_WIDTH / 6.5f, SCREEN_HEIGHT / 2.3f, 0), D3DXVECTOR2(SCREEN_WIDTH / 5.0f, SCREEN_HEIGHT / 2.4f), WHITE(1.0f), Sprite::LAYER_3);
	
	//ロゴ
	Logo = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.5f, 0), D3DXVECTOR2(SCREEN_WIDTH/1.4f, SCREEN_HEIGHT/1.4f), WHITE(1.0f), Sprite::LAYER_4);
	Logo->SetTexture(GetTexture(TEX_GELFLOGO));

	//スタート
	Start = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.8f, SCREEN_HEIGHT / 1.4f, 0), D3DXVECTOR2(250.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_INTERFACE);
	Start->SetTexture(GetTexture(TEX_INTERFACE_PUSH_START));
	
	//チュートリアル
	//Tutorial = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 5.5f, SCREEN_HEIGHT / 1.4f, 0), D3DXVECTOR2(250.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_INTERFACE);
	//Tutorial->SetTexture(GetTexture(TEX_INTERFACE_PUSH));
	//コレクション
	//Collection = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.4f, 0), D3DXVECTOR2(250.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_INTERFACE);
	//Collection->SetTexture(GetTexture(TEX_INTERFACE_PUSH_COLLECTION));
	//設定
	Config = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 1.5f, SCREEN_HEIGHT / 1.4f, 0), D3DXVECTOR2(250.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_INTERFACE);
	Config->SetTexture(GetTexture(TEX_INTERFACE_PUSH_OPTION));

	Flahing = 0;
	AddFlashingNum = 0.01f;
}

void Title::Uninit(void)
{
	ItemManager::Reset();
	Manager::ReleaseObject();
}

void Title::Update(void)
{	
	/*点滅処理（仮）*/
	if (Flahing > 1.0f || Flahing < 0)
	{
		AddFlashingNum = -AddFlashingNum;
		//いーたんのランダムアニメーション
		TEatan->SetState(static_cast<Eatan::EATAN_STATE>(CMath::RandomI((int)TEatan->EATAN_STATE_MOVE,(int)TEatan->EATAN_STATE_MAX)));
	}
	Flahing += AddFlashingNum;

//#if _DEBUG

	//各種明滅処理
	switch (MenuNum)
	{
	case TMenu_Start:
		//切り替え
		if (VC::Instance()->Trigger(COMMAND_OK) && Fade::Instance()->Mode() == Fade::FADE_NONE)
		{
			Manager::Instance()->SetScene(Manager::SCENE_GAME);
			Sound::Instance()->Play(SE_DECISION);
		}
		//Start点滅
		Start->SetColor(WHITE(Flahing));
		break;
		/*
	case TMenu_Tutorial:
		//チュートリアル点滅
		Tutorial->SetColor(WHITE(Flahing));
		break;
	case TMenu_Collection:
		//コレクション点滅
		Collection->SetColor(WHITE(Flahing));
		break;
		*/
	case TMenu_Config:
		if (VC::Instance()->Trigger(COMMAND_OK) && Fade::Instance()->Mode() == Fade::FADE_NONE)
		{
			Manager::Instance()->SetScene(Manager::SCENE_OPTION);
			Sound::Instance()->Play(SE_DECISION);
		}
		//設定点滅
		Config->SetColor(WHITE(Flahing));
		break;
	default:
		break;
	}

	

//#endif

	/*キャラゆれ*/
	//動きテスト
	Usagi->SetRot(cosf(DEG2RAD(RotFream))*PI / 10.0f);
	Kuma->SetRot(cosf(DEG2RAD(RotFream))*PI / 10.0f);
	Neko->SetRot(cosf(DEG2RAD(RotFream))*PI / 10.0f);
	RotFream += 3.0f;
	

	/**/
	//メニュー選択
	int OldMenuNum = MenuNum;
	if (VC::Instance()->Trigger(COMMAND_LEFT) || VC::Instance()->Trigger(COMMAND_UP))
	{
		MenuNum = (MenuNum + 1) % TMenu_Num;
		Sound::Instance()->Play(SE_CURSOL);
	}
	if (VC::Instance()->Trigger(COMMAND_RIGHT) || VC::Instance()->Trigger(COMMAND_DOWN))
	{
		MenuNum = (MenuNum + (TMenu_Num-1)) % TMenu_Num;
		Sound::Instance()->Play(SE_CURSOL);
	}
	//条件チェック
	if (OldMenuNum != MenuNum)
	{
		Start->SetColor(WHITE(1));
		//Tutorial->SetColor(WHITE(1));
		//Collection->SetColor(WHITE(1));
		Config->SetColor(WHITE(1));
		Flahing = 0;
	}

	//背景スクロールテスト
	scroll_num += 0.0001f;
	Bg->SetUV(D3DXVECTOR4(scroll_num, 0.2f, 1, 0.7f));


}

void Title::Draw(void)
{


}
