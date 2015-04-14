//=============================================================================
// レンダラークラス
//Autor: Arai Yuhki
//=============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_
//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
#include "Fade.h"
//=============================================================================
//マクロ定義
//=============================================================================
#define Object_MAX (10)
#define THREE_D_AREA_WIDTH (795.0f)
#define THREE_D_AREA_HEIGHT (875.0f)
//=============================================================================
// クラス
//=============================================================================
class CObject;
class ShaderManager;
class ScreenRender;
class Blur;
class CRenderer
{
public:
	CRenderer();
	virtual ~CRenderer();
	HRESULT Init(HWND hWnd,BOOL bWindow);
	

	void Uninit(void);
	void Update(void);
	void Draw(void);
	static  LPDIRECT3DDEVICE9 GetDevice(void){return Device;}
	static bool SetFade(float frame,CFade::FADE mode,D3DXCOLOR color = D3DXCOLOR(0,0,0,1.0f));
	static void SetStream2D(void);
	static void SetStream3D(void);
	static void SetRenderMode(short num);
	CFade* GetFade(void){return Fade;}
private:
#ifdef _DEBUG
	void DrawFPS(void);
#endif

	void Init3D(void);
	void Init2D(void);

	static LPDIRECT3D9				g_D3D;
	static LPDIRECT3DDEVICE9		Device;
	static ShaderManager* _ShaderManager;
	static LPDIRECT3DVERTEXBUFFER9 VtxBuff[2];
	static CFade* Fade;
	static ScreenRender* Screen;
	Blur* Blur;
	LPDIRECT3DTEXTURE9 Texture[2];
	LPDIRECT3DSURFACE9 Surface[2];

	void Render3D(void);
	void Render2D(void);

	void Draw3DScreen(LPDIRECT3DTEXTURE9 tex);
	D3DXMATRIX WorldMtx;
	LPDIRECT3DTEXTURE9 Texture3D;
	LPDIRECT3DSURFACE9 Surface3D;

	LPDIRECT3DTEXTURE9 Texture2D;
	LPDIRECT3DSURFACE9 Surface2D;
};
inline DWORD FtoDW(FLOAT f) { return *((DWORD*)&f); }

#endif