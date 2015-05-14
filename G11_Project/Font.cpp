//=============================================================================
//文字クラス
//=============================================================================
#include "Font.h"
#include "Mutex.h"
#include <windows.h>
#include <tchar.h>
#include <cstdlib>

const char* FaceName = "小塚明朝 Pro B\0";

FONT_DATA Font::FontData;
LOGFONT Font::FastFont = { 260,0,0,0,500,0,0,0,SHIFTJIS_CHARSET,OUT_TT_ONLY_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_MODERN,_T("ＭＳ Ｐ明朝") };
//=============================================================================
//文字クラス
//=============================================================================
//整数で保管する色構造体
//=============================================================================
struct SColor {
	unsigned char A,R,G,B;
	unsigned color;
	SColor(unsigned color) : color(color),B(color & 0xff),G((color >> 8) & 0xff),R((color >> 16) & 0xff),A((color >> 24) & 0xff) {};
};

//=============================================================================
//コンストラクタ
//=============================================================================
Font::Font()
{

}
//=============================================================================
//デストラクタ
//=============================================================================
Font::~Font()
{
	if (Texture != nullptr)
	{
		Texture->Release();
		Texture = nullptr;
	}
}
//=============================================================================
//フォントデータの初期化
//=============================================================================
void Font::Initialize(void)
{
	strcpy(FontData.faceName,"小塚明朝 Pro B");
	FontData.charSet = SHIFTJIS_CHARSET;
	FontData.fontHeight = 450;
	FontData.weight = 0;
	FontData.penSize = 5;
	FontData.edgeColor = D3DXCOLOR(0,0,0,1.0f);
	FontData.fillColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	FontData.quality = 3;
}
//=============================================================================
//生成
//=============================================================================
Font* Font::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,char* str)
{
	Font* font = new Font;
	if (font == nullptr) { return nullptr; }

	font->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	font->_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	font->_Size = D3DXVECTOR3(size.x,size.y,1.0f);

	font->Texture = createOutlineFontTexture(Window::Instance()->Device(),str);

	return font;
}
//=============================================================================
// 高速フォント生成
//=============================================================================
Font* Font::CreateFast(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,char* str)
{
	Font* font = new Font;
	if (font == nullptr) { return nullptr; }

	font->Texture = createFastFontTexture(Window::Instance()->Device(),font,_T(str));
	font->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	font->_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	//Font->_Size += D3DXVECTOR3(size.x,size.y,1.0f);

	return font;
}
//=============================================================================
//更新
//=============================================================================
void Font::Update(void)
{

}
//=============================================================================
//文字テクスチャを作成
//=============================================================================
LPDIRECT3DTEXTURE9 Font::createOutlineFontTexture(IDirect3DDevice9 *dev,const char *str,RECT *info)
{
	struct Color {
		unsigned char A,R,G,B;
		unsigned color;
		Color(unsigned color) : color(color),B(color & 0xff),G((color >> 8) & 0xff),R((color >> 16) & 0xff),A((color >> 24) & 0xff) {}
	};
	Color cEdgeColor(FontData.edgeColor),cFillColor(FontData.fillColor);

	int penSize = FontData.penSize;
	penSize *= FontData.quality;
	IDirect3DTexture9 *tex = 0;

	// デバイスコンテキスト作成
	HDC hDC = GetDC(NULL);
	HDC memDC = CreateCompatibleDC(hDC);
	ReleaseDC(NULL,hDC);   // 解放していいよ

	// フォント作成
	LOGFONTA lf = {};
	lf.lfHeight = FontData.fontHeight * FontData.quality;    // 拡大サイズで
	lf.lfCharSet = FontData.charSet;
	lf.lfWeight = FontData.weight;
	lf.lfQuality = ANTIALIASED_QUALITY;
	memcpy(lf.lfFaceName,FontData.faceName,strlen(FontData.faceName));
	HFONT hFont = CreateFontIndirectA(&lf);
	HFONT oldFont = (HFONT)SelectObject(memDC,hFont);     // フォントを設定しないとエラーになります

	// BMPを作成
	TEXTMETRICA tm;
	GLYPHMETRICS gm;
	MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	int len = IsDBCSLeadByte(str[0]) ? 2 : 1;
	UINT code = (len == 2 ? (unsigned char)str[0] << 8 | (unsigned char)str[1] : (unsigned char)str[0]);
	GetTextMetricsA(memDC,&tm);
	GetGlyphOutlineA(memDC,code,GGO_METRICS,&gm,0,0,&mat);

	RECT charRegion = {     // LT - RB
		gm.gmptGlyphOrigin.x - penSize / 2,
		tm.tmAscent - gm.gmptGlyphOrigin.y - penSize / 2,
		gm.gmptGlyphOrigin.x + gm.gmBlackBoxX + penSize / 2,
		tm.tmAscent - gm.gmptGlyphOrigin.y + gm.gmBlackBoxY + penSize / 2
	};

	RECT charWH = { 0,0,(gm.gmBlackBoxX + penSize + FontData.quality - 1) / FontData.quality * FontData.quality,(gm.gmBlackBoxY + penSize + FontData.quality - 1) / FontData.quality * FontData.quality };
	int bmpW = charWH.right;
	int bmpH = charWH.bottom;
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = bmpW;
	bmpInfo.bmiHeader.biHeight = -bmpH;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	unsigned char *p = 0;
	HBITMAP hBitMap = CreateDIBSection(0,&bmpInfo,DIB_RGB_COLORS,(void**)&p,0,0);
	HBITMAP oldBMP = (HBITMAP)SelectObject(memDC,hBitMap);

	// BMP背景を青色で初期化
	HBRUSH bgBrush = (HBRUSH)CreateSolidBrush(RGB(0,0,255));
	FillRect(memDC,&charWH,bgBrush);
	DeleteObject(bgBrush);

	// パスを描く
	// パス色は緑、塗は赤
	HPEN   hPen = (HPEN)CreatePen(PS_SOLID,penSize,RGB(0,255,0));
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255,0,0));
	HPEN   oldPen = (HPEN)SelectObject(memDC,hPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC,hBrush);

	SetBkMode(memDC,TRANSPARENT);
	BeginPath(memDC);
	TextOutA(memDC,-charRegion.left,-charRegion.top,str,len);
	EndPath(memDC);
	StrokeAndFillPath(memDC);

	SelectObject(memDC,oldPen);
	SelectObject(memDC,oldBrush);
	SelectObject(memDC,oldFont);
	SelectObject(memDC,oldBMP);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(hFont);

	// DirectXのテクスチャにBMPの色分けを使いフォントを穿つ
	int texW = charWH.right / FontData.quality;
	int texH = charWH.bottom / FontData.quality;
	int q2 = FontData.quality * FontData.quality;
	
	Mutex::Instance()->Enter();
	dev->CreateTexture(texW,texH,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&tex,0);
	Mutex::Instance()->Leave();
	
	D3DLOCKED_RECT lockR;
	if (SUCCEEDED(tex->LockRect(0,&lockR,0,0))) {
		char *d = (char*)lockR.pBits;
		unsigned BMPPitch = (charWH.right * 3 + 3) / 4 * 4;
		for (int y = 0; y < texH; y++) {
			for (int x = 0; x < texW; x++) {
				unsigned &v = *((unsigned*)d + x + y * texW);   // テクスチャのピクセル位置
				unsigned alph = 0;
				unsigned edge = 0;
				unsigned fill = 0;
				// quality倍率分点を平均化
				for (int i = 0; i < FontData.quality; i++) {
					for (int j = 0; j < FontData.quality; j++) {
						alph += p[(y * FontData.quality + i) * BMPPitch + (x * FontData.quality + j) * 3 + 0];
						edge += p[(y * FontData.quality + i) * BMPPitch + (x * FontData.quality + j) * 3 + 1];
						fill += p[(y * FontData.quality + i) * BMPPitch + (x * FontData.quality + j) * 3 + 2];
					}
				}
				alph /= q2;
				edge /= q2;
				fill /= q2;

				// 透過度がある場合はエッジ色を採用
				// 不透明の場合はブレンド色を採用
				unsigned a = 0xff - alph;
				if (a < 0xff) {
					// 半透明
					unsigned r = cEdgeColor.R;
					unsigned g = cEdgeColor.G;
					unsigned b = cEdgeColor.B;
					a = (a * cEdgeColor.A) >> 8;
					v = a << 24 | r << 16 | g << 8 | b;
				}
				else {
					// 不透明
					unsigned r = ((cEdgeColor.R * edge) >> 8) + ((cFillColor.R * fill) >> 8);
					unsigned g = ((cEdgeColor.G * edge) >> 8) + ((cFillColor.G * fill) >> 8);
					unsigned b = ((cEdgeColor.B * edge) >> 8) + ((cFillColor.B * fill) >> 8);
					a = ((cEdgeColor.A * edge) >> 8) + ((cFillColor.A * fill) >> 8);
					v = a << 24 | r << 16 | g << 8 | b;
				}
			}
		}
		tex->UnlockRect(0);
	}

	DeleteObject(hBitMap);
	DeleteDC(memDC);

	if (info) {
		info->left = charRegion.left / FontData.quality;
		info->top = charRegion.top / FontData.quality;
		info->right = (gm.gmCellIncX + penSize) / FontData.quality;
		info->bottom = (tm.tmHeight + penSize) / FontData.quality;
	}
	return tex;
}

LPDIRECT3DTEXTURE9 Font::createFastFontTexture(IDirect3DDevice9 *dev,Font* instance,const char *str)
{
	IDirect3DTexture9 *tex = 0;

	HFONT hFont = CreateFontIndirect(&FastFont);
	if (hFont == nullptr)
	{
		MessageBox(nullptr,"高速フォントのハンドルの生成に失敗しました","ERROR!",MB_OK | MB_ICONERROR);
		return nullptr; 
	}

	//デバイスにフォントを持たせる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc,hFont);

	wchar_t c[32] = L"test";

	//フォントビットマップを取得
	int len = MultiByteToWideChar(CP_ACP,0,str,-1,NULL,0);
	MultiByteToWideChar(CP_ACP,0,str,-1,c,len);

	UINT code = c[0];

	
	const int gradFlag = GGO_GRAY8_BITMAP;//GRAY2とGRAY8もある

	int grad = 0;//階調の最大値

	switch (gradFlag)
	{
	case GGO_GRAY2_BITMAP:
		grad = 4;
		break;
	case GGO_GRAY4_BITMAP:
		grad = 16;
		break;
	case GGO_GRAY8_BITMAP:
		grad = 64;
		break;
	default:
		break;
	}
	if (grad == 0)
	{
		MessageBox(nullptr,"高速フォントの階調がイレギュラーです","ERROR!",MB_OK|MB_ICONERROR);
		return nullptr;
	}

	TEXTMETRIC textMetric;
	GetTextMetrics(hdc,&textMetric);
	GLYPHMETRICS glyphMetric;
	CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{0,1} };
	DWORD size = GetGlyphOutlineW(hdc,code,gradFlag,&glyphMetric,0,NULL,&mat);
	BYTE *pMono = new BYTE[size];
	GetGlyphOutlineW(hdc,code,gradFlag,&glyphMetric,size,pMono,&mat);

	//デバイスコンテキストとフォントハンドルを開放
	SelectObject(hdc,oldFont);
	ReleaseDC(NULL,hdc);

	//テクスチャの作成
	D3DLOCKED_RECT lockedRect;

	int width = (glyphMetric.gmBlackBoxX + 3) / 4 * 4;
	int height = glyphMetric.gmBlackBoxY;

	Mutex::Instance()->Enter();
	dev->CreateTexture((UINT)width,(UINT)height,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&tex,NULL);
	Mutex::Instance()->Leave();

	tex->LockRect(0,&lockedRect,NULL,0);//ロック
	DWORD *TexBuffer = (DWORD*)lockedRect.pBits;//テクスチャメモリへのポインタを取得

	

	for(int y = 0;y < height;y++)
	{
		for (int x = 0;x < width;x++)
		{
			DWORD alpha = pMono[y*width + x]*255/grad;
			TexBuffer[y*width + x] = (alpha << 24) | 0x00ffffff;
		}
	}

	tex->UnlockRect(0);//アンロック
	delete[] pMono;

	instance->SetSize(D3DXVECTOR3((float)width,(float)height,1.0f));
	instance->SetOffsetPos(D3DXVECTOR2((float)glyphMetric.gmptGlyphOrigin.x,(float)-glyphMetric.gmptGlyphOrigin.y));

	return tex;
}
//=============================================================================
//フォントを変更
//=============================================================================
void Font::SetFaceName(char faceName[])
{
	strcpy(FontData.faceName,faceName);
}

void Font::SetFastFontFacename(char name[])
{
	strcpy(FastFont.lfFaceName,name);

}

//====================================================
//Stringクラス
//====================================================
String* String::Top = nullptr;
String* String::Cur = nullptr;
D3DXMATRIX String::SclMtx;
D3DXMATRIX String::Identity;

const char* HarfHeightChar[] = 
{
	"～",
	"ー",
	"＝",
};
const char* HarfSizeChar[] =
{
	"ぁ","ぃ","ぅ","ぇ","ぉ",
	"ゃ","ゅ","ょ","ゎ","っ",
	"ァ","ィ","ゥ","ェ","ォ",
	"ャ","ュ","ョ","ヮ","ッ","・",
};
String::String()
{

	Char = nullptr;
	_CommitMatrix = false;
	CharRotZ = nullptr;
	LinkList();
}

String::~String()
{
	UnlinkList();
}

//=============================================================================
//自身をリストに追加
//=============================================================================
void String::LinkList(void)
{
	if (Top != NULL)//二つ目以降の処理
	{
		String* Polygon = Cur;
		Polygon->Next = this;
		Prev = Polygon;
		Next = NULL;
		Cur = this;
	}
	else//最初の一つの時の処理
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
	
}
//=============================================================================
//自身をリストから削除
//=============================================================================
void String::UnlinkList(void)
{
	if (Prev == NULL)//先頭
	{
		if (Next != NULL)//次がある
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if (Next == NULL)//終端
	{
		if (Prev != NULL)//前がある
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;

}

//=============================================================================
//初期化
//=============================================================================
void String::Initialize(void)
{
	D3DXMatrixIdentity(&Identity);
	D3DXMatrixScaling(&SclMtx,1.0f,1.0f,1.0f);
	Font::Initialize();
}

//=============================================================================
//生成
//=============================================================================
String* String::Create(const D3DXVECTOR2& pos,float size,char* str)
{
	String* string = new String;
	if (string == nullptr){ return nullptr; }

	int num = strlen(str);
	char String[256] = { '\0' };
	strcpy(String,str);
	float offset = 0;
	for (int cnt = 0;cnt < num;)
	{//何文字あるか数える＆全体の長さを決める
		int len = IsDBCSLeadByte(str[cnt]) ? 2 : 1;
		if ((len == 1 && islower((int)(str[cnt]))) || CheckHarfSize(&str[cnt]))
		{
			offset += size / 1.5f;
		}
		else
		{
			offset += size;
		}
		string->CharNum++;
		cnt += len;
	}
	string->Char = new Font*[string->CharNum];
	string->_Pos = pos;
	string->_RotZ = 0;
	string->_Color = 
	string->_MaskColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	string->_Length = offset;

	float charSize = size;//文字同士が触れないように少し大きめのオフセット値に設定する

	float offsetX = -(offset/2.5f);//全長の半分のマイナス値を最初の文字のオフセットに設定

	int len = 0;
	for (int cnt = 0,charNum = 0;charNum < num && cnt < string->CharNum;cnt++)
	{
		//文字を作成
		len = IsDBCSLeadByte(String[charNum]) ? 2 : 1;//2バイト文字？
		if (len == 1 && islower((int)(String[charNum])))
		{//半角英数で小文字だった時
			if (cnt != 0){ offsetX += charSize/1.5f; }

			string->Char[cnt] = Font::Create(D3DXVECTOR2(pos.x + offsetX,pos.y + size / 4.0f),D3DXVECTOR2(size / 2.0f,size / 2.0f),&String[charNum]);
			string->Char[cnt]->SetOffsetPos(D3DXVECTOR2(offsetX,size / 4.0f));
		}
		else if (CheckHarfSize(&String[charNum]))
		{
			if (cnt != 0){ offsetX += charSize / 1.5f; }
			string->Char[cnt] = Font::Create(D3DXVECTOR2(pos.x + offsetX,pos.y + size / 4.0f),D3DXVECTOR2(size / 2.0f,size / 2.0f),&String[charNum]);
			string->Char[cnt]->SetOffsetPos(D3DXVECTOR2(offsetX,size / 4.0f));
		}
		else if (CheckHarfHeight(&String[charNum]))
		{//高さを半分にする必要のある文字の場合
			if (cnt != 0){ offsetX += charSize; }
			string->Char[cnt] = Font::Create(D3DXVECTOR2(pos.x + offsetX,pos.y),D3DXVECTOR2(size,size / 2.0f),&String[charNum]);
			string->Char[cnt]->SetOffsetPos(D3DXVECTOR2(offsetX,0));
		}
		else
		{
			if (cnt != 0){ offsetX += charSize; }
			string->Char[cnt] = Font::Create(D3DXVECTOR2(pos.x + offsetX,pos.y),D3DXVECTOR2(size,size),&String[charNum]);
			string->Char[cnt]->SetOffsetPos(D3DXVECTOR2(offsetX,0));
		}
		charNum += len;
	}
	string->Init();
	return string;

}

String* String::CreateFast(const D3DXVECTOR2& pos,float size,char* str)
{
	String* string = new String;
	if (string == nullptr){ return nullptr; }

	int num = strlen(str);
	char String[256] = { '\0' };
	strcpy(String,str);
	
	for (int cnt = 0;cnt < num;)
	{//何文字あるか数える
		int len = IsDBCSLeadByte(str[cnt]) ? 2 : 1;
		string->CharNum++;
		cnt += len;
	}
	string->Char = new Font*[string->CharNum];
	string->_Pos = pos;
	string->_RotZ = 0;
	string->_Color =
	string->_MaskColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	Font::SetFastFontSize((LONG)size);

	float charSize = size;//文字同士が触れないように少し大きめのオフセット値に設定する

	float offset = 0;
	int len = 0;
	for (int cnt = 0,charNum = 0;charNum < num && cnt < string->CharNum;cnt++)
	{
		//文字を作成
		len = IsDBCSLeadByte(String[charNum]) ? 2 : 1;//2バイト文字？
		string->Char[cnt] = Font::CreateFast(D3DXVECTOR2(pos.x,pos.y),D3DXVECTOR2(size,size),&String[charNum]);
		float Csize = string->Char[cnt]->Size().x;
		offset += (Csize<charSize / 2.0f) ? charSize / 2.0f : Csize;
		string->Char[cnt]->AddPosY(string->Char[cnt]->OffsetPos().y);
		
		charNum += len;
	}
	string->_Length = offset;
	float offsetX = -(offset*0.5f);
	for (int cnt = 0,charNum = 0;charNum < num && cnt < string->CharNum;cnt++)
	{
		string->Char[cnt]->AddPosX(offsetX);
		string->Char[cnt]->AddOffsetPos(D3DXVECTOR2(offsetX,size/2.0f));
		float Csize = string->Char[cnt]->Size().x;
		offsetX += (Csize<charSize / 2.0f) ? charSize / 2.0f : Csize;
		charNum += len;
	}
	string->Init();
	return string;

}
//=============================================================================
//個別初期化
//=============================================================================
void String::Init(void)
{
	//それぞれの文字の回転を保存させるための変数を用意
	CharRotZ = new float[CharNum];
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		CharRotZ[cnt] = 0;
	}
}
//=============================================================================
//すべて更新
//=============================================================================
void String::UpdateAll(void)
{
	String* string = Top;
	String* next = nullptr;

	while (string)
	{
		next = string->Next;

		string->Update();

		string = next;
	}
}
//=============================================================================
//すべてリリース
//=============================================================================
void String::ReleaseAll(void)
{
	String* string = Top;
	String* next = nullptr;

	while (string)
	{
		next = string->Next;

		string->Release();

		string = next;
	}
}

//=============================================================================
//更新
//=============================================================================
void String::Update(void)
{
	CastMatrix();
}

//=============================================================================
//文字の位置を再計算
//=============================================================================
void String::CastMatrix(void)
{//文字列を移動したり回転させた場合、位置を再計算する
	if (_CommitMatrix)
	{
		D3DXMATRIX WorldMatrix = Identity;
		D3DXMatrixRotationZ(&RotMtx,_RotZ);
		D3DXMatrixTranslation(&TransMtx,_Pos.x,_Pos.y,0);

		D3DXMatrixMultiply(&WorldMatrix,&WorldMatrix,&SclMtx);
		D3DXMatrixMultiply(&WorldMatrix,&WorldMatrix,&RotMtx);
		D3DXMatrixMultiply(&WorldMatrix,&WorldMatrix,&TransMtx);

		D3DXVECTOR3 pos;

		for (int cnt = 0;cnt < CharNum;cnt++)
		{
			pos = D3DXVECTOR3(Char[cnt]->OffsetPos().x,Char[cnt]->OffsetPos().y,0);
			D3DXVec3TransformCoord(&pos,&pos,&WorldMatrix);
			Char[cnt]->SetPos(pos);
			Char[cnt]->SetRot(_RotZ+CharRotZ[cnt]);
		}

		_CommitMatrix = false;

	}
}
//=============================================================================
//色を変更
//=============================================================================
void String::CommitColor(void)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetColor(_Color);
	}
}

//=============================================================================
//色を変更
//=============================================================================
void String::CommitMaskColor(void)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetMaskColor(_MaskColor);
	}
}
//=============================================================================
//高さを変えるべきか判断
//=============================================================================
bool String::CheckHarfHeight(char* str)
{
	int num = sizeof(HarfHeightChar) / 4;
	for (int cnt = 0;cnt < num;cnt++)
	{
		if (HarfHeightChar[cnt][0] == str[0] && HarfHeightChar[cnt][1] == str[1])
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
//サイズを変えるべきか判断
//=============================================================================
bool String::CheckHarfSize(char* str)
{
	int num = sizeof(HarfSizeChar) / 4;
	for (int cnt = 0;cnt < num;cnt++)
	{
		if (HarfSizeChar[cnt][0] == str[0] && HarfSizeChar[cnt][1] == str[1])
		{
			return true;
		}
	}
	return false;
}
//=============================================================================
//開放
//=============================================================================
void String::Release(void)
{
	if (Char != nullptr)
	{
		for (int cnt = 0;cnt < CharNum;cnt++)
		{
			if (Char[cnt] != nullptr)
			{
				Char[cnt]->SetRelease();
				Char[cnt] = nullptr;
			}
		}
		delete[] Char;
	}

	if (CharRotZ != nullptr)
	{
		delete[] CharRotZ;
		CharRotZ = nullptr;
	}

	delete this;
}
//=============================================================================
//一文字ごとの回転をセット
//=============================================================================
void String::SetCharRot(float rot)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetRot(rot);
		CharRotZ[cnt] = rot;
	}
}//=============================================================================
//一文字の回転をセット
//=============================================================================
void String::SetCharRot(float rot,int index)
{
	if (index >= 0 && index < CharNum)
	{
		Char[index]->SetRot(rot);
		CharRotZ[index] = rot;
	}
}
//=============================================================================
//一文字ごとの回転を加算
//=============================================================================
void String::AddCharRot(float rot)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->AddRot(rot);
		CharRotZ[cnt] += rot;
	}
}
//=============================================================================
//一文字の回転を加算
//=============================================================================
void String::AddCharRot(float rot,int index)
{
	if (index >= 0 && index < CharNum)
	{
		Char[index]->AddRot(rot);
		CharRotZ[index] += rot;
	}
}

void String::SetMask(LPDIRECT3DTEXTURE9 mask)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetMask(mask);
	}
}

void String::SetPass(short pass)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetPass((int)pass);
	}
}