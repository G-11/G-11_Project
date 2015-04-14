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
class CKeyboard
{
public:
	
	~CKeyboard();
	HRESULT Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	static void Uninit(void);
	void Update(void);

	bool Press(int nKey);
	bool Trigger(int nKey);
	bool Release(int nKey);
	bool Repeat(int nKey);

	static CKeyboard* Instance(void){ return Self; }

private:
	CKeyboard(){}
	CKeyboard(const CKeyboard& other){};
	CKeyboard &operator=(const CKeyboard& other){};

	static CKeyboard* Self;
	BYTE KeyState[256];		//キー入力情報
	BYTE TriggerKeyState[256];	//トリガーキー入力情報
	BYTE ReleaseKeyState[256];	//リリースキー入力情報
	BYTE RepeateKeyState[256];	//リピートキー入力情報
	short RepeateKeyFlame[256];	//リピートキー用フレームカウント

	LPDIRECTINPUTDEVICE8 InputDevice;

};

#endif