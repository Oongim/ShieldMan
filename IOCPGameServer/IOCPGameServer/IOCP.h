#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")

#include <vector>
#include <set>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <chrono>
#include <string>

#include "protocol.h"

using namespace std;
using namespace chrono;

enum C_STATUS { ST_FREE, ST_ALLOC, ST_ACTIVE };
enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT };

struct EXOVER {
	WSAOVERLAPPED 	over;
	ENUMOP			op;
	char			io_buf[MAX_BUF_SIZE];
	union {
		WSABUF			wsabuf;
		SOCKET			c_socket;
	};
};

struct CLIENT
{
	mutex 	m_cl;
	SOCKET	m_s;
	int		m_id;
	EXOVER  m_recv_over;
	int   m_prev_size;
	char  m_packe_buf[255];
	atomic<C_STATUS> m_status;

	bool m_isevent;
	float m_x, m_y, m_z;
	float m_yaw, m_pitch, m_roll;
	float m_speed, m_direction;
	PLAYER_POSITION_TYPE m_type;
	char m_name[MAX_ID_LEN + 1];
};