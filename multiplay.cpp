#define _CRT_SECURE_NO_WARNINGS
#include "multiplay.h"
#include "xinput.h"
#include "time.h"
#include "multi_skillorb.h"
#include "multi_enemy.h"
#include "ini.h"
#include "move_scene_anim.h"
#include <windows.h>
#include <thread>

//#define DEBUG_LINK
//#define DEBUG_INPUT
//#define DEBUG_LOCKED;
//#define DEBUG_SENDLEN
std::wstring ParamPath;
std::map<int, CLIENT_DATA_SERVER_SIDE> MultiPlayServer::clients_;


MultiPlayFlowServerSide *MultiPlayServer::gameMode = nullptr;
/*******************************************************
  Server
********************************************************/
MultiPlayServer::MultiPlayServer() {
	WSAData data;
	Startup(v2_2, data);

	gameMode = new MultiPlayFlowServerSide(this);
}

int MultiPlayServer::Register(Address clientAddr, HEADER &header, Socket sockfd) {
	// ロック
	//lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "REGISTER LOCK";
#endif

	// プレイヤー作成
	Vector2 pos = Vector2(200, 200);
	float rot = 0.0f;
	Vector2 vel = Vector2::Zero;
	ServerPlayer *player = new ServerPlayer();
	player->SetMoveAttribute(new ServerFire(player));
	player->SetAttackAttribute(new ServerWater(player));
	if (gameMode->GetMap()->startPosition.size())
		player->transform.position = *gameMode->GetMap()->startPosition.begin();

	// ヘッダーの更新
	header.command = HEADER::RESPONSE_LOGIN;
	header.id = maxID++;

	// クライアントデータの作成
	CLIENT_DATA_SERVER_SIDE clientData = {
		header,
		sockfd,
		clientAddr,
		player
	};


	// プレイヤー追加
	clients_[header.id] = clientData;


	// 送信
	header.command = HEADER::RESPONSE_LOGIN;
	SendTo(this->sockfd_, (char *)&header, sizeof(HEADER), 0, clientAddr);
	std::cout << "Res >> ID:" << (maxID - 1) << " Login" << std::endl;



	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - REGISTER UNLOCK" << std::endl;
#endif
	//lock_.Unlock();

	return maxID - 1;
}

void MultiPlayServer::Unregister(int id) {
	// ロック
	//lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "UNREGISTER LOCK";
#endif


	// 解除
	clients_[id].header.command = HEADER::RESPONSE_LOGOUT;
	clients_[id].sockfd_.Close();

	// 送信
	SendTo(sockfd_, (char *)&clients_[id].header, sizeof(HEADER), 0, clients_[id].clientAddr_);
	std::cout << "Res >> ID:" << " Logout" << std::endl;

	// 削除
	delete clients_[id].player_;
	clients_.erase(id);



	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - UNREGISTER UNLOCK" << std::endl;
#endif
	//lock_.Unlock();
}

void MultiPlayServer::AllUnregister(void) {
	while (clients_.size())
	{
		Unregister(clients_.begin()->second.header.id);
	}
}

void MultiPlayServer::PlayerUpdate(void) {
	// ロック
	//lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "UPD LOCK";
#endif

	if (gameMode->GetGame()->IsPlayerMove()) {
		// プレイヤーの更新
		for (auto &kvp : clients_) {
			auto &client = kvp.second;
			auto &player = client.player_;

			Input::SetState(0, client.currentInput);
			Input::SetPreviousState(0, client.previousInput);
			player->map = gameMode->GetMap();
			player->Loop();
			gameMode->GetMap()->Collision(player->transform.position, player->transform.scale, &player->velocity, &player->blownVelocity, &player->gravityVelocity);

			// 移動させる
			player->transform.position += player->velocity + player->blownVelocity + player->gravityVelocity;
#ifdef DEBUG_INPUT
			std::cout << Input::GetStickLeft(0).x << ", " << Input::GetStickLeft(0).y << std::endl;
#endif
#ifdef DEBUG_LINK
			std::cout << player->transform.position.x << ", " << player->transform.position.y << std::endl;
#endif
		}

		// スキルオーブの更新
		gameMode->GetMap()->GetSkillOrbs()->AllLoop();

		// エネミーの更新
		gameMode->GetMap()->GetEnemies()->AllLoop();

		// 攻撃オブジェクトの更新
		gameMode->GetMap()->GetAttacks()->AllLoop();

		// 攻撃判定の更新
		gameMode->GetMap()->AttackUpdate();
	}
	// ゲームモードの更新
	gameMode->Update(clients_);


	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - UPD UNLOCK" << std::endl;
#endif
	//lock_.Unlock();
}

void MultiPlayServer::RecvUpdate(void) {
	// リクエスト情報を書き出す
	REQUEST_PLAYER req;
	req.ParseRequest(recvBuff);



	// ロック
	//lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "RCV LOCK";
#endif

	// プレイヤーの検索
	auto iterator = clients_.find(req.input.id);

	// 検索したなら
	if (iterator != clients_.end()) {
		CLIENT_DATA_SERVER_SIDE &data = iterator->second;
		// 入力情報を設定
		data.currentInput = req.input.curInput;
		data.previousInput = req.input.preInput;

#ifdef DEBUG_INPUT
		Input::SetState(1, data.currentInput);
		Input::SetPreviousState(1, data.previousInput);
		std::cout << Input::GetStickLeft(1).x << ", " << Input::GetPreviousStickLeft(1).y << std::endl;
#endif
	}

	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - RCV UNLOCK" << std::endl;
#endif
	//lock_.Unlock();
}

void MultiPlayServer::SendUpdate(void) {
	//DWORD startTime, currentTime, onceFrameTime;
	//startTime = currentTime = timeGetTime();
	//onceFrameTime = 1000 / 60;

	//while (!isFinish) {

		//currentTime = timeGetTime();

		//if (currentTime - startTime > onceFrameTime) {
		//	startTime = currentTime;

			//// ロック
			//lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "SND LOCK";
#endif
	// レスポンスの作成
	RESPONSE_PLAYER res;

	// レスポンス情報の登録
	res.mode = gameMode->GetMode();
	res.maxTime = gameMode->GetMaxTime();
	res.time = gameMode->GetTime();

	// クライアント情報の登録
	for (auto &kvp : clients_) {
		auto &client = kvp.second;
		auto &player = client.player_;
		res.clients.push_back({
			client.header.id,
			player->GetMoveAttribute()->GetAttribute(), player->GetAttackAttribute()->GetAttribute(),
			player->animType,
			player->transform.position, player->velocity, player->attackVelocity, player->chargeVelocity,
			player->score, player->damageEffectAttributeType, player->skillPoint, 0 }
		);
	}

	// オブジェクト情報の登録
	if (gameMode->GetMap()) {
		res.AddObjects(gameMode->GetMap()->GetSkillOrbs());		// スキルオーブ
		res.AddObjects(gameMode->GetMap()->GetEnemies());		// エネミー	
		res.AddObjects(gameMode->GetMap()->GetAttacks());		// アタック
	}

	// レスポンスの作成
	sendBuff = nullptr;
	res.CreateResponse(sendBuff, 0);

	// ゲームモードのレスポンス内容の結合
	gameMode->CreateResponse(sendBuff);

	// クライアント全員に送信する
	for (auto &kvp : clients_) {
		auto &client = kvp.second;

		// 登録されていないならスキップ
		if (client.header.id < 0) continue;

		HEADER header;
		header.id = client.header.id;
		header.command = HEADER::RESPONSE_UPDATE;
		memcpy(&sendBuff[0], &header, sizeof(HEADER));

#ifdef DEBUG_SENDLEN
		std::cout << "SENDBUFF : " << sendBuff.Length() << std::endl;
#endif

		// 送信
		SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, client.clientAddr_);
	}

	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - SND UNLOCK" << std::endl;
#endif
	//lock_.Unlock();
//}
//}
}

void MultiPlayServer::OpenTerminal(void) {
	// ソケット作成
	sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);
	// バインド用アドレス作成
	Address addr = Address(AddressFamily::IPV4, INADDR_ANY, PORT);
	// バインド
	sockfd_.Bind(addr);

	DWORD startTime, currentTime, onceFrameTime;
	startTime = currentTime = timeGetTime();
	onceFrameTime = 1000 / 60;


	// SendUpdate()をスレッドを立てて関数を呼び出す
	std::thread sendUpdateFunc(&MultiPlayServer::SendUpdate, this);



	MSG msg;
	while (true) {
		// メッセージ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				isFinish = true;
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// 受信
		{
			FD fd;
			fd.Add(sockfd_);
			Select(&fd, nullptr, nullptr, 0, 1);
			if (fd.Contains(sockfd_)) {
				Address clientAddr;
				int clientAddrLen = sizeof(clientAddr);
				char buff[MAX_BUFF] = {};



				// 受信
				int buffLen = RecvFrom(sockfd_, buff, MAX_BUFF, 0, &clientAddr, &clientAddrLen);

				if (0 <= buffLen) {
					HEADER *pHeader = (HEADER *)buff;

					switch (pHeader->command)
					{
					case HEADER::COMMAND::REQUEST_LOGIN: {
						// 登録
						std::cout << "Req << ID:*" << " Login" << std::endl;
						int id = Register(clientAddr, *pHeader, 0);
						// 送信
						std::cout << id << " : 登録しました" << std::endl;
						break;
					};
					case HEADER::COMMAND::REQUEST_LOGOUT: {
						// 解除
						std::cout << "Req << ID:" << pHeader->id << " Logout" << std::endl;
						Unregister(pHeader->id);
						std::cout << pHeader->id << " : 解除しました" << std::endl;
						break;
					}
					case HEADER::COMMAND::REQUEST_UPDATE: {
						// 更新
						recvBuff.Push(buff, buffLen);
						RecvUpdate();
						break;
					}
					}
				}
			}
		}

		// 更新
		{
			currentTime = timeGetTime();
			if (currentTime - startTime > onceFrameTime) {
				startTime = currentTime;
				if (clients_.size()) {

					PlayerUpdate();
				}
				SendUpdate();

				// スキルオールチートシート
				for (int i = 0; i < 10; i++) {
					std::string vk = std::to_string(i);
					if (GetAsyncKeyState(vk.c_str()[0])) {
						auto iterator = clients_.find(i);
						if (iterator == clients_.end()) continue;
						if (GetAsyncKeyState(VK_UP)) {
							iterator->second.player_->skillPoint++;
						}
						if (GetAsyncKeyState(VK_DOWN)) {
							iterator->second.player_->skillPoint--;
							if (iterator->second.player_->skillPoint < 0) iterator->second.player_->skillPoint = 0;
						}
					}
				}
			}
		}



		if (GetAsyncKeyState(VK_ESCAPE)) {
			isFinish = true;
			break;
		}

		Time::Update();

		recvBuff = nullptr;
	}

	sendUpdateFunc.join();
}











int MultiPlayClient::id = -1;
MultiPlayFlowClientSide *MultiPlayClient::gameMode = nullptr;
std::unordered_map<int, ClientPlayer *> MultiPlayClient::clients;


/*******************************************************
  Client
********************************************************/
Vector2 MultiPlayClient::offset;
MultiPlayClient::MultiPlayClient() : texNo(LoadTexture("data/texture/player.png")) {
	WSAData data;
	Startup(v2_2, data);

	gameMode = new MultiPlayFlowClientSide(this);

	// スレッドを立てる
	sendUpdateFunc = std::thread(&MultiPlayClient::SendUpdate, this);
	recvUpdateFunc = std::thread(&MultiPlayClient::Update, this);

	// 受信用領域を確保する
	recvTmpBuff = new char[MAX_BUFF];

	// シーン遷移アニメーションの初期化
	MoveScene::Initialize();
}

int MultiPlayClient::Register(std::string serverAddress) {
	HEADER header;

	// IPV4アドレスの登録
	if (serverAddress == "") {
		std::wstring path = ROOT_PATH + L"multiplay.ini";
		serverAddress = ini::GetString(path.c_str(), L"System", L"Addr");
	}

	// ソケット作成
	sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);

	// アドレス作成
	serverAddr = Address(AddressFamily::IPV4, serverAddress.c_str(), PORT);

	// コマンド設定
	header.command = HEADER::COMMAND::REQUEST_LOGIN;

	// 操作
	readfd_.Add(sockfd_);

	// 送信
	std::cout << "Req >> ID:*" << " Login" << std::endl;
	SendTo(sockfd_, (char *)&header, sizeof(HEADER), 0, serverAddr);

	// 受信
	while (true) {
		Recv(sockfd_, (char *)&header, sizeof(HEADER), 0);
		if (header.command = HEADER::RESPONSE_LOGIN) break;
	}

	// IDを記録
	id = header.id;

	std::cout << "Res << ID:" << header.id << " Login" << std::endl;
	std::cout << header.id << "番目に登録しました。" << std::endl;

	return header.id;
}

void MultiPlayClient::Unregister(void) {
	HEADER header;

	// コマンド設定
	header.command = HEADER::REQUEST_LOGOUT;
	// ID設定
	header.id = id;

	// 送信
	std::cout << "Req << ID:" << header.id << " Logout" << std::endl;
	SendTo(sockfd_, (char *)&header, sizeof(HEADER), 0, serverAddr);

	std::cout << id << "を解除しました。" << std::endl;

	sockfd_.Close();
}

void MultiPlayClient::PlayerUpdate(void) {
	// カメラ座標の計算
	if (res_.clients.size()) {
		float posY = res_.clients.begin()->position.y;		// プレイヤーのY座標
		float centerY = Graphical::GetHeight() * 0.35f;		// 画面中心にするY座標
		float velY = res_.clients.begin()->moveVelocity.y;	// 加算するY座標

		float nextY = posY - centerY + velY * 10.0f;		// 移動先のY座標
		float ratio = 0.85f;								// 滑らかにする倍率
		offset += Vector2(0.0f, (nextY - offset.y) * ratio);
	}
	// ゲームモードの描画
	gameMode->Draw(res_, offset);

	// オブジェクトの描画
	for (auto iterator = objects.begin(); iterator != objects.end();) {
		// オブジェクト
		auto &object = iterator->second;

		// オブジェクトが表示されていないなら
		if (object->isPrevShow == false && object->isShow == false) {
			// 削除するイテレータを保存
			auto delIterator = iterator;

			// イテレータを一つ前に戻す
			iterator++;

			// 削除時に呼び出す
			delIterator->second->Release();

			// データの削除
			delete object;

			// イテレータの削除
			objects.erase(delIterator);
			continue;
		}

		// ループ処理
		object->Loop();
		object->isPrevShow = object->isShow;
		iterator++;
	}

	// プレイヤーの描画
	for (auto &player : clients) player.second->Loop();

	// エフェクトの描画
	if (gameMode->GetMap()) gameMode->GetMap()->GetEffects()->Draw(offset);

	// ライトエフェクトの描画
	lightEffect.Draw(offset);

	// UIの描画
	gameMode->DrawUI(res_);

	// シーン遷移アニメーション
	MoveScene::Loop();


#ifdef DEBUG_LINK
	if (res.clients.size()) std::cout << res.clients.begin()->position.x << ", " << res.clients.begin()->position.y << std::endl;
#endif
}

void MultiPlayClient::SendUpdate(void) {
	DWORD startTime, currentTime, onceFrameTime;
	startTime = currentTime = timeGetTime();
	onceFrameTime = 1000 / 60;

	while (!isFinish) {
		currentTime = timeGetTime();

		if (currentTime - startTime > onceFrameTime) {
			startTime = currentTime;
			// リクエストの作成
			REQUEST_PLAYER req;
			req.input = { id, Input::GetState(0), Input::GetPreviousState(0), move_, action_ };
			//req.input.curInput.sThumbLX = (GetAsyncKeyState('A') - GetAsyncKeyState('D')) * +16384;
			//req.input.curInput.sThumbLY = (GetAsyncKeyState('W') - GetAsyncKeyState('S')) * -16384;
			req.CreateRequest(sendBuff, id);

			// 送信
			SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, serverAddr);
			sendBuff = nullptr;

			// 入力更新
			Input::Update();
		}
	}
}

void MultiPlayClient::RecvUpdate(int waitTime) {
	int	buffLen = 0;
	bool IsNotLag = true;
	int recvCount = 0;		// 受信回数

	while (true) {
		// ファイルディスクリプタ
		FD tmp;
		memcpy(&tmp, &readfd_, sizeof(FD));
		Select(&tmp, nullptr, nullptr, 0, 1);

		// 受信
		if (tmp.Contains(sockfd_)) {
			// 受信する
			buffLen = Recv(sockfd_, recvTmpBuff, MAX_BUFF, 0);
			recvCount++;

			// 終了
			if (recvCount && buffLen <= 0) return;

			// ラグ対策ループをしない
			if (recvCount && !IsNotLag) break;
		}
		// 受信終了
		else if (recvCount) {
			break;
		}
	}


	Address serverAddr;
	int serverAddrLen = sizeof(serverAddr);
	memset(&recvBuff[0], 0, sizeof(char) * recvBuff.Length());


	if (0 < buffLen) {
		RESPONSE_PLAYER res;

		// データを取り込む
		recvBuff.Push(recvTmpBuff, buffLen);

		// レスポンスの解析
		res.ParseResponse(recvBuff);

		// クライアント更新
		for (auto &client : res.clients) {
			// IDを検索する
			auto iterator = clients.find(client.id);

			// オブジェクトが作成されていないなら作成する
			if (iterator == clients.end()) {
				auto player = new ClientPlayer(client.moveAttributeType, client.attackAttributeType, Transform(client.position));
				player->id = client.id;
				clients[client.id] = player;
			}
			else {
				auto &player = iterator->second;
				player->isShow = true;
				player->skillPoint = client.skillPoint;
				player->score = client.score;
				player->transform.position = client.position;
				player->velocity = client.moveVelocity;
				player->attackVelocity = client.attackVelocity;
				player->chargeVelocity = client.warpVelocity;
				player->animType = client.animType;
				player->moveAttributeType = client.moveAttributeType;
				player->attackAttributeType = client.attackAttributeType;
				player->damageEffectAttributeType = client.damageEffectAttributeType;
			}			
		}

		// オブジェクト更新
		for (auto &object : res.objects) {
			// IDを検索する
			auto iterator = objects.find(object.id);

			// オブジェクトが作成されていないなら作成する
			if (iterator == objects.end()) {
				GameObjectClientSide *pObject = nullptr;
				switch (object.tag) {
				case MULTI_OBJECT_TYPE::MULTI_ENEMY1: pObject = new Enemy1ClientSide(Transform(object.position)); break;
				case MULTI_OBJECT_TYPE::MULTI_ENEMY2: pObject = new Enemy2ClientSide(Transform(object.position)); break;
				case MULTI_OBJECT_TYPE::MULTI_ENEMY3: pObject = new Enemy3ClientSide(Transform(object.position)); break;
				case MULTI_OBJECT_TYPE::MULTI_ATTACK_ENEMY2: pObject = new AttackEnemy2ClientSide(Transform(object.position)); break;
				case MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_SMALL: pObject = new ClientSkillOrbSmall(); break;
				case MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_MIDIUM: pObject = new ClientSkillOrbMidium(); break;
				case MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_BIG: pObject = new ClientSkillOrbBig(); break;
				case MULTI_OBJECT_TYPE::MULTI_ATTACK_THUNDER: pObject = new ClientThunderAttack(Transform(object.position)); break;
				case MULTI_OBJECT_TYPE::MULTI_ATTACK_THUNDER2: pObject = new ClientThunder2Attack(Transform(object.position)); break;
				}
				if (pObject) objects[object.id] = pObject;
			}
			else {
				auto &obj = iterator->second;
				obj->isShow = true;
				obj->transform.position = object.position;
				obj->velocity = object.velocity;
				obj->damageEffectAttributeType = object.damageEffectAttributeType;
			}
		}

		// モードがNONEではないなら
		if (res.mode != MULTI_MODE::NONE) {
			// 受信したモードと実行しているゲームモードが同じなら解析する
			if (gameMode && res.mode == gameMode->GetMode()) gameMode->ParseResponse(recvBuff);

		}

		// 初期化
		recvBuff = nullptr;

		res_ = res;
	}
}

void MultiPlayClient::Update() {
	while (!isFinish) {
		if (GetAsyncKeyState(VK_ESCAPE)) {
			isFinish = true;
			break;
		}
		Graphical::Clear(Color(Color(1, 1, 1, 1) * 0.5f));
		Time::Update();
		RecvUpdate(1);
		PlayerUpdate();
		Graphical::Present();
		recvBuff = nullptr;
	}
}
