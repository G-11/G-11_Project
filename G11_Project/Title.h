//
//Title.h
//
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"
#include "Scene.h"
class Sprite;
class Eatan;

class Title :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

	Sprite *Bg;//背景

	Sprite *Usagi;//ウサギキャラ
	Sprite *Kuma;//熊キャラ
	Sprite *Neko;//ネコキャラ
	Eatan *TEatan;//いーたん

	Sprite *Logo;//点滅用画像
	Sprite *Start;//
	Sprite *Tutorial;
	Sprite *Collection;
	Sprite *Config;

	
	float Flahing,AddFlashingNum,RotFream;//α値,加える値,左右ゆれ用
	int MenuNum;//メニュー選択用


	float test_num;


};

#endif