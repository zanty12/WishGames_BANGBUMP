/**
* @file map.h
 * @brief Map�N���X���`���܂��B
 * @author Cheung To Hung
 * @date 2023/11/22
 *
 * ���̃N���X�́A�Q�[���}�b�v���\������Z����2D�O���b�h��\���܂��B
 */

#pragma once
#include "cell.h"

class Camera;

/**
 * @class Map
 * @brief �Q�[���}�b�v���\������Z����2D�O���b�h��\���܂��B
 *
 * ���̃N���X�́A�}�b�v�̍X�V�ƕ`��A�Z���̔z�u�Ǝ擾�̂��߂̃��\�b�h��񋟂��܂��B
 */
class Map
{
private:
    Cell** map_; ///< �}�b�v���\������Z����2D�z��B
    int width_; ///< �}�b�v�̕��i�Z���P�ʁj�B
    int height_; ///< �}�b�v�̍����i�Z���P�ʁj�B
public:
    /**
  * @brief �f�t�H���g�R���X�g���N�^�͍폜����Ă��܂��B
  *
  * ���̃R���X�g���N�^�́A�T�C�Y���w�肹����Map���쐬���邱�Ƃ�h�����߂ɍ폜����Ă��܂��B
  */
    Map() = delete;
    /**
     * @brief �w�肵���T�C�Y�ŐV����Map���\�z���܂��B
     * @param width �}�b�v�̕��i�Z���P�ʁj�B
     * @param height �}�b�v�̍����i�Z���P�ʁj�B
     */
    Map(int width, int height) : width_(width), height_(height)
    {
        map_ = new Cell*[height_ * width_];
        for (int i = 0; i < height_ * width_; i++)
        {
            map_[i] = nullptr;
        }
    }

    /**
     * @brief Map��j�����܂��B
     *
     * ���̃f�X�g���N�^�́A�}�b�v���̂��ׂẴZ�����폜���܂��B
     */
    ~Map()
    {
        for (int i = 0; i < width_ * height_; i++)
        {
            if (map_[i] != nullptr)
            {
                delete map_[i];
            }
        }
    }
    /**
      * @brief �}�b�v���̂��ׂẴZ�����X�V���܂��B
      *
      * ���̃��\�b�h�́A�}�b�v���̊e�Z����Update���\�b�h���Ăяo���܂��B
      */
    void Update();

    /**
     * @brief �}�b�v���̂��ׂẴZ����`�悵�܂��B
     *
     * ���̃��\�b�h�́A�}�b�v���̊e�Z����Draw���\�b�h���Ăяo���܂��B
     */
    void Draw(Camera* camera) const;

    /**
     * @brief �w�肵���ʒu�Ɏw�肵���^�C�v�̃Z����z�u���܂��B
     * @param x �ʒu��x���W�B
     * @param y �ʒu��y���W�B
     * @param cell_type �z�u����Z���̃^�C�v�B
     */
    void PutCell(int x, int y, int cell_type) const;
    /**
     * @brief �w�肵���ʒu�̃Z�����擾���܂��B
     * @param x �ʒu��x���W�B
     * @param y �ʒu��y���W�B
     * @return �w�肵���ʒu�̃Z���B
     */
    Cell* GetCell(int x, int y) const
    {
        if(x< 0 || y < 0 || x >= width_ || y >= height_)
            return nullptr;
        return map_[y * width_ + x];
    }

    /**
     * @brief �w�肵����Έʒu�̃Z�����擾���܂��B
     * @param x �ʒu��x���W�B
     * @param y �ʒu��y���W�B
     * @return �w�肵���ʒu�̃Z���B
     */
    Cell* GetCell(float x, float y) const
    {
        const int idx = std::floor((x - GameObject::SIZE_ / 2) / GameObject::SIZE_);
        const int idy = std::floor((y - GameObject::SIZE_ / 2) / GameObject::SIZE_);
        return GetCell(idx,idy);
    }
    int GetWidth() const
    {
        return width_;
    }
    int GetHeight() const
    {
        return height_;
    }
};
