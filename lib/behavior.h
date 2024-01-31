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


class Behavior {
private:
	bool isDestroy = false;							// 削除関数

private:
	int priority = 0;								// 優先度
	Behavior *previous = nullptr;					// 前ポインタ
	Behavior *next = nullptr;						// 次ポインタ
	Behavior *high = nullptr;						// 高優先度
	Behavior *low = nullptr;						// 低優先度
	Behavior *parent = nullptr;						// 親オブジェクト
	Behavior *head = nullptr;						// 子オブジェクトの先頭ポインタ
	Behavior *tail = nullptr;						// 子オブジェクトの末尾ポインタ
	int count = 0;
	std::list<Behavior *> creates;
	std::list<Behavior *> deletes;

public:
	std::string name;								// 名前



private:
	void push_left(Behavior *instance);
	void push_right(Behavior *instance);
	void push_up(Behavior *instance);
	void push_down(Behavior *instance);


	/// <summary>
	/// ポップ
	/// </summary>
	/// <param name="instance"></param>
	void pop(Behavior *instance);

	/// <summary>
	/// プッシュ
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

	// 作成
	template<class T, class... Args>
	T *Add(Args... args) {
		// インスタンスを生成する
		T *instance = new T(args...);
		Behavior *mono = instance;
		creates.push_back(instance);
		return instance;
	}
	// キャスト
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
