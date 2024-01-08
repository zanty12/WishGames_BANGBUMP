#include "asset.h"

const std::map<textures,std::string> Asset::textures_ = {
    {textures::player,"data/texture/player.png"},
    {textures::bg_stage1_front,"data/texture/bg/BG_stage1_front.png"},
    {textures::bg_stage1_back,"data/texture/bg/BG_stage1_back.png"},
    {textures::bg_stage2_front,"data/texture/bg/BG_stage2_front.png"},
    {textures::bg_stage2_back,"data/texture/bg/BG_stage2_back.png"},
    {textures::bg_stage3_front,"data/texture/bg/BG_stage3_front.png"},
    {textures::bg_stage3_back,"data/texture/bg/BG_stage3_back.png"},
};

const std::map<sounds,std::string> Asset::sounds_ = std::map<sounds,std::string>{};

template <typename T>
std::string Asset::GetAsset(T asset)
{
    return GetAsset<T>(asset);
}

template<>
std::string Asset::GetAsset<textures>(const textures asset)
{
    if(textures_.find(asset) != textures_.end())
        return textures_.at(asset);
    else return "";
}

template<>
std::string Asset::GetAsset<sounds>(const sounds asset){
    if(sounds_.find(asset) != sounds_.end())
        return sounds_.at(asset);
    else return "";
}
