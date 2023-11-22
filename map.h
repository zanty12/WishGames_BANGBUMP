#pragma once
#include "cell.h"

class Map
{
private:
		Cell** map_;
		int width_;
		int height_;
	public:
	Map() = delete;
	Map(int width, int height) : width_(width), height_(height) //横と縦サイズ指定してマップ作成
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
		}
	}
	void Update()
	{
		for(int i = 0; i < width_ * height_; i++)
		{
			map_[i]->Update();
		}
	}
	void Draw()
	{
		for(int i = 0; i < width_ * height_; i++)
		{
			map_[i]->Draw();
		}
	}
	void PutCell(int x, int y, int cell_type);
	//マス座標を入力してセルを取得
	Cell* GetCell (int x, int y) const
	{
		return map_[y * width_ + x];
	}
	//絶対座標を入力してセルを取得
	Cell* GetCell(float x, float y) const
	{
		const int idx = std::floor((x - GameObject::size_ / 2) / GameObject::size_);
		const int idy = std::floor((y - GameObject::size_ / 2) / GameObject::size_);
		return map_[idy * width_ + idx];
	}
};