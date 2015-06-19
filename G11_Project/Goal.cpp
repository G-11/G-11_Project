//================================================================================
//	ゴールオブジェクト
//	Author: Toshiyuki Tsukagoshi
//================================================================================
#include "Goal.h"
#include "Texture.h"
#include "Collision.h"
#include "Game.h"
#include "Player.h"
#include "Manager.h"
#include "Fade.h"

Player* Goal::player = nullptr;

//================================================================================
//	コンストラクタ
//================================================================================
Goal::Goal(int priority) :Sprite(priority)
{

}

//================================================================================
//	生成
//================================================================================
Goal* Goal::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority)
{
	Goal* goal = new Goal(priority);
	goal->_Pos = pos;
	goal->_Size = D3DXVECTOR3(size.x, size.y, 0.0f);
	goal->_Color = color;

	//テクスチャできたらテクスチャ作成
	goal->SetTexture(GetTexture(TEX_GOAL));

	return goal;
}

//================================================================================
//	更新
//================================================================================
void Goal::Update()
{
	if (Collision::Quad(_Pos, _Quad, 4, player->Pos(), player->Quad(), 4))
	{
		if (Fade::Instance()->Mode() == Fade::FADE_NONE)
		{
			Manager::Instance()->SetScene(Manager::SCENE_RESULT);
		}
	}

	Sprite::Update();
}

//================================================================================
//	描画
//================================================================================
void Goal::Draw()
{
	Sprite::Draw();
}

