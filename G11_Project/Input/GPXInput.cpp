#define _CRT_SECURE_NO_WARNINGS	//åxçêëŒçÙ
#define DIRECTINPUT_VERSION (0x0800)// åxçêëŒçÙóp

#pragma comment(lib,"XInput9_1_0.lib")


#include "GPXInput.h"
#include<windows.h>
#include<Xinput.h>

const float THUMB_BASE = 32767.0f;
const float TRIGGER_BASE = 255.0f;
const int VIBRATION_MULTI = 65535;

const WORD key[] =
{
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT
};

GPXInput::GPXInput()
{
	ConnectedId = -1;
	for (int cnt = 0;cnt < GAMEPAD_MAX;cnt++)
	{
		Old[cnt] = false;
		_Press[cnt] = false;
		_Trigger[cnt] = false;
		_Release[cnt] = false;
		_Repeat[cnt] = false;
		RepeatFrame[cnt] = 0;
	}
	VibrationState[0] = VibrationState[1] = 0;
	VibrationFrame[0] = VibrationFrame[1] = 0;
	_StickX[0] = _StickX[1] = 0;
	_StickY[0] = _StickY[1] = 0;
	_Shoulder[0] = _Shoulder[1] = 0;
}

bool GPXInput::Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd)
{
	DWORD dwResult;
	for (short i = 0; i< 4; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state,sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i,&state);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected 
			ConnectedId = i;
			break;
		}
	}
	if (ConnectedId == -1)
	{
		//MessageBox(nullptr,"ÉQÅ[ÉÄÉpÉbÉhÇ™ê⁄ë±Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒ","INFO",MB_OK|MB_ICONINFORMATION);
		return false;
	}

	return true;
}
void GPXInput::Uninit(void)
{
	if (Self != nullptr)
	{
		delete Self;
		Self = nullptr;
	}
}
void GPXInput::Update(void)
{
	if (ConnectedId == -1)
	{
		return;
	}
	XINPUT_STATE InputState;

	XInputGetState(ConnectedId,&InputState);

	memcpy(Old,_Press,sizeof(bool)*GAMEPAD_MAX);

	for (int cnt = 0;cnt < GAMEPAD_MAX;cnt++)
	{
		_Press[cnt] = (InputState.Gamepad.wButtons&key[cnt])?true:false;
		_Trigger[cnt] = (Old[cnt] ^ _Press[cnt]) & _Press[cnt];
		_Release[cnt] = (Old[cnt] ^ _Press[cnt]) & Old[cnt];

		if (_Press[cnt] == true)
		{
			if (RepeatFrame[cnt] < REPEAT_FRAME)
			{
				_Repeat[cnt] = _Trigger[cnt];
			}
			else
			{
				_Repeat[cnt] = _Press[cnt];
			}
			RepeatFrame[cnt]++;
		}
		else
		{
			RepeatFrame[cnt] = 0;
			_Repeat[cnt] = false;
		}
	}
	int Thumb = InputState.Gamepad.sThumbLX;
	if (Thumb < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Thumb > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		Thumb = 0;
	}
	_StickX[LEFT]	= (float)(Thumb / THUMB_BASE);

	Thumb = InputState.Gamepad.sThumbLY;
	if (Thumb < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Thumb > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		Thumb = 0;
	}
	_StickY[LEFT]	= (float)(Thumb / THUMB_BASE);

	Thumb = InputState.Gamepad.sThumbRX;
	if (Thumb < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && Thumb > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		Thumb = 0;
	}
	_StickX[RIGHT]	= (float)(Thumb / THUMB_BASE);

	Thumb = InputState.Gamepad.sThumbRY;
	if (Thumb < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && Thumb > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		Thumb = 0;
	}
	_StickY[RIGHT]	= (float)(Thumb / THUMB_BASE);

	_Shoulder[LEFT] = (float)(InputState.Gamepad.bLeftTrigger / TRIGGER_BASE);
	_Shoulder[RIGHT] = (float)(InputState.Gamepad.bRightTrigger / TRIGGER_BASE);

	for (int cnt = 0;cnt < 2;cnt++)
	{
		if (VibrationFrame[cnt] < 0)
		{
			VibrationState[cnt] = 0;
			XINPUT_VIBRATION vibration;
			vibration.wLeftMotorSpeed = (WORD)(VibrationState[LEFT] * VIBRATION_MULTI);
			vibration.wRightMotorSpeed = (WORD)(VibrationState[RIGHT] * VIBRATION_MULTI);
			XInputSetState(ConnectedId,&vibration);
		}
		else
		{
			VibrationFrame[cnt]--;
		}
	}
}

void GPXInput::SetVibration(float left,int frameL,float right,int frameR)
{
	VibrationState[LEFT] = left;
	VibrationState[RIGHT] = right;
	VibrationFrame[LEFT] = frameL;
	VibrationFrame[RIGHT] = frameR;
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = (WORD)(left*VIBRATION_MULTI);
	vibration.wRightMotorSpeed = (WORD)(right*VIBRATION_MULTI);
	XInputSetState(ConnectedId,&vibration);
}

void GPXInput::SetVibrationL(float left,int frameL)
{
	VibrationState[LEFT] = left;
	VibrationFrame[LEFT] = frameL;
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = (WORD)(VibrationState[LEFT] * VIBRATION_MULTI);
	vibration.wRightMotorSpeed = (WORD)(VibrationState[RIGHT] * VIBRATION_MULTI);
	XInputSetState(ConnectedId,&vibration);
}

void GPXInput::SetVibrationR(float right,int frameR)
{
	VibrationState[RIGHT] = right;
	VibrationFrame[RIGHT] = frameR;
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = (WORD)(VibrationState[LEFT] * VIBRATION_MULTI);
	vibration.wRightMotorSpeed = (WORD)(VibrationState[RIGHT] * VIBRATION_MULTI);
	XInputSetState(ConnectedId,&vibration);
}