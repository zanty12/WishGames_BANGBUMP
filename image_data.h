#pragma once

#include <map>
#include "asset.h"
#include "texture.h"

struct IMAGE_DATA
{
	int texNo;
	int xMatrixNum;
	int yMatrixNum;

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

	ImageDataDictionary()
	{
		//★完成した素材をセットしていく★
		//player
		img_data_[player] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player)), 1, 1);

		//attibute_move
		img_data_[fire_move] = IMAGE_DATA(LoadTexture(Asset::GetAsset(fire_move)), 5, 6);
		img_data_[dark_move] = IMAGE_DATA(LoadTexture(Asset::GetAsset(dark_move)), 5, 3);
		img_data_[thunder_move] = IMAGE_DATA(LoadTexture(Asset::GetAsset(thunder_move)), 5, 3);
		img_data_[wind_move] = IMAGE_DATA(LoadTexture(Asset::GetAsset(wind_move)), 5, 6);
		img_data_[dark_move_charge] = IMAGE_DATA(LoadTexture(Asset::GetAsset(dark_move_charge)), 5, 10);
		img_data_[thunder_move_charge] = IMAGE_DATA(LoadTexture(Asset::GetAsset(thunder_move_charge)), 5, 3);

		//attibute_attack
		img_data_[fire_attack] = IMAGE_DATA(LoadTexture(Asset::GetAsset(fire_attack)), 5, 6);
		img_data_[dark_attack] = IMAGE_DATA(LoadTexture(Asset::GetAsset(dark_attack)), 5, 6);
		img_data_[thunder_attack] = IMAGE_DATA(LoadTexture(Asset::GetAsset(thunder_attack)), 5, 6);
		img_data_[wind_attack] = IMAGE_DATA(LoadTexture(Asset::GetAsset(wind_attack)), 5, 6);

		//enemy
		img_data_[enemy1_anim] = IMAGE_DATA(LoadTexture(Asset::GetAsset(enemy1_anim)), 5, 4);
		img_data_[enemy2_anim] = IMAGE_DATA(LoadTexture(Asset::GetAsset(enemy2_anim)), 5, 6);
		img_data_[enemy3_anim] = IMAGE_DATA(LoadTexture(Asset::GetAsset(enemy3_anim)), 5, 6);

		//boss
		img_data_[boss_idle] = IMAGE_DATA(LoadTexture(Asset::GetAsset(boss_idle)), 5, 9);

		//effect
		img_data_[effect_dead] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_dead)), 5, 14);
		img_data_[effect_enemydead] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_enemydead)), 5, 8);
		img_data_[effect_spawn] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_spawn)), 5, 3);
		img_data_[effect_hit] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit)), 5, 2);
		img_data_[effect_hit_fire] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit_fire)), 5, 2);
		img_data_[effect_hit_dark] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit_dark)), 5, 2);
		img_data_[effect_hit_thunder] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit_thunder)), 5, 2);
		img_data_[effect_hit_wind] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit_wind)), 5, 2);

	}
};
