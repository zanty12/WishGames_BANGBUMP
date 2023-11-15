#pragma once
#include "lib/dxlib.h"

class ShaderManager {
private:
	static DX::DX11::GRAPHICAL::Shader texShader;
	static DX::DX11::GRAPHICAL::Shader colShader;

public:
	static void Initialize(void);
	static void Release(void);

	static void SetTextureMode(void) { texShader.Update(); }
	static void SetColorMode(void) { colShader.Update(); }
};