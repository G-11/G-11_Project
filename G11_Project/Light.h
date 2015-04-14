#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "main.h"
class Light
{
private:
	static Light* Top;
	static Light* Cur;
	Light* Next;
	Light* Prev;

	void LinkList(void);
	void UnlinkList(void);
	static void AllOff(void);
	void Reset(void);

protected:
	D3DLIGHT9 _Light;
	D3DXVECTOR3 Vector;
	D3DXCOLOR Color;
	bool Switch;
	static int LightNum;

public:
	Light(){LinkList();}
	virtual ~Light(){LightNum--;}
	static Light* CreateDirectional(D3DXVECTOR3 Vec,D3DXCOLOR color);
	static Light* CreatePoint(D3DXVECTOR3 Pos,float Range,float Attenuation,D3DXCOLOR diffuse);
	static Light* CreatePoint(D3DXVECTOR3 Pos,float Range,float Attenuation,D3DXCOLOR diffuse,D3DXCOLOR Specular,D3DXCOLOR Ambient);
	static Light* CreateSpot(D3DXVECTOR3 Vec,D3DXVECTOR3 Pos,float Range,float Attenuation,float Theta,float Phi,float FallOff,D3DXCOLOR diffuse);
	static Light* CreateSpot(D3DXVECTOR3 Vec,D3DXVECTOR3 Pos,float Range,float Attenuation,float Theta,float Phi,float FallOff,D3DXCOLOR diffuse,D3DXCOLOR Specular,D3DXCOLOR Ambient);
	static void ReleaseAll(void);
	static void Release(int index);
	static void Swicth(int index,bool flag);

	D3DXCOLOR GetColor(void){return Color;}
	D3DXVECTOR3 GetVec(void){return Vector;}
	static Light* GetLight(int Index);
	static int GetNum(void){return LightNum;}
	D3DXVECTOR3 GetPos(void);
	float GetRange(void);
	void SetRange(float range);
	D3DLIGHTTYPE GetType(void){return _Light.Type;}
	void SetColor(D3DXCOLOR color);
	void AddColor(D3DXCOLOR color);
	void SetVec(D3DXVECTOR3 Vec);
	void SetPos(D3DXVECTOR3 Pos);
	void AddPos(D3DXVECTOR3 Pos);
};

#endif