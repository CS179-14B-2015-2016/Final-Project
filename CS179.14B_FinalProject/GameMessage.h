#pragma once

#include <cstdlib>
#include <cstdint>

enum MessageType : uint8_t {
	Connect = 0, Disconnect = 1, Reconnect = 2, Ping = 3, Pong = 4, Broadcast = 6
};

enum BroadcastType : uint8_t {
	None = 0, Status = 1, Attack = 2, Hit = 3
};

enum Face : uint8_t {
	UP = 0, FRONT = 1, LEFT = 2, RIGHT = 3, NONE = 4
};

enum playerChar : uint8_t {
	WAR = 0, PESTILENCE = 1, FAMINE = 2, DEATH = 3, MATTHEW = 4, MARK = 5, LUKE = 6, JOHN = 7
};

typedef uint32_t ID;

#pragma pack(push, 1)

struct Message {
	ID origin;
	MessageType message_type;
	BroadcastType broadcast_type;
	uint16_t size;
	uint8_t data[];
};

struct StatusMessage {
	// uint32_t order;
	float px, py;
	// float vx, vy;
	int hp;
	// Face face;
	playerChar unit;
};

struct AttackMessage {
	int attack;
};

#pragma pack(pop)
