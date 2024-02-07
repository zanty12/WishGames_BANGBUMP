#include "asset.h"

#include "skillorb.h"

const std::map<textures, std::string> Asset::textures_ = {
    //player
     {textures::player, "data/texture/player.png"},

     {textures::player1_12, "data/texture/Player/player1/player1_12.png"},
     {textures::player1_13, "data/texture/Player/player1/player1_13.png"},
     {textures::player1_14, "data/texture/Player/player1/player1_14.png"},
     {textures::player1_21, "data/texture/Player/player1/player1_21.png"},
     {textures::player1_23, "data/texture/Player/player1/player1_23.png"},
     {textures::player1_24, "data/texture/Player/player1/player1_24.png"},
     {textures::player1_31, "data/texture/Player/player1/player1_31.png"},
     {textures::player1_32, "data/texture/Player/player1/player1_32.png"},
     {textures::player1_34, "data/texture/Player/player1/player1_34.png"},
     {textures::player1_41, "data/texture/Player/player1/player1_41.png"},
     {textures::player1_42, "data/texture/Player/player1/player1_42.png"},
     {textures::player1_43, "data/texture/Player/player1/player1_43.png"},

     {textures::player2_12, "data/texture/Player/player2/player2_12.png"},
     {textures::player2_13, "data/texture/Player/player2/player2_13.png"},
     {textures::player2_14, "data/texture/Player/player2/player2_14.png"},
     {textures::player2_21, "data/texture/Player/player2/player2_21.png"},
     {textures::player2_23, "data/texture/Player/player2/player2_23.png"},
     {textures::player2_24, "data/texture/Player/player2/player2_24.png"},
     {textures::player2_31, "data/texture/Player/player2/player2_31.png"},
     {textures::player2_32, "data/texture/Player/player2/player2_32.png"},
     {textures::player2_34, "data/texture/Player/player2/player2_34.png"},
     {textures::player2_41, "data/texture/Player/player2/player2_41.png"},
     {textures::player2_42, "data/texture/Player/player2/player2_42.png"},
     {textures::player2_43, "data/texture/Player/player2/player2_43.png"},

     {textures::player3_12, "data/texture/Player/player3/player3_12.png"},
     {textures::player3_13, "data/texture/Player/player3/player3_13.png"},
     {textures::player3_14, "data/texture/Player/player3/player3_14.png"},
     {textures::player3_21, "data/texture/Player/player3/player3_21.png"},
     {textures::player3_23, "data/texture/Player/player3/player3_23.png"},
     {textures::player3_24, "data/texture/Player/player3/player3_24.png"},
     {textures::player3_31, "data/texture/Player/player3/player3_31.png"},
     {textures::player3_32, "data/texture/Player/player3/player3_32.png"},
     {textures::player3_34, "data/texture/Player/player3/player3_34.png"},
     {textures::player3_41, "data/texture/Player/player3/player3_41.png"},
     {textures::player3_42, "data/texture/Player/player3/player3_42.png"},
     {textures::player3_43, "data/texture/Player/player3/player3_43.png"},

     {textures::player4_12, "data/texture/Player/player4/player4_12.png"},
     {textures::player4_13, "data/texture/Player/player4/player4_13.png"},
     {textures::player4_14, "data/texture/Player/player4/player4_14.png"},
     {textures::player4_21, "data/texture/Player/player4/player4_21.png"},
     {textures::player4_23, "data/texture/Player/player4/player4_23.png"},
     {textures::player4_24, "data/texture/Player/player4/player4_24.png"},
     {textures::player4_31, "data/texture/Player/player4/player4_31.png"},
     {textures::player4_32, "data/texture/Player/player4/player4_32.png"},
     {textures::player4_34, "data/texture/Player/player4/player4_34.png"},
     {textures::player4_41, "data/texture/Player/player4/player4_41.png"},
     {textures::player4_42, "data/texture/Player/player4/player4_42.png"},
     {textures::player4_43, "data/texture/Player/player4/player4_43.png"},

     //attibute_move
     {textures::fire_move, "data/texture/Move/effect_fire_move.png"},
     {textures::dark_idle, "data/texture/Move/effect_water_idle.png"},
     {textures::dark_move, "data/texture/Move/effect_water_move.png"},
     {textures::thunder_move, "data/texture/Move/effect_thunder_move.png"},
     {textures::wind_idle, "data/texture/Move/effect_wind_idle.png"},
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
     {textures::boss_idle,"data/texture/Boss/boss_idle.png"},
     {textures::boss_attack_fire,"data/texture/Boss/effect_boss_fire.png"},
     {textures::boss_attack_thunder,"data/texture/Boss/effect_boss_thunder.png"},
     {textures::boss_attack_water,"data/texture/Boss/effect_boss_water.png"},
     {textures::boss_attack_wind,"data/texture/Boss/effect_boss_wind.png"},

     //effect
     {textures::effect_dead,"data/texture/Effect/effect_dead.png"},
     {textures::effect_enemydead,"data/texture/Effect/effect_enemydead.png"},
     {textures::effect_spawn,"data/texture/Effect/effect_spawn.png"},
     {textures::effect_water_indicator,"data/texture/Effect/UI_water_indicator.png"},
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
    {textures::thunder_indicator, "data/texture/UI/thunder_indicator.png"},
};

const std::map<sounds, std::string> Asset::sounds_ = std::map<sounds, std::string>{
    {sounds::TitleBGM,"data/sound/title.wav"},
    {sounds::Attribute_selectionBGM,"data/sound/attribute_selection.wav"},
    {sounds::PracticeBGM,"data/sound/practice.wav"},
    {sounds::Multi_s1BGM,"data/sound/multi_s1.wav"},
    {sounds::Multi_s2BGM,"data/sound/multi_s2.wav"},
    {sounds::Multi_s3BGM,"data/sound/multi_s3.wav"},

    {sounds::Fire_atk,"data/sound/fire_atk.wav"},
    {sounds::Fire_move,"data/sound/fire_move.wav"},
    {sounds::Mizu_atk,"data/sound/MIZU_attack.wav"},
    {sounds::Mizu_move,"data/sound/mizu_move.wav"},
    {sounds::Thunder_atk,"data/sound/LITENING_attack.wav"},
    {sounds::Thunder_move,"data/sound/thunder_move.wav"},
    {sounds::Wind_atk,"data/sound/wind_atk.wav"},
    {sounds::Wind_move,"data/sound/wind_move.wav"},

    {sounds::Enemy_damage_1,"data/sound/enemy_damage_1.wav"},
    {sounds::Enemy_damage_2,"data/sound/enemy_damage_2.wav"},
    {sounds::Enemy_damage_2,"data/sound/enemy_atk.wav"},
    {sounds::Enemy_destroy,"data/sound/enemy_destroy.wav"},

    {sounds::Game_start,"data/sound/game_start.wav"},
    {sounds::Game_over,"data/sound/gameover.wav"},

    {sounds::Button_selection,"data/sound/button_selection.wav"},
    {sounds::Attribute,"data/sound/attribute.wav"},

    {sounds::Get_point,"data/sound/get_point.wav"},
    {sounds::Point_falling_1,"data/sound/point_falling_1.wav"},
    {sounds::Point_falling_2,"data/sound/point_falling_2.wav"},

    {sounds::block,"data/sound/block.wav"},
    {sounds::block2,"data/sound/block2.wav"},

    {sounds::Player_damage1,"data/sound/player_damage1.wav"},
    {sounds::Player_damage2,"data/sound/player_damage2.wav"},
    {sounds::Player_damage3,"data/sound/player_damage3.wav"},
 
};

const std::map<maps, std::string> Asset::maps_ = std::map<maps, std::string>{
    {maps::test, "data/map/1.csv"},
    {maps::test2, "data/map/2.csv"},
    {maps::single_stage_1, "data/map/SoloPlay_map.csv"},
    {maps::single_stage_2, "data/map/Boss_Map.csv"},
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
