#include "Mouse.h"


Mouse* Mouse::Self = new Mouse;
HWND Mouse::m_phWnd;

Mouse::~Mouse()
{
	if (InputDevice != nullptr)
	{
		InputDevice->Release();
		InputDevice = nullptr;
	}
}

//=============================================================================
//初期化
//=============================================================================
HRESULT Mouse::Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd)
{
	DIPROPDWORD dipdw;
	// 「DirectInputデバイス」オブジェクトの作成
	if( FAILED( DInput->CreateDevice( GUID_SysMouse,&InputDevice, NULL ) ) )
	{
		MessageBox(hWnd,"マウスのインプットデバイスの作成に失敗","ERROR!",MB_OK|MB_ICONERROR);
		return E_FAIL;
	} 
	
	// デバイスをマウスに設定
	if( FAILED( InputDevice->SetDataFormat( &c_dfDIMouse2 ) ) )
	{
		MessageBox(hWnd,"データフォーマットの設定に失敗","ERROR!",MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// 協調レベルの設定
	if( FAILED( InputDevice->SetCooperativeLevel( 
		hWnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )) )
	{
		MessageBox(hWnd,"強調モードの設定に失敗","ERROR!",MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// the header
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;
	// the data
	dipdw.dwData = 16;
	
	InputDevice->SetProperty(DIPROP_AXISMODE,&dipdw.diph);

	// デバイスを「取得」する
	InputDevice->Acquire();

	m_phWnd = hWnd;
	WheelStatus = WHEEL_NEUTRAL;

	m_phWnd = hWnd;

	_MoveX = _MoveY = 0;
	return S_OK;
}

void Mouse::Uninit(void)
{
	delete Self;
}

//=============================================================================
//更新
//=============================================================================
void Mouse::Update(void)
{
	DIMOUSESTATE2 CurrentMouseStatus;
	if(FAILED(InputDevice->GetDeviceState(sizeof(DIMOUSESTATE2),&CurrentMouseStatus)))
	{
		InputDevice->Acquire();
		return ;
	}
	POINT Pos;
	GetCursorPos(&Pos);
	ScreenToClient(m_phWnd,&Pos);	//相対座標を絶対座標に変換

	_MoveX = (float)(Pos.x - MouseStatus.lX);
	_MoveY = (float)(Pos.y - MouseStatus.lY);
	MouseStatus.lX = Pos.x;
	MouseStatus.lY = Pos.y;
	if(CurrentMouseStatus.lZ - MouseStatus.lZ == 0)
	{
		WheelStatus = WHEEL_NEUTRAL;
	}
	else if(CurrentMouseStatus.lZ - MouseStatus.lZ > 0)
	{
		WheelStatus = WHEEL_UP;
	}
	else
	{
		WheelStatus = WHEEL_DOWN;
	}

	
	//キー入力判定
	for(int cnt=0;cnt<3;cnt++)
	{
		TriggerMouse[cnt] = (MouseStatus.rgbButtons[cnt]^CurrentMouseStatus.rgbButtons[cnt])&CurrentMouseStatus.rgbButtons[cnt];
		ReleaseMouse[cnt] = (MouseStatus.rgbButtons[cnt]^CurrentMouseStatus.rgbButtons[cnt])&~CurrentMouseStatus.rgbButtons[cnt];
		MouseStatus.rgbButtons[cnt] = CurrentMouseStatus.rgbButtons[cnt];
	}
}

//=============================================================================
// マウスのX座標取得
//=============================================================================
float Mouse::PosX(void)
{
	return (float)MouseStatus.lX;
}
//=============================================================================
// マウスのY座標取得
//=============================================================================
float Mouse::PosY(void)
{
	return (float)MouseStatus.lY;
}
//=============================================================================
// マウスのウィールを取得
//=============================================================================
int Mouse::Wheel(void)
{
	return WheelStatus;
}
//=============================================================================
// マウスのプレス取得
//=============================================================================
bool Mouse::Press(int nKey)
{
	return MouseStatus.rgbButtons[nKey] ? true:false;
}
//=============================================================================
// マウスのトリガー取得
//=============================================================================
bool Mouse::Trigger(int nKey)
{
	return TriggerMouse[nKey] ? true:false;
}
//=============================================================================
// マウスのリリース取得
//=============================================================================
bool Mouse::Release(int nKey)
{
	return ReleaseMouse[nKey] ? true:false;
}
//=============================================================================
// マウスの水平方向の移動量取得
//=============================================================================
float Mouse::MoveX(void)
{
	return _MoveX;
}
//=============================================================================
// マウスの垂直方向の移動量取得
//=============================================================================
float Mouse::MoveY(void)
{
	return _MoveY;
}