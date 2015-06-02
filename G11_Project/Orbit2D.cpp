//=============================================================================
//ポリラインエフェクト2Dクラス
//=============================================================================
//=============================================================================
//インクルード
//=============================================================================
#include "Orbit2D.h"
#include "Renderer.h"
#include "Shader2D.h"
#include "Texture.h"
#include "Mutex.h"
//=============================================================================
//グローバル変数
//=============================================================================
Orbit2D* Orbit2D::Top_ = nullptr;
Orbit2D* Orbit2D::Cur_ = nullptr;
int Orbit2D::Num_ = 0;
CShader2D* Orbit2D::_Shader = nullptr;
bool Orbit2D::_PauseFlag = false;

//=============================================================================
//コンストラクタ
//=============================================================================
Orbit2D::Orbit2D()
{
	Parent_Pos = nullptr;
	Pos = D3DXVECTOR3(0,0,0);
	VtxBuff = nullptr;
	DivideNum = 0;
	Width_ = 0;
	_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	ReleaseFlag_ = false;
	frame = 0;
	Pass = CShader2D::ADD;

	LinkList();
}

//=============================================================================
//デストラクタ
//=============================================================================
Orbit2D::~Orbit2D()
{
	UnlinkList();
}

//=============================================================================
//自身をリストに追加
//=============================================================================
void Orbit2D::LinkList(void)
{
	if (Top_ != NULL)//二つ目以降の処理
	{
		Orbit2D* Polygon = Cur_;
		Polygon->Next_ = this;
		Prev_ = Polygon;
		Next_ = NULL;
		Cur_ = this;
	}
	else//最初の一つの時の処理
	{
		Top_ = this;
		Cur_ = this;
		Prev_ = NULL;
		Next_ = NULL;
	}
	Num_++;
}

//=============================================================================
//自身をリストから削除
//=============================================================================
void Orbit2D::UnlinkList(void)
{
	if (Prev_ == NULL)//先頭
	{
		if (Next_ != NULL)//次がある
		{
			Next_->Prev_ = NULL;
			Top_ = Next_;
		}
		else//最後の一つだった
		{
			Top_ = NULL;
			Cur_ = NULL;
		}
	}
	else if (Next_ == NULL)//終端
	{
		if (Prev_ != NULL)//前がある
		{
			Prev_->Next_ = NULL;
			Cur_ = Prev_;
		}
		else//最後の一つだった
		{
			Top_ = NULL;
			Cur_ = NULL;
		}
	}
	else//前後にデータがあるとき
	{
		Prev_->Next_ = Next_;
		Next_->Prev_ = Prev_;
	}

	Prev_ = NULL;
	Next_ = NULL;
	Num_--;
}
//=============================================================================
//作成
//引数：D3DXVECTOR3 初期位置
//		int 分割数
//		float 幅
//		D3DXVECTOR3* 親の座標ポインタ
//=============================================================================
Orbit2D* Orbit2D::Create(const D3DXVECTOR3& pos,int divide,float width,D3DXVECTOR3* parent,D3DXVECTOR3* parent2)
{
	Orbit2D* orbit = new Orbit2D;
	if (orbit == nullptr)
	{
		return orbit;
	}
	orbit->DivideNum = divide;
	orbit->Width_ = width;
	if (parent != nullptr && parent2 != nullptr)
	{
		orbit->Parent_Pos2[0] = parent;
		orbit->Parent_Pos2[1] = parent2;
	}
	else if (parent != nullptr)
	{
		orbit->Parent_Pos = parent;
	}
	orbit->Texture = GetTexture(TEX_NONE);
	orbit->Init(pos);

	return orbit;
}
//=============================================================================
//初期化
//引数：初期座標
//=============================================================================
void Orbit2D::Init(const D3DXVECTOR3& pos)
{
	LPDIRECT3DDEVICE9 device = Window::Instance()->Device();

	Mutex::Instance()->Enter();
	device->CreateVertexBuffer(sizeof(VERTEX_2D)*(DivideNum * 2),D3DUSAGE_WRITEONLY,FVF_CUSTOM,D3DPOOL_MANAGED,&VtxBuff,NULL);
	Mutex::Instance()->Leave();

	//親が居るなら初期座標も親の座標
	(Parent_Pos != nullptr) ? Pos = *Parent_Pos : Pos = pos;
	CountDown = DivideNum;
	VERTEX_2D* vtx;

	VtxBuff->Lock(0,0,(void**)&vtx,0);

	for (int cnt = 0;cnt < DivideNum * 2;cnt++)
	{
		vtx[cnt].vtx.x = Pos.x;
		vtx[cnt].vtx.y = (cnt % 2 == 0) ? Pos.y + Width_ / 2.0f : Pos.y - Width_ / 2.0f;
		vtx[cnt].vtx.z = 0;

		vtx[cnt].tex.x = (1.0f / DivideNum)*(cnt/2);
		vtx[cnt].tex.y = 1.0f - (cnt%2);
	}

	VtxBuff->Unlock();
}
//=============================================================================
//更新
//=============================================================================
void Orbit2D::Update(void)
{
	float angle = 0;
	D3DXVECTOR2 dis = D3DXVECTOR2(0,0);

	if (!ReleaseFlag_)
	{
		if (Parent_Pos != nullptr)
		{
			dis.x = (Parent_Pos->x - Pos.x);
			dis.y = (Parent_Pos->y - Pos.y);

			angle = -Atan2(dis.x,dis.y);
			Pos = *Parent_Pos;
		}
		else if (Parent_Pos2[0] != nullptr && Parent_Pos2[1] != nullptr)
		{
			Pos = *Parent_Pos2[0];
		}
		else
		{
			D3DXVECTOR2 oldPos = Pos;
			Pos += _Speed;
			dis.x = (Pos.x - oldPos.x);
			dis.y = (Pos.y - oldPos.y);

			angle = -Atan2(dis.x,dis.y);
		}
	}

	//一番最後の座標を記憶
	D3DXVECTOR3 tailPos = D3DXVECTOR3(0,0,0);

	//処理が特殊なので、頂点バッファをいじる
	VERTEX_2D* vtx;
	VtxBuff->Lock(0,0,(void**)&vtx,0);

	for (int cnt = DivideNum * 2 - 1;cnt > 1;cnt--)
	{//奇数と偶数が同じ一つ前の座標を移す
		vtx[cnt].vtx = vtx[(cnt-2)].vtx;
	}
	tailPos = vtx[DivideNum*2-1].vtx;
	
	if (!ReleaseFlag_)
	{//頭の二点はコピー出来ないので計算し直す
		if (Parent_Pos2[0] != nullptr && Parent_Pos2[1] != nullptr)
		{
			vtx[0].vtx.x = Parent_Pos2[0]->x;
			vtx[0].vtx.y = Parent_Pos2[0]->y;

			vtx[1].vtx.x = Parent_Pos2[1]->x;
			vtx[1].vtx.y = Parent_Pos2[1]->y;
		}
		else
		{
			vtx[0].vtx.x = cosf(angle + PI)*Width_ / 2.0f + Pos.x;
			vtx[0].vtx.y = sinf(angle + PI)*Width_ / 2.0f + Pos.y;

			vtx[1].vtx.x = cosf(angle)*Width_ / 2.0f + Pos.x;
			vtx[1].vtx.y = sinf(angle)*Width_ / 2.0f + Pos.y;
		}
	}

	VtxBuff->Unlock();
	
	//開放フラグが立っているなら
	if (ReleaseFlag_)
	{
		CountDown--;
		if (CountDown < 0)
		{//頭の座標と最後の座標がほぼ一致したら開放する
			Release();
		}
		
	}
}
//=============================================================================
// ポーズ
//=============================================================================
void Orbit2D::Pause(void)
{

}
//=============================================================================
//描画
//=============================================================================
void Orbit2D::Draw(void)
{
	Window::Instance()->Device()->SetStreamSource(0,VtxBuff,0,sizeof(VERTEX_2D));

	_Shader->SetFloatArray(CShader2D::DIFFUSE,_Color,4);
	_Shader->SetTexture(Texture);

	_Shader->Draw((CShader2D::PASS)Pass,D3DPT_TRIANGLESTRIP,(DivideNum - 1) * 2);
}
//=============================================================================
//開放
//=============================================================================
void Orbit2D::Release(void)
{
	SafeRelease(VtxBuff);
	delete this;
}
//=============================================================================
//全部更新
//=============================================================================
void Orbit2D::UpdateAll(void)
{
	Orbit2D* orbit = Top_;
	Orbit2D* next = nullptr;

	while (orbit)
	{
		next = orbit->Next_;

		if (_PauseFlag)
		{
			orbit->Pause();
		}
		else
		{
			orbit->Update();
		}

		orbit = next;
	}
}
//=============================================================================
//全部描画
//=============================================================================
void Orbit2D::DrawAll(void)
{
	
	D3DXVECTOR4 uv(0,0,1.0f,1.0f);

	if (_Shader == nullptr){ _Shader = CShader2D::Instance(); }

	_Shader->SetIdentity(CShader2D::WORLD_MTX);
	Window::Instance()->SetVtxDecl(VTX_DECL_2D);

	_Shader->DrawBegin();
	Orbit2D* orbit = Top_;
	while (orbit)
	{
		orbit->Draw();

		orbit = orbit->Next_;
	}
	_Shader->DrawEnd();

	Renderer::SetStream2D();

}
//=============================================================================
//全部開放
//=============================================================================
void Orbit2D::ReleaseAll(void)
{
	Orbit2D* orbit = Top_;
	Orbit2D* next = nullptr;

	while (orbit)
	{
		next = orbit->Next_;

		orbit->Release();

		orbit = next;
	}
}