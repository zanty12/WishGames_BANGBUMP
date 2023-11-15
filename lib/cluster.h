/*==============================================================================

	Title		: CLUSTER
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _CLUSTER_H_
#define _CLUSTER_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <string>
#include <list>
#include "behavior.h"

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
* enum�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class�錾
------------------------------------------------------------------------------*/
class Behavior;

class Cluster {
private:
	int count = 0;
	Behavior *head = nullptr;
	Behavior *tail = nullptr;

protected:
	std::list<Behavior *> creates;
	std::list<Behavior *> deletes;
	std::list<Behavior *> pushs;
	std::list<Behavior *> pops;

	friend Behavior;

protected:
	/// <summary>
	/// �|�b�v
	/// </summary>
	/// <param name="instance"></param>
	void pop(Behavior *instance);

	/// <summary>
	/// �v�b�V��
	/// </summary>
	void push(Behavior *instance);

public:
	virtual ~Cluster();

	/// <summary>
	/// �쐬
	/// </summary>
	template<class T, class... Args>
	T *Create(Args... args) {
		// �C���X�^���X�𐶐�����
		T *instance = new T(args...);
		Behavior *mono = instance;
		creates.push_back(instance);
		return instance;
	}


	Behavior *GetHead(void) { return head; }
	Behavior *GetTail(void) { return tail; }

	/// <summary>
	/// �J�n�֐�
	/// </summary>
	void Start(void);

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Draw(void);

	/// <summary>
	/// �x�X�V�֐�
	/// </summary>
	void LateUpdate(void);

	/// <summary>
	/// �I���֐�
	/// </summary>
	void End(void);

	/// <summary>
	/// ���
	/// </summary>
	/// <param name=""></param>
	void Release(void);

	/// <summary>
	/// �擾
	/// </summary>
	/// <param name="type">�^�C�v</param>
	Behavior *Get(std::string type);

	/// <summary>
	/// �v�f��
	/// </summary>
	int Count(void) { return count; }
};

#endif
