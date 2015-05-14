//
//Result.h
//
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"
#include "Scene.h"

class Result :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


};

#endif