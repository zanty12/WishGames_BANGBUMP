#pragma once

#include <map>
#include "asset.h"
#include "texture.h"

struct IMAGE_DATA
{
	//未登録は-1を返すようにする
	int texNo = -1;	//テクスチャナンバー

	//0で割るとエラーなのでそれの防止
	int xMatrixNum = 1;	//横の画像の数
	int yMatrixNum = 1;	//縦の画像の数

	IMAGE_DATA() {}	//消すとエラー

	//----------------------------------------
	// int tex_num		テクスチャナンバー
	// int x_matrix_num		横の画像の数
	// int y_matrix_num		縦の画像の数
	//----------------------------------------
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
	static std::map<textures, IMAGE_DATA> img_data_;

	static void InitDictionary();

};
