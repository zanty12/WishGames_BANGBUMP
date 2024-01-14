
#include "animator.h"
#include "gamebase.h"
#include "renderer.h"
#include "gameobject.h"

#include "time.h"
#include "asset.h"

//★それぞれのアニメーションごとに設定していく★
void Animator::InitDictionary(void)
{
    //player
    DICTIONARY_[PLAYER_IDOL] = ANIM_DATA(5, 14, 0, 0, 4, 5);        //待機
    DICTIONARY_[PLAYER_MOVE] = ANIM_DATA(5, 14, 0, 6, 4, 11);       //移動
    DICTIONARY_[PLAYER_ATTACK] = ANIM_DATA(5, 14, 0, 12, 4, 13);    //攻撃

    DICTIONARY_[PLAYER_CHARGE_TM] = ANIM_DATA(5, 20, 0, 14, 4, 19); //thunder移動
    DICTIONARY_[PLAYER_CHARGE_TA] = ANIM_DATA(5, 16, 0, 14, 4, 15); //thunder攻撃

    //enemy
    DICTIONARY_[ENEMY_1] = ANIM_DATA(5, 4, 0, 0, 2, 3, LoadTexture(Asset::GetAsset(enemy1_anim)));
    DICTIONARY_[ENEMY_2] = ANIM_DATA(5, 6, LoadTexture(Asset::GetAsset(enemy2_anim)));
    DICTIONARY_[ENEMY_3] = ANIM_DATA(5, 6, LoadTexture(Asset::GetAsset(enemy3_anim)));

    //boss
    DICTIONARY_[BOSS_IDLE] = ANIM_DATA(5, 9, 0, 0, 3, 8, LoadTexture(Asset::GetAsset(boss_idle)));

}

Animator::Animator(GameObject* game_object)
    : parent_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()), loop_anim_(NONE), loop_anim_next_(NONE),
    now_matrix_number_(0), u_(0.0f), v_(0.0f), isAnim_(false), invert_(1)//初期化
{
    InitDictionary();

    if (!GameBase::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}

Animator::Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time)
    : parent_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()), loop_anim_(NONE), loop_anim_next_(NONE),
    img_change_time_(img_change_time), fps_(fps), isAnim_(isAnim),
    x_matrix_num_(x_matrix_num), y_matrix_num_(y_matrix_num), now_time_(0.0f), is_loop_(false),
    now_matrix_number_(0), u_(0.0f), v_(0.0f), invert_(1)//初期化
{
    InitDictionary();

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
    InitDictionary();

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

    if (!isAnim_)
    {
        return; //アニメーションしないならば抜ける
    }

    now_time_ += Time::GetDeltaTime();

    if (loop_anim_ != loop_anim_next_)
    {
        Reset();
    }
    //アニメーションができない状態なら抜ける
    if (loop_anim_ == NONE && x_matrix_num_ == 0 && y_matrix_num_ == 0)
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
    if (xMatrix == DICTIONARY_[loop_anim_].loop_end_x && yMatrix == DICTIONARY_[loop_anim_].loop_end_y)
    {
        //横の最大数 * 目的の縦の場所 + 目的の横の場所(10*4+6=46みたいな)
        now_matrix_number_ = x_matrix_num_ * DICTIONARY_[loop_anim_].loop_start_y + DICTIONARY_[loop_anim_].loop_start_x - 1;//この後インクリメントするので1引いておく
    }
}

void Animator::Reset(void)
{
    loop_anim_ = loop_anim_next_;

    if (loop_anim_next_ == NONE)
    {
        now_matrix_number_ = 0;
        return;
    }

    isAnim_ = true;
    if (DICTIONARY_[loop_anim_].texNo != -1)
    {
        texNo_ = DICTIONARY_[loop_anim_].texNo;
    }
    else
    {
        if (parent_)
        {
            texNo_ = parent_->GetTexNo();
        }
    }

    if (parent_ && parent_->GetType() == OBJ_PLAYER)
    {
        PlayerAnim();
    }
    else
    {
        x_matrix_num_ = DICTIONARY_[loop_anim_].matrix_num_x;
        y_matrix_num_ = DICTIONARY_[loop_anim_].matrix_num_y;
    }

    now_matrix_number_ = x_matrix_num_ * DICTIONARY_[loop_anim_].loop_start_y + DICTIONARY_[loop_anim_].loop_start_x - 1;//この後インクリメントするので1引いておく
}

void Animator::PlayerAnim(void)
{
    Player* player = dynamic_cast<Player*>(parent_);
    if (player == nullptr)
    {
        return;
    }

    //移動アトリビュートがTHUNDERの時
    if (player->GetAttribute()->GetAttribute() == ATTRIBUTE_TYPE_THUNDER)
    {
        x_matrix_num_ = 5;
        y_matrix_num_ = 20;
    }
    //攻撃アトリビュートがTHUNDERの時
    else if (player->GetAttackAttribute()->GetAttribute() == ATTRIBUTE_TYPE_THUNDER)
    {
        x_matrix_num_ = 5;
        y_matrix_num_ = 16;
    }
    //どちらでもない時
    else
    {
        x_matrix_num_ = DICTIONARY_[loop_anim_].matrix_num_x;
        y_matrix_num_ = DICTIONARY_[loop_anim_].matrix_num_y;
    }
}
