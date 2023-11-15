/*==============================================================================

	Title		: FONT
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_GRAPHICAL_FONT_H_
#define _DX11_GRAPHICAL_FONT_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
//#include <DirectXTex.h>
#include <string>
#include <list>
#include "dx11_device3d.h"
#include "dxs11_resource.h"

/*------------------------------------------------------------------------------
* define宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		namespace GRAPHICAL {
			class Font {
			public:
				class Character : public Resource {
				private:
					wchar_t m_wcode = 0;
					TEXTMETRIC tm = {};
					GLYPHMETRICS gm = {};

				public:
					get<wchar_t> wcode = m_wcode;

				public:
					Character() { }
					Character(std::string fontName, wchar_t wcode, unsigned int pixelFontSize) { Load(fontName, wcode, pixelFontSize); }
					~Character() { Release(); }

					const GLYPHMETRICS GetGM(void) { return gm; }
					const TEXTMETRIC GetTM(void) { return tm; }
					Vector2 GetPosition(void) { return Vector2(gm.gmptGlyphOrigin.x, tm.tmHeight + gm.gmptGlyphOrigin.y - tm.tmDescent); }
					Vector2 GetScale(void) { return Vector2(gm.gmCellIncX, gm.gmBlackBoxY); }
					float GetWidth(void) { return gm.gmptGlyphOrigin.x + gm.gmCellIncX; }

					/// <summary>
					/// ロード
					/// </summary>
					/// <param name="fontname">フォント名</param>
					/// <param name="wcode">文字コード</param>
					/// <param name="fontSize">フォントサイズ</param>
					/// <returns></returns>
					HRESULT Load(std::string fontName, wchar_t wcode, unsigned int pixelFontSize);
				};





			private:
				std::string fontName = "";
				unsigned int m_pixelFontSize = 0;
				std::list<Character*> character;
				Character half[123] = {};
				Character hiragana[84] = {};
				Character katakana[84] = {};
				
			public:
				get<unsigned int>pixelFontSize = m_pixelFontSize;


			public:
				Font(std::string fontName, unsigned int pixelFontSize);
				~Font();

				Character *operator[] (wchar_t wcode);
			};
		}
	}
}

#endif
