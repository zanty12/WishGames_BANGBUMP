
#include "animator.h"
#include "gamebase.h"
#include "renderer.h"
#include "gameobject.h"

#include "time.h"
#include "asset.h"
#include "image_data.h"

std::map<textures, IMAGE_DATA> ImageDataDictionary::img_data_;

std::map<LOOP_ANIM, ANIM_DATA> Animator::DICTIONARY_;

//textures enumのみの登録
ANIM_DATA::ANIM_DATA(textures texture)
{
    texture_enum = texture;
    loop_start_x = 0;
    loop_start_y = 0;
    loop_end_x = ImageDataDictionary::img_data_[texture_enum].xMatrixNum - 1;
    loop_end_y = ImageDataDictionary::img_data_[texture_enum].yMatrixNum - 1;
}

//★それぞれのアニメーションごとに設定していく★
void Animator::InitDictionary(void)
{
    //DICTIONARY_[PLAYER] = ANIM_DATA(LoadTexture(Asset::GetAsset(player)), 5, 6);

    //player
    DICTIONARY_[PLAYER_IDLE_ANIM] = ANIM_DATA(0, 0, 4, 5);
    DICTIONARY_[PLAYER_ATTACK_ANIM] = ANIM_DATA(0, 6, 3, 7);
    DICTIONARY_[PLAYER_FD_MOVE_ANIM] = ANIM_DATA(4, 7, 3, 13);
    DICTIONARY_[PLAYER_TW_MOVE_ANIM] = ANIM_DATA(4, 7, 4, 7);
    DICTIONARY_[PLAYER_TA_CHARGE_ANIM] = ANIM_DATA(); //thunderアタックチャージ（属性ごとに設定するため未設定）
    DICTIONARY_[PLAYER_TM_CHARGE_ANIM] = ANIM_DATA(0, 8, 2, 8); //thunder移動チャージ

    //move
    DICTIONARY_[FIRE_MOVE_ANIM] = ANIM_DATA(fire_move);
    DICTIONARY_[DARK_IDLE_ANIM] = ANIM_DATA(dark_idle);
    DICTIONARY_[DARK_MOVE_ANIM] = ANIM_DATA(dark_move);
    DICTIONARY_[THUNDER_MOVE_ANIM] = ANIM_DATA(thunder_move);
    DICTIONARY_[WIND_IDLE_ANIM] = ANIM_DATA(wind_idle);
    DICTIONARY_[WIND_MOVE_ANIM] = ANIM_DATA(wind_move);
    DICTIONARY_[DARK_MOVE_CHARGE_ANIM] = ANIM_DATA(0, 0, 2, 9, dark_move_charge);
    DICTIONARY_[THUNDER_MOVE_CHARGE_ANIM] = ANIM_DATA(thunder_move_charge);

    //attack
    DICTIONARY_[FIRE_ATTACK_ANIM] = ANIM_DATA(0, 0, 0, 5, fire_attack);
    DICTIONARY_[DARK_ATTACK_ANIM] = ANIM_DATA(dark_attack);
    DICTIONARY_[THUNDER_ATTACK_ANIM] = ANIM_DATA(thunder_attack);
    DICTIONARY_[WIND_ATTACK_ANIM] = ANIM_DATA(wind_attack);

    //enemy
    DICTIONARY_[ENEMY_1_ANIM] = ANIM_DATA(0, 0, 2, 3, enemy1_anim);
    DICTIONARY_[ENEMY_2_ANIM] = ANIM_DATA(enemy2_anim);
    DICTIONARY_[ENEMY_3_ANIM] = ANIM_DATA(enemy3_anim);
    DICTIONARY_[ENEMY_2_ATTACK] = ANIM_DATA(enemy2_attack);

    //boss
    DICTIONARY_[BOSS_IDLE_ANIM] = ANIM_DATA(0, 0, 3, 8, boss_idle);
    DICTIONARY_[BOSS_FIRE_ANIM] = ANIM_DATA(boss_attack_fire);
    DICTIONARY_[BOSS_THUNDER_ANIM] = ANIM_DATA(boss_attack_thunder);
    DICTIONARY_[BOSS_WATER_ANIM] = ANIM_DATA(boss_attack_water);
    DICTIONARY_[BOSS_WIND_ANIM] = ANIM_DATA(boss_attack_wind);

    //effect
    DICTIONARY_[EFFECT_DEAD_ANIM] = ANIM_DATA(effect_dead);
    DICTIONARY_[EFFECT_ENEMYDEAD_ANIM] = ANIM_DATA(effect_enemydead);
    DICTIONARY_[EFFECT_SPAWN_ANIM] = ANIM_DATA(effect_spawn);
    DICTIONARY_[EFFECT_WATER_INDICATOR] = ANIM_DATA(effect_water_indicator);
    DICTIONARY_[EFFECT_HIT_ANIM] = ANIM_DATA(effect_hit);
    DICTIONARY_[EFFECT_HIT_FIRE_ANIM] = ANIM_DATA(effect_hit_fire);
    DICTIONARY_[EFFECT_HIT_DARK_ANIM] = ANIM_DATA(effect_hit_dark);
    DICTIONARY_[EFFECT_HIT_THUNDER_ANIM] = ANIM_DATA(effect_hit_thunder);
    DICTIONARY_[EFFECT_HIT_WIND_ANIM] = ANIM_DATA(effect_hit_wind);



}

Animator::Animator()
    :loop_anim_(ANIM_NONE), loop_anim_next_(ANIM_NONE),
    now_matrix_number_(0), u_(0.0f), v_(0.0f), isAnim_(false), invert_(1)
{

    if (!GameBase::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}

Animator::Animator(GameObject* game_object)
    : parent_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()), loop_anim_(ANIM_NONE), loop_anim_next_(ANIM_NONE),
    now_matrix_number_(0), u_(0.0f), v_(0.0f), isAnim_(false), invert_(1)//初期化
{

    if (!GameBase::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}

Animator::Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time)
    : parent_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()), loop_anim_(ANIM_NONE), loop_anim_next_(ANIM_NONE),
    img_change_time_(img_change_time), fps_(fps), isAnim_(isAnim),
    x_matrix_num_(x_matrix_num), y_matrix_num_(y_matrix_num), now_time_(0.0f), is_loop_(false),
    now_matrix_number_(0), u_(0.0f), v_(0.0f), invert_(1)//初期化
{

    if (!GameBase::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}

Animator::Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time, bool is_loop, LOOP_ANIM loop_anim)
    : parent_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()),
    fps_(fps), isAnim_(isAnim), img_change_time_(img_change_time),
    x_matrix_num_(x_matrix_num), y_matrix_num_(y_matrix_num), now_time_(0.0f), is_loop_(is_loop),
    loop_anim_(loop_anim),
    now_matrix_number_(0), u_(0.0f), v_(0.0f), invert_(1)//初期化
{

    if (!GameBase::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}

void Animator::RendererRegister(void)
{
    if (!GameBase::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}



void Animator::Update(void)
{
    //今のゲームオブジェクトの状態を反映させる
    if (parent_ != nullptr)
    {
        if (parent_->GetDiscard() == true)
        {
            Discard();
            return;
        }

        pos_ = parent_->GetPos();
        scale_ = parent_->GetScale();
        rot_ = parent_->GetRot();
        color_ = parent_->GetColor();
    }

    if (loop_anim_ != loop_anim_next_)
    {
        Reset();
    }

    if (!isAnim_)
    {
        return; //アニメーションしないならば抜ける
    }

    now_time_ += Time::GetDeltaTime();


    //アニメーションができない状態なら抜ける
    if (loop_anim_ == ANIM_NONE && x_matrix_num_ <= 1 && y_matrix_num_ <= 1)
    {
        isAnim_ = false;
        return;
    }

    if (now_time_ >= img_change_time_)
    {
        now_matrix_number_++;

        if (now_matrix_number_ >= x_matrix_num_ * y_matrix_num_)
        {
            now_matrix_number_ = 0;
        }

        u_ = (now_matrix_number_ % x_matrix_num_) * UWidth();
        v_ = (now_matrix_number_ / x_matrix_num_) * VHeight();

        //反転は右端に行くとおかしくなるのでそれの修正
        if (invert_ == -1)
        {
            u_ += UWidth();
        }

        if (is_loop_) {
            LoopAnimation();
        }

        now_time_ = 0.0f;
    }

}

void Animator::LoopAnimation(void)
{
    int xMatrix = now_matrix_number_ % x_matrix_num_;
    int yMatrix = now_matrix_number_ / x_matrix_num_;
    if (xMatrix == loop_end_x_ && yMatrix == loop_end_y_)
    {
        //横の最大数 * 目的の縦の場所 + 目的の横の場所(10*4+6=46みたいな)
        now_matrix_number_ = x_matrix_num_ * loop_start_y_ + loop_start_x_ - 1;//この後インクリメントするので1引いておく
    }
}

void Animator::Reset(void)
{
    loop_anim_ = loop_anim_next_;

    if (loop_anim_next_ == ANIM_NONE)
    {
        now_matrix_number_ = 0;
        return;
    }

    isAnim_ = true;
    is_loop_ = true;

    //DICTIONARY_にテクスチャデータが登録されていればそれを使う
    if (DICTIONARY_[loop_anim_].texture_enum != texture_none)
    {
        textures texture = DICTIONARY_[loop_anim_].texture_enum;

        texNo_ = ImageDataDictionary::img_data_[texture].texNo;
        x_matrix_num_ = ImageDataDictionary::img_data_[texture].xMatrixNum;
        y_matrix_num_ = ImageDataDictionary::img_data_[texture].yMatrixNum;
    }
    //DICTIONARY_にテクスチャデータが登録されていなければAnimatorのImageDataDictionaryに登録されているテクスチャを使う
    else
    {
        texNo_ = ImageDataDictionary::img_data_[texture_enum_].texNo;
        x_matrix_num_ = ImageDataDictionary::img_data_[texture_enum_].xMatrixNum;
        y_matrix_num_ = ImageDataDictionary::img_data_[texture_enum_].yMatrixNum;
    }

    //ループの設定
    if (DICTIONARY_[loop_anim_].loop_start_x != -1)
    {
        loop_start_x_ = DICTIONARY_[loop_anim_].loop_start_x;
        loop_start_y_ = DICTIONARY_[loop_anim_].loop_start_y;
        loop_end_x_ = DICTIONARY_[loop_anim_].loop_end_x;
        loop_end_y_ = DICTIONARY_[loop_anim_].loop_end_y;
    }

    //シングルプレイの時の画像マトリックス設定
    if (parent_ && parent_->GetType() == OBJ_PLAYER)
    {
        Player* player = dynamic_cast<Player*>(parent_);
        if (player != nullptr)
        {
            PlayerAnim(player->GetAttribute()->GetAttribute(), player->GetAttackAttribute()->GetAttribute());
        }
    }

    now_matrix_number_ = x_matrix_num_ * loop_start_y_ + loop_start_x_ - 1;//この後インクリメントするので1引いておく
}

void Animator::PlayerAnim(ATTRIBUTE_TYPE move, ATTRIBUTE_TYPE attack)
{
    if (loop_anim_ == PLAYER_TA_CHARGE_ANIM)
    {
        if (move == ATTRIBUTE_TYPE_WIND)
        {
            loop_start_x_ = 0;
            loop_start_y_ = 8;
            loop_end_x_ = 4;
            loop_end_y_ = 8;
        }
        else
        {
            loop_start_x_ = 4;
            loop_start_y_ = 13;
            loop_end_x_ = 3;
            loop_end_y_ = 14;
        }
    }


}


