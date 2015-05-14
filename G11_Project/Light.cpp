#include "Light.h"

CLight* CLight::Top=NULL;
CLight* CLight::Cur=NULL;
int CLight::LightNum = 0;
//=============================================================================
//自身をリストに追加
//=============================================================================
void CLight::LinkList(void)
{
	if(Top != NULL)//二つ目以降の処理
	{
		CLight* pLight = Cur;
		pLight->Next = this;
		Prev = pLight;
		Next = NULL;
		Cur = this;
	}
	else//最初の一つの時の処理
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
}
//=============================================================================
//自身をリストから削除
//=============================================================================
void CLight::UnlinkList(void)
{
	if(Prev == NULL)//先頭
	{
		if(Next != NULL)//次がある
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if(Next == NULL)//終端
	{
		if(Prev != NULL)//前がある
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;
}
//=============================================================================
//すべて削除
//=============================================================================
void CLight::ReleaseAll(void)
{
	CLight* pLight = Top;
	CLight* Next = NULL;
	while(pLight)
	{
		Next = pLight->Next;
		delete pLight;
		
		pLight = Next;
	}
}
//=============================================================================
//自身を削除
//=============================================================================
void CLight::Release(int index)
{
	CLight* pLight = Top;
	CLight* Next = NULL;
	int num=0;
	while(pLight)
	{
		Next = pLight->Next;
		if(num == index)
		{
			AllOff();//一度全てのライトを消す
			pLight->UnlinkList();
			pLight->Vector = D3DXVECTOR3(0,0,0);
			pLight->Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			pLight->Reset();//再設定する
			delete pLight;
			
			break;
		}
		num++;
		pLight = Next;
	}
	
}
//=============================================================================
//指向性ライトを作成
//引数: Vec 方向ベクトル
//		Color 色
//Create○○は作成したライトのポインタが返ってくる
//=============================================================================
CLight* CLight::CreateDirectional(D3DXVECTOR3 Vec,D3DXCOLOR color)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));

	pLight->Light.Type = D3DLIGHT_DIRECTIONAL;
	pLight->Light.Diffuse = color;
	pLight->Vector = Vec;

	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;
}
//=============================================================================
//ポイントライトを作成(スペキュラとアンビエントをディフューズと同じにする)
//引数:	D3DXVECTOR3	Pos		光源位置
//		float		Range	影響範囲
//		float		Attenuation	減衰係数	(0.0f～1.0f 1,0fで減衰なし)
//		D3DXCOLOR	diffuse		 色
//=============================================================================
CLight* CLight::CreatePoint(D3DXVECTOR3 Pos,float Range,float Attenuation,D3DXCOLOR diffuse)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));

	pLight->Light.Type = D3DLIGHT_POINT;
	pLight->Light.Diffuse = diffuse;
	pLight->Light.Ambient = diffuse;
	pLight->Light.Specular = diffuse;
	pLight->Light.Position = Pos;
	pLight->Light.Range = Range;
	pLight->Light.Attenuation0 = Attenuation;//定常減衰係数
	pLight->Light.Attenuation1 = 0.0f;		//線形減衰係数	AttenuationNは組み合わせ次第でいろいろ変化するらしい
	pLight->Light.Attenuation2 = 0.0f;		//平方減衰係数

	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;
}
//=============================================================================
//ポイントライトを作成(スペキュラとアンビエントを別個に設定する)
//引数:	D3DXVECTOR3	Pos		光源位置
//		float		Range	影響範囲
//		float		Attenuation	減衰係数	(0.0f～1.0f 1,0fで減衰なし)
//		D3DXCOLOR	diffuse		 拡散反射光色
//		D3DXCOLOR	Specular	 鏡面反射光色
//		D3DXCOLOR	Ambient		 環境光色
//=============================================================================
CLight* CLight::CreatePoint(D3DXVECTOR3 Pos,float Range,float Attenuation,D3DXCOLOR diffuse,D3DXCOLOR Specular,D3DXCOLOR Ambient)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));

	pLight->Light.Type = D3DLIGHT_POINT;
	pLight->Light.Diffuse = diffuse;
	pLight->Light.Ambient = Ambient;
	pLight->Light.Specular = Specular;
	pLight->Light.Position = Pos;
	pLight->Light.Range = Range;
	pLight->Light.Attenuation0 = Attenuation;
	pLight->Light.Attenuation1 = 0.0f;
	pLight->Light.Attenuation2 = 0.0f;

	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;
}
//=============================================================================
//スポットライトを作成(スペキュラとアンビエントをディフューズと同じに設定する)
//引数:	D3DXVECTOR3	Pos		光源位置
//		float		Range	影響範囲
//		float		Attenuation	減衰係数	(0.0f～1.0f 1,0fで減衰なし)
//		float		Theta	内側のコーンの角度	(0.0f～π)
//		float		Phi		外側のコーンの角度	(0.0f～π)
//		float		FallOff	内側から外側への減衰率(1.0f以外は処理速度の関係で非推奨)
//		D3DXCOLOR	diffuse		 拡散反射光色
//		D3DXCOLOR	Specular	 鏡面反射光色
//		D3DXCOLOR	Ambient		 環境光色
//=============================================================================
CLight* CLight::CreateSpot(D3DXVECTOR3 Vec,D3DXVECTOR3 Pos,float Range,float Attenuation,float Theta,float Phi,float FallOff,D3DXCOLOR diffuse)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));

	pLight->Vector = Vec;		//向き
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);

	pLight->Light.Type = D3DLIGHT_SPOT;
	pLight->Light.Diffuse = diffuse;
	pLight->Light.Ambient = diffuse;
	pLight->Light.Specular = diffuse;
	pLight->Light.Range = Range;
	pLight->Light.Attenuation0 = Attenuation;
	pLight->Light.Attenuation1 = 0.0f;
	pLight->Light.Attenuation2 = 0.0f;
	pLight->Light.Position = Pos;

	pLight->Light.Theta = Theta;//内側のコーンの角度
	pLight->Light.Phi = Phi;	//外側のコーンの角度
	pLight->Light.Falloff = FallOff;	//内側から外側に向かう際の減衰率
	
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;

}
//=============================================================================
//スポットライトを作成(スペキュラとアンビエントを別個に設定する)
//引数:	D3DXVECTOR3	Pos		光源位置
//		float		Range	影響範囲
//		float		Attenuation	減衰係数	(0.0f～1.0f 1,0fで減衰なし)
//		float		Theta	内側のコーンの角度	(0.0f～π)
//		float		Phi		外側のコーンの角度	(0.0f～π)
//		float		FallOff	内側から外側への減衰率(1.0f以外は処理速度の関係で非推奨)
//		D3DXCOLOR	diffuse		 拡散反射光色
//		D3DXCOLOR	Specular	 鏡面反射光色
//		D3DXCOLOR	Ambient		 環境光色
//=============================================================================
CLight* CLight::CreateSpot(D3DXVECTOR3 Vec,D3DXVECTOR3 Pos,float Range,float Attenuation,float Theta,float Phi,float FallOff,D3DXCOLOR diffuse,D3DXCOLOR Specular,D3DXCOLOR Ambient)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));
	
	pLight->Light.Type = D3DLIGHT_SPOT;
	pLight->Light.Diffuse = diffuse;
	pLight->Light.Ambient = Ambient;
	pLight->Light.Specular = Specular;
	pLight->Light.Range = Range;
	pLight->Light.Attenuation0 = Attenuation;
	pLight->Light.Attenuation1 = 0.0f;
	pLight->Light.Attenuation2 = 0.0f;
	pLight->Light.Position = Pos;

	pLight->Light.Theta = Theta;//内側のコーンの角度
	pLight->Light.Phi = Phi;	//外側のコーンの角度
	pLight->Light.Falloff = FallOff;	//内側から外側に向かう際の減衰率
	pLight->Vector = Vec;		//向き

	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;

}
//=============================================================================
//index番目のライトのスイッチを操作
//=============================================================================
void CLight::Swicth(int index,bool flag)
{
	CLight* pLight = Top;
	CLight* Next = NULL;
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	int num=0;
	while(pLight)
	{
		Next = pLight->Next;
		if(num == index)
		{
			BOOL Flag = FALSE;
			if(flag==true)
			{
				Flag = TRUE;
			}
			else
			{
				Flag = FALSE;
			}
			Device->LightEnable(index,Flag);
		}
		pLight = Next;
		num++;
	}
}
//=============================================================================
//Index番目のライトのインスタンスを取得
//=============================================================================
CLight* CLight::GetLight(int Index)
{
	CLight* pLight = Top;
	CLight* Next = NULL;
	int num=0;
	while(pLight)
	{
		Next = pLight->Next;
		if(num == Index)
		{
			return pLight;
		}
		num++;
		pLight = Next;
	}
	return NULL;
}
//=============================================================================
//再設定
//=============================================================================
void CLight::Reset(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = Top;
	int num=0;
	while(pLight)
	{
		Device->SetLight(num,&pLight->Light);
		num++;
		pLight = pLight->Next;
	}
}
//=============================================================================
//全部のライトを切る
//=============================================================================
void CLight::AllOff(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = Top;
	int num=0;
	while(pLight)
	{
		Device->LightEnable(num,FALSE);
		num++;
		pLight = pLight->Next;
	}
}
//=============================================================================
//光源位置を取得
//指向性ライトの場合は全部0が帰る
//=============================================================================
D3DXVECTOR3 CLight::GetPos(void)
{
	if(Light.Type != D3DLIGHT_DIRECTIONAL)
	{
		return Light.Position;
	}
	return D3DXVECTOR3(0,0,0);
}
//=============================================================================
//光の影響範囲の取得
//指向性ライトの場合は-1が帰る
//=============================================================================
float CLight::GetRange(void)
{
	if(Light.Type != D3DLIGHT_DIRECTIONAL)
	{
		return Light.Range;
	}
	return -1;
}
//=============================================================================
//光の影響範囲の設定
//=============================================================================
void CLight::SetRange(float range)
{
	Light.Range = range;
	Reset();
}
//=============================================================================
//色をセット
//=============================================================================
void CLight::SetColor(D3DXCOLOR color)
{
	Light.Diffuse = color;
	Reset();

}
//=============================================================================
//色を加算
//=============================================================================
void CLight::AddColor(D3DXCOLOR color)
{
	Light.Diffuse.r += color.r;
	Light.Diffuse.g += color.g;
	Light.Diffuse.b += color.b;
	Light.Diffuse.a += color.a;
	Reset();

}
//=============================================================================
//方向をセット
//=============================================================================
void CLight::SetVec(D3DXVECTOR3 Vec)
{
	Vector = Vec;
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction,&Vector);
	Reset();

}
//=============================================================================
//光源位置をセット
//=============================================================================
void CLight::SetPos(D3DXVECTOR3 Pos)
{
	Light.Position = Pos;
	Reset();
}
//=============================================================================
//光源位置を加算
//=============================================================================
void CLight::AddPos(D3DXVECTOR3 Pos)
{
	Light.Position.x += Pos.x;
	Light.Position.y += Pos.y;
	Light.Position.z += Pos.z;
	Reset();
}