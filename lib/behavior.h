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
#include "cluster.h"

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
class Cluster;

class Behavior {
private:
	bool isDestroy = false;

protected:
	std::string type;								// タイプ
	int priority = 0;								// 優先度
	Behavior *previous = nullptr;					// 前ポインタ
	Behavior *next = nullptr;						// 次ポインタ
	Behavior *high = nullptr;						// 高優先度
	Behavior *low = nullptr;						// 低優先度
	Cluster *parent = nullptr;						// 親オブジェクト
	Cluster *child = nullptr;

	friend Cluster;

public:
	bool isActive = true;
	std::string name;



private:
	void push_left(Behavior *instance);
	void push_right(Behavior *instance);
	void push_up(Behavior *instance);
	void push_down(Behavior *instance);

protected:
	void start(void);
	void update(void);
	void draw(void);
	void lateupdate(void);
	void end(void);

public:
	Behavior(std::string type) : type(type) { }
	Behavior(std::string type, int priority) : type(type), priority(priority) { }
	virtual ~Behavior();

	Behavior *GetNext(void) { return next; }
	Behavior *GetPrevious(void) { return previous; }

	virtual void Start(void) { };
	virtual void Update(void) { };
	virtual void Draw(void) { };
	virtual void LateUpdate(void) { };
	virtual void End(void) { };

	/// <summary>
	/// 削除
	/// </summary>
	void Destroy(void);

	/// <summary>
	/// 外れる
	/// </summary>
	void SetParent(Behavior *parent);

	/// <summary>
	/// すべての子オブジェクトを対象にチェックを掛ける
	/// </summary>
	/// <param name=""></param>
	void AllChildForeach(void(*func)(Behavior &behavior));
};

#endif
