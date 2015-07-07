#pragma once
#ifndef _STAGE_H_
#define _STAGE_H_

typedef enum
{
	STAGE_MODE0 = 0,
	STAGE_MODE1,
	STAGE_MODE2,
	STAGE_MODE3,
	STAGE_MODEMAX
}STAGE_MODE;

class Gimmick;
class Sprite;
class Interface;
class Camera2D;

class Stage
{
protected:
	
public:

	Stage();
	virtual ~Stage();

	virtual void Init(void){};

	virtual void Release(void){};
	virtual void Update(void);

	Interface* GetInterface(){ return _Interface; }

protected:
	static Sprite	*_Field;
	Interface* _Interface;
	Camera2D* _Camera;
};

#endif