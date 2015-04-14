#ifndef _MOUSE_H_
#define _MOUSE_H_

#include"VC.h"
class CMouse
{
public:
	enum
	{
		MOUSE_LEFT =0,
		MOUSE_RIGHT,
		MOUSE_CENTER,
		MOUSE_MAX,
	};

	enum
	{
		WHEEL_NEUTRAL=0,
		WHEEL_UP,
		WHEEL_DOWN,
		WHEEL_MAX
	};

	HRESULT Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	void Update(void);
	static void Uninit(void);
	float PosX(void);	//X座標取得
	float PosY(void);	//Y座標取得
	float MoveX(void);	//X座標取得
	float MoveY(void);	//Y座標取得
	int Wheel(void);	//ウィールを取得
	bool Press(int nKey);	//プレス情報
	bool Trigger(int nKey);	//トリガー情報
	bool Release(int nKey);	//リリース情報
	static void SetHWND(HWND hWnd){m_phWnd = hWnd;}

	static CMouse* Instance(void){return Self;}
private:
	CMouse(){};
	~CMouse();
	CMouse(const CMouse& other){};
	CMouse &operator= (const CMouse& other){};
	static CMouse* Self;

	LPDIRECTINPUTDEVICE8 InputDevice;
	DIMOUSESTATE2 MouseStatus;				//マウスのステータス保存用
	BYTE TriggerMouse[3];							//マウスボタンのトリガー情報
	BYTE ReleaseMouse[3];							//マウスボタンのリリース情報
	int WheelStatus;								//ウィールの情報
	float _MoveX;
	float _MoveY;
	POINT pos;
	static HWND m_phWnd;
};

#endif