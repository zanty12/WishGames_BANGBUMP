/*==============================================================================

	Title		: BUFFER
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _BUFFER_H_
#define _BUFFER_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <iostream>

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
class Buffer {
private:
	char *buff = nullptr;
	unsigned int length = 0;
	unsigned int maxLength = 0;



private:
	Buffer(char *pBuff, unsigned int length, unsigned int maxLength) : buff(pBuff), length(length), maxLength(maxLength) { }

	/// <summary>
	/// ���
	/// </summary>
	void substitute(char *src, unsigned int srcLength);

	/// <summary>
	/// �����Z
	/// </summary>
	Buffer add(char *src, unsigned int srcLength);

	/// <summary>
	/// ��������Z
	/// </summary>
	void add_substitute(char *src, unsigned int srcLength);


public:
	Buffer() { }
	Buffer(unsigned int length) { AddMalloc(length); }
	Buffer(const Buffer &buff) { substitute(buff.buff, buff.length); }
	Buffer(const char *buff) { substitute((char *)buff, strlen(buff)); }
	~Buffer() { Refresh(); }

	void Add(Buffer &src) { add_substitute(src.buff, src.length); }

	void Add(char *buff, unsigned int length) { if (buff && length) add_substitute(buff, length); }

	char *begin(void) { return buff; }

	unsigned int size() { return length; }

	Buffer operator<<(Buffer &src) { return add(src.buff, src.length); }

	Buffer operator<<(const char *buff) { return add((char *)buff, strlen(buff)); }

	Buffer operator<<(std::string str) { return add((char *)str.c_str(), str.size()); }

	Buffer operator<=(Buffer src) {
		add_substitute(src.buff, src.length);
		return *this;
	}

	Buffer operator<=(const char *buff) {
		add_substitute((char *)buff, strlen(buff));
		return *this;
	}

	Buffer operator<=(std::string str) {
		add_substitute((char *)str.c_str(), str.size());
		return *this;
	}

	void AddMalloc(unsigned int length);

	void AddMaxMalloc(unsigned int length);

	void Refresh(void);

	const char *ToString(void) { return buff; }
};

#endif
