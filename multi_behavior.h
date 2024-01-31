/*==============================================================================

	Title		: BEHAVIOR
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _MULTI_BEHAVIOR_H_
#define _MULTI_BEHAVIOR_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <string>

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
#include <string>
#include <list>


class MultiBehavior {
private:
	bool isDestroy = false;							// �폜�֐�

private:
	int priority = 0;								// �D��x
	MultiBehavior *previous = nullptr;					// �O�|�C���^
	MultiBehavior *next = nullptr;						// ���|�C���^
	MultiBehavior *high = nullptr;						// ���D��x
	MultiBehavior *low = nullptr;						// ��D��x
	MultiBehavior *parent = nullptr;						// �e�I�u�W�F�N�g
	MultiBehavior *head = nullptr;						// �q�I�u�W�F�N�g�̐擪�|�C���^
	MultiBehavior *tail = nullptr;						// �q�I�u�W�F�N�g�̖����|�C���^
	int count = 0;
	std::list<MultiBehavior *> creates;
	std::list<MultiBehavior *> deletes;

public:
	std::string name;								// ���O



private:
	void push_left(MultiBehavior *instance);
	void push_right(MultiBehavior *instance);
	void push_up(MultiBehavior *instance);
	void push_down(MultiBehavior *instance);


	/// <summary>
	/// �|�b�v
	/// </summary>
	/// <param name="instance"></param>
	void pop(MultiBehavior *instance);

	/// <summary>
	/// �v�b�V��
	/// </summary>
	void push(MultiBehavior *instance);



public:
	class iterator {
	private:
		MultiBehavior *instance = nullptr;


	public:
		iterator() = default;
		iterator(MultiBehavior *instance) : instance(instance) { }
		iterator operator++() { return iterator(instance = instance->next); }
		iterator operator--() { return iterator(instance = instance->previous); }
		iterator operator++(int) { return iterator(instance = instance->next); }
		iterator operator--(int) { return iterator(instance = instance->previous); }
		bool operator==(iterator i) { return instance == i.instance; }
		bool operator!=(iterator i) { return instance != i.instance; }
		MultiBehavior &operator*() { return *instance; }
		MultiBehavior *operator->() { return instance; }
	};

	MultiBehavior() { }
	MultiBehavior(std::string name) : name(name) { }
	MultiBehavior(int priority) : priority(priority) { }
	MultiBehavior(std::string name, int priority) : name(name), priority(priority) { }
	virtual ~MultiBehavior();

	// �쐬
	template<class T, class... Args>
	T *Add(Args... args) {
		// �C���X�^���X�𐶐�����
		T *instance = new T(args...);
		MultiBehavior *mono = instance;
		creates.push_back(instance);
		return instance;
	}
	// �L���X�g
	template<class T>
	T *Cast(void) { return (T *)this; }
	std::list<MultiBehavior *> Find(MultiBehavior *(*func)(MultiBehavior *instance));
	void Destroy(void);
	void AllLoop(void);
	void SetParent(MultiBehavior *parent);
	int Count(void) { return count; }

	iterator begin(void) { return iterator(head); }
	iterator end(void) { return tail ? iterator(tail->next) : iterator(); }
	iterator get_parent(void) { return iterator(parent); }
	iterator get_next(void) { return iterator(next); }
	iterator get_previous(void) { return iterator(previous); }

	virtual void Initialize(void) { };
	virtual void Release(void) { };
	virtual void Loop(void) { };
};

#endif
