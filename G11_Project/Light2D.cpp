#include "Light2D.h"
#include "Shader2D.h"
#include "LightParticle.h"

LightScreen* LightScreen::Self = new LightScreen;

LightScreen::LightScreen()
{
	Surface = nullptr;
	Texture = nullptr;
	ResultSurface = nullptr;
	Result = nullptr;
	_Shader = nullptr;
}

bool LightScreen::Init(void)
{
	Window* window = Window::Instance();
	LPDIRECT3DDEVICE9 Device = window->Device();
	D3DXCreateTexture(Device,(UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Texture);
	D3DXCreateTexture(Device,(UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Result);
	Texture->GetSurfaceLevel(0,&Surface);
	Result->GetSurfaceLevel(0,&ResultSurface);

	_Shader = CShader2D::Instance();

	WorldMtx = _Shader->Identity();
	D3DXMATRIX scl,rot,trans;

	D3DXMatrixScaling(&scl,window->WindowSize().x,window->WindowSize().y,1.0f);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&scl);
	D3DXMatrixRotationYawPitchRoll(&rot,0,0,0);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&rot);
	D3DXMatrixTranslation(&trans,window->WindowSize().x / 2.0f,window->WindowSize().y/2.0f,0);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&trans);

	return true;
}

void LightScreen::Finalize(void)
{
	SafeRelease(Surface);
	SafeRelease(ResultSurface);
	SafeRelease(Texture);
	SafeRelease(Result);
	delete Self;
	Self = nullptr;
	LightParticle::ReleaseAll();
}

LPDIRECT3DTEXTURE9 LightScreen::Render(LPDIRECT3DTEXTURE9 tex)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	LPDIRECT3DSURFACE9 OldSurface;

	Device->GetRenderTarget(0,&OldSurface);
	Device->SetRenderTarget(0,Surface);

	//陽炎マップをクリアする
	Device->Clear(0L,nullptr,(D3DCLEAR_TARGET | D3DCLEAR_STENCIL),D3DCOLOR_ARGB(0,0,0,0),1.0f,0);


	LightParticle::DrawAll();
	_Shader->DrawBegin();


	Device->SetRenderTarget(0,ResultSurface);

	Device->Clear(0L,nullptr,(D3DCLEAR_TARGET | D3DCLEAR_STENCIL),D3DXCOLOR(0.0f,0,0,0),1.0f,0);

	_Shader->SetTexture(tex);
	_Shader->SetMask(Texture);
	D3DXCOLOR color = WHITE(1.0f);
	D3DXVECTOR4 uv(0,0,1.0f,1.0f);
	_Shader->SetMatrix(CShader2D::WORLD_MTX,WorldMtx);
	_Shader->SetFloatArray(CShader2D::DIFFUSE,color,4);
	_Shader->SetFloatArray(CShader2D::MASK_COLOR,color,4);
	_Shader->SetFloatArray(CShader2D::UV,uv,4);
	_Shader->SetFloatArray(CShader2D::MASK_UV,uv,4);

	_Shader->Commit();
	_Shader->Draw(CShader2D::ALPHA_MASK,D3DPT_TRIANGLESTRIP);
	
	_Shader->DrawEnd();

	//最初のサーフェイスに戻す
	Device->SetRenderTarget(0,OldSurface);

	//後始末
	if (OldSurface != nullptr)
	{
		OldSurface->Release();
		OldSurface = nullptr;
	}

	return Result;
}