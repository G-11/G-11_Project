//=============================================================================
// マネージャークラス
// Createby Arai Yuhki
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include<process.h>

#include "Input\VC.h"

#include "Manager.h"
#include "Light.h"
#include "Camera.h"
#include "Camera2D.h"
#include "Renderer.h"
#include "Polygon.h"
#include "Orbit2D.h"
#include "Sprite.h"
#include "Font.h"
#include "Shaim3DEffect.h"
#include "ShimmerParticle2D.h"
#include "Mutex.h"
#include "Loading.h"
//#include "Model.h"

#include "Scene.h"
//=============================================================================
// グローバル変数
//=============================================================================
CRenderer* CManager::Render = nullptr;
CSound*		CManager::Sound = nullptr;
DebugProc* CManager::Debug = nullptr;
#ifdef _DEBUG
bool ScreenShotMode = false;

bool GetScreenShotMode(void);
#endif

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	Render = nullptr;
	Sound = nullptr;
}
//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{

	Render = new CRenderer;
	if (FAILED(Render->Init(hWnd,bWindow)))
	{
		return E_FAIL;
	}
	if (FAILED(VC::Instance()->Init(hWnd)))
	{
		return E_FAIL;
	}
	if(Render == nullptr)
	{
		return E_FAIL;
	}


	Debug = new DebugProc;
	Debug->Init();
	Loading::Instance()->Init();

	Loading::Instance()->Update();
	
	Scene = nullptr;

	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,Thread,InitTexture,0,NULL);
	DWORD code;
	while (true)
	{
		Loading::Instance()->Update();
		GetExitCodeThread(handle,&code);
		if (code != STILL_ACTIVE)
		{
			CloseHandle(handle);
			break;
		}
	}

	handle = (HANDLE)_beginthreadex(NULL,0,SceneInit,this,0,NULL);
	while (true)
	{
		Loading::Instance()->Update();
		GetExitCodeThread(handle,&code);
		if (code != STILL_ACTIVE)
		{
			CloseHandle(handle);
			break;
		}
	}

	//CModel::Init();
	
	CRenderer::SetFade(60,CFade::FADE_IN,D3DXCOLOR(0,0,0,0));
	Current = Next = SCENE_WEAPON_SELECT;
	SceneChangeFlag = false;
	
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	if (Debug != nullptr)
	{
		Debug->Uninit();
		delete Debug;
		Debug = nullptr;
	}
	if (Scene != nullptr)
	{
		Scene->Uninit();
		delete Scene;
		Scene = nullptr;
	}
	ReleaseObject();
	CCamera::ReleaseAll();
	CCamera2D::ReleaseAll();
	Light::ReleaseAll();
	UninitTexture();
	
	Loading::Finalize();
	Mutex::Finalize();
	VC::Instance()->Uninit();
	//CModel::ReleaseAll();
	//CModel::Uninit();
	if(Render != nullptr)
	{
		Render->Uninit();
		delete Render;
		Render = nullptr;
	}

#ifdef _DEBUG

#endif
}
//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	VC::Instance()->Update();
	CCamera::UpdateAll();
	CCamera2D::UpdateAll();
	if (Scene!=nullptr)
	{
		Scene->Update();
	}
	Render->Update();


	ChangeScene();
	
	
}
//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{

	Render->Draw();

}

void CManager::Control(void)
{

}

void CManager::ChangeScene(void)
{
	CFade* Fade = CFade::Instance();
	if (SceneChangeFlag && Fade->Active()==false)
	{
		if (Scene != nullptr)
		{
			Scene->Uninit();
			delete Scene;
			Scene = nullptr;
		}

		/*
		switch (Next)
		{

		default:
			break;
		}
		*/

		if (Scene != nullptr)
		{
			HANDLE handle = (HANDLE)_beginthreadex(NULL,0,SceneInit,this,0,NULL);
			DWORD code;
			while (true)
			{
				Loading::Instance()->Update();
				GetExitCodeThread(handle,&code);
				if (code != STILL_ACTIVE)
				{
					CloseHandle(handle);
					break;
				}
			}
			Fade->Start(30,CFade::FADE_IN,BLACK(0.0f));
			SceneChangeFlag = false;
		}
	}
}

void CManager::SetScene(SCENE scene)
{

	Next = scene;
	CFade::Instance()->Start(30,CFade::FADE_OUT,BLACK(1.0f));
	SceneChangeFlag = true;

}
void CManager::ReleaseObject(void)
{
	String::ReleaseAll();
	Sprite::ReleaseAll();
	CPolygon::ReleaseAll();
	Shaim3DEffect::ReleaseAll();
	ShimmerParticle2D::ReleaseAll();
}

unsigned __stdcall CManager::Thread(void* func)
{
	Function function = (Function)func;
	function();

	_endthreadex(0);

	return 0;
}

unsigned __stdcall CManager::SceneInit(void* func)
{
	CManager* manager = (CManager*)func;
	if (manager->Scene != nullptr)
	{
		manager->Scene->Init();
	}
	_endthreadex(0);

	return 0;
}

void CManager::SetPause(bool flag)
{
	Sprite::SetPause(flag);
	CPolygon::SetPause(flag);
	Shaim3DEffect::SetPause(flag);
	Orbit2D::SetPause(flag);
	ShimmerParticle2D::SetPause(flag);
}
#ifdef _DEBUG
bool GetScreenShotMode(void)
{
	return ScreenShotMode;
}
#endif