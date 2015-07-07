//================================================================================
//	いーたんクラス
//	Author:Tsukagoshi Toshiyuki
//================================================================================
#include "Eatan.h"
#include "Shader2D.h"
#include "Texture.h"


//================================================================================
//	定数
//================================================================================
//アニメーションの分割数
#define EATAN_PARTITION_STOP		(2)			
#define EATAN_PARTITION_MOVE		(2)
#define EATAN_PARTITION_EAT			(6)
#define EATAN_PARTITION_GLAD		(6)
#define EATAN_PARTITION_ATTACK		(2)
#define EATAN_PARTITION_MASTICATION	(2)
#define EATAN_PARTITION_REVERSE		(3)
#define EATAN_PARTITION_HAVE		(2)
#define EATAN_PARTITION_WORRIED		(2)
#define EATAN_PARTITION_EARN		(1)
#define EATAN_PARTITION_DASH		(1)
#define EATAN_PARTITION_SLEEP		(1)

//アニメーションのスピード
#define EATAN_ANIMATION_SPEED_STOP			(0.03f)
#define EATAN_ANIMATION_SPEED_MOVE			(0.03f)
#define EATAN_ANIMATION_SPEED_EAT			(0.2f)
#define EATAN_ANIMATION_SPEED_GLAD			(0.03f)
#define EATAN_ANIMATION_SPEED_ATTACK		(0.2f)
#define EATAN_ANIMATION_SPEED_MASTICATION	(0.2f)
#define EATAN_ANIMATION_SPEED_REVERSE		(0.02f)
#define EATAN_ANIMATION_SPEED_HAVE			(0.03f)
#define EATAN_ANIMATION_SPEED_WORRIED		(0.03f)
#define EATAN_ANIMATION_SPEED_EARN			(0.01f)
#define EATAN_ANIMATION_SPEED_DASH			(0.01f)
#define EATAN_ANIMATION_SPEED_SLEEP			(0.01f)

//アニメーションの繰り返し数(0で無限ループ、1で繰り返しなし,それ以外は指定回数繰り返し)
#define EATAN_ANIMATION_REPEATE_STOP			(0)
#define EATAN_ANIMATION_REPEATE_MOVE			(0)
#define EATAN_ANIMATION_REPEATE_EAT				(1)
#define EATAN_ANIMATION_REPEATE_GLAD			(1)
#define EATAN_ANIMATION_REPEATE_ATTACK			(1)
#define EATAN_ANIMATION_REPEATE_MASTICATION		(5)
#define EATAN_ANIMATION_REPEATE_REVERSE			(1)
#define EATAN_ANIMATION_REPEATE_HAVE			(0)
#define EATAN_ANIMATION_REPEATE_WORRIED			(3)
#define EATAN_AMIMATION_REPEATE_EARN			(0)
#define EATAN_AMIMATION_REPEATE_DASH			(0)
#define EATAN_ANIMATION_REPEATE_SLEEP			(0)

//上下に揺れる移動量
//
#define EATAN_SWAY_HEIGHT		(0.05f)

//上下に揺れるスピード
#define EATAN_SWAY_SPEED		(0.04f)

//================================================================================
//	コンストラクタ
//================================================================================
Eatan::Eatan(int priority) :Sprite(priority)
{
	//ステータスの初期化
	_State = EATAN_STATE_STOP;			//停止状態で生成
	OldState = EATAN_STATE_STOP;
	NextState = EATAN_STATE_NON;
	MaxPartition = 0;
	SwayCount = 0;

	AnimationPartition[EATAN_STATE_STOP] = EATAN_PARTITION_STOP;
	AnimationPartition[EATAN_STATE_MOVE] = EATAN_PARTITION_MOVE;
	AnimationPartition[EATAN_STATE_EAT] = EATAN_PARTITION_EAT;
	AnimationPartition[EATAN_STATE_GLAD] = EATAN_PARTITION_GLAD;
	AnimationPartition[EATAN_STATE_ATTACK] = EATAN_PARTITION_ATTACK;
	AnimationPartition[EATAN_STATE_REVERSE] = EATAN_PARTITION_REVERSE;
	AnimationPartition[EATAN_STATE_HAVE] = EATAN_PARTITION_HAVE;
	AnimationPartition[EATAN_STATE_WORRIED] = EATAN_PARTITION_WORRIED;
	AnimationPartition[EATAN_STATE_MASTICATION] = EATAN_PARTITION_MASTICATION;
	AnimationPartition[EATAN_STATE_EARN] = EATAN_PARTITION_EARN;
	AnimationPartition[EATAN_STATE_DASH] = EATAN_PARTITION_DASH;
	AnimationPartition[EATAN_STATE_SLEEP] = EATAN_PARTITION_SLEEP;

	AnimationSpeed[EATAN_STATE_STOP] = EATAN_ANIMATION_SPEED_STOP;
	AnimationSpeed[EATAN_STATE_MOVE] = EATAN_ANIMATION_SPEED_MOVE;
	AnimationSpeed[EATAN_STATE_EAT] = EATAN_ANIMATION_SPEED_EAT;
	AnimationSpeed[EATAN_STATE_GLAD] = EATAN_ANIMATION_SPEED_GLAD;
	AnimationSpeed[EATAN_STATE_ATTACK] = EATAN_ANIMATION_SPEED_ATTACK;
	AnimationSpeed[EATAN_STATE_REVERSE] = EATAN_ANIMATION_SPEED_REVERSE;
	AnimationSpeed[EATAN_STATE_HAVE] = EATAN_ANIMATION_SPEED_HAVE;
	AnimationSpeed[EATAN_STATE_WORRIED] = EATAN_ANIMATION_SPEED_WORRIED;
	AnimationSpeed[EATAN_STATE_MASTICATION] = EATAN_ANIMATION_SPEED_MASTICATION;
	AnimationSpeed[EATAN_STATE_EARN] = EATAN_ANIMATION_SPEED_EARN;
	AnimationSpeed[EATAN_STATE_DASH] = EATAN_ANIMATION_SPEED_DASH;
	AnimationSpeed[EATAN_STATE_SLEEP] = EATAN_ANIMATION_SPEED_SLEEP;

	AnimationRepeat[EATAN_STATE_STOP] =			EATAN_ANIMATION_REPEATE_STOP;
	AnimationRepeat[EATAN_STATE_MOVE] =			EATAN_ANIMATION_REPEATE_MOVE;
	AnimationRepeat[EATAN_STATE_EAT] =			EATAN_ANIMATION_REPEATE_EAT;
	AnimationRepeat[EATAN_STATE_GLAD] =			EATAN_ANIMATION_REPEATE_GLAD;
	AnimationRepeat[EATAN_STATE_ATTACK] =		EATAN_ANIMATION_REPEATE_ATTACK;
	AnimationRepeat[EATAN_STATE_REVERSE] =		EATAN_ANIMATION_REPEATE_REVERSE;
	AnimationRepeat[EATAN_STATE_HAVE] =			EATAN_ANIMATION_REPEATE_HAVE;
	AnimationRepeat[EATAN_STATE_WORRIED] =		EATAN_ANIMATION_REPEATE_WORRIED;
	AnimationRepeat[EATAN_STATE_MASTICATION] =	EATAN_ANIMATION_REPEATE_MASTICATION;
	AnimationRepeat[EATAN_STATE_EARN] =			EATAN_AMIMATION_REPEATE_EARN;
	AnimationRepeat[EATAN_STATE_DASH] =			EATAN_AMIMATION_REPEATE_DASH;
	AnimationRepeat[EATAN_STATE_SLEEP] =		EATAN_ANIMATION_REPEATE_SLEEP;

	//アニメーション枚数の最大値
	for (int i = 0; i < EATAN_STATE_MAX; i++)
	{
		MaxPartition = max(MaxPartition, AnimationPartition[i]);
	}

	StateList.clear();

	LoopCount = 0;
}

//================================================================================
//	生成
//================================================================================
Eatan* Eatan::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority)
{
	Eatan* eatan = new Eatan(priority);
	eatan->_Pos = pos;
	eatan->_Size = D3DXVECTOR3(size.x, size.y, 0);
	eatan->_Color = color;

	eatan->SetTexture(GetTexture(TEX_EATAN));
	eatan->SetSwayFlag(false);

	return eatan;
}

//================================================================================
//	更新
//================================================================================
void Eatan::Update()
{
	//上下にふわふわ移動
	if (SwayFlag)
	{
		SwayCount += EATAN_SWAY_SPEED;
		SetOffsetY( sinf(SwayCount)*EATAN_SWAY_HEIGHT - 0.2f);
	}

	//前回と状態が変わっていたらアニメーション用カウンタをリセット
	if (OldState != _State)
	{
		AnimationCount = 0.0f;
	}
	//変更されていなければカウンタの更新
	else
	{
		AnimationCount += AnimationSpeed[_State];

		if (AnimationCount > AnimationPartition[_State])
		{
			AnimationCount = 0.0f;
			//アニメーションのループ
			//無限ループのアニメーション
			if (AnimationRepeat[_State] == 0)
			{

			}
			//繰り返し回数指定のアニメーション
			else
			{
				LoopCount++;

				//繰り返し終了
				if (LoopCount + 1 > AnimationRepeat[_State])
				{
					if (SwayFlag)
					{
						_State = EATAN_STATE_STOP;
					}
					else
					{
						_State = EATAN_STATE_STOP;
					}

					LoopCount = 0;

					//次のステートが設定されていたらそちらに変更
					EATAN_STATE state = EATAN_STATE_NON;
					if (!StateList.empty())state = StateList.front();
					if (state != EATAN_STATE_NON)
					{
						_State = state;
						StateList.pop_front();
					}
				}

				
				
			}


		}
	}

	//移動方向の判定	
	if (_Speed.x >= 0)
	{
		//テクスチャUVの変更
		SetUV(D3DXVECTOR4((1.0f / MaxPartition)*(int)AnimationCount, (1.0f / EATAN_STATE_MAX)*_State, 1.0f / (float)MaxPartition, 1.0f / (float)EATAN_STATE_MAX));
	}
	else
	{
		//テクスチャUVの変更
		SetUV(D3DXVECTOR4((1.0f / MaxPartition)*((int)AnimationCount + 1), (1.0f / EATAN_STATE_MAX)*_State, 1.0f / (float)-MaxPartition, 1.0f / (float)EATAN_STATE_MAX));
	}

	//ステートの保存
	OldState = _State;

	Sprite::Update();
}

//================================================================================
//　描画処理
//================================================================================
void Eatan::Draw()
{
	Sprite::Draw();
}

//================================================================================
//	ステートの設定
//================================================================================
void Eatan::SetState(EATAN_STATE State)
{
	//以下の状態のときはステートの変更を受け付けない
	//食べる
	//攻撃
	//吐く
	if (_State != EATAN_STATE_EAT &&
		_State != EATAN_STATE_ATTACK &&
		_State != EATAN_STATE_REVERSE)
	{
		_State = State;
	}
}

void Eatan::AddState(EATAN_STATE state)
{
	//リスト末尾と同じステートは無視する
	
	EATAN_STATE LastState = EATAN_STATE_NON;
	if(!StateList.empty())StateList.back();
	if (LastState == state)
	{
		return;
	}

	StateList.push_back(state);
}