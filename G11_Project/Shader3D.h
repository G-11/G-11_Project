#ifndef _SHADER_3D_H_
#define _SHADER_3D_H_

#include "main.h"
//===================================================
//3Dシェーダークラス
// シングルトン
//===================================================
class CShader3D
{
public:

	typedef enum
	{
		PROJECTION = 0,
		VIEW_PORT,
		VIEW_MTX,
		INV_VIEW_MTX,
		SCL_MTX,
		ROT_MTX,
		POS_MTX,
		LIGHT_VEC,
		AMBIENT,
		DIFFUSE,
		LIGHT_COLOR,
		SIZE,
		UV,
		TEXTURE,
		MASK,
		STATE_MAX
	}STATE;

	typedef enum
	{
		NORMAL = 0,
		ADD,
		SUB,
		NOLIGHT,
		BILLBOARD,
		ADD_EFFECT,
		SUB_EFFECT,
		TEX_MAX
	}PASS;

	HRESULT Init(void);
	void Uninit(void);

	void DrawBegin(void){ Effect->Begin(&Pass,0); }
	void DrawEnd(void){ Effect->End(); }

	void BeginPass(PASS pass){ Effect->BeginPass(pass); }
	void EndPass(void){ Effect->End(); }

	void Commit(void){ Effect->CommitChanges(); }
	void Draw(PASS pass,_D3DPRIMITIVETYPE pt,int PrimitiveNum = 2,int offset = 0);

	void SetFloat(STATE param,float f)				{ Effect->SetFloat(State[param],f); }
	void SetFloatArray(STATE param,float* f,int num){ Effect->SetFloatArray(State[param],f,num); }
	void SetMatrix(STATE param,D3DXMATRIX mtx)		{ Effect->SetMatrix(State[param],&mtx); }
	void SetTexture(LPDIRECT3DTEXTURE9 tex)			{ Effect->SetTexture(State[TEXTURE],tex); }
	void SetMask(LPDIRECT3DTEXTURE9 tex)			{ Effect->SetTexture(State[MASK],tex); }

	static CShader3D* Instance(void){ return Self; }

private:

	const static LPSTR ParamName[];

	CShader3D();
	CShader3D(const CShader3D& other){};
	CShader3D &operator=(const CShader3D& other){};

	static CShader3D* Self;
	D3DXHANDLE State[STATE_MAX];
	D3DXHANDLE Technique[TEX_MAX];
	LPD3DXEFFECT Effect;
	UINT Pass;

};
#endif