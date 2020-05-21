#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib,"ws2_32")
#include <exception>
#include <functional>
#include <iostream>
#include "Windows/MinWindows.h"
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//#define MAX_BUFFER 255
//constexpr int SERVER_PORT = 9500;

#pragma pack(1)
struct Position {
public:
	BYTE size, type, id, key;
	float x, y, z;
	Position() {};
	Position(BYTE size, BYTE type, BYTE id, BYTE key, float x, float y, float z) :size(size), type(type),
		id(id), key(key), x(x), y(y), z(z) {};
};

struct Rocation {
public:
	float yaw, pitch, roll;
};
#pragma pack()


enum INPUT_KEY { RIGHT_KEY = 1, LEFT_KEY = 2, UP_KEY = 3, DOWN_KEY = 4, NONE_KEY = 5 };
enum PACKET_TYPE { PACKET_PLAYER_MOVE = 1, PACKET_PLAYER_SPAWN = 2, PACKET_PLAYER_OUT = 3 };
