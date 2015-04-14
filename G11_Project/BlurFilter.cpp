#include "BlurFilter.h"

Blur* Blur::Self = new Blur;

Blur::Blur()
{
	for (int cnt = 0;cnt < 2;cnt++)
	{
		
		Texture[cnt] = nullptr;	
		Surface[cnt] = nullptr;
	}
}

Blur::~Blur()
{
	for (int cnt = 0;cnt < 2;cnt++)
	{
		if (Texture[cnt] != nullptr)
		{
			Texture[cnt]->Release();
			Texture[cnt] = nullptr;
		}

		if (Surface[cnt] != nullptr)
		{
			Surface[cnt]->Release();
			Surface[cnt] = nullptr;
		}
	}
}

void Blur::Init(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	
	LPD3DXBUFFER pError = NULL;

#ifdef _DEBUG
	if (FAILED(D3DXCreateEffectFromFile(Device,"data/shaders/Blur.fx",NULL,NULL,D3DXSHADER_SKIPVALIDATION,NULL,&Effect,&pError)))
	{
		MessageBox(nullptr,"ƒuƒ‰[ƒtƒBƒ‹ƒ^[¶¬Ž¸”s","Œx",MB_OK);
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		pError->Release();
		pError = NULL;
		return ;
	}
#else
	if (FAILED(D3DXCreateEffectFromFile(Device,"data/shaders/Blur.cso",NULL,NULL,D3DXSHADER_SKIPVALIDATION,NULL,&Effect,&pError)))
	{
		MessageBox(nullptr,"ƒuƒ‰[ƒtƒBƒ‹ƒ^[¶¬Ž¸”s","Œx",MB_OK);
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		pError->Release();
		pError = NULL;
		return ;
	}
#endif

	Projection2D = D3DXMATRIX(
		2.0f,0,0,0,
		0,-2.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1.0f);
	Effect->SetMatrix("Projection2D",&Projection2D);
	
	float texel_x[5] = { 0 };
	float texel_y[5] = { 0 };
	for (int cnt = 0;cnt < 5;cnt++)
	{
		texel_x[cnt] = (1.0f / SCREEN_WIDTH) * (cnt + 1);
		texel_y[cnt] = (1.0f / SCREEN_HEIGHT) * (cnt + 1);
	}
	Effect->SetFloatArray("TexelU",texel_x,5);
	Effect->SetFloatArray("TexelV",texel_y,5);
	Effect->SetTechnique("Blur");

	Effect->GetParameterByName(DiffuseMap,"DiffuseMap");

	for (int cnt = 0;cnt < 2;cnt++)
	{
		Device->CreateTexture((UINT)SCREEN_WIDTH,(UINT)SCREEN_HEIGHT,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Texture[cnt],NULL);
		Texture[cnt]->GetSurfaceLevel(0,&Surface[cnt]);
	}
}

void Blur::Finalize(void)
{
	if (Self != nullptr)
	{
		delete Self;
		Self = nullptr;
	}
}


LPDIRECT3DTEXTURE9 Blur::Render(LPDIRECT3DTEXTURE9 tex)
{
	LPDIRECT3DSURFACE9 OldSurface = nullptr;
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	
	Device->GetRenderTarget(0,&OldSurface);
	Effect->SetTexture(DiffuseMap,tex);
	Device->SetRenderTarget(0,Surface[0]);
	Device->Clear(0L,nullptr,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0x0,1.0f,0L);

	Effect->Begin(0,0);
	Effect->BeginPass(0);
	//•`‰æ
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	Effect->EndPass();
	Effect->End();

	Device->SetRenderTarget(0,Surface[1]);
	Device->Clear(0L,nullptr,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0x0,1.0f,0L);

	Effect->SetTexture(0,Texture[0]);
	Effect->Begin(0,0);
	Effect->BeginPass(1);
	//•`‰æ
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	Effect->EndPass();
	Effect->End();

	Device->SetRenderTarget(0,OldSurface);

	if (OldSurface!=nullptr)
	{
		OldSurface->Release();
		OldSurface = nullptr;
	}

	return Texture[1];
}