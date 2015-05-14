//===========================================================
//リスト作成用テンプレート関数
//主にあたり判定用リストの作成と破棄
//Creater:Arai Yuhki
//===========================================================
#ifndef _LIST_H_
#define _LIST_H_

template<class T>
class CList
{
public:
	static void LinkHitList(T* This);
	static void UnlinkHitList(T* This);


};
/*使用例
class CObject
{
protected:(もしくはpublic:)
	static CObject* HitTop;	//これらの宣言が必須
	static CObject* HitCur;
	CObject* HitNext;
	CObject* HitPrev;
};
	CObject* pObject = new CObject;
	CList<CObject>::LinkList(pObject);

*/

//--------------------------------------------------------
//あたり判定用リストに追加
//型:引数に渡す変数のクラス型
//引数 追加したいクラスオブジェクトのポインタ
//--------------------------------------------------------
template<class T>void CList<T>::LinkHitList(T* This)
{
	if(This->HitTop != NULL)//二つ目以降の処理
	{
		T* pObj = This->HitCur;
		pObj->HitNext = This;
		This->HitPrev = pObj;
		This->HitNext = NULL;
		This->HitCur = This;
	}
	else//最初の一つの時の処理
	{
		This->HitTop = This;
		This->HitCur = This;
		This->HitPrev = NULL;
		This->HitNext = NULL;
	}
}
//--------------------------------------------------------
//あたり判定用リストから
//型:引数に渡す変数のクラス型
//引数 削除したいクラスオブジェクトのポインタ
//--------------------------------------------------------
template<class T>void CList<T>::UnlinkHitList(T* This)
{
	if(This==NULL)
	{
		return;
	}
	if(This->HitPrev == NULL)//先頭
	{
		if(This->HitNext != NULL)//次がある
		{
			This->HitNext->HitPrev = NULL;
			This->HitTop = This->HitNext;
		}
		else//最後の一つだった
		{
			This->HitTop = NULL;
			This->HitCur = NULL;
		}
	}
	else if(This->HitNext == NULL)//終端
	{
		if(This->HitPrev != NULL)//前がある
		{
			This->HitPrev->HitNext = NULL;
			This->HitCur = This->HitPrev;
		}
		else//最後の一つだった
		{
			This->HitTop = NULL;
			This->HitCur = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		This->HitPrev->HitNext = This->HitNext;
		This->HitNext->HitPrev = This->HitPrev;
	}

	This->HitPrev = NULL;
	This->HitNext = NULL;

}
#endif