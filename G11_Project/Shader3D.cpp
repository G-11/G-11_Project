#include "Shader3D.h"

CShader3D* CShader3D::Self = new CShader3D;

const LPSTR CShader3D::ParamName[CShader3D::STATE_MAX] =
{
	"Projection",
	"ViewPort",
	"ViewMtx",
	"InvViewMtx",
	"Scl",
	"Rot",
	"Pos",
	"LightVec",
	"Ambient",
	"Color",
	"LightColor",
	"Size",
	"uv",
	"DiffuseMap",
	"MaskMap",
};

CShader3D::CShader3D()
{
	Effect = nullptr;
}
HRESULT CShader3D::Init(void)
{
	LPD3DXBUFFER Error = NULL;
#ifdef _DEBUG
	if (FAILED(D3DXCreateEffectFromFile(Window::Instance()->Device(),"data/shaders/Shader3D.fx",NULL,NULL,D3DXSHADER_DEBUG,NULL,&Effect,&Error)))
	{
		MessageBox(NULL,"3Dエフェクトを作成できませんでした","ERROR!",MB_OK | MB_ICONERROR);
		OutputDebugStringA((LPCSTR)Error->GetBufferPointer());
		Error->Release();
		Error = NULL;
		return E_FAIL;
	}
#else
	if (FAILED(D3DXCreateEffectFromFile(Window::Instance()->Device(),"data/shaders/Shader3D.cso",NULL,NULL,D3DXSHADER_SKIPVALIDATION,NULL,&Effect,&Error)))
	{
		MessageBox(NULL,"3Dエフェクトを作成できませんでした","ERROR!",MB_OK | MB_ICONERROR);
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

	D3DXMATRIX scl;
	D3DXMatrixScaling(&scl,1.0f,1.0f,1.0f);
	Effect->SetMatrix(State[SCL_MTX],&scl);
	Effect->SetTechnique("Polygon3D");

	return S_OK;
}

void CShader3D::Uninit(void)
{
	SafeRelease(Effect);

	Self = nullptr;
	delete this;
}

void CShader3D::Draw(PASS pass,_D3DPRIMITIVETYPE pt,int PrimitiveNum,int offset)
{
	Effect->BeginPass(pass);
	Window::Instance()->Device()->DrawPrimitive(pt,offset,PrimitiveNum);
	Effect->EndPass();
}