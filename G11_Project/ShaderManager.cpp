#include "ShaderManager.h"
#include "Shader2D.h"
#include "Shader3D.h"
#include "Shimmer.h"
#include "BlurFilter.h"

ShaderManager* ShaderManager::Self = new ShaderManager;

ShaderManager::ShaderManager()
{
	_Shader2D = nullptr;
}
HRESULT ShaderManager::Init(void)
{

	_BlurFilter = Blur::Instance();
	_BlurFilter->Init();

	_Shader2D = Shader2D::Instance();
	if (FAILED(_Shader2D->Init()))
	{
		return E_FAIL;

	}
	_Shader3D = Shader3D::Instance();

	if (FAILED(_Shader3D->Init()))
	{
		Uninit();
		return E_FAIL;
	}

	_Shimmer = Shimmer::Instance();
	if (FAILED(_Shimmer->Init()))
	{
		Uninit();
		return E_FAIL;
	}
	

	return S_OK;
}

void ShaderManager::Uninit(void)
{
	if (_Shader2D != nullptr)
	{
		_Shader2D->Uninit();
		_Shader2D = nullptr;
	}

	if (_Shader3D != nullptr)
	{
		_Shader3D->Uninit();
		_Shader3D = nullptr;
	}

	if (_Shimmer != nullptr)
	{
		_Shimmer->Finalize();
		_Shimmer = nullptr;
	}

	if (_BlurFilter != nullptr)
	{
		_BlurFilter->Finalize();
		_BlurFilter = nullptr;
	}

	Self = nullptr;
	delete this;
}