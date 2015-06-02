//
//Result.h
//
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"
#include "Scene.h"
#include "Item.h"
class Sprite;
class Gauge;


typedef struct 
{
	int id;
	bool vis;
}VIS_CHAR;


class Result :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetResultCharId(int id);
private:
	Sprite *BG;//背景
	Sprite *Push;//Pushボタン
	Sprite *REatan;//リザルト用いーたん
	Sprite *CharFream;//コレクション用キャラ枠

	Sprite *ResultChar[6];//キャラクタ数

	Sprite *GaugeFream;//ゲージ外枠
	Gauge *GlowGauge;//ゲージ

	static VIS_CHAR ResultCharId[6];//キャラの表示非表示
	float flahing, add_flahing_num;//α値,加える値 
	bool charFlag;//キャラクタを
	int charVisTime;//取得キャラ表示時間
	int charVisCnt;
	void ResetResultChar(void);

};

#endif