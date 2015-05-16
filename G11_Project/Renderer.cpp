//=============================================================================
// インクルード
//=============================================================================
#include "Renderer.h"
#include "Fade.h"
#include "Camera.h"
#include "Camera2D.h"
#include "Manager.h"
#include "ShaderManager.h"
#include "Shader2D.h"
#include "Shader3D.h"
#include "Shimmer.h"
#include "Light2D.h"

#include "Sprite.h"
#include "Shaim3DEffect.h"
#include "ShimmerParticle2D.h"
#include "LightParticle.h"
#include "Orbit2D.h"
#include "Font.h"

#include "ScreenRender.h"
#include "BlurFilter.h"
#include "Loading.h"
//#include "Model.h"
#include "Light.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3D9				Renderer::g_D3D = NULL;
LPDIRECT3DDEVICE9		Renderer::Device = NULL;
ShaderManager*			Renderer::_ShaderManager = nullptr;
LPDIRECT3DVERTEXBUFFER9 Renderer::VtxBuff[2] = { nullptr };
Fade*					Renderer::Fade = nullptr;
CScreenRender*			Renderer::Screen = nullptr;
//=============================================================================
// コンストラクタ
//=============================================================================
Renderer::Renderer()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
Renderer::~Renderer()
{
	
}
//=============================================================================
// 初期化
//=============================================================================
HRESULT Renderer::Init(HWND hWnd,BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;
	Window* window = Window::Instance();

	// Direct3Dオブジェクトの生成
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_D3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= (UINT)window->WindowSize().x;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= (UINT)window->WindowSize().y;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成

	if(FAILED(g_D3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,D3DFMT_DXT1,D3DFMT_R32F,D3DFMT_D24S8)))
	{
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う
	}
	else
	{
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;				// デプスバッファとして24bitを使う
	}
	

	if(bWindow)
	{// ウィンドウモード
		d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.BackBufferFormat           = D3DFMT_R5G6B5;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT,						// ディスプレイアダプタ
									D3DDEVTYPE_HAL,							// ディスプレイタイプ
									hWnd,									// フォーカスするウインドウへのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// デバイス作成制御の組み合わせ
									&d3dpp,									// デバイスのプレゼンテーションパラメータ
									&Device)))							// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp,
										&Device)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp,
											&Device)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}
	window->SetDevice(Device);
	window->Show();

	// レンダーステートパラメータの設定
	Device->SetRenderState(D3DRS_CLIPPING,TRUE);	//クリッピング有効

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	Device->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	Device->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	
	// サンプラーステートパラメータの設定
	//Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	//Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	//Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	//Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数


	window->InitVD();
	Camera3D* camera = Camera3D::Create(D3DXVECTOR3(0,300.0f,-500.0f),D3DXVECTOR3(0,0,0));

	Camera2D* camera2D = Camera2D::Create(D3DXVECTOR3(0.0f,0.0f,0));

	if (FAILED(Device->CreateVertexBuffer(sizeof(CUSTOM_VTX)* 4 * 2,D3DUSAGE_WRITEONLY,FVF_CUSTOM,D3DPOOL_MANAGED,&VtxBuff[0],NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_2D)* 4 * 2,D3DUSAGE_WRITEONLY,FVF_VERTEX_2D,D3DPOOL_MANAGED,&VtxBuff[1],NULL)))
	{
		return E_FAIL;
	}

	_ShaderManager = ShaderManager::Instance();
	_ShaderManager->Init();
	
	Init3D();
	Init2D();

	for (int cnt = 0;cnt < SURFACE_NUM;cnt++)
	{
		GetDevice()->CreateTexture((UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Texture[cnt],NULL);
		Texture[cnt]->GetSurfaceLevel(0,&Surface[cnt]);
	}

	Screen = new CScreenRender;
	Screen->Init();
	CLoading::SetRenderer(Screen);

	String::Initialize();

	Fade = Fade::Instance();
	Fade->Init();

	return S_OK;
}

void Renderer::Init2D(void)
{
	Shaim3DEffect::Initialize();
	VERTEX_2D* Vtx2;
	Window* window = Window::Instance();

	VtxBuff[1]->Lock(0,0,(void**)&Vtx2,0);

	Vtx2[0].vtx = D3DXVECTOR3(-0.5f,-0.5f,0);
	Vtx2[1].vtx = D3DXVECTOR3(0.5f,-0.5f,0);
	Vtx2[2].vtx = D3DXVECTOR3(-0.5f,0.5f,0);
	Vtx2[3].vtx = D3DXVECTOR3(0.5f,0.5f,0);

	Vtx2[0].tex = D3DXVECTOR2(0,0);
	Vtx2[1].tex = D3DXVECTOR2(1,0);
	Vtx2[2].tex = D3DXVECTOR2(0,1);
	Vtx2[3].tex = D3DXVECTOR2(1,1);

	VtxBuff[1]->Unlock();

	Device->CreateTexture((UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Texture2D,NULL);
	Texture2D->GetSurfaceLevel(0,&Surface2D);
}

//=============================================================================
//3Dオブジェクトの初期化
//=============================================================================
void Renderer::Init3D(void)
{
	Window* window = Window::Instance();
	D3DXMATRIX Mtx;
	D3DXMatrixIdentity(&WorldMtx);
	D3DXMatrixScaling(&Mtx,SCREEN_WIDTH,SCREEN_HEIGHT,1.0f);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&Mtx);
	D3DXMatrixTranslation(&Mtx,SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&Mtx);

	D3DXVECTOR4 vec = -D3DXVECTOR4(0.15f,-0.8f,0.05f,0.0f);
	D3DXVec4Normalize(&vec,&vec);

	CShader3D::Instance()->SetFloatArray(CShader3D::LIGHT_VEC,vec,4);
	CShader3D::Instance()->SetFloatArray(CShader3D::LIGHT_COLOR,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),4);
	CUSTOM_VTX* Vtx;
	VtxBuff[0]->Lock(0,0,(void**)&Vtx,0);

	Vtx[0].vtx = D3DXVECTOR3(-0.5f,0.5f,0);
	Vtx[1].vtx = D3DXVECTOR3(0.5f,0.5f,0);
	Vtx[2].vtx = D3DXVECTOR3(-0.5f,-0.5f,0);
	Vtx[3].vtx = D3DXVECTOR3(0.5f,-0.5f,0);

	Vtx[0].nor = D3DXVECTOR3(0,0,-1.0f);
	Vtx[1].nor = D3DXVECTOR3(0,0,-1.0f);
	Vtx[2].nor = D3DXVECTOR3(0,0,-1.0f);
	Vtx[3].nor = D3DXVECTOR3(0,0,-1.0f);

	Vtx[0].tex = D3DXVECTOR2(0,0);
	Vtx[1].tex = D3DXVECTOR2(1,0);
	Vtx[2].tex = D3DXVECTOR2(0,1);
	Vtx[3].tex = D3DXVECTOR2(1,1);

	VtxBuff[0]->Unlock();

	GetDevice()->CreateTexture((UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Texture3D,NULL);
	Texture3D->GetSurfaceLevel(0,&Surface3D);
}
//=============================================================================
// 終了処理
//=============================================================================
void Renderer::Uninit(void)
{
	String	::ReleaseAll();
	Sprite	::ReleaseAll();
	Shaim3DEffect::ReleaseAll();
	Orbit2D	::ReleaseAll();
	LightParticle::ReleaseAll();
	//CModel::ReleaseAll();

	Fade::Finalize();
	_ShaderManager->Uninit();
	_ShaderManager = nullptr;
	SafeDelete(Screen);
	SafeRelease(VtxBuff[0]);
	SafeRelease(VtxBuff[1]);
	
	SafeRelease(Surface3D);
	SafeRelease(Surface2D);
	
	for (int cnt = 0;cnt < SURFACE_NUM;cnt++)
	{
		SafeRelease(Surface[cnt]);
		SafeRelease(Texture[cnt]);
	}

	SafeRelease(Texture3D);
	SafeRelease(Texture2D);

	SafeRelease(g_D3D);
	SafeRelease(Device);

}
//=============================================================================
// 更新
//=============================================================================
void Renderer::Update(void)
{
	String	::UpdateAll();
	Sprite	::UpdateAll();
	Shaim3DEffect::UpdateAll();
	ShimmerParticle2D::UpdateAll();//陽炎パーティクルを更新する
	LightParticle::UpdateAll();
	Orbit2D	::UpdateAll();

	Fade->Update();

	//CModel		::UpdateAll();
}
//=============================================================================
// 描画
//=============================================================================
void Renderer::Draw(void)
{
#ifdef _DEBUG
	static float AVtime = 0;
#endif
	LPDIRECT3DSURFACE9 OldSurface = nullptr;

	Device->GetRenderTarget(0,&OldSurface);
	
	if(SUCCEEDED(Device->BeginScene()))	//描画開始
	{
		//カメラをセット
		Camera3D::Set(0);
		Camera2D::Set(0);
		
		RenderAffect();
		
		//レンダリングモードを適用してスクリーンを描画
		Device->SetRenderTarget(0,Surface[1]);

		if (ShimmerParticle2D::Num() > 0)
		{
			LPDIRECT3DTEXTURE9 tex = CShimmer::Instance()->Render(Texture[0]);
			Screen->RenderWindow(tex);
		}
		else
		{
			Screen->RenderWindow(Texture[0]);
		}

		Device->SetRenderTarget(0,Surface[2]);

		if (LightParticle::Num() > 0)
		{
			LPDIRECT3DTEXTURE9 tex = LightScreen::Instance()->Render(Texture[1]);
			Screen->RenderWindow(tex);
		}
		else
		{
			Screen->RenderWindow(Texture[1]);
		}
		Device->SetRenderTarget(0,Surface[3]);
		Screen->RenderScreen(Texture[2]);
		Fade->Draw(1);//3D描画のみのフェード描画

		for (int cnt = Sprite::LAYER_3;cnt < Sprite::LAYER_INTERFACE;cnt++)
		{
			Sprite::DrawAll(cnt);
		}
		//Screen->RenderScreen(Texture2D,2,false);
		
		//レンダリングモードを適用しないものはこっちで描く
		Sprite::DrawAll(Sprite::LAYER_INTERFACE);
		Fade->Draw();

#ifdef _DEBUG
		CDebugProc::Draw();
#endif

		//最後にまとめたものを描画する
		Device->SetRenderTarget(0,OldSurface);

		SafeRelease(OldSurface);

		Screen->RenderWindow(Texture[3]);

		//CDebugProc::DrawFPSGraph();
		Device->EndScene();	//描画終了
	}
	// バックバッファとフロントバッファの入れ替え
	Device->Present(NULL,NULL,NULL,NULL);

}

//=============================================================================
//3Dポリゴンを描く準備
//=============================================================================
void Renderer::SetStream3D(void)
{
	Device->SetStreamSource(0,VtxBuff[0],0,sizeof(CUSTOM_VTX));
	Window::Instance()->SetVtxDecl(VTX_DECL_CUSTOM);
}

//=============================================================================
//2Dポリゴンを描く準備
//=============================================================================
void Renderer::SetStream2D(void)
{
	Device->SetStreamSource(0,VtxBuff[1],0,sizeof(VERTEX_2D));
	Window::Instance()->SetVtxDecl(VTX_DECL_2D);
}

//=============================================================================
//3Dオブジェクトを描画
//=============================================================================
void Renderer::Render3D(void)
{
	Device->SetRenderTarget(0,Surface3D);
	Device->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_RGBA(255,255,255,0),1.0f,0);

}

void Renderer::Draw3DScreen(LPDIRECT3DTEXTURE9 tex)
{
	Device->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_RGBA(255,255,255,0),1.0f,0);
	CShader2D* shader = CShader2D::Instance();
	if (tex != nullptr)
	{
		shader->SetTexture(tex);
	}
	SetStream2D();
	shader->SetMatrix(CShader2D::WORLD_MTX,WorldMtx);
	
	shader->SetFloatArray(CShader2D::DIFFUSE,WHITE(1.0f),4);
	
	shader->DrawBegin();

	shader->Draw(CShader2D::NORMAL,D3DPT_TRIANGLESTRIP);

	shader->DrawEnd();
}
//=============================================================================
//2Dオブジェクトを描画
//=============================================================================
void Renderer::Render2D(void)
{
	Device->SetRenderTarget(0,Surface2D);
	Device->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_RGBA(0,0,0,0),1.0f,0);

	SetStream2D();
	Shaim3DEffect::DrawAll();
	Orbit2D::DrawAll();
	for (int cnt = 0;cnt < Sprite::LAYER_INTERFACE;cnt++)
	{
		Sprite::DrawAll(cnt);
	}

	CShader2D::Instance()->SetFloatArray(CShader2D::UV,D3DXVECTOR4(0,0,1.0f,1.0f),4);
}

void Renderer::RenderAffect(void)
{
	Device->SetRenderTarget(0,Surface[0]);
	Device->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_RGBA(0,0,0,0),1.0f,0);

	SetStream2D();
	Shaim3DEffect::DrawAll();
	Orbit2D::DrawAll();
	for (int cnt = 0;cnt < Sprite::LAYER_3;cnt++)
	{
		Sprite::DrawAll(cnt);
	}

	CShader2D::Instance()->SetFloatArray(CShader2D::UV,D3DXVECTOR4(0,0,1.0f,1.0f),4);
}
//=============================================================================
//フェードを設定
//=============================================================================
bool Renderer::SetFade(float frame,Fade::FADE mode,D3DXCOLOR color)
{
	if (Fade)
	{
		if (Fade->Active() == false)
		{
			Fade->Start(frame,mode,color);
			return true;
		}
	}
	return false;
}
//=============================================================================
//レンダリングモードを変更
//=============================================================================
void Renderer::SetRenderMode(short num)
{
	Screen->SetMode((int)num);
}
//=============================================================================
// FPS描画
//=============================================================================
#ifdef _DEBUG
void Renderer::DrawFPS(void)
{

}
#endif