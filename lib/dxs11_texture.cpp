#include "dxs11_texture.h"

namespace DX {
	namespace DX11 {
		namespace GRAPHICAL {
			HRESULT Texture::Load(char *path) {
				if (resource) Release();
				HRESULT hr = S_OK;
	
				DirectX::ScratchImage image = DirectX::ScratchImage();

				// マルチバイト文字列からワイド文字列へ変換
				setlocale(LC_CTYPE, "jpn");
				wchar_t wFilename[256];
				size_t ret;
				mbstowcs_s(&ret, wFilename, path, 256);

				// WIC画像を読み込む
				hr = DirectX::LoadFromWICFile(wFilename, DirectX::WIC_FLAGS_NONE, &info, image);
				if (hr < 0)image.Release();
				ERROR_CHECK(hr, "Error filepath.");

				hr = DirectX::CreateShaderResourceView(Device3D::Get(), image.GetImages(), image.GetImageCount(), info, &resource);
				image.Release();

				return hr;
			}

			HRESULT Texture::Load(const char *path) {
				return Load((char *)path);
			}
		}
	}
}