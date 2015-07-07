//
//Result.h
//
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"
#include "Scene.h"
#include "Item.h"

#define RESULTCHARMAX (6)
class Sprite;
class Gauge;


typedef struct 
{
	int id;
	float Score;
	bool vis;
	bool shadow;
}VIS_CHAR;


class Result :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetResultCharId(int id);
private:
	Sprite *BG;//背景
	Sprite *Push;//Pushボタン
	Sprite *REatan;//リザルト用いーたん
	Sprite *CharFream;//コレクション用キャラ枠

	Item *ResultChar[RESULTCHARMAX];//キャラクタ数


	Sprite *GaugeFream;//ゲージ外枠
	Gauge *GlowGauge;//ゲージ

	static VIS_CHAR ResultCharId[RESULTCHARMAX];//キャラの表示非表示
	float flahing, add_flahing_num;//α値,加える値 
	bool charFlag;//キャラクタを
	int charVisTime;//取得キャラ表示時間
	int charVisCnt;
	int gaugeCnt;//ゲージ用カウント数、全体に対し取得したアイテム分のゲージ上昇用
	int gaugeChangeColor;//ゲージの色変え用変数
	float Score;
	void ResetResultChar(void);
};

#endif