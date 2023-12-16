#pragma once

#include <map>

#include "lib/vector.h"
#include "graphical.h"
#include "sprite.h"
#include "gameobject.h"

struct ANIM_DATA
{
    int loop_start_x, loop_start_y;
    int loop_end_x, loop_end_y;

    ANIM_DATA() {}
    ANIM_DATA(int start_x, int start_y, int end_x, int end_y)
    {
        loop_start_x = start_x;
        loop_start_y = start_y;
        loop_end_x = end_x;
        loop_end_y = end_y;
    }
};

//ここに全てのアニメーションの名前を書く
enum LOOP_ANIM
{
    FIRE,
    WIND,

};


class GameObject;
class Animator
{
private:
    std::map<LOOP_ANIM, ANIM_DATA> DICTIONARY_;

    GameObject* my_object_;  //アニメーション対象のゲームオブジェクト

    Vector2 pos_, scale_;
    float rot_ = 0.0f;
    int texNo_;

    LOOP_ANIM loop_anim_; //ループする画像の種類
    LOOP_ANIM loop_anim_next_; //ループする画像の種類

    int fps_;      //フレームレート
    bool isAnim_ = false;   //アニメーションするか
    bool isMovable_; //移動するか
    int x_matrix_num_, y_matrix_num_;   //横と縦の画像の枚数
    int loop_start_x_, loop_start_y_;   //ループする初めの場所
    int loop_end_x_, loop_end_y_;       //ループする終わりの場所
    bool is_loop_ = false;              //ループ判定（treu=ループ）
    float img_change_time_ = 0.0f;          //画像を切り替える時間の間隔
    float now_time_ = 0.0f;

    int now_matrix_number_ = 0;   //現在の行列の位置
    float u_ = 0.0f, v_ = 0.0f;
    Color color_ = Color(1.0f, 1.0f, 1.0f, 1.0f);

    GameObject* parent_;

public:
    Animator() = delete;

    Animator(GameObject* game_object);
    //--------------------------------------------------------------------------------
    // fps              フレームレート
    // isAnim           アニメーションするかどうか
    // x_matrix_num     横の画像の数
    // y_matrix_num     縦の画像の数
    // img_change_time  次の画像に切り替えるまでの秒数
    //--------------------------------------------------------------------------------
    Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time);
    //--------------------------------------------------------------------------------
    // fps              フレームレート
    // isAnim           アニメーションするかどうか
    // x_matrix_num     横の画像の数
    // y_matrix_num     縦の画像の数
    // img_change_time  次の画像に切り替えるまでの秒数
    // is_loop          特定の場所をループするかどうか
    // loop_anim        ループさせるアニメーション
    //--------------------------------------------------------------------------------
    Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time, bool is_loop,
        LOOP_ANIM loop_anim);

    ~Animator() = default;

    void Update();

    void Draw()
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(texNo_, Vector2(GetPos().x * scale_x, GetPos().y * scale_y), rot_,
            Vector2(scale_.x * scale_x, scale_.y * scale_y), color_);
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
    void SetIsAnim(bool isAnim) {
        if (x_matrix_num_ != 0 && y_matrix_num_ != 0)//セットされていないものはアニメーションすることが許されない
            isAnim_ = isAnim;
    }
    bool GetIsMovable(void) const { return isMovable_; }
    void SetIsMovable(bool isMovable) { isMovable_ = isMovable; }

    void SetIsLoop(bool is_loop) { is_loop_ = is_loop; }    //ループの設定（treu=ループ）
    void SetImgChangeTime(float img_change_time) { img_change_time_ = img_change_time; }    //次の画像に切り替える間隔 [1.0f=１秒]

    void SetLoopImg(LOOP_ANIM loop_anim) { loop_anim_next_ = loop_anim; }    //ループするイメージの設定

    float UWidth(void) const { return 1.0f / x_matrix_num_; }    //UV(U)の幅を取得
    float VHeight(void) const { return 1.0f / y_matrix_num_; }  //UV(V)の高さを取得
    float GetU(void) const { return u_; }   //UV(U)の値を取得
    float GetV(void) const { return v_; }   //UV(V)の値を取得

private:
    void InitDictionly(void);
    void LoopAnimation(void);
    void Reset(void);
};
