#pragma once
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"

//FVF‚Ì‘ã‚í‚è
typedef enum
{
	VTX_DECL_3D = 0,
	VTX_DECL_2D,
	VTX_DECL_POINT_SPRITE,
	VTX_DECL_CUSTOM,
	VD_MAX
}VTX_DECL;

class Manager;

class Window
{
public:

	int Initialize(HINSTANCE hInstance,int cmdShow);
	static void Finalize(void);

	int Run(void);

	void Show(void);

	void InitVD(void);

	void SetVtxDecl(VTX_DECL type){ _Device->SetVertexDeclaration(VertexDeclaration[type]); }
	void SetDevice(LPDIRECT3DDEVICE9 device){ _Device = device; }

	LPDIRECT3DDEVICE9 Device(void){return _Device; }
	HWND hWnd(void)const{ return _hWnd; }
	HINSTANCE hInstance(void)const{ return _hInstance; }
	int CmdShow(void)const{ return _CmdShow; }
	D3DXVECTOR2 WindowSize(void)const{ return Size; }
	Manager* manager(void){return _Manager; }
	static Window* Instance(void){ return Self; }

private:

	Window(){};
	Window(const Window& other){}
	~Window(){};
	Window &operator= (const Window& other){};


	static Window* Self;

	DWORD ExecLastTime,FPSLastTime,CurrentTime,FrameCount;
	int FPS;
	HWND _hWnd;
	HINSTANCE _hInstance;
	int _CmdShow;
	D3DXVECTOR2 Size;
	bool FullScreen;
	UINT FPSLimit;
	Manager* _Manager;
	WNDCLASSEX  wcex;
	LPDIRECT3DVERTEXDECLARATION9 VertexDeclaration[VD_MAX];
	LPDIRECT3DDEVICE9 _Device;
	const static char* ClassName;
	const static char* WindowName;

	static LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static LRESULT CALLBACK InitializeDialog(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	
	void Uninit(void);


};


#endif