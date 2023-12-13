#include "animator.h"
#include "gameobject.h"
#include "game.h"
#include "renderer.h"

#include "time.h"

Animator::Animator(GameObject* game_object)
        : my_object_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()),
          now_matrix_number_(0),u_(0.0f),v_(0.0f), isAnim_(false)//������
{
    if(!Game::GetRenderer()->Add(this))
        std::cout<<"error creating animator for obj at "<<pos_.x<<", "<<pos_.y<<std::endl;
}

Animator::Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time)
    : my_object_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()), img_change_time_(img_change_time),
      fps_(fps), isAnim_(isAnim),
      x_matrix_num_(x_matrix_num), y_matrix_num_(y_matrix_num), now_time_(0.0f), is_loop_(false),
      now_matrix_number_(0), u_(0.0f), v_(0.0f)//������
{
    if (!Game::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}

Animator::Animator(GameObject* game_object, int fps, bool isAnim, int x_matrix_num, int y_matrix_num, float img_change_time, bool is_loop,
            int loop_start_x, int loop_start_y, int loop_end_x, int loop_end_y)
    : my_object_(game_object), pos_(game_object->GetPos()), scale_(game_object->GetScale()), texNo_(game_object->GetTexNo()),
      fps_(fps), isAnim_(isAnim), img_change_time_(img_change_time),
      x_matrix_num_(x_matrix_num), y_matrix_num_(y_matrix_num), now_time_(0.0f), is_loop_(is_loop),
      loop_start_x_(loop_start_x), loop_start_y_(loop_start_y), loop_end_x_(loop_end_x), loop_end_y_(loop_end_y),
      now_matrix_number_(0), u_(0.0f), v_(0.0f)//������
{
    if (!Game::GetRenderer()->Add(this))
        std::cout << "error creating animator for obj at " << pos_.x << ", " << pos_.y << std::endl;
}



void Animator::Update(void)
{
    //���̃Q�[���I�u�W�F�N�g�̏�Ԃ𔽉f������
    pos_ = my_object_->GetPos();
    scale_ = my_object_->GetScale();
    rot_ = my_object_->GetRot();

    if (!isAnim_)
    {
        return; //�A�j���[�V�������Ȃ��Ȃ�Δ�����
    }

    now_time_ += Time::GetDeltaTime();

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
    if (xMatrix == loop_end_x_ && yMatrix == loop_end_y_)
    {
        //���̍ő吔 * �ړI�̏c�̏ꏊ + �ړI�̉��̏ꏊ(10*4+6=46�݂�����)
        now_matrix_number_ = x_matrix_num_ * loop_start_y_ + loop_start_x_ - 1;//���̌�C���N�������g����̂�1�����Ă���
    }
}
