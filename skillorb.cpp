#include "skillorb.h"


SkillOrb::SkillOrb(int cellX, int cellY, SKILLORB_ATTRIBUTE_DESC attributeDesc, SKILLORB_SIZE_DESC sizeDesc)
    : MovableObj(
        Vector2(cellX *GameObject::SIZE_ + GameObject::SIZE_ / 2,
            cellY *GameObject::SIZE_ + GameObject::SIZE_ / 2),
        0.0f,
        LoadTexture("data/TEXTURE/skillorb.png"),
        Vector2::Zero
    ) {
    // �^�C�v�̎w��
    SetType(OBJ_ITEM);

    // �T�C�Y�֘A�̎w��
    SetRadius(sizeDesc.radius);
    SetSize(sizeDesc.sizeType);
    value_ = sizeDesc.value;

    // �����֘A�̎w��
    SetAttribute(attributeDesc.attributeType);
    GetAnimator()->SetColor(attributeDesc.color);
}

SkillOrb::SkillOrb(Vector2 pos, SKILLORB_ATTRIBUTE_DESC attributeDesc, SKILLORB_SIZE_DESC sizeDesc)
    : MovableObj(pos, 0.0f, LoadTexture("data/TEXTURE/skillorb.png"), Vector2::Zero) {
    // ���W�̎w��
    SetPos(pos);

    // �^�C�v�̎w��
    SetType(OBJ_ITEM);

    // �T�C�Y�֘A�̎w��
    SetRadius(sizeDesc.radius);
    SetSize(sizeDesc.sizeType);
    value_ = sizeDesc.value;

    // �����֘A�̎w��
    SetAttribute(attributeDesc.attributeType);
    GetAnimator()->SetColor(attributeDesc.color);
}

// ��������肷��
ATTRIBUTE_TYPE SkillOrb::ParseAttribute(MAP_READ type) {
    int iBase = MAP_READ_ORB_SMALL_FIRE;
    int iType = (type - iBase) / 3;

    return (ATTRIBUTE_TYPE)iType;
}
// �T�C�Y����肷��
SKILLORB_SIZE_TYPE SkillOrb::ParseSize(MAP_READ type) {
    int iBase = MAP_READ_ORB_SMALL_FIRE;
    int iType = (type - iBase) % 3;

    return (SKILLORB_SIZE_TYPE)iType;
}