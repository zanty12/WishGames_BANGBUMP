#pragma once

#include <map>
#include "asset.h"
#include "texture.h"

struct IMAGE_DATA
{
	//���o�^��-1��Ԃ��悤�ɂ���
	int texNo = -1;	//�e�N�X�`���i���o�[

	//0�Ŋ���ƃG���[�Ȃ̂ł���̖h�~
	int xMatrixNum = 1;	//���̉摜�̐�
	int yMatrixNum = 1;	//�c�̉摜�̐�

	IMAGE_DATA() {}	//�����ƃG���[

	//----------------------------------------
	// int tex_num		�e�N�X�`���i���o�[
	// int x_matrix_num		���̉摜�̐�
	// int y_matrix_num		�c�̉摜�̐�
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
