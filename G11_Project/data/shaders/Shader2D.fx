float4x4 WorldMtx;
float4x4 Projection;
float4x4 ViewMtx;
float4x4 ViewPort;
float4x4 Scl;
float4x4 Rot;
float4x4 Pos;
float2 Offset;
float4 Diffuse;
float4 MaskColor;
float4 UV;
float4 MaskUV;
texture DiffuseMap;
texture Mask;

sampler diffuseSampler = sampler_state {
	Texture = <DiffuseMap>;

	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

sampler maskSampler = sampler_state {
	Texture = <Mask>;

	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float3 pos	:POSITION0;
	float2 UV	:TEXCOORD0;
};

struct VS_OUT
{
	vector pos		:POSITION0;
	float4 color	:COLOR0;
	float2 UV		:TEXCOORD0;
	float2 maskUV	:TEXCOORD1;
};

//=========================================================
//頂点シェーダー
//=========================================================

VS_OUT VS(VS_IN Input)
{
	VS_OUT Out;
	float4 pos = float4(Input.pos,1.0f);
	pos.x += Offset.x;
	pos.y += Offset.y;

	float4 output = mul(pos,WorldMtx);
	output = mul(output,Projection);

	Out.pos = output;
	Out.color = Diffuse;
	Out.UV = Input.UV * float2(UV.z,UV.w) + float2(UV.x,UV.y);
	Out.maskUV = Input.UV * float2(MaskUV.z,MaskUV.w) + float2(MaskUV.x,MaskUV.y);

	return Out;

}


//=========================================================
//ピクセルシェーダー
//=========================================================
//ノーマル
//=========================================================
float4 PS(VS_OUT Input) : COLOR
{
	float4 Out = tex2D(diffuseSampler,Input.UV) * Input.color;

	return Out;
}
//=========================================================
//アルファマスク
//=========================================================
float4 PS_AlphaMask(VS_OUT Input):COLOR
{
	float4 Out = tex2D(diffuseSampler,Input.UV) * Input.color;
	float4 Mask = tex2D(maskSampler,Input.maskUV);
	Out.a *= Mask.a;
	return Out;
}
//=========================================================
//カラーマスク
//=========================================================
float4 PS_ColorMask(VS_OUT Input) :COLOR
{
	float4 Out = tex2D(diffuseSampler,Input.UV) * Input.color;
	float4 Mask = tex2D(maskSampler,Input.maskUV) * MaskColor;
	Out.rgb = (Out.rgb * (1.0f - Mask.a) + Mask.rgb * Mask.a);
	return Out;
}
//=========================================================
//ネガマスク
//=========================================================
float4 PS_NegaMask(VS_OUT Input) :COLOR
{
	float4 Out = tex2D(diffuseSampler,Input.UV) * Input.color;
	float4 Nega = Out;
	Nega.r = 1.0f-Out.r;
	Nega.g = 1.0f-Out.g;
	Nega.b = 1.0f-Out.b;
	float4 Mask = tex2D(maskSampler,Input.maskUV);
	Out.rgb = (Out.rgb * (1.0f - Mask.a) + Nega.rgb * Mask.a);
	return Out;
}
//=========================================================
//
//=========================================================
float4 PS_Sky(VS_OUT Input) : COLOR
{
	float4 Out = tex2D(diffuseSampler,Input.UV) * Input.color;
	Out.rgb *= tex2D(maskSampler,float2(MaskUV.x,0.5f));
	return Out;
}
//=========================================================
//テクニック
//=========================================================
technique Polygon2D
{
	pass Normal
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader	= compile vs_2_0 VS();
		PixelShader		= compile ps_2_0 PS();
	}

	pass Add
	{
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = SRCALPHA;
		ZEnable = true;
		ZWriteEnable = true;
		CullMode = NONE;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}

	pass Sub
	{
		BlendOp = REVSUBTRACT;
		DestBlend = ONE;
		SrcBlend = SRCALPHA;
		ZEnable = true;
		ZWriteEnable = true;
		CullMode = NONE;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}

	pass AlphaMask
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_AlphaMask();
	}

	pass ColorMask
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_ColorMask();
	}
	pass NegaMask
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_NegaMask();
	}

	pass Sky
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_Sky();

	}
};