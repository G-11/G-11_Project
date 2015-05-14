static float4x4 WorldMtx;
float4x4 Projection;
float4x4 ViewMtx;
float4x4 InvViewMtx;
float4x4 ViewPort;
float4x4 Scl;
float4x4 Rot;
float4x4 Pos;
float3 LightVec;
float4 Ambient = float4(0.01f,0.01f,0.05f,1.0f);
float4 LightColor;
float4 Color;
float3 Size;
float4 uv;//left,top,width,height

texture DiffuseMap;
texture MaskMap;

//テクスチャサンプラー
sampler diffuseSampler = sampler_state {
	Texture = <DiffuseMap>;

	//MinFilter = ANISOTROPIC;
	MinFilter = LINEAR;
	//MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

//マスクサンプラー
sampler maskSampler = sampler_state {
	Texture = <MaskMap>;

	//MinFilter = ANISOTROPIC;
	MinFilter = LINEAR;
	//MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float3 pos : POSITION0;
	float3 nor : NORMAL0;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};

//変換マトリックスの作成
float4 CreateWVP(vector pos)
{
	pos.x *= Size.x;
	pos.y *= Size.y;
	WorldMtx = mul(Scl,Rot);
	WorldMtx = mul(WorldMtx,Pos);
	float4 Out;
	Out = mul(pos,WorldMtx);
	Out = mul(Out,ViewMtx);
	Out = mul(Out,Projection);
	
	return Out;
}

//ビルボードマトリックス変換
float4 BillboardWVP(vector pos)
{
	pos.x *= Size.x;
	pos.y *= Size.y;
	WorldMtx = mul(Scl,Rot);
	WorldMtx = mul(WorldMtx,Pos);
	float4 Out;
	WorldMtx = mul(WorldMtx,InvViewMtx);
	Out = mul(pos,WorldMtx);
	Out = mul(Out,ViewMtx);
	Out = mul(Out,Projection);

	return Out;
}

//=========================================================
//頂点シェーダー
//=========================================================
//=========================================================
//標準(ハーフランバート)
//=========================================================
VS_OUT VS_Normal(VS_IN Input)
{
	VS_OUT Output;

	float4 Out;
	Out = CreateWVP(float4(Input.pos,1.0f));

	Output.pos = Out;
	Input.nor = mul(Input.nor,WorldMtx).xyz;
	float vec = max(0.0f,dot(Input.nor,LightVec.xyz));//法線とライトのベクトルの内積をとる
	float p = dot(Input.nor,LightVec.xyz);
	p = p*0.5f + 0.5f;
	p = p * p;
	Output.color = float4(Color.rgb*vec*p,Color.a);//法線を色に掛ける
	Output.color.rgb += Ambient.rgb;//環境光源
	Output.uv = Input.uv*float2(uv.z,uv.w) + float2(uv.x,uv.y);
	
	return Output;
}

//=========================================================
//ライティングなし
//=========================================================

VS_OUT VS_NoLight(VS_IN Input)
{
	VS_OUT Output;

	float4 Out;
	Out = CreateWVP(float4(Input.pos,1.0f));

	Output.pos = Out;
	Output.color = Color;
	Output.uv = Input.uv*float2(uv.z,uv.w) + float2(uv.x,uv.y);

	return Output;
}

//=========================================================
//ビルボード変換
//=========================================================
VS_OUT VS_Billboard(VS_IN Input)
{
	VS_OUT Output;
	float4 Out;
	Out = BillboardWVP(float4(Input.pos,1.0f));

	Output.pos = Out;
	Output.color = Color;
	Output.uv = Input.uv*float2(uv.z,uv.w) + float2(uv.x,uv.y);

	return Output;
}

//=========================================================
//ピクセルシェーダー
//=========================================================
//=========================================================
// 標準
//=========================================================
float4 PS_Normal(VS_OUT Input) : COLOR0
{
	return tex2D(diffuseSampler,Input.uv)*Input.color*LightColor;
}

//=========================================================
//ライト無し
//=========================================================
float4 PS_NoLight(VS_OUT Input) : COLOR0
{
	return tex2D(diffuseSampler,Input.uv)*Input.color;
}

//=========================================================
//テクニック
//=========================================================
technique Polygon3D
{
	//通常
	pass Normal
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS_Normal();
		PixelShader = compile ps_2_0 PS_Normal();
	}

	//加算
	pass Add
	{
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = SRCALPHA;
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_NoLight();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//減算
	pass Sub
	{
		BlendOp = REVSUBTRACT;
		DestBlend = ONE;
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_Normal();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//通常
	pass Nolight
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_NoLight();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//---------------------------------------------------------
	//ビルボード
	//---------------------------------------------------------
	pass Billboard
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_Billboard();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//加算
	pass AddEffect
	{
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = SRCALPHA;
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_Billboard();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//減算
	pass SubEffect
	{
		BlendOp = REVSUBTRACT;
		DestBlend = ONE;
		SrcBlend = SRCALPHA;
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_Billboard();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

};