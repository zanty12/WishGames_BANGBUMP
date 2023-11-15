/*==============================================================================

	Title		: PRIMITIVE_BUFFER
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_PRIMITIVE_BUFFER_H_
#define _DX11_PRIMITIVE_BUFFER_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include "dx11_buffer.h"

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

		enum PRIMITIVE_SET_TYPE {
			PRIMITIVE_SET_TYPE_NONE,
			PRIMITIVE_SET_TYPE_COPY,
			PRIMITIVE_SET_TYPE_POINTER,
		};

		namespace CORE {
			/// <summary>
			/// �v���~�e�B�u�Ɋ֘A����o�b�t�@
			/// </summary>
			/// <typeparam name="T"></typeparam>
			template<class T>
			class PrimitiveBuffer : public Buffer {
			protected:
				T *pResource = nullptr;									// �o�b�t�@
				UINT resourceNum = 0;									// �o�b�t�@�̗v�f��
				PRIMITIVE_SET_TYPE setType = PRIMITIVE_SET_TYPE_NONE;	// �Z�b�g�^�C�v

			public:

				/// <summary>
				/// ���\�[�X�̍쐬
				/// </summary>
				/// <param name="pResource">���\�[�X</param>
				/// <param name="resourceNum">�v�f��</param>
				/// <param name="setType">�^�C�v</param>
				HRESULT CreateResource(T *pResource, UINT resourceNum, PRIMITIVE_SET_TYPE setType) {
					NULL_CHECK(pResource);
					CREATE_CHECK(this->pResource);

					if (setType == PRIMITIVE_SET_TYPE_NONE) return -1;
					this->resourceNum = resourceNum;
					this->setType = setType;

					// ���\�[�X�̍쐬
					switch (setType) {
					case PRIMITIVE_SET_TYPE_COPY:
						this->pResource = new T[resourceNum];
						memcpy(this->pResource, pResource, sizeof(T) * resourceNum);
						break;

					case PRIMITIVE_SET_TYPE_POINTER:
						this->pResource = pResource;
						break;
					}
					
					return 0;
				}

				/// <summary>
				/// ���\�[�X�̉��
				/// </summary>
				void Release(void) override {
					// ���
					Buffer::Release();
					if(setType == PRIMITIVE_SET_TYPE_COPY) 
						if (pResource) delete pResource;

					pResource = nullptr;
					resourceNum = 0;
					setType = PRIMITIVE_SET_TYPE_NONE;
				}

				/// <summary>
				/// �����̎擾
				/// </summary>
				unsigned int GetCount(void) { return resourceNum; }

				/// <summary>
				/// �擾
				/// </summary>
				/// <param name="index">�C���f�b�N�X</param>
				T *GetResource(UINT index) { if (index < resourceNum) return &pResource[index]; return nullptr; }

				/// <summary>
				/// �Z�b�g
				/// </summary>
				/// <param name="index">�C���f�b�N�X</param>
				/// <param name="resource">���\�[�X</param>
				void SetResource(UINT index, T resource) { if (index < resourceNum) pResource[index] = resource; }
			};
		}
	}
}

#endif
