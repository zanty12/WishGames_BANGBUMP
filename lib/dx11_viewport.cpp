#include "dx11_viewport.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			VIEWPORT::VIEWPORT(HWND hwnd) {
				RECT rect;
				GetClientRect(hwnd, &rect);

				viewport.TopLeftX = 0.0f;
				viewport.TopLeftY = 0.0f;
				viewport.Width = rect.right - rect.left;
				viewport.Height = rect.bottom - rect.top;
				viewport.MinDepth = 0.0f;
				viewport.MaxDepth = 1.0f;
			}

			VIEWPORT::VIEWPORT(Vector2 leftTop, Vector2 size) {
				viewport.TopLeftX = leftTop.x;
				viewport.TopLeftY = leftTop.y;
				viewport.Width = size.x;
				viewport.Height = size.y;
				viewport.MinDepth = 0.0f;
				viewport.MaxDepth = 1.0f;
			}
		}
	}
}
