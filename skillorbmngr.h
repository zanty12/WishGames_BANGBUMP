#pragma once
#include "skillorb.h"

class SkillOrbMngr {
private:
    std::list<SkillOrb *> skillOrbs_;

public:
    ~SkillOrbMngr();

    void Update(void);
    /*
    * スキルポイントをポップする
    * cellX : セル座標X
    * cellY : セル座標Y
    * attributeType : 属性
    * sizeType : 
    */
    void Pop(int cellX, int cellY, SKILLORB_SIZE_TYPE sizeType,bool isMovable = false);
    /*
    * スキルポイントをポップする
    * pos : 座標
    * attributeType : 属性
    * sizeType : サイズ
    */
    void Pop(Vector2 pos, SKILLORB_SIZE_TYPE sizeType,bool isMovable = false);

    std::list<SkillOrb *> &GetSkillOrbs(void) { return skillOrbs_; }
};