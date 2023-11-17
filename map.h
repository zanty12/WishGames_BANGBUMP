#pragma once
#include "cell.h".

class Map
{
private:
		Cell** map_;
		int width_;
		int height_;
	public:
	Map() = delete;
	Map(int width, int height) : width_(width), height_(height)
	{
		map_ = new Cell*[height_ * width_];
		for (int i = 0; i < height_ * width_; i++)
		{
			map_[i] = nullptr;
		}
	}
	~Map()
	{
		for(int i = 0; i < width_ * height_; i++)
		{
			if(map_[i] != nullptr)
			{
				delete map_[i];
			}
		})
	}
	void Draw();
	void Update();
	void PutCell(int x, int y, int cell_type);
	Cell* GetCell(int x, int y);
};