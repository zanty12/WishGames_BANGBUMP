#pragma once
#include "attribute_type.h"
#include "movableobj.h"
#include "gameobject.h"
#include "mapenum.h"
#include "texture.h"
#include <type_traits>

enum SKILLORB_SIZE_TYPE
{
    SKILLORB_SIZE_TYPE_SMALL,
    SKILLORB_SIZE_TYPE_MID,
    SKILLORB_SIZE_TYPE_BIG,
};


class SKILLORB_SIZE_DESC
{
public:
    int value;
    float radius;
    SKILLORB_SIZE_TYPE sizeType;


    SKILLORB_SIZE_DESC() = default;

    /*
    * value : スキルポイントの値
    * radius : 半径
    * sizeType : サイズの種類
    */
    SKILLORB_SIZE_DESC(int value, float radius, SKILLORB_SIZE_TYPE sizeType)
        : value(value), radius(radius), sizeType(sizeType)
    {
    }

    static SKILLORB_SIZE_DESC Big(void)
    {
        return SKILLORB_SIZE_DESC(
            5, // ポイント
            100.0f, // 半径
            SKILLORB_SIZE_TYPE_BIG // サイズの種類
        );
    }

    static SKILLORB_SIZE_DESC Mid(void)
    {
        return SKILLORB_SIZE_DESC(
            2, // ポイント
            60.0f, // 半径
            SKILLORB_SIZE_TYPE_MID // サイズの種類
        );
    }

    static SKILLORB_SIZE_DESC Small(void)
    {
        return SKILLORB_SIZE_DESC(
            1, // ポイント
            25.0f, // 半径
            SKILLORB_SIZE_TYPE_SMALL // サイズの種類
        );
    }
};

/*class SKILLORB_ATTRIBUTE_DESC {
public:
    ATTRIBUTE_TYPE attributeType;
    Color color;


    SKILLORB_ATTRIBUTE_DESC() = default;

    /*
    * attributeType : スキルポイントの属性
    * color : スキルポイントの色
    #1#
    SKILLORB_ATTRIBUTE_DESC(ATTRIBUTE_TYPE attributeType, Color color)
        : attributeType(attributeType), color(color)
    {}

    static SKILLORB_ATTRIBUTE_DESC Fire(void) {
        return SKILLORB_ATTRIBUTE_DESC(
            ATTRIBUTE_TYPE_FIRE,    // 属性
            Color(1.0f, 0.0f, 0.0f) // 色
        );
    }

    static SKILLORB_ATTRIBUTE_DESC Dark(void) {
        return SKILLORB_ATTRIBUTE_DESC(
            ATTRIBUTE_TYPE_DARK,    // 属性
            Color(0.0f, 0.0f, 1.0f) // 色
        );
    }

    static SKILLORB_ATTRIBUTE_DESC Wind(void) {
        return SKILLORB_ATTRIBUTE_DESC(
            ATTRIBUTE_TYPE_WIND,    // 属性
            Color(0.0f, 1.0f, 0.0f) // 色
        );
    }

    static SKILLORB_ATTRIBUTE_DESC Thunder(void) {
        return SKILLORB_ATTRIBUTE_DESC(
            ATTRIBUTE_TYPE_THUNDER, // 属性
            Color(1.0f, 1.0f, 0.0f) // 色
        );
    }
};*/

class SkillOrb
{
protected:
    int value_ = 1;
    float radius_ = 25.0f;
    SKILLORB_SIZE_TYPE size_ = SKILLORB_SIZE_TYPE_SMALL;
    bool is_movable_ = false;
    GameObject *gameObject_ = nullptr;

public:
    SkillOrb(SKILLORB_SIZE_DESC desc, GameObject *gameObject) : gameObject_(gameObject)
    {
        value_ = desc.value;
        radius_ = desc.radius;
        size_ = desc.sizeType;
    }

    virtual ~SkillOrb() = default;

    // サイズを特定する
    static SKILLORB_SIZE_TYPE ParseSize(MAP_READ type)
    {
        int iBase = MAP_READ_ORB_SMALL;
        int iType = (type - iBase) % 3;

        return static_cast<SKILLORB_SIZE_TYPE>(iType);
    }

    SKILLORB_SIZE_TYPE GetSize(void) const { return size_; }
    void SetSize(SKILLORB_SIZE_TYPE size) { size_ = size; }
    float GetRadius(void) const { return radius_; }
    void SetRadius(float radius) { radius_ = radius; }
    int GetValue() const { return value_; }
    void SetValue(int value) { value_ = value; }
    bool IsMovable(void) const { return is_movable_; }
    void SetMovable(bool is_movable) { is_movable_ = is_movable; }
    Vector2 GetPos(void) { return gameObject_ ? gameObject_->GetPos() : Vector2(); }
    float GetRot(void) { return gameObject_ ? gameObject_->GetRot() : 0.0f; }
    Vector2 GetScl(void) { return gameObject_ ? gameObject_->GetScale() : Vector2(); }
    std::string GetID(void) { return gameObject_ ? gameObject_->GetID() : ""; }
    bool GetDiscard(void) { return gameObject_ ? gameObject_->GetDiscard() : false; }
    GameObject *GetGameObject(void) { return gameObject_ ? gameObject_ : nullptr; }
    void SetGameObject(GameObject *gameObject) { gameObject_ = gameObject; }
};

class SkillOrbMovable : public MovableObj, public SkillOrb
{
public:
    /*
    * cellX : CellのX座標
    * cellY : CellのY座標
    * attributeDesc : スキルポイントの属性情報
    * sizeDesc : スキルポイントのサイズ情報
    */
    //SkillOrb(int cellX, int cellY, SKILLORB_ATTRIBUTE_DESC attributeDesc, SKILLORB_SIZE_DESC sizeDesc);
    SkillOrbMovable(int cellX, int cellY, SKILLORB_SIZE_DESC sizeDesc);
    /*
    * pos : 座標
    * value : スキルポイントの値
    * attributeDesc : スキルポイントの属性情報
    * sizeDesc : スキルポイントのサイズ情報
    */
    //SkillOrb(Vector2 pos, SKILLORB_ATTRIBUTE_DESC attributeDesc, SKILLORB_SIZE_DESC sizeDesc);
    SkillOrbMovable(Vector2 pos, SKILLORB_SIZE_DESC sizeDesc);

    // 属性を特定する
    //static ATTRIBUTE_TYPE ParseAttribute(MAP_READ type);

    /*ATTRIBUTE_TYPE GetAttribute(void) const { return attribute_; }
    void SetAttribute(ATTRIBUTE_TYPE attribute) { attribute_ = attribute; }*/

    ~SkillOrbMovable() override = default;

    void Update(void) override;
};

class SkillOrbStatic : public GameObject, public SkillOrb
{
public:
    SkillOrbStatic(int cellX, int cellY, SKILLORB_SIZE_DESC desc);
    SkillOrbStatic(Vector2 pos, SKILLORB_SIZE_DESC sizeDesc);
    void Update(void) override;
    ~SkillOrbStatic() override = default;

};
