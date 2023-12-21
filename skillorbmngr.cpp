#include "skillorbmngr.h"

SkillOrbMngr::~SkillOrbMngr() {
    for (auto skillOrb : skillOrbs_) {
        delete skillOrb;
    }
}

// スキルポイントをポップする
void SkillOrbMngr::Pop(int cellX, int cellY, ATTRIBUTE_TYPE attributeType, SKILLORB_SIZE_TYPE sizeType) {
    SKILLORB_ATTRIBUTE_DESC attributeDesc;
    SKILLORB_SIZE_DESC sizeDesc;
    SkillOrb *pSkillOrb = nullptr;

    // 属性情報の指定
    switch (attributeType)
    {
    case ATTRIBUTE_TYPE_FIRE: attributeDesc = SKILLORB_ATTRIBUTE_DESC::Fire(); break;
    case ATTRIBUTE_TYPE_WIND: attributeDesc = SKILLORB_ATTRIBUTE_DESC::Wind(); break;
    case ATTRIBUTE_TYPE_DARK: attributeDesc = SKILLORB_ATTRIBUTE_DESC::Dark(); break;
    case ATTRIBUTE_TYPE_THUNDER: attributeDesc = SKILLORB_ATTRIBUTE_DESC::Thunder(); break;
    }
    // サイズ情報の指定
    switch (sizeType)
    {
    case SKILLORB_SIZE_TYPE_SMALL: sizeDesc = SKILLORB_SIZE_DESC::Small(); break;
    case SKILLORB_SIZE_TYPE_MID: sizeDesc = SKILLORB_SIZE_DESC::Mid(); break;
    case SKILLORB_SIZE_TYPE_BIG: sizeDesc = SKILLORB_SIZE_DESC::Big(); break;
    }

    // スキルポイントを追加する
    pSkillOrb = new SkillOrb(cellX, cellY, attributeDesc, sizeDesc);
    skillOrbs_.push_back(pSkillOrb);
}
// スキルポイントをポップする
void SkillOrbMngr::Pop(Vector2 pos, ATTRIBUTE_TYPE attributeType, SKILLORB_SIZE_TYPE sizeType) {
    SKILLORB_ATTRIBUTE_DESC attributeDesc;
    SKILLORB_SIZE_DESC sizeDesc;
    SkillOrb *pSkillOrb = nullptr;

    // 属性情報の指定
    switch (attributeType)
    {
    case ATTRIBUTE_TYPE_FIRE: attributeDesc = SKILLORB_ATTRIBUTE_DESC::Fire(); break;
    case ATTRIBUTE_TYPE_WIND: attributeDesc = SKILLORB_ATTRIBUTE_DESC::Wind(); break;
    case ATTRIBUTE_TYPE_DARK: attributeDesc = SKILLORB_ATTRIBUTE_DESC::Dark(); break;
    case ATTRIBUTE_TYPE_THUNDER: attributeDesc = SKILLORB_ATTRIBUTE_DESC::Thunder(); break;
    }
    // サイズ情報の指定
    switch (sizeType)
    {
    case SKILLORB_SIZE_TYPE_SMALL: sizeDesc = SKILLORB_SIZE_DESC::Small(); break;
    case SKILLORB_SIZE_TYPE_MID: sizeDesc = SKILLORB_SIZE_DESC::Mid(); break;
    case SKILLORB_SIZE_TYPE_BIG: sizeDesc = SKILLORB_SIZE_DESC::Big(); break;
    }

    // スキルポイントを追加する
    pSkillOrb = new SkillOrb(pos, attributeDesc, sizeDesc);
    skillOrbs_.push_back(pSkillOrb);
}