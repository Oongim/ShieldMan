#pragma once
#include "IOCP.h"


CLIENT g_clients[MAX_USER];
HANDLE g_iocp;
SOCKET l_socket;
int g_user_id = 0;
int g_user_cnt = 0;
bool g_isEvent = false;
float chax = 0.f;
float chay = 0.f;
float Lpit = 0.f;
float Lyaw = 0.f;
float Lrol = 0.f;
float Rpit = 0.f;
float Ryaw = 0.f;
float Rrol = 0.f;

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

void send_leave_packet(int user_id, int o_id)
{
	/*sc_packet_leave p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LEAVE;

	g_clients[user_id].m_cl.lock();
	g_clients[user_id].m_viewlist.erase(o_id);
	g_clients[user_id].m_cl.unlock();

	send_packet(user_id, &p);*/
}

void send_move_packet(int user_id, int mover)
{
	//sc_packet_move p;
	//p.id = mover;
	//p.size = sizeof(p);
	//p.type = S2C_MOVE;
	//p.x = g_clients[mover].x;
	//p.y = g_clients[mover].y;
	//p.move_time = g_clients[mover].m_movetime;
	//send_packet(user_id, &p);
}

void matching_game(int user_id, char id[])
{
	//g_clients[user_id].m_cl.lock();
	//strcpy_s(g_clients[user_id].m_name, name);
	//g_clients[user_id].m_name[MAX_ID_LEN] = NULL;
	//send_login_ok_packet(user_id);
	//g_clients[user_id].m_status = ST_ACTIVE;
	//g_clients[user_id].m_cl.unlock();

	//for (int i = 0; i < MAX_USER; i++)
	//{
	//	if (user_id == i) continue;
	//	if (user_id != i)
	//	{

	//		send_enter_packet(user_id, i);
	//		send_enter_packet(i, user_id);
	//	}
	//}


}

void send_enter_ok_packet(int user_id, int o_id)
{
	sc_packet_enter_ok p;
	p.id = user_id;
	//cout << user_id << endl;
	p.size = sizeof(p);
	p.type = S2C_ENTER_OK;
	strcpy_s(p.name, g_clients[user_id].m_name);
	send_packet(user_id, &p);
}

void send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_ENTER;
	strcpy_s(p.name, g_clients[o_id].m_name);
	send_packet(user_id, &p);
}

void enter_game(int user_id, char name[])
{
	CLIENT& g_c = g_clients[user_id];

	g_c.m_cl.lock();
	strcpy_s(g_c.m_name, name);
	g_c.m_name[MAX_ID_LEN] = NULL;
	send_enter_ok_packet(user_id, user_id);
	g_clients[user_id].m_status = ST_ACTIVE;
	g_c.m_cl.unlock();

	for (int i = 0; i < MAX_USER; i++)
	{
		if (user_id == i) continue;

		if (ST_ACTIVE == g_clients[i].m_status)
		{
				send_enter_packet(user_id, i);
				if (i != user_id)
				{
					send_enter_packet(i, user_id);
				}
				g_clients[i].m_isevent = true;
				cout << user_id << "번 클라이언트가 " << i << "번 클라이언트에게 참여를 알림" << endl;
		}
	}
	g_c.m_cl.lock();
	g_c.m_cl.unlock();

	//cout << user_id << " enter_game " << endl;
}

void send_pick_position_packet(int user_id, int o_id)
{
	sc_packet_pick_position p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_PICK;
	p.ppt = g_clients[o_id].m_type;
	send_packet(user_id, &p);
}

void pick_position(int user_id, PLAYER_POSITION_TYPE ppt)
{
	CLIENT& g_c = g_clients[user_id];
	bool picksuccess = true;
	g_c.m_cl.lock();
	for (int i = 0; i < MAX_USER; i++)
	{
		if (user_id == i) continue;
		if (user_id != i)
		{
			if (ST_ACTIVE == g_clients[i].m_status)
			{
				if (g_clients[i].m_type != ppt)
					continue;
				else
				{
					picksuccess = false;
					break;
				}
			}
		}

	}
	if (!picksuccess)
	{
		g_c.m_type = PPT_NONE;
		cout << user_id << "번 클라이언트는 해당 선택을 거절당함" << endl;
		send_pick_position_packet(user_id, user_id);
	}
	else
	{
		g_c.m_type = ppt;

		for (int i = 0; i < MAX_USER; i++)
		{
			if (user_id == i) continue;
			if (user_id != i)
			{
				if (ST_ACTIVE == g_clients[i].m_status)
				{
					send_pick_position_packet(user_id, i);
					send_pick_position_packet(i, user_id);
					if (g_c.m_type == PPT_ARML)
						cout << user_id << "번 클라이언트가 " << i << "번 에게" << " 왼팔 선택을 알림" << endl;
					else if (g_c.m_type == PPT_BODY)
						cout << user_id << "번 클라이언트가 " << i << "번 에게" << " 몸통 선택을 알림" << endl;
					else if (g_c.m_type == PPT_ARMR)
						cout << user_id << "번 클라이언트가 " << i << "번 에게" << " 오른팔 선택을 알림" << endl;
				}
			}
		}
	}
	g_c.m_cl.unlock();

}

void send_ready_position_packet(int user_id, int o_id)
{
	sc_packet_ready_postion p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_READY;
	send_packet(user_id, &p);
}

void ready_position(int user_id)
{
	CLIENT& g_c = g_clients[user_id];

	g_c.m_cl.lock();
	if (g_c.m_type != PPT_NONE)
	{
		send_ready_position_packet(user_id, user_id);
		for (int i = 0; i < MAX_USER; i++)
		{
			if (user_id == i) continue;
			if (ST_ACTIVE == g_clients[i].m_status)
			{
				send_ready_position_packet(user_id, i);
				send_ready_position_packet(i, user_id);
				cout << user_id << "번 클라이언트가 " << i << "번 에게" << "준비됐음을 알림" << endl;
			}
		}
	}
	else
	{
		send_ready_position_packet(user_id, user_id);
		cout << user_id << "번 클라이언트는 포지션을 선택하지 않아 준비가 불가능" << endl;
	}
	g_c.m_cl.unlock();
}

void send_connect(int user_id)
{
	sc_packet_connect p;
	p.size = sizeof(p);
	p.type = S2C_CONNECT;
	p.isEvent = g_clients[user_id].m_isevent;
	g_clients[user_id].m_isevent = false;
	send_packet(user_id, &p);
}

void send_characterinfo(int user_id, int o_id)
{
	sc_packet_character_info p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_CHARACTERINFO;
	p.rp = Rpit;
	p.ry = Ryaw;
	p.rr = Rrol;
	p.lp = Lpit;
	p.ly = Lyaw;
	p.lr = Lrol;
	p.cx = chax;
	p.cy = chay;
	send_packet(user_id, &p);
}

void send_ingame(int user_id, int o_id, float x, float y, float z, float pitch, float yaw, float roll, float cx, float cy, float cz)
{
	//sc_packet_in_game p;
	//p.id = o_id;
	//p.size = sizeof(p);
	//p.type = S2C_INGAME;
	//p.x = x;
	//p.y = y;
	//p.z = z;
	//p.yaw = Ryaw;
	//p.pitch = Rpit;
	//p.roll = Rrol;
	//p.cx = chax;
	//p.cy = chay;
	//p.cz = cz;
	//send_packet(user_id, &p);
}

void do_rotator_and_move(int user_id, float rp, float ry, float rr, float lp, float ly, float lr, float cx, float cy, float cz)
{
	CLIENT& g_c = g_clients[user_id];

	if (cx != 0.f)
	{
		chax = cx;
		chay = cy;
		//cout << chax << "      " << chay << "       " << endl;
	}
	if (rp !=0.f)
	{
		Rpit += rp;
		Ryaw += ry;
		Rrol += rr;
	}
	if (lp != 0.f)
	{
		Lpit += lp;
		Lyaw += ly;
		Lrol += lr;
	}

	send_characterinfo(user_id, user_id);
	//cout << user_id << "번 클라이언트 " << "  rp : " << rp << ", ry : " << ry <<",  rr : " << rr << endl;

	for (int i = 0; i < MAX_USER; i++)
	{
		if (user_id == i) continue;
		if (ST_ACTIVE == g_clients[i].m_status)
		{
			send_characterinfo(user_id, i);

			//send_ingame(user_id, i, x, y, z, pitch, yaw, roll, cx, cy, cz);
			//if(user_id == 0)
			//if(user_id == 6 && i == 0)
			//cout << user_id << "번 클라이언트가 " << i << "번 에게 Lyaw : " << Lyaw << ", Lpit : " << Lpit << ", Lrol : " << Lrol << endl;
			//else if (user_id == 6 && i == 2)
			//	cout << user_id << "번 클라이언트가 " << i << "번 에게 Lyaw : " << Lyaw << ", Lpit : " << Lpit << ", Lrol : " << Lrol << endl;
			//cout << user_id << "번 클라이언트가 " << i << "번 에게 x : " << x << ", y : " << y << ", roll : " << z << endl;
			//if(user_id == 2 && i == 0)
				//cout << user_id << "번 클라이언트가 " << i << "번 에게 yaw : " << yaw << ", pitch : " << pitch << ", roll : " << roll << endl;
			//cout << user_id << "번 클라이언트가 " << i << "번 에게 cx : " << cx << ", cy : " << cy << ", cy : " << cz << endl;
		}
	}

}

int cnt = 0;
void process_packet(int user_id, char* buf)
{
	switch (buf[1]) {
	case C2S_CONNECT:
	{
		cs_packet_connect* packet = reinterpret_cast<cs_packet_connect*>(buf);
		send_connect(user_id);
	}
	break;
	case C2S_ENTER:
	{
		cs_packet_enter* packet = reinterpret_cast<cs_packet_enter*>(buf);
		//cout << user_id << "g_clients[user_id].m_id : " << packet->name << endl;
		cout << g_clients[user_id].m_id << "번 클라이언트의 msg : C2S_ENTER" << endl;
		++cnt;
			enter_game(user_id, packet->name);


	}
	break;
	case C2S_MATCHING:
	{
		//cs_packet_matching* packet = reinterpret_cast<cs_packet_matching*>(buf);
		//matching_game(user_id, packet->id);
		//enter_game(user_id, packet->name);
	}
	break;
	case C2S_PICK:
	{
		cs_packet_pick_position* packet = reinterpret_cast<cs_packet_pick_position*>(buf);
		cout << g_clients[user_id].m_id << "번 클라이언트의 msg : C2S_PICK" << endl;
		pick_position(user_id, packet->ppt);
		//do_move(user_id, packet->direction);
	}
	break;

	case C2S_READY:
	{
		cs_packet_ready_postion* packet = reinterpret_cast<cs_packet_ready_postion*>(buf);
		cout << g_clients[user_id].m_id << "번 클라이언트의 msg : C2S_READY" << endl;
		ready_position(user_id);
		//do_move(user_id, packet->direction);
	}
	break;

	case C2S_INGAME:
	{
		cs_packet_in_game* packet = reinterpret_cast<cs_packet_in_game*>(buf);
		do_rotator_and_move(packet->id, packet->rp, packet->ry, packet->rr,
									packet->lp, packet->ly, packet->lr,
									packet->cx, packet->cy, packet->cz);

		
	}
		break;

	default:
		cout << "Unknown Packet Type Error!\n";
		DebugBreak();
		exit(-1);
	}
}

void disconnect(int user_id)
{
	/*send_leave_packet(user_id, user_id);
	g_clients[user_id].m_cl.lock();
	g_clients[user_id].m_status = ST_ALLOC;
	closesocket(g_clients[user_id].m_s);
	for (auto& cl : g_clients)
	{
		if (user_id == cl.m_id) continue;

		if (ST_ACTIVE == cl.m_status)
		{
			send_leave_packet(cl.m_id, user_id);
		}
	}
	g_clients[user_id].m_status = ST_FREE;
	g_clients[user_id].m_cl.unlock();*/
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
				disconnect(user_id);
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
				disconnect(user_id);
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