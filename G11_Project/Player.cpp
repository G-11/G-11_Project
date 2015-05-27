//================================================================================
//	プレイヤークラス
//	Author:Tsukagoshi Toshiyuki
//================================================================================

//================================================================================
//	include
//================================================================================
#include "Player.h"
#include "Shader2D.h"
#include "Texture.h"
#include "Input\VC.h"
#include "Item.h"
#include "Interface.h"
#include "Wall.h"
#include "Collision.h"
#include "Camera2D.h"

//================================================================================
//	定数
//================================================================================
//移動スピード
#define PLAYER_SPEED		(0.5f)

//慣性の減衰率
#define PLAYER_INERTIA		(0.95f)


//================================================================================
//	コンストラクタ
//================================================================================
Player::Player(int Priority) :Eatan(Priority)
{
	HitCheckFlag = true;
	ActionFlag = true;
}

//================================================================================
//	デストラクタ
//================================================================================
Player::~Player()
{
	//カメラを初期位置に変更
	Camera2D* camera = Camera2D::GetCamera(0);
	camera->SetPosP(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//================================================================================
//	プレイヤー生成
//================================================================================
Player* Player::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority)
{
	Player* player = new Player(priority);
	player->_Pos = pos;
	player->_Size = D3DXVECTOR3(size.x, size.y, 0);
	player->_Color = color;

	player->SetTexture(GetTexture(TEX_EATAN));
	player->SetSwayFlag(true);
	player->SetState(EATAN_STATE_STOP);

	return player;
}

//================================================================================
//	更新処理
//================================================================================
void Player::Update()
{
	VC* vc = VC::Instance();

	//操作フラグ
	if (ActionFlag)
	{
		//****************************************
		//	移動
		//****************************************

		_Speed += D3DXVECTOR3(vc->LStickX(), vc->LStickY(), 0.0f) * PLAYER_SPEED;
		_Speed *= PLAYER_INERTIA;


		//****************************************
		//	攻撃
		//****************************************
		if (vc->Trigger(COMMAND_B))
		{
			SetState(EATAN_STATE_ATTACK);
		}

		//****************************************
		//	食べる
		//****************************************
		if (vc->Trigger(COMMAND_A))
		{
			SetState(EATAN_STATE_EAT);

		}

		//食べる判定
		if (_State == EATAN_STATE_EAT)
		{
			//口を開け切ったとき
			if (AnimationCount >= AnimationPartition[EATAN_STATE_EAT] / 2.0f)
			{
				//当たり判定
				Item* item = Item::HitCheck(_Pos, _Size);
				if (item != nullptr)
				{
					float score = item->Score();
					Interface::AddScore(score);
					item->SetRelease();
				}
			}
		}
	}

	//当たり判定フラグ
	if (HitCheckFlag)
	{
		List<Wall>* wall = Wall::HitList();
		VALUE<Wall>* itr = wall->Begin();

		while (itr)
		{
			D3DXVECTOR3 reflectVec(0, 0, 0);
			D3DXVECTOR3 *quad = itr->Data->Quad();

			if (Collision::CircleQuad(_Pos, 10.0f, quad, 4, _Speed, &reflectVec))
			{
				D3DXVECTOR3 speed = _Speed;
				float angle = atan2(reflectVec.x, reflectVec.y);
				float sp = D3DXVec3Length(&speed);
				_Speed.x = sinf(angle)*sp;
				_Speed.y = cosf(angle)*sp;
				angle = RAD2DEG(angle);
				SetPos(OldPos);
			}

			itr = itr->_Next;
		}

	}
	OldPos = _Pos;

	Eatan::Update();
}

//================================================================================
//	描画処理
//================================================================================
void Player::Draw()
{

	Eatan::Draw();
}