Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer WorldBuffer : register(b0)
{
	matrix world;
};
cbuffer ViewBuffer : register(b1)
{
	matrix view;
};
cbuffer ProjectionBuffer : register(b2)
{
	matrix projection;
};
cbuffer ColorBuffer : register(b3)
{
	float4 color;
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXUV;
};

struct VSOutput
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD0;
	float4 col : COLOR0;
};




VSOutput VS(float4 pos : POSITION,
	float2 uv : TEXUV)
{
	VSOutput output;
	pos = mul(pos, world);
	pos = mul(pos, view);
	output.pos = mul(pos, projection);
	output.uv = uv;
	output.col = color;

	return output;
}

float4 PS(VSOutput input) : SV_Target0
{
	float4 tex = g_texture.Sample(g_sampler, input.uv);
	float4 col = input.col;
	return tex /** col*/;

}
