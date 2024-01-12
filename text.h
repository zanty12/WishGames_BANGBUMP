#pragma once
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#include "graphical.h"
#include <dwrite.h>
#include <d2d1.h>
#include "dwrite_3.h"
#include <d2d1helper.h>

class Text
{
private:
    //directwrite
    static IDWriteFactory5* pDWriteFactory_;
    static IDWriteTextFormat* pTextFormat_;
    static IDWriteFontSetBuilder1* pFontSetBuilder_;
    static IDWriteFontCollection1* pFontCollection_;
    static IDWriteFontSet* pFontSet_;
    static IDWriteFontFile* pFontFile_;
    //D2D
    static ID2D1Factory* pD2DFactory_;
    static ID2D1RenderTarget* pRT_;
    static ID2D1SolidColorBrush* pSolidBrush_;
    static IDXGISurface* pBackBuffer_;

    //text formats
    static std::wstring font_;
    static float font_size_;
    static DWRITE_FONT_WEIGHT font_weight_;
    static DWRITE_FONT_STYLE font_style_;
    static Color font_color_;

public:
    static HRESULT CreateResources();
    static void DiscardResources();
    static void TextStart();
    static void TextEnd();

    //左上原点
    static void WriteText(const WCHAR* text, float X, float Y, float Width, float Height);

    //スタイル変更
    static HRESULT ChangeFont(const std::wstring font);
    static HRESULT ChangeFontSize(int size);
    static HRESULT SetFontColor(Color color);
    static HRESULT SetFontWeight(DWRITE_FONT_WEIGHT weight);
    static HRESULT SetFontStyle(DWRITE_FONT_STYLE style);

};
