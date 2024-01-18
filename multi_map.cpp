#include "multi_map.h"
#include "mapenum.h"
#include "asset.h"
#include "sprite.h"
#include "texture.h"
#include "lib/collider2d.h"
#include <fstream>
#include <iostream>
#include <sstream>

void MultiMap::Initialize() {
	texNumbers[MAP_READ_WALL] = LoadTexture("data/texture/wall.png");
	texNumbers[MAP_READ_FLOOR] = LoadTexture("data/texture/wall.png");
	texNumbers[MAP_READ_PENFLOOR] = LoadTexture("data/texture/penfloor.png");
	texNumbers[MAP_READ_CLOUD] = LoadTexture("data/texture/cloud.png");
	texNumbers[MAP_READ_BLOCK] = LoadTexture("data/texture/wall.png");
	texNumbers[MAP_READ_ORB_SMALL] = LoadTexture(Asset::textures_.at(textures::skill_orb));
	texNumbers[MAP_READ_ORB_MID] = LoadTexture(Asset::textures_.at(textures::skill_orb));
	texNumbers[MAP_READ_ORB_BIG] = LoadTexture(Asset::textures_.at(textures::skill_orb));
	texNumbers[MAP_READ_SPIKE_LEFT] = LoadTexture("data/texture/spike.png");
	texNumbers[MAP_READ_SPIKE_RIGHT] = LoadTexture("data/texture/spike.png");
	texNumbers[MAP_READ_SPIKE_UP] = LoadTexture("data/texture/spike.png");
	texNumbers[MAP_READ_SPIKE_DOWN] = LoadTexture("data/texture/spike.png");
	texNumbers[MAP_READ_MULTI_AREA_CAPTURE] = LoadTexture("data/texture/area_capture.png");
	// エネミー
	texNumbers[MAP_READ_KOOPA] = LoadTexture("data/texture/enemy1.png");
	texNumbers[MAP_READ_HAMMERBRO] = LoadTexture("data/texture/enemy2.png");
	texNumbers[MAP_READ_PHANTOM] = LoadTexture("data/texture/enemy3.png");

	gameObjects = new MultiBehavior("MapMngr");
}

void MultiMap::Release(void) {
	// マップデータの削除
	if (map) delete map;    
	map = nullptr;

	// オブジェクトデータ削除
	if (gameObjects) delete gameObjects;
	gameObjects = nullptr;
}

void MultiMap::Load(std::string path)
{
	Release();

	std::ifstream file(path);
	if (!file)
	{
		std::cout << "error loading map";
		return;
	}
	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	std::string item;
	std::getline(ss, item, ',');
	width = stoi(item);
	std::getline(ss, item, ',');
	height = stoi(item);
	// 領域確保
	map = new int[width * height];
	memset(map, -1, sizeof(int) * width * height);
	int y = height - 1; //上から読み込む
	while (std::getline(file, line))
	{
		std::stringstream ss2(line);
		std::string item2;
		int x = 0;
		while (std::getline(ss2, item2, ','))
		{
			//if got nothing
			if (item2 == "")
			{
				x++;
				continue;
			};
			// start spawn
			if (item2 == "S" || item2 == "s") {
				startPosition.push_back(Vector2(x, y));
			}
			// map
			else {
				// 登録
				GetMap(x, y) = stoi(item2);
			}
			x++;
		}
		std::cout << std::endl;
		y--;
	}
	file.close();
}

void MultiMap::Draw(Vector2 offset) {
	Vector2 screen = Vector2(Graphical::GetWidth(), Graphical::GetHeight());                        // 画面のサイズ
	Vector2Int leftBottomIdx = ToIndex(offset);                                                     // 左下のインデックス
	Vector2Int rightTopIdx = ToIndex(offset + screen);                                              // 右上のインデックス

	if (leftBottomIdx.x < 0) leftBottomIdx.x = 0;
	if (rightTopIdx.x >= width) rightTopIdx.x = width;
	if (leftBottomIdx.y < 0) leftBottomIdx.y = 0;
	if (rightTopIdx.y >= height) rightTopIdx.y = height;

	// 描画
	for (int x = leftBottomIdx.x; x < rightTopIdx.x; x++) {
		for (int y = leftBottomIdx.y; y < rightTopIdx.y; y++) {
			// 範囲外なら処理をしない
			if (x < 0 || y < 0 || x >= width || y >= height) continue;

			// ブロックのIDを取得
			int id = GetMap(x, y);
			// 描画できるなら
			if (id != -1) {
				int texNo = texNumbers[id];
				Vector2 pos = ToPosition({ x, y }) - offset;
				DrawSprite(texNo, pos, 0.0f, Vector2::One * cellSize, Color::White);
			}
		}
	}
}

int MultiMap::Collision(Vector2 &position, float radius, Vector2 *outNormal) {
	Vector2 screen = Vector2(Graphical::GetWidth(), Graphical::GetHeight());						// 画面のサイズ
	Vector2Int leftBottomIdx = ToIndex(position - Vector2(radius, radius));							// 左下のインデックス
	Vector2Int rightTopIdx = ToIndex(position + Vector2(radius, radius));							// 右上のインデックス

	if (leftBottomIdx.x < 0) leftBottomIdx.x = 0;
	if (rightTopIdx.x >= width) rightTopIdx.x = width;
	if (leftBottomIdx.y < 0) leftBottomIdx.y = 0;
	if (rightTopIdx.y >= height) rightTopIdx.y = height;

	// 判定
	using namespace PHYSICS;
	Vector2 normals[] = {
		Vector2::Up, Vector2::Right, Vector2::Down, Vector2::Left
	};
	
	for (int x = leftBottomIdx.x; x < rightTopIdx.x; x++) {
		for (int y = leftBottomIdx.y; y < rightTopIdx.y; y++) {
			// 範囲外なら処理をしない
			if (x < 0 || y < 0 || x >= width || y >= height) continue;

			// ブロックのIDを取得
			int id = GetMap(x, y);
			// 判定できるなら
			if (id != -1) {
				Vector2 cellPos = ToPosition({ x, y });							// セルの座標
				Vector2 startPos, endPos;										// 線分の始点終点
				Vector2 normal;													// 線分の法線ベクトル
				for (int i = 0; i < 4; i++) {
					Vector2 tmpCellPos = cellPos + normals[i] * cellSize;
					// 判定するセルを調べて記録する
					if (Vector2::Dot(normals[i], tmpCellPos - cellPos) < 0) {
						normal = normals[i];
						startPos = tmpCellPos + normals[i].Normal() * cellSize;
						endPos = tmpCellPos - normals[i].Normal() * cellSize;
						break;
					}
				}

				Vertex2 cellCollider(startPos, endPos);
				Vertex1 playerCollider(position, radius);
				NearHit hit;
				if (Collider2D::Touch(playerCollider, cellCollider, &hit)) {
					if (outNormal) *outNormal = normal;
					return id;
				}
			}
		}
	}
	return -1;
}

