#include "Pause.h"
#include "Game.h"
#include "Sprite.h"
#include "Input/VC.h"
#include "Input/Keyboard.h"
#include "Manager.h"
#include "ItemManager.h"

typedef enum
{
	PReturn_Game = 0,
	PBack_Stage,
	PRead_Operate,

	PMenu_Num
}PauseMenu;

PAUSECHARDATA Pause::PauseData[PAUSECHARMAX];
bool Pause::CharIdflag[PAUSECHARMAX];
Pause::Pause()
{
	for (int cnt = 0;cnt < PAUSECHARMAX;cnt++)
	{
		PauseChar[cnt] = nullptr;
	}
}

Pause::~Pause()
{

}

void Pause::Init(void)
{
	MenuNum = PReturn_Game;

	//背景
	BG = Sprite::Create(vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), vector2(SCREEN_WIDTH, SCREEN_HEIGHT), WHITE(0.0f), Sprite::LAYER_INTERFACE);
	BG->SetTexture(GetTexture(TEX_OPTION_BG));

	//キャラ枠
	CharFream = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 3.0f,0),D3DXVECTOR2(180,700),WHITE(1.0f),Sprite::LAYER_INTERFACE);
	CharFream->SetTexture(GetTexture(TEX_GLOE_GAUGE_FRAME));//枠のテクスチャ
	CharFream->SetRot(D3DX_PI / 2);//枠を90度へ
	CharFream->SetAlpha(0);

	// ゲームに戻るボタン
	ReturnGame = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 1.85f,0),D3DXVECTOR2(280.0f,30.0f),WHITE(1.0f),Sprite::LAYER_INTERFACE);
	ReturnGame->SetTexture(GetTexture(TEX_OPTION_BACKTO_GAME));
	ReturnGame->SetAlpha(0);

	// ステージ選択に戻るボタン
	BackStage = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 1.45f,0),D3DXVECTOR2(320.0f,20.0f),WHITE(1.0f),Sprite::LAYER_INTERFACE);
	BackStage->SetTexture(GetTexture(TEX_OPTION_BACKTO_STAGESELECT));
	BackStage->SetAlpha(0);

	// 操作説明ボタン
	ReadOperate = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 1.2f,0),D3DXVECTOR2(280.0f,30.0f),WHITE(1.0f),Sprite::LAYER_INTERFACE);
	ReadOperate->SetTexture(GetTexture(TEX_OPTION_OPERATION));
	ReadOperate->SetAlpha(0);

	Carsol = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 1.2f,SCREEN_HEIGHT / 1.1f,0),D3DXVECTOR2(100.0f,100.0f),WHITE(1.0f),Sprite::LAYER_INTERFACE);
	Carsol->SetTexture(GetTexture(TEX_CURSOR));
	Carsol->SetAlpha(0);

	for (int cnt = 0,num = 0;cnt < PAUSECHARMAX;num++)
	{

		ItemData item = ItemManager::GetItemFromArrayNum((uint)num);
		if (item.id == -1)
		{
			break;
		}
		if (item.Score > 0)
		{
			PauseChar[cnt] = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f + (100 * cnt),SCREEN_HEIGHT / 3,0),D3DXVECTOR2(84.0f,84.0f),BLACK(0.0f),Sprite::LAYER_INTERFACE);
			PauseChar[cnt]->SetTexture(GetTexture((TEX)(TEX_ITEM_OFFSET + item.itemId)));
			PauseChar[cnt]->SetColor(BLACK(0));
			PauseData[cnt].CharId = item.id;
			cnt++;
		}
	}
	OldItemNum = ItemManager::ListSize();
	AlphaFlag = false;
}

void Pause::Uninit(void)
{
	ResetPauseChar();
}

void Pause::Update(void)
{
	if (!AlphaFlag)
	{
		CharFream->SetAlpha(0);
		ReturnGame->SetAlpha(0);
		BackStage->SetAlpha(0);
		ReadOperate->SetAlpha(0);
		Carsol->SetAlpha(0);
		BG->SetAlpha(0);

		for (int i = 0; i < PAUSECHARMAX; i++)
		{
			if (PauseChar[i])
			{
				PauseChar[i]->SetAlpha(0);
			}
		}
		AlphaFlag = true;
	}
	else
	{
		int num = ItemManager::ListSize();
		if (OldItemNum != num)
		{
			ReCreate();
		}
		for (int cnt = 0;cnt < PAUSECHARMAX;cnt++)
		{
			ItemData item = ItemManager::GetItem(PauseData[cnt].CharId);
			if (PauseChar[cnt] != nullptr)
			{
				if (item.Active)
				{
					PauseChar[cnt]->SetColor(WHITE(0));
				}
			}
		}
	}
}

void Pause::ReCreate(void)
{
	
	for (int cnt = 0,num = 0;cnt < PAUSECHARMAX;num++)
	{
		ItemData item = ItemManager::GetItemFromArrayNum((uint)num);
		if (item.id == -1)
		{
			break;
		}
		if (item.Score > 0)
		{
			if (PauseChar[cnt] != nullptr)
			{
				PauseChar[cnt]->SetRelease();
				PauseChar[cnt] = nullptr;
			}
			PauseChar[cnt] = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f + (100 * cnt),SCREEN_HEIGHT / 3,0),D3DXVECTOR2(84.0f,84.0f),BLACK(0.0f),Sprite::LAYER_INTERFACE);
			PauseChar[cnt]->SetTexture(GetTexture((TEX)(TEX_ITEM_OFFSET + item.itemId)));
			PauseChar[cnt]->SetColor(BLACK(0));
			PauseData[cnt].CharId = item.id;
			cnt++;
		}
	}
	
}

void Pause::ResetPauseChar(void)
{
	for (int i = 0; i < PAUSECHARMAX; i++)
	{
		CharIdflag[i] = false;
		PauseChar[i] = nullptr;
	}
}

void Pause::PauseUpdate()
{

	if (AlphaFlag)
	{
		CharFream->SetAlpha(1);
		ReturnGame->SetAlpha(1);
		BackStage->SetAlpha(1);
		ReadOperate->SetAlpha(1);
		Carsol->SetAlpha(1);
		BG->SetAlpha(1);

		for (int i = 0; i < PAUSECHARMAX; i++)
		{
			if (PauseChar[i])
			{
				PauseChar[i]->SetAlpha(1);
			}
		}
		AlphaFlag = false;
	}

	switch (MenuNum)
	{
	case PReturn_Game:
		Carsol->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 1.85f, 0));
		break;
	case PBack_Stage:

		Carsol->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 1.45f, 0));
		break;									  	
	case PRead_Operate:							  	
		Carsol->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 3.0f,SCREEN_HEIGHT / 1.2f,0));
		break;
	}

	//メニュー選択
	if (VC::Instance()->Trigger(COMMAND_UP))
	{
		MenuNum = (MenuNum + 2) % PMenu_Num;
	}
	if (VC::Instance()->Trigger(COMMAND_DOWN))
	{
		MenuNum = (MenuNum + (PMenu_Num - 2)) % PMenu_Num;
	}
	if (VC::Instance()->Trigger(COMMAND_RIGHT))
	{
		MenuNum = (MenuNum + 1) % PMenu_Num;
	}
	if (VC::Instance()->Trigger(COMMAND_LEFT))
	{
		MenuNum = (MenuNum + (PMenu_Num - 1)) % PMenu_Num;
	}

}