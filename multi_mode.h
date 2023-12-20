#pragma once
#include "multi_header.h"

class MultiPlayServerSide {
protected:
	MapMngr *map_ = nullptr;

public:
	MultiPlayServerSide(MapMngr* map) : map_(map) { }
	virtual void Update(std::list<CLIENT_DATA_SERVER_SIDE>& clients) = 0;
	virtual void CreateResponse(Storage& out) = 0;
};

class MultiPlayClientSide {
private:

public:
	MultiPlayClientSide(){ }
	virtual void Draw(CLIENT_DATA_CLIENT_SIDE client) = 0;
	virtual void ParseResponse(Storage& in) = 0;
};