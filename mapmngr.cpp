/**
 * @file mapmngr.cpp
 * @brief MapMngrクラスを実装します。
 * @author Cheung To Hung
 * @date 2023/11/22
 */

#include "mapmngr.h"
#include "map.h"
#include <fstream>
#include <sstream>
#include <string>
#include "mapenum.h"
#include "debugui.h"
#include "texture.h"

/**
 * @brief MapMngrクラスのコンストラクタ。
 * @param file_name マップデータを読み込むファイルの名前。
 */

MapMngr::MapMngr(const char* file_name)
{
    Read(file_name);
}

/**
 * @brief 指定したファイルからマップデータを読み込みます。
 * @param file_name マップデータを読み込むファイルの名前。
 * @return マップデータの読み込みが成功した場合はtrue、それ以外の場合はfalse。
 *
 * マップファイルを読み込み、マップを構築します。
 */

bool MapMngr::Read(const char* file_name)
{
    std::ifstream file(file_name);
    if (!file)
    {
        std::cout << "error loading map";
        return false;
    }
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    std::string item;
    std::getline(ss, item, ',');
    const int width = stoi(item);
    std::getline(ss, item, ',');
    const int height = stoi(item);
    map_ = new Map(width, height);
    int y = height - 1; //上から読み込む
    while (std::getline(file, line))
    {
        std::stringstream ss2(line);
        std::string item2;
        int x = 0;
        while (std::getline(ss2, item2, ','))
        {
            //if got nothing
            if (item2 == "")
            {
                x++;
                continue;
            };
            if (item2 == "S")
                spawn_ = Vector2(x * GameObject::size_ + GameObject::size_ / 2,
                                 y * GameObject::size_ + GameObject::size_ / 2);
            else if (stoi(item2) <= MAP_READ_ORB_BIG)
                map_->PutCell(x, y, stoi(item2));
            else
            {
                enemy_mngr_->Spawn(x, y, stoi(item2));
            }
            x++;
        }
        std::cout << std::endl;
        y--;
    }
    file.close();
    return true;
}

void MapMngr::DebugMenu()
{
    ImGui::Begin("Map");
    ImGui::Text("Spawn: %f, %f", spawn_.x, spawn_.y);
    ImGui::BeginTable("map", map_->GetWidth());
    for (int row = 0; row < map_->GetHeight(); row++)
    {
        ImGui::TableNextRow();
        for (int column = 0; column < map_->GetWidth(); column++)
        {
            ImGui::TableSetColumnIndex(column);
        }
    }
    ImGui::EndTable();
    ImGui::End();
}

void MapMngr::ArrangeBlkTex() const
{
    for (int y = 1; y < map_->GetHeight() - 1; y++)
    {
        for (int x = 1; x < map_->GetWidth() - 1; x++)
        {
            Cell* cell = map_->GetCell(x, y);
            if (cell == nullptr)
                continue;
            if (cell->GetCellType() == MAP_READ_FLOOR)
            {
                //change texture to start or end depending on surrounding block
                if ((map_->GetCell(x - 1, y) == nullptr || map_->GetCell(x - 1, y)->GetCellType() == MAP_READ_WALL) &&
                    map_->GetCell(x + 1, y)->GetCellType() == MAP_READ_FLOOR) //左は空気か壁で右は床
                    cell->SetTexNo(LoadTexture("data/TEXTURE/floor_start.png"));
                else if ((map_->GetCell(x + 1, y) == nullptr || map_->GetCell(x + 1, y)->GetCellType() == MAP_READ_WALL) &&
                    map_->GetCell(x - 1, y)->GetCellType() == MAP_READ_FLOOR)
                    cell->SetTexNo(LoadTexture("data/TEXTURE/floor_end.png"));
            }
        }
    }
}
