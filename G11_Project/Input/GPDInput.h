#pragma once
#ifndef _GP_DIRECTINPUT_H_
#define _GP_DIRECTINPUT_H_
#include "GamePad.h"



class GPDInput:public GamePad
{
private:
	GPDInput(const GPDInput& other){}
	GPDInput &operator=(const GPDInput& other){};

	static LPDIRECTINPUTDEVICE8 InputDevice;

	static LPDIRECTINPUT8 DInput;
	static DIDEVCAPS DeviceCaps;

	const static short key[GAMEPAD_MAX];

public:
	GPDInput(){};
	~GPDInput(){};

	bool Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	void Update(void);
	void Uninit(void);
	
	bool Press(char button)const{ return _Press[key[button]]; }
	bool Trigger(char button)const{ return _Trigger[key[button]]; }
	bool Release(char button)const{ return _Release[key[button]]; }
	bool Repeat(char button)const{ return _Repeat[key[button]]; }
	float StickX(char direction)const{ return _StickX[direction]; }
	float StickY(char direction)const{ return -_StickY[direction]; }
	float Shoulder(char direction)const{ return _Shoulder[direction]; }

	static BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE* instance,void* context);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *instance,VOID *context);


};

#endif