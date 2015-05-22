//=============================================================================
// マネージャークラス
// CreateBy Arai Yuhki
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
//=============================================================================
// 前方宣言
//=============================================================================
class Renderer;
class CPlayer;
class Scene;
class CDebugProc;

typedef void (*Function)(void);
//=============================================================================
// クラス定義
//=============================================================================
class Manager
{
public:

	typedef enum
	{
		SCENE_TITLE = 0,
		SCENE_OPTION,
		SCENE_GAME,
		SCENE_RESULT,
		SCENE_MAX
	}SCENE;

	static void Initialize(void);
	static void Finalize(void);
	HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWinfow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static Renderer* GetRenderer(void){return _Render;}
	void SetScene(SCENE next);
	static void ReleaseObject(void);
	static void SetPause(bool flag);

	static Manager* Instance(void){ return Self; }

private:

	Manager();
	Manager(const Manager& other){}
	~Manager(){};
	Manager &operator= (const Manager& other){};

	static Renderer* _Render;

	void Control(void);
	void ChangeScene(void);
	
	Scene* Scene;

	SCENE Current;
	SCENE Next;
	bool SceneChangeFlag;

	static CDebugProc* Debug;
	static Manager* Self;

	static unsigned __stdcall Thread(void*);
	static unsigned __stdcall SceneInit(void*);
};
#endif