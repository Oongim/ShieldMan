#pragma once

constexpr int MAX_ID_LEN = 50;
constexpr int MAX_STR_LEN = 255;
constexpr int MAX_BUFFER = 1024;

constexpr auto MAX_BUF_SIZE = 1024;
constexpr auto MAX_USER = 1000;

#define SERVER_PORT		9000

#define C2S_MATCHING		1
#define C2S_PICK				2
#define C2S_READY			3
#define C2S_MOVE			4
#define C2S_ROTATOR		5
#define C2S_ENTER			6
#define C2S_CONNECT		7

#define S2C_MATCHING_OK		1
#define S2C_MATCHING_FAIL		2
#define S2C_MATCHING_WAIT		3
#define S2C_PICK						3
#define S2C_READY					4
#define S2C_START					5
#define S2C_MOVE					6
#define S2C_ROTATOR				7
#define S2C_ENTER					8
#define S2C_ENTER_OK				9
#define S2C_CONNECT				10
enum PLAYER_POSITION_TYPE
{
	PPT_NONE = 0, PPT_ARMR, PPT_BODY, PPT_ARML
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

struct sc_packet_start		//room
{

};

struct sc_packet_move
{
	char size;
	char type;
	int id;
	float x, y, z;	
};

struct sc_packet_move_mouse			//우리 게임에서의 카메라, 팔 움직임
{
	char size;
	char type;
	int id;
	float yaw, pitch, roll;
};

struct sc_packet_enter	
{
	char size;
	char type;
	int id;
	char name[MAX_ID_LEN];	
};

struct sc_packet_enter_ok	
{
	char size;
	char type;
	int id;
	char name[MAX_ID_LEN];
};

//server -> client

//client -> server
struct cs_packet_matching_encounter	//lobby
{
	char size;
	char type;
	int id;
};

struct cs_packet_matching_wait			//lobby
{
	char size;
	char type;
	char name[MAX_ID_LEN];
};

struct cs_packet_pick_position		//room
{
	char size;
	char type;
	char name[MAX_ID_LEN];
	PLAYER_POSITION_TYPE ppt;
};

struct cs_packet_ready_postion		//room
{
	char size;
	char type;
	char name[MAX_ID_LEN];
};

struct cs_packet_move_player		//in game
{
	char size;
	char type;
	int id;
	float x, y, z;								//내 현재 위치?
};

struct cs_packet_move_mouse		//in game	//우리 게임에서의 카메라, 팔 움직임
{
	char size;
	char type;
	int id;
	float yaw, pitch, roll;
};

struct cs_packet_enter		//우리 게임에서의 카메라, 팔 움직임
{
	char size;
	char type;
	char name[MAX_ID_LEN];
};

struct sc_packet_connect
{
	char size;
	char type;
	bool isEvent;
};

struct cs_packet_connect
{
	char size;
	char type;
};
#pragma pack (pop)