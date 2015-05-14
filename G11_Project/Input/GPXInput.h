#ifndef _GP_XINPUT_H_
#define _GP_XINPUT_H_
#include "GamePad.h"

class GPXInput:public GamePad
{
public:
	GPXInput();

	bool Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool Press(char num)const{ return _Press[num]; }
	bool Trigger(char num)const{ return _Trigger[num]; }
	bool Release(char num)const{ return _Release[num]; }
	bool Repeat(char num)const{ return _Repeat[num]; }

	float StickX(char direction)const{ return _StickX[direction]; }
	float StickY(char direction)const{ return _StickY[direction]; }
	float Shoulder(char direction)const{ return _Shoulder[direction]; }

	void SetVibration(float left,int frameL,float right,int frameR);
	void SetVibrationL(float left,int frameL);
	void SetVibrationR(float right,int frameR);


private:
	GPXInput(const GPXInput &other){};
	GPXInput &operator=(const GPXInput& other){};

	short ConnectedId;

	float VibrationState[2];
	int VibrationFrame[2];
};

#endif