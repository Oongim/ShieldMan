#include"Iocp.h"

ClientInfo g_clients[MAX_USER];

ClientInfo::ClientInfo()
	:m_x(0.f), m_y(0.f), m_z(0.f),
	m_pitch(0.f), m_roll(0.f), m_yaw(0.f),
	m_speed(0.f), m_direction(0.f), m_hp(100.0f)
{

}
NpcInfo::NpcInfo()
	: m_x(0.f), m_y(0.f), m_z(0.f),
	m_pitch(0.f), m_roll(0.f), m_yaw(0.f),
	m_speed(0.f), m_direction(0.f)
{}
Iocp::Iocp() {}
Iocp::~Iocp() {}


/*******************************
Packet Process
******************************/
void Iocp::do_recv(char id) {

	DWORD flags = 0;

	//ZeroMemory(&g_clients[id].m_over_ex.overlapped, sizeof(g_clients[id].m_over_ex.overlapped));

	if (WSARecv(g_clients[id].m_socket, &g_clients[id].m_over_ex.wsa_buffer, 1, NULL, &flags, &(g_clients[id].m_over_ex.overlapped), 0)) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			std::cout << "¿À·ù:" << WSAGetLastError();
			std::cout << "Error - IO pending Failure recv" << std::endl;
			while (true);
		}
	}
	else {
		//	std::cout << "Non Overlapped Recv return" << std::endl;
		//	while (true);
	}

}
bool Iocp::IsValid(char id) {
	return g_clients[id].m_used && g_clients[id].m_mainLevel;
}
void Iocp::PacketProcess(const char key, const char* packet) {

	char pkType = packet[1];

	switch (pkType) {
	case CS_MOVE_PLAYER: {
		const Cs_packet_move_player* movePlayer =
			reinterpret_cast<const Cs_packet_move_player*>(packet);

		//Set Location
		g_clients[key].m_x = movePlayer->x;
		g_clients[key].m_y = movePlayer->y;
		g_clients[key].m_z = movePlayer->z;

		//Set Velocity
		g_clients[key].m_speed = movePlayer->speed;
		g_clients[key].m_direction = movePlayer->direction;

		//printf("[%d] X:%f Y:%f Z:%f \n",key,g_clients[key].m_x, g_clients[key].m_y, g_clients[key].m_z);
		for (int i = 0; i < MAX_USER; ++i) {
			if (i == key)continue;
			if (IsValid(i) == false)continue;

			Send_PlayerMove_packet(i, key);
		}
		break;
	}
	case CS_ROTATER_PLAYER: {
		const Cs_packet_rotator_player* rotatorPlayer =
			reinterpret_cast<const Cs_packet_rotator_player*>(packet);

		g_clients[key].m_yaw = rotatorPlayer->yaw;
		g_clients[key].m_pitch = rotatorPlayer->pitch;
		g_clients[key].m_roll = rotatorPlayer->roll;


		//std::cout << " yaw: " << g_clients[key].clientinfo.m_yaw;
		//std::cout << " pitch: " << g_clients[key].clientinfo.m_pitch;
		//std::cout << " roll: " << g_clients[key].clientinfo.m_roll << std::endl;

		for (int i = 0; i < MAX_USER; ++i) {
			if (i == key)continue;
			if (IsValid(i) == false)continue;

			Send_RotaterPlayer_packet(i, key);

		}

		break;
	}
	case CS_EVENT_PLAYER: {
		const Cs_packet_event_player* eventPlayer =
			reinterpret_cast<const Cs_packet_event_player*>(packet);
		PlayerEventProcess(key,eventPlayer->pt);
		break;
	}
	case CS_CHANGE_LEVEL: {
		const Cs_packet_change_level* changeLevel =
			reinterpret_cast<const Cs_packet_change_level*>(packet);
		char new_id = key;
		g_clients[new_id].m_mainLevel = true;

		for (int i = 0; i < MAX_USER; ++i) {
			if (IsValid(i) == false)continue;
			if (i == new_id)continue;

			Send_SpawnPlayer_packet(i, new_id);
		}
		for (int i = 0; i < MAX_USER; ++i) {
			if (IsValid(i) == false)continue;
			if (i == new_id)continue;
			Send_SpawnPlayer_packet(new_id, i);
		}

		break;
	}
	default:
		std::cout << "PacketProcess:: An undefined packet received" << std::endl;
		while (1);
		break;
	}
}
void Iocp::PlayerEventProcess(const char key,const PLAYER_EVENT_TYPE type) {

	switch (type) {
	case PET_MOVE: {
		g_clients[key].m_event_player = PET_MOVE;
	//	std::cout << "Send Shot Packet" << std::endl;
		break;
	}
	case PET_JUMP: {
		g_clients[key].m_event_player = PET_JUMP;
	//	std::cout << "Send Flash Packet" << std::endl;
		break;
	}

	default:
		std::cout << "Not defined EVENT_TYPE" << std::endl;
		while (1);
		break;
	}
	for (int i = 0; i < MAX_USER; ++i) {
		if (i == key)continue;
		if (IsValid(i) == false)continue;
		Send_EventPlayer_packet(i, key);
	}

}

/*******************************
Send Process
******************************/
void Iocp::Send_packet(const char id, const void* packet) {

	const char* p = reinterpret_cast<const char*>(packet);
	OVER_EX* ov = nullptr;
	ov = new OVER_EX;

	if (ov == nullptr) {
		std::cout << "Send_packet::ov==nullptr" << std::endl;
		while (1);
	}
	else {

		ov->wsa_buffer.len = p[0];
		ov->wsa_buffer.buf = ov->buffer;
		ov->ev=EV_SEND;
		memcpy(ov->buffer, p, p[0]);
		ZeroMemory(&ov->overlapped, sizeof(ov->overlapped));

		if (WSASend(g_clients[id].m_socket, &ov->wsa_buffer, 1, 0, 0, &ov->overlapped, NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				std::cout<<"["<<(int)id<<"]"<< "Error I0 Pending Failure send::"<<GetLastError() << std::endl;

			}
		}
		else {

				/*std::cout << "non overlapped send " << std::endl;
					while (1);*/
		}
	}
}
void Iocp::Send_SpawnPlayer_packet(const char id, const char new_id) {
	Sc_packet_spawn_player packet;

	packet.id = new_id;
	packet.size = sizeof(packet);
	packet.type = SC_SPAWN_PLAYER;
	packet.x = g_clients[new_id].m_x;
	packet.y = g_clients[new_id].m_y;
	packet.z = g_clients[new_id].m_z;
	packet.pitch = g_clients[new_id].m_pitch;
	packet.yaw = g_clients[new_id].m_yaw;
	packet.roll = g_clients[new_id].m_roll;
	packet.hp = g_clients[new_id].m_hp;
	Send_packet(id, &packet);
}
void Iocp::Send_PlayerMove_packet(const char id, const char move_id) {
	Sc_packet_move_player packet;
	//Set Packet Infor
	packet.id = move_id;
	packet.type = SC_MOVE_PLAYER;
	packet.size = sizeof(packet);
	//Set Location
	packet.x = g_clients[move_id].m_x;
	packet.y = g_clients[move_id].m_y;
	packet.z = g_clients[move_id].m_z;
	//Set speed and direciton
	packet.speed = g_clients[move_id].m_speed;
	packet.direction= g_clients[move_id].m_direction;

	Send_packet(id, &packet);
}
void Iocp::Send_EventPlayer_packet(const char id, const char event_id) {
	Sc_packet_event_player packet;
	packet.id = event_id;
	packet.type = SC_EVENT_PLAYER;
	packet.pt = g_clients[event_id].m_event_player;
	packet.size=sizeof(packet);
	Send_packet(id, &packet);
}
void Iocp::Send_RotaterPlayer_packet(const char id, const char rotater_id) {
	Sc_packet_rotator_player packet;
	packet.id = rotater_id;
	packet.size = sizeof(packet);
	packet.type = SC_ROTATER_PLAYER;

	packet.pitch = g_clients[rotater_id].m_pitch;
	packet.roll = g_clients[rotater_id].m_roll;
	packet.yaw = g_clients[rotater_id].m_yaw;
	Send_packet(id, &packet);
}

