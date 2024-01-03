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

#include "animator.h"
#include "collider.h"
#include "sprite.h"
#include "lib/dxlib.h"
#include "graphical.h"

enum OBJECT_TYPE
{
    OBJ_SOLID, //貫通不能
    OBJ_PENETRABLE, //下貫通
    OBJ_VOID, //貫通可能
    OBJ_SPIKE, //トゲ
    OBJ_PLAYER, //プレイヤー
    OBJ_ENEMY, //敵
    OBJ_BULLET, //敵２のバレット
    OBJ_ATTACK, //攻撃
    OBJ_ITEM, //アイテム(スキル玉しかないけど)
};

class Animator;
class Collider;
class GameObject
{
private:
    Vector2 pos_;
    float rot_;
    int tex_;
    Vector2 scale_ = Vector2{SIZE_, SIZE_}; //大きさ
    Color color_ = Color(1.0f, 1.0f, 1.0f, 1.0f);
    Animator* animator_;
    Collider* collider_;
    OBJECT_TYPE type_ = OBJ_SOLID;
    bool is_discard_ = false; //破棄するかどうか
    int id_;

public:
    static constexpr float SIZE_ = 64.0f; //１マスの標準サイズ
	GameObject() = default;
    GameObject(Vector2 pos, float rot, int tex_number);
    virtual ~GameObject(){
        if (animator_)animator_->Discard();
        if (collider_)collider_->Discard();
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
    Animator* GetAnimator(void) const { return animator_; } //アニメーターゲット
    void SetAnimator(Animator* animator) { animator_ = animator; } //アニメーターセット
    Collider* GetCollider(void) const { return collider_; } //コライダーゲット
    void SetCollider(Collider* collider) { collider_ = collider; } //コライダーセット
    OBJECT_TYPE GetType(void) const { return type_; } //オブジェクトタイプゲット
    void SetType(OBJECT_TYPE type) { type_ = type; } //オブジェクトタイプセット
    void Discard(); //破棄する
    bool GetDiscard() const { return is_discard_; } //破棄するかどうか
    int GetID() const { return id_; } //IDゲット


    virtual void Update(void) = 0;

    static Vector2 GetWorldCoord(Vector2 pos);

};
