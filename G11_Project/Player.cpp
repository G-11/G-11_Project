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
#include "GrowupEffect.h"
#include "CheckPoint.h"
#include "Clog.h"
//================================================================================
//	定数
//================================================================================
//移動スピード
#define PLAYER_SPEED		(0.5f)

//慣性の減衰率
#define PLAYER_INERTIA		(0.93f)

//反射方向の開き(DEG)		
#define PLAYER_REFRECT_RAND (30.0f)

//ギミックに当たった時の押し出されるスピード
#define PLAYER_REFRECT_SPEED (5.0f)


//================================================================================
//	コンストラクタ
//================================================================================
Player::Player(int Priority) :Eatan(Priority)
{
	HitCheckFlag = true;
	ActionFlag = true;
	CurrentCheckPoint = nullptr;
	CheckPointPos = D3DXVECTOR3(0,0,0);
	HavingObject = nullptr;
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

	bool have = false;
	//当たり判定フラグ
	if (HitCheckFlag)
	{
		HitWall();
		HitCheckPoint();
		have = HitClog();

	}

	//操作フラグ
	if (ActionFlag)
	{
		//****************************************
		//	移動
		//****************************************

		_Speed += D3DXVECTOR3(vc->LStickX(), vc->LStickY(), 0.0f) * PLAYER_SPEED;
		_Speed *= PLAYER_INERTIA;

		if (_State == EATAN_STATE_EAT || _State == EATAN_STATE_MASTICATION || _State == EATAN_STATE_REVERSE)
		{
			_Speed *= 0.5f;
		}

		//持っている物に座標を渡す
		if (HavingObject != nullptr)
		{
			D3DXVECTOR3 pos = _Pos;
			pos.y += _Size.y/2.0f;
			HavingObject->SetPos(pos);
		}

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
			if (!have && HavingObject != nullptr)
			{
				HavingObject->SetHave(false);
				HavingObject = nullptr;
			}
			else
			{
				SetState(EATAN_STATE_EAT);
			}

		}

		//食べる判定
		if (_State == EATAN_STATE_EAT)
		{
			
			//当たり判定
			Item* item = Item::HitCheck(_Pos, _Size);
			if (item != nullptr)
			{
				float score = item->Score();
				Interface::AddScore(score);
				Score = (int)score;
				item->Action(&_Pos, AnimationPartition[EATAN_STATE_EAT], (float)AnimationSpeed[EATAN_STATE_EAT]);
				

				if (score > 0)
				{
					GrowupEffect::Creates(_Pos,&_Pos,250.0f,60,12);
					AddState(EATAN_STATE_MASTICATION);
					AddState(EATAN_STATE_EAT);
					AddState(EATAN_STATE_MASTICATION);
					AddState(EATAN_STATE_GLAD);
				}
				else
				{
					GrowupEffect::Creates(_Pos,&_Pos,250.0f,60,12,CShader2D::SUB);
					AddState(EATAN_STATE_MASTICATION);
					AddState(EATAN_STATE_EAT);
					AddState(EATAN_STATE_MASTICATION);
					AddState(EATAN_STATE_REVERSE);
				}
			}
			
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
	CDebugProc::Print("Player:x.%f,y.%f",_Pos.x,_Pos.y);
	Eatan::Draw();
}

void Player::HitWall(void)
{
	List<Wall>* wall = Wall::HitList();
	VALUE<Wall>* itr = wall->Begin();

	while (itr)
	{
		D3DXVECTOR3 reflectVec(0,0,0);
		D3DXVECTOR3 *quad = itr->Data->Quad();

		if (Collision::CircleQuad(_Pos,5.0f,quad,4,_Speed,&reflectVec))
		{
			if (!itr->Data->HitAffect())
			{
				D3DXVECTOR3 wallSpeed = itr->Data->Speed();
				//壁の移動ベクトルからプレイヤー方向の成分のみを加算
				D3DXVECTOR3 playerwall = _Pos - itr->Data->Pos();
				D3DXVec3Normalize(&playerwall,&playerwall);
				float wallReflect = D3DXVec3Dot(&playerwall,&wallSpeed);
				playerwall *= (wallReflect + PLAYER_REFRECT_SPEED);

				//反射ベクトルをランダム変化させる
				float randReflect = Randf(-PLAYER_REFRECT_RAND*0.5,PLAYER_REFRECT_RAND*0.5);

				D3DXVECTOR3 speed = _Speed;
				float angle = atan2(reflectVec.x,reflectVec.y) + DEG2RAD(randReflect);
				float sp = D3DXVec3Length(&(speed + playerwall));
				_Speed.x = sinf(angle)*sp;
				_Speed.y = cosf(angle)*sp;
				angle = RAD2DEG(angle);
				SetPos(OldPos);
			}
		}

		itr = itr->_Next;
	}
}

void Player::HitCheckPoint(void)
{
	VALUE<CheckPoint>* checkPoint = CheckPoint::HitList()->Begin();

	while (checkPoint)
	{
		if (!checkPoint->Data->Active())
		{
			if (Collision::Circle(_Pos,10.0f,checkPoint->Data->Pos(),checkPoint->Data->Size().x))
			{
				checkPoint->Data->SetActive(true);
				CheckPointPos = checkPoint->Data->Pos();

				if (CurrentCheckPoint != nullptr){ CurrentCheckPoint->SetActive(false); }

				CurrentCheckPoint = checkPoint->Data;
				break;
			}
		}
		checkPoint = checkPoint->_Next;
	}
}

bool Player::HitClog(void)
{
	if (HavingObject == nullptr && VC::Instance()->Trigger(COMMAND_A))
	{
		VALUE<Clog>* clog = Clog::HitList()->Begin();

		while (clog)
		{
			if (Collision::Circle(_Pos,_Size.x / 2.0f,clog->Data->Pos(),clog->Data->Size().x))
			{
				clog->Data->SetHave(true);
				HavingObject = clog->Data;
				_State = EATAN_STATE_HAVE;
				return true;
			}
			clog = clog->_Next;
		}
	}
	return false;
}