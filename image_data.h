#pragma once

#include <map>
#include "asset.h"
#include "texture.h"

struct IMAGE_DATA
{
	int texNo;
	int xMatrixNum;
	int yMatrixNum;

	IMAGE_DATA(int tex_num, int x_matrix_num, int y_matrix_num)
	{
		texNo = tex_num;
		xMatrixNum = x_matrix_num;
		yMatrixNum = y_matrix_num;
	}
};

class ImageDataDictionary
{
public:
	std::map<textures, IMAGE_DATA> img_data_;

	ImageDataDictionary()
	{
		//šŠ®¬‚µ‚½‘fŞ‚ğƒZƒbƒg‚µ‚Ä‚¢‚­š
		img_data_[player] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player)), 1, 1);
	}
};
