/*==============================================================================

    Title		: STRING
    Description :
    Date		:
    Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _STRING_H_
#define _STRING_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <string.h>
#include <math.h>

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
* class�錾
------------------------------------------------------------------------------*/

class STRING {
    unsigned int maxLength = 0;
    unsigned int length = 0;
    char *text = nullptr;
    STRING *src = nullptr;

    void assignment(char *text, unsigned int length) {
        if (!text) return;
        STRING *dest = src ? src : this;
        unsigned int resultLength = dest->length + length;
        if (maxLength < resultLength) {
            // �ϐ�
            char *resultText = new char[resultLength + 1];

            // ���
            memcpy(resultText, dest->text, dest->length);
            memcpy(&resultText[dest->length], text, length);
            resultText[resultLength] = '\0';

            // ���
            if (dest->text) delete dest->text;

            // �X�V
            dest->maxLength = dest->length = resultLength;
            dest->text = resultText;
        }
        else {
            // ���
            memcpy(&dest->text[dest->length], text, length);
            dest->text[resultLength] = '\0';

            // �X�V
            dest->length = resultLength;
        }
    }
    void reverse_assignment(char *text, unsigned int length) {
        if (!text) return;
        STRING *dest = src ? src : this;
        unsigned int resultLength = dest->length + length;
        if (maxLength < resultLength) {
            // �ϐ�
            char *resultText = new char[resultLength + 1];

            // ���
            memcpy(resultText, text, length);
            memcpy(&resultText[length], dest->text, dest->length);
            resultText[resultLength] = '\0';

            // ���
            if (dest->text) delete dest->text;

            // �X�V
            dest->maxLength = dest->length = resultLength;
            dest->text = resultText;
        }
        else {
            // ���
            memcpy(&dest->text[length], dest->text, dest->length);
            memcpy(dest->text, text, length);
            dest->text[resultLength] = '\0';

            // �X�V
            dest->length = resultLength;
        }
    }
    void assignment(char text) {
        unsigned int resultLength = this->length + 1;
        STRING *dest = src ? src : this;
        if (maxLength < resultLength) {
            // �ϐ�
            char *resultText = new char[resultLength + 1];

            // ���
            memcpy(resultText, dest->text, dest->length);
            resultText[dest->length] = text;
            resultText[resultLength] = '\0';

            // ���
            if (dest->text) delete dest->text;

            // �X�V
            dest->maxLength = dest->length = resultLength;
            dest->text = resultText;
        }
        else {
            // ���
            dest->text[dest->length] = text;
            dest->text[resultLength] = '\0';

            // �X�V
            dest->length = resultLength;
        }
    }
    void reverse_assignment(char text) {
        unsigned int resultLength = this->length + 1;
        STRING *dest = src ? src : this;
        if (maxLength < resultLength) {
            // �ϐ�
            char *resultText = new char[resultLength + 1];

            // ���
            memcpy(&resultText[1], dest->text, dest->length);
            resultText[0] = text;
            resultText[resultLength] = '\0';

            // ���
            if (dest->text) delete dest->text;

            // �X�V
            dest->maxLength = dest->length = resultLength;
            dest->text = resultText;
        }
        else {
            // ���
            memcpy(&dest->text[1], dest->text, dest->length);
            dest->text[0] = text;
            dest->text[resultLength] = '\0';

            // �X�V
            dest->length = resultLength;
        }
    }
    void parse(int x) {
        int value = x;
        unsigned int digit = log10f(value) + 1;
        STRING text(digit + 1);

        // �}�C�i�X
        if (value < 0) text << '-';
        // ����
        for (unsigned int i = 0; i < digit; i++) {
            text.reverse_assignment(('0' + value % 10));
            value *= 0.1f;
        }

        assignment(text.text, text.length);
    }
    void parse(float x) {
        int iValue = x;
        unsigned int digit = log10f(iValue) + 1;
        STRING text(digit + 1);

        // �}�C�i�X
        if (iValue < 0) text << '-';
        // ����
        for (unsigned int i = 0; i < digit; i++) {
            text.reverse_assignment(('0' + iValue % 10));
            iValue /= 10;
        }

        text << '.';

        // ����
        float fValue = x - (unsigned int)x;
        digit = 5;
        for (unsigned int i = 0; i < digit; i++) {
            fValue *= 10.0f;
            text.assignment(('0' + (unsigned int)fValue));
            fValue = fValue - (unsigned int)fValue;
        }

        assignment(text.text, text.length);
    }

public:
    STRING() : length(0), maxLength(0), text(nullptr) {}
    STRING(unsigned int maxLength) : length(0), maxLength(maxLength) { text = new char[maxLength + 1]; memset(text, 0, sizeof(char) * maxLength); }
    STRING(const char *text) {
        unsigned int length = strlen(text);
        assignment((char *)text, length);
    }
    STRING(char *text) {
        unsigned int length = strlen(text);
        assignment(text, length);
    }
    STRING(STRING &text) {
        if (text.src) src = text.src;
        else src = &text;
    }
    ~STRING() {
        if (text) delete text;
        length = 0;
        text = nullptr;
    }

    STRING operator = (STRING text) {
        if (this->text) delete this->text;
        length = maxLength = 0;
        assignment(text.text, text.length);
        return *this;
    }
    STRING operator << (const char *text) {
        unsigned int length = strlen(text);
        assignment((char *)text, length);
        return *this;
    }
    STRING operator << (char *text) {
        unsigned int length = strlen(text);
        assignment(text, length);
        return *this;
    }
    STRING operator << (char text) {
        assignment(text);
        return *this;
    }
    STRING operator << (const STRING &text) {
        assignment(text.text, text.length);
        return *this;
    }
    STRING operator << (int value) {
        parse(value);
        return *this;
    }
    STRING operator << (float value) {
        parse(value);
        return *this;
    }

    operator const char *() { return text; }
    const char *str() { return text; }
    static STRING null() { return STRING(); }
};

#endif
