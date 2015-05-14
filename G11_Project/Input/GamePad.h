#pragma once
#ifndef _GAME_PAD_H_
#define _GAME_PAD_H_
#include "../main.h"

typedef enum
{
	GAMEPAD_A = 0,
	GAMEPAD_B,
	GAMEPAD_X,
	GAMEPAD_Y,
	GAMEPAD_BACK,
	GAMEPAD_START,
	GAMEPAD_LEFT_SHOULDER,
	GAMEPAD_RIGHT_SHOULDER,
	GAMEPAD_LEFT_THUMB,
	GAMEPAD_RIGHT_THUMB,
	GAMEPAD_DPAD_DOWN,
	GAMEPAD_DPAD_UP,
	GAMEPAD_DPAD_LEFT,
	GAMEPAD_DPAD_RIGHT,
	GAMEPAD_MAX
};

enum
{
	LEFT = 0,
	RIGHT,
	LR
};

class GamePad
{
public:
	static GamePad* Initialize(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	virtual bool Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;

	virtual bool Press(char num)const = 0;
	virtual bool Trigger(char num)const = 0;
	virtual bool Release(char num)const = 0;
	virtual bool Repeat(char num)const = 0;
	virtual float StickX(char direction)const = 0;
	virtual float StickY(char direction)const = 0;
	virtual float Shoulder(char direction)const = 0;

	virtual void SetVibration(float left,int frameL,float right,int frameR){}
	virtual void SetVibrationL(float left,int frameL){}
	virtual void SetVibrationR(float right,int frameR){}

	static GamePad* Instance(void){ return Self; }

protected:
	static const int REPEAT_FRAME = 15;
	static GamePad* Self;

	bool Old[GAMEPAD_MAX];
	bool _Press[GAMEPAD_MAX];
	bool _Trigger[GAMEPAD_MAX];
	bool _Release[GAMEPAD_MAX];
	bool _Repeat[GAMEPAD_MAX];
	int RepeatFrame[GAMEPAD_MAX];
	float _StickX[2];
	float _StickY[2];
	float _Shoulder[2];
};


#endif