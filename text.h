#pragma once
#include "graphical.h"
#include <dwrite.h>
#include <d2d1.h>
#include <d2d1helper.h>

class Text
{
private:
    //directwrite
    IDWriteFactory* pDWriteFactory_ = nullptr;
    IDWriteTextFormat* pTextFormat_ = nullptr;
    //D2D
    ID2D1Factory* pD2DFactory_ = nullptr;
    ID2D1RenderTarget* pRT_ = nullptr;
    ID2D1SolidColorBrush* pSolidBrush_ = nullptr;

public:
    HRESULT CreateResources();
    void WriteText(const WCHAR* text, float X, float Y, float Width, float Height);

};
