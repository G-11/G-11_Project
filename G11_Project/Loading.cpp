#include "Loading.h"
#include "Mutex.h"
#include "Renderer.h"
#include "Shader2D.h"
#include "Camera2D.h"

CLoading* CLoading::Self = new CLoading;
CScreenRender* CLoading::ScreenRender = nullptr;

CLoading::CLoading()
{
	Texture = nullptr;
}

CLoading::~CLoading()
{

}

void CLoading::Init(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	if (FAILED(D3DXCreateTextureFromFile(Device,"data/texture/Load.png",&Texture)))
	{
		MessageBox(NULL,"ロード画面のテクスチャの生成に失敗しました","ERROR",MB_OK | MB_ICONERROR);
		return ;
	}
	float offsetU = 1.0f/3.0f;
	for (int cnt = 0;cnt < PolygonNum;cnt++)
	{
		_Pos[cnt].x = SCREEN_WIDTH - (100.0f*(cnt+1));
		_Pos[cnt].y = SCREEN_HEIGHT - 100.0f;
		_Pos[cnt].z = 0;
		_Size[cnt] = D3DXVECTOR3(150.0f,150.0f,1.0f);
		_Rot[cnt] = 0;
		uv[cnt] = D3DXVECTOR4(offsetU*(cnt+1),0,offsetU,1.0f);
	}
	frame = 0;
	mutex = Mutex::Instance();
	QueryPerformanceFrequency(&sys);
	QueryPerformanceCounter(&start);
}

void CLoading::Uninit(void)
{
	SafeRelease(Texture);
}

void CLoading::Finalize(void)
{
	Self->Uninit();

	SafeDelete(Self);
}

void CLoading::Update(void)
{
	if (ScreenRender != nullptr)
	{
		QueryPerformanceCounter(&now);
		double updateTime = (double)(now.QuadPart - start.QuadPart)*1000 / (double)sys.QuadPart;
		if (updateTime >= 15.0f)
		{
			LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
			Camera2D::GetCamera(0)->SetNoMove();
			CShader2D* shader = CShader2D::Instance();

			CastMatrix();
			Renderer::SetStream2D();

			mutex->Enter();

			if (SUCCEEDED(Device->BeginScene()))	//描画開始
			{
				Device->Clear(0L,nullptr,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0x0,1.0f,0L);
				shader->SetTexture(Texture);
				shader->SetFloatArray(CShader2D::UV,D3DXVECTOR4(0,0,1.0f,1.0f),4);
				shader->SetFloatArray(CShader2D::DIFFUSE,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),4);
				shader->DrawBegin();
				for (int cnt = 0;cnt < PolygonNum;cnt++)
				{
					_Pos[cnt].y = sinf(DEG2RAD(frame + (cnt*90.0f)))*20.0f + SCREEN_HEIGHT - 100.0f;
					_Rot[cnt] = cosf(DEG2RAD(frame + cnt*20.0f))*PI / 10.0f;
					shader->SetMatrix(CShader2D::WORLD_MTX,WorldMatrix[cnt]);
					shader->SetFloatArray(CShader2D::UV,uv[cnt],4);
					shader->Draw(CShader2D::NORMAL,D3DPT_TRIANGLESTRIP);
				}
				shader->DrawEnd();
				Device->EndScene();
			}
			Device->Present(NULL,NULL,NULL,NULL);
			mutex->Leave();
			frame += 6.0f;
			start = now;
		}
	}
}

void CLoading::SetRenderer(CScreenRender* sr)
{
	ScreenRender = sr;
}

void CLoading::CastMatrix(void)
{
	CShader2D* shader = CShader2D::Instance();
	D3DXMATRIX SclMtx,RotMtx,TransMtx;

	for (int cnt = 0;cnt < PolygonNum;cnt++)
	{
		WorldMatrix[cnt] = shader->Identity();

		D3DXMatrixScaling(&SclMtx,_Size[cnt].x,_Size[cnt].y,_Size[cnt].z);
		D3DXMatrixRotationYawPitchRoll(&RotMtx,0,0,_Rot[cnt]);
		D3DXMatrixTranslation(&TransMtx,_Pos[cnt].x,_Pos[cnt].y,_Pos[cnt].z);
		D3DXMatrixMultiply(&WorldMatrix[cnt],&WorldMatrix[cnt],&SclMtx);
		D3DXMatrixMultiply(&WorldMatrix[cnt],&WorldMatrix[cnt],&RotMtx);
		D3DXMatrixMultiply(&WorldMatrix[cnt],&WorldMatrix[cnt],&TransMtx);
		
	}
}