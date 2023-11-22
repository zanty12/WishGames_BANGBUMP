#include "mapmngr.h"
#include "map.h"
#include <fstream>
#include <sstream>
#include <string>
#include "mapenum.h"

MapMngr::MapMngr(const char* file_name)
{
	Read(file_name);
}

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
	int y = height - 1; //ã‚©‚ç“Ç‚Ýž‚Þ
	while (std::getline(file, line))
	{
		std::stringstream ss2(line);
		std::string item2;
		int x = 0;
		while (std::getline(ss2, item2, ','))
		{
			//if got nothing
			if (item2 == "") continue;
			if (item2 == "S")
				spawn_ = Vector2(x * GameObject::size_ + GameObject::size_ / 2, y * GameObject::size_ + GameObject::size_ / 2);
			else if(stoi(item2) <= MAP_READ_ORB_BIG)
				map_->PutCell(x, y, stoi(item2));
			else
			{
				enemy_mngr_->Spawn(x, y, stoi(item2));
			}
			x++;
		}
		std::cout<<std::endl;
		y--;
	}
	file.close();
	return true;
}
