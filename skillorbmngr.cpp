#include "skillorbmngr.h"

SkillOrbMngr::~SkillOrbMngr()
{
    for (auto skillOrb : skillOrbs_)
    {
        delete skillOrb;
    }
}

// スキルポイントをポップする
void SkillOrbMngr::Pop(int cellX, int cellY, SKILLORB_SIZE_TYPE sizeType, bool isMovable)
{
    SKILLORB_SIZE_DESC sizeDesc;
    SkillOrb* pSkillOrb = nullptr;
    // サイズ情報の指定
    switch (sizeType)
    {
    case SKILLORB_SIZE_TYPE_SMALL: sizeDesc = SKILLORB_SIZE_DESC::Small();
        break;
    case SKILLORB_SIZE_TYPE_MID: sizeDesc = SKILLORB_SIZE_DESC::Mid();
        break;
    case SKILLORB_SIZE_TYPE_BIG: sizeDesc = SKILLORB_SIZE_DESC::Big();
        break;
    default:
        sizeDesc = SKILLORB_SIZE_DESC::Small();
        break;
    }
    // スキルポイントを追加する
    if (isMovable)
        pSkillOrb = new SkillOrbMovable(cellX, cellY, sizeDesc);
    else
        pSkillOrb = new SkillOrbStatic(cellX, cellY, sizeDesc);
    skillOrbs_.push_back(pSkillOrb);
}

// スキルポイントをポップする
void SkillOrbMngr::Pop(Vector2 pos, SKILLORB_SIZE_TYPE sizeType,bool isMovable)
{
    SKILLORB_SIZE_DESC sizeDesc;
    SkillOrb* pSkillOrb = nullptr;

    // サイズ情報の指定
    switch (sizeType)
    {
    case SKILLORB_SIZE_TYPE_SMALL: sizeDesc = SKILLORB_SIZE_DESC::Small();
        break;
    case SKILLORB_SIZE_TYPE_MID: sizeDesc = SKILLORB_SIZE_DESC::Mid();
        break;
    case SKILLORB_SIZE_TYPE_BIG: sizeDesc = SKILLORB_SIZE_DESC::Big();
        break;
    default:
        sizeDesc = SKILLORB_SIZE_DESC::Small();
        break;
    }
    // スキルポイントを追加する
    if (isMovable)
        pSkillOrb = new SkillOrbMovable(pos, sizeDesc);
    else
        pSkillOrb = new SkillOrbStatic(pos, sizeDesc);
    skillOrbs_.push_back(pSkillOrb);
}
