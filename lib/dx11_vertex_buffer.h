/*==============================================================================

	Title		: VERTEX_BUFFER
	Description : ���_�\����
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_VERTEX_BUFFER_H_
#define _DX11_VERTEX_BUFFER_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include "dx11_primitive_buffer.h"
#include "dx11_vertex_struct.h"

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
		namespace CORE {
			/// <summary>
			/// ���_�o�b�t�@
			/// </summary>
			/// <typeparam name="T"></typeparam>
			template <class T>
			class VertexBuffer : public PrimitiveBuffer<T> {
			public:

				/// <summary>
				/// ���_�o�b�t�@�̍쐬
				/// </summary>
				/// <param name="vertex">���_�z��</param>
				/// <param name="vertexNum">���_�z��̗v�f��</param>
				/// <param name="device">�f�o�C�X�|�C���^�[</param>
				/// <returns></returns>
				HRESULT Create(T *vertex, UINT vertexNum, PRIMITIVE_SET_TYPE setType, ID3D11Device *device) {
					NULL_CHECK(device);
					HRESULT hr;

					// �T�u���\�[�X
					D3D11_SUBRESOURCE_DATA desc;
					desc.pSysMem = vertex;
					desc.SysMemPitch = sizeof(T);
					desc.SysMemSlicePitch = 0;

					// �o�b�t�@�̍쐬
					UINT vertexSize = sizeof(T) * vertexNum;
					hr = Buffer::Create(Buffer::VertexDesc(vertexSize), &desc, device);
					ERROR_CHECK(hr, "Error create buffer.");

					// ���\�[�X�̍쐬
					hr = PrimitiveBuffer<T>::CreateResource(vertex, vertexNum, setType);

					return hr;
				}
			};
		}
	}
}

#endif
