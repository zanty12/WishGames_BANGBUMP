//--------------------------------------------------------------------------------
// 
// ゲームオブジェクト（インターフェース）[gameobject.h]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/16
// 最終更新日	2023/11/16
//
//  Update:
//  2023/11/16 [根本賢] ゲームオブジェクトのインターフェースを作成
// 2023/11/30 Cheung To Hung 描画とscale,colorを追加
//--------------------------------------------------------------------------------

#pragma once

#include "sprite.h"
#include "lib/dxlib.h"
#include "graphical.h"

class GameObject
{
private:
    Vector2 pos_;
    float rot_;
    int tex_;
    Vector2 scale_ = Vector2{size_, size_}; //大きさ
    Color color_ = Color(1.0f, 1.0f, 1.0f, 1.0f);

public:
    static constexpr float size_ = 96.0f; //１マスの標準サイズ\
	GameObject() = default;
    GameObject(Vector2 pos, float rot, int tex_number)
        : pos_(pos), rot_(rot), tex_(tex_number)
    {
    }

    void SetPos(Vector2 pos) { pos_ = pos; } //ポジションセット
    Vector2 GetPos(void) const { return pos_; } //ポジションゲット
    void SetRot(float rot) { rot_ = rot; } //回転セット
    float GetRot(void) const { return rot_; } //回転ゲット
    void SetTexNo(int tex_number) { tex_ = tex_number; } //テクスチャセット
    int GetTexNo(void) const { return tex_; } //テクスチャゲット
    void SetScale(Vector2 scale) { scale_ = scale; } //大きさセット
    Vector2 GetScale(void) const { return scale_; } //大きさゲット
    void SetColor(Color color) { color_ = color; } //色セット
    Color GetColor(void) const { return color_; } //色ゲット


    virtual void Update(void) = 0;

    virtual void Draw(void)
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(GetTexNo(), Vector2(GetPos().x * scale_x, GetPos().y * scale_y), GetRot(),
                   Vector2(scale_.x * scale_x, scale_.y * scale_y), color_);
    }

    virtual void Draw(Vector2 offset)
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(GetTexNo(), Vector2((GetPos().x - offset.x) * scale_x, (GetPos().y - offset.y) * scale_y), GetRot(),
                   Vector2(scale_.x * scale_x, scale_.y * scale_y), color_);
    }
};
