/**
* @file mapenum.h
 * @brief MAP_READ�񋓌^���`���܂��B
 * @author Cheung To Hung
 * @date 2023/11/22
 *
 * ���̗񋓌^�́AMap�ɔz�u�ł��邳�܂��܂ȃ^�C�v�̃Z����\�����߂Ɏg�p����܂��B
 */

#pragma once

/**
 * @enum MAP_READ
 * @brief Map�ɔz�u�ł��邳�܂��܂ȃ^�C�v�̃Z����\���񋓌^�B
 */
typedef enum {
    MAP_READ_NONE, ///< ��̃Z����\���܂��B
    MAP_READ_WALL, ///< �ǃZ����\���܂��B
    MAP_READ_FLOOR, ///< ���Z����\���܂��B
    MAP_READ_PENFLOOR, ///< �ђʉ\�ȏ��Z����\���܂��B
    MAP_READ_CLOUD, ///< �_�Z����\���܂��B
    MAP_READ_THUNDERCLOUD, ///< ���_�Z����\���܂��B
    MAP_READ_BLOCK, ///< �u���b�N�Z����\���܂��B
    MAP_READ_ORB_SMALL_FIRE, ///< ���X�L���ʃZ����\���܂��B
    MAP_READ_ORB_MID_FIRE, ///< ���X�L���ʃZ����\���܂��B
    MAP_READ_ORB_BIG_FIRE, ///< ��X�L���ʃZ����\���܂��B
    MAP_READ_ORB_SMALL_DARK, ///< ���X�L���ʃZ����\���܂��B
    MAP_READ_ORB_MID_DARK, ///< ���X�L���ʃZ����\���܂��B
    MAP_READ_ORB_BIG_DARK, ///< ��X�L���ʃZ����\���܂��B
    MAP_READ_ORB_SMALL_WIND, ///< ���X�L���ʃZ����\���܂��B
    MAP_READ_ORB_MID_WIND, ///< ���X�L���ʃZ����\���܂��B
    MAP_READ_ORB_BIG_WIND, ///< ��X�L���ʃZ����\���܂��B
    MAP_READ_ORB_SMALL_THUNDER, ///< ���X�L���ʃZ����\���܂��B
    MAP_READ_ORB_MID_THUNDER, ///< ���X�L���ʃZ����\���܂��B
    MAP_READ_ORB_BIG_THUNDER, ///< ��X�L���ʃZ����\���܂��B
    MAP_READ_SPIKE_LEFT, ///< �������̃g�Q�Z����\���܂��B
    MAP_READ_SPIKE_RIGHT, ///< �E�����̃g�Q�Z����\���܂��B
    MAP_READ_SPIKE_UP, ///< ������̃g�Q�Z����\���܂��B
    MAP_READ_SPIKE_DOWN, ///< �������̃g�Q�Z����\���܂��B
    MAP_READ_MULTI_AREA_CAPTURE, ///< �}���`�̐�̃G���A��\���܂��B
    // �p�ꖼ�W��
    MAP_READ_KOOPA, // �G�l�~�[_1
    MAP_READ_HAMMERBRO, // �G�l�~�[_2
    MAP_READ_PHANTOM, // �G�l�~�[_3
}MAP_READ;