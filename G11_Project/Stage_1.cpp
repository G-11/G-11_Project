#include "Stage_1.h"
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
#include "Interface.h"
#include "Shader2D.h"

#include "Ivy.h"
#include "Gate.h"
#include "Stump.h"
#include "Sensor.h"
#include "FixedCamera.h"
#include "Clog.h"
#include "Item.h"
#include "Goal.h"
#include "Taizou.h"

#define ITEM_SIZE (D3DXVECTOR2(84.0f,84.0f))
//=============================================================================
//コンストラクタ
//=============================================================================
Stage_1::Stage_1()
{
	//ステージサイズ 1280*4x900
}

//=============================================================================
//デストラクタ
//=============================================================================
Stage_1::~Stage_1()
{

}

//=============================================================================
//生成
//=============================================================================
Stage_1 *Stage_1::Create()
{
	Stage_1 *stage1 = new Stage_1();

	stage1->Init();

	return stage1;
}

//=============================================================================
//初期化
//=============================================================================
void Stage_1::Init(void)
{
	Camera2D *camera = Camera2D::GetCamera(0);

	D3DXVECTOR3* quad;

	Sound::Instance()->Play(BGM_STAGE1);
	_Interface = new Interface;
	_Interface->Init(3600*4,100);

	// 背景
	_Field = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0),D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT),WHITE(1.0f),Sprite::LAYER_BACKGROUND);
	_Field->SetTexture(GetTexture(TEX_FIELD1_BG));

	_Field->SetMask(GetTexture(TEX_SKY_MAP));
	_Field->SetPass(CShader2D::SKY);

	// 床
	Wall *wall = Wall::CreateFloor(D3DXVECTOR2(SCREEN_WIDTH * 4.0f, 945.0f), D3DXVECTOR2(SCREEN_WIDTH * 8.0f, 256.0f), TEX_FLOOR1_BG, Sprite::LAYER_1);
	quad = wall->LocalQuadBase();
	quad[0].y = -0.1f;
	quad[1].y = -0.1f;
	wall->SetLocalQuadBase(quad);
	camera->SetFloor(wall);

	// 天井
	wall = Wall::CreateFloor(D3DXVECTOR2(SCREEN_WIDTH * 4.0f, 45.0f), D3DXVECTOR2(SCREEN_WIDTH * 8.0f, 256.0f), TEX_CILLING1_BG, Sprite::LAYER_1);
	quad = wall->LocalQuadBase();
	quad[2].y = 0.1f;
	quad[3].y = 0.1f;
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
	quad[0].y = -0.1f;
	quad[1].y = -0.1f;
	wall->SetLocalQuadBase(quad);
	wall->SetRot(-(PI / 2.0f));

	camera->SetLeftLimit(560.0f);
	camera->SetRightLimit(SCREEN_WIDTH*8.0f - 660.0f);

	// 仮のアイテム座標　※一時的に書いたもの
	D3DXVECTOR2 itemPos = D3DXVECTOR2(1000.0f, SCREEN_HEIGHT / 2.0f);
	D3DXVECTOR2 size = D3DXVECTOR2(64.0f, 128.0f);
	Item::Create(D3DXVECTOR3(1000.0f,SCREEN_HEIGHT / 2.0f,0),ITEM_SIZE,WHITE(1.0f),ITEM_ID_USAGI,Sprite::LAYER_2);

	wall = Wall::Create(D3DXVECTOR2(1000.0f,245.0f),D3DXVECTOR2(192.0f,64.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);

	wall = Wall::Create(D3DXVECTOR2(1130.0f, 340.0f), D3DXVECTOR2(64.0f,256.0f),TEX_BREAKABLE_OBJ, Sprite::LAYER_2);

	wall = Wall::Create(D3DXVECTOR2(1000.0f, 435.0f), D3DXVECTOR2(192.0f,64.0f),TEX_BREAKABLE_OBJ, Sprite::LAYER_2);

	StartDevice *start = Ivy::Create(D3DXVECTOR2(850.0f,160.0f),D3DXVECTOR2(130.0f,260.0f),Sprite::LAYER_0);
	
	Wall::Create(vector2(450.0f,350.0f),vector2(128.0f,64.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);

	Sprite* board = Sprite::Create(vector3(450.0f,230.0f,0),vector2(256.0f,256.0f),WHITE(1.0f));
	board->SetTexture(GetTexture(TEX_HINT1));

	board = Sprite::Create(vector3(1350.0f,800.0f,0),vector2(256.0f,256.0f),WHITE(1.0f));
	board->SetTexture(GetTexture(TEX_HINT2));

	board = BreakObject::Create(vector2(1880.0f,850.0f),vector2(256.0f,256.0f),TEX_HINT3,true,Sprite::LAYER_0);
	

	board = Sprite::Create(vector3(2350.0f,130.0f,0),vector2(256.0f,256.0f),WHITE(1.0f));
	board->SetTexture(GetTexture(TEX_HINT1));

	Gimmick* gimmick = Gimmick::Create(D3DXVECTOR2(850.0f,340.0f),D3DXVECTOR2(128.0f,256.0f),Sprite::LAYER_2);
	gimmick->SetTexture(GetTexture(TEX_LOG_GIMMICK));
	gimmick->SetAction([](Gimmick* gimmick){ 
		gimmick->AddSpeedY(0.48f);
		gimmick->AddPos(gimmick->Speed());
		if (gimmick->Pos().y > 1000.0f){ 
			gimmick->SetRelease();
		}
	});
	gimmick->AddStartDevice(start);

	start = Stump::Create(D3DXVECTOR2(1350.0f,963.5f),D3DXVECTOR2(128.0f,128.0f),Sprite::LAYER_0);

	Clog::Create(D3DXVECTOR2(1000.0f,800.0f),D3DXVECTOR2(128.0f,128.0f));

	//最初のゲート
	gimmick = Gate::Create(D3DXVECTOR2(1700.0f,700.0f),D3DXVECTOR2(128.0f,512.0f),TEX_LOG_GIMMICK,Sprite::LAYER_0);
	gimmick->SetAction([](Gimmick* gimmick){
		D3DXVECTOR3 pos = (D3DXVECTOR3(1700.0f,1250.0f,0) - gimmick->Pos())*0.01f;
		gimmick->AddPos(pos);
	});
	
	gimmick->AddStartDevice(start);

	gimmick = Gate::Create(D3DXVECTOR2(1700.0f,250.0f),D3DXVECTOR2(128.0f,500.0f),TEX_LOG_GIMMICK,Sprite::LAYER_0);
	
	gimmick->SetAction([](Gimmick* gimmick){
		D3DXVECTOR3 pos = (D3DXVECTOR3(1700.0f,-350.0f,0) - gimmick->Pos())*0.01f;
		gimmick->AddPos(pos);
	});
	gimmick->AddStartDevice(start);

	wall = Wall::Create(D3DXVECTOR2(1900.0f,256.0f),D3DXVECTOR2(256.0f,512.0f),TEX_BREAKABLE_OBJ);

	Item::Create(D3DXVECTOR3(2500.0f,890.0f,0),ITEM_SIZE,WHITE(1.0f),ITEM_ID_KUMA,Sprite::LAYER_2);
	for (int num = 0;num < 3;num++)
	{
		for (int cnt = 0;cnt < 3;cnt++)
		{
			BreakObject* breakObj = BreakObject::Create(D3DXVECTOR2(2200.0f + num*300.0f,890.0f - cnt*64.0f),D3DXVECTOR2(128.0f,256.0f),TEX_LOG_BREAK);
			breakObj->SetRot(PI / 2.0f);
		}
	}
	
	wall = Wall::Create(D3DXVECTOR2(2350.0f, 300.0f), D3DXVECTOR2(256.0f, 128.0f), TEX_BREAKABLE_OBJ,Sprite::LAYER_1);
	wall = Wall::Create(D3DXVECTOR2(2610.0f, 300.0f), D3DXVECTOR2(256.0f, 128.0f), TEX_BREAKABLE_OBJ,Sprite::LAYER_1);
	wall = Wall::Create(D3DXVECTOR2(2870.0f, 300.0f), D3DXVECTOR2(256.0f, 128.0f), TEX_BREAKABLE_OBJ,Sprite::LAYER_1);

	wall = Wall::Create(D3DXVECTOR2(2965.0f, 105.0f), D3DXVECTOR2(64.0f, 256.0f), TEX_BREAKABLE_OBJ);

	start = StartDevice::CreateTrigger(D3DXVECTOR2(2550.0f, 100.0f), D3DXVECTOR2(64.0f, 64.0f));
	start->SetTexture(GetTexture(TEX_UNBREAKABLE_OBJ));

	gimmick = Gimmick::Create(D3DXVECTOR2(2740.0f, 120.0f), D3DXVECTOR2(128.0f, 256.0f), Sprite::LAYER_0);
	gimmick->SetTexture(GetTexture(TEX_LOG_GIMMICK));

	gimmick->SetAction([](Gimmick* act){
		act->AddPosY((-200 - act->Pos().y) * 0.05f);
	});
	gimmick->AddStartDevice(start);

	Item::Create(D3DXVECTOR3(2870.0f, 100.0f, 0.0f), ITEM_SIZE, WHITE(1.0f), ITEM_ID_USI);

	wall = Wall::Create(D3DXVECTOR2(3800.0f, 370.0f), D3DXVECTOR2(1024.0f, 64.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_1);
	wall = Wall::Create(D3DXVECTOR2(4345.0f, 178.0f), D3DXVECTOR2(64.0f, 448.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_0);

	wall = Wall::Create(D3DXVECTOR2(3800.0f, 670.0f), D3DXVECTOR2(1024.0f, 64.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_1);
	wall = Wall::Create(D3DXVECTOR2(4345.0f, 865.0f), D3DXVECTOR2(64.0f, 448.0f), TEX_BREAKABLE_OBJ, Sprite::LAYER_0);

	// 上段
	for (int cnt = 0; cnt < 5; cnt++)
	{
		wall = BreakObject::Create(D3DXVECTOR2(3370.0f + 150.0f * cnt,200.0f),D3DXVECTOR2(150.0f,300.0f),TEX_LOG_BREAK,true,Sprite::LAYER_0);
	}

	// 中段
	for (int cnt = 0; cnt < 7; cnt++)
	{
		wall = BreakObject::Create(D3DXVECTOR2(3370.0f + 150.0f * cnt,525.0f),D3DXVECTOR2(150.0f,300.0f),TEX_LOG_BREAK,true,Sprite::LAYER_0);
	}

	// 下段
	for (int cnt = 0; cnt < 5; cnt++)
	{
		wall = BreakObject::Create(D3DXVECTOR2(3370.0f + 150.0f * cnt,850.0f),D3DXVECTOR2(150.0f,300.0f),TEX_LOG_BREAK,true,Sprite::LAYER_0);
	}

	Item::Create(D3DXVECTOR3(4150.0f, 200.0f, 0.0f), ITEM_SIZE, WHITE(1.0f), ITEM_ID_SAI);
	Item::Create(D3DXVECTOR3(4150.0f, 850.0f, 0.0f), ITEM_SIZE, WHITE(1.0f), ITEM_ID_NEKO);

	Wall::Create(D3DXVECTOR2(5590.0f,100.0f),D3DXVECTOR2(1280.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	Wall::Create(D3DXVECTOR2(5654.0f,850.0f),D3DXVECTOR2(1408.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	Wall::Create(D3DXVECTOR2(5000.0f,480.0f),D3DXVECTOR2(128.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);

	wall = Wall::Create(D3DXVECTOR2(5000.0f,288.0f),D3DXVECTOR2(128.0f,256.0f),TEX_LOG_WALL,Sprite::LAYER_0);
	wall->SetUVHeight(-1.0f);
	wall->SetUVWidth(1.0f);

	BoundObject* boundObj = BoundObject::Create(D3DXVECTOR2(5000.0f,810.0f),D3DXVECTOR2(128.0f,256.0f),TEX_LOG_GIMMICK,4,Sprite::LAYER_0);
	boundObj->SetOffsetY(-0.5f);
	boundObj->SetAction([](BoundObject* obj)
	{
		if (obj->Rot().z < PI / 2.0f)
		{
			obj->AddRot(DEG2RAD(10.0f));
		}
		else
		{
			obj->SetRot(DEG2RAD(90.0f));
		}
	});

	boundObj = BoundObject::Create(D3DXVECTOR2(5070.0f,480.0f),D3DXVECTOR2(128.0f,384.0f),TEX_LOG_GIMMICK,4,Sprite::LAYER_0);
	boundObj->SetOffsetY(-0.5f);
	boundObj->SetRot(PI/2.0f);
	boundObj->SetAction([](BoundObject* obj)
	{
		if (obj->Rot().z > 0)
		{
			obj->AddRot(DEG2RAD(-10.0f));
		}
		else
		{
			obj->SetRot(0);
		}
	});
	Wall::Create(D3DXVECTOR2(5490.0f,480.0f),D3DXVECTOR2(128.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);

	wall = Wall::Create(D3DXVECTOR2(5480.0f,680.0f),D3DXVECTOR2(128.0f,256.0f),TEX_LOG_WALL,Sprite::LAYER_0);
	wall->SetUVHeight(1.0f);
	wall->SetUVWidth(1.0f);

	boundObj = BoundObject::Create(D3DXVECTOR2(5480.0f,148.0f),D3DXVECTOR2(128.0f,256.0f),TEX_LOG_GIMMICK,4,Sprite::LAYER_0);
	boundObj->SetOffsetY(-0.5f);
	boundObj->SetRot(PI);
	boundObj->SetAction([](BoundObject* obj)
	{
		if (obj->Rot().z > PI / 2.0f)
		{
			obj->AddRot(DEG2RAD(-10.0f));
		}
		else
		{
			obj->SetRot(DEG2RAD(90.0f));
		}
	});

	boundObj = BoundObject::Create(D3DXVECTOR2(5560.0f,480.0f),D3DXVECTOR2(128.0f,384.0f),TEX_LOG_GIMMICK,4,Sprite::LAYER_0);
	boundObj->SetOffsetY(-0.5f);
	boundObj->SetRot(PI / 2.0f);
	boundObj->SetAction([](BoundObject* obj)
	{
		if (obj->Rot().z < PI)
		{
			obj->AddRot(DEG2RAD(10.0f));
		}
		else
		{
			obj->SetRot(PI);
		}
	});
	Wall::Create(D3DXVECTOR2(6032.0f,590.0f),D3DXVECTOR2(128.0f,384.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	Wall::Create(D3DXVECTOR2(6160.0f,590.0f),D3DXVECTOR2(128.0f,384.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	Wall::Create(D3DXVECTOR2(6288.0f,590.0f),D3DXVECTOR2(128.0f,384.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	
	boundObj = BoundObject::Create(D3DXVECTOR2(6000.0f,148.0f),D3DXVECTOR2(128.0f,256.0f),TEX_LOG_GIMMICK,4,Sprite::LAYER_0);
	boundObj->SetOffsetY(-0.5f);
	boundObj->SetRot(PI);
	boundObj->SetAction([](BoundObject* obj)
	{
		if (obj->Rot().z > PI / 2.0f)
		{
			obj->AddRot(DEG2RAD(-10.0f));
		}
		else
		{
			obj->SetRot(DEG2RAD(90.0f));
		}
	});

	Clog::Create(D3DXVECTOR2(5750.0f,700.0f),D3DXVECTOR2(75.0f,75.0f));
	Item::Create(vector3(6480.0f,600.0f,0),ITEM_SIZE,WHITE(1.0f),ITEM_ID_BUTA,Sprite::LAYER_2);
	for (int cnt = 0;cnt < 7;cnt++)
	{
		wall = BreakObject::Create(D3DXVECTOR2(6480.0f,470.0f + cnt*64.0f),D3DXVECTOR2(128.0f,256.0f),TEX_LOG_BREAK,false);
		wall->SetRot(PI / 2.0f);
	}

	Wall::Create(D3DXVECTOR2(6730.0f,384.0f),D3DXVECTOR2(256.0f,768.0f),TEX_BREAKABLE_OBJ);

	Taizou::Create(D3DXVECTOR2(7700.0f,750.0f));

	Wall::Create(vector2(7316.0f,600.0f),vector2(512.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	Wall::Create(vector2(8080.0f,600.0f),vector2(512.0f,128.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	Wall::Create(vector2(7124.0f,800.0f),vector2(128.0f,256.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);
	Wall::Create(vector2(8278.0f,800.0f),vector2(128.0f,256.0f),TEX_BREAKABLE_OBJ,Sprite::LAYER_2);

	Sensor* sensor = Sensor::Create(vector2(7700.0f,850.0f),50.0f);
	Clog::Create(vector2(7700.0f,850.0f),vector2(128.0f,128.0f));
	sensor->SetAction([](Gimmick* gimmick)
	{
		VALUE<Creature>* creature = Creature::HitList()->Begin();
		bool hit = false;
		while (creature)
		{
			if (Collision::CircleDot(gimmick->Pos(),70.0f,creature->Data->Pos()))
			{
				((Sensor*)gimmick)->AddFrame();
				hit = true;
				break;
			}
			creature = creature->_Next;
		}
		if (hit)
		{
			if (((Sensor*)gimmick)->Frame() >= 60 && creature)
			{
				creature->Data->Capture();
			}
		}
		else
		{
			((Sensor*)gimmick)->SetFrame();
		}
	});

	start = FixedArea::Create(vector2(7700.0f,550.0f),vector2(1000.0f,700.0f));
	start->SetHitClog(false);
	
	gimmick = FixedCamera::Create(vector2(7700.0f,450.0f),vector2(7700.0f - SCREEN_WIDTH/2.0f,600.0f - SCREEN_HEIGHT/2.0f));
	gimmick->AddStartDevice(start);

	start = Stump::Create(vector2(7300.0f,540.0f),vector2(128.0f,128.0f));

	sensor->AddStartDevice(start);
	gimmick = Gate::Create(vector2(7900.0f,450.0f),vector2(128.0f,256.0f),TEX_LOG_GIMMICK,Sprite::LAYER_0);
	gimmick->SetAction([](Gimmick* gimmick){
		float y = (800.0f - gimmick->Pos().y)*0.1f;
		gimmick->AddPosY(y);
	});
	gimmick->AddStartDevice(start);

	start = Stump::Create(vector2(8100.0f,540.0f),vector2(128.0f,128.0f));
	sensor->AddStartDevice(start);
	gimmick = Gate::Create(vector2(7500.0f,450.0f),vector2(128.0f,256.0f),TEX_LOG_GIMMICK,Sprite::LAYER_0);
	gimmick->SetAction([](Gimmick* gimmick){
		float y = (800.0f - gimmick->Pos().y)*0.1f;
		gimmick->AddPosY(y);
	});
	gimmick->AddStartDevice(start);

	for (int y = 0;y < 12;y++)
	{
		for (int x = 0;x < 6;x++)
		{
			wall = BreakObject::Create(vector2(8700.0f + x*200.0f,100.0f + 72.0f*y),vector2(128.0f,256.0f),TEX_LOG_BREAK);
			wall->SetRot(PI/2.0f);
		}
	}

	//ゴール
	Goal* goal = Goal::Create(D3DXVECTOR3(10000.0f,800.0f,0.0f),D3DXVECTOR2(256.0f,256.0f),WHITE(1.0f),Sprite::LAYER_2);

}

//=============================================================================
//更新
//=============================================================================
void Stage_1::Update(void)
{
	Stage::Update();
}

//=============================================================================
//更新
//=============================================================================
void Stage_1::Release(void)
{
	delete this;
}