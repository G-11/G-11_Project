#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "main.h"

//===================================================
//シェーダーマネージャークラス
// シングルトン
//===================================================

class CShader2D;
class CShader3D;
class CShimmer;
class CBlurFilter;
class LightScreen;

class ShaderManager
{
public:
	virtual ~ShaderManager(){};

	HRESULT Init(void);
	void Uninit(void);

	CShader2D* Shader2D(void){ return _Shader2D; }
	CShader3D* Shader3D(void){ return _Shader3D; }
	CShimmer* Shimmer(void){ return _Shimmer; }
	CBlurFilter* BlurFilter(void){ return _BlurFilter; }
	LightScreen* lightScreen(void){ return _LightScreen; }

	static ShaderManager* Instance(void)
	{
		if (Self == nullptr)
		{
			Self = new ShaderManager;
		}
		return Self;
	}

private:
	ShaderManager();
	ShaderManager(const ShaderManager& other);
	ShaderManager &operator=(const ShaderManager& other){};

	static ShaderManager* Self;
	CShader2D* _Shader2D;
	CShader3D* _Shader3D;
	CShimmer* _Shimmer;
	CBlurFilter* _BlurFilter;
	LightScreen* _LightScreen;

};

#endif