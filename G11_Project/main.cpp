#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//=============================================================================
//
// 俺の庭 [main.cpp]
// Author : Arai Yuhki
//
//=============================================================================
#include "main.h"
#include "Window.h"
#include "Manager.h"
#include "Input/Keyboard.h"
#include "Renderer.h"
#include<time.h>
#include "resource.h"

//#include "Mouse.h"

#ifdef _DEBUG
	#if CUSTOM_ALLOCAT_ENABLE
	#else
		#ifndef DBG_NEW
			#define DBG_NEW					new(_NORMAL_BLOCK,__FILE__,__LINE__)
			#define new DBG_NEW
		#endif
	#endif
#endif
//#endif
//=============================================================================
// マクロ定義
//=============================================================================


//=============================================================================
// 構造体定義
//=============================================================================

//=============================================================================
// グローバル変数:
//=============================================================================


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(0);
	
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）
	Window* window = Window::Instance();
	if (window->Initialize(hInstance,nCmdShow) == -1)
	{
		MessageBox(nullptr,"デバイスが生成できませんでした","ERROR",MB_OK|MB_ICONERROR);
		return -1;
	}

	int msg = window->Run();

	window->Finalize();

	return msg;
	
}