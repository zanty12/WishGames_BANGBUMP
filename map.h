/**
* @file map.h
 * @brief Mapクラスを定義します。
 * @author Cheung To Hung
 * @date 2023/11/22
 *
 * このクラスは、ゲームマップを構成するセルの2Dグリッドを表します。
 */

#pragma once
#include "cell.h"

class Camera;

/**
 * @class Map
 * @brief ゲームマップを構成するセルの2Dグリッドを表します。
 *
 * このクラスは、マップの更新と描画、セルの配置と取得のためのメソッドを提供します。
 */
class Map
{
private:
    Cell** map_; ///< マップを構成するセルの2D配列。
    int width_; ///< マップの幅（セル単位）。
    int height_; ///< マップの高さ（セル単位）。
public:
    /**
  * @brief デフォルトコンストラクタは削除されています。
  *
  * このコンストラクタは、サイズを指定せずにMapを作成することを防ぐために削除されています。
  */
    Map() = delete;
    /**
     * @brief 指定したサイズで新しいMapを構築します。
     * @param width マップの幅（セル単位）。
     * @param height マップの高さ（セル単位）。
     */
    Map(int width, int height) : width_(width), height_(height)
    {
        map_ = new Cell*[height_ * width_];
        for (int i = 0; i < height_ * width_; i++)
        {
            map_[i] = nullptr;
        }
    }

    /**
     * @brief Mapを破棄します。
     *
     * このデストラクタは、マップ内のすべてのセルを削除します。
     */
    ~Map()
    {
        for (int i = 0; i < width_ * height_; i++)
        {
            if (map_[i] != nullptr)
            {
                delete map_[i];
            }
        }
    }
    /**
      * @brief マップ内のすべてのセルを更新します。
      *
      * このメソッドは、マップ内の各セルでUpdateメソッドを呼び出します。
      */
    void Update();

    /**
     * @brief マップ内のすべてのセルを描画します。
     *
     * このメソッドは、マップ内の各セルでDrawメソッドを呼び出します。
     */
    void Draw(Camera* camera) const;

    /**
     * @brief 指定した位置に指定したタイプのセルを配置します。
     * @param x 位置のx座標。
     * @param y 位置のy座標。
     * @param cell_type 配置するセルのタイプ。
     */
    void PutCell(int x, int y, int cell_type) const;
    /**
     * @brief 指定した位置のセルを取得します。
     * @param x 位置のx座標。
     * @param y 位置のy座標。
     * @return 指定した位置のセル。
     */
    Cell* GetCell(int x, int y) const
    {
        if(x< 0 || y < 0 || x >= width_ || y >= height_)
            return nullptr;
        return map_[y * width_ + x];
    }

    /**
     * @brief 指定した絶対位置のセルを取得します。
     * @param x 位置のx座標。
     * @param y 位置のy座標。
     * @return 指定した位置のセル。
     */
    Cell* GetCell(float x, float y) const
    {
        const int idx = std::floor((x - GameObject::SIZE_ / 2) / GameObject::SIZE_);
        const int idy = std::floor((y - GameObject::SIZE_ / 2) / GameObject::SIZE_);
        return GetCell(idx,idy);
    }
    int GetWidth() const
    {
        return width_;
    }
    int GetHeight() const
    {
        return height_;
    }
};
