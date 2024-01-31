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
* include宣言
------------------------------------------------------------------------------*/
#include <string>

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
* enum宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
#include <string>
#include <list>


class MultiBehavior {
private:
	bool isDestroy = false;							// 削除関数

private:
	int priority = 0;								// 優先度
	MultiBehavior *previous = nullptr;					// 前ポインタ
	MultiBehavior *next = nullptr;						// 次ポインタ
	MultiBehavior *high = nullptr;						// 高優先度
	MultiBehavior *low = nullptr;						// 低優先度
	MultiBehavior *parent = nullptr;						// 親オブジェクト
	MultiBehavior *head = nullptr;						// 子オブジェクトの先頭ポインタ
	MultiBehavior *tail = nullptr;						// 子オブジェクトの末尾ポインタ
	int count = 0;
	std::list<MultiBehavior *> creates;
	std::list<MultiBehavior *> deletes;

public:
	std::string name;								// 名前



private:
	void push_left(MultiBehavior *instance);
	void push_right(MultiBehavior *instance);
	void push_up(MultiBehavior *instance);
	void push_down(MultiBehavior *instance);


	/// <summary>
	/// ポップ
	/// </summary>
	/// <param name="instance"></param>
	void pop(MultiBehavior *instance);

	/// <summary>
	/// プッシュ
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

	// 作成
	template<class T, class... Args>
	T *Add(Args... args) {
		// インスタンスを生成する
		T *instance = new T(args...);
		MultiBehavior *mono = instance;
		creates.push_back(instance);
		return instance;
	}
	// キャスト
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
