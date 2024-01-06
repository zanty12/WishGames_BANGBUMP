#pragma once
#include <memory.h>
#include <string.h>
#include <cstddef>



class Storage {
	typedef char buffer;
	buffer *head = nullptr, *tail = nullptr;
	buffer *begin = nullptr, *end = nullptr;
	unsigned int maxSize = 0;
	unsigned int capacitySize = 0;
	const unsigned int onceSize = 4;

private:

	unsigned int maximum(unsigned int size) {
		return (size & ~(onceSize - 1)) + onceSize;
	}

	unsigned int length(buffer *begin, buffer *end) {
		return end - begin;
	}

	// �������m��
	void allocate(buffer *dst, unsigned int size);

	// �R�s�[
	void copy(buffer *dst, buffer *src, unsigned int size);

	// ���������
	void release(void);


public:
	Storage(unsigned int sliceSize = 0) : onceSize(sliceSize) { }

	void Refresh(buffer *src, unsigned int size);

	void Push(buffer *src, unsigned int size);

	template<class T>
	T *Pop() {
		// �|�b�v����f�[�^�������Ȃ�
		if (end - begin <= 0) return nullptr;

		// ���ʂ̃|�C���^
		T *rst = (T *)begin;

		// �J�n�|�C���^���ړ�����
		begin += sizeof(T);

		return rst;
	}

	template<>
	char *Pop<char>() {
		// �|�b�v����f�[�^�������Ȃ�
		if (end - begin <= 0) return nullptr;

		// ���ʂ̃|�C���^
		char *rst = begin;

		// ������̒������擾
		unsigned int length = strlen(begin);

		// �J�n�|�C���^���ړ�����
		begin += length;

		return rst;
	}

	void Release(void);

	void operator =(char *src);

	void operator <<(char *src);

	char *operator >>(char *&dst);

	void operator =(const char *src);

	void operator <<(const char *src);

	template<class T>
	void operator =(T src) {
		// �T�C�Y�̎擾
		unsigned int size__ = sizeof(src);

		// �������
		Refresh((buffer *)&src, size__);
	}

	void operator =(nullptr_t p) {
		// �擪�A�h���X�A�����A�h���X���ړ�����
		begin = end = head;

		// �T�C�Y��������
		maxSize = 0;
	}

	template<class T>
	void operator <<(T src) {
		// �T�C�Y�̎擾
		unsigned int size__ = sizeof(src);

		// ���Z����
		Push((buffer *)&src, size__);
	}

	template<class T>
	void operator >>(T &dst) {
		// �擾����
		T* tmp = Pop<T>();

		// �������
		if (tmp) dst = *tmp;
	}

	void operator <<(Storage src) {
		// ���Z����
		Push(src.head, src.maxSize);
	}

	operator char *() { return head; }

	unsigned int Length(void) { return maxSize; }

	unsigned int CapacityLength(void) { return capacitySize; }

	unsigned int Current(void) { return end - begin; }

	void Allocate(unsigned int len) { allocate(end, len); }
};