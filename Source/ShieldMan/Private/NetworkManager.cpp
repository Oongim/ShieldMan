// Fill out your copyright notice in the Description page of Project Settings.

#include "NetWorkManager.h"
// Sets default values for this component's properties
UNetworkManager::UNetworkManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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
	//UE_LOG(LogTemp, Error, TEXT("IP : %s", IP));
	GetIp(TCHAR_TO_ANSI(*IP));
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
		if (packet_size <= rest + m_playerInfo.m_saved_packet_size)
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
		case S2C_ENTER_OK:
		{
			const sc_packet_enter_ok* my_packet = reinterpret_cast<const sc_packet_enter_ok*>(packet);
			m_room = my_packet->room;
			m_type = my_packet->ctype;
			if (false == isConnect)
			{
				m_playerInfo.m_cid = my_packet->id;
				isConnect = true;
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

			m_isfull = packet_S2C_CONNECT->isfull;
			
			break;
		}

		case S2C_CHARACTERINFO:
		{
			const sc_packet_character_info* packet_S2C_character_info = reinterpret_cast<const sc_packet_character_info*>(packet);

			m_playerInfo.rp = packet_S2C_character_info->rp;
			m_playerInfo.ry = packet_S2C_character_info->ry;
			m_playerInfo.rr = packet_S2C_character_info->rr;
			m_playerInfo.lp = packet_S2C_character_info->lp;
			m_playerInfo.ly = packet_S2C_character_info->ly;
			m_playerInfo.lr = packet_S2C_character_info->lr;
			m_playerInfo.cx = packet_S2C_character_info->cx;
			m_playerInfo.cy = packet_S2C_character_info->cy;
			break;
		}
			
		case S2C_LEAVE:
		{
			const sc_packet_leave* packet_S2C_leave = reinterpret_cast<const sc_packet_leave*>(packet);
			DisConnect();
		}

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
	packet.host = m_host;
	strcpy_s(packet.name, m_playerInfo.m_id);

	Send_Packet(&packet);

}

void UNetworkManager::Send_Connect()
{
	cs_packet_connect packet;

	packet.type = C2S_CONNECT;
	packet.size = sizeof(packet);
	packet.room = m_room;
	Send_Packet(&packet);

}

void UNetworkManager::Send_InGame(float rp, float ry, float rr, float lp, float ly, float lr, float cx, float cy, float cz)
{
	cs_packet_in_game packet;

	packet.type = C2S_INGAME;
	packet.size = sizeof(packet);

	packet.rp = rp;
	packet.ry = ry;
	packet.rr = rr;
	packet.lp = lp;
	packet.ly = ly;
	packet.lr = lr;
	packet.cx = cx;
	packet.cy = cy;
	packet.cz = 0;
	packet.ctype = m_type;
	packet.room = m_room;
	packet.id = m_playerInfo.m_cid;
	Send_Packet(&packet);
}

void UNetworkManager::Send_Leave_Packet()
{
	cs_packet_leave packet;
	packet.size = sizeof(packet);
	packet.type = C2S_LEAVE;
	packet.room = m_room;
	Send_Packet(&packet);
}
void UNetworkManager::DisConnect()
{
	closesocket(m_playerInfo.m_socket);
}

void UNetworkManager::Send_Packet(const void* data) {
	const char* packet = reinterpret_cast<const char*>(data);

	if (send(m_playerInfo.m_socket, packet, (size_t)packet[0], 0) == SOCKET_ERROR)
	{
		UE_LOG(LogTemp, Warning, TEXT("Send ERROR"));
	}

}

void UNetworkManager::SetPlayerID(FString id)
{
	char* t = TCHAR_TO_ANSI(*id);
	strcpy_s(m_playerInfo.m_id, t);
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
	m_action(-1), m_packet_size(0), m_saved_packet_size(0), m_type(BODY), m_cid(-1) {
	m_myController = nullptr;
	ZeroMemory(&m_wsabuf, sizeof(m_wsabuf));
	clearoverlapped();
}

PlayerInfo::PlayerInfo(float x, float y, float z, char id[MAX_ID_LEN], int cid)
	: m_x(x), m_y(y), m_z(z), m_cid(cid) {
	m_myController = nullptr;
	strcpy_s(m_id, id);
	m_type = BODY;
	clearoverlapped();
}

PlayerInfo::PlayerInfo(float x, float y, float z, char id[MAX_ID_LEN], float yaw, float pitch, float roll, int cid)
	: m_x(x), m_y(y), m_z(z), m_pitch(pitch), m_yaw(yaw), m_roll(roll), m_cid(cid)
{
	strcpy_s(m_id, id);
	m_type = BODY;
	m_myController = nullptr;
	clearoverlapped();
}

