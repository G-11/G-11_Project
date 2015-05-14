//=============================================================================
//数字表示クラス
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Sprite.h"

//クラス定義
class Number:public Sprite
{
public:
	Number(int priority = 1):Sprite(priority){}
	~Number(){}
	static Number* Create(D3DXVECTOR3 pos,D3DXVECTOR2 size,int num = 0,int priority = 0);
	void SetNum(int num);
	int Num(void){ return _Number; }
private:
	int _Number;
};

#endif