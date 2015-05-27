#include "Fade.h"
#include "Renderer.h"
#include "Shader2D.h"
#include "Camera2D.h"

Fade* Fade::Self = new Fade;

void Fade::Init(void)
{
	for (int cnt = 0;cnt < 2;cnt++)
	{
		Color[cnt] =
		DestColor[cnt] =
		SubColor[cnt] = D3DXCOLOR(0,0,0,1.0f);
		Texture = nullptr;
		_Mode[cnt] = FADE_NONE;
		Fading[cnt] = false;
	}
	Size = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0);
	uv = D3DXVECTOR4(0,0,1.0f,1.0f);
	_Shader = CShader2D::Instance();
	D3DXMatrixIdentity(&WorldMtx);

}

void Fade::Uninit(void)
{
	if (Texture != nullptr)
	{
		Texture->Release();
		Texture = nullptr;
	}
}

void Fade::Finalize(void)
{
	delete Self;
}
void Fade::Update(void)
{
	for (int cnt = 0;cnt < 2;cnt++)
	{
		if (_Mode[cnt] == FADE_IN)
		{
			if (Color[cnt].a <= DestColor[cnt].a)
			{
				_Mode[cnt] = FADE_NONE;
				Fading[cnt] = false;
				SubColor[cnt] = D3DXCOLOR(0,0,0,0);
			}
		}
		else if (_Mode[cnt] != FADE_NONE)
		{
			if (Color[cnt].a >= DestColor[cnt].a)
			{
				(_Mode[cnt] == FADE_INFINIT) ? Fading[cnt] = true : Fading[cnt] = false;
				_Mode[cnt] = FADE_NONE;
				SubColor[cnt] = D3DXCOLOR(0,0,0,0);
			}
		}
		Color[cnt] += SubColor[cnt];
	}
}

void Fade::Draw(int layer)
{
	if (Fading[layer])
	{
		Renderer::SetStream2D();
		D3DXVECTOR3 _Pos(0,0,0);
		_Pos.x = Window::Instance()->WindowSize().x / 2.0f;
		_Pos.y = Window::Instance()->WindowSize().y / 2.0f;
		_Pos.z = 0;

		Camera2D* camera = Camera2D::GetCamera(0);
		camera->SetNoMove();
		WorldMtx = _Shader->Identity();
		D3DXMATRIX MtxScl,MtxRot,MtxTrans;
		D3DXMatrixScaling(&MtxScl,Size.x,Size.y,1.0f);
		D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxScl);

		D3DXMatrixTranslation(&MtxTrans,_Pos.x,_Pos.y,_Pos.z);
		D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxTrans);

		_Shader->SetMatrix(CShader2D::WORLD_MTX,WorldMtx);

		//テクスチャの設定
		_Shader->SetTexture(Texture);
		_Shader->SetFloatArray(CShader2D::DIFFUSE,Color[layer],4);
		_Shader->SetFloatArray(CShader2D::UV,uv,4);
		//ポリゴンを描画
		_Shader->DrawBegin();
		_Shader->Draw(CShader2D::NORMAL,D3DPT_TRIANGLESTRIP);
		_Shader->DrawEnd();

		Camera2D::Set(0);
	}
}

void Fade::Start(float frame,FADE mode,const D3DXCOLOR &color,int layer)
{
	if (layer < 0 || layer > 1)
	{
		for (int cnt = 0;cnt < 2;cnt++)
		{
			_Mode[cnt] = mode;
			DestColor[cnt] = color;
			if (mode == FADE_IN)
			{
				Color[cnt].a = 1.0f;
			}
			else
			{
				Color[cnt].a = 0.0f;
			}

			SubColor[cnt] = (DestColor[cnt] - Color[cnt]);
			SubColor[cnt] /= frame;
			Fading[cnt] = true;
		}
	}
	else
	{
		_Mode[layer] = mode;
		DestColor[layer] = color;
		if (mode == FADE_IN)
		{
			Color[layer].a = 1.0f;
		}
		else
		{
			Color[layer].a = 0.0f;
		}

		SubColor[layer] = (DestColor[layer] - Color[layer]);
		SubColor[layer] /= frame;
		Fading[layer] = true;
	}
}