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
	if (FAILED(D3DXCreateTextureFromFile(Device,"data/texture/LoadGear.png",&Texture)))
	{
		MessageBox(NULL,"ロード画面のテクスチャの生成に失敗しました","ERROR",MB_OK | MB_ICONERROR);
		return ;
	}

	_Pos[0].x = SCREEN_WIDTH - 200.0f;
	_Pos[0].y = SCREEN_HEIGHT - 200.0f;
	_Pos[1].x = SCREEN_WIDTH - 100.0f;
	_Pos[1].y = SCREEN_HEIGHT - 100.0f;

	_Pos[0].z = _Pos[1].z = 0;

	_Size[0] = D3DXVECTOR3(200.0f,200.0f,1.0f);
	_Size[1] = _Size[0] * 0.8f;
	_Rot[0] = 0;
	_Rot[1] = DEG2RAD(15.0f);
	mutex = Mutex::Instance();
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
		LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
		Camera2D::GetCamera(0)->SetNoMove();
		CShader2D* shader = CShader2D::Instance();
		CastMatrix();
		Renderer::SetStream2D();

		_Rot[0] += DEG2RAD(0.1f);
		_Rot[1] -= DEG2RAD(0.1f);
		mutex->Enter();

		if (SUCCEEDED(Device->BeginScene()))	//描画開始
		{
			Device->Clear(0L,nullptr,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0x0,1.0f,0L);
			shader->SetTexture(Texture);
			shader->SetFloatArray(CShader2D::UV,D3DXVECTOR4(0,0,1.0f,1.0f),4);
			shader->SetFloatArray(CShader2D::DIFFUSE,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),4);
			shader->DrawBegin();
			for (int cnt = 0;cnt < 2;cnt++)
			{
				shader->SetMatrix(CShader2D::WORLD_MTX,WorldMatrix[cnt]);
				shader->Draw(CShader2D::NORMAL,D3DPT_TRIANGLESTRIP);
			}
			shader->DrawEnd();
			//ScreenRender->RenderWindow(Texture);
			Device->EndScene();
		}
		Device->Present(NULL,NULL,NULL,NULL);
		mutex->Leave();
	}
}

void CLoading::SetRenderer(CScreenRender* sr)
{
	ScreenRender = sr;
}

void CLoading::CastMatrix(void)
{
	CShader2D* shader = CShader2D::Instance();
	WorldMatrix[0] = WorldMatrix[1] = shader->Identity();
	D3DXMATRIX SclMtx,RotMtx,TransMtx;

	for (int cnt = 0;cnt < 2;cnt++)
	{
		D3DXMatrixScaling(&SclMtx,_Size[cnt].x,_Size[cnt].y,_Size[cnt].z);
		D3DXMatrixRotationYawPitchRoll(&RotMtx,0,0,_Rot[cnt]);
		D3DXMatrixTranslation(&TransMtx,_Pos[cnt].x,_Pos[cnt].y,_Pos[cnt].z);
		D3DXMatrixMultiply(&WorldMatrix[cnt],&WorldMatrix[cnt],&SclMtx);
		D3DXMatrixMultiply(&WorldMatrix[cnt],&WorldMatrix[cnt],&RotMtx);
		D3DXMatrixMultiply(&WorldMatrix[cnt],&WorldMatrix[cnt],&TransMtx);
		
	}
}