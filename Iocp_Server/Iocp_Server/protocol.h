#pragma once
#define MAX_BUFFER 1024
#define SERVER_ADDR "127.0.0.1"
#define MAX_USER 10
#define MAX_ACRONS 100
#define MAX_LEN 15

//const int SERVER_PORT = 9500;
constexpr int SERVER_PORT		= 9500;

//Client->Server
constexpr int CS_MOVE_PLAYER			= 1;
constexpr int CS_ROTATER_PLAYER		= 2;
constexpr int CS_EVENT_PLAYER			= 3;
constexpr int CS_CHANGE_LEVEL		= 4;

//Server->Client
constexpr int SC_MOVE_PLAYER			= 1;
constexpr int SC_ROTATER_PLAYER		= 2;
constexpr int SC_SPAWN_PLAYER		= 3;
constexpr int SC_EVENT_PLAYER			= 4;

enum PLAYER_EVENT_TYPE {
	PET_MOVE, PET_JUMP
};

#pragma pack(push,1)

//Client->server

struct Cs_packet_move_player {
	char size;
	char type;
	float x, y, z;
	float direction;
	float speed;
};

struct Cs_packet_rotator_player {
	char size;
	char type;
	float pitch, yaw, roll;
};

struct Cs_packet_event_player {
	char size;
	char type;
	PLAYER_EVENT_TYPE pt;
};

struct Cs_packet_change_level {
	char size;
	char type;
};

//Server->client

//Spawn Packet
struct Sc_packet_spawn_player {
	char size;
	char type;
	char id;
	float x, y, z;
	float pitch, yaw, roll;
	float hp;
	bool is_crouch;
	bool is_zoom;
};

struct Sc_packet_rotator_player {
	char size;
	char type;
	char id;
	float pitch, yaw, roll;
};

struct Sc_packet_event_player {
	char size;
	char type;
	char id;
	PLAYER_EVENT_TYPE pt;
};

struct Sc_packet_move_player {
	char size;
	char type;
	char id;
	float x, y, z;
	float direction;
	float speed;
};


#pragma pack(pop)


