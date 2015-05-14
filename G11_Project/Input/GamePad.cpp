#include "GamePad.h"
#include "GPDInput.h"
#include "GPXInput.h"

GamePad* GamePad::Self = nullptr;

GamePad* GamePad::Initialize(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd)
{
	Self = new GPXInput;
	if (!Self->Init(DInput,hInstance,hWnd))
	{
		Self->Uninit();
		Self = new GPDInput;
		if (!Self->Init(DInput,hInstance,hWnd))
		{
			Self->Uninit();
			Self = nullptr;
			//MessageBox(nullptr,"ゲームパッドが接続されていません","INFO",MB_OK | MB_ICONINFORMATION);
		}
	}
	return Self;
}

void GamePad::Uninit(void)
{
	if (Self != nullptr)
	{
		delete Self ;
		Self = nullptr;
	}
}