//
//Title.h
//
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"
#include "Scene.h"
class Sprite;

class Title :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	Sprite *m_logo;//点滅用画像
	float flahing,add_flahing_num;//α値,加える値

};

#endif