//=============================================================================
//アウトラインフォントクラス(まだ修正箇所残)
//2Dポリゴンクラスを継承
//=============================================================================
#ifndef _FONT_H_
#define _FONT_H_

//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Sprite.h"
#include <ctype.h>

//=============================================================================
//フォントデータ構造体
//=============================================================================
typedef struct
{
	char faceName[32];
	unsigned char charSet;
	unsigned fontHeight;
	unsigned weight;
	int penSize;
	unsigned edgeColor;
	unsigned fillColor;
	int quality;
}FONT_DATA;

//=============================================================================
//文字クラス定義
//=============================================================================
class Font :public Sprite
{
public:
	Font();
	~Font();
	static void Initialize(void);
	static Font* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,char* str);
	static Font* CreateFast(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,char* str);

	void Update(void);

	void SetOffsetPos(const D3DXVECTOR2& pos){ _OffsetPos = pos;}
	void AddOffsetPos(const D3DXVECTOR2& pos){ _OffsetPos += pos; }

	D3DXVECTOR2 OffsetPos(void)const{ return _OffsetPos; }

	static void SetFaceName(char faceName[]);
	static void SetEdgeColor(const D3DXCOLOR& color){ FontData.edgeColor = color; }
	static void SetFillColor(const D3DXCOLOR& color){ FontData.fillColor = color; }
	static void SetPenSize(const int size){ FontData.penSize = size; }
	static void SetQuality(const int quality){ FontData.quality = quality; }
	static void SetWeight(const UINT weight){ FontData.weight = weight; }
	static void SetCharSet(unsigned char charset){ FontData.charSet = charset; }
	static void SetFontHeight(const unsigned height){ FontData.fontHeight = height; }

	static void SetFastFontSize(const LONG& size){ FastFont.lfHeight = size; }
	static void SetFastFontFacename(char name[]);
	

protected:

	D3DXVECTOR2 _OffsetPos;

	void CreateTex(char* str,unsigned fontHeight,unsigned weight,int quality);
	static FONT_DATA FontData;
	static LOGFONT FastFont;

	static LPDIRECT3DTEXTURE9 createOutlineFontTexture(IDirect3DDevice9 *dev,const char *str = "～",RECT *info = 0);
	static LPDIRECT3DTEXTURE9 createFastFontTexture(IDirect3DDevice9 *dev,Font* instance,const char *str = "～");
};

//=============================================================================
//文字列クラス定義
//=============================================================================
class String
{
public:
	String();
	~String();

	static void Initialize(void);

	virtual void Release(void);

	static void UpdateAll(void);
	static void ReleaseAll(void);

	static String* Create(const D3DXVECTOR2& pos,float size,char* str);
	static String* CreateFast(const D3DXVECTOR2& pos,float size,char* str);

	void SetPos(const D3DXVECTOR2& pos){_Pos = pos;_CommitMatrix = true;}
	void SetPosX(float x){ _Pos.x = x; _CommitMatrix = true;}
	void SetPosY(float y){ _Pos.x = y; _CommitMatrix = true;}
	void SetPosZ(float z){ _Pos.x = z; _CommitMatrix = true;}

	void SetRot(float rot){ _RotZ = rot; _CommitMatrix = true; }

	void SetColor(const D3DXCOLOR& color)	{ _Color = color;	CommitColor(); }
	void SetRed(float red)					{ _Color.r = red;	CommitColor(); }
	void SetBlue(float blue)				{ _Color.b = blue;	CommitColor(); }
	void SetGreen(float green)				{ _Color.g = green;	CommitColor(); }
	void SetAlpha(float alpha)				{ _Color.a = alpha;	CommitColor(); }

	void SetMaskColor(const D3DXCOLOR& color)	{ _MaskColor = color;	CommitMaskColor(); }
	void SetMaskRed(float red)					{ _MaskColor.r = red;	CommitMaskColor(); }
	void SetMaskBlue(float blue)				{ _MaskColor.b = blue;	CommitMaskColor(); }
	void SetMaskGreen(float green)				{ _MaskColor.g = green;	CommitMaskColor(); }
	void SetMaskAlpha(float alpha)				{ _MaskColor.a = alpha;	CommitMaskColor(); }

	void SetCharRot(float rot);
	void SetCharRot(float rot,int index);

	void SetMask(LPDIRECT3DTEXTURE9 mask);
	void SetPass(short pass);

	void AddPos(const D3DXVECTOR2& pos){ _Pos += pos;_CommitMatrix = true; }
	void AddPosX(float x){ _Pos.x += x; _CommitMatrix = true; }
	void AddPosY(float y){ _Pos.x += y; _CommitMatrix = true; }
	void AddPosZ(float z){ _Pos.x += z; _CommitMatrix = true; }

	void AddRot(float rot){ _RotZ += rot; _CommitMatrix = true; }

	void AddColor(const D3DXCOLOR& color)	{ _Color += color;		CommitColor(); }
	void AddRed(float red)					{ _Color.r += red;		CommitColor(); }
	void AddBlue(float blue)				{ _Color.b += blue;		CommitColor(); }
	void AddGreen(float green)				{ _Color.g += green;	CommitColor(); }
	void AddAlpha(float alpha)				{ _Color.a += alpha;	CommitColor(); }

	void AddMaskColor(const D3DXCOLOR& color)	{ _MaskColor += color;		CommitMaskColor(); }
	void AddMaskRed(float red)					{ _MaskColor.r += red;		CommitMaskColor(); }
	void AddMaskBlue(float blue)				{ _MaskColor.b += blue;		CommitMaskColor(); }
	void AddMaskGreen(float green)				{ _MaskColor.g += green;	CommitMaskColor(); }
	void AddMaskAlpha(float alpha)				{ _MaskColor.a += alpha;	CommitMaskColor(); }

	void AddCharRot(float rot);
	void AddCharRot(float rot,int index);

	D3DXVECTOR2 Pos(void)const{ return _Pos; }
	float Rot(void)const{ return _RotZ; }
	D3DXCOLOR Color(void)const{ return _Color; }
	D3DXCOLOR MaskColor(void)const{ return _MaskColor; }
	float Length(void)const{ return _Length; }


protected:

	virtual void Init(void);
	virtual void Update(void);
	

	static D3DXMATRIX Identity;
	static D3DXMATRIX SclMtx;
	D3DXMATRIX RotMtx;
	D3DXMATRIX TransMtx;


	D3DXVECTOR2 _Pos;
	float _RotZ;
	D3DXCOLOR _Color;
	D3DXCOLOR _MaskColor;
	float* CharRotZ;
	bool _CommitMatrix;

	void CastMatrix(void);
	void CommitColor(void);
	void CommitMaskColor(void);

	static bool CheckHarfHeight(char* str);
	static bool CheckHarfSize(char* str);

private:

	void LinkList(void);
	void UnlinkList(void);

	static String* Top;
	static String* Cur;
	String* Next;
	String* Prev;

	Font** Char;
	int CharNum;

	float _Length;
	
};
#endif