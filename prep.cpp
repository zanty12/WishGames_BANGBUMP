#include "prep.h"

std::map<VIDEO_FILE, std::string> video_file_map =
{
    {FIRE_MOVE, "./data/video/fire_move.mp4"},
    {FIRE_ATTACK, "./data/video/fire_attack.mp4"},
    {WIND_MOVE, "./data/video/wind_move.mp4"},
    {WIND_ATTACK, "./data/video/wind_attack.mp4"},
    {THUNDER_MOVE, "./data/video/thunder_move.mp4"},
    {THUNDER_ATTACK, "./data/video/thunder_attack.mp4"},
    {DARK_MOVE, "./data/video/dark_move.mp4"},
    {DARK_ATTACK, "./data/video/dark_attack.mp4"},
};

Prep::Prep(SceneMngr* scene_mngr) : scene_mngr_(scene_mngr)
{
    SetNewVideo(move_);
    SetNewVideo(attack_);
    //通常テキスチャー
    character_ = LoadTexture("data/texture/UI/player.png");
    tex_fire_move_ = LoadTexture("data/texture/UI/fire_move.png");
    tex_fire_attack_ = LoadTexture("data/texture/UI/fire_attack.png");
    tex_wind_move_ = LoadTexture("data/texture/UI/wind_move.png");
    tex_wind_attack_ = LoadTexture("data/texture/UI/wind_attack.png");
    tex_thunder_move_ = LoadTexture("data/texture/UI/thunder_move.png");
    tex_thunder_attack_ = LoadTexture("data/texture/UI/thunder_attack.png");
    tex_dark_move_ = LoadTexture("data/texture/UI/dark_move.png");
    tex_dark_attack_ = LoadTexture("data/texture/UI/dark_attack.png");
    //uiテキスチャー
    tex_bg_ = LoadTexture("data/texture/UI/prep_bg.png");
    tex_select_arrow_ = LoadTexture("data/texture/UI/select_arrow.png");
}

void Prep::Update()
{
    //動画の切り替え
    if (move_ != move_next_)
    {
        delete video_list_.at(move_);
        video_list_.erase(move_);
        move_ = move_next_;
        SetNewVideo(move_);
    }
    if (attack_ != attack_next_)
    {
        delete video_list_.at(attack_);
        video_list_.erase(attack_);
        attack_ = attack_next_;
        SetNewVideo(attack_);
    }
    if (is_move_)
    {
        video_list_.at(move_)->Update();
    }
    else
    {
        video_list_.at(attack_)->Update();
    }
}

void Prep::Draw()
{
    //背景
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    DrawSprite(tex_bg_, Vector2(static_cast<float>(Graphical::GetWidth()) / 2,
                                static_cast<float>(Graphical::GetHeight()) / 2), 0.0f,
               Vector2(1920 * scale_x, 1080 * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    //キャラクター
    DrawSprite(character_, Vector2(700 * scale_x, 1080 / 2 * scale_y), 0.0f,
               Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    //移動属性
    int move_tex;
    switch (move_)
    {
    case FIRE_MOVE:
        move_tex = tex_fire_move_;
        break;
    case WIND_MOVE:
        move_tex = tex_wind_move_;
        break;
    case THUNDER_MOVE:
        move_tex = tex_thunder_move_;
        break;
    case DARK_MOVE:
        move_tex = tex_dark_move_;
        break;
    default:
        move_tex = tex_fire_move_;
        break;
    }
    //draw move at same position
    DrawSprite(move_tex, Vector2(700 * scale_x, 1080 / 2 * scale_y), 0.0f,
               Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    //攻撃属性
    int attack_tex;
    switch (attack_)
    {
    case FIRE_ATTACK:
        attack_tex = tex_fire_attack_;
        break;
    case WIND_ATTACK:
        attack_tex = tex_wind_attack_;
        break;
    case THUNDER_ATTACK:
        attack_tex = tex_thunder_attack_;
        break;
    case DARK_ATTACK:
        attack_tex = tex_dark_attack_;
        break;
    default:
        attack_tex = tex_fire_attack_;
        break;
    }
    //draw attack at same position
    DrawSprite(attack_tex, Vector2(700 * scale_x, 1080 / 2 * scale_y), 0.0f,
               Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    if (is_move_)
    {
        video_list_.at(move_)->Draw();
    }
    else
    {
        video_list_.at(attack_)->Draw();
    }
}

void Prep::DebugMenu()
{
    ImGui::Begin("Move Attribute");
    ImGui::RadioButton("Fire", (int*)&move_next_, FIRE_MOVE);
    ImGui::RadioButton("Wind", (int*)&move_next_, WIND_MOVE);
    ImGui::RadioButton("Thunder", (int*)&move_next_, THUNDER_MOVE);
    ImGui::RadioButton("Dark", (int*)&move_next_, DARK_MOVE);
    if (ImGui::Button("select"))
    {
        if (!is_move_)
        {
            is_move_ = true;
            delete video_list_.at(move_);
            video_list_.erase(move_);
            SetNewVideo(move_);
        }
    }
    ImGui::End();
    ImGui::Begin("Attack Attribute");
    ImGui::RadioButton("Fire", (int*)&attack_next_, FIRE_ATTACK);
    ImGui::RadioButton("Wind", (int*)&attack_next_, WIND_ATTACK);
    ImGui::RadioButton("Thunder", (int*)&attack_next_, THUNDER_ATTACK);
    ImGui::RadioButton("Dark", (int*)&attack_next_, DARK_ATTACK);
    if (ImGui::Button("select"))
    {
        if (is_move_)
        {
            is_move_ = false;
            delete video_list_.at(attack_);
            video_list_.erase(attack_);
            SetNewVideo(attack_);
        }
    }
    ImGui::End();
    ImGui::Begin("Game Start");
    if (ImGui::Button("Start"))
    {
        scene_mngr_->ChangeScene(SCENE_GAME);
    }
    ImGui::End();
}

void Prep::SetNewVideo(VIDEO_FILE video_file)
{
    video_list_.emplace(video_file, new Video(video_file_map.at(video_file).c_str()));
    video_list_.at(video_file)->SetSize(Vector2(853.3 * static_cast<float>(Graphical::GetWidth()) / 1920,
                                           480 * static_cast<float>(Graphical::GetHeight()) / 1080));
    video_list_.at(video_file)->SetLoop(true);
    video_list_.at(video_file)->SetWindowPos(Vector2(1400 * static_cast<float>(Graphical::GetWidth()) / 1920,
                                                1000 / 2 * static_cast<float>(Graphical::GetHeight()) / 1080));
}


