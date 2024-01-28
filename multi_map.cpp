#include "multi_map.h"
#include "mapenum.h"
#include "asset.h"
#include "sprite.h"
#include "texture.h"
#include "lib/collider2d.h"
#include "lib/win_time.h"
#include "multiplay.h"
#include "multi_skillorb.h"
#include "multi_attack.h"
#include "multi_enemy.h"
#include <fstream>
#include <iostream>
#include <sstream>

void MultiMap::Initialize() {
	texNumbers[MAP_READ_WALL] = LoadTexture("data/texture/wall.png");
	texNumbers[MAP_READ_FLOOR] = LoadTexture("data/texture/wall.png");
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
}

void MultiMap::Release(void) {
	// マップデータの削除
	if (map) delete map;    
	if (collMap) delete collMap;
	map = nullptr;
	collMap = nullptr;

	// オブジェクトデータ削除
	if (enemies) delete enemies;
	if (skillOrbs) delete skillOrbs;
	if (attacks) delete attacks;
	enemies = nullptr;
	skillOrbs = nullptr;
	attacks = nullptr;

	// 占領データ削除
	areaCaptures.clear();
}


void MultiMap::Load(std::string path)
{
	Release();

	skillOrbs = new MultiBehavior("SkillOrbMngr");
	enemies = new MultiBehavior("EnemiesMngr");
	attacks = new MultiBehavior("AttackMngr");

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
	collMap = new int[width * height];
	memset(map, -1, sizeof(int) * width * height);
	memset(collMap, -1, sizeof(int) * width * height);
	int y = height - 1; //上から読み込む
	while (std::getline(file, line))
	{
		std::stringstream ss2(line);
		std::string item2;
		int x = 0;
		while (std::getline(ss2, item2, ','))
		{
			Vector2 position = Vector2(x, y) * cellSize;

			//if got nothing
			if (item2 == "")
			{
				x++;
				continue;
			};
			// start spawn
			if (item2 == "S" || item2 == "s") {
				startPosition.push_back(position);
			}
			// map
			else {
				int id = stoi(item2);
				// スキルオーブの登録
				if (id == MAP_READ_ORB_SMALL) skillOrbs->Add<ServerSkillOrbSmall>(Transform(position));
				else if (id == MAP_READ_ORB_MID) skillOrbs->Add<ServerSkillOrbMidium>(Transform(position));
				else if (id == MAP_READ_ORB_BIG) skillOrbs->Add<ServerSkillOrbBig>(Transform(position));
				// エネミーの登録
				else if (id == MAP_READ_KOOPA) enemies->Add<Enemy1ServerSide>(this);
				else if(id == MAP_READ_HAMMERBRO ||
					id == MAP_READ_PHANTOM) {
					//skillOrbs->Add<ServerSkillOrb>(Transform(Vector2(x, y) * cellSize));
				}
				// エリアキャプチャの登録
				else if (id == MAP_READ_MULTI_AREA_CAPTURE) {
					areaCaptures.push_front(position);
				}
				// 登録
				else {
					GetColliderMap(x, y) = id;
					GetMap(x, y) = id;
				}
			}
			x++;
		}
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


	// 描画（背景）
	DrawSprite(backBGTexNo, screen * 0.5f, 0.0f, screen, Color::White);

	// 描画（ブロック）
	for (int x = leftBottomIdx.x; x <= rightTopIdx.x; x++) {
		for (int y = leftBottomIdx.y; y <= rightTopIdx.y; y++) {
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

int MultiMap::Collision(Vector2 &position, float radius) {
	Vector2 screen = Vector2(Graphical::GetWidth(), Graphical::GetHeight());						// 画面のサイズ
	Vector2Int leftBottomIdx = ToIndex(position - Vector2(radius, radius));							// 左下のインデックス
	Vector2Int rightTopIdx = ToIndex(position + Vector2(radius, radius));							// 右上のインデックス
	leftBottomIdx.x--;
	leftBottomIdx.y--;
	rightTopIdx.x++;
	rightTopIdx.y++;

	if (leftBottomIdx.x < 0) leftBottomIdx.x = 0;
	if (rightTopIdx.x >= width) rightTopIdx.x = width;
	if (leftBottomIdx.y < 0) leftBottomIdx.y = 0;
	if (rightTopIdx.y >= height) rightTopIdx.y = height;

	// 判定
	using namespace PHYSICS;
	NearHit hit;
	float minDistance = -1;
	int id = -1;
	
	for (int x = leftBottomIdx.x; x <= rightTopIdx.x; x++) {
		for (int y = leftBottomIdx.y; y <= rightTopIdx.y; y++) {
			// 範囲外なら処理をしない
			if (x < 0 || y < 0 || x >= width || y >= height) continue;

			// ブロックのIDを取得
			int tmpId = GetColliderMap(x, y);
			// 判定できるなら
			if (tmpId != -1) {
				Vector2 cellPos = ToPosition({ x, y });							// セルの座標
				Vertex1 playerCollision(position, radius);
				Vertex4 cellCollision(cellPos, 0.0f, Vector2(cellSize, cellSize));
				NearHit tmpHit;


				if (Collider2D::Touch(playerCollision, cellCollision, &tmpHit)) {

					float distance = (position - cellPos).Distance();
					if (minDistance < 0.0f || distance < minDistance) {
						hit = tmpHit;
						minDistance = distance;
						id = tmpId;
					}
				}
			}
		}
	}

	if (id != -1) {
		position = hit.position -hit.tilt.Normal() * cellSize * 0.25f;
	}
	return id;
}

void MultiMap::DropSkillOrb(unsigned int drop, Vector2 position, float magnitude) {
	// ドロップする
	while (0 < drop) {
		// ドロップする値
		int tmpDrop = 0;

		// ドロップするスキルオーブ
		ServerSkillOrb *skillorb = nullptr;

		// スキルポイントをドロップ（大）
		if (drop >= ServerSkillOrbBig::AddPoint) {
			skillorb = GetSkillOrbs()->Add<ServerSkillOrbBig>(Transform(position));
			tmpDrop = ServerSkillOrbBig::AddPoint;
		}
		// スキルポイントをドロップ（中）
		else if (drop >= ServerSkillOrbMidium::AddPoint) {
			skillorb = GetSkillOrbs()->Add<ServerSkillOrbMidium>(Transform(position));
			tmpDrop = ServerSkillOrbMidium::AddPoint;
		}
		// スキルポイントをドロップ（小）
		else if (drop >= ServerSkillOrbSmall::AddPoint) {
			skillorb = GetSkillOrbs()->Add<ServerSkillOrbSmall>(Transform(position));
			tmpDrop = ServerSkillOrbSmall::AddPoint;
		}
		else return;

		// 吹き飛ばす
		float rad = MATH::Rand(0.0f, MATH::PI);
		skillorb->velocity = Vector2(std::cos(rad), std::sinf(rad)) * magnitude;

		// スキルポイントを減らす
		drop -= tmpDrop;
	}
}

void MultiMap::AttackUpdate(void) {
	for (auto &attackObject : *GetAttacks()) {
		auto attack = attackObject.Cast<AttackServerSide>();

		// プレイヤーと判定
		for (auto &kvp : MultiPlayServer::clients_) {

			auto &player = kvp.second.player_;
			float maxRadius = attack->radius + player->radius;
			float maxRadiusSq = maxRadius * maxRadius;

			// イテレータの取得
			auto iterator = attack->touchGameObjects.find(player);

			// ダメージを与えてもいい時間ではないなら終了
			if (iterator != attack->touchGameObjects.end()) {
				if (iterator->second.GetNowTime() * 0.001f < attack->spanTime) continue;
				else iterator->second.Start();
			}
			// 計測開始
			else {
				attack->touchGameObjects[player] = WIN::Time();
				attack->touchGameObjects[player].Start();
			}

			// ダメージ
			if (maxRadiusSq >= Vector2::DistanceSq(attack->transform.position, player->transform.position)) {
				
				player->Damage(attack);
			}
		}


		// エネミーと判定
		for (auto &instance : *enemies) {
			auto enemy = instance.Cast<GameObjectServerSide>();
			float maxRadius = attack->radius + enemy->radius;
			float maxRadiusSq = maxRadius * maxRadius;

			// イテレータの取得
			auto iterator = attack->touchGameObjects.find(enemy);

			// ダメージを与えてもいい時間ではないなら終了
			if (iterator != attack->touchGameObjects.end()) {
				if (iterator->second.GetNowTime() * 0.001f < attack->spanTime) continue;
			}
			// 計測開始
			else {
				attack->touchGameObjects[enemy] = WIN::Time();
				attack->touchGameObjects[enemy].Start();
			}

			// ダメージ
			if (maxRadiusSq >= Vector2::DistanceSq(attack->transform.position, enemy->transform.position)) {
				// ダメージ

			}
		}
	}
}