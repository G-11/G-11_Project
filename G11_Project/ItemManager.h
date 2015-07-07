#pragma once
#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

#include "Item.h"
#include<vector>
#include<map>

struct ItemData
{
	ItemData(){ itemId = ITEM_ID_MAX; Score = 0; id = -1; Active = false; }
	ITEM_ID itemId;
	float Score;
	int id;
	bool Active;
	
};

class ItemManager
{
public:
	static void Reset(void);

	static void SetItem(ITEM_ID itemId,float score,int id);
	static int ListSize(void){ return ItemList.size(); }
	static ItemData GetItem(uint num);
	static ItemData GetItemFromArrayNum(uint num);
	static void Eat(int id);//Itemクラスのnumberの値を元に検索し、見つかればそのインスタンスを食べたことにする
	

private:
	static std::vector<ItemData> ItemList;
	//static std::map<int,ItemData> ItemList;

};

#endif