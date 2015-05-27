#include "Stage_1.h"
#include "Game.h"
#include "Gimmick.h"
#include "Collision.h"
#include "Player.h"
#include "Texture.h"

bool Stage_1::hitChk = false;

//=============================================================================
//コンストラクタ
//=============================================================================
Stage_1::Stage_1()
{
	StartDevice* start_device;
	start_device = StartDevice::CreateSwitch(D3DXVECTOR2(SCREEN_WIDTH*0.75f,SCREEN_HEIGHT*0.5f),D3DXVECTOR2(30.0f,30.0f));
	_gimmick = Gimmick::Create(D3DXVECTOR2(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT / 4),
								D3DXVECTOR2(50.0f, 50.0f),
								Sprite::LAYER_2);

	_gimmick->SetTexture(GetTexture(TEX_ITEM_KUMA));
	_gimmick->SetAction([](Gimmick *gimmick){gimmick->AddPosY(1.0f); });
	_gimmick->AddStartDevice(start_device);

}

//=============================================================================
//生成
//=============================================================================
Stage_1 *Stage_1::Create()
{
	Stage_1 *stage1 = new Stage_1();

	return stage1;
}

//=============================================================================
//破棄
//=============================================================================
void Stage_1::Release(void)
{
	SafeRelease(_gimmick);
}

//=============================================================================
//更新
//=============================================================================
void Stage_1::Update(void)
{
	
}