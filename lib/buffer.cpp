#include "buffer.h"

/// <summary>
/// ���
/// </summary>
void Buffer::substitute(char *src, unsigned int srcLength) {
	unsigned int newMaxLength = (int)(srcLength * 0.25f) * 4 + 4;

	// ���Ă��Ȃ�
	if (newMaxLength <= maxLength) {
		length = srcLength;
	}
	// ��ꂽ�ꍇ
	else {
		if (buff) delete[] buff;
		buff = new char[newMaxLength];
		length = srcLength;
		maxLength = newMaxLength;
		memset(buff, 0, sizeof(char) * maxLength);
	}

	memcpy(buff, src, sizeof(char) * srcLength);
}

/// <summary>
/// �����Z
/// </summary>
Buffer Buffer::add(char *src, unsigned int srcLength) {
	unsigned int newLength = length + srcLength;
	unsigned int newMaxLength = (int)(newLength * 0.25f) * 4 + 4;
	char *newBuff = new char[newMaxLength];
	memset(&newBuff[0], 0, sizeof(char) * newMaxLength);
	memcpy(&newBuff[0], buff, sizeof(char) * length);
	memcpy(&newBuff[length], src, sizeof(char) * srcLength);

	return Buffer(newBuff, newLength, newMaxLength);
}

/// <summary>
/// ��������Z
/// </summary>
void Buffer::add_substitute(char *src, unsigned int srcLength) {
	unsigned int newLength = length + srcLength;
	unsigned int newMaxLength = (int)(newLength * 0.25f) * 4 + 4;
	char *newBuff = buff;

	// ���Ă��Ȃ�
	if (newMaxLength <= maxLength) {
		newMaxLength = maxLength;
	}
	// ��ꂽ�ꍇ
	else {
		newBuff = new char[newMaxLength];
		memset(&newBuff[0], 0, sizeof(char) * newMaxLength);
	}

	memcpy(&newBuff[0], buff, sizeof(char) * length);
	memcpy(&newBuff[length], src, sizeof(char) * srcLength);

	if (maxLength < newMaxLength && buff) delete[] buff;
	buff = newBuff;
	length = newLength;
	maxLength = newMaxLength;
}

void Buffer::AddMalloc(unsigned int length) {
	unsigned int newLength = length + this->maxLength;
	unsigned int newMaxLength = (int)(length * 0.25f) * 4 + 4;
	char *newBuff = new char[newMaxLength];

	memset(&newBuff[0], 0, sizeof(char) * newMaxLength);
	memcpy(&newBuff[0], buff, sizeof(char) * this->length);

	if (buff) delete[] buff;
	this->buff = newBuff;
	this->length = newLength;
	this->maxLength = newMaxLength;
}

void Buffer::AddMaxMalloc(unsigned int length) {
	unsigned int newLength = length + this->maxLength;
	unsigned int newMaxLength = (int)(length * 0.25f) * 4 + 4;
	char *newBuff = new char[newMaxLength];

	memset(&newBuff[0], 0, sizeof(char) * newMaxLength);
	memcpy(&newBuff[0], buff, sizeof(char) * this->length);

	if (buff) delete[] buff;
	this->buff = newBuff;
	this->maxLength = newMaxLength;
}

void Buffer::Refresh(void) {
	if (buff) delete[] buff;
	buff = nullptr;
	length = maxLength = 0;
}