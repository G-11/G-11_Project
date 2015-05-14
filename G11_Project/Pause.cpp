#include "Pause.h"
#include "Game.h"
#include "Sprite.h"
#include "Input/VC.h"
#include "Input/Keyboard.h"
#include "Manager.h"

bool CPause::PauseFlag = false;

CPause::CPause()
{

}

CPause::~CPause()
{

}

void CPause::Init(void)
{

}

void CPause::Uninit(void)
{

}

void CPause::Update(void)
{
	if (PauseFlag)
	{
	}
}

void CPause::SetPause(bool flag)
{
	PauseFlag = flag;
}