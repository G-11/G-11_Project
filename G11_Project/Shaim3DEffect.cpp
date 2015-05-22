#include "Shaim3DEffect.h"
#include "Shaim3DParts.h"
#include "Camera.h"
#include "Shader3D.h"
#include "Shader2D.h"
#include "Renderer.h"
#include "Mutex.h"

int Shaim3DEffect::Num_ = 0;
Camera3D* Shaim3DEffect::Camera = nullptr;
CShader3D* Shaim3DEffect::_Shader3D = nullptr;
CShader2D* Shaim3DEffect::_Shader2D = nullptr;
Shaim3DEffect* Shaim3DEffect::Top_ = nullptr;
Shaim3DEffect* Shaim3DEffect::Cur_ = nullptr;
bool Shaim3DEffect::PauseFlag = false;

Shaim3DEffect::Shaim3DEffect()
{
	_Pos = D3DXVECTOR3(0,0,0);
	_Speed = D3DXVECTOR3(0,0,0);
	_Size = D3DXVECTOR3(1.0f,1.0f,1.0f);
	LinkList();
	Num_++;
}

Shaim3DEffect::~Shaim3DEffect()
{
	UnlinkList();
}

void Shaim3DEffect::LinkList(void)
{
	if (Top_ != NULL)//“ñ‚Â–ÚˆÈ~‚Ìˆ—
	{
		Shaim3DEffect* Polygon = Cur_;
		Polygon->Next_ = this;
		Prev_ = Polygon;
		Next_ = NULL;
		Cur_ = this;
	}
	else//Å‰‚Ìˆê‚Â‚ÌŽž‚Ìˆ—
	{
		Top_ = this;
		Cur_ = this;
		Prev_ = NULL;
		Next_ = NULL;
	}
}

void Shaim3DEffect::UnlinkList(void)
{
	if (Prev_ == NULL)//æ“ª
	{
		if (Next_ != NULL)//ŽŸ‚ª‚ ‚é
		{
			Next_->Prev_ = NULL;
			Top_ = Next_;
		}
		else//ÅŒã‚Ìˆê‚Â‚¾‚Á‚½
		{
			Top_ = NULL;
			Cur_ = NULL;
		}
	}
	else if (Next_ == NULL)//I’[
	{
		if (Prev_ != NULL)//‘O‚ª‚ ‚é
		{
			Prev_->Next_ = NULL;
			Cur_ = Prev_;
		}
		else//ÅŒã‚Ìˆê‚Â‚¾‚Á‚½
		{
			Top_ = NULL;
			Cur_ = NULL;
		}
	}
	else//‘OŒã‚Éƒf[ƒ^‚ª‚ ‚é‚Æ‚«
	{
		Prev_->Next_ = Next_;
		Next_->Prev_ = Prev_;
	}

	Prev_ = NULL;
	Next_ = NULL;

	Num_--;

}

void Shaim3DEffect::Initialize(void)
{
	Camera = Camera3D::Create(D3DXVECTOR3(0,0,-200.0f),D3DXVECTOR3(0,0,0));
}

void Shaim3DEffect::Init(const D3DXVECTOR2& size)
{
	Mutex::Instance()->Enter();
	Window* window = Window::Instance();
	window->Device()->CreateTexture((UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Texture,NULL);
	Texture->GetSurfaceLevel(0,&Surface);
	
	Mutex::Instance()->Leave();

	Parts = new Shaim3DParts*[PartsNum];
	for (int cnt = 0;cnt < PartsNum;cnt++)
	{
		Parts[cnt] = Shaim3DParts::Create(D3DXVECTOR3(0,0,0),size);
	}
}
Shaim3DEffect* Shaim3DEffect::Create(const D3DXVECTOR3& pos,const D3DXVECTOR3& size,int partsnum)
{
	Shaim3DEffect* effect = new Shaim3DEffect;
	if (effect == nullptr)
	{
		return effect;
	}
	effect->_Pos = pos;
	effect->_Size = size;
	effect->PartsNum = partsnum;
	effect->Init();

	return effect;
}

void Shaim3DEffect::UpdateAll(void)
{
	Shaim3DEffect* effect = Top_;
	Shaim3DEffect* next = nullptr;

	while (effect)
	{
		next = effect->Next_;
		if (PauseFlag)
		{
			effect -> Pause();
		}
		else
		{
			effect->Update();
		}
		effect = next;
	}
}

void Shaim3DEffect::DrawAll(void)
{
	Shaim3DEffect* effect = Top_;
	Shaim3DEffect* next = nullptr;
	LPDIRECT3DSURFACE9 OldSurface;

	if (_Shader3D == nullptr){ _Shader3D = CShader3D::Instance(); Shaim3DParts::SetShader(CShader3D::Instance()); }
	if (_Shader2D == nullptr){ _Shader2D = CShader2D::Instance(); }

	Window::Instance()->Device()->GetRenderTarget(0,&OldSurface);
	Camera->Set();


	Renderer::SetStream3D();
	_Shader3D->DrawBegin();
	while (effect)
	{
		next = effect->Next_;
		effect->DrawParts();
		effect = next;
	}
	_Shader3D->DrawEnd();

	if (OldSurface != nullptr)
	{
		Window::Instance()->Device()->SetRenderTarget(0,OldSurface);
		OldSurface->Release();
		OldSurface = nullptr;
	}

	Renderer::SetStream2D();

	D3DXCOLOR color(WHITE(1.0f));
	_Shader2D->SetFloatArray(CShader2D::DIFFUSE,color,4);
	
	D3DXVECTOR4 uv(0,0,1.0f,1.0f);
	_Shader2D->SetFloatArray(CShader2D::UV,uv,4);

	//‰ñ“]‚ð”½‰f
	D3DXMATRIX MtxRot;
	D3DXMatrixRotationYawPitchRoll(&MtxRot,0,0,0);
	_Shader2D->SetMatrix(CShader2D::ROT_MTX,MtxRot);


	effect = Top_;
	next = nullptr;
	_Shader2D->DrawBegin();
	while (effect)
	{
		next = effect->Next_;
		effect->Draw();
		effect = next;
	}
	_Shader2D->DrawEnd();

	Camera3D::Set(0);
}

void Shaim3DEffect::ReleaseAll(void)
{
	Shaim3DEffect* effect = Top_;
	Shaim3DEffect* next = nullptr;

	while (effect)
	{
		next = effect->Next_;
		effect->Release();
		effect = next;
	}
}

void Shaim3DEffect::Release(void)
{
	if (Parts != nullptr)
	{
		for (int cnt = 0;cnt < PartsNum;cnt++)
		{
			if (Parts[cnt] != nullptr)
			{
				Parts[cnt]->Release();
				Parts[cnt] = nullptr;
			}
		}
		delete[] Parts;
	}

	SafeRelease(Surface);
	SafeRelease(Texture);

	delete this;
}

void Shaim3DEffect::Update(void)
{
	for (int cnt = 0;cnt < PartsNum;cnt++)
	{
		Parts[cnt]->Update();
	}
}
void Shaim3DEffect::DrawParts(void)
{
	
	Window::Instance()->Device()->SetRenderTarget(0,Surface);
	Window::Instance()->Device()->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_RGBA(0,0,0,0),1.0f,0);

	for (int cnt = 0;cnt < PartsNum;cnt++)
	{
		Parts[cnt]->Draw();
	}
}

void Shaim3DEffect::Draw(void)
{
	D3DXMATRIX MtxScl,MtxRot,MtxTrans;
	WorldMtx = _Shader2D->Identity();

	//ˆÊ’u‚ð”½‰f
	D3DXMatrixScaling(&MtxScl,_Size.x,_Size.y,1.0f);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxScl);
	D3DXMatrixTranslation(&MtxTrans,_Pos.x,_Pos.y,_Pos.z);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxTrans);

	_Shader2D->SetMatrix(CShader2D::WORLD_MTX,WorldMtx);
	/*
	_Shader->SetFloatArray(CShader2D::SIZE,_Size,3);
	_Shader->SetMatrix(CShader2D::ROT_MTX,MtxRot);
	_Shader->SetMatrix(CShader2D::POS_MTX,MtxTrans);
	*/
	//ƒeƒNƒXƒ`ƒƒ‚ÌÝ’è
	_Shader2D->SetTexture(Texture);

	//ƒ|ƒŠƒSƒ“‚ð•`‰æ
	_Shader2D->Draw(CShader2D::NORMAL,D3DPT_TRIANGLESTRIP);
}