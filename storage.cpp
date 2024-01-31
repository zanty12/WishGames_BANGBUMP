#include <memory.h>
#include <string.h>
#include "storage.h"

// メモリ確保
void Storage::allocate(buffer *dst, unsigned int size) {
	// dstからsize分確保する
	// 実際に確保するメモリサイズを計算 (先頭アドレスから中間アドレスまでのサイズを求め、そこから確保するサイズを足す)
	unsigned int size__ = maximum(dst - head + size);



	// 領域がいっぱいなら
	if (capacitySize < size__) {
		// メモリ確保
		buffer *buff__ = new buffer[size__];

		// 初期化
		memset(buff__, 0, size__);

		// コピー
		if (head) memcpy(buff__, head, size__);
		capacitySize = size__;

		// メモリ解放
		delete[] head;

		// beginとendのインデックスを保持
		unsigned int tmpBeginIndex = begin - head;
		unsigned int tmpEndIndex = end - head;

		// アドレスの登録
		head = buff__;
		tail = &buff__[size__];
		begin = &buff__[tmpBeginIndex];
		end = &buff__[tmpEndIndex];
	}
}

// コピー
void Storage::copy(buffer *dst, buffer *src, unsigned int size) {
	unsigned int size__ = maximum(maxSize + size);

	// コピー
	memcpy(dst, src, size);
}

// メモリ解放
void Storage::release(void) {
	if (head) delete[] head;
	head = tail = begin = end = nullptr;
	maxSize = capacitySize = 0;
}


void Storage::Refresh(buffer *src, unsigned int size) {
	// 領域確保
	if (capacitySize < size) allocate(head, size);

	// コピー
	if (0 < size) copy(head, src, size);

	// アドレスの更新
	begin = head;
	end = &begin[size];

	// サイズの更新
	maxSize = size;
}

void Storage::Push(buffer *src, unsigned int size) {
	// 領域確保
	if (capacitySize < end - head + size) allocate(end, size);

	// コピー
	copy(end, src, size);

	// アドレスの更新
	end = &end[size];

	// サイズの更新
	maxSize += size;
}


void Storage::Release(void) {
	release();
}

void Storage::operator =(char *src) {
	// サイズの取得
	unsigned int size__ = strlen(src) + 1;

	// 代入する
	Refresh(src, size__);

	// \0の分戻す
	end--;

	// サイズの更新
	maxSize += size__;
}

void Storage::operator <<(char *src) {
	// サイズの取得
	unsigned int size__ = strlen(src) + 1;

	// 加算する
	Push(src, size__);

	// \0の分戻す
	end--;

	// サイズの更新
	maxSize += size__;
}

char *Storage::operator >>(char *&dst) {
	// 取得する
	return dst = Pop<char>();
}

void Storage::operator =(const char *src) {
	// サイズの取得
	unsigned int size__ = strlen(src) + 1;

	// 代入する
	Refresh((char *)src, size__);

	// \0の分戻す
	end--;
}

void Storage::operator <<(const char *src) {
	// サイズの取得
	unsigned int size__ = strlen(src) + 1;

	// 加算する
	Push((char *)src, size__);

	// \0の分戻す
	end--;
}
