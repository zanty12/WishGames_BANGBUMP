#include "shader.h"

DX::DX11::GRAPHICAL::Shader ShaderManager::texShader;
DX::DX11::GRAPHICAL::Shader ShaderManager::colShader;
DX::DX11::GRAPHICAL::Shader ShaderManager::linShader;
DX::DX11::GRAPHICAL::Shader ShaderManager::cirShader;

void ShaderManager::Initialize(void) {
	using namespace DX::DX11::CORE;
	using namespace DX::DX11::GRAPHICAL;

	SHADER_DESC desc;
	{
		D3D11_INPUT_ELEMENT_DESC elmDesc[] = {
			INPUTLAYOUT::Desc("POSITION", 0, FORMAT_FLOAT3),
			INPUTLAYOUT::Desc("TEXUV", 0, 12, FORMAT_FLOAT2),
		};
		SHADER_DESC desc;
		desc.desc = elmDesc;
		desc.descNum = 2;
		desc.vertexEntryPoint = "VS";
		desc.pixelEntryPoint = "PS";
		desc.vertexPath = desc.pixelPath = L"data/shader/color-shader.hlsl";
		colShader.Create(desc);
	}
	{
		D3D11_INPUT_ELEMENT_DESC elmDesc[] = {
			INPUTLAYOUT::Desc("POSITION", 0, FORMAT_FLOAT3),
			INPUTLAYOUT::Desc("TEXUV", 0, 12, FORMAT_FLOAT2),
		};
		SHADER_DESC desc;
		desc.desc = elmDesc;
		desc.descNum = 2;
		desc.vertexEntryPoint = "VS";
		desc.pixelEntryPoint = "PS";
		desc.vertexPath = desc.pixelPath = L"data/shader/texture-shader.hlsl";
		texShader.Create(desc);	
	}
	{
		D3D11_INPUT_ELEMENT_DESC elmDesc[] = {
			INPUTLAYOUT::Desc("POSITION", 0, FORMAT_FLOAT3),
			INPUTLAYOUT::Desc("TEXUV", 0, 12, FORMAT_FLOAT2),
		};
		SHADER_DESC desc;
		desc.desc = elmDesc;
		desc.descNum = 2;
		desc.vertexEntryPoint = "VS";
		desc.pixelEntryPoint = "PS";
		desc.vertexPath = desc.pixelPath = L"data/shader/line-shader.hlsl";
		linShader.Create(desc);
	}
	{
		D3D11_INPUT_ELEMENT_DESC elmDesc[] = {
			INPUTLAYOUT::Desc("POSITION", 0, FORMAT_FLOAT3),
			INPUTLAYOUT::Desc("TEXUV", 0, 12, FORMAT_FLOAT2),
		};
		SHADER_DESC desc;
		desc.desc = elmDesc;
		desc.descNum = 2;
		desc.vertexEntryPoint = "CircleVS";
		desc.pixelEntryPoint = "CirclePS";
		desc.vertexPath = desc.pixelPath = L"data/shader/ui-shader.hlsl";
		cirShader.Create(desc);
	}
}

void ShaderManager::Release(void) {
	texShader.Release();
	colShader.Release();
	linShader.Release();
	cirShader.Release();
}