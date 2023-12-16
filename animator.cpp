
#include "animator.h"
#include "gameobject.h"
#include "game.h"
#include "renderer.h"

#include "time.h"


void Animator::InitDictionary(void)
{
    DICTIONARY_[FIRE] = ANIM_DATA(0, 0, 1, 1);//�����B���ꂼ��̃A�j���[�V�������Ƃɐݒ肵�Ă�����
    DICTIONARY_[WIND] = ANIM_DATA(2, 1, 3, 5);

}

Animator::Animator(GameObject* game_object)
    : parent_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()),
    now_matrix_number_(0), u_(0.0f), v_(0.0f), isAnim_(false)//������
{
    InitDictionary();

    if (!Game::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}

Animator::Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time)
    : parent_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()), img_change_time_(img_change_time),
    fps_(fps), isAnim_(isAnim),
    x_matrix_num_(x_matrix_num), y_matrix_num_(y_matrix_num), now_time_(0.0f), is_loop_(false),
    now_matrix_number_(0), u_(0.0f), v_(0.0f)//������
{
    InitDictionary();

    if (!Game::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}

Animator::Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time, bool is_loop, LOOP_ANIM loop_anim)
    : parent_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()),
    fps_(fps), isAnim_(isAnim), img_change_time_(img_change_time),
    x_matrix_num_(x_matrix_num), y_matrix_num_(y_matrix_num), now_time_(0.0f), is_loop_(is_loop),
    loop_anim_(loop_anim),
    now_matrix_number_(0), u_(0.0f), v_(0.0f)//������
{
    InitDictionary();

    if (!Game::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}



void Animator::Update(void)
{
    //���̃Q�[���I�u�W�F�N�g�̏�Ԃ𔽉f������
    pos_ = parent_->GetPos();
    scale_ = parent_->GetScale();
    rot_ = parent_->GetRot();

    if (!isAnim_)
    {
        return; //�A�j���[�V�������Ȃ��Ȃ�Δ�����
    }

    now_time_ += Time::GetDeltaTime();

    if (loop_anim_ != loop_anim_next_)
    {
        Reset();
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
        //���̍ő吔 * �ړI�̏c�̏ꏊ + �ړI�̉��̏ꏊ(10*4+6=46�݂�����)
        now_matrix_number_ = x_matrix_num_ * DICTIONARY_[loop_anim_].loop_start_y + DICTIONARY_[loop_anim_].loop_start_x - 1;//���̌�C���N�������g����̂�1�����Ă���
    }
}

void Animator::Reset(void)
{
    loop_anim_ = loop_anim_next_;
    now_matrix_number_ = x_matrix_num_ * DICTIONARY_[loop_anim_].loop_start_y + DICTIONARY_[loop_anim_].loop_start_x - 1;//���̌�C���N�������g����̂�1�����Ă���
}
