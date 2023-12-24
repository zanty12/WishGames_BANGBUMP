#include "prep.h"

std::map<VIDEO_FILE, std::string> video_file_map =
{
    {FIRE_MOVE, "./data/video/test.mp4"},
    {FIRE_ATTACK, "./data/video/test1.mp4"},
    {WIND_MOVE, "./data/video/test.mp4"},
    {WIND_ATTACK, "./data/video/test1.mp4"},
    {THUNDER_MOVE, "./data/video/test.mp4"},
    {THUNDER_ATTACK, "./data/video/test1.mp4"},
    {DARK_MOVE, "./data/video/test.mp4"},
    {DARK_ATTACK, "./data/video/test1.mp4"},
};

Prep::Prep(SceneMngr* scene_mngr) : scene_mngr_(scene_mngr)
{
    video_list_.emplace(move_, new Video(video_file_map.at(move_).c_str()));
    video_list_.at(move_)->SetScale(0.5);
    video_list_.at(move_)->SetLoop(true);
    video_list_.emplace(attack_, new Video(video_file_map.at(attack_).c_str()));
    video_list_.at(attack_)->SetScale(0.5);
    video_list_.at(attack_)->SetLoop(true);
}

void Prep::Update()
{
    if(move_ != move_next_)
    {
        delete video_list_.at(move_);
        video_list_.erase(move_);
        move_ = move_next_;
        video_list_.emplace(move_, new Video(video_file_map.at(move_).c_str()));
        video_list_.at(move_)->SetScale(0.5);
        video_list_.at(move_)->SetLoop(true);
    }

    if(attack_ != attack_next_)
    {
        delete video_list_.at(attack_);
        video_list_.erase(attack_);
        attack_ = attack_next_;
        video_list_.emplace(attack_, new Video(video_file_map.at(attack_).c_str()));
        video_list_.at(attack_)->SetScale(0.5);
        video_list_.at(attack_)->SetLoop(true);
    }

    for(auto& video : video_list_)
    {
        video.second->Update();
    }
}

void Prep::Draw()
{
    for(auto& video : video_list_)
    {
        video.second->Draw();
    }
    ImGui::Begin("Move Attribute");
    ImGui::RadioButton("Fire", (int*)&move_next_, FIRE_MOVE);
    ImGui::RadioButton("Wind", (int*)&move_next_, WIND_MOVE);
    ImGui::RadioButton("Thunder", (int*)&move_next_, THUNDER_MOVE);
    ImGui::RadioButton("Dark", (int*)&move_next_, DARK_MOVE);
    ImGui::End();
    ImGui::Begin("Attack Attribute");
    ImGui::RadioButton("Fire", (int*)&attack_next_, FIRE_ATTACK);
    ImGui::RadioButton("Wind", (int*)&attack_next_, WIND_ATTACK);
    ImGui::RadioButton("Thunder", (int*)&attack_next_, THUNDER_ATTACK);
    ImGui::RadioButton("Dark", (int*)&attack_next_, DARK_ATTACK);
    ImGui::End();
}

void Prep::DebugMenu()
{

}
