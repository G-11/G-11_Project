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

//================================================================================
//	アイテムクラス
//================================================================================
class Item :public Sprite
{
public:

	static Item* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, const int &id, int priority = 0);

	Item(int priority = 0);
	~Item(void);

	void Update();
	void Draw();

	void SetID(int ID){ _ID = ID; }
	void SetScore(float Score){ _Score = Score; }

	int	GetID(){ return _ID; }
	float GetScore(){ return _Score; }

private:
	static std::list<Item*> ItemList;
	int _ID;
	float _Score;
	static const float ItemScore[ITEM_ID_MAX];
};