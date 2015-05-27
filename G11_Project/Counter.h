//=============================================================================
//スコアクラス
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Sprite.h"

//=============================================================================
//前方宣言
//=============================================================================
class Number;

//クラス定義
class Counter
{
public:
	static Counter* Create(const D3DXVECTOR3& pos,const D3DXVECTOR2& size,int CountPlace,bool camma = true);
	void SetNum(int Num){TargetScore = Num;}
	void SetCurrentNum(int num){ TargetScore = CurrentScore = num; }
	void AddNum(int Num){TargetScore += Num;}

	void SetPos(const D3DXVECTOR3& pos){ _Pos = DestPos = pos; }
	void SetDestPos(const D3DXVECTOR3& pos,float frame){DestPos = pos;MoveFrame = frame;}
	void SetMoveFrame(float frame){MoveFrame = frame;}
	void SetSize(const D3DXVECTOR2& size);
	void SetColor(const D3DXCOLOR& color);

	void AddPos(const D3DXVECTOR3& pos){ _Pos += pos;DestPos = _Pos; }
	void AddDestPos(const D3DXVECTOR3& pos){DestPos += pos;}
	void AddSize(const D3DXVECTOR2& size);
	void AddColor(const D3DXCOLOR& color);

	D3DXVECTOR3 Pos(void){ return _Pos; }
	D3DXCOLOR Color(void){ return _Color; }
	int GetNum(void){ return CurrentScore; }
	void Update(void);
	void Release(void);

private:
	int CountPlace;//桁数
	int CurrentScore;//現在値
	int TargetScore;//この値を目指して増減する
	int MaxScore;//表示可能な最大値
	float MoveFrame;
	D3DXVECTOR3 _Pos;
	D3DXVECTOR3 DestPos;
	D3DXVECTOR2 Size;
	D3DXCOLOR _Color;
	Number* number[20];
};

#endif