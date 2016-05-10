#pragma once
#include <vector>

enum MessageType : uint8_t {
	CONNECT = 0,	// client->server: string( desiredName )
	CONFIRM,		// server->client: string( assignedID + " " + assignedName )
	MAP_REQUEST,	// client->server: string( "" )
	MAP_DATA,		// server->client: MapMessage{ height, width } + string( mapData )
	READY,			// client->server: string( "" )
	GAME_START,		// server->client: string( "" )
	UPDATE_INPUT,	// client->server: UpdateInputMessage{  }
	UPDATE_DATA,	// server->client: UpdateDataMessage{  }
	GAME_FINISH,	// server->client: string( "" )
	DISCONNECT		// client->server: string( "" )
};

struct MessageHeader {
	MessageType type;
	size_t size;
};

struct MapMessage {
	size_t clients;
	size_t height;
	size_t width;
};

struct UpdateDataMessage {
	uint8_t ID;
	bool isAlive;
	float posX;
	float posY;
	int spriteDir;
	int spriteAction;
	int currCostume;
};

struct UpdateInputMessage {
	uint8_t ID;
	
	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;
	
	bool interact;
	bool shift;
};