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
cbuffer RatioBuffer : register(b4)
{
	float4 uiRatio;
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
	float4 beforePos : VECTOR;
	float ratio : FLOAT;
};




VSOutput CircleVS(float4 pos : POSITION,
	float2 uv : TEXUV)
{
	VSOutput output;
	float4 beforePos = pos;
	float4 afterPos = mul(pos, world);
	afterPos = mul(afterPos, view);
	output.pos = mul(afterPos, projection);
	output.beforePos = beforePos;
	output.uv = uv;
	output.col = color;
	output.ratio = uiRatio;

	return output;
}

float4 CirclePS(VSOutput input) : SV_Target0
{
	float pi = 3.141592f;
	float uiRad = input.ratio * 2 * pi - pi;
	float gageRad = atan2(-input.beforePos.x, input.beforePos.y);
	
	float4 tex = g_texture.Sample(g_sampler, input.uv);
	float4 col = input.col;	
		
	if (uiRad <= gageRad) return float4(0,0,0,0);
	else return tex * col;

}
