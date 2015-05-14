#ifndef _SHIMMER_H_
#define _SHIMMER_H_
#include "main.h"
//===================================================
//陽炎クラス
// シングルトン
//===================================================

class CShimmer
{
public:

	HRESULT Init(void);
	LPDIRECT3DTEXTURE9 Render(LPDIRECT3DTEXTURE9 tex);
	static void Finalize(void);
	void SetFluctuation(float param){ Effect->SetFloat(Param[FLUCTUATION],param); }

	static CShimmer* Instance(void){ return Self; }
	void SetBlur(bool flag){ _BlurFlag = flag; }
	bool BlurFlag(void)const{ return _BlurFlag; }

private:
	CShimmer();
	~CShimmer();
	CShimmer(const CShimmer& other){};
	CShimmer &operator= (const CShimmer& other){};

	void ConductBlur(void);

	enum
	{
		FLUCTUATION = 0,
		SHIMMER_MAP,
		BACK_BUFFER,
		PARAM_MAX
	};

	const static char* ParamName[PARAM_MAX];

	static CShimmer* Self;

	D3DXHANDLE Param[PARAM_MAX];

	bool _BlurFlag;
	LPDIRECT3DSURFACE9 Surface;
	LPDIRECT3DSURFACE9 Blur[2];
	LPDIRECT3DSURFACE9 ResultSurface;

	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DTEXTURE9 BlurTex[2];
	LPDIRECT3DTEXTURE9 Result;
	LPD3DXEFFECT Effect;
};

#endif