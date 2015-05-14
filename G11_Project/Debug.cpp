#include "Debug.h"
#include <stdio.h>
#include <string.h>

LPD3DXFONT CDebugProc::pFont = nullptr;
char CDebugProc::m_Str[];
bool CDebugProc::DispFlag = true;
RECT CDebugProc::DebugRect = {0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT};
float CDebugProc::updateTime = 0;
float CDebugProc::drawTime = 0;

LPDIRECT3DVERTEXBUFFER9 CDebugProc::GraphBG = nullptr;
LPDIRECT3DVERTEXBUFFER9 CDebugProc::UpdateGraph = nullptr;
LPDIRECT3DVERTEXBUFFER9 CDebugProc::DrawGraph = nullptr;

D3DXVECTOR3 CDebugProc::updatePoint[30] = { D3DXVECTOR3(0,0,0) };
D3DXVECTOR3 CDebugProc::drawPoint[30] = { D3DXVECTOR3(0,0,0) };

CDebugProc::CDebugProc()
{
	DispFlag = true;

#ifdef _DEBUG
	LPDIRECT3DDEVICE9 device = Window::Instance()->Device();

	device->CreateVertexBuffer(sizeof(VERTEX) * 4,D3DUSAGE_WRITEONLY,FVF_CUSTOM,D3DPOOL_MANAGED,&GraphBG,NULL);
	device->CreateVertexBuffer(sizeof(VERTEX) * 30,D3DUSAGE_WRITEONLY,FVF_CUSTOM,D3DPOOL_MANAGED,&UpdateGraph,NULL);
	device->CreateVertexBuffer(sizeof(VERTEX) * 30,D3DUSAGE_WRITEONLY,FVF_CUSTOM,D3DPOOL_MANAGED,&DrawGraph,NULL);

	VERTEX* vtx = nullptr;
	GraphBG->Lock(0,0,(void**)&vtx,0);
	vtx[0].vtx = D3DXVECTOR3(5.0f,SCREEN_HEIGHT / 2.0f,0);
	vtx[1].vtx = D3DXVECTOR3(105.0f,SCREEN_HEIGHT / 2.0f,0);
	vtx[2].vtx = D3DXVECTOR3(5.0f,SCREEN_HEIGHT - 100.0f,0);
	vtx[3].vtx = D3DXVECTOR3(105.0f,SCREEN_HEIGHT - 100.0f,0);
	vtx[0].diffuse = vtx[1].diffuse = vtx[2].diffuse = vtx[3].diffuse = D3DXCOLOR(0,0,0,0.4f);
	GraphBG->Unlock();
	float one = 100.0f/30.0f;
	for (int cnt = 0;cnt < 30;cnt++)
	{
		updatePoint[cnt] = drawPoint[cnt] = D3DXVECTOR3(one*cnt+5.0f,SCREEN_HEIGHT-100.0f,0);
	}
	UpdateGraph->Lock(0,0,(void**)&vtx,0);
	for (int cnt = 0;cnt < 30;cnt++)
	{
		vtx[cnt].vtx = updatePoint[cnt];
		vtx[cnt].diffuse = GREEN(1.0f);
	}
	UpdateGraph->Unlock();

	DrawGraph->Lock(0,0,(void**)&vtx,0);
	for (int cnt = 0;cnt < 30;cnt++)
	{
		vtx[cnt].vtx = drawPoint[cnt];
		vtx[cnt].diffuse = BLUE(1.0f);
	}
	DrawGraph->Unlock();
#endif
}

CDebugProc::~CDebugProc()
{
	if(pFont != NULL)
	{
		pFont->Release();
		pFont = NULL;
	}
	SafeRelease(UpdateGraph);
	SafeRelease(DrawGraph);
	SafeRelease(GraphBG);
}

void CDebugProc::Init(void)
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

void CDebugProc::Uninit(void)
{

}
void CDebugProc::Print(char* fmt,...)
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

void CDebugProc::Draw(void)
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

void CDebugProc::DrawFPSGraph(void)
{
#ifdef _DEBUG
	VERTEX* vtx = nullptr;
	UpdateGraph->Lock(0,0,(void**)&vtx,0);
	for (int cnt = 0;cnt < 30;cnt++)
	{
		vtx[cnt].vtx = updatePoint[cnt];
		vtx[cnt].diffuse = GREEN(1.0f);
	}
	UpdateGraph->Unlock();

	DrawGraph->Lock(0,0,(void**)&vtx,0);
	for (int cnt = 0;cnt < 30;cnt++)
	{
		vtx[cnt].vtx = drawPoint[cnt];
		vtx[cnt].diffuse = BLUE(1.0f);
	}
	DrawGraph->Unlock();

	LPDIRECT3DDEVICE9 device = Window::Instance()->Device();
	device->SetFVF(FVF_VERTEX_2D);
	device->SetStreamSource(0,GraphBG,0,sizeof(VERTEX));
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	device->SetStreamSource(0,UpdateGraph,0,sizeof(VERTEX));
	device->DrawPrimitive(D3DPT_LINESTRIP,0,29);

	device->SetStreamSource(0,DrawGraph,0,sizeof(VERTEX));
	device->DrawPrimitive(D3DPT_LINESTRIP,0,29);

#endif
}

void CDebugProc::SetUpdaeTime(float update,float draw)
{
	
	for (int cnt = 28;cnt <= 0;cnt--)
	{
		updatePoint[cnt].y = updatePoint[ cnt + 1 ].y;
		drawPoint[cnt].y = drawPoint[ cnt + 1 ].y;
	}
	float per = update / 16.6f;
	updatePoint[29].y = SCREEN_HEIGHT - 100.0f - (80.0f*per);
	per = draw / 16.6f;
	drawPoint[29].y = SCREEN_HEIGHT - 100.0f - (80.0f*per);
}