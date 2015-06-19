//
//Result.cpp
//

#include "Result.h"
#include "Sprite.h"
#include "Renderer.h"
#include "Manager.h"
#include "Gauge.h"


VIS_CHAR Result::ResultCharId[RESULTCHARMAX];

void Result::Init(void)
{
	Window* window = Window::Instance();


	BG = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), WHITE(1.0f), Sprite::LAYER_4);
	BG->SetTexture(GetTexture(TEX_TITLE_BG));
	BG->SetUV(D3DXVECTOR4(0, 0.2f, 1.0f, 0.7f));//画像調整


	Push = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH /1.2f, SCREEN_HEIGHT / 1.1f, 0), D3DXVECTOR2(250.0f, 100.0f), WHITE(1.0f), Sprite::LAYER_4);
	Push->SetTexture(GetTexture(TEX_INTERFACE_PUSH));

	//ゲージ
	GlowGauge = Gauge::Create(D3DXVECTOR2(SCREEN_WIDTH / 3.8f, SCREEN_HEIGHT / 1.4f), D3DXVECTOR2(100, 600), 0, 100.0f, 0, UP, Sprite::LAYER_INTERFACE);
	GlowGauge->SetDelay(0.02f);//ゲージの伸びるスピード？
	GlowGauge->SetTexture(GetTexture(TEX_GLOW_GAUGE));
	GlowGauge->SetRot(D3DX_PI/2);//90度へ
	GlowGauge->SetColor(RED(1.0f));

	//ゲージ枠
	GaugeFream = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT/1.4f, 0), D3DXVECTOR2(100, 600), WHITE(1.0f), Sprite::LAYER_INTERFACE);
	GaugeFream->SetTexture(GetTexture(TEX_GLOE_GAUGE_FRAME));//枠のテクスチャ
	GaugeFream->SetRot(D3DX_PI/2);//枠を90度へ



	//キャラ枠
	CharFream = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3.0f, 0), D3DXVECTOR2(180, 700), WHITE(1.0f), Sprite::LAYER_INTERFACE);
	CharFream->SetTexture(GetTexture(TEX_GLOE_GAUGE_FRAME));//枠のテクスチャ
	CharFream->SetRot(D3DX_PI / 2);//枠を90度へ

	charVisCnt = 0; 
	charVisTime = 0;
	charFlag = false;



	flahing = 0;
	add_flahing_num = 0.01f;
	gaugeCnt = 0;
	gaugeChangeColor = 0;

}

void Result::Uninit(void)
{
	ResetResultChar();
	Manager::ReleaseObject();
}

void Result::Update(void)
{
	/*点滅処理（仮）*/
	if (flahing > 1.0f || flahing < 0)
	{
		add_flahing_num = -add_flahing_num;
	}
	flahing += add_flahing_num;
	Push->SetColor(WHITE(flahing));
	//ここまで

	VC* vc = VC::Instance();
	if (vc->Trigger(COMMAND_OK) && Fade::Instance()->Mode() == Fade::FADE_NONE)
	{
		Manager::Instance()->SetScene(Manager::SCENE_TITLE);
	}

	//キャラ表示
	if (!charFlag)
	{
		//テスト 
		if (charVisTime > 30)
		{
			//表示フラグが立っていたら
			if (ResultCharId[charVisCnt].vis)
			{
				//ResultCharID内に入っているIDに対応するキャラの表示
				Item::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f + (100 * charVisCnt), SCREEN_HEIGHT / 3, 0), D3DXVECTOR2(84.0f, 84.0f), WHITE(1.0f), ResultCharId[charVisCnt].id, Sprite::LAYER_INTERFACE);
				charVisTime = 0;//Timeリセット
				gaugeCnt++;
			}
			//全体キャラ数までいったら
			if (charVisCnt > RESULTCHARMAX)
			{
				charFlag = true;//キャラクタ最大数まで表示したら
			}
			charVisCnt++;//
		}
		charVisTime++;//時間
	}

	if (charFlag)
	{
		float sub = RESULTCHARMAX*1.0f;
		if (gaugeChangeColor < gaugeCnt / sub * 100)
		{
			gaugeChangeColor++;
			switch (gaugeChangeColor)
			{
			case 25:
				GlowGauge->SetColor(YELLOW(1.0f));
				break;
			case 50:
				GlowGauge->SetColor(CYAN(1.0f));
				break;
			case 100:
				GlowGauge->SetColor(WHITE(1.0f));
				break;
			}
		}
		else
		{
			gaugeChangeColor = (int)(gaugeCnt / sub * 100);
		}
		GlowGauge->SetCurrent((float)gaugeChangeColor);
		
	}

}

void Result::Draw(void)
{

}

void Result::SetResultCharId(int id)
{
	for (int i = 0; i < RESULTCHARMAX; i++)
	{
		if (!ResultCharId[i].vis)
		{
			ResultCharId[i].id = id;
			ResultCharId[i].vis = true;
			break;
		}
	}
}


void Result::ResetResultChar(void)
{
	for (int i = 0; i < RESULTCHARMAX; i++)
	{
		ResultCharId[i].vis = false;
	}
}
