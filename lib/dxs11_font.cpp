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

				// �t�H���g�f�[�^
				// https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-logfonta
				log.lfHeight = pixelFontSize;								// �t�H���g�̍���
				log.lfWidth = 0;										// �t�H���g�̕��i0�̏ꍇ�c���䂪�ƍ������j
				log.lfEscapement = 0;									// �E���x�N�g���ƃf�o�C�X�� x ���̊Ԃ̊p�x�� 10 ���� 1 �̊p�x�Ŏw��
				log.lfOrientation = 0;									// �e�����̊�{���ƃf�o�C�X�� x ���̊Ԃ̊p�x�� 10 ���� 1 �̊p�x�Ŏw��
				log.lfWeight = fontWeight;								// �t�H���g�̑���
				log.lfItalic = false;									// �Α̃t�H���g
				log.lfUnderline = false;								// �����t���t�H���g
				log.lfStrikeOut = false;								// ���������t�H���g
				log.lfCharSet = SHIFTJIS_CHARSET;						// �����Z�b�g�iSHIFT-JIS�j
				log.lfOutPrecision = OUT_TT_ONLY_PRECIS;				// �o�͐��x
				log.lfClipPrecision = CLIP_DEFAULT_PRECIS;				// �N���b�s���O���x
				log.lfQuality = PROOF_QUALITY;							// �o�͕i��
				log.lfPitchAndFamily = DEFAULT_PITCH | FF_MODERN;		// �s�b�`�E�t�@�~��
				strcpy(log.lfFaceName, fontName.c_str());				// �t�H���g�����R�s�[


				// �n���h���f�o�C�X�R���e�L�X�g����n���h���t�H���g���쐬
				HFONT hFont = CreateFontIndirectA(&log);
				HDC hdc = GetDC(NULL);
				HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
				unsigned int code = (UINT)wcode;						// �擾���镶���R�[�h

				// �r�b�g�}�b�v���擾
				TEXTMETRIC tm;
				GLYPHMETRICS gm;
				GetTextMetrics(hdc, &tm);
				const MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
				// �T�C�Y���擾����
				DWORD size = GetGlyphOutline(
					hdc,												// �n���h���f�o�C�X�R���e�L�X�g
					code,												// �擾���镶��
					GGO_GRAY4_BITMAP,									// �擾����f�[�^�`��
					&gm,												// �����̔z�u�ꏊ���擾
					0,													// �T�C�Y
					NULL,												// �t�H���g�����擾
					&Mat												// �����̕ϊ��s��
				);
				// �f�[�^���擾
				BYTE *data = new BYTE[size];
				GetGlyphOutline(
					hdc,												// �n���h���f�o�C�X�R���e�L�X�g
					code,												// �擾���镶��
					GGO_GRAY4_BITMAP,									// �擾����f�[�^�`��
					&gm,												// �����̔z�u�ꏊ���擾
					size,												// �T�C�Y
					data,												// �t�H���g�����擾
					&Mat												// �����̕ϊ��s��
				);

				// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̉��
				SelectObject(hdc, oldFont);
				DeleteObject(hFont);
				ReleaseDC(NULL, hdc);


				// �e�N�X�`���̍쐬
				D3D11_TEXTURE2D_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Width = gm.gmCellIncX;								// ��
				desc.Height = gm.gmBlackBoxY;							// ����
				desc.MipLevels = 1;										// �~�b�v���x��
				desc.ArraySize = 1;										// �z��T�C�Y
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// �t�H�[�}�b�g�iRGBA�F255,255,255,255�j
				desc.SampleDesc.Count = 1;
				desc.Usage = D3D11_USAGE_DYNAMIC;						// �g�p���@�i�������݋��j
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;			// �V�F�[�_�Ƃ��Ďg�p
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPU���珑�����݋���


				// ShaderResourceView�̏����쐬����
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				ZeroMemory(&srvDesc, sizeof(srvDesc));
				srvDesc.Format = desc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = desc.MipLevels;


				ID3D11Texture2D *texture = NULL;
				hr = Device3D::Get()->CreateTexture2D(&desc, 0, &texture);
				D3D11_MAPPED_SUBRESOURCE textureResource;
				// ��������
				hr = Device3D::GetContext()->Map(
					texture,
					0,
					D3D11_MAP_WRITE_DISCARD,
					0,
					&textureResource
				);
				BYTE *pData = (BYTE *)textureResource.pData;
				int width = (gm.gmBlackBoxX + 3) / 4 * 4;				// ��
				int height = gm.gmBlackBoxY;							// ����
				DWORD x, y;
				DWORD Alpha, Color;
				// �f�[�^�̎擾
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						Alpha = data[x + width * y] * 255;
						if (code == 0x20 || code == 0x3000)				//���p�S�p�X�y�[�X�͍����ɐ�������̂ŁAColor��0�ɂ���
							Color = 0x0;
						else
							Color = 0x00ffffff | (Alpha << 24);
						memcpy((BYTE *)pData + textureResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
					}
				}
				// �I��
				Device3D::GetContext()->Unmap(texture, 0);
				// �L�^
				this->tm = tm;
				this->gm = gm;

				// �t�H���g�V�F�[�_�̍쐬
				Device3D::Get()->CreateShaderResourceView(texture, &srvDesc, &resource);

				// �̈�̊J��
				delete[] data;

				texture->Release();
				texture = nullptr;
				return hr;
			}



			Font::Font(std::string fontName, unsigned int pixelFontSize) : fontName(fontName), m_pixelFontSize(pixelFontSize) {
				for (wchar_t w = 0; w <= L'z'; w++)
					half[(unsigned int)w].Load(fontName, w, pixelFontSize);

				for (wchar_t w = L'��'; w <= L'��'; w++)
					hiragana[w - L'��'].Load(fontName, w, pixelFontSize);

				for (wchar_t w = L'�@'; w <= L'��'; w++)
					katakana[w - L'�@'].Load(fontName, w, pixelFontSize);
			}

			Font::~Font() {
				for (Character *character : this->character) {
					delete character;
				}
			}

			Font::Character *Font::operator[](wchar_t wcode) {
				int i = (wcode - L'�@');

				// ���p
				if(wcode <= 'z')
					return &half[wcode];
				// �� ~ ��
				else if(wcode <= L'��')
					return &hiragana[wcode - L'��'];
				// �A ~ ��
				else if(wcode <= L'��')
					return &katakana[wcode - L'�@'];
				// ���̑�
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
