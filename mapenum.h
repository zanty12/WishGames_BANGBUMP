/**
* @file mapenum.h
 * @brief MAP_READ列挙型を定義します。
 * @author Cheung To Hung
 * @date 2023/11/22
 *
 * この列挙型は、Mapに配置できるさまざまなタイプのセルを表すために使用されます。
 */

#pragma once

/**
 * @enum MAP_READ
 * @brief Mapに配置できるさまざまなタイプのセルを表す列挙型。
 */
typedef enum {
    MAP_READ_NONE, ///< 空のセルを表します。
    MAP_READ_WALL, ///< 壁セルを表します。
    MAP_READ_FLOOR, ///< 床セルを表します。
    MAP_READ_PENFLOOR, ///< 貫通可能な床セルを表します。
    MAP_READ_CLOUD, ///< 雲セルを表します。
    MAP_READ_THUNDERCLOUD, ///< 雷雲セルを表します。
    MAP_READ_BLOCK, ///< ブロックセルを表します。
    MAP_READ_ORB_SMALL_FIRE, ///< 小スキル玉セルを表します。
    MAP_READ_ORB_MID_FIRE, ///< 中スキル玉セルを表します。
    MAP_READ_ORB_BIG_FIRE, ///< 大スキル玉セルを表します。
    MAP_READ_ORB_SMALL_DARK, ///< 小スキル玉セルを表します。
    MAP_READ_ORB_MID_DARK, ///< 中スキル玉セルを表します。
    MAP_READ_ORB_BIG_DARK, ///< 大スキル玉セルを表します。
    MAP_READ_ORB_SMALL_WIND, ///< 小スキル玉セルを表します。
    MAP_READ_ORB_MID_WIND, ///< 中スキル玉セルを表します。
    MAP_READ_ORB_BIG_WIND, ///< 大スキル玉セルを表します。
    MAP_READ_ORB_SMALL_THUNDER, ///< 小スキル玉セルを表します。
    MAP_READ_ORB_MID_THUNDER, ///< 中スキル玉セルを表します。
    MAP_READ_ORB_BIG_THUNDER, ///< 大スキル玉セルを表します。
    MAP_READ_SPIKE_LEFT, ///< 左向きのトゲセルを表します。
    MAP_READ_SPIKE_RIGHT, ///< 右向きのトゲセルを表します。
    MAP_READ_SPIKE_UP, ///< 上向きのトゲセルを表します。
    MAP_READ_SPIKE_DOWN, ///< 下向きのトゲセルを表します。
    MAP_READ_MULTI_AREA_CAPTURE, ///< マルチの占領エリアを表します。
    // 英語名標準
    MAP_READ_KOOPA, // エネミー_1
    MAP_READ_HAMMERBRO, // エネミー_2
    MAP_READ_PHANTOM, // エネミー_3
}MAP_READ;