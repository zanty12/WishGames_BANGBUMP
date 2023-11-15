/*==============================================================================

	Title		: GET_SET
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _GET_SET_H_
#define _GET_SET_H_


/*------------------------------------------------------------------------------
* includeéŒ¾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* defineéŒ¾
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
* externéŒ¾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototypeéŒ¾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* enuméŒ¾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* classéŒ¾
------------------------------------------------------------------------------*/
template<class T>
class get {
private:
	T &variable;
	T(*func)(T) = nullptr;

public:
	get(T &variable) : variable(variable) { }
	get(T &variable, T(*func)(T)) : variable(variable), func(func) { }
	operator T() { return Get(); }
	T &operator->() { return variable; }
	T Get(void) { return func ? func(variable) : variable; }
};

template<class T>
class set {
private:
	T &variable;
	T(*func)(T) = nullptr;

public:
	set(T &variable) : variable(variable) { }
	set(T &variable, T(*func)(T)) :variable(variable), func(func) { }
	void operator =(T value) { Set(value); }
	void Set(T value) { variable = func ? func(value) : value; }
};

template<class T>
class set_get {
private:
	T &variable;
	T(*set)(T) = nullptr;
	T(*get)(T) = nullptr;

public:
	set_get(T &variable) : variable(variable) { }
	set_get(T &variable, T(*set)(T), T(*get)(T)) : variable(variable), set(set), get(get) { }

	void operator =(T value) { Set(value); }
	void Set(T value) { variable = set ? set(value) : value; }
	operator T() { return Get(); }
	T Get(void) { return get ? get(variable) : variable; }
};

#endif
