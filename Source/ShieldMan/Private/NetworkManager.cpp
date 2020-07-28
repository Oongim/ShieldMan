// Fill out your copyright notice in the Description page of Project Settings.

#include "NetWorkManager.h"
// Sets default values for this component's properties
UNetworkManager::UNetworkManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UNetworkManager::BeginPlay()
{
	
	//Super::BeginPlay();
	//if (ConnectServer("127.0.0.1"))		//ConnectServer --> GetIp + 서버 연결 작업
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UNetworkManager BeginPlay"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("UNetworkManager ConnectServer Error"));
	//}
	// ...

}

bool UNetworkManager::ConnectServer(FString IP)
{
	//GetIp(TCHAR_TO_ANSI(*IP));
	GetIp("127.0.0.1");
	//UE_LOG(LogTemp, Warning, TEXT("UNetworkManager ConnectServer"));

	WSADATA wsa;

	int addrlen = 0;
	addrlen = sizeof(m_serverAddr);
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("WSAstartup ERROR"));

		return false;
	}

	m_playerInfo.m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, NULL);
	if (m_playerInfo.m_socket == INVALID_SOCKET)
	{
		UE_LOG(LogTemp, Error, TEXT("m_playerInfo.m_socket INVALID SOCKET ERROR"));
		return false;
	}

	ZeroMemory(&m_serverAddr, sizeof(m_serverAddr));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(SERVER_PORT);

	//Get ip to Editer
	m_serverAddr.sin_addr.s_addr = inet_addr(m_ip);
	//Server Connect Error
	if (connect(m_playerInfo.m_socket, reinterpret_cast<SOCKADDR*>(&m_serverAddr), addrlen) == SOCKET_ERROR)
	{
		UKismetSystemLibrary::PrintString(this, "connect Time out Server \n");
		//	UE_LOG(LogTemp, Error, TEXT("connect ERROR"));
		return false;
	}

	m_connecting = TRUE;


	m_event[nTotalSockets] = WSACreateEvent();
	if (WSAEventSelect(m_playerInfo.m_socket, m_event[nTotalSockets], FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("WSAEventSelect WSASyncSelect ERROR"));
		return false;
	}
	else
	{
		m_playerInfo.m_wsabuf.buf = m_playerInfo.m_buffer;
		m_playerInfo.m_wsabuf.len = MAX_BUFFER;
		Send_Enter_Packet();
		//UE_LOG(LogTemp, Error, TEXT("CONNECT Success"));
		return true;
	}
}

void UNetworkManager::GetIp(const char* ip)
{
	m_ip = ip;
}

// Called every frame
void UNetworkManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	//RecvPacket();
	//RecvPacket --> ObserveEvents --> ReadPacket --> PacketProcess(이벤트 처리, 받아들이는 과정)


	//무언가 동작했을 때 
	//Send_Packet(reinterpret_cast<char*>(&m_playerInfo));


}


bool UNetworkManager::RecvPacket()
{
	return Read_Packet();
}

bool UNetworkManager::Read_Packet()
{
	bool result = false;
	DWORD iobyte = 0, flags = 0;
	ZeroMemory(&m_playerInfo.m_over, sizeof(m_playerInfo.m_over));
	int ret = WSARecv(m_playerInfo.m_socket, &m_playerInfo.m_wsabuf, 1, &iobyte, &flags, &m_playerInfo.m_over, NULL);	//데드락
	//if (ret <= 0)
	//{
	//	int err_code = WSAGetLastError();
	//	UE_LOG(LogTemp, Warning, TEXT("RECV PACKET SOCKET_ERROR %d"), err_code);
	//	//return false;
	//}

	char* ptr = m_playerInfo.m_buffer;
	int rest = iobyte;
	int packet_size = 0;
	if (0 != m_playerInfo.m_saved_packet_size)
	{
		packet_size = m_playerInfo.m_recv_packet_buf[0];
	}

	while (0 < rest)
	{
		if (0 == packet_size)
		{
			packet_size = *ptr;
		}
//		int required = packet_size - m_playerInfo.m_saved_packet_size;

		//if (required <= rest)
		if(packet_size <= rest + m_playerInfo.m_saved_packet_size)
		{

			memcpy(m_playerInfo.m_recv_packet_buf + m_playerInfo.m_saved_packet_size,
											ptr, packet_size - m_playerInfo.m_saved_packet_size);

			ptr += packet_size - m_playerInfo.m_saved_packet_size;
			rest -= packet_size - m_playerInfo.m_saved_packet_size;
			packet_size = 0;

			result = PacketProcess(m_playerInfo.m_recv_packet_buf);

			m_playerInfo.m_saved_packet_size = 0;
		}
		else
		{
			memcpy(m_playerInfo.m_recv_packet_buf + m_playerInfo.m_saved_packet_size, ptr, rest);
			rest = 0;
			m_playerInfo.m_saved_packet_size += rest;
			ptr += rest;
		}

	}
	return result;
}

bool UNetworkManager::PacketProcess(const char* packet)
{
	
	if (m_connecting)
	{

		auto t = packet[1];
		//UE_LOG(LogTemp, Warning, TEXT("packet %d"), t);
		switch (packet[1])
		{
		case S2C_ENTER:
		{
			const sc_packet_enter* my_packet = reinterpret_cast<const sc_packet_enter*>(packet);

			if (!m_OtherPlayer[0].m_connecting)
			{
				m_OtherPlayer[0].m_connecting = true;
				m_OtherPlayer[0].m_cid = my_packet->id;
				strcpy_s(m_OtherPlayer[0].m_name, my_packet->name);
				
				UE_LOG(LogTemp, Warning, TEXT("id[%d]: m_OtherPlayer[0] id : %d"), m_playerInfo.m_cid, m_OtherPlayer[0].m_cid);
			}
			else if (!m_OtherPlayer[1].m_connecting)
			{
				m_OtherPlayer[1].m_connecting = true;
				m_OtherPlayer[1].m_cid = my_packet->id;
				strcpy_s(m_OtherPlayer[1].m_name, my_packet->name);
				UE_LOG(LogTemp, Warning, TEXT("id[%d]: m_OtherPlayer[1] id : %d"), m_playerInfo.m_cid, m_OtherPlayer[1].m_cid);
			}
			//UE_LOG(LogTemp, Warning, TEXT("UNetworkManager PacketProcess S2C_ENTER"));
			break;
		}

		case S2C_ENTER_OK:
		{
			const sc_packet_enter_ok* my_packet = reinterpret_cast<const sc_packet_enter_ok*>(packet);
			
			UE_LOG(LogTemp, Warning, TEXT("id[%d]: UNetworkManager PacketProcess S2C_ENTER_OK"), m_playerInfo.m_cid);
			if (false == isConnect)
			{
				m_playerInfo.m_cid = my_packet->id;
				isConnect = true;
			}
			break;
		}

		case S2C_PICK:
		{
			const sc_packet_pick_position* packet_S2C_PICK = reinterpret_cast<const sc_packet_pick_position*>(packet);
			UE_LOG(LogTemp, Warning, TEXT("id[%d]: UNetworkManager PacketProcess S2C_PICK"), m_playerInfo.m_cid);
			
			if (packet_S2C_PICK->id == PPT_NONE)
			{
				break;
			}
			else if (m_playerInfo.m_cid == packet_S2C_PICK->id)
			{
				m_playerInfo.m_type = (PLAYER_POSITION_TYPE)packet_S2C_PICK->type;
			}
			else if (m_OtherPlayer[0].m_cid == packet_S2C_PICK->id)
			{
				m_OtherPlayer[0].m_type = (PLAYER_POSITION_TYPE)packet_S2C_PICK->type;
			}
			else if (m_OtherPlayer[1].m_cid == packet_S2C_PICK->id)
			{
				m_OtherPlayer[1].m_type = (PLAYER_POSITION_TYPE)packet_S2C_PICK->type;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("id[%d]: ERROR ID S2C_PICK"), m_playerInfo.m_cid);
			}

			break;
		}
		case S2C_READY:
		{
			const sc_packet_ready_postion* pickPlayer = reinterpret_cast<const sc_packet_ready_postion*>(packet);
			UE_LOG(LogTemp, Warning, TEXT("id[%d]: UNetworkManager PacketProcess S2C_READY"), m_playerInfo.m_cid);

			break;
		}	
		case S2C_CONNECT:
		{
			const sc_packet_connect* packet_S2C_CONNECT = reinterpret_cast<const sc_packet_connect*>(packet);
			if (packet_S2C_CONNECT->isEvent)
			{
				RecvPacket();
			}
			UE_LOG(LogTemp, Warning, TEXT("id[%d]: UNetworkManager PacketProcess S2C_CONNECT"), m_playerInfo.m_cid);

			break;
		}

		case S2C_INGAME:
		{
			const sc_packet_in_game* packet_S2C_INGAME = reinterpret_cast<const sc_packet_in_game*>(packet);
			m_recvcid = packet_S2C_INGAME->id;

			m_playerInfo.m_x = packet_S2C_INGAME->x;
			m_playerInfo.m_y = packet_S2C_INGAME->y;
			m_playerInfo.m_z = packet_S2C_INGAME->z;
			m_playerInfo.m_roll = packet_S2C_INGAME->roll;
			m_playerInfo.m_pitch = packet_S2C_INGAME->pitch;
			m_playerInfo.m_yaw = packet_S2C_INGAME->yaw;
			m_playerInfo.m_camerax = packet_S2C_INGAME->cx;
			m_playerInfo.m_cameray = packet_S2C_INGAME->cy;
			m_playerInfo.m_cameraz = packet_S2C_INGAME->cz;

			break;
		}

		//case S2C_MOVE:
		//{
		//	const sc_packet_move* packet_S2C_ROTATOR = reinterpret_cast<const sc_packet_move*>(packet);
		//	m_recvcid = packet_S2C_ROTATOR->id;
		//	//if (m_playerInfo.m_cid == packet_S2C_ROTATOR->id)
		//	//{
		//	m_playerInfo.m_x = packet_S2C_ROTATOR->x;
		//	m_playerInfo.m_y = packet_S2C_ROTATOR->y;
		//	m_playerInfo.m_z = packet_S2C_ROTATOR->z;
		//	//m_playerInfo.m_changed = true;
		//	UE_LOG(LogTemp, Warning, TEXT("x : %f      y : %f"), m_playerInfo.m_x, m_playerInfo.m_y);
		////}
		////else
		////{
		////	m_OtherPlayer[0].m_roll = packet_S2C_ROTATOR->roll;
		////	m_OtherPlayer[0].m_pitch = packet_S2C_ROTATOR->pitch;
		////	m_OtherPlayer[0].m_yaw = packet_S2C_ROTATOR->yaw;
		////	m_OtherPlayer[0].m_changed = true;
		////}
		////UE_LOG(LogTemp, Warning, TEXT("id[%d]: UNetworkManager PacketProcess S2C_ROTATOR"), m_playerInfo.m_cid);
		//	break;
		//}

		//case S2C_CAMERA:
		//{
		//	const sc_packet_move* packet_S2C_ROTATOR = reinterpret_cast<const sc_packet_move*>(packet);
		//	m_recvcid = packet_S2C_ROTATOR->id;
		//	//if (m_playerInfo.m_cid == packet_S2C_ROTATOR->id)
		//	//{
		//	m_playerInfo.m_x = packet_S2C_ROTATOR->x;
		//	m_playerInfo.m_y = packet_S2C_ROTATOR->y;
		//	m_playerInfo.m_z = packet_S2C_ROTATOR->z;
		//	//m_playerInfo.m_changed = true;
		//	UE_LOG(LogTemp, Warning, TEXT("x : %f      y : %f"), m_playerInfo.m_x, m_playerInfo.m_y);
		//	//}
		//	//else
		//	//{
		//	//	m_OtherPlayer[0].m_roll = packet_S2C_ROTATOR->roll;
		//	//	m_OtherPlayer[0].m_pitch = packet_S2C_ROTATOR->pitch;
		//	//	m_OtherPlayer[0].m_yaw = packet_S2C_ROTATOR->yaw;
		//	//	m_OtherPlayer[0].m_changed = true;
		//	//}
		//	//UE_LOG(LogTemp, Warning, TEXT("id[%d]: UNetworkManager PacketProcess S2C_ROTATOR"), m_playerInfo.m_cid);
		//	break;
		//}

		default:
			UE_LOG(LogTemp, Error, TEXT(" %d PacketProcess:: An undefined packet received"), (int)packet[1]);
			break;
		}
	}
	return true;
}

void UNetworkManager::Send_Enter_Packet()
{
	cs_packet_enter packet;

	packet.type = C2S_ENTER;
	packet.size = sizeof(packet);
	strcpy_s(packet.name, m_playerInfo.m_id);

	Send_Packet(&packet);

}

void UNetworkManager::Send_Pick_Position()
{
	cs_packet_pick_position packet;

	packet.type = C2S_PICK;
	packet.size = sizeof(packet);
	strcpy_s(packet.name, m_playerInfo.m_id);
	packet.ppt = m_playerInfo.m_type;
	Send_Packet(&packet);

}

void UNetworkManager::Send_Ready_Position()
{
	cs_packet_ready_postion packet;

	packet.type = C2S_READY;
	packet.size = sizeof(packet);
	strcpy_s(packet.name, m_playerInfo.m_id);
	Send_Packet(&packet);

}

void UNetworkManager::Send_Connect()
{
	cs_packet_connect packet;

	packet.type = C2S_CONNECT;
	packet.size = sizeof(packet);
	Send_Packet(&packet);

}

void UNetworkManager::Send_InGame(float x, float y, float z, float pitch, float yaw, float roll, float cx, float cy, float cz)
{
	cs_packet_in_game packet;

	packet.type = C2S_INGAME;
	packet.size = sizeof(packet);

	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.pitch = pitch;
	packet.yaw = yaw;
	packet.roll = roll;
	packet.cx = cx;
	packet.cy = cy;
	packet.cz = cz;

	packet.id = m_playerInfo.m_cid;
	Send_Packet(&packet);
}

void UNetworkManager::Send_MoveForward_Packet(float axisValue, FVector Position, float Speed, float Direction)
{
	if (axisValue != 0)
	{
		SetPlayerPosition(Position);
		SetPlayerVelocity(Speed, Direction);
		Send_PlayerMove_packet();
	}
}

void UNetworkManager::Send_MoveRight_Packet(float axisValue, FVector Position, float Speed, float Direction)
{

	if (axisValue != 0) {
		SetPlayerPosition(Position);
		SetPlayerVelocity(Speed, Direction);
		Send_PlayerMove_packet();
	}
}

void UNetworkManager::Send_MoveStop_Packet(FVector Position, float Speed, float Direction)
{
	SetPlayerPosition(Position);
	SetPlayerVelocity(0.0f, Direction);
	Send_PlayerMove_packet();
}

void UNetworkManager::DisConnect()
{

}

void UNetworkManager::Send_EventPlayer_Packet(UE_Player_EVENT_TYPE type)
{

	//Cs_packet_event_player packet;
	//packet.size = sizeof(packet);
	//packet.type = CS_EVENT_PLAYER;

	//switch (type) {
	//case SM_MOVE:
	//	packet.pt = PET_MOVE;
	//	break;
	//case SM_JUMP:
	//	packet.pt = PET_JUMP;
	//	break;
	//default:
	//	UE_LOG(LogTemp, Error, TEXT(" An undefined Event Player packet send"));
	//	break;
	//}
	//Send_Packet(&packet);
}

void UNetworkManager::Send_Packet(const void* data) {
	const char* packet = reinterpret_cast<const char*>(data);

	if (send(m_playerInfo.m_socket, packet, (size_t)packet[0], 0) == SOCKET_ERROR)
	{
		UE_LOG(LogTemp, Warning, TEXT("Send ERROR"));
	}

}

void UNetworkManager::Send_PlayerMove_packet()
{
	//Cs_packet_move_player packet;

	//packet.type = CS_MOVE_PLAYER;
	//packet.size = sizeof(packet);

	////Location
	//packet.x = m_playerInfo.m_x;
	//packet.y = m_playerInfo.m_y;
	//packet.z = m_playerInfo.m_z;

	////speed and direction
	//packet.speed = m_playerInfo.m_speed;
	//packet.direction = m_playerInfo.m_direction;

	//Send_Packet(&packet);
}

void UNetworkManager::Send_Rotator_Packet(float axisValue, FRotator rotator)
{
	//Cs_packet_rotator_player packet;
	//SetPlayerRotation(rotator);
	//packet.type = CS_ROTATER_PLAYER;
	//packet.size = sizeof(packet);
	////Rotation
	//packet.pitch = m_playerInfo.m_pitch;
	//packet.yaw = m_playerInfo.m_yaw;
	//packet.roll = m_playerInfo.m_roll;


	//Send_Packet(&packet);
}

//void UNetworkManager::EventPlayerProcess(APlayerController* player, PLAYER_EVENT_TYPE type)
//{
//	UE_Player_EVENT_TYPE ue_type;
//	switch (type) {
//	case PET_MOVE:
//		ue_type = SM_MOVE;
//		break;
//	case PET_JUMP:
//		ue_type = SM_JUMP;
//		break;
//	default:
//		UE_LOG(LogTemp, Error, TEXT(" An undefined Event Player packet received"));
//		break;
//	}
//	EventPlayerEvent.Broadcast(player, ue_type);
//}

void UNetworkManager::SetPlayerID(FString id)
{
	char *t = TCHAR_TO_ANSI(*id);
	strcpy_s(m_playerInfo.m_id,t);
}



void UNetworkManager::SetPlayerPosition(FVector& lhs) {
	m_playerInfo.m_x = lhs.X;
	m_playerInfo.m_y = lhs.Y;
	m_playerInfo.m_z = lhs.Z;
}

void UNetworkManager::SetPlayerCamera(FVector& lhs) {
	m_playerInfo.m_camerax = lhs.X;
	m_playerInfo.m_cameray = lhs.Y;
	m_playerInfo.m_cameraz = lhs.Z;
}

void UNetworkManager::SetPlayerRotation(FRotator& lhs) {
	m_playerInfo.m_pitch = lhs.Pitch;
	m_playerInfo.m_roll = lhs.Roll;
	m_playerInfo.m_yaw = lhs.Yaw;
}

void UNetworkManager::SetType(int pick)
{
	
	m_playerInfo.m_type = (PLAYER_POSITION_TYPE)pick;
}

int UNetworkManager::GetCID()
{
	return m_playerInfo.m_cid;
}


void UNetworkManager::SetPlayerVelocity(float speed, float direction) {
	m_playerInfo.m_speed = speed;
	m_playerInfo.m_direction = direction;
}

void PlayerInfo::clearoverlapped()
{
	m_over.Internal = 0;
	m_over.InternalHigh = 0;
	m_over.Offset = 0;
	m_over.OffsetHigh = 0;
}

PlayerInfo::PlayerInfo()
	:m_x(0.0f), m_y(0.0f), m_z(0.0f), m_pitch(0.0f), m_yaw(0.0f), m_roll(0.0f),
	m_speed(0.0f), m_direction(0.0f), m_index(0),
	m_action(-1), m_packet_size(0), m_saved_packet_size(0), m_type(PPT_NONE), m_cid(-1) {
	m_myController = nullptr;
	ZeroMemory(&m_wsabuf, sizeof(m_wsabuf));
	clearoverlapped();
}

PlayerInfo::PlayerInfo(float x, float y, float z, char id[MAX_ID_LEN], int cid)
	: m_x(x), m_y(y), m_z(z), m_cid(cid) {
	m_myController = nullptr;
	strcpy_s(m_id, id);
	m_type = PPT_NONE;
	clearoverlapped();
}

PlayerInfo::PlayerInfo(float x, float y, float z, char id[MAX_ID_LEN], float yaw, float pitch, float roll, int cid)
	: m_x(x), m_y(y), m_z(z), m_pitch(pitch), m_yaw(yaw), m_roll(roll), m_cid(cid)
{
	strcpy_s(m_id, id);
	m_type = PPT_NONE;
	m_myController = nullptr;
	clearoverlapped();
}

OtherPlayerInfo::OtherPlayerInfo() : m_x(0.0f), m_y(0.0f), m_z(0.0f), m_pitch(0.0f), m_yaw(0.0f), m_roll(0.0f),
m_speed(0.0f), m_direction(0.0f),  m_type(PPT_NONE), m_cid(-1)
{
	m_otherController = nullptr;
	m_connecting = false;
}

OtherPlayerInfo::OtherPlayerInfo(float x, float y, float z, char id[MAX_ID_LEN], APlayerController* controller, int cid)
{
	m_otherController = controller;
	m_x = x;
	m_y = y;
	m_z = z;
	m_roll = 0;
	m_yaw = 0;
	m_pitch = 0;
	m_speed = 0;
	m_direction = 0;
	strcpy_s(m_id, id);
	m_cid = cid;
	m_type = PPT_NONE;
	m_connecting = false;
}


