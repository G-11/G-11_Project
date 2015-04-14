//=============================================================================
// キーボードクラス
// Createby Arai Yuhki
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "Keyboard.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define REPEATE_TRIGGER (20)

CKeyboard* CKeyboard::Self = new CKeyboard;

CKeyboard::~CKeyboard()
{
	if (InputDevice != nullptr)
	{
		InputDevice->Release();
		InputDevice = nullptr;
	}
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CKeyboard::Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd)
{

	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = DInput->CreateDevice(GUID_SysKeyboard,&InputDevice,NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,"デバイスオブジェクトの作成に失敗しました","ERROR!",(MB_ICONERROR | MB_OK));
		return hr;
	}
	// データフォーマットを設定
	hr = InputDevice->SetDataFormat(&c_dfDIKeyboard);

	if(FAILED(hr))
	{
		MessageBox(NULL,"データフォーマットの設定に失敗しました","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = InputDevice->SetCooperativeLevel(hWnd,(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	if(FAILED(hr))
	{
		MessageBox(NULL,"強調モードの設定に失敗しました","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	InputDevice->Acquire();
	return S_OK;
}

void CKeyboard::Uninit(void)
{
	delete Self;
	Self = nullptr;
}

//=============================================================================
// 更新
//=============================================================================
void CKeyboard::Update(void)
{
	BYTE CurrentKeyState[256] ={0};
	if(SUCCEEDED(InputDevice->GetDeviceState(sizeof(CurrentKeyState),&CurrentKeyState[0])))
	{
		for(int nKey=0;nKey<256;nKey++)
		{
			TriggerKeyState[nKey] = (KeyState[nKey]^CurrentKeyState[nKey]) & CurrentKeyState[nKey];	//トリガー取得
			ReleaseKeyState[nKey] = (KeyState[nKey]^CurrentKeyState[nKey]) & KeyState[nKey];//リリース取得

			//リピート取得
			if((KeyState[nKey]&CurrentKeyState[nKey]))
			{
				RepeateKeyFlame[nKey]++;//押されている間はカウントアップ
			}
			else
			{
				RepeateKeyFlame[nKey] = 0;	//離されたら0に戻す
			}

			if(RepeateKeyFlame[nKey]>REPEATE_TRIGGER)
			{
				RepeateKeyState[nKey] = CurrentKeyState[nKey];	//指定フレーム数押され続けたらホールド
			}
			else
			{
				RepeateKeyState[nKey] = TriggerKeyState[nKey];	//指定されたフレーム数までトリガー
			}
			KeyState[nKey] = CurrentKeyState[nKey];
		}
	}
	else
	{
		InputDevice->Acquire();
	}

}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool CKeyboard::Press(int nKey)
{
	return KeyState[nKey] ? true:false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool CKeyboard::Trigger(int nKey)
{
	return TriggerKeyState[nKey] ? true:false;
}
//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool CKeyboard::Release(int nKey)
{
	return ReleaseKeyState[nKey] ? true:false;
}
//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool CKeyboard::Repeat(int nKey)
{
	return RepeateKeyState[nKey] ? true:false;
}

