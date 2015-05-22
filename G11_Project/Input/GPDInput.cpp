#define _CRT_SECURE_NO_WARNINGS	//警告対策
#define DIRECTINPUT_VERSION (0x0800)// 警告対策用


#include "GPDInput.h"
#include"../Debug.h"

DIDEVCAPS GPDInput::DeviceCaps;
LPDIRECTINPUT8 GPDInput::DInput = nullptr;
LPDIRECTINPUTDEVICE8 GPDInput::InputDevice = nullptr;

const short GPDInput::key[GAMEPAD_MAX] = {1,2,3,0,6,7,4,5,8,9,12,10,13,11};

bool GPDInput::Init(LPDIRECTINPUT8 dInput,HINSTANCE hInstance,HWND hWnd)
{
	DInput = dInput;
	DInput->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoystickCallback,NULL,DIEDFL_ATTACHEDONLY);

	if (InputDevice == nullptr){ return false; }

	InputDevice->SetDataFormat(&c_dfDIJoystick);
	InputDevice->SetCooperativeLevel(hWnd,DISCL_EXCLUSIVE|DISCL_FOREGROUND);
	InputDevice->GetCapabilities(&DeviceCaps);
	InputDevice->EnumObjects(EnumAxesCallback,(void*)hWnd,DIDFT_AXIS);

	HRESULT hr = InputDevice->Poll();
	if (FAILED(hr))
	{
		hr = InputDevice->Acquire();
		if (FAILED(hr))
		{
			InputDevice->Acquire();
		}
	}

	return true;
}

void GPDInput::Uninit(void)
{
	if (InputDevice != nullptr)
	{
		InputDevice->Unacquire();
		InputDevice->Release();
		InputDevice = nullptr;
	}
	if (Self != nullptr)
	{
		delete Self;
		Self = nullptr;
	}
}

void GPDInput::Update(void)
{
	HRESULT hr = InputDevice->Poll();

	if (SUCCEEDED(hr))
	{
		DIJOYSTATE PadState;
		InputDevice->GetDeviceState(sizeof(DIJOYSTATE),&PadState);

		for (int cnt = 0,button = 0;cnt < 12;cnt++)
		{
			if (cnt == 6)
			{
				_Shoulder[LEFT] = (PadState.rgbButtons[cnt] & 0x80) ? 1.0f : 0;
			}
			else if (cnt == 7)
			{
				_Shoulder[RIGHT] = (PadState.rgbButtons[cnt] & 0x80) ? 1.0f : 0;
			}
			else
			{
				_Press[button] =	((PadState.rgbButtons[cnt] & 0x80)) ? true : false;
				_Trigger[button] =	(Old[cnt] ^ _Press[cnt]) & _Press[cnt];
				_Release[button] =	(Old[cnt] ^ _Press[cnt]) & Old[cnt];
				(_Press[button]) ? RepeatFrame[button]++ : RepeatFrame[button] = 0;
				_Repeat[button] = (RepeatFrame[button] > REPEAT_FRAME) ? _Press[button] : _Trigger[button];
				Old[button] = _Press[button];
				button++;
			}
			
			
		}
		for (int cnt = GAMEPAD_DPAD_DOWN;cnt < GAMEPAD_MAX;cnt++)
		{
			_Press[cnt] = _Trigger[cnt] = _Release[cnt] = _Repeat[cnt] = false;
		}
		int direction = PadState.rgdwPOV[0];

		if (direction >= 0)
		{
			if (direction != 0)
			{
				direction /= 4500;
			}

			if (direction % 2 == 1)
			{
				direction = (int)(direction * 0.5f);
				_Press[GAMEPAD_DPAD_DOWN + (direction + 1)%4] = true;
				_Press[GAMEPAD_DPAD_DOWN + (direction)] = true;
			}
			else
			{
				direction = (int)(direction * 0.5f);
				_Press[GAMEPAD_DPAD_DOWN + direction] = true;
			}
		}
		for (int cnt = GAMEPAD_DPAD_DOWN;cnt < GAMEPAD_MAX;cnt++)
		{
			_Trigger[cnt] = (Old[cnt] ^ _Press[cnt]) & _Press[cnt];
			_Release[cnt] = (Old[cnt] ^ _Press[cnt]) & Old[cnt];
			(_Press[cnt]) ? RepeatFrame[cnt]++ : RepeatFrame[cnt] = 0;
			_Repeat[cnt] = (RepeatFrame[cnt] > REPEAT_FRAME) ? _Press[cnt] : _Trigger[cnt];

			Old[cnt] = _Press[cnt];

		}

		_StickX[LEFT] =		PadState.lX / 1000.0f;
		_StickY[LEFT] =		PadState.lY / 1000.0f;
		_StickX[RIGHT] =	PadState.lZ / 1000.0f;
		_StickY[RIGHT] =	PadState.lRz / 1000.0f;
		
	}
	else
	{
		for (int cnt = 0;cnt < GAMEPAD_MAX;cnt++)
		{
			_Press[cnt] = false;
			_Trigger[cnt] = false;
			_Release[cnt] = false;
			_Repeat[cnt] = false;
			RepeatFrame[cnt] = 0;
			_StickX[LEFT] = 0;
			_StickY[LEFT] = 0;
			_StickX[RIGHT] = 0;
			_StickY[RIGHT] = 0;
		}
		InputDevice->Acquire();
	}
}

BOOL CALLBACK GPDInput::EnumJoystickCallback(const DIDEVICEINSTANCE* instance,void* context)
{
	HRESULT hr;
	hr = DInput->CreateDevice(instance->guidInstance,&InputDevice,NULL);
	if (FAILED(hr)){return DIENUM_CONTINUE; }

	return DIENUM_STOP;
}

BOOL CALLBACK GPDInput::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *instance,VOID *context)
{
	HRESULT		hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = instance->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;
	hr = InputDevice->SetProperty(DIPROP_RANGE,&diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	//LRスティックのデッドゾーンの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_BYID;
	diprop.diph.dwObj = instance->dwType;
	diprop.dwData = 2000;//ニュートラルポジションから前後20%は無視する
	hr = InputDevice->SetProperty(DIPROP_DEADZONE,&diprop.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}