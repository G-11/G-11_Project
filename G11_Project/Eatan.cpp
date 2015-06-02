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
#define EATAN_PARTITION_STOP		(1)			
#define EATAN_PARTITION_MOVE		(2)
#define EATAN_PARTITION_EAT			(6)
#define EATAN_PARTITION_GLAD		(6)
#define EATAN_PARTITION_ATTACK		(2)
#define EATAN_PARTITION_REVERSE		(3)

//アニメーションのスピード
#define EATAN_ANIMATION_SPEED_STOP		(0.03f)
#define EATAN_ANIMATION_SPEED_MOVE		(0.03f)
#define EATAN_ANIMATION_SPEED_EAT		(0.2f)
#define EATAN_ANIMATION_SPEED_GLAD		(0.03f)
#define EATAN_ANIMATION_SPEED_ATTACK	(0.2f)
#define EATAN_ANIMATION_SPEED_REVERSE	(0.02f)

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

	AnimationSpeed[EATAN_STATE_STOP] = EATAN_ANIMATION_SPEED_STOP;
	AnimationSpeed[EATAN_STATE_MOVE] = EATAN_ANIMATION_SPEED_MOVE;
	AnimationSpeed[EATAN_STATE_EAT] = EATAN_ANIMATION_SPEED_EAT;
	AnimationSpeed[EATAN_STATE_GLAD] = EATAN_ANIMATION_SPEED_GLAD;
	AnimationSpeed[EATAN_STATE_ATTACK] = EATAN_ANIMATION_SPEED_ATTACK;
	AnimationSpeed[EATAN_STATE_REVERSE] = EATAN_ANIMATION_SPEED_REVERSE;

	//アニメーション枚数の最大値
	for (int i = 0; i < EATAN_STATE_MAX; i++)
	{
		MaxPartition = max(MaxPartition, AnimationPartition[i]);
	}
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
		SetOffsetY( sinf(SwayCount)*EATAN_SWAY_HEIGHT);
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
			//繰り返さないモーション
			//食べる
			//攻撃
			if (_State == EATAN_STATE_EAT ||
				_State == EATAN_STATE_ATTACK ||
				_State == EATAN_STATE_GLAD ||
				_State == EATAN_STATE_REVERSE)
			{
				if (SwayFlag)
				{
					_State = EATAN_STATE_MOVE;
				}
				else
				{
					_State = EATAN_STATE_STOP;
				}
			}

			//次のステートが設定されていたらそちらに変更
			if (NextState != EATAN_STATE_NON)
			{
				_State = NextState;
				NextState = EATAN_STATE_NON;
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