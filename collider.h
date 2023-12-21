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
    float bounciness_ = 1.0f; //’e—Í
    bool is_discard_ = false; //”jŠü‚·‚é‚©‚Ç‚¤‚©

    protected:
    std::list<Collider*> collision_;

public:
    Collider() = delete;
    Collider(COLLIDER_TYPE type, GameObject* parent);
    virtual ~Collider() = default;
    virtual void Update() = 0;
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
    std::list<Collider*> GetCollision() const { return collision_; }
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

};
