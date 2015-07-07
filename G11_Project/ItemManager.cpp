#include"ItemManager.h"

std::vector<ItemData> ItemManager::ItemList;

void ItemManager::Reset(void)
{
	ItemList.clear();
}

void ItemManager::SetItem(ITEM_ID itemId,float score,int id)
{
	ItemData data;
	data.itemId = itemId;
	data.Score = score;
	data.id = id;
	ItemList.push_back(data);
}

ItemData ItemManager::GetItem(uint id)
{
	for (uint cnt = 0;cnt < ItemList.size();cnt++)
	{
		if (ItemList[cnt].id == id)
		{
			return ItemList[cnt];
		}
	}
	
	return ItemData();
}

ItemData ItemManager::GetItemFromArrayNum(uint num)
{
	if (num < ItemList.size())
	{
		return ItemList[num];
	}
	return ItemData();
}

void ItemManager::Eat(int id)
{
	for (uint cnt = 0;cnt < ItemList.size();cnt++)
	{
		if (ItemList[cnt].id == id)
		{
			ItemList[cnt].Active = true;
		}
	}
}