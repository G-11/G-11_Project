#pragma once
#ifndef _PAUSE_H_
#define _PAUSE_H_
#include "main.h"
#include "Scene.h"
#include "Item.h"
#define PAUSECHARMAX (6)
typedef struct
{
	int CharId;
	bool Charvisbleflag;
}PAUSECHARDATA;

class Scene;
class Pause : public Scene
{
public:
	Pause();
	~Pause();

	void Init(void);
	void Update(void);
	void Uninit(void);


	void PauseUpdate(void);

private:

	static PAUSECHARDATA PauseData[PAUSECHARMAX];
	static bool CharIdflag[PAUSECHARMAX];
	void ResetPauseChar(void);
	void ReCreate(void);

	Sprite *PauseChar[PAUSECHARMAX];//キャラ数
	Sprite *CharFream;//キャラ枠
	Sprite *ReturnGame;//ゲームに戻る
	Sprite *BackStage;//ステージに戻る
	Sprite *ReadOperate;//操作説明
	Sprite *Carsol;//選択用カーソル
	Sprite *BG;//背景
	int MenuNum;//メニュー選択用
	int OldItemNum;

	bool AlphaFlag;//α値をいじるためのフラグ
};

#endif