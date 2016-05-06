#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED
#include <vector>

enum MessageType: uint8_t {
	CONNECT = 0,	// client->server  "<username>"
	CONFIRM,		// server->client  "<ID> <username>"
	MAP_REQ,		// client->server	n/a
	MAP,			// server->client	MapMessage
	WAIT,			// client->server	ID
	GAME_START,		// server->client	n/a
	POSITION,		// server<->client PositionMessage
	GAME_OVER,		// server->client  "<username>"
};

struct MessageHeader {
	MessageType type;
	size_t size;
};

struct MapMessage {
	size_t row;
	size_t col;
	size_t floor;
};

struct PositionMessage {
	uint8_t id;
	uint16_t posX;
	uint16_t posY;
	uint8_t direction;
	bool moving;
};

#endif // MESSAGE_H_INCLUDED