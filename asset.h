/**
 * asset.h
 * basically a wrapper for all texture and sound assets(when they are added)
 *
 * author: DavidC date: 2024/01/08
 */

#pragma once
#include <map>
#include <string>

enum textures
{
    texture_none = -1,  //テクスチャ無し

    //player
    player,
    player1_12,
    player1_13,
    player1_14,
    player1_21,
    player1_23,
    player1_24,
    player1_31,
    player1_32,
    player1_34,
    player1_41,
    player1_42,
    player1_43,

    player2_12,
    player2_13,
    player2_14,
    player2_21,
    player2_23,
    player2_24,
    player2_31,
    player2_32,
    player2_34,
    player2_41,
    player2_42,
    player2_43,

    player3_12,
    player3_13,
    player3_14,
    player3_21,
    player3_23,
    player3_24,
    player3_31,
    player3_32,
    player3_34,
    player3_41,
    player3_42,
    player3_43,

    player4_12,
    player4_13,
    player4_14,
    player4_21,
    player4_23,
    player4_24,
    player4_31,
    player4_32,
    player4_34,
    player4_41,
    player4_42,
    player4_43,

    //attibute_move
    fire_move,
    dark_idle,
    dark_move,
    thunder_move,
    wind_idle,
    wind_move,
    dark_move_charge,
    thunder_move_charge,

    //attibute_attack
    fire_attack,
    dark_attack,
    thunder_attack,
    wind_attack,

    //enemy
    enemy1_anim,
    enemy2_anim,
    enemy3_anim,
    enemy2_attack,

    //boss
    boss_idle,
    boss_attack_fire,
    boss_attack_thunder,
    boss_attack_water,
    boss_attack_wind,

    //effect
    effect_dead,
    effect_enemydead,
    effect_spawn,
    effect_water_indicator,
    effect_hit,
    effect_hit_fire,
    effect_hit_dark,
    effect_hit_thunder,
    effect_hit_wind,

    //block
    skill_orb,
    //background
    bg_stage1_front,
    bg_stage1_back,
    bg_stage2_front,
    bg_stage2_back,
    bg_stage3_front,
    bg_stage3_back,
    //UI
    timer,
    thunder_indicator,
};

enum sounds
{
    //bgm
    //se
    SE_1,
    SE_2,
    SE_3,
    SE_4,
    SE_5,
    SE_6,

};

enum maps
{
    test,
    test2,
    single_stage_1,
    single_stage_2,
};

enum fonts
{
    notosans_jp,
    wapuro_mincho,
};

class Asset
{
public:
    static const std::map<textures,std::string> textures_;
    static const std::map<sounds,std::string> sounds_;
    static const std::map<maps,std::string> maps_;
    static const std::map<fonts,std::wstring> fonts_;

    template<typename T>
    static std::string GetAsset(T asset);
    static std::wstring GetFont(const fonts asset);
};



