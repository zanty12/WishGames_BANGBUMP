#include <memory.h>
#include <string.h>
#include "storage.h"

// �������m��
void Storage::allocate(buffer *dst, unsigned int size) {
	// dst����size���m�ۂ���
	// ���ۂɊm�ۂ��郁�����T�C�Y���v�Z (�擪�A�h���X���璆�ԃA�h���X�܂ł̃T�C�Y�����߁A��������m�ۂ���T�C�Y�𑫂�)
	unsigned int size__ = maximum(dst - head + size);



	// �̈悪�����ς��Ȃ�
	if (capacitySize < size__) {
		// �������m��
		buffer *buff__ = new buffer[size__];

		// ������
		memset(buff__, 0, size__);

		// �R�s�[
		if (head) memcpy(buff__, head, size__);
		capacitySize = size__;

		// ���������
		delete[] head;

		// begin��end�̃C���f�b�N�X��ێ�
		unsigned int tmpBeginIndex = begin - head;
		unsigned int tmpEndIndex = end - head;

		// �A�h���X�̓o�^
		head = buff__;
		tail = &buff__[size__];
		begin = &buff__[tmpBeginIndex];
		end = &buff__[tmpEndIndex];
	}
}

// �R�s�[
void Storage::copy(buffer *dst, buffer *src, unsigned int size) {
	unsigned int size__ = maximum(maxSize + size);

	// �R�s�[
	memcpy(dst, src, size);
}

// ���������
void Storage::release(void) {
	if (head) delete[] head;
	head = tail = begin = end = nullptr;
	maxSize = capacitySize = 0;
}


void Storage::Refresh(buffer *src, unsigned int size) {
	// �̈�m��
	if (capacitySize < size) allocate(head, size);

	// �R�s�[
	if (0 < size) copy(head, src, size);

	// �A�h���X�̍X�V
	begin = head;
	end = &begin[size];

	// �T�C�Y�̍X�V
	maxSize = size;
}

void Storage::Push(buffer *src, unsigned int size) {
	// �̈�m��
	if (capacitySize < end - head + size) allocate(end, size);

	// �R�s�[
	copy(end, src, size);

	// �A�h���X�̍X�V
	end = &end[size];

	// �T�C�Y�̍X�V
	maxSize += size;
}


void Storage::Release(void) {
	release();
}

void Storage::operator =(char *src) {
	// �T�C�Y�̎擾
	unsigned int size__ = strlen(src) + 1;

	// �������
	Refresh(src, size__);

	// \0�̕��߂�
	end--;

	// �T�C�Y�̍X�V
	maxSize += size__;
}

void Storage::operator <<(char *src) {
	// �T�C�Y�̎擾
	unsigned int size__ = strlen(src) + 1;

	// ���Z����
	Push(src, size__);

	// \0�̕��߂�
	end--;

	// �T�C�Y�̍X�V
	maxSize += size__;
}

char *Storage::operator >>(char *&dst) {
	// �擾����
	return dst = Pop<char>();
}

void Storage::operator =(const char *src) {
	// �T�C�Y�̎擾
	unsigned int size__ = strlen(src) + 1;

	// �������
	Refresh((char *)src, size__);

	// \0�̕��߂�
	end--;
}

void Storage::operator <<(const char *src) {
	// �T�C�Y�̎擾
	unsigned int size__ = strlen(src) + 1;

	// ���Z����
	Push((char *)src, size__);

	// \0�̕��߂�
	end--;
}
