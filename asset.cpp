#include "asset.h"

#include "skillorb.h"

const std::map<textures, std::string> Asset::textures_ = {
    //player
    {textures::player, "data/texture/player.png"},

    //attibute_move
    {textures::fire_move, "data/texture/Move/effect_fire_move.png"},
    {textures::dark_move, "data/texture/Move/effect_water_move.png"},
    {textures::thunder_move, "data/texture/Move/effect_thunder_move.png"},
    {textures::wind_move, "data/texture/Move/effect_wind_move.png"},
    {textures::dark_move_charge, "data/texture/Move/effect_water_charge.png"},
    {textures::thunder_move_charge, "data/texture/Move/effect_thunder_charge.png"},

    //attibute_attack
    {textures::fire_attack, "data/texture/Attack/effect_fire_attack.png"},
    {textures::dark_attack, "data/texture/Attack/effect_water_attack.png"},
    {textures::thunder_attack, "data/texture/Attack/effect_thunder_arrow.png"},
    {textures::wind_attack, "data/texture/Attack/effect_wind_attack.png"},

    //enemy
    {textures::enemy1_anim,"data/texture/Enemy/enemy1_anim.png"},
    {textures::enemy2_anim,"data/texture/Enemy/enemy2_anim.png"},
    {textures::enemy3_anim,"data/texture/Enemy/enemy3_anim.png"},
    {textures::enemy2_attack,"data/texture/Enemy/effect_enemy2_attack.png"},

    //boss
    {textures::boss_idle,"data/texture/boss_idle.png"},

    //effect
    {textures::effect_dead,"data/texture/Effect/effect_dead.png"},
    {textures::effect_enemydead,"data/texture/Effect/effect_enemydead.png"},
    {textures::effect_spawn,"data/texture/Effect/effect_spawn.png"},
    {textures::effect_hit,"data/texture/Effect/HitEffect/effect_hit_all.png"},
    {textures::effect_hit_fire,"data/texture/Effect/HitEffect/effect_hit_fire.png"},
    {textures::effect_hit_dark,"data/texture/Effect/HitEffect/effect_hit_water.png"},
    {textures::effect_hit_thunder,"data/texture/Effect/HitEffect/effect_hit_thunder.png"},
    {textures::effect_hit_wind,"data/texture/Effect/HitEffect/effect_hit_wind.png"},

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
        return maps_.at(test);
    else return "";
}

std::wstring Asset::GetFont(const fonts asset)
{
    if (fonts_.find(asset) != fonts_.end())
        return fonts_.at(asset);
    else return L"";
}
