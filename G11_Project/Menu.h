//=============================================================================
//メニュークラス
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_
//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include<vector>

//=============================================================================
//前方宣言
//=============================================================================
class String;
class Sprite;
//=============================================================================
//クラス定義
//=============================================================================
class CMenu
{
public:
	CMenu();
	~CMenu();

	static CMenu* Create(const D3DXVECTOR2& pos,float captionSize,bool left = true,float itemSize = -1,const D3DXCOLOR& itemEdgeColor = D3DXCOLOR(0,0,0,1.0f),const D3DXCOLOR& itemFillColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	void Init(void);
	void Update(void);
	void Release(void);

	void SetCaptionSize(float size){_CaptionSize = size; }
	void SetItemSize(float size){ _ItemSize = size; }
	void SetCaption(char* caption);
	void AddItem(char* item);

	void SetPos(const D3DXVECTOR2& pos){ _Pos = pos; }
	void AddPos(const D3DXVECTOR2& pos){ _Pos += pos; }

	D3DXVECTOR2& Pos(void){ return _Pos; }

	void SwitchSelect(void);
	void SwitchOver(void);
	int ID(void){ return SelectID; }
	bool Select(void){ return _Select; }
	bool Over(void){ return _Over; }

private:
	String* Caption;
	String** Item;
	D3DXVECTOR2* DestItemPos;
	std::vector<char*> ItemList;

	D3DXVECTOR2 _Pos;
	D3DXCOLOR ItemEdgeColor;
	D3DXCOLOR ItemFillColor;
	float _CaptionSize;
	float _ItemSize;
	bool _Over;
	bool _Select;
	bool _Left;
	int ItemMax;
	int SelectID;
	float Alpha;
};


#endif