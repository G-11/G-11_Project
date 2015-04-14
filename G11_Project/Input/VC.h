//=================================================================
//仮想コントローラークラス
//=================================================================
#ifndef _VC_H_
#define _VC_H_

//=================================================================
//インクルード
//=================================================================
#include"../main.h"

enum
{
	COMMAND_OK = 0,
	COMMAND_CANCEL,
	COMMAND_A,
	COMMAND_B,
	COMMAND_X,
	COMMAND_Y,
	COMMAND_MAX
};
//=================================================================
//前方宣言
//=================================================================
class CGamePad;
class CKeyboard;
class CMouse;

//クラス定義
class VC
{
public:

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);

	void SetVibration(float left,int frameL,float right,int frameR);
	void SetVibrationL(float left,int frameL);
	void SetVibrationR(float right,int frameR);


	CGamePad* GamePad(void);
	CKeyboard* Keyboard(void);
	CMouse* Mouse(void);

	bool Press(int command){ return _Press[command]; }
	bool Trigger(int command){ return _Trigger[command]; }
	bool Release(int command){ return _Release[command]; }
	bool Repeat(int command){ return _Repeat[command]; }

	static VC* Instance(void){return Self;}
private:
	VC();
	VC(const VC& other){};
	VC &operator=(const VC& other){};

	void ClearBuffer(void);

	static VC* Self;

	CGamePad	*_GamePad;
	CKeyboard	*_Keyboard;
	CMouse		*_Mouse;
	LPDIRECTINPUT8 DInput;

	bool _Press[COMMAND_MAX];
	bool _Trigger[COMMAND_MAX];
	bool _Release[COMMAND_MAX];
	bool _Repeat[COMMAND_MAX];

	float LAxisX;
	float LAxisY;

	float RAxisX;
	float RAxisY;
};

#endif