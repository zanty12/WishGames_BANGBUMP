#include "asset.h"

#include "skillorb.h"

const std::map<textures, std::string> Asset::textures_ = {
    {textures::player, "data/texture/player.png"},
    {textures::fire_attack, "data/texture/attack/fire_attack.png"},
    {textures::wind_attack, "data/texture/attack/wind_attack.png"},
    {textures::water_attack, "data/texture/attack/water_attack.png"},
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
    {maps::test, "data/map/1.csv"},
    {maps::single_stage_1, "data/map/single_stage_1.csv"},
    {maps::single_stage_2, "data/map/2.csv"},
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
