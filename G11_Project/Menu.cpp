//=============================================================================
//インクルード
//=============================================================================
#include "Menu.h"
#include "Font.h"
#include "Input/VC.h"
#include "Shader2D.h"
//=============================================================================
//コンストラクタ
//=============================================================================
CMenu::CMenu()
{
	ItemList.clear();
	Caption = nullptr;
	Item = nullptr;
	_Over = false;
	_Select = false;
}

//=============================================================================
//デストラクタ
//=============================================================================
CMenu::~CMenu()
{

}

//=============================================================================
//生成(これだけだと使えない)
//=============================================================================)
CMenu* CMenu::Create(const D3DXVECTOR2& pos,float captionSize,bool left,float itemSize,const D3DXCOLOR& itemEdgeColor,const D3DXCOLOR& itemFillColor)
{
	CMenu* menu = new CMenu;
	if (menu == nullptr){ return nullptr; }
	menu->_Pos = pos;
	menu->_CaptionSize = captionSize;
	menu->_ItemSize = (itemSize<=0)?captionSize * 0.7f:itemSize;
	menu->ItemEdgeColor = itemEdgeColor;
	menu->ItemFillColor = itemFillColor;
	menu->_Left = left;

	return menu;
}

//=============================================================================
//起動
//=============================================================================
void CMenu::Init(void)
{
	ItemMax = ItemList.size();

	//メニュー内容を作成
	Item = new String*[ItemMax];
	DestItemPos = new D3DXVECTOR2[ItemMax];
	Font::SetEdgeColor(ItemEdgeColor);
	Font::SetFillColor(ItemFillColor);
	float length = FLT_MAX;
	
	if (Caption != nullptr){ length = Caption->Length(); }

	for (int cnt = 0;cnt < ItemMax;cnt++)
	{
		char* str = ItemList[cnt];
		Item[cnt] = String::Create(D3DXVECTOR2(_Pos.x,_Pos.y + _ItemSize),_ItemSize,str);
		DestItemPos[cnt] = D3DXVECTOR2(_Pos.x,_Pos.y + _ItemSize);
		(cnt==0)?Item[cnt]->SetAlpha(1.0f):Item[cnt]->SetAlpha(0);

		if (length < Item[cnt]->Length())
		{
			length = Item[cnt]->Length();
		}
	}
	D3DXVECTOR3 pos;
	pos.x = Item[0]->Pos().x;
	pos.y = Item[0]->Pos().y;
	pos.z = 0;

	Font::SetEdgeColor(D3DXCOLOR(0,0,0,1.0f));
	Font::SetFillColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
	ItemList.clear();
	if (!ItemList.empty())
	{
		ItemList.clear();
	}

	Alpha = -0.02f;
}

//=============================================================================
//更新
//=============================================================================
void CMenu::Update(void)
{
	/*
	if (_Select)
	{
		
		if (GetControllerTrigger(BUTTON_UP))
		{
			SelectID--;
		}
		else if (GetControllerTrigger(BUTTON_DOWN))
		{
			SelectID++;
		}

		SelectID %= ItemMax;
		if (SelectID < 0)
		{
			SelectID = ItemMax - 1;
		}

		D3DXVECTOR2 pos;
		for (int cnt = 0;cnt < ItemMax;cnt++)
		{
			if (cnt == SelectID)
			{
				if (Item[cnt]->Color().a < 1.0f)
				{
					Item[cnt]->AddAlpha(0.05f);
				}
			}
			else
			{
				(Item[cnt]->Color().a < 0.3f) ? Item[cnt]->AddAlpha(0.05f) : Item[cnt]->AddAlpha(-0.05f);
			}
			
			pos = _Pos;
			pos.x -= (_Left) ? Caption->Length() + Item[cnt]->Length() / 2.0f : -(Caption->Length() + Item[cnt]->Length() / 2.0f);
			pos.y += (cnt - SelectID)*(_ItemSize*1.5f);
			DestItemPos[cnt] = pos;
		}
	}
	else
	{
		for (int cnt = 0;cnt < ItemMax;cnt++)
		{
			if (cnt != SelectID && Item[cnt]->Color().a > 0.0f)
			{
				Item[cnt]->AddAlpha(-0.05f);
			}
			else if (cnt == SelectID && Item[cnt]->Color().a < 1.0f)
			{
				Item[cnt]->AddAlpha(0.05f);
			}
			
		}
	}
	if (_Over)
	{
		Caption->AddMaskAlpha(Alpha);
		if (Caption->MaskColor().a <= 0 || Caption->MaskColor().a >= 1.0f)
		{
			Alpha *= -1;
		}
	}
	D3DXVECTOR2 pos;
	for (int cnt = 0;cnt < ItemMax;cnt++)
	{
		pos = (DestItemPos[cnt] -  Item[cnt]->Pos())*0.2f;
		Item[cnt]->AddPos(pos);
	}
	*/
}

//=============================================================================
//選択されているかどうかをスイッチング
//=============================================================================
void CMenu::SwitchSelect(void)
{
	if (_Select)
	{
		_Select = false;
		
		float offset = 0;
		for (int cnt = 0;cnt < ItemMax;cnt++)
		{
			DestItemPos[cnt] = _Pos;
			DestItemPos[cnt].y += _ItemSize/2.0f + _CaptionSize/2.0f;
		}
		D3DXVECTOR3 pos;
		pos.x = DestItemPos[SelectID].x;
		pos.y = DestItemPos[SelectID].y;
		pos.z = 0;
	}
	else
	{
		_Select = true;
		D3DXVECTOR2 pos;
		for (int cnt = 0;cnt < ItemMax;cnt++)
		{
			pos = _Pos;
			pos.x -= (_Left) ? Caption->Length() + Item[cnt]->Length() / 2.0f : -(Caption->Length() + Item[cnt]->Length() / 2.0f);
			pos.y += (cnt - SelectID)*(_ItemSize*1.5f);
			DestItemPos[cnt] = pos;
		}
	}

}

//=============================================================================
//開放
//=============================================================================
void CMenu::Release(void)
{
	if (Item != nullptr)
	{
		for (int cnt = 0;cnt < ItemMax;cnt++)
		{
			if (Item[cnt] != nullptr)
			{
				Item[cnt]->Release();
			}
			
		}
		delete[] Item;
		Item = nullptr;
	}
	if (Caption != nullptr)
	{
		Caption->Release();
		Caption = nullptr;
	}
	if (DestItemPos != nullptr)
	{
		delete[] DestItemPos;
		DestItemPos = nullptr;
	}


	delete this;
}
//=============================================================================
//項目名をセット
//=============================================================================
void CMenu::SetCaption(char* caption)
{
	if (Caption != nullptr)
	{
		Caption->Release();
		Caption = nullptr;
	}
	Caption = String::Create(D3DXVECTOR2(_Pos.x,_Pos.y-(_CaptionSize*1.2f)),_CaptionSize,caption);	
	Caption->SetMask(GetTexture(TEX_NONE));
	Caption->SetMaskColor(YELLOW(1.0f));
}

//=============================================================================
//内容を追加
//=============================================================================
void CMenu::AddItem(char* item)
{
	ItemList.push_back(item);
}

void CMenu::SwitchOver(void)
{
	if (_Over)
	{
		_Over = false;
		Caption->SetPass(CShader2D::NORMAL);
	}
	else
	{
		_Over = true;
		Caption->SetPass(CShader2D::COLOR_MASK);
	}
}