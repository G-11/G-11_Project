#include "Stage_2.h"
#include "Game.h"
#include "Gimmick.h"
#include "Collision.h"
#include "Player.h"
#include "Texture.h"
#include "Wall.h"
#include "BoundObject.h"
#include "BreakObject.h"
#include "StartDevice.h"
#include "Camera2D.h"

#include "Ivy.h"
#include "Gate.h"
#include "Stump.h"
#include "Clog.h"
#include "Item.h"
#include "Shader2D.h"
#include "Interface.h"

#define ITEM_SIZE (D3DXVECTOR2(84.0f,84.0f))
//=============================================================================
//コンストラクタ
//=============================================================================
Stage_2::Stage_2()
{
	//ステージサイズ 1280*4x900
}

//=============================================================================
//デストラクタ
//=============================================================================
Stage_2::~Stage_2()
{

}

//=============================================================================
//生成
//=============================================================================
Stage_2 *Stage_2::Create()
{
	Stage_2 *stage2 = new Stage_2();

	stage2->Init();

	return stage2;
}

//=============================================================================
//初期化
//=============================================================================
void Stage_2::Init(void)
{
	Camera2D *camera = Camera2D::GetCamera(0);

	D3DXVECTOR3* quad;

	_Interface = new Interface;
	_Interface->Init(18000, 100);

	_Field = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), WHITE(1.0f), Sprite::LAYER_BACKGROUND);
	_Field->SetTexture(GetTexture(TEX_FIELD1_BG));

	_Field->SetMask(GetTexture(TEX_SKY_MAP));
	_Field->SetPass(CShader2D::SKY);

	// 床
	Wall *wall = Wall::CreateFloor(D3DXVECTOR2(SCREEN_WIDTH * 4.0f, 945.0f), D3DXVECTOR2(SCREEN_WIDTH * 8.0f, 150.0f), TEX_FLOOR1_BG, Sprite::LAYER_1);
	quad = wall->LocalQuadBase();
	quad[0].y = -0.1f;
	quad[1].y = -0.1f;
	wall->SetLocalQuadBase(quad);
	camera->SetFloor(wall);

	// 天井
	wall = Wall::CreateFloor(D3DXVECTOR2(SCREEN_WIDTH * 4.0f, 45.0f), D3DXVECTOR2(SCREEN_WIDTH * 8.0f, 150.0f), TEX_CILLING1_BG, Sprite::LAYER_1);
	quad = wall->LocalQuadBase();
	quad[0].y = 0.1f;
	quad[1].y = 0.1f;
	wall->SetLocalQuadBase(quad);
	camera->SetCilling(wall);

	// 左の壁
	wall = Wall::CreateFloor(D3DXVECTOR2(0.0f,450.0f),D3DXVECTOR2(1280.0f,256.0f),TEX_FLOOR1_BG,Sprite::LAYER_1);
	quad = wall->LocalQuadBase();
	quad[0].y = -0.1f;
	quad[1].y = -0.1f;
	wall->SetLocalQuadBase(quad);
	wall->SetRot(PI/2.0f);

	wall = Wall::CreateFloor(D3DXVECTOR2(SCREEN_WIDTH*8.0f,450.0f),D3DXVECTOR2(1280.0f,256.0f),TEX_FLOOR1_BG,Sprite::LAYER_1);
	quad = wall->LocalQuadBase();
	quad[0].y = 0.1f;
	quad[1].y = 0.1f;
	wall->SetLocalQuadBase(quad);
	wall->SetRot(-(PI / 2.0f));

	camera->SetLeftLimit(560.0f);
	camera->SetRightLimit(SCREEN_WIDTH*8.0f - 560.0f);

	////// 最初のギミック（壁）//////

	wall = Wall::Create(D3DXVECTOR2(900.0f, 610.0f), D3DXVECTOR2(256.0f, 640.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_2);
	wall->SetUVHeight(10.0f);

	// 丸太は吹っ飛ばせるんですよ
	BreakObject	*BOwall = BreakObject::Create(D3DXVECTOR2(1350.0f, 100.0f), D3DXVECTOR2(128.0f, 256.0f), TEX_LOG_BREAK);
	BOwall->SetRot(PI / 2);
	BOwall->SetUV(0, 0, 1, 1);

	BOwall = BreakObject::Create(D3DXVECTOR2(1350.0f,174.0f),D3DXVECTOR2(128.0f,256.0f),TEX_LOG_BREAK);
	BOwall->SetRot(PI / 2);
	BOwall->SetUV(0,0,1,1);

	// 丸太の影にアイテムが・・・
	Item::Create(D3DXVECTOR3(1350.0f, 140.0f, 0), ITEM_SIZE, WHITE(1.0f), 1, Sprite::LAYER_1);
	
	wall = Wall::Create(D3DXVECTOR2(1350.0f, 520.0f), D3DXVECTOR2(256.0f, 576.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_2);
	wall->SetUVHeight(10.0f);

	// 思わせぶりな石
	Clog::Create(D3DXVECTOR2(1800.0f, 180.0f), D3DXVECTOR2(75.0f, 75.0f));

	wall = Wall::Create(D3DXVECTOR2(1800.0f, 580.0f), D3DXVECTOR2(256.0f, 704.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_2);
	wall->SetUVHeight(10.0f);

	wall = Wall::Create(D3DXVECTOR2(2250.0f, 400.0f), D3DXVECTOR2(256.0f, 704.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_2);
	wall->SetUVHeight(10.0f);

	////// ゲートを開けるギミック //////

	// この時点では取れないんだよなぁ・・・
	Item::Create(D3DXVECTOR3(2750.0f, 550.0f, 0), ITEM_SIZE, WHITE(1.0f), 1, Sprite::LAYER_1);

	// 木の囲い
	wall = Wall::Create(D3DXVECTOR2(2650.0f,550.0f),D3DXVECTOR2(100.0f,250.0f),TEX_LOG_WALL,Sprite::LAYER_2);
	wall->SetUV(0, 0, 1, 1);

	wall = Wall::Create(D3DXVECTOR2(2750.0f,450.0f),D3DXVECTOR2(100.0f,250.0f),TEX_LOG_WALL,Sprite::LAYER_2);
	wall->SetUV(0, 0, 1, 1);
	wall->SetRot(PI / 2.0f);

	StartDevice *start = StartDevice::CreateTrigger(D3DXVECTOR2(3000.0f, 550.0f), D3DXVECTOR2(64.0f, 64.0f));
	start->SetTexture(GetTexture(TEX_UNBREAKABLE_OBJ));

	Gimmick *gimmick = Gimmick::Create(D3DXVECTOR2(2850.0f, 550.0f), D3DXVECTOR2(100.0f, 250.0f), Sprite::LAYER_2);
	gimmick->SetTexture(GetTexture(TEX_LOG_GIMMICK));

	gimmick->SetAction([](Gimmick* act){
		act->AddPosY(5.0f);
	});

	gimmick->AddStartDevice(start);

	wall = Wall::Create(D3DXVECTOR2(2750.0f,650.0f),D3DXVECTOR2(100.0f,250.0f),TEX_LOG_WALL,Sprite::LAYER_2);
	wall->SetUV(0, 0, 1, 1);
	wall->SetRot(PI / 2.0f);

	// 天井増設
	wall = Wall::Create(D3DXVECTOR2(4300.0f, 140.0f), D3DXVECTOR2(1920.0f, 192.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_2);
	wall = Wall::Create(D3DXVECTOR2(4300.0f,900.0f),D3DXVECTOR2(1920.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	wall = Wall::Create(D3DXVECTOR2(3750.0f,530.0f),D3DXVECTOR2(768.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	Wall::Create(vector2(4440.0f,530.0f),vector2(128.0f,128.0f),TEX_BREAKABLE_OBJ);
	Wall::Create(D3DXVECTOR2(5012.0f,530.0f),D3DXVECTOR2(512.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	// ゲートとスイッチの連続
	float pos = 0;
	for (int cnt = 0;cnt < 5;cnt++)
	{
		if (cnt % 2 == 0)
		{
			start = StartDevice::CreateON(vector2(3530.0f + pos,276.0f),vector2(64.0f,64.0f));
			start->SetTexture(GetTexture(TEX_UNBREAKABLE_OBJ));

			start = StartDevice::CreateTrigger(vector2(3430.0f + pos,401.0f),vector2(64.0f,64.0f));
			start->SetTexture(GetTexture(TEX_BREAKABLE_OBJ));
			
		}
		else
		{
			start = StartDevice::CreateON(vector2(3530.0f + pos,276.0f),vector2(64.0f,64.0f));
			start->SetTexture(GetTexture(TEX_UNBREAKABLE_OBJ));

			start = StartDevice::CreateTrigger(vector2(3370.0f + pos ,401.0f),vector2(64.0f,64.0f));
			start->SetTexture(GetTexture(TEX_BREAKABLE_OBJ));
		}
		gimmick = Gimmick::Create(D3DXVECTOR2(3630.0f+pos,710.0f),D3DXVECTOR2(128.0,256.0f),Sprite::LAYER_0);
		gimmick->SetTexture(GetTexture(TEX_LOG_GIMMICK));
		gimmick->AddStartDevice(start);
		gimmick->SetAction([](Gimmick* gimmick){
			gimmick->AddPosY(3.48f);
			if (gimmick->Pos().y > 2000.0f)
			{
				gimmick->SetRelease();
			}
		});

		if (cnt == 2)
		{
			gimmick = Gimmick::Create(vector2(4631.0f,530.0f),vector2(128.0f,256.0f),Sprite::LAYER_0);
			gimmick->SetRot(PI * 1.5f);
			gimmick->SetTexture(GetTexture(TEX_LOG_GIMMICK));
			gimmick->AddStartDevice(start);
			gimmick->SetAction([](Gimmick* gimmick)
			{

				if (gimmick->Frame() < 60)
				{
					gimmick->AddPosX(5.0f);
				}
			});
		}

		if (cnt % 2 == 0)
		{
			start = StartDevice::CreateON(vector2(3530.0f + pos,656.0f),vector2(64.0f,64.0f));
			start->SetTexture(GetTexture(TEX_UNBREAKABLE_OBJ));

			start = StartDevice::CreateTrigger(vector2(3370.0f + pos,791.0f),vector2(64.0f,64.0f));
			start->SetTexture(GetTexture(TEX_BREAKABLE_OBJ));

		}
		else
		{
			start = StartDevice::CreateON(vector2(3530.0f + pos,656.0f),vector2(64.0f,64.0f));
			start->SetTexture(GetTexture(TEX_UNBREAKABLE_OBJ));

			start = StartDevice::CreateTrigger(vector2(3370.0f + pos,791.0f),vector2(64.0f,64.0f));
			start->SetTexture(GetTexture(TEX_BREAKABLE_OBJ));
		}
		gimmick = Gimmick::Create(D3DXVECTOR2(3630.0f + pos,350.0f),D3DXVECTOR2(128.0,256.0f),Sprite::LAYER_0);
		gimmick->SetTexture(GetTexture(TEX_LOG_GIMMICK));
		gimmick->AddStartDevice(start);
		gimmick->SetAction([](Gimmick* gimmick){
			gimmick->AddPosY(-3.48f);
			if (gimmick->Pos().y < 0.0f)
			{
				gimmick->SetRelease();
			}
		});

		if (cnt == 1)
		{
			gimmick = Gimmick::Create(vector2(4255.0f,530.0f),vector2(128.0f,256.0f),Sprite::LAYER_1);
			gimmick->SetRot(PI / 2.0f);
			gimmick->SetTexture(GetTexture(TEX_LOG_GIMMICK));
			gimmick->AddStartDevice(start);
			gimmick->SetAction([](Gimmick* gimmick)
			{
				
				if (gimmick->Frame() < 60)
				{
					gimmick->AddPosX(-5.0f);
				}
			});
		}
		pos += 400.0f;
	}

	Item::Create(vector3(5000.0f,690.0f,0),ITEM_SIZE,WHITE(1.0f),ITEM_ID_BUTA);

	bool breakFlag[9][12] = { { 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0 },
							  { 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0 },
							  { 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
							  { 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0 },
							  { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 },
							  { 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0 },
							  { 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0 },
							  { 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0 },
							  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 } };

	for(int num = 0;num < 9;num++)
	{
		for (int cnt = 0;cnt < 12;cnt++)
		{
			if (breakFlag[num][cnt])
			{
				BOwall = BreakObject::Create(vector2(5800.0f + num*200.0f, 110.0f + (cnt*72.0f)), vector2(128.0f, 256.0f), TEX_LOG_BREAK);
				BOwall->SetRot(PI / 2.0f);
			}
			else
			{
				wall = Wall::Create(vector2(5800.0f + num*200.0f, 110.0f + (cnt*72.0f)), vector2(128.0f, 256.0f), TEX_LOG_WALL);
				wall->SetUV();
				wall->SetRot(PI / 2.0f);
			}
		}
	}

}

//=============================================================================
//更新
//=============================================================================
void Stage_2::Update(void)
{
	Stage::Update();
}

//=============================================================================
//更新
//=============================================================================
void Stage_2::Release(void)
{
	delete this;
}