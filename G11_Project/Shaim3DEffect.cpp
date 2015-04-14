#include "Shaim3DEffect.h"
#include "Shaim3DParts.h"
#include "Camera.h"
#include "Shader3D.h"
#include "Shader2D.h"
#include "Renderer.h"
#include "Mutex.h"

int Shaim3DEffect::_Num = 0;
CCamera* Shaim3DEffect::Camera = nullptr;
Shader3D* Shaim3DEffect::_Shader3D = nullptr;
Shader2D* Shaim3DEffect::_Shader2D = nullptr;
Shaim3DEffect* Shaim3DEffect::_Top = nullptr;
Shaim3DEffect* Shaim3DEffect::_Cur = nullptr;
bool Shaim3DEffect::PauseFlag = false;

Shaim3DEffect::Shaim3DEffect()
{
	_Pos = D3DXVECTOR3(0,0,0);
	_Speed = D3DXVECTOR3(0,0,0);
	_Size = D3DXVECTOR3(1.0f,1.0f,1.0f);
	LinkList();
	_Num++;
}

Shaim3DEffect::~Shaim3DEffect()
{
	UnlinkList();
}

void Shaim3DEffect::LinkList(void)
{
	if (_Top != NULL)//“ñ‚Â–ÚˆÈ~‚Ìˆ—
	{
		Shaim3DEffect* Polygon = _Cur;
		Polygon->_Next = this;
		_Prev = Polygon;
		_Next = NULL;
		_Cur = this;
	}
	else//Å‰‚Ìˆê‚Â‚ÌŽž‚Ìˆ—
	{
		_Top = this;
		_Cur = this;
		_Prev = NULL;
		_Next = NULL;
	}
}

void Shaim3DEffect::UnlinkList(void)
{
	if (_Prev == NULL)//æ“ª
	{
		if (_Next != NULL)//ŽŸ‚ª‚ ‚é
		{
			_Next->_Prev = NULL;
			_Top = _Next;
		}
		else//ÅŒã‚Ìˆê‚Â‚¾‚Á‚½
		{
			_Top = NULL;
			_Cur = NULL;
		}
	}
	else if (_Next == NULL)//I’[
	{
		if (_Prev != NULL)//‘O‚ª‚ ‚é
		{
			_Prev->_Next = NULL;
			_Cur = _Prev;
		}
		else//ÅŒã‚Ìˆê‚Â‚¾‚Á‚½
		{
			_Top = NULL;
			_Cur = NULL;
		}
	}
	else//‘OŒã‚Éƒf[ƒ^‚ª‚ ‚é‚Æ‚«
	{
		_Prev->_Next = _Next;
		_Next->_Prev = _Prev;
	}

	_Prev = NULL;
	_Next = NULL;

	_Num--;

}

void Shaim3DEffect::Initialize(void)
{
	Camera = CCamera::Create(D3DXVECTOR3(0,0,-200.0f),D3DXVECTOR3(0,0,0));
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
	Shaim3DEffect* effect = _Top;
	Shaim3DEffect* next = nullptr;

	while (effect)
	{
		next = effect->_Next;
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
	DebugProc::Print("PartsNum:%d\nEffectNum:%d\n",Shaim3DParts::Num(),_Num);
	Shaim3DEffect* effect = _Top;
	Shaim3DEffect* next = nullptr;
	LPDIRECT3DSURFACE9 OldSurface;

	if (_Shader3D == nullptr){ _Shader3D = Shader3D::Instance(); Shaim3DParts::SetShader(Shader3D::Instance()); }
	if (_Shader2D == nullptr){ _Shader2D = Shader2D::Instance(); }

	Window::Instance()->Device()->GetRenderTarget(0,&OldSurface);
	Camera->Set();


	CRenderer::SetStream3D();
	_Shader3D->DrawBegin();
	while (effect)
	{
		next = effect->_Next;
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

	CRenderer::SetStream2D();

	D3DXCOLOR color(WHITE(1.0f));
	_Shader2D->SetFloatArray(Shader2D::DIFFUSE,color,4);
	
	D3DXVECTOR4 uv(0,0,1.0f,1.0f);
	_Shader2D->SetFloatArray(Shader2D::UV,uv,4);

	//‰ñ“]‚ð”½‰f
	D3DXMATRIX MtxRot;
	D3DXMatrixRotationYawPitchRoll(&MtxRot,0,0,0);
	_Shader2D->SetMatrix(Shader2D::ROT_MTX,MtxRot);


	effect = _Top;
	next = nullptr;
	_Shader2D->DrawBegin();
	while (effect)
	{
		next = effect->_Next;
		effect->Draw();
		effect = next;
	}
	_Shader2D->DrawEnd();

	CCamera::Set(0);
}

void Shaim3DEffect::ReleaseAll(void)
{
	Shaim3DEffect* effect = _Top;
	Shaim3DEffect* next = nullptr;

	while (effect)
	{
		next = effect->_Next;
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

	_Shader2D->SetMatrix(Shader2D::WORLD_MTX,WorldMtx);
	/*
	_Shader->SetFloatArray(Shader2D::SIZE,_Size,3);
	_Shader->SetMatrix(Shader2D::ROT_MTX,MtxRot);
	_Shader->SetMatrix(Shader2D::POS_MTX,MtxTrans);
	*/
	//ƒeƒNƒXƒ`ƒƒ‚ÌÝ’è
	_Shader2D->SetTexture(Texture);

	//ƒ|ƒŠƒSƒ“‚ð•`‰æ
	_Shader2D->Draw(Shader2D::NORMAL,D3DPT_TRIANGLESTRIP);
}