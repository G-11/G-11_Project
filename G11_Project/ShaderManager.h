#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "main.h"

class Shader2D;
class Shader3D;
class Shimmer;
class Blur;

class ShaderManager
{
public:
	virtual ~ShaderManager(){};

	HRESULT Init(void);
	void Uninit(void);

	Shader2D* shader2D(void){ return _Shader2D; }
	Shader3D* shader3D(void){ return _Shader3D; }
	Shimmer* shimmer(void){ return _Shimmer; }
	Blur* BlurFilter(void){ return _BlurFilter; }

	static ShaderManager* Instance(void){return Self;}

private:
	ShaderManager();
	ShaderManager(const ShaderManager& other);
	ShaderManager &operator=(const ShaderManager& other){};

	static ShaderManager* Self;
	Shader2D* _Shader2D;
	Shader3D* _Shader3D;
	Shimmer* _Shimmer;
	Blur* _BlurFilter;

};

#endif