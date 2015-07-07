//=================================================================
//仮想コントローラークラス
//=================================================================
//=================================================================
//インクルード
//=================================================================
#include "VC.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"

//=================================================================
//スタティック変数定義
//=================================================================
VC* VC::Self = new VC;

typedef struct
{
	UINT Key[5];
	UINT Pad[5];
}COMMAND;

const COMMAND Command[COMMAND_MAX]= 
{
	{ { DIK_Z,DIK_RETURN,-1,-1,-1 },{ GAMEPAD_A,GAMEPAD_START,-1,-1,-1 } },
	{ { DIK_X,-1,-1,-1,-1 },{ GAMEPAD_B,-1,-1,-1,-1 } },
	{ { DIK_Q,-1,-1,-1,-1 },{ GAMEPAD_A,-1,-1,-1,-1 } },
	{ { DIK_E,-1,-1,-1,-1 },{ GAMEPAD_B,-1,-1,-1,-1 } },
	{ { DIK_A,-1,-1,-1,-1 },{ GAMEPAD_X,-1,-1,-1,-1 } },
	{ { DIK_S,-1,-1,-1,-1 },{ GAMEPAD_Y,-1,-1,-1,-1 } },
	{ { DIK_P,-1,-1,-1,-1 },{ GAMEPAD_START,-1,-1,-1,-1 } },
	{ { DIK_UP,-1,-1,-1,-1 },{ GAMEPAD_DPAD_UP,-1,-1,-1,-1} },
	{ { DIK_RIGHT,-1,-1,-1,-1 },{ GAMEPAD_DPAD_RIGHT,-1,-1,-1,-1 } },
	{ { DIK_DOWN,-1,-1,-1,-1 },{ GAMEPAD_DPAD_DOWN,-1,-1,-1,-1 } },
	{ { DIK_LEFT,-1,-1,-1,-1 },{ GAMEPAD_DPAD_LEFT,-1,-1,-1,-1 } },
};

typedef struct KEY_BUFF
{
	KEY_BUFF(){ Press = Trigger = Release = Repeat = false;}
	bool Press;
	bool Trigger;
	bool Release;
	bool Repeat;
}KEY_BUFF;
//=================================================================
//コンストラクタ
//=================================================================
VC::VC()
{
	_GamePad = nullptr;
	_Keyboard = nullptr;
	_Mouse = nullptr;
	DInput = nullptr;
}

//=================================================================
//初期化
//=================================================================
HRESULT VC::Init(HWND hWnd)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);

	HRESULT hr;
	if (DInput == NULL)
	{
		hr = DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&DInput,NULL);

		if (FAILED(hr))
		{
			MessageBox(NULL,"インプットオブジェクトの作成に失敗しました","ERROR!",(MB_ICONERROR | MB_OK));
			return hr;
		}
	}
	_GamePad = GamePad::Initialize(DInput,hInstance,hWnd);

	_Keyboard = Keyboard::Instance();
	if (FAILED(_Keyboard->Init(DInput,hInstance,hWnd)))
	{
		MessageBox(nullptr,"キーボードの初期化に失敗しました","ERROR!",MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	_Mouse = Mouse::Instance();
	if (FAILED(_Mouse->Init(DInput,hInstance,hWnd)))
	{
		MessageBox(nullptr,"マウスの初期化に失敗しました","ERROR!",MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	for (int cnt = 0;cnt < COMMAND_MAX;cnt++)
	{
		_Press[cnt] = false;
		_Trigger[cnt] = false;
		_Release[cnt] = false;
		_Repeat[cnt] = false;
	}
	LAxisX = LAxisY = 0;
	RAxisX = RAxisY = 0;

	return S_OK;

}
//=================================================================
//終了処理
//=================================================================
void VC::Uninit(void)
{
	if (_GamePad != nullptr)
	{
		_GamePad->Uninit();
		_GamePad = nullptr;
	}

	if (_Keyboard != nullptr)
	{
		_Keyboard->Uninit();
		_Keyboard = nullptr;
	}

	if (_Mouse != nullptr)
	{
		_Mouse->Uninit();
		_Mouse = nullptr;
	}

	if (DInput != nullptr)
	{
		DInput->Release();
		DInput = nullptr;
	}

	delete this;
	Self = nullptr;
}
//=================================================================
//更新
//=================================================================
void VC::Update(void)
{
	if (_GamePad != nullptr)
	{
		_GamePad->Update();
	}
	if (_Keyboard != nullptr)
	{
		_Keyboard->Update();
	}
	if (_Mouse != nullptr)
	{
		_Mouse->Update();
	}
	//ClearBuffer();
	for (int cnt = 0;cnt < COMMAND_MAX;cnt++)
	{
		KEY_BUFF buff;
		for (int num = 0;num < 5;num++)
		{
			if (Command[cnt].Key[num] != -1)
			{
				buff.Press	 |= _Keyboard->Press(Command[cnt].Key[num]);
				buff.Trigger |= _Keyboard->Trigger(Command[cnt].Key[num]);
				buff.Release |= _Keyboard->Release(Command[cnt].Key[num]);
				buff.Repeat	 |= _Keyboard->Repeat(Command[cnt].Key[num]);
			}
			if (_GamePad != nullptr)
			{
				if (Command[cnt].Pad[num] != -1)
				{
					buff.Press |= _GamePad->Press(Command[cnt].Pad[num]);
					buff.Trigger |= _GamePad->Trigger(Command[cnt].Pad[num]);
					buff.Release |= _GamePad->Release(Command[cnt].Pad[num]);
					buff.Repeat |= _GamePad->Repeat(Command[cnt].Pad[num]);
				}
			}
			_Press[cnt] = buff.Press;
			_Trigger[cnt] = buff.Trigger;
			_Release[cnt] = buff.Release;
			_Repeat[cnt] = buff.Repeat;
		}
	}
	if (_GamePad != nullptr)
	{
		LAxisX = _GamePad->StickX(LEFT);
		RAxisX = _GamePad->StickX(RIGHT);
		LAxisY = _GamePad->StickY(LEFT);
		RAxisY = _GamePad->StickY(RIGHT);
	}
	else
	{
		LAxisX = 0;
		RAxisX = 0;
		LAxisY = 0;
		RAxisY = 0;
	}

	if (LAxisX == 0 && _Keyboard->Press(DIK_A))
	{
		LAxisX = -1.0f;
	}
	else if (LAxisX == 0 && _Keyboard->Press(DIK_D))
	{
		LAxisX = 1.0f;
	}

	if (LAxisY == 0 && _Keyboard->Press(DIK_W))
	{
		LAxisY = 1.0f;
	}
	else if (LAxisY == 0 && _Keyboard->Press(DIK_S))
	{
		LAxisY = -1.0f;
	}

	if (RAxisX == 0 && _Keyboard->Press(DIK_LEFT))
	{
		RAxisX = -1.0f;
	}
	else if (RAxisX == 0 && _Keyboard->Press(DIK_RIGHT))
	{
		RAxisX = 1.0f;
	}

	if (RAxisY == 0 && _Keyboard->Press(DIK_UP))
	{
		RAxisY = 1.0f;
	}
	else if (RAxisY == 0 && _Keyboard->Press(DIK_DOWN))
	{
		RAxisY = -1.0f;
	}
}

void VC::ClearBuffer(void)
{
	for (int cnt = 0;cnt < COMMAND_MAX;cnt++)
	{
		for (int num = 0;num < 5;num++)
		{
			_Press[cnt] |= false;
			_Trigger[cnt] |= false;
			_Release[cnt] |= false;
			_Repeat[cnt] |= false;
		}
	}
}

//=================================================================
//ゲッター
//=================================================================
GamePad* VC::gamePad(void)
{
	return _GamePad;
}
Keyboard* VC::keyboard(void)
{
	return _Keyboard;
}
Mouse* VC::mouse(void)
{
	return _Mouse;
}

void VC::SetVibration(float left,int frameL,float right,int frameR)
{
	_GamePad->SetVibration(left,frameL,right,frameR);
}
void VC::SetVibrationL(float left,int frameL)
{
	_GamePad->SetVibrationL(left,frameL);
}
void VC::SetVibrationR(float right,int frameR)
{
	_GamePad->SetVibrationR(right,frameR);
}
