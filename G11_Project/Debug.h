#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "main.h"
#define LENGTH_STRING_BUFF (1028)	//バッファ量

class DebugProc
{
public:
	DebugProc();
	~DebugProc();
	void Init(void);
	void Uninit(void);

	static void Print(char* fmt,...);
	static void Draw(void);
	static bool IsEnableDisp(void){return DispFlag;}
	static void EnableDisp(bool bDisp){DispFlag = bDisp;}

private:
	static LPD3DXFONT pFont;
	static char m_Str[LENGTH_STRING_BUFF];
	static bool DispFlag;
	static RECT DebugRect;
};

#endif