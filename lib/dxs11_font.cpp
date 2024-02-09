#include "dxs11_font.h"

namespace DX {
	namespace DX11 {
		namespace GRAPHICAL {
			HRESULT Font::Character::Load(std::string fontName, wchar_t wcode, unsigned int pixelFontSize) {
				CREATE_CHECK(resource);
				HRESULT hr;

				LOGFONTA log;
				int fontWeight = 0;
				m_wcode = wcode;

				// フォントデータ
				// https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-logfonta
				log.lfHeight = pixelFontSize;								// フォントの高さ
				log.lfWidth = 0;										// フォントの幅（0の場合縦横比が照合される）
				log.lfEscapement = 0;									// 脱走ベクトルとデバイスの x 軸の間の角度を 10 分の 1 の角度で指定
				log.lfOrientation = 0;									// 各文字の基本線とデバイスの x 軸の間の角度を 10 分の 1 の角度で指定
				log.lfWeight = fontWeight;								// フォントの太さ
				log.lfItalic = false;									// 斜体フォント
				log.lfUnderline = false;								// 下線付きフォント
				log.lfStrikeOut = false;								// 取り消し線フォント
				log.lfCharSet = SHIFTJIS_CHARSET;						// 文字セット（SHIFT-JIS）
				log.lfOutPrecision = OUT_TT_ONLY_PRECIS;				// 出力精度
				log.lfClipPrecision = CLIP_DEFAULT_PRECIS;				// クリッピング精度
				log.lfQuality = PROOF_QUALITY;							// 出力品質
				log.lfPitchAndFamily = DEFAULT_PITCH | FF_MODERN;		// ピッチ・ファミリ
				strcpy(log.lfFaceName, fontName.c_str());				// フォント名をコピー


				// ハンドルデバイスコンテキストからハンドルフォントを作成
				HFONT hFont = CreateFontIndirectA(&log);
				HDC hdc = GetDC(NULL);
				HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
				unsigned int code = (UINT)wcode;						// 取得する文字コード

				// ビットマップを取得
				TEXTMETRIC tm;
				GLYPHMETRICS gm;
				GetTextMetrics(hdc, &tm);
				const MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
				// サイズを取得する
				DWORD size = GetGlyphOutline(
					hdc,												// ハンドルデバイスコンテキスト
					code,												// 取得する文字
					GGO_GRAY4_BITMAP,									// 取得するデータ形式
					&gm,												// 文字の配置場所を取得
					0,													// サイズ
					NULL,												// フォント情報を取得
					&Mat												// 文字の変換行列
				);
				// データを取得
				BYTE *data = new BYTE[size];
				GetGlyphOutline(
					hdc,												// ハンドルデバイスコンテキスト
					code,												// 取得する文字
					GGO_GRAY4_BITMAP,									// 取得するデータ形式
					&gm,												// 文字の配置場所を取得
					size,												// サイズ
					data,												// フォント情報を取得
					&Mat												// 文字の変換行列
				);

				// デバイスコンテキストとフォントハンドルの解放
				SelectObject(hdc, oldFont);
				DeleteObject(hFont);
				ReleaseDC(NULL, hdc);


				// テクスチャの作成
				D3D11_TEXTURE2D_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Width = gm.gmCellIncX;								// 幅
				desc.Height = gm.gmBlackBoxY;							// 高さ
				desc.MipLevels = 1;										// ミップレベル
				desc.ArraySize = 1;										// 配列サイズ
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// フォーマット（RGBA：255,255,255,255）
				desc.SampleDesc.Count = 1;
				desc.Usage = D3D11_USAGE_DYNAMIC;						// 使用方法（書き込み許可）
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;			// シェーダとして使用
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPUから書き込み許可


				// ShaderResourceViewの情報を作成する
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				ZeroMemory(&srvDesc, sizeof(srvDesc));
				srvDesc.Format = desc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = desc.MipLevels;


				ID3D11Texture2D *texture = NULL;
				hr = Device3D::Get()->CreateTexture2D(&desc, 0, &texture);
				D3D11_MAPPED_SUBRESOURCE textureResource;
				// 書き込む
				hr = Device3D::GetContext()->Map(
					texture,
					0,
					D3D11_MAP_WRITE_DISCARD,
					0,
					&textureResource
				);
				BYTE *pData = (BYTE *)textureResource.pData;
				int width = (gm.gmBlackBoxX + 3) / 4 * 4;				// 幅
				int height = gm.gmBlackBoxY;							// 高さ
				DWORD x, y;
				DWORD Alpha, Color;
				// データの取得
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						Alpha = data[x + width * y] * 255;
						if (code == 0x20 || code == 0x3000)				//半角全角スペースは左下に線が入るので、Colorを0にする
							Color = 0x0;
						else
							Color = 0x00ffffff | (Alpha << 24);
						memcpy((BYTE *)pData + textureResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
					}
				}
				// 終了
				Device3D::GetContext()->Unmap(texture, 0);
				// 記録
				this->tm = tm;
				this->gm = gm;

				// フォントシェーダの作成
				Device3D::Get()->CreateShaderResourceView(texture, &srvDesc, &resource);

				// 領域の開放
				delete[] data;

				texture->Release();
				texture = nullptr;
				return hr;
			}



			Font::Font(std::string fontName, unsigned int pixelFontSize) : fontName(fontName), m_pixelFontSize(pixelFontSize) {
				for (wchar_t w = 0; w <= L'z'; w++)
					half[(unsigned int)w].Load(fontName, w, pixelFontSize);

				for (wchar_t w = L'ぁ'; w <= L'ん'; w++)
					hiragana[w - L'ぁ'].Load(fontName, w, pixelFontSize);

				for (wchar_t w = L'ァ'; w <= L'ン'; w++)
					katakana[w - L'ァ'].Load(fontName, w, pixelFontSize);
			}

			Font::~Font() {
				for (Character *character : this->character) {
					delete character;
				}
			}

			Font::Character *Font::operator[](wchar_t wcode) {
				int i = (wcode - L'ァ');

				// 半角
				if(wcode <= 'z')
					return &half[wcode];
				// あ ~ ん
				else if(wcode <= L'ん')
					return &hiragana[wcode - L'ぁ'];
				// ア ~ ン
				else if(wcode <= L'ン')
					return &katakana[wcode - L'ァ'];
				// その他
				else {
					for (Character *font : character)
						if (font->wcode == wcode) return font;

					Character *createFont = new Character(fontName, wcode, m_pixelFontSize);
					character.push_back(createFont);
					return createFont;
				}
			}
		}
	}
}
