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
    //player
    player,
    //enemy
    //block
    //background
    bg_stage1_front,
    bg_stage1_back,
    bg_stage2_front,
    bg_stage2_back,
    bg_stage3_front,
    bg_stage3_back,
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
};

class Asset
{
public:
    static const std::map<textures,std::string> textures_;
    static const std::map<sounds,std::string> sounds_;
    static const std::map<maps,std::string> maps_;

    template<typename T>
    static std::string GetAsset(T asset);

};



