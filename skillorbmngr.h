#pragma once
#include "skillorb.h"

class SkillOrbMngr {
private:
    std::list<SkillOrb *> skillOrbs_;

public:
    ~SkillOrbMngr();

    /*
    * スキルポイントをポップする
    * cellX : セル座標X
    * cellY : セル座標Y
    * attributeType : 属性
    * sizeType : 
    */
    void Pop(int cellX, int cellY, ATTRIBUTE_TYPE attributeType, SKILLORB_SIZE_TYPE sizeType);
    /*
    * スキルポイントをポップする
    * pos : 座標
    * attributeType : 属性
    * sizeType : サイズ
    */
    void Pop(Vector2 pos, ATTRIBUTE_TYPE attributeType, SKILLORB_SIZE_TYPE sizeType);

    std::list<SkillOrb *> &GetSkillOrbs(void) { return skillOrbs_; }
};