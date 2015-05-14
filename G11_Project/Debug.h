#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "main.h"
#define LENGTH_STRING_BUFF (1028)	//バッファ量

class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);

	static void Print(char* fmt,...);
	static void Draw(void);
	static void DrawFPSGraph(void);
	static void SetUpdaeTime(float update,float draw);
	static bool IsEnableDisp(void){return DispFlag;}
	static void EnableDisp(bool bDisp){DispFlag = bDisp;}

private:
	static LPD3DXFONT pFont;
	static char m_Str[LENGTH_STRING_BUFF];
	static bool DispFlag;
	static RECT DebugRect;
	static float updateTime;
	static float drawTime;

	static LPDIRECT3DVERTEXBUFFER9 GraphBG;
	static LPDIRECT3DVERTEXBUFFER9 UpdateGraph;
	static LPDIRECT3DVERTEXBUFFER9 DrawGraph;
	static D3DXVECTOR3 updatePoint[30];
	static D3DXVECTOR3 drawPoint[30];
};

#endif