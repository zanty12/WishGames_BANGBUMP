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
    player1_12_idle,
    player1_13_idle,
    player1_14_idle,
    player1_21_idle,
    player1_23_idle,
    player1_24_idle,
    player1_31_idle,
    player1_32_idle,
    player1_34_idle,
    player1_41_idle,
    player1_42_idle,
    player1_43_idle,

    //attibute_move
    fire_move,
    dark_move,
    thunder_move,
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

    //effect
    effect_dead,
    effect_enemydead,
    effect_spawn,
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
};

enum sounds
{
    //bgm
    //se
};

enum maps
{
    test,
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



