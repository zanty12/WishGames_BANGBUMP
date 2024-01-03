#pragma once
#include "skillorb.h"

class SkillOrbMngr {
private:
    std::list<SkillOrb *> skillOrbs_;

public:
    ~SkillOrbMngr();

    /*
    * �X�L���|�C���g���|�b�v����
    * cellX : �Z�����WX
    * cellY : �Z�����WY
    * attributeType : ����
    * sizeType : 
    */
    void Pop(int cellX, int cellY, ATTRIBUTE_TYPE attributeType, SKILLORB_SIZE_TYPE sizeType);
    /*
    * �X�L���|�C���g���|�b�v����
    * pos : ���W
    * attributeType : ����
    * sizeType : �T�C�Y
    */
    void Pop(Vector2 pos, ATTRIBUTE_TYPE attributeType, SKILLORB_SIZE_TYPE sizeType);

    std::list<SkillOrb *> &GetSkillOrbs(void) { return skillOrbs_; }
};