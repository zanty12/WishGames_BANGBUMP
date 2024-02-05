#pragma once
#include "skillorb.h"

class SkillOrbMngr {
private:
    std::list<SkillOrb *> skillOrbs_;

public:
    ~SkillOrbMngr();

    void Update(void);
    /*
    * �X�L���|�C���g���|�b�v����
    * cellX : �Z�����WX
    * cellY : �Z�����WY
    * attributeType : ����
    * sizeType : 
    */
    void Pop(int cellX, int cellY, SKILLORB_SIZE_TYPE sizeType,bool isMovable = false);
    /*
    * �X�L���|�C���g���|�b�v����
    * pos : ���W
    * attributeType : ����
    * sizeType : �T�C�Y
    */
    void Pop(Vector2 pos, SKILLORB_SIZE_TYPE sizeType,bool isMovable = false);

    std::list<SkillOrb *> &GetSkillOrbs(void) { return skillOrbs_; }
};