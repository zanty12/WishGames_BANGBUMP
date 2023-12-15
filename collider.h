#pragma once
#include "lib/collider2d.h"
#include "gameobject.h"

enum COLLIDER_TYPE
{
    CIRCLE,
    RECTANGLE,
    LINE,
};

class Collider
{
private:
    COLLIDER_TYPE type_;
    Vector2 pos_;
    bool is_movable_= false;
    GameObject* parent_;
    std::list<Collider*> collision_;

public:
    Collider() = delete;
    Collider(COLLIDER_TYPE type, GameObject* parent);
    virtual ~Collider() = default;
    virtual void Update() = 0;

    //getter and setter
    void SetPos(Vector2 pos) { pos_ = pos; }
    Vector2 GetPos() const { return pos_; }
    void SetIsMove(bool is_move) { is_movable_ = is_move; }
    GameObject* GetParent() const { return parent_; }
    std::list<Collider*> GetCollision() const { return collision_; }
    void SetCollision(const std::list<Collider*>& collision) { collision_ = collision; }
};
