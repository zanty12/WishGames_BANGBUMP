#include "text.h"

ID2D1RenderTarget* g_RT = NULL;
ID2D1SolidColorBrush* g_Brush = NULL;
IDWriteTextFormat* g_TextFormat = NULL;

HRESULT Text::CreateResources()
{
    HRESULT hr = S_OK;
    DX::DX11::Renderer renderer = Graphical::GetRenderer();
    IDXGISwapChain* swapChain = renderer.GetSwapchain();

    // Direct2D,DirectWriteの初期化
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_);
    if (FAILED(hr))
        return hr;

    hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer_));
    if (FAILED(hr))
        return hr;

    FLOAT dpiX;
    FLOAT dpiY;
    pD2DFactory_->GetDesktopDpi(&dpiX, &dpiY);

    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                                                                       D2D1::PixelFormat(
                                                                           DXGI_FORMAT_UNKNOWN,
                                                                           D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

    hr = pD2DFactory_->CreateDxgiSurfaceRenderTarget(pBackBuffer_, &props, &pRT_);
    if (FAILED(hr))
        return hr;

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                             reinterpret_cast<IUnknown**>(&pDWriteFactory_));
    if (FAILED(hr))
        return hr;

    //関数CreateTextFormat()
    //第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
    //第2引数：フォントコレクション（nullptr）
    //第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
    //第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
    //第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
    //第6引数：フォントサイズ（20, 30等）
    //第7引数：ロケール名（L""）
    //第8引数：テキストフォーマット（&pTextFormat_）
    hr = pDWriteFactory_->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                                           DWRITE_FONT_STRETCH_NORMAL, 20, L"", &pTextFormat_);
    if (FAILED(hr))
        return hr;

    //関数SetTextAlignment()
    //第1引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, DWRITE_TEXT_ALIGNMENT_TRAILING：後, DWRITE_TEXT_ALIGNMENT_CENTER：中央,
    //                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED：行いっぱい）
    hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    if (FAILED(hr))
        return hr;

    //関数CreateSolidColorBrush()
    //第1引数：フォント色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定）
    hr = pRT_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pSolidBrush_);
    if (FAILED(hr))
        return hr;


    return S_OK;
}

void Text::DiscardResources()
{
    if (pBackBuffer_) pBackBuffer_->Release();
    if (pSolidBrush_) pSolidBrush_->Release();
    if (pRT_) pRT_->Release();
    if (pTextFormat_) pTextFormat_->Release();
    if (pDWriteFactory_) pDWriteFactory_->Release();
    if (pD2DFactory_) pD2DFactory_->Release();
}

void Text::TextStart()
{
    pRT_->BeginDraw();
}

void Text::TextEnd()
{
    pRT_->EndDraw();
}

void Text::WriteText(const WCHAR* text, float X, float Y, float Width, float Height)
{
    pRT_->DrawText(text, wcslen(text), pTextFormat_, D2D1::RectF(X, Y, X + Width, Y + Height), pSolidBrush_);
}

HRESULT Text::ChangeFont(const std::wstring font)
{
    font_ = font;
    //release TextFormat
    if (pTextFormat_) pTextFormat_->Release();
    //create new TextFormat
    HRESULT hr = pDWriteFactory_->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                                           DWRITE_FONT_STRETCH_NORMAL, font_size_, L"", &pTextFormat_);
    return hr;
}

HRESULT Text::ChangeFontSize(int size)
{
    font_size_ = size;
    //release TextFormat
    if (pTextFormat_) pTextFormat_->Release();
    //create new TextFormat
    HRESULT hr = pDWriteFactory_->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                                           DWRITE_FONT_STRETCH_NORMAL, size, L"", &pTextFormat_);
    return hr;
}

HRESULT Text::SetFontColor(Color color)
{
    font_color_ = color;
    //release brush
    if (pSolidBrush_) pSolidBrush_->Release();
    //create new brush
    HRESULT hr = pRT_->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b, color.a), &pSolidBrush_);
    return hr;
}

HRESULT Text::SetFontWeight(DWRITE_FONT_WEIGHT weight)
{
    font_weight_ = weight;
    //release TextFormat
    if (pTextFormat_) pTextFormat_->Release();
    //create new TextFormat
    HRESULT hr = pDWriteFactory_->CreateTextFormat(L"メイリオ", nullptr, weight, DWRITE_FONT_STYLE_NORMAL,
                                           DWRITE_FONT_STRETCH_NORMAL, font_size_, L"", &pTextFormat_);
    return hr;
}