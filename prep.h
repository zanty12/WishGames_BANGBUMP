/** �����V�[��
 * �v���C���[�̑�����I������V�[��
 *
 * Author: Cheung To Hung Date: 2023/12/29
 */

#pragma once
#include "scene.h"
#include "player.h"
#include "video.h"

enum VIDEO_FILE
{
    FIRE_MOVE,
    FIRE_ATTACK,
    WIND_MOVE,
    WIND_ATTACK,
    THUNDER_MOVE,
    THUNDER_ATTACK,
    DARK_MOVE,
    DARK_ATTACK,
};

class SceneMngr;
class Prep : public Scene
{
private:
    //�v���C���[�̑���
    VIDEO_FILE move_ = FIRE_MOVE;
    VIDEO_FILE attack_ = FIRE_ATTACK;
    VIDEO_FILE move_next_ = FIRE_MOVE;
    VIDEO_FILE attack_next_ = FIRE_ATTACK;
    //�d�l�������Ȃ����ǁA�ꉞ
    std::map<VIDEO_FILE, Video*> video_list_;

    SceneMngr* scene_mngr_;

    bool is_move_ = true;

    //�ʏ�e�L�X�`���[
    int character_;
    int tex_fire_move_;
    int tex_fire_attack_;
    int tex_wind_move_;
    int tex_wind_attack_;
    int tex_thunder_move_;
    int tex_thunder_attack_;
    int tex_dark_move_;
    int tex_dark_attack_;
    //ui�e�L�X�`���[
    int tex_bg_;
    int tex_select_arrow_;

public:
    Prep() = delete;
    Prep(SceneMngr* scene_mngr);

    ~Prep() override
    {
        for (auto& video : video_list_)
        {
            delete video.second;
        }
    }

    void Update() override;
    void Draw() override;
    void DebugMenu() override;

private:
    void SetNewVideo(VIDEO_FILE video_file);
};
