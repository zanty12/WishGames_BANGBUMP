#pragma once
#include "lib/vector.h"
#include "graphical.h"
#include "sprite.h"
#include "gameobject.h"

class GameObject;
class Animator
{
private:
    Vector2 pos_, scale_;

    int fps_;      //フレームレート
    bool isAnim_;   //アニメーションするか
    bool isMovable_; //移動するか
    int texNo_;
    float u_ = 0.0f, v_ = 0.0f;
    Color color_ = Color(1.0f, 1.0f, 1.0f, 1.0f);
    float rot_ = 0.0f;

    GameObject* parent_;

public:
    Animator() = delete;

    Animator(Vector2 pos, Vector2 scale, int fps, bool isAnim, int texNo);

    ~Animator() = default;

    void Update();

    void Draw()
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(texNo_, Vector2(GetPos().x * scale_x, GetPos().y * scale_y), rot_,
                   Vector2(scale_.x * scale_x, scale_.y * scale_y),color_);
    }

    void Draw(Vector2 offset)
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(texNo_, Vector2((GetPos().x - offset.x) * scale_x, (GetPos().y - offset.y) * scale_y), rot_,
                       Vector2(scale_.x * scale_x, scale_.y * scale_y), color_);
    }

    void SetPos(Vector2 pos) { pos_ = pos; }
    Vector2 GetPos(void) const { return pos_; }
    void SetScale(Vector2 scale) { scale_ = scale; }
    Vector2 GetScale(void) const { return scale_; }
    void SetTexNo(int texNo) { texNo_ = texNo; }
    int GetTexNo(void) const { return texNo_; }
    void SetColor(Color color) { color_ = color; }
    Color GetColor(void) const { return color_; }
    void SetRot(float rot) { rot_ = rot; }
    float GetRot(void) const { return rot_; }
    void SetParent(GameObject* parent) { parent_ = parent; }
    GameObject* GetParent(void) const { return parent_; }
    bool GetIsAnim(void) const { return isAnim_; }
    void SetIsAnim(bool isAnim) { isAnim_ = isAnim; }
    bool GetIsMovable(void) const { return isMovable_; }
    void SetIsMovable(bool isMovable) { isMovable_ = isMovable; }
};
