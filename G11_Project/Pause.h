#pragma once
#ifndef _PAUSE_H_
#define _PAUSE_H_

class Sprite;
class CPause
{
public:
	CPause();
	~CPause();

	void Init(void);
	void Update(void);
	void Uninit(void);

	static void SetPause(bool flag);

private:
	static bool PauseFlag;

};

#endif