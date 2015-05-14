#include "Shader2D.h"

CShader2D* CShader2D::Self = new CShader2D;

const LPSTR CShader2D::ParamName[CShader2D::STATE_MAX] =
{
	"Projection",
	"ViewMtx",
	"ViewPort",
	"WorldMtx",
	"Scl",
	"Rot",
	"Pos",
	"Offset",
	"Diffuse",
	"MaskColor",
	"UV",
	"MaskUV",
	"DiffuseMap",
	"Mask",
};

CShader2D::CShader2D()
{
	Effect = nullptr;
}
HRESULT CShader2D::Init(void)
{
	LPD3DXBUFFER Error = NULL;
#ifdef _DEBUG
	if (FAILED(D3DXCreateEffectFromFile(Window::Instance()->Device(),"data/shaders/Shader2D.fx",NULL,NULL,D3DXSHADER_DEBUG,NULL,&Effect,&Error)))
	{
		MessageBox(NULL,"2Dエフェクトを作成できませんでした","ERROR!",MB_OK|MB_ICONERROR);
		OutputDebugStringA((LPCSTR)Error->GetBufferPointer());
		Error->Release();
		Error = NULL;
		return E_FAIL;
	}
#else
	if (FAILED(D3DXCreateEffectFromFile(Window::Instance()->Device(),"data/shaders/Shader2D.cso",NULL,NULL,D3DXSHADER_SKIPVALIDATION,NULL,&Effect,&Error)))
	{
		MessageBox(NULL,"2Dエフェクトを作成できませんでした","ERROR!",MB_OK | MB_ICONERROR);
		OutputDebugStringA((LPCSTR)Error->GetBufferPointer());
		Error->Release();
		Error = NULL;
		return E_FAIL;
	}
#endif
	char error[256] = { 0 };
	for (int cnt = 0;cnt < STATE_MAX;cnt++)
	{
		State[cnt] = Effect->GetParameterByName(NULL,ParamName[cnt]);
		if (State[cnt] == NULL)
		{
			sprintf_s(error,"%sが見つかりません",ParamName[cnt]);
			OutputDebugStringA(error);
		}
	}

	D3DXMatrixIdentity(&MtxIdentity);

	D3DXMATRIX scl;
	D3DXMatrixScaling(&scl,1.0f,1.0f,1.0f);
	Effect->SetMatrix(State[SCL_MTX],&scl);
	Effect->SetTechnique("Polygon2D");

	return S_OK;
}

void CShader2D::Uninit(void)
{
	SafeRelease(Effect);

	Self = nullptr;
	delete this;
}

void CShader2D::Draw(PASS pass,_D3DPRIMITIVETYPE pt,int PrimitiveNum,int offset)
{
	Effect->BeginPass(pass);
	Window::Instance()->Device()->DrawPrimitive(pt,offset,PrimitiveNum);
	Effect->EndPass();
}