#pragma once
#include <cstdlib>
#include <cstdint>

enum class MessageType : uint8_t {
	Connect = 0, Disconnect = 1, Reconnect = 2, Status = 3, Ping = 4, Pong = 5, Attack = 6
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
	MessageType type;
	uint16_t size;
	uint8_t data[];
};

struct Status {
	float px, py;
	float vx, vy;
	Face face;
	int hp;
	playerChar unit;
};

struct StatusMessage {
	ID id;
	uint32_t order;
	Status stat;
};

struct AttackMessage {
	ID id;
	int attack;
};

#pragma pack(pop)
