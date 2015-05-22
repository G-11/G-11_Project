//
//Result.h
//
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"
#include "Scene.h"

class Sprite;

class Result :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	Sprite *BG;
	Sprite *Push;//

	float flahing, add_flahing_num;//ƒ¿’l,‰Á‚¦‚é’l 

};

#endif