#pragma once

constexpr int MAX_ID_LEN = 50;
constexpr int MAX_STR_LEN = 255;
constexpr int MAX_BUFFER = 1024;

constexpr auto MAX_BUF_SIZE = 1024;
constexpr auto MAX_USER = 1000;
constexpr auto MAX_ROOM = 10;
#define SERVER_PORT		9000

#define C2S_MATCHING		1
#define C2S_INGAME			4
#define C2S_ENTER			5
#define C2S_CONNECT		6
#define C2S_LEAVE			7

#define S2C_MATCHING_OK		1
#define S2C_MATCHING_FAIL		2
#define S2C_MATCHING_WAIT		3
#define S2C_PICK						3
#define S2C_READY					4
#define S2C_START					5
#define S2C_ENTER					7
#define S2C_ENTER_OK				8
#define S2C_CONNECT				9
#define S2C_CHARACTERINFO		10
#define S2C_LEAVE					11

enum PLAYER_POSITION_TYPE
{
	NONE = -1, BODY, ARMR, ARML
};

//struct EXOVER {
//	WSAOVERLAPPED 	over;
//	ENUMOP			op;
//	char			io_buf[MAX_BUF_SIZE];
//	union {
//		WSABUF			wsabuf;
//		SOCKET			c_socket;
//	};
//};

#pragma pack(push ,1)

//server -> client
struct sc_packet_matching_ok		//lobby
{
	char size;
	char type;
	int playercnt;
	char name;
};

struct sc_packet_matching_wait		//lobby
{
	char size;
	char type;
	int playercnt;
	char name;
};

struct sc_packet_pick_position_fail		//lobby
{
	char size;
	char type;
	char name;
};

struct sc_packet_pick_position //room
{
	char size;
	char type;
	int id;
	PLAYER_POSITION_TYPE ppt;
};

struct sc_packet_ready_postion		//room
{
	char size;
	char type;
	int id;
};

struct sc_packet_character_info		//우리 게임에서의 카메라, 팔 움직임
{
	char size;
	char type;
	int id;
	float rp, ry, rr;
	float lp, ly, lr;
	float cx, cy;
};

struct sc_packet_enter	
{
	char size;
	char type;
	int id;
	int room;
	char name[MAX_ID_LEN];	
};

struct sc_packet_enter_ok	
{
	char size;
	char type;
	int id;
	int ctype;
	int room;
	char name[MAX_ID_LEN];
};

struct sc_packet_leave {
	char size;
	char type;
	int id;
};
//server -> client

//client -> server

struct cs_packet_leave {
	char size;
	char type;
	int room;
};

struct cs_packet_enter
{
	char size;
	char type;
	bool host;
	char name[MAX_ID_LEN];
};

struct sc_packet_connect
{
	char size;
	char type;
	bool isfull;
};

struct cs_packet_connect
{
	char size;
	char type;
	int room;
};

struct cs_packet_in_game	//in game	//우리 게임에서의 카메라, 팔 움직임
{
	char size;
	char type;
	int id;
	int ctype;
	int room;
	float rp, ry,rr;
	float lp, ly, lr;
	float cx, cy, cz;
};


#pragma pack (pop)