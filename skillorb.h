#pragma once
#include "attribute_type.h"
#include "movableobj.h"
#include "texture.h"

enum SKILLORB_SIZE_TYPE {
    SKILLORB_SIZE_TYPE_SMALL,
    SKILLORB_SIZE_TYPE_MID,
    SKILLORB_SIZE_TYPE_BIG,
};


class SKILLORB_SIZE_DESC {
public:
    int value;
    float radius;
    SKILLORB_SIZE_TYPE sizeType;


    SKILLORB_SIZE_DESC() = default;

    /*
    * value : �X�L���|�C���g�̒l
    * radius : ���a
    * sizeType : �T�C�Y�̎��
    */
    SKILLORB_SIZE_DESC(int value, float radius, SKILLORB_SIZE_TYPE sizeType)
        : value(value), radius(radius), sizeType(sizeType)
    {}

    static SKILLORB_SIZE_DESC Big(void) {
        return SKILLORB_SIZE_DESC(
            5,                      // �|�C���g
            100.0f,                 // ���a
            SKILLORB_SIZE_TYPE_BIG  // �T�C�Y�̎��
        );
    }

    static SKILLORB_SIZE_DESC Mid(void) {
        return SKILLORB_SIZE_DESC(
            2,                      // �|�C���g
            60.0f,                  // ���a
            SKILLORB_SIZE_TYPE_MID  // �T�C�Y�̎��
        );
    }

    static SKILLORB_SIZE_DESC Small(void) {
        return SKILLORB_SIZE_DESC(
            1,                      // �|�C���g
            25.0f,                  // ���a
            SKILLORB_SIZE_TYPE_SMALL// �T�C�Y�̎��
        );
    }
};

class SKILLORB_ATTRIBUTE_DESC {
public:
    ATTRIBUTE_TYPE attributeType;
    Color color;


    SKILLORB_ATTRIBUTE_DESC() = default;

    /*
    * attributeType : �X�L���|�C���g�̑���
    * color : �X�L���|�C���g�̐F
    */
    SKILLORB_ATTRIBUTE_DESC(ATTRIBUTE_TYPE attributeType, Color color)
        : attributeType(attributeType), color(color)
    {}

    static SKILLORB_ATTRIBUTE_DESC Fire(void) {
        return SKILLORB_ATTRIBUTE_DESC(
            ATTRIBUTE_TYPE_FIRE,    // ����
            Color(1.0f, 0.0f, 0.0f) // �F
        );
    }

    static SKILLORB_ATTRIBUTE_DESC Dark(void) {
        return SKILLORB_ATTRIBUTE_DESC(
            ATTRIBUTE_TYPE_DARK,    // ����
            Color(0.0f, 0.0f, 1.0f) // �F
        );
    }

    static SKILLORB_ATTRIBUTE_DESC Wind(void) {
        return SKILLORB_ATTRIBUTE_DESC(
            ATTRIBUTE_TYPE_WIND,    // ����
            Color(0.0f, 1.0f, 0.0f) // �F
        );
    }

    static SKILLORB_ATTRIBUTE_DESC Thunder(void) {
        return SKILLORB_ATTRIBUTE_DESC(
            ATTRIBUTE_TYPE_THUNDER, // ����
            Color(1.0f, 1.0f, 0.0f) // �F
        );
    }
};

class SkillOrb : public MovableObj {
private:
    int value_ = 0;
    float radius_ = 10.0f;
    ATTRIBUTE_TYPE attribute_ = ATTRIBUTE_TYPE_FIRE;
    SKILLORB_SIZE_TYPE size_ = SKILLORB_SIZE_TYPE_SMALL;

public:
    /*
    * cellX : Cell��X���W
    * cellY : Cell��Y���W
    * attributeDesc : �X�L���|�C���g�̑������
    * sizeDesc : �X�L���|�C���g�̃T�C�Y���
    */
    SkillOrb(int cellX, int cellY, SKILLORB_ATTRIBUTE_DESC attributeDesc, SKILLORB_SIZE_DESC sizeDesc);

    /*
    * pos : ���W
    * value : �X�L���|�C���g�̒l
    * attributeDesc : �X�L���|�C���g�̑������
    * sizeDesc : �X�L���|�C���g�̃T�C�Y���
    */
    SkillOrb(Vector2 pos, SKILLORB_ATTRIBUTE_DESC attributeDesc, SKILLORB_SIZE_DESC sizeDesc);

    // ��������肷��
    static ATTRIBUTE_TYPE ParseAttribute(MAP_READ type);
    // �T�C�Y����肷��
    static SKILLORB_SIZE_TYPE ParseSize(MAP_READ type);




    float GetRadius(void) const { return radius_; }
    void SetRadius(float radius) { radius_ = radius; GetAnimator()->SetScale(Vector2(radius_, radius_)); }

    ATTRIBUTE_TYPE GetAttribute(void) const { return attribute_; }
    void SetAttribute(ATTRIBUTE_TYPE attribute) { attribute_ = attribute; }

    SKILLORB_SIZE_TYPE GetSize(void) const { return size_; }
    void SetSize(SKILLORB_SIZE_TYPE size) { size_ = size; }

    void Update(void) override {};
};
