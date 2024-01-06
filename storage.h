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

	// メモリ確保
	void allocate(buffer *dst, unsigned int size);

	// コピー
	void copy(buffer *dst, buffer *src, unsigned int size);

	// メモリ解放
	void release(void);


public:
	Storage(unsigned int sliceSize = 0) : onceSize(sliceSize) { }

	void Refresh(buffer *src, unsigned int size);

	void Push(buffer *src, unsigned int size);

	template<class T>
	T *Pop() {
		// ポップするデータがもうない
		if (end - begin <= 0) return nullptr;

		// 結果のポインタ
		T *rst = (T *)begin;

		// 開始ポインタを移動する
		begin += sizeof(T);

		return rst;
	}

	template<>
	char *Pop<char>() {
		// ポップするデータがもうない
		if (end - begin <= 0) return nullptr;

		// 結果のポインタ
		char *rst = begin;

		// 文字列の長さを取得
		unsigned int length = strlen(begin);

		// 開始ポインタを移動する
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
		// サイズの取得
		unsigned int size__ = sizeof(src);

		// 代入する
		Refresh((buffer *)&src, size__);
	}

	void operator =(nullptr_t p) {
		// 先頭アドレス、末尾アドレスを移動する
		begin = end = head;

		// サイズを初期化
		maxSize = 0;
	}

	template<class T>
	void operator <<(T src) {
		// サイズの取得
		unsigned int size__ = sizeof(src);

		// 加算する
		Push((buffer *)&src, size__);
	}

	template<class T>
	void operator >>(T &dst) {
		// 取得する
		T* tmp = Pop<T>();

		// 代入する
		if (tmp) dst = *tmp;
	}

	void operator <<(Storage src) {
		// 加算する
		Push(src.head, src.maxSize);
	}

	operator char *() { return head; }

	unsigned int Length(void) { return maxSize; }

	unsigned int CapacityLength(void) { return capacitySize; }

	unsigned int Current(void) { return end - begin; }

	void Allocate(unsigned int len) { allocate(end, len); }
};