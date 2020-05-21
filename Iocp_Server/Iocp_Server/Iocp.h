#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib,"ws2_32")

#define REVIVE_TIME 7s

#include<WinSock2.h>
#include<thread>
#include<vector>
#include<mutex>
#include<chrono>
#include<queue>
#include <sqlext.h>  
#include<iostream>
#include"protocol.h"
#include"event_type.h"


using namespace std::chrono;


struct OVER_EX {
	WSAOVERLAPPED overlapped;
	WSABUF wsa_buffer;
	char buffer[MAX_BUFFER];  //커널이 사용할 버퍼
	EVENT_TYPE ev; //recv or send
	char id;
};

class SOCKETINFO {
private:
public:
	OVER_EX m_over_ex;
	SOCKET m_socket;
	char m_recv_packet_buf[MAX_BUFFER];  //패킷이 조립될 버퍼
	int m_prev_size = 0;
	bool m_used;
	bool m_mainLevel;
	SOCKETINFO() {
		ZeroMemory(&m_over_ex.overlapped, sizeof(m_over_ex.overlapped));
		m_over_ex.wsa_buffer.buf = m_over_ex.buffer;
		m_over_ex.wsa_buffer.len = MAX_BUFFER;
		m_over_ex.ev = EV_RECV;
		m_used = false;
		m_mainLevel = false;
		m_socket = 0;
	}
};

struct ClientInfo : public SOCKETINFO{
public:
	float m_x, m_y, m_z;
	float m_pitch, m_yaw, m_roll;
	float m_speed, m_direction;
	float m_hp;
	bool m_is_swap;

	std::mutex player_l;
	PLAYER_EVENT_TYPE m_event_player;
public:
	ClientInfo();
};

struct NpcInfo : public SOCKETINFO {
	float m_x, m_y, m_z;
	float m_pitch, m_yaw, m_roll;
	float m_speed, m_direction;
	NpcInfo();

};

struct EVENT_ST {
	char id;
	EVENT_TYPE type; //이벤트 타입
	high_resolution_clock::time_point start_time;

	constexpr bool operator<(const EVENT_ST& lhs)const {
		return start_time > lhs.start_time;
	}
};

class Iocp{
private:
public:
	 HANDLE iocp;
	Iocp();
	~Iocp();

	void PacketProcess(const char,const char*);
	void do_recv(char id);
	bool IsValid(char id);
	void PlayerEventProcess(const char key,const PLAYER_EVENT_TYPE type);
	void Send_packet(const char, const void*);
	void Send_SpawnPlayer_packet(const char id,const char new_id);
	void Send_EventPlayer_packet(const char id, const char event_id);
	void Send_RotaterPlayer_packet(const char id, const char rotater_id);
	void Send_PlayerMove_packet(const char id, const char move_id);
};

void Worker_thread();
void Accept_thread();
void Timer_thread();

extern ClientInfo g_clients[MAX_USER];

extern std::priority_queue<EVENT_ST> g_timer_queue;
extern std::mutex timer_l;