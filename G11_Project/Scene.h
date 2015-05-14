#ifndef _SCENE_H_
#define _SCENE_H_
#include "main.h"

class Scene
{
public:
	Scene(){frame=0;}
	virtual void Init(void){};
	virtual void Uninit(void){};
	virtual void Update(void){};
	virtual void Draw(void){};
	
protected:
	int frame;
};

#endif