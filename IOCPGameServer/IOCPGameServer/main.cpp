#pragma once
#include "IOCP.h"


CLIENT g_clients[MAX_USER];
HANDLE g_iocp;
SOCKET l_socket;
int g_user_id = 0;
int g_user_cnt = 0;
//float chax = 0.f;
//float chay = 270.f;
//float Lpit = 0.f;
//float Lyaw = 0.f;
//float Lrol = 0.f;
//float Rpit = 0.f;
//float Ryaw = 0.f;
//float Rrol = 0.f;

ROOM g_room[MAX_ROOM];
int g_now_room = 0;

void send_packet(int user_id, void* p)
{
	char* buf = reinterpret_cast<char*>(p);

	CLIENT& u = g_clients[user_id];

	EXOVER* exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);
	WSASend(u.m_s, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}

void send_enter_ok_packet(int user_id, int o_id, int room, int ctype)
{
	sc_packet_enter_ok p;
	p.id = user_id;
	p.size = sizeof(p);
	p.ctype = ctype;
	p.room = room;
	p.type = S2C_ENTER_OK;
	strcpy_s(p.name, g_clients[user_id].m_name);
	send_packet(user_id, &p);
}

void send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.room = g_now_room;
	p.type = S2C_ENTER;
	strcpy_s(p.name, g_clients[o_id].m_name);
	send_packet(user_id, &p);
}

void enter_game(int user_id, char name[], bool host)
{
	CLIENT& g_c = g_clients[user_id];

	g_c.m_cl.lock();
	int assign_room = NONE;
	int assign_ctype = NONE;
	if (true == host)
	{
		for (int i = 0; i < MAX_ROOM; ++i)
		{
			if (false == g_room[i].full && NONE == g_room[i].p1)
			{
				g_room[i].p1 = user_id;
				assign_room = i;
				assign_ctype = BODY;
				//cout << assign_room << "방 개설" << endl;
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < MAX_ROOM; ++i)
		{
			if (NONE == g_room[i].p2)
			{
				g_room[i].p2 = user_id;
				assign_room = i;
				assign_ctype = ARMR;
				break;
			}
			else if (NONE == g_room[i].p3)
			{
				g_room[i].p3 = user_id;
				assign_room = i;
				assign_ctype = ARML;
				g_room[i].full = true;
				break;
			}
		}
		//cout << assign_room << "방 참여" << endl;
	}
	
	strcpy_s(g_c.m_name, name);
	g_c.m_name[MAX_ID_LEN] = NULL;
	send_enter_ok_packet(user_id, user_id, assign_room, assign_ctype);
	g_clients[user_id].m_status = ST_ACTIVE;
	g_c.m_cl.unlock();

}

void send_connect(int user_id, int room)
{
	sc_packet_connect p;
	p.size = sizeof(p);
	p.type = S2C_CONNECT;
	p.isfull = g_room[room].full;
	send_packet(user_id, &p);
}

void send_characterinfo(int user_id, int o_id, int room)
{
	sc_packet_character_info p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_CHARACTERINFO;
	p.rp = g_room[room].Rpit;
	p.ry = g_room[room].Ryaw;
	p.rr = g_room[room].Rrol;
	p.lp = g_room[room].Lpit;
	p.ly = g_room[room].Lyaw;
	p.lr = g_room[room].Lrol;
	p.cx = g_room[room].chax;
	p.cy = g_room[room].chay;
	send_packet(user_id, &p);
}

void do_rotator_and_move(int user_id, float rp, float ry, float rr, float lp, float ly, float lr, float cx, float cy, float cz, int type, int room)
{
	CLIENT& g_c = g_clients[user_id];

	if (type == BODY)
	{
		g_room[room].chax = cx;
		g_room[room].chay = cy;
	}
	if (type == ARMR)
	{
		g_room[room].Rpit += rp;
		g_room[room].Ryaw += ry;
		g_room[room].Rrol += rr;
	}
	if (type == ARML)
	{
		g_room[room].Lpit += lp;
		g_room[room].Lyaw += ly;
		g_room[room].Lrol += lr;
	}

	send_characterinfo(user_id, user_id, room);

	for (int i = 0; i < MAX_USER; i++)
	{
		if (user_id == i) continue;
		if (ST_ACTIVE == g_clients[i].m_status)
		{
			send_characterinfo(user_id, i, room);
		}
	}

}

void send_leave_packet(int user_id)
{
	sc_packet_leave p;
	p.id = user_id;
	p.size = sizeof(p);
	p.type = S2C_LEAVE;
	send_packet(user_id, &p);
}

void disconnect(int user_id, int room)
{
	//
	//send_leave_packet(g_room[room].p1);
	//g_clients[g_room[room].p1].m_cl.lock();
	//g_clients[g_room[room].p1].m_status = ST_ALLOC;
	//closesocket(g_clients[g_room[room].p1].m_s);
	//g_clients[g_room[room].p1].m_status = ST_FREE;
	//g_clients[g_room[room].p1].m_cl.unlock();

	//send_leave_packet(g_room[room].p2);
	//g_clients[g_room[room].p2].m_cl.lock();
	//g_clients[g_room[room].p2].m_status = ST_ALLOC;
	//closesocket(g_clients[g_room[room].p2].m_s);
	//g_clients[g_room[room].p2].m_status = ST_FREE;
	//g_clients[g_room[room].p2].m_cl.unlock();

	//send_leave_packet(g_room[room].p3);
	//g_clients[g_room[room].p3].m_cl.lock();
	//g_clients[g_room[room].p3].m_status = ST_ALLOC;
	//closesocket(g_clients[g_room[room].p3].m_s);
	//g_clients[g_room[room].p3].m_status = ST_FREE;
	//g_clients[g_room[room].p3].m_cl.unlock();

	//g_room[room].p1 = NONE;
	//g_room[room].p2 = NONE;
	//g_room[room].p3 = NONE;
	//g_room[room].full = false;
	//g_room[room].chax = 0.f;
	//g_room[room].chay = 270.f;
	//g_room[room].Lpit = 0.f;
	//g_room[room].Lyaw = 0.f;
	//g_room[room].Lrol = 0.f;
	//g_room[room].Rpit = 0.f;
	//g_room[room].Ryaw = 0.f;
	//g_room[room].Rrol = 0.f;
}


void process_packet(int user_id, char* buf)
{
	switch (buf[1]) {
	case C2S_CONNECT:
	{
		cs_packet_connect* packet = reinterpret_cast<cs_packet_connect*>(buf);
		send_connect(user_id, packet->room);
	}
	break;
	case C2S_ENTER:
	{
		cs_packet_enter* packet = reinterpret_cast<cs_packet_enter*>(buf);
		//cout << user_id << "g_clients[user_id].m_id : " << packet->name << endl;
		//cout << g_clients[user_id].m_id << "번 클라이언트의 msg : C2S_ENTER" << endl;
		enter_game(user_id, packet->name, packet->host);

	}
	break;

	

	case C2S_INGAME:
	{
		cs_packet_in_game* packet = reinterpret_cast<cs_packet_in_game*>(buf);
		do_rotator_and_move(user_id, packet->rp, packet->ry, packet->rr,
									packet->lp, packet->ly, packet->lr,
									packet->cx, packet->cy, packet->cz, packet->ctype, packet->room);

		
	}
		break;


	case C2S_LEAVE:
	{
		cs_packet_leave* packet = reinterpret_cast<cs_packet_leave*>(buf);
		disconnect(user_id, packet->room);
	}
	default:
		cout << "Unknown Packet Type Error!\n";
		DebugBreak();
		exit(-1);
	}
}


void recv_packet_construct(int user_id, int io_byte)
{
	CLIENT& cu = g_clients[user_id];
	EXOVER& r_o = cu.m_recv_over;

	int rest_byte = io_byte;
	char* p = r_o.io_buf;
	int packet_size = 0;
	if (0 != cu.m_prev_size) packet_size = cu.m_packe_buf[0];
	while (rest_byte > 0) {
		if (0 == packet_size) packet_size = *p;
		if (packet_size <= rest_byte + cu.m_prev_size)
		{
			memcpy(cu.m_packe_buf + cu.m_prev_size, p, packet_size - cu.m_prev_size);
			p += packet_size - cu.m_prev_size;
			rest_byte -= packet_size - cu.m_prev_size;
			packet_size = 0;
			process_packet(user_id, cu.m_packe_buf);
			cu.m_prev_size = 0;
		}
		else {
			memcpy(cu.m_packe_buf + cu.m_prev_size, p, rest_byte);
			cu.m_prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void initialize_clients()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		g_clients[i].m_id = i;
		g_clients[i].m_status = ST_FREE;
		g_clients[i].m_isevent = false;
	}
}


void worker_thread()
{
	while (true)
	{
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);
		//cout << user_id << " login" << endl;
		CLIENT& cl = g_clients[user_id];
		switch (exover->op)
		{
		case OP_RECV:
			if (0 == io_byte)
			{
				for (int i = 0; i < MAX_ROOM; ++i)
				{
					if(g_room[i].p1 == user_id || g_room[i].p2 == user_id || g_room[i].p3 == user_id)
						disconnect(user_id, i);
				}
				
			}
			else
			{
				recv_packet_construct(user_id, io_byte);
				ZeroMemory(&cl.m_recv_over.over, sizeof(cl.m_recv_over.over));
				DWORD flags = 0;
				WSARecv(cl.m_s, &cl.m_recv_over.wsabuf, 1, NULL, &flags, &cl.m_recv_over.over, NULL);

				
			}
			break;

		case OP_SEND:
			if (0 == io_byte)
			{
				for (int i = 0; i < MAX_ROOM; ++i)
				{
					if (g_room[i].p1 == user_id || g_room[i].p2 == user_id || g_room[i].p3 == user_id)
						disconnect(user_id, i);
				}
			}
			delete exover;
			break;

		case OP_ACCEPT:
		{
			int user_id = -1;
			for (int i = 0; i < MAX_USER; ++i)
			{
				lock_guard<mutex> gl{ g_clients[i].m_cl };
				if (ST_FREE == g_clients[i].m_status)
				{
					g_clients[i].m_status = ST_ALLOC;
					user_id = i;
					break;
				}
				g_user_cnt += 1;
			}


			SOCKET c_socket = exover->c_socket;
			if (-1 == user_id)
				closesocket(c_socket);
			else {
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), g_iocp, user_id, 0);
				CLIENT& nc = g_clients[user_id];
				//cout << "nc : " << user_id << endl;
				nc.m_prev_size = 0;
				nc.m_recv_over.op = OP_RECV;
				ZeroMemory(&nc.m_recv_over.over, sizeof(nc.m_recv_over.over));
				nc.m_recv_over.wsabuf.buf = nc.m_recv_over.io_buf;
				nc.m_recv_over.wsabuf.len = MAX_BUF_SIZE;
				nc.m_s = c_socket;
				DWORD flags = 0;
				WSARecv(c_socket, &nc.m_recv_over.wsabuf, 1, NULL, &flags, &nc.m_recv_over.over, NULL);
			}
			c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exover->c_socket = c_socket;
			ZeroMemory(&exover->over, sizeof(exover->over));
			AcceptEx(l_socket, c_socket, exover->io_buf, NULL,
				sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
		}
		break;
		}
	}
}


int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	l_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVER_PORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, reinterpret_cast<sockaddr *>(&s_address), sizeof(s_address));

	listen(l_socket, SOMAXCONN);

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	initialize_clients();

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(l_socket), g_iocp, 999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER accept_over;
	ZeroMemory(&accept_over.over, sizeof(accept_over.over));
	accept_over.op = OP_ACCEPT;
	accept_over.c_socket = c_socket;
	AcceptEx(l_socket, c_socket, accept_over.io_buf, NULL, sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, NULL, &accept_over.over);

	vector <thread> worker_threads;
	for (int i = 0; i < 4; ++i) worker_threads.emplace_back(worker_thread);
	for (auto& th : worker_threads) th.join();

}