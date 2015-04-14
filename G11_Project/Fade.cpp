#include "Fade.h"
#include "Renderer.h"
#include "Shader2D.h"
#include "Camera2D.h"

CFade* CFade::Self = new CFade;

void CFade::Init(void)
{
	Color =
	DestColor =
	SubColor = D3DXCOLOR(0,0,0,1.0f);
	Texture = nullptr;
	_Mode = FADE_NONE;
	Fading = false;
	Size = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0);
	uv = D3DXVECTOR4(0,0,1.0f,1.0f);
	_Shader = Shader2D::Instance();
	D3DXMatrixIdentity(&WorldMtx);

}

void CFade::Uninit(void)
{
	if (Texture != nullptr)
	{
		Texture->Release();
		Texture = nullptr;
	}
}

void CFade::Finalize(void)
{
	delete Self;
}
void CFade::Update(void)
{
	if (_Mode == FADE_IN)
	{
		if (Color.a <= DestColor.a)
		{
			_Mode = FADE_NONE;
			Fading = false;
			SubColor = D3DXCOLOR(0,0,0,0);
		}
	}
	else if (_Mode != FADE_NONE)
	{
		if (Color.a >= DestColor.a)
		{
			(_Mode==FADE_INFINIT)? Fading = true :Fading = false;
			_Mode = FADE_NONE;
			SubColor = D3DXCOLOR(0,0,0,0);
		}
	}
	Color += SubColor;
}

void CFade::Draw(void)
{
	if (Fading)
	{
		CRenderer::SetStream2D();
		CCamera2D* camera = CCamera2D::GetCamera(0);
		WorldMtx = _Shader->Identity();
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + camera->GetPosP().x*0.265f,SCREEN_HEIGHT / 2 - camera->GetPosP().y*0.265f,0);
		D3DXMATRIX MtxScl,MtxRot,MtxTrans;
		D3DXMatrixScaling(&MtxScl,Size.x,Size.y,1.0f);
		D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxScl);

		D3DXMatrixTranslation(&MtxTrans,pos.x,pos.y,0);
		D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxTrans);

		_Shader->SetMatrix(Shader2D::WORLD_MTX,WorldMtx);

		//_Shader->SetMatrix(Shader2D::POS_MTX,MtxTrans);
		//_Shader->SetFloatArray(Shader2D::SIZE,Size,3);

		//テクスチャの設定
		_Shader->SetTexture(Texture);
		_Shader->SetFloatArray(Shader2D::DIFFUSE,Color,4);
		_Shader->SetFloatArray(Shader2D::UV,uv,4);
		//ポリゴンを描画
		_Shader->DrawBegin();
		_Shader->Draw(Shader2D::NORMAL,D3DPT_TRIANGLESTRIP);
		_Shader->DrawEnd();
		
	}
}

void CFade::Start(float frame,FADE mode,const D3DXCOLOR &color)
{
	_Mode = mode;
	DestColor = color;
	if (mode == FADE_IN)
	{
		Color.a = 1.0f;
	}
	else
	{
		Color.a = 0.0f;
	}
	
	SubColor = (DestColor-Color);
	SubColor /= frame;
	Fading = true;
}