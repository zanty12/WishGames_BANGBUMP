#pragma once
#include <iostream>
#include "lib/network.h"
#include "lib/vector.h"
#include "mapmngr.h"
#include "player.h"

#define SERVER_ADDRESS "10.192.121.53"
#define MAX_MEMBER (4)
#define PORT (8080)

using namespace Network;

struct LOCAL_DATA {
	enum COMMAND {
		NONE,
		REQUEST_LOGIN,
		RESPONSE_LOGIN,
		REQUEST_LOGOUT,
		RESPONSE_LOGOUT,
		REQUEST_UPDATE,
		RESPONSE_UPDATE
	};

	int command = NONE;
	int id = -1;
};

struct REQUEST {
	struct PLAYER {
		Vector2 stick;
	};
	struct EFFECT {
		enum {
			NONE,
			FIRE,
			WATER,
			WIND,

		};

		Vector2 pos;
		float rot = 0.0f;
		Vector2 scl = Vector2::One;
		float power = 1.0f;
		int command = NONE;

		EFFECT(Vector2 pos, float rot, Vector2 scl, float power, int command) :
			pos(pos), rot(rot), scl(scl), power(power), command(command) { }
	};

	enum {
		NONE,
		MOVE,
	};

	int command = NONE;
	Vector2 vector;
};

struct RESPONSE {
	struct PLAYER {
		enum {
			NONE = 0x00,
			DAMAGE = 0x01,
		};

		int hp = 0;
		Vector2 pos;
		float rot = 0.0f;
		int status = NONE;

	};
	struct EFFECT {
		enum {
			NONE,
			FIRE,
			WATER,
			WIND,

		};

		Vector2 pos;
		float rot = 0.0f;
		Vector2 scl = Vector2::One;
		float power = 1.0f;
		int type = NONE;

		EFFECT(Vector2 pos, float rot, Vector2 scl, float power, int type) :
			pos(pos), rot(rot), scl(scl), power(power), type(type) { }
	};
	
	UINT playerNum = 0;
	UINT effectNum = 0;
	PLAYER *players_ = nullptr;
	EFFECT *effectors_ = nullptr;


};

class MultiServer : Server {
private:
	struct CLIENT_DATA {
		Socket sockfd_;
		Address clientAddr_;
		LOCAL_DATA localData;
	}; 

	int nowMember_ = 0;
	Socket sockfd_;
	CLIENT_DATA players_[MAX_MEMBER] = {};


private:
	int Register(Address clientAddr, LOCAL_DATA *localData, Socket sockfd);

	void Unregister(int id);

	void SendUpdate(LOCAL_DATA *pLocalData);

public:
	void OpenTerminal(void);
};

class Client : public Server {
private:
	int id = -1;
	Socket sockfd_;
	Address serverAddr;
	FD readfd_;
	Game game;

public:
	LOCAL_DATA localData;
	LOCAL_DATA players_[MAX_MEMBER - 1] = {};


	int Register();

	void Unregister();

	void Send(void);

	void Update(int waitTime);
};

