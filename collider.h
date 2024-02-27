#pragma once
#include "lib/collider2d.h"
#include "lib/dxlib.h"
//#include "gameobject.h"
#include <list>

enum COLLIDER_TYPE
{
    CIRCLE,
    RECTANGLE,
};

class GameObject;
class Collider
{
private:
    COLLIDER_TYPE type_;
    Vector2 pos_;
    bool is_movable_= false;
    GameObject* parent_;
    float bounciness_ = 0.0f; //弾力, 0.0fの場合なし、以上の場合反射の速度はその値に乗算する
    bool is_discard_ = false; //破棄するかどうか
    bool LoD_ = false; //LoD用
    int weight_ = 1;

    protected:
    std::list<Collider*> collision_ = std::list<Collider*>();

public:
    Collider() = delete;
    Collider(COLLIDER_TYPE type, GameObject* parent,bool movable);
    virtual ~Collider() = default;
    virtual void Update() = 0;
    virtual void Draw(int tex, Vector2 offset);
    virtual bool Collide(Collider* other) = 0;

    virtual void CollisionInteract() = 0;
    virtual void CollisionSolid(Collider* other) = 0;
    virtual void CollisionPen(Collider* other) = 0;

    //getter and setter
    void SetPos(Vector2 pos) { pos_ = pos; }
    Vector2 GetPos() const { return pos_; }
    COLLIDER_TYPE GetType() const { return type_; }
    bool GetIsMovable() const { return is_movable_; }
    void SetIsMovable(bool is_move) { is_movable_ = is_move; }
    GameObject* GetParent() const { return parent_; }
    std::list<Collider*> GetCollision() const { if (!collision_.empty())return collision_; return std::list<Collider*>(); }

    void SetCollision(const std::list<Collider*>& collision) { collision_ = collision; }
    float GetBounciness() const { return bounciness_; }
    void SetBounciness(float bounciness) { if(bounciness > 1.0f) bounciness_ = bounciness; else bounciness_ = 1.0f;}
    Vector2 Clamp(const Vector2& val, const Vector2& min, const Vector2& max)
    {
        using namespace std;
        return Vector2(max(min.x, min(max.x, val.x)), max(min.y, min(max.y, val.y)));
    }
    void AddCollision(Collider* other)
    {
        collision_.push_back(other);
        collision_.unique();
    }
    void Discard(){is_discard_ = true;}
    bool GetDiscard() const { return is_discard_; }
    void SetLoD(bool LoD) { LoD_ = LoD; }
    bool GetLoD() const { return LoD_; }
    void SetWeight(int weight) { weight_ = weight; }
    int GetWeight() const { return weight_; }

    //対象のコライダーをリストから外す
    void RemoveCollision(Collider* collider) {
        collision_.remove_if([collider](Collider* col) {
            if (col->GetParent() == collider->GetParent()) {
                return true;
            }
            return false;
            }
        );
    }
};
