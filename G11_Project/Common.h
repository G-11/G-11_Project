#pragma once
#ifndef _COMMON_H_
#define _COMMON_H_
#include "main.h"

typedef D3DXVECTOR2 vector2;
typedef D3DXVECTOR3 vector3;
typedef unsigned int uint;

D3DXVECTOR3 Vector3(const D3DXVECTOR2& vector2,float z);
D3DXVECTOR3 Vector3Zero(void);


#endif