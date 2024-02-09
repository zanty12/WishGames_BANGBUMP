

#include "image_data.h"

void ImageDataDictionary::InitDictionary()
{
	img_data_[texture_none] = IMAGE_DATA(-1, 1, 1);	//ï‘Ç∑ílÇ-1Ç…ÇµÇƒï`âÊÇµÇ»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ

	//ÅöäÆê¨ÇµÇΩëfçﬁÇÉZÉbÉgÇµÇƒÇ¢Ç≠Åö
	//player
	img_data_[player] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player)), 1, 1);

	img_data_[player1_12] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_12)), 5, 14);
	img_data_[player1_13] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_13)), 5, 9);
	img_data_[player1_14] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_14)), 5, 8);
	img_data_[player1_21] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_21)), 5, 14);
	img_data_[player1_23] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_23)), 5, 9);
	img_data_[player1_24] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_24)), 5, 8);
	img_data_[player1_31] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_31)), 5, 15);
	img_data_[player1_32] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_32)), 5, 15);
	img_data_[player1_34] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_34)), 5, 9);
	img_data_[player1_41] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_41)), 5, 14);
	img_data_[player1_42] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_42)), 5, 14);
	img_data_[player1_43] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player1_43)), 5, 9);

	/*
	img_data_[player2_12] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_12)), 5, 14);
	img_data_[player2_13] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_13)), 5, 9);
	img_data_[player2_14] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_14)), 5, 8);
	img_data_[player2_21] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_21)), 5, 14);
	img_data_[player2_23] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_23)), 5, 9);
	img_data_[player2_24] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_24)), 5, 8);
	img_data_[player2_31] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_31)), 5, 15);
	img_data_[player2_32] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_32)), 5, 15);
	img_data_[player2_34] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_34)), 5, 9);
	img_data_[player2_41] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_41)), 5, 14);
	img_data_[player2_42] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_42)), 5, 14);
	img_data_[player2_43] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player2_43)), 5, 9);

	img_data_[player3_12] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_12)), 5, 14);
	img_data_[player3_13] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_13)), 5, 9);
	img_data_[player3_14] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_14)), 5, 8);
	img_data_[player3_21] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_21)), 5, 14);
	img_data_[player3_23] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_23)), 5, 9);
	img_data_[player3_24] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_24)), 5, 8);
	img_data_[player3_31] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_31)), 5, 15);
	img_data_[player3_32] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_32)), 5, 15);
	img_data_[player3_34] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_34)), 5, 9);
	img_data_[player3_41] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_41)), 5, 14);
	img_data_[player3_42] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_42)), 5, 14);
	img_data_[player3_43] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player3_43)), 5, 9);

	img_data_[player4_12] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_12)), 5, 14);
	img_data_[player4_13] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_13)), 5, 9);
	img_data_[player4_14] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_14)), 5, 8);
	img_data_[player4_21] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_21)), 5, 14);
	img_data_[player4_23] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_23)), 5, 9);
	img_data_[player4_24] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_24)), 5, 8);
	img_data_[player4_31] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_31)), 5, 15);
	img_data_[player4_32] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_32)), 5, 15);
	img_data_[player4_34] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_34)), 5, 9);
	img_data_[player4_41] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_41)), 5, 14);
	img_data_[player4_42] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_42)), 5, 14);
	img_data_[player4_43] = IMAGE_DATA(LoadTexture(Asset::GetAsset(player4_43)), 5, 9);
	*/

	//attibute_move
	img_data_[fire_move] = IMAGE_DATA(LoadTexture(Asset::GetAsset(fire_move)), 5, 6);
	img_data_[dark_idle] = IMAGE_DATA(LoadTexture(Asset::GetAsset(dark_idle)), 5, 6);
	img_data_[dark_move] = IMAGE_DATA(LoadTexture(Asset::GetAsset(dark_move)), 5, 3);
	img_data_[thunder_move] = IMAGE_DATA(LoadTexture(Asset::GetAsset(thunder_move)), 5, 3);
	img_data_[wind_idle] = IMAGE_DATA(LoadTexture(Asset::GetAsset(wind_idle)), 5, 6);
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
	img_data_[enemy2_attack] = IMAGE_DATA(LoadTexture(Asset::GetAsset(enemy2_attack)), 5, 6);

	//boss
	img_data_[boss_idle] = IMAGE_DATA(LoadTexture(Asset::GetAsset(boss_idle)), 5, 9);
	img_data_[boss_attack_fire] = IMAGE_DATA(LoadTexture(Asset::GetAsset(boss_attack_fire)), 5, 6);
	img_data_[boss_attack_thunder] = IMAGE_DATA(LoadTexture(Asset::GetAsset(boss_attack_thunder)), 5, 6);
	img_data_[boss_attack_water] = IMAGE_DATA(LoadTexture(Asset::GetAsset(boss_attack_water)), 5, 6);
	img_data_[boss_attack_wind] = IMAGE_DATA(LoadTexture(Asset::GetAsset(boss_attack_wind)), 5, 1);

	//effect
	img_data_[effect_dead] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_dead)), 5, 15);
	img_data_[effect_enemydead] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_enemydead)), 5, 8);
	img_data_[effect_spawn] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_spawn)), 5, 3);
	img_data_[effect_water_indicator] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_water_indicator)), 5, 4);
	img_data_[effect_water_atk_indicator] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_water_atk_indicator)), 5, 2);
	img_data_[effect_hit] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit)), 5, 2);
	img_data_[effect_hit_fire] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit_fire)), 5, 2);
	img_data_[effect_hit_dark] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit_dark)), 5, 2);
	img_data_[effect_hit_thunder] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit_thunder)), 5, 2);
	img_data_[effect_hit_wind] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_hit_wind)), 5, 2);
	img_data_[effect_EX] = IMAGE_DATA(LoadTexture(Asset::GetAsset(effect_EX)), 5, 6);
}
