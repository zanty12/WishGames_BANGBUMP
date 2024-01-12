#pragma once
#include "graphical.h"
#include <dwrite.h>
#include <d2d1.h>
#include <d2d1helper.h>

class Text
{
private:
    //directwrite
    static IDWriteFactory* pDWriteFactory_ = nullptr;
    static IDWriteTextFormat* pTextFormat_ = nullptr;
    //D2D
    static ID2D1Factory* pD2DFactory_ = nullptr;
    static ID2D1RenderTarget* pRT_ = nullptr;
    static ID2D1SolidColorBrush* pSolidBrush_ = nullptr;
    static IDXGISurface* pBackBuffer_ = nullptr;

    //text formats
    static std::wstring font_ = L"メイリオ";
    static float font_size_;
    static DWRITE_FONT_WEIGHT font_weight_;
    static DWRITE_FONT_STYLE font_style_;
    static Color font_color_;

public:
    static HRESULT CreateResources();
    static void DiscardResources();
    //左上原点
    static void TextStart();
    static void TextEnd();
    static void WriteText(const WCHAR* text, float X, float Y, float Width, float Height);
    static HRESULT ChangeFont(const std::wstring font);
    static HRESULT ChangeFontSize(int size);
    static HRESULT SetFontColor(Color color);
    static HRESULT SetFontWeight(DWRITE_FONT_WEIGHT weight);
    static HRESULT SetFontStyle(DWRITE_FONT_STYLE style);

};
