#ifndef _GAME_PAD_H_
#define _GAME_PAD_H_

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
	RIGHT
};

class CGamePad
{
public:
	void Init(void);
	static void Uninit(void);
	void Update(void);

	bool Press(short num){ return PressKey[num]; }
	bool Trigger(short num){ return TriggerKey[num]; }
	bool Release(short num){ return ReleaseKey[num]; }
	bool Repeate(short num){ return RepeateKey[num]; }

	float StickX(short direction){ return ThumbX[direction]; }
	float StickY(short direction){ return ThumbY[direction]; }
	float LRTrigger(short direction){ return _Trigger[direction]; }

	void SetVibration(float left,int frameL,float right,int frameR);
	void SetVibrationL(float left,int frameL);
	void SetVibrationR(float right,int frameR);

	static CGamePad* Instance(void){ return Self; }

private:
	CGamePad();
	CGamePad(const CGamePad &other){};
	CGamePad &operator=(const CGamePad& other){};
	
	static CGamePad* Self;

	short ConnectedId;

	bool OldKey[GAMEPAD_MAX];
	bool PressKey[GAMEPAD_MAX];
	bool TriggerKey[GAMEPAD_MAX];
	bool ReleaseKey[GAMEPAD_MAX];
	bool RepeateKey[GAMEPAD_MAX];
	int RepeateFrame[GAMEPAD_MAX];
	float ThumbX[2];
	float ThumbY[2];
	float _Trigger[2];

	float VibrationState[2];
	int VibrationFrame[2];
};

#endif