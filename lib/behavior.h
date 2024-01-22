/*==============================================================================

	Title		: BEHAVIOR
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _BEHAVIOR_H_
#define _BEHAVIOR_H_


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


class Behavior {
private:
	bool isDestroy = false;							// �폜�֐�

private:
	int priority = 0;								// �D��x
	Behavior *previous = nullptr;					// �O�|�C���^
	Behavior *next = nullptr;						// ���|�C���^
	Behavior *high = nullptr;						// ���D��x
	Behavior *low = nullptr;						// ��D��x
	Behavior *parent = nullptr;						// �e�I�u�W�F�N�g
	Behavior *head = nullptr;						// �q�I�u�W�F�N�g�̐擪�|�C���^
	Behavior *tail = nullptr;						// �q�I�u�W�F�N�g�̖����|�C���^
	int count = 0;
	std::list<Behavior *> creates;
	std::list<Behavior *> deletes;

public:
	std::string name;								// ���O



private:
	void push_left(Behavior *instance);
	void push_right(Behavior *instance);
	void push_up(Behavior *instance);
	void push_down(Behavior *instance);


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
	class iterator {
	private:
		Behavior *instance = nullptr;


	public:
		iterator() = default;
		iterator(Behavior *instance) : instance(instance) { }
		iterator operator++() { return iterator(instance = instance->next); }
		iterator operator--() { return iterator(instance = instance->previous); }
		iterator operator++(int) { return iterator(instance = instance->next); }
		iterator operator--(int) { return iterator(instance = instance->previous); }
		bool operator==(iterator i) { return instance == i.instance; }
		bool operator!=(iterator i) { return instance != i.instance; }
		Behavior &operator*() { return *instance; }
		Behavior *operator->() { return instance; }
	};

	Behavior() { }
	Behavior(std::string name) : name(name) { }
	Behavior(int priority) : priority(priority) { }
	Behavior(std::string name, int priority) : name(name), priority(priority) { }
	virtual ~Behavior();

	// �쐬
	template<class T, class... Args>
	T *Add(Args... args) {
		// �C���X�^���X�𐶐�����
		T *instance = new T(args...);
		Behavior *mono = instance;
		creates.push_back(instance);
		return instance;
	}
	// �L���X�g
	template<class T>
	T *Cast(void) { return (T *)this; }
	std::list<Behavior *> Find(Behavior *(*func)(Behavior *instance));
	void Destroy(void);
	void AllLoop(void);
	void SetParent(Behavior *parent);
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
