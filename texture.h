#pragma once
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <DirectXTex.h>
#include <string>
#include "lib/stdlib.h"
#include "lib/dxlib.h"

using namespace DX::DX11::GRAPHICAL;


int LoadTexture(std::string path);

Texture *GetTexture(int index);

void AllReleaseTexture(void);

#endif