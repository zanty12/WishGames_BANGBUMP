#include "asset.h"

#include "skillorb.h"

const std::map<textures, std::string> Asset::textures_ = {
    {textures::player,"data/texture/player.png"},
    //player
    {textures::player1_12_idle,"data/texture/Player/player1_12_idle.png"},
    {textures::player1_13_idle,"data/texture/Player/player1_13_idle.png"},
    {textures::player1_14_idle,"data/texture/Player/player1_14_idle.png"},
    {textures::player1_21_idle,"data/texture/Player/player1_21_idle.png"},
    {textures::player1_23_idle,"data/texture/Player/player1_23_idle.png"},
    {textures::player1_24_idle,"data/texture/Player/player1_24_idle.png"},
    {textures::player1_31_idle,"data/texture/Player/player1_31_idle.png"},
    {textures::player1_32_idle,"data/texture/Player/player1_32_idle.png"},
    {textures::player1_34_idle,"data/texture/Player/player1_34_idle.png"},
    {textures::player1_41_idle,"data/texture/Player/player1_41_idle.png"},
    {textures::player1_42_idle,"data/texture/Player/player1_42_idle.png"},
    {textures::player1_43_idle,"data/texture/Player/player1_43_idle.png"},

    //enemy
    {textures::enemy1_anim,"data/texture/Enemy/enemy1_anim.png"},
    {textures::enemy2_anim,"data/texture/Enemy/enemy2_anim.png"},
    {textures::enemy3_anim,"data/texture/Enemy/enemy3_anim.png"},

    //boss
    {textures::boss_idle,"data/texture/boss_idle.png"},

    {textures::skill_orb, "data/texture/blocks/skillorb.png"},
    {textures::bg_stage1_front, "data/texture/bg/BG_stage1_front.png"},
    {textures::bg_stage1_back, "data/texture/bg/BG_stage1_back.png"},
    {textures::bg_stage2_front, "data/texture/bg/BG_stage2_front.png"},
    {textures::bg_stage2_back, "data/texture/bg/BG_stage2_back.png"},
    {textures::bg_stage3_front, "data/texture/bg/BG_stage3_front.png"},
    {textures::bg_stage3_back, "data/texture/bg/BG_stage3_back.png"},
    {textures::timer, "data/texture/UI/timer.png"},
};

const std::map<sounds, std::string> Asset::sounds_ = std::map<sounds, std::string>{};

const std::map<maps, std::string> Asset::maps_ = std::map<maps, std::string>{
    {test, "data/map/1.csv"},
    {single_stage_1, "data/map/single_stage_1.csv"},
    {single_stage_2, "data/map/2.csv"},
};

const std::map<fonts, std::wstring> Asset::fonts_ = {
    {fonts::notosans_jp, L"data/texture/NotoSansJP-Bold.ttf"},
    {fonts::wapuro_mincho, L"data/texture/wapuro-mincho.otf"},
};

template <typename T>
std::string Asset::GetAsset(T asset)
{
    return GetAsset<T>(asset);
}

template <>
std::string Asset::GetAsset<textures>(const textures asset)
{
    if (textures_.find(asset) != textures_.end())
        return textures_.at(asset);
    else return "";
}

template <>
std::string Asset::GetAsset<sounds>(const sounds asset)
{
    if (sounds_.find(asset) != sounds_.end())
        return sounds_.at(asset);
    else return "";
}

template <>
std::string Asset::GetAsset<maps>(const maps asset)
{
    if (maps_.find(asset) != maps_.end())
        return maps_.at(asset);
    else return "";
}

std::wstring Asset::GetFont(const fonts asset)
{
    if (fonts_.find(asset) != fonts_.end())
        return fonts_.at(asset);
    else return L"";
}
