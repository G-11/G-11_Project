#pragma once
#ifndef _MUTEX_H_
#define _MUTEX_H_

#include<Windows.h>

class Mutex
{
public:
	
	static void Finalize(void){ delete Self; }

	static Mutex* Instance(void){ return Self; }

	void Enter(void){ WaitForSingleObject(Handle,INFINITE); }
	void Leave(void){ ReleaseMutex(Handle); }

private:
	Mutex(){ Handle = CreateMutex(NULL,FALSE,NULL); }
	~Mutex(){ CloseHandle(Handle); }
	Mutex(const Mutex& other){};
	Mutex &operator= (const Mutex& other){};

	static Mutex* Self;

	HANDLE Handle;
};

#endif