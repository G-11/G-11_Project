#define _CRT_SECURE_NO_WARNINGS	//åxçêëŒçÙ
#define DIRECTINPUT_VERSION (0x0800)// åxçêëŒçÙóp

#pragma comment(lib,"XInput9_1_0.lib")


#include "GamePad.h"
#include<windows.h>
#include<Xinput.h>

const int REPEATE_FRAME = 10;
const float THUMB_BASE = 32767.0f;
const float TRIGGER_BASE = 255.0f;
const int VIBRATION_MULTI = 65535;

CGamePad* CGamePad::Self = new CGamePad;

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

CGamePad::CGamePad()
{
	ConnectedId = -1;
	for (int cnt = 0;cnt < GAMEPAD_MAX;cnt++)
	{
		OldKey[cnt] = false;
		PressKey[cnt] = false;
		TriggerKey[cnt] = false;
		ReleaseKey[cnt] = false;
		RepeateKey[cnt] = false;
		RepeateFrame[cnt] = 0;
	}
	VibrationState[0] = VibrationState[1] = 0;
	VibrationFrame[0] = VibrationFrame[1] = 0;
	ThumbX[0] = ThumbX[1] = 0;
	ThumbY[0] = ThumbY[1] = 0;
	_Trigger[0] = _Trigger[1] = 0;
}

void CGamePad::Init(void)
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
		MessageBox(nullptr,"ÉQÅ[ÉÄÉpÉbÉhÇ™ê⁄ë±Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒ","INFO",MB_OK|MB_ICONINFORMATION);
	}
}
void CGamePad::Uninit(void)
{
	delete Self;
	Self = nullptr;
}
void CGamePad::Update(void)
{
	if (ConnectedId == -1)
	{
		return;
	}
	XINPUT_STATE InputState;

	XInputGetState(ConnectedId,&InputState);

	memcpy(OldKey,PressKey,sizeof(bool)*GAMEPAD_MAX);

	for (int cnt = 0;cnt < GAMEPAD_MAX;cnt++)
	{
		PressKey[cnt] = (InputState.Gamepad.wButtons&key[cnt])?true:false;
		TriggerKey[cnt] = (OldKey[cnt] ^ PressKey[cnt]) & PressKey[cnt];
		ReleaseKey[cnt] = (OldKey[cnt] ^ PressKey[cnt]) & OldKey[cnt];

		if (PressKey[cnt] == true)
		{
			if (RepeateFrame[cnt] < REPEATE_FRAME)
			{
				RepeateKey[cnt] = TriggerKey[cnt];
			}
			else
			{
				RepeateKey[cnt] = PressKey[cnt];
			}
			RepeateFrame[cnt]++;
		}
		else
		{
			RepeateFrame[cnt] = 0;
			RepeateKey[cnt] = false;
		}
	}
	int Thumb = InputState.Gamepad.sThumbLX;
	if (Thumb < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Thumb > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		Thumb = 0;
	}
	ThumbX[LEFT]	= (float)(Thumb / THUMB_BASE);

	Thumb = InputState.Gamepad.sThumbLY;
	if (Thumb < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && Thumb > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		Thumb = 0;
	}
	ThumbY[LEFT]	= (float)(Thumb / THUMB_BASE);

	Thumb = InputState.Gamepad.sThumbRX;
	if (Thumb < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && Thumb > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		Thumb = 0;
	}
	ThumbX[RIGHT]	= (float)(Thumb / THUMB_BASE);

	Thumb = InputState.Gamepad.sThumbRY;
	if (Thumb < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && Thumb > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		Thumb = 0;
	}
	ThumbY[RIGHT]	= (float)(Thumb / THUMB_BASE);

	_Trigger[LEFT]	= (float)(InputState.Gamepad.bLeftTrigger / TRIGGER_BASE);
	_Trigger[RIGHT]	= (float)(InputState.Gamepad.bRightTrigger / TRIGGER_BASE);

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

void CGamePad::SetVibration(float left,int frameL,float right,int frameR)
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

void CGamePad::SetVibrationL(float left,int frameL)
{
	VibrationState[LEFT] = left;
	VibrationFrame[LEFT] = frameL;
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = (WORD)(VibrationState[LEFT] * VIBRATION_MULTI);
	vibration.wRightMotorSpeed = (WORD)(VibrationState[RIGHT] * VIBRATION_MULTI);
	XInputSetState(ConnectedId,&vibration);
}

void CGamePad::SetVibrationR(float right,int frameR)
{
	VibrationState[RIGHT] = right;
	VibrationFrame[RIGHT] = frameR;
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = (WORD)(VibrationState[LEFT] * VIBRATION_MULTI);
	vibration.wRightMotorSpeed = (WORD)(VibrationState[RIGHT] * VIBRATION_MULTI);
	XInputSetState(ConnectedId,&vibration);
}