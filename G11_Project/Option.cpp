#include "Option.h"
#include "Sprite.h"
#include "Gauge.h"
#include "Font.h"

Option::Option()
{
	BGMFrame = SEFrame = nullptr;
	BGMGauge = SEGauge = nullptr;
	Cursor = nullptr;

	Select = 0;
}

void Option::Init(void)
{
	Sprite* bg = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0),D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT),WHITE(1.0f),Sprite::LAYER_BACKGROUND);
	bg->SetTexture(GetTexture(TEX_FIELD1_BG));

	String* string = String::CreateFast(D3DXVECTOR2(SCREEN_WIDTH / 2.0f,120.0f),80.0f,"‚¹‚Á‚Ä‚¢");
	string->SetColor(BLACK(1.0f));

	BGMGauge = Gauge::Create(D3DXVECTOR2(384.0f,200.0f),D3DXVECTOR2(128.0f,512.0f),0,1.0f,1.0f,UP,Sprite::LAYER_1);
	BGMGauge->SetRot(DEG2RAD(90.0f));
	BGMGauge->SetTexture(GetTexture(TEX_GLOW_GAUGE));
	BGMFrame = Sprite::Create(D3DXVECTOR3(640.0f,200.0f,0),D3DXVECTOR2(128.0f,512.0f),WHITE(1.0f),Sprite::LAYER_2);
	BGMFrame->SetRot(DEG2RAD(90.0f));
	BGMFrame->SetTexture(GetTexture(TEX_GLOE_GAUGE_FRAME));

	SEGauge = Gauge::Create(D3DXVECTOR2(384.0f,350.0f),D3DXVECTOR2(128.0f,512.0f),0,1.0f,1.0f,UP,Sprite::LAYER_1);
	SEGauge->SetRot(DEG2RAD(90.0f));
	SEGauge->SetTexture(GetTexture(TEX_GLOW_GAUGE));

	SEFrame = Sprite::Create(D3DXVECTOR3(640.0f,350.0f,0),D3DXVECTOR2(128.0f,512.0f),WHITE(1.0f),Sprite::LAYER_2);
	SEFrame->SetRot(DEG2RAD(90.0f));
	SEFrame->SetTexture(GetTexture(TEX_GLOE_GAUGE_FRAME));

}

void Option::Uninit(void)
{

}

void Option::Update(void)
{

}