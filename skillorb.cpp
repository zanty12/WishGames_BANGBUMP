#include "skillorb.h"


SkillOrb::SkillOrb(int cellX, int cellY, SKILLORB_ATTRIBUTE_DESC attributeDesc, SKILLORB_SIZE_DESC sizeDesc)
    : MovableObj(
        Vector2(cellX *GameObject::SIZE_ + GameObject::SIZE_ / 2,
            cellY *GameObject::SIZE_ + GameObject::SIZE_ / 2),
        0.0f,
        LoadTexture("data/TEXTURE/skillorb.png"),
        Vector2::Zero
    ) {
    // タイプの指定
    SetType(OBJ_ITEM);

    // サイズ関連の指定
    SetRadius(sizeDesc.radius);
    SetSize(sizeDesc.sizeType);
    value_ = sizeDesc.value;

    // 属性関連の指定
    SetAttribute(attributeDesc.attributeType);
    GetAnimator()->SetColor(attributeDesc.color);
}

SkillOrb::SkillOrb(Vector2 pos, SKILLORB_ATTRIBUTE_DESC attributeDesc, SKILLORB_SIZE_DESC sizeDesc)
    : MovableObj(pos, 0.0f, LoadTexture("data/TEXTURE/skillorb.png"), Vector2::Zero) {
    // 座標の指定
    SetPos(pos);

    // タイプの指定
    SetType(OBJ_ITEM);

    // サイズ関連の指定
    SetRadius(sizeDesc.radius);
    SetSize(sizeDesc.sizeType);
    value_ = sizeDesc.value;

    // 属性関連の指定
    SetAttribute(attributeDesc.attributeType);
    GetAnimator()->SetColor(attributeDesc.color);
}

// 属性を特定する
ATTRIBUTE_TYPE SkillOrb::ParseAttribute(MAP_READ type) {
    int iBase = MAP_READ_ORB_SMALL_FIRE;
    int iType = (type - iBase) / 3;

    return (ATTRIBUTE_TYPE)iType;
}
// サイズを特定する
SKILLORB_SIZE_TYPE SkillOrb::ParseSize(MAP_READ type) {
    int iBase = MAP_READ_ORB_SMALL_FIRE;
    int iType = (type - iBase) % 3;

    return (SKILLORB_SIZE_TYPE)iType;
}