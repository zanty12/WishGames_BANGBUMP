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
* include宣言
------------------------------------------------------------------------------*/
#include <string>
#include <list>
#include "behavior.h"

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
	/// ポップ
	/// </summary>
	/// <param name="instance"></param>
	void pop(Behavior *instance);

	/// <summary>
	/// プッシュ
	/// </summary>
	void push(Behavior *instance);

public:
	virtual ~Cluster();

	/// <summary>
	/// 作成
	/// </summary>
	template<class T, class... Args>
	T *Create(Args... args) {
		// インスタンスを生成する
		T *instance = new T(args...);
		Behavior *mono = instance;
		creates.push_back(instance);
		return instance;
	}


	Behavior *GetHead(void) { return head; }
	Behavior *GetTail(void) { return tail; }

	/// <summary>
	/// 開始関数
	/// </summary>
	void Start(void);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 遅更新関数
	/// </summary>
	void LateUpdate(void);

	/// <summary>
	/// 終了関数
	/// </summary>
	void End(void);

	/// <summary>
	/// 解放
	/// </summary>
	/// <param name=""></param>
	void Release(void);

	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="type">タイプ</param>
	Behavior *Get(std::string type);

	/// <summary>
	/// 要素数
	/// </summary>
	int Count(void) { return count; }
};

#endif
