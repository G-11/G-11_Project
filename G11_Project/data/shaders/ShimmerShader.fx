float Fluctuation = 0.04f;
float4x4 Projection;

float TexelU[5];
float TexelV[5];

texture shimmerMap;
texture DiffuseMap;

sampler shimmerSampler = sampler_state{
	Texture = <shimmerMap>;

	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

sampler diffuseSampler = sampler_state {
	Texture = <DiffuseMap>;

	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};

//=============================================================================
//頂点シェーダ
//=============================================================================
VS_OUT VS(VS_IN Input)
{
	VS_OUT Output;

	Output.pos = mul(float4(Input.pos,1.0f),Projection);
	Output.color = float4(1.0f,1.0f,1.0f,1.0f);
	Output.uv = Input.uv;

	return Output;
}
//=============================================================================
//通常シェーダー
//=============================================================================
float4 PS(VS_OUT Input) : COLOR
{
	return tex2D(shimmerSampler,Input.uv);
}
//=============================================================================
//陽炎シェーダー
//=============================================================================
float4 PS_Shimmer(VS_OUT Input) : COLOR
{
	//陽炎テクスチャから色情報を取得し、テクセルの移動量を計算する
	float4 ShimmerColor = tex2D(shimmerSampler,Input.uv);

	//色の値を-1.0f～1.0fに変換する
	ShimmerColor.xy = (ShimmerColor.xy - 0.5f) * 2.0f;

	float4 color = tex2D(diffuseSampler,Input.uv - ((ShimmerColor.xy) * ShimmerColor.z * Fluctuation)*ShimmerColor.w);
	color.a = min(1.0f,color.a*1.5f);
	
	return color;
	//return ShimmerColor;

}

//=============================================================================
//X方向にぼかす
//=============================================================================
float4 PS_BlurX(VS_OUT Input) : COLOR
{
	//テクセルを取得   
	float2 Texel0 = Input.uv + float2(-TexelU[0],0.0f);
	float2 Texel1 = Input.uv + float2(-TexelU[1],0.0f);
	float2 Texel2 = Input.uv + float2(-TexelU[2],0.0f);
	float2 Texel3 = Input.uv + float2(-TexelU[3],0.0f);
	float2 Texel4 = Input.uv + float2(-TexelU[4],0.0f);

	float2 Texel5 = Input.uv + float2(TexelU[0],0.0f);
	float2 Texel6 = Input.uv + float2(TexelU[1],0.0f);
	float2 Texel7 = Input.uv + float2(TexelU[2],0.0f);
	float2 Texel8 = Input.uv + float2(TexelU[3],0.0f);
	float2 Texel9 = Input.uv + float2(TexelU[4],0.0f);

	//取得したテクセル位置のカラー情報を取得する。
	//それぞれのカラー値に重みをかけている。この重み値はすべての合計が 1.0f になるように調整する。
	float4 p0 = tex2D(shimmerSampler,Input.uv) * 0.20f;

	float4 p1 = tex2D(shimmerSampler,Texel0) * 0.12f;
	float4 p2 = tex2D(shimmerSampler,Texel1) * 0.10f;
	float4 p3 = tex2D(shimmerSampler,Texel2) * 0.08f;
	float4 p4 = tex2D(shimmerSampler,Texel3) * 0.06f;
	float4 p5 = tex2D(shimmerSampler,Texel4) * 0.04f;

	float4 p6 = tex2D(shimmerSampler,Texel5) * 0.12f;
	float4 p7 = tex2D(shimmerSampler,Texel6) * 0.10f;
	float4 p8 = tex2D(shimmerSampler,Texel7) * 0.08f;
	float4 p9 = tex2D(shimmerSampler,Texel8) * 0.06f;
	float4 p10 = tex2D(shimmerSampler,Texel9) * 0.04f;

	//カラーを合成する
	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

//=============================================================================
//Y方向にぼかす
//=============================================================================
float4 PS_BlurY(VS_OUT Input) : COLOR0
{
	//テクセルを取得   
	float2 Texel0 = Input.uv + float2(0.0f,-TexelV[0]);
	float2 Texel1 = Input.uv + float2(0.0f,-TexelV[1]);
	float2 Texel2 = Input.uv + float2(0.0f,-TexelV[2]);
	float2 Texel3 = Input.uv + float2(0.0f,-TexelV[3]);
	float2 Texel4 = Input.uv + float2(0.0f,-TexelV[4]);

	float2 Texel5 = Input.uv + float2(0.0f,TexelV[0]);
	float2 Texel6 = Input.uv + float2(0.0f,TexelV[1]);
	float2 Texel7 = Input.uv + float2(0.0f,TexelV[2]);
	float2 Texel8 = Input.uv + float2(0.0f,TexelV[3]);
	float2 Texel9 = Input.uv + float2(0.0f,TexelV[4]);

	//取得したテクセル位置のカラー情報を取得する。
	//それぞれのカラー値に重みをかけている。この重み値はすべての合計が 1.0f になるように調整する。
	float4 p0 = tex2D(shimmerSampler,Input.uv) * 0.20f;

	float4 p1 = tex2D(shimmerSampler,Texel0) * 0.12f;
	float4 p2 = tex2D(shimmerSampler,Texel1) * 0.10f;
	float4 p3 = tex2D(shimmerSampler,Texel2) * 0.08f;
	float4 p4 = tex2D(shimmerSampler,Texel3) * 0.06f;
	float4 p5 = tex2D(shimmerSampler,Texel4) * 0.04f;

	float4 p6 = tex2D(shimmerSampler,Texel5) * 0.12f;
	float4 p7 = tex2D(shimmerSampler,Texel6) * 0.10f;
	float4 p8 = tex2D(shimmerSampler,Texel7) * 0.08f;
	float4 p9 = tex2D(shimmerSampler,Texel8) * 0.06f;
	float4 p10 = tex2D(shimmerSampler,Texel9) * 0.04f;

	//カラーを合成する
	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

//=============================================================================
//テクニック
//=============================================================================
technique Shimmer
{
	pass Shimmer
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_Shimmer();
	}

	pass BlurX
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_BlurX();
	}

	pass BlurY
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_BlurY();
	}
	
};