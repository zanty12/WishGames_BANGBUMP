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
#include "cluster.h"

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
class Cluster;

class Behavior {
private:
	bool isDestroy = false;

protected:
	std::string type;								// �^�C�v
	int priority = 0;								// �D��x
	Behavior *previous = nullptr;					// �O�|�C���^
	Behavior *next = nullptr;						// ���|�C���^
	Behavior *high = nullptr;						// ���D��x
	Behavior *low = nullptr;						// ��D��x
	Cluster *parent = nullptr;						// �e�I�u�W�F�N�g
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
	/// �폜
	/// </summary>
	void Destroy(void);

	/// <summary>
	/// �O���
	/// </summary>
	void SetParent(Behavior *parent);

	/// <summary>
	/// ���ׂĂ̎q�I�u�W�F�N�g��ΏۂɃ`�F�b�N���|����
	/// </summary>
	/// <param name=""></param>
	void AllChildForeach(void(*func)(Behavior &behavior));
};

#endif
