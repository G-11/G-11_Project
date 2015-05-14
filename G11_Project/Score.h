//=============================================================================
//スコアクラス
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Polygon.h"

//=============================================================================
//前方宣言
//=============================================================================
class CNumber;

//クラス定義
class CScore
{
public:
	static CScore* Create(D3DXVECTOR3 pos,D3DXVECTOR2 size,int CountPlace,bool camma = true);
	void SetNum(int Num){TargetScore = Num;}
	void SetCurrentNum(int num){ TargetScore = CurrentScore = num; }
	void AddNum(int Num){TargetScore += Num;}

	void SetPos(D3DXVECTOR3 pos){ Pos = DestPos = pos; }
	void SetDestPos(const D3DXVECTOR3& pos,float frame){DestPos = pos;MoveFrame = frame;}
	void SetMoveFrame(float frame){MoveFrame = frame;}
	void SetSize(D3DXVECTOR2 size);
	void SetColor(D3DXCOLOR color);

	void AddPos(D3DXVECTOR3 pos){ Pos += pos;DestPos = Pos; }
	void AddDestPos(const D3DXVECTOR3& pos){DestPos += pos;}
	void AddSize(D3DXVECTOR2 size);
	void AddColor(D3DXCOLOR color);

	D3DXVECTOR3 GetPos(void){ return Pos; }
	D3DXCOLOR GetColor(void){ return Color; }
	unsigned long GetNum(void){ return CurrentScore; }
	void Update(void);
	void Release(void);

private:
	int CountPlace;//桁数
	unsigned long CurrentScore;//現在値
	unsigned long TargetScore;//この値を目指して増減する
	unsigned long MaxScore;//表示可能な最大値
	float MoveFrame;
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 DestPos;
	D3DXVECTOR2 Size;
	D3DXCOLOR Color;
	CNumber* Number[20];
};

#endif