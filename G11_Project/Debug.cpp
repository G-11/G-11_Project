#include "Debug.h"
#include <stdio.h>
#include <string.h>

LPD3DXFONT DebugProc::pFont = nullptr;
char DebugProc::m_Str[];
bool DebugProc::DispFlag = true;
RECT DebugProc::DebugRect = {0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT};

DebugProc::DebugProc()
{
	DispFlag = true;
}

DebugProc::~DebugProc()
{
	if(pFont != NULL)
	{
		pFont->Release();
		pFont = NULL;
	}
}

void DebugProc::Init(void)
{
	if(pFont == NULL)
	{
		D3DXCreateFont(Window::Instance()->Device(),0,0,15,0,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,
						"ÉÅÉCÉäÉI",&pFont);
	}
	for(int cnt=0;cnt<LENGTH_STRING_BUFF;cnt++)
	{
		m_Str[cnt] = 0;
	}
}

void DebugProc::Uninit(void)
{

}
void DebugProc::Print(char* fmt,...)
{
#ifdef _DEBUG
	char Buff[LENGTH_STRING_BUFF] = {0};
	va_list argp;
	va_start(argp, fmt);
	vsprintf_s(Buff, 256, fmt, argp);
	va_end(argp);

	strcat(m_Str,Buff);
#endif
}

void DebugProc::Draw(void)
{
#ifdef _DEBUG

	if(DispFlag == true)
	{
		pFont->DrawTextA(NULL,m_Str,-1,&DebugRect,DT_LEFT,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));	
	}
	for(int cnt=0;cnt<1024;cnt++)
	{
		m_Str[cnt] = 0;
	}
#endif
}