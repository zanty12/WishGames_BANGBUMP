/*==============================================================================

	Title		: PRIMITIVE
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_PRIMITIVE_H_
#define _DX11_PRIMITIVE_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <string.h>
#include "dx11_vertex_buffer.h"
#include "dx11_index_buffer.h"

/*------------------------------------------------------------------------------
* define�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class�錾
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		namespace GRAPHICAL {
			enum BUFFER_TYPE {
				BUFFER_TYPE_NONE,
				BUFFER_TYPE_VERTEX_ONLY,
				BUFFER_TYPE_VERTEX_AND_INDEX
			};

			template<class T>
			class Primitive {
			private:
				CORE::VertexBuffer<T> vertexBuffer;			// ���_�o�b�t�@
				CORE::IndexBuffer indexBuffer;					// �C���f�b�N�X�o�b�t�@

			public:

				~Primitive() { Release(); }

				/// <summary>
				/// �o�b�t�@�̍쐬
				/// </summary>
				/// <param name="vertex">���_�o�b�t�@�z��</param>
				/// <param name="vertexNum">���_�o�b�t�@��</param>
				/// <param name="index">�C���f�b�N�X�o�b�t�@�z��</param>
				/// <param name="indexNum">�C���f�b�N�X�o�b�t�@��</param>
				/// <param name="setType">�^�C�v</param>
				/// <returns></returns>
				HRESULT Create(T *vertex, UINT vertexNum, DWORD *index, UINT indexNum, PRIMITIVE_SET_TYPE setType) {
					NULL_CHECK(vertex);
					CREATE_CHECK((ID3D11Buffer *)vertexBuffer);
					CREATE_CHECK((ID3D11Buffer *)indexBuffer);

					// �o�b�t�@�̍쐬
					if (vertex) vertexBuffer.Create(vertex, vertexNum, setType, Device3D::Get());
					if (index) indexBuffer.Create(index, indexNum, setType, Device3D::Get());
					
					return 0;
				}

				/// <summary>
				/// ���
				/// </summary>
				void Release(void) {
					vertexBuffer.Release();
					indexBuffer.Release();
				}

				/// <summary>
				/// �X�V
				/// </summary>
				void Update(void) {
					ID3D11Buffer *vertexBuffer = GetVertexBuffer();
					ID3D11Buffer *indexBuffer = GetIndexBuffer();
					// ���_
					if (vertexBuffer) {
						UINT stride = sizeof(T);																// 1���_�̗̈�
						UINT offset = 0;																			// 1���_�̎g�p���Ȃ��̈�i�_�~�[�j
						// ���_�o�b�t�@�̓���
						Device3D::GetContext()->IASetVertexBuffers(
							0,
							1,
							&vertexBuffer,																			// ���_�o�b�t�@
							&stride,																				// (stride + offset)�̃T�C�Y���|�C���^�����炷
							&offset
						);

						// ���_���\�[�X
						D3D11_MAPPED_SUBRESOURCE vertexResource;
						Device3D::GetContext()->Map(
							vertexBuffer,																			// ���_�o�b�t�@
							0,																						// �T�u���\�[�X
							D3D11_MAP_WRITE_DISCARD,																// �}�b�v�^�C�v�i�o�b�t�@�ɏ������ނ��߂�GPU�̃A�N�Z�X�����ۂ���j
							0,																						// �t���O
							&vertexResource																			// ���\�[�X
						);

						// ���_�f�[�^�̎擾
						T *fromVertex = GetVertexPointer();
						T *toVertex = (T *)vertexResource.pData;
						// �f�[�^�̃R�s�[
						memcpy(toVertex, fromVertex, sizeof(T) * GetVertexCount());


						// GPU�ɃA�N�Z�X��������
						Device3D::GetContext()->Unmap(vertexBuffer, 0);
					}
					// �C���f�b�N�X
					if (indexBuffer) {
						UINT offset = 0;																			// 1�C���f�b�N�X�̎g�p���Ȃ��̈�i�_�~�[�j
						ID3D11Buffer *indexBuffer = GetIndexBuffer();

						// ���_�o�b�t�@�̓���
						Device3D::GetContext()->IASetIndexBuffer(
							indexBuffer,																			// �C���f�b�N�X�o�b�t�@
							DXGI_FORMAT_R16_UINT,																	// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
							offset																					// offset�̃_�~�[�����炷
						);

						// �C���f�b�N�X���\�[�X
						D3D11_MAPPED_SUBRESOURCE indexResource;
						Device3D::GetContext()->Map(
							indexBuffer,																			// �C���f�b�N�X�o�b�t�@
							0,																						// �T�u���\�[�X
							D3D11_MAP_WRITE_DISCARD,																// �}�b�v�^�C�v�i�o�b�t�@�ɏ������ނ��߂�GPU�̃A�N�Z�X�����ۂ���j
							0,																						// �t���O
							&indexResource																			// ���\�[�X
						);

						// �C���f�b�N�X�f�[�^�̎擾
						DWORD *fromIndex = (DWORD *)GetIndexPointer();
						DWORD *toIndex = (DWORD *)indexResource.pData;
						// �f�[�^�̃R�s�[
						memcpy(toIndex, fromIndex, sizeof(DWORD) * GetIndexCount());

						// GPU�ɃA�N�Z�X��������
						Device3D::GetContext()->Unmap(indexBuffer, 0);
					}
				}

				/// <summary>
				/// ���_���̎擾
				/// </summary>
				/// <returns></returns>
				UINT GetVertexCount(void) { return vertexBuffer.GetCount(); }

				/// <summary>
				/// �C���f�b�N�X���̎擾
				/// </summary>
				/// <returns></returns>
				UINT GetIndexCount(void) { return indexBuffer.GetCount(); }

				/// <summary>
				/// ���_�\���̂̃T�C�Y�̎擾
				/// </summary>
				/// <returns></returns>
				UINT GetVertexStructByteSize(void) { return sizeof(T); }

				/// <summary>
				/// �C���f�b�N�X�\���̂̃T�C�Y�̎擾
				/// </summary>
				/// <returns></returns>
				UINT GetIndexStructByteSize(void) { return sizeof(DWORD); }

				/// <summary>
				/// ���_�o�b�t�@�̎擾
				/// </summary>
				/// <returns></returns>
				ID3D11Buffer *GetVertexBuffer(void) { return vertexBuffer; }

				/// <summary>
				/// �C���f�b�N�X�o�b�t�@�̎擾
				/// </summary>
				/// <returns></returns>
				ID3D11Buffer *GetIndexBuffer(void) { return indexBuffer; }

				/// <summary>
				/// ���_�|�C���^�̎擾
				/// </summary>
				/// <returns></returns>
				T *GetVertexPointer(void) { return vertexBuffer.GetResource(0); }

				/// <summary>
				/// �C���f�b�N�X�|�C���^�̎擾
				/// </summary>
				/// <returns></returns>
				DWORD *GetIndexPointer(void) { return indexBuffer.GetResource(0); }
			};
		}
	}
}

#endif
