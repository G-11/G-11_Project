//=============================================================================
// キーボードクラス
// CreateBy Arai Yuhki
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
//=============================================================================
// インクルード
//=============================================================================
#include"VC.h"
#include <Windows.h>
//=============================================================================
// クラス定義
//=============================================================================
class Keyboard
{
public:
	
	~Keyboard();
	HRESULT Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	static void Uninit(void);
	void Update(void);

	bool Press(int nKey);
	bool Trigger(int nKey);
	bool Release(int nKey);
	bool Repeat(int nKey);

	static Keyboard* Instance(void){ return Self; }

private:
	Keyboard(){}
	Keyboard(const Keyboard& other){};
	Keyboard &operator=(const Keyboard& other){};

	static Keyboard* Self;
	BYTE KeyState[256];		//キー入力情報
	BYTE TriggerKeyState[256];	//トリガーキー入力情報
	BYTE ReleaseKeyState[256];	//リリースキー入力情報
	BYTE RepeateKeyState[256];	//リピートキー入力情報
	short RepeateKeyFlame[256];	//リピートキー用フレームカウント

	LPDIRECTINPUTDEVICE8 InputDevice;

};

#endif