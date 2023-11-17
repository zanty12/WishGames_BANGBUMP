#include "mapmngr.h"
#include "map.h"
#include <fstream>
#include <sstream>
#include <string>

MapMngr::MapMngr(const char* file_name)
{
	std::ifstream file(file_name);
	if (!file)
	{
		std::cout << "error loading map";
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
	int y = height; //ã‚©‚ç“Ç‚Ýž‚Þ
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string item;
		int x = 0;
		while (std::getline(ss, item, ','))
		{
			map_->PutCell(x, y, stoi(item));
			x++;
		}
		y--;
	}
	file.close();
}