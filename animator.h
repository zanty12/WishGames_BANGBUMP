#pragma once

#include <map>

#include "lib/vector.h"
#include "graphical.h"
#include "sprite.h"
#include "asset.h"
#include "attribute_type.h"

struct ANIM_DATA
{
    textures texture_enum;

    int loop_start_x, loop_start_y;
    int loop_end_x, loop_end_y;

    ANIM_DATA() {
        loop_start_x = loop_start_y = loop_end_x = loop_end_y = -1; //設定しないときは-1にして例外処理に通す
    };
    //----------------------------------------
    // int start_x      ループアニメのスタート画像[よこ]（右端 = 0）
    // int start_y      ループアニメのスタート画像[たて]（上端 = 0）
    // int end_x        ループアニメの終端画像[よこ]
    // int end_y        ループアニメの終端画像[たて]
    // textures texture_enum   テクスチャenum
    //----------------------------------------
    ANIM_DATA(int start_x, int start_y, int end_x, int end_y, textures texture = texture_none)
    {
        texture_enum = texture;

        loop_start_x = start_x;
        loop_start_y = start_y;
        loop_end_x = end_x;
        loop_end_y = end_y;
    }
    //----------------------------------------
    // textures texture_enum   テクスチャenum
    //----------------------------------------
    ANIM_DATA(textures texture);
};

//ここに全てのアニメーションの名前を書く
enum LOOP_ANIM
{
    MULTI_NONE = -1,

    //player
    PLAYER_IDOL_ANIM,    //待機
    PLAYER_ATTACK_ANIM,  //攻撃
    PLAYER_FD_MOVE_ANIM,    //fire,dark移動
    PLAYER_TW_MOVE_ANIM,    //thunder,wind移動
    PLAYER_TM_CHARGE_ANIM,  //thunder移動チャージ
    PLAYER_TA_CHARGE_ANIM,  //thunderアタックチャージ

    //attibute_move
    FIRE_MOVE_ANIM,
    DARK_MOVE_ANIM,
    THUNDER_MOVE_ANIM,
    WIND_MOVE_ANIM,
    DARK_MOVE_CHARGE_ANIM,
    THUNDER_MOVE_CHARGE_ANIM,

    //attibute_attack
    FIRE_ATTACK_ANIM,
    DARK_ATTACK_ANIM,
    THUNDER_ATTACK_ANIM,
    WIND_ATTACK_ANIM,


    //enemy
    ENEMY_1_ANIM,
    ENEMY_2_ANIM,
    ENEMY_3_ANIM,
    ENEMY_2_ATTACK,

    //boss
    BOSS_IDLE_ANIM,

    //effect
    EFFECT_DEAD_ANIM,
    EFFECT_ENEMYDEAD_ANIM,
    EFFECT_SPAWN_ANIM,
    EFFECT_HIT_ANIM,
    EFFECT_HIT_FIRE_ANIM,
    EFFECT_HIT_DARK_ANIM,
    EFFECT_HIT_THUNDER_ANIM,
    EFFECT_HIT_WIND_ANIM,


};

class GameObject;

class Animator
{
private:
    static std::map<LOOP_ANIM, ANIM_DATA> DICTIONARY_;

    GameObject* parent_;  //アニメーション対象のゲームオブジェクト

    Vector2 pos_, scale_;
    float rot_ = 0.0f;
    int texNo_;
    bool is_discard_ = false;

    LOOP_ANIM loop_anim_; //ループする画像の種類
    LOOP_ANIM loop_anim_next_; //ループする画像の種類

    int fps_;      //フレームレート
    bool isAnim_ = false;   //アニメーションするか
    bool isMovable_; //移動するか
    int x_matrix_num_, y_matrix_num_;   //横と縦の画像の枚数
    int loop_start_x_, loop_start_y_;   //ループする初めの場所
    int loop_end_x_, loop_end_y_;       //ループする終わりの場所
    bool is_loop_ = false;              //ループ判定（treu=ループ）
    float img_change_time_ = 0.0333f;          //画像を切り替える時間の間隔
    float now_time_ = 0.0f;

    int now_matrix_number_ = 0;   //現在の行列の位置
    float u_ = 0.0f, v_ = 0.0f;
    Color color_ = Color(1.0f, 1.0f, 1.0f, 1.0f);

    int invert_ = 1;              //反転

    textures texture_enum_;       //そのオブジェクトが持つテクスチャのenum textures

public:
    Animator();

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
        if (isAnim_)
        {
            DrawSprite(texNo_, Vector2((GetPos().x - offset.x) * scale_x, (GetPos().y - offset.y) * scale_y), rot_,
                Vector2(scale_.x * scale_x, scale_.y * scale_y), color_, Vector2(GetU(), GetV()), Vector2(UWidth(), VHeight()));
        }
        else
        {
            DrawSprite(texNo_, Vector2((GetPos().x - offset.x) * scale_x, (GetPos().y - offset.y) * scale_y), rot_,
                Vector2(scale_.x * scale_x, scale_.y * scale_y), color_);
        }
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
    bool GetIsAnim(void) const { return isAnim_; }
    void SetIsAnim(bool isAnim) {
            isAnim_ = isAnim;
    }
    bool GetIsMovable(void) const { return isMovable_; }
    void SetIsMovable(bool isMovable) { isMovable_ = isMovable; }
    void SetTexenum(textures texture_enum) { texture_enum_ = texture_enum; } //enum texturesセット
    textures GetTexenum() { return texture_enum_; } //enum texturesゲット

    void SetIsLoop(bool is_loop) { is_loop_ = is_loop; }    //ループの設定（treu=ループ）
    void SetImgChangeTime(float img_change_time) { img_change_time_ = img_change_time; }    //次の画像に切り替える間隔 [1.0f=１秒]

    void SetLoopAnim(LOOP_ANIM loop_anim) { loop_anim_next_ = loop_anim; }    //ループするアニメーションの設定
    LOOP_ANIM GetLoopAnim(void) const { return loop_anim_; }                  //今ループしているアニメーション
    LOOP_ANIM GetLoopAnimNext(void) const { return loop_anim_next_; }         //次にループするアニメーション

    float UWidth(void) const { return (1.0f / x_matrix_num_) * invert_; }    //UV(U)の幅を取得
    float VHeight(void) const { return 1.0f / y_matrix_num_; }  //UV(V)の高さを取得
    float GetU(void) const { return u_; }   //UV(U)の値を取得
    float GetV(void) const { return v_; }   //UV(V)の値を取得

    void Invert(void) { invert_ *= -1; }    //反転

    void SetParent(GameObject* parent) { parent_ = parent; }
    GameObject* GetParent(void) const { return parent_; }

    void Discard() { is_discard_ = true; }
    bool GetDiscard() const { return is_discard_; }

    void StatusUpdate(Vector2 pos, Vector2 scale, float rot) {
        pos_ = pos;
        scale_ = scale;
        rot_ = rot;
    }

    void RendererRegister(void);    //レンダラーに登録

    void PlayerAnim(ATTRIBUTE_TYPE move,ATTRIBUTE_TYPE attack);


    static void InitDictionary(void);  //辞書登録
private:
    void LoopAnimation(void);
    void Reset(void);

};
