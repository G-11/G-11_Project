//================================================================================
//	アイテムクラス
//	Author:Tsukagoshi Toshiyuki
//================================================================================
#pragma once

//================================================================================
//	include
//================================================================================
#include "Sprite.h"
#include <list>

//================================================================================
//	列挙
//================================================================================
typedef enum
{
	ITEM_ID_NON = 0,
	ITEM_ID_USAGI,
	ITEM_ID_NEKO,
	ITEM_ID_KUMA,
	ITEM_ID_MAX,
}ITEM_ID;

class Item;

//================================================================================
//	アイテムクラス
//================================================================================
class Item :public Sprite
{
public:
	//定数
	const static D3DXVECTOR2 DEFAULT_ITEM_SIZE;

	enum
	{
		ITEM_STATE_NON = 0,
		ITEM_STATE_EATED,
		ITEM_STATE_MAX,
	};
public:
	Item(int priority = 0);
	~Item();

	static Item* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, const int &id, int priority = Sprite::LAYER_2);

	virtual void Update();

	void SetID(int ID){ _ID = ID; }
	void SetScore(float Score){ _Score = Score; }

	int	ID(){ return _ID; }
	float Score(){ return _Score; }
	static List<Item>* ItemList(void){return &_ItemList; }
	static void InitList(void){ _ItemList.Clear(); }

	void Eated(D3DXVECTOR3* pos, float CountMax, float AnimationSpeed);			//食べられた時のアニメーション
	virtual void Action();		//アイテム取得時の動作
	virtual void Action(D3DXVECTOR3* pos, float CountMax, float AnimationSpeed);
	static Item* HitCheck(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Size);

	int State(){ return _State; }

protected:
	static List<Item> _ItemList;

	VALUE<Item>* SelfIterator;
	int _ID;
	float _Score;
	static const float ItemScore[ITEM_ID_MAX];

	int _State;
	D3DXVECTOR3* PlayerPos;
	float _CountMax;
	float _AnimationSpeed;
	float EatedCount;
};