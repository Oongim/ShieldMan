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

	// ...

}
// Called every frame
void UNetworkManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
bool UNetworkManager::ConnectServer(FString IP) {
	GetIp(TCHAR_TO_ANSI(*IP));
	//GetIp("127.0.0.1");

	return StartServer();
}

void UNetworkManager::RecvPacket() {
	ObserveEvents();
}
void UNetworkManager::Send_MoveForward_Packet(float axisValue, FVector Position, float Speed, float Direction) {

	if (axisValue != 0) {
		SetPlayerPosition(Position);
		SetPlayerVelocity(Speed, Direction);
		Send_PlayerMove_packet();
	}
}
void UNetworkManager::Send_MoveRight_Packet(float axisValue, FVector Position, float Speed, float Direction) {

	if (axisValue != 0) {
		SetPlayerPosition(Position);
		SetPlayerVelocity(Speed, Direction);
		Send_PlayerMove_packet();
	}
}
void UNetworkManager::Send_MoveStop_Packet(FVector Position, float Speed, float Direction) {
	SetPlayerPosition(Position);
	SetPlayerVelocity(0.0f, Direction);
	Send_PlayerMove_packet();

}
void UNetworkManager::DisConnect() {

}

void UNetworkManager::ObserveEvents() {
	int retval;
	int i;
	//while (1) {
	i = WSAWaitForMultipleEvents(1, m_event, FALSE, 0, FALSE);
	if (i == WSA_WAIT_FAILED)
		return;

	i -= WSA_WAIT_EVENT_0;


	retval = WSAEnumNetworkEvents(m_playerInfo.m_socket, m_event[i], &m_netWorkEvents);

	if (retval == SOCKET_ERROR)
		return;


	//FD_READ
	if (m_netWorkEvents.lNetworkEvents & FD_READ) {

		if (m_netWorkEvents.iErrorCode[FD_READ_BIT] != 0) {
			UE_LOG(LogTemp, Warning, TEXT("netWorkEvents FD_READ ERROR"));
			return;
		}
		else {
			Read_Packet();
		}
	}
	//FD_CLOSE
	if (m_netWorkEvents.lNetworkEvents & FD_CLOSE) {
		if (m_netWorkEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
			UE_LOG(LogTemp, Warning, TEXT("netWorkEvents FD_CLOSE ERROR"));
		}


	}

	//	}


}

void UNetworkManager::Send_EventPlayer_Packet(UE_Player_EVENT_TYPE type) {

	Cs_packet_event_player packet;
	packet.size = sizeof(packet);
	packet.type = CS_EVENT_PLAYER;

	switch (type) {
	case SM_MOVE:
		packet.pt = PET_MOVE;
		break;
	case SM_JUMP:
		packet.pt = PET_JUMP;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT(" An undefined Event Player packet send"));
		break;
	}
	Send_Packet(&packet);
}

void UNetworkManager::GetIp(const char* ip) {

	m_ip = ip;
}
BOOL UNetworkManager::StartServer() {
	WSADATA wsa;

	int addrlen = 0;
	addrlen = sizeof(m_serverAddr);
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		UE_LOG(LogTemp, Error, TEXT("WSAstartup ERROR"));

		return false;
	}

	m_playerInfo.m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, NULL);
	if (m_playerInfo.m_socket == INVALID_SOCKET) {
		UE_LOG(LogTemp, Error, TEXT("INVALID SOCKET ERROR"));
		return false;
	}

	ZeroMemory(&m_serverAddr, sizeof(m_serverAddr));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(SERVER_PORT);

	//Get ip to Editer
	m_serverAddr.sin_addr.s_addr = inet_addr(m_ip);
	//Server Connect Error
	if (connect(m_playerInfo.m_socket, reinterpret_cast<SOCKADDR*>(&m_serverAddr), addrlen) == SOCKET_ERROR) {
		UKismetSystemLibrary::PrintString(this, "Time out Server \n");
		//	UE_LOG(LogTemp, Error, TEXT("connect ERROR"));
		return false;
	}

	m_connecting = TRUE;
	isConnect = true;

	m_event[nTotalSockets] = WSACreateEvent();
	if (WSAEventSelect(m_playerInfo.m_socket, m_event[nTotalSockets], FD_READ | FD_CLOSE) == SOCKET_ERROR) {
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("WSASyncSelect ERROR"));
		return false;
	}
	else {
		m_playerInfo.m_wsabuf.buf = m_playerInfo.m_buffer;
		m_playerInfo.m_wsabuf.len = MAX_BUFFER;
		return true;
	}
}
void UNetworkManager::Send_Packet(const void* data) {
	const char* packet = reinterpret_cast<const char*>(data);

	if (send(m_playerInfo.m_socket, packet, (size_t)packet[0], 0) == SOCKET_ERROR) {
		UE_LOG(LogTemp, Warning, TEXT("Send ERROR"));
	}
}
void UNetworkManager::Send_PlayerMove_packet() {
	Cs_packet_move_player packet;

	packet.type = CS_MOVE_PLAYER;
	packet.size = sizeof(packet);

	//Location
	packet.x = m_playerInfo.m_x;
	packet.y = m_playerInfo.m_y;
	packet.z = m_playerInfo.m_z;

	//speed and direction
	packet.speed = m_playerInfo.m_speed;
	packet.direction = m_playerInfo.m_direction;

	Send_Packet(&packet);
}
void UNetworkManager::Send_Rotator_Packet(float axisValue, FRotator rotator) {
	Cs_packet_rotator_player packet;
	SetPlayerRotation(rotator);
	packet.type = CS_ROTATER_PLAYER;
	packet.size = sizeof(packet);
	//Rotation
	packet.pitch = m_playerInfo.m_pitch;
	packet.yaw = m_playerInfo.m_yaw;
	packet.roll = m_playerInfo.m_roll;


	Send_Packet(&packet);
}

void UNetworkManager::Send_ChangeLevel_Packet() {
	Cs_packet_change_level packet;
	packet.size = sizeof(packet);
	packet.type = CS_CHANGE_LEVEL;
	Send_Packet(&packet);
}



void UNetworkManager::Read_Packet() {


	DWORD iobyte = 0, ioflag = 0;

	int ret = WSARecv(m_playerInfo.m_socket, &m_playerInfo.m_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);

	if (ret) {
		int err_code = WSAGetLastError();
		UE_LOG(LogTemp, Warning, TEXT("RECV PACKET ERROR %d"), err_code);

	}

	char* ptr = m_playerInfo.m_buffer;
	int rest = iobyte;
	char packet_size = 0;

	if (0 < m_playerInfo.m_saved_packet_size) { packet_size = m_playerInfo.m_recv_packet_buf[0]; }

	while (0 < rest) {

		if (0 == packet_size) { packet_size = ptr[0]; }
		int required = packet_size - m_playerInfo.m_saved_packet_size;

		if (required <= rest) {

			memcpy(m_playerInfo.m_recv_packet_buf + m_playerInfo.m_saved_packet_size, ptr, required);
			PacketProcess(m_playerInfo.m_recv_packet_buf);

			rest -= required;
			ptr += required;

			packet_size = 0;
			m_playerInfo.m_saved_packet_size = 0;
		}
		else {
			memcpy(m_playerInfo.m_recv_packet_buf + m_playerInfo.m_saved_packet_size, ptr, rest);
			rest = 0;
			m_playerInfo.m_saved_packet_size += rest;

		}

	}

}
void UNetworkManager::PacketProcess(const char* packet) {
	if (m_connecting) {

		switch (packet[1]) {
		case SC_MOVE_PLAYER: {
			//	MoveOtherPlayer(packet);
			const Sc_packet_move_player* movePlayer =
				reinterpret_cast<const Sc_packet_move_player*>(packet);

			auto iter = std::find_if(m_OtherPlayer.begin(), m_OtherPlayer.end(), [=](const OtherPlayerInfo& lhs) {
				return lhs.m_id == movePlayer->id;
				});

			if (iter != m_OtherPlayer.end() && iter->m_otherController != nullptr) {
				iter->m_x = movePlayer->x;
				iter->m_y = movePlayer->y;
				iter->m_z = movePlayer->z;
				iter->m_direction = movePlayer->direction;
				iter->m_speed = movePlayer->speed;

				if (iter->m_otherController->GetCharacter() != nullptr) {
					//Set Location
					iter->m_otherController->GetCharacter()->SetActorLocation(
						FVector{ iter->m_x,iter->m_y,iter->m_z });
					MovePlayerEvent.Broadcast(iter->m_otherController, FVector{ iter->m_x,iter->m_y,iter->m_z },
						iter->m_speed, iter->m_direction);
				}
			}
			break;
		}
		case SC_EVENT_PLAYER: {
			const Sc_packet_event_player* eventPlayer =
				reinterpret_cast<const Sc_packet_event_player*>(packet);

			auto iter = std::find_if(m_OtherPlayer.begin(), m_OtherPlayer.end(), [=](const OtherPlayerInfo& lhs) {
				return lhs.m_id == eventPlayer->id;
				});

			if (iter != m_OtherPlayer.end() && iter->m_otherController != nullptr) {
				EventPlayerProcess(iter->m_otherController, eventPlayer->pt);
			}
			break;
		}
		case SC_ROTATER_PLAYER: {
			const Sc_packet_rotator_player* rotatorPlayer =
				reinterpret_cast<const Sc_packet_rotator_player*>(packet);

			//Set Rotation
			auto iter = std::find_if(m_OtherPlayer.begin(), m_OtherPlayer.end(), [=](const OtherPlayerInfo& lhs) {
				return lhs.m_id == rotatorPlayer->id;
				});

			if (iter != m_OtherPlayer.end() && iter->m_otherController != nullptr) {
				iter->m_pitch = rotatorPlayer->pitch;
				iter->m_yaw = rotatorPlayer->yaw;
				iter->m_roll = rotatorPlayer->roll;

				if (iter->m_otherController->GetCharacter() != nullptr) {
					iter->m_otherController->GetCharacter()->SetActorRotation(FRotator{ 0.0f,iter->m_yaw,0.0f });
					iter->m_otherController->SetControlRotation(
						FRotator{ iter->m_pitch,iter->m_yaw,iter->m_roll });
					RotatorPlayerEvent.Broadcast(iter->m_otherController,
						FRotator{ iter->m_pitch,iter->m_yaw,iter->m_roll });
				}
			}
			break;
		}
		case SC_SPAWN_PLAYER: {
			UE_LOG(LogTemp, Warning, TEXT("PacketProcess:: recv SC_SPAWN_PLAYER"));

			const Sc_packet_spawn_player* spawnPlayer = reinterpret_cast<const Sc_packet_spawn_player*>(packet);

			FVector v{ spawnPlayer->x,spawnPlayer->y,spawnPlayer->z };
			FRotator r{ spawnPlayer->pitch,spawnPlayer->yaw,spawnPlayer->roll };
			float hp = spawnPlayer->hp;
			bool is_zoom = spawnPlayer->is_zoom;
			bool is_crouch = spawnPlayer->is_crouch;

			PutNewPlayerEvent.Broadcast(spawnPlayer->id, v, r, hp, is_zoom, is_crouch);
			break;
		}

		default:
			UE_LOG(LogTemp, Error, TEXT(" %d PacketProcess:: An undefined packet received"), (int)packet[1]);
			break;
		}
	}
}
void UNetworkManager::EventPlayerProcess(APlayerController* player, PLAYER_EVENT_TYPE type) {
	UE_Player_EVENT_TYPE ue_type;
	switch (type) {
	case PET_MOVE:
		ue_type = SM_MOVE;
		break;
	case PET_JUMP:
		ue_type = SM_JUMP;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT(" An undefined Event Player packet received"));
		break;
	}
	EventPlayerEvent.Broadcast(player, ue_type);
}


bool UNetworkManager::IsOtherPlayer(int32 playerIndex) {
	return m_playerInfo.m_index != playerIndex;
}
bool UNetworkManager::IsUsedOtherPlayerController(int32 playerIndex) {


	auto iter = std::find_if(m_OtherPlayer.begin(), m_OtherPlayer.end(), [=](const OtherPlayerInfo& lhs) {
		return lhs.m_id == playerIndex;
		});

	if (iter != m_OtherPlayer.end()) {
		return true;
	}
	else {
		return false;
	}

}
void UNetworkManager::SetOtherPlayerController(APlayerController* OtherPlayerController, int32 spawnNumber) {

	if (OtherPlayerController == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("OtherPlayerController Nullptr"));
	}
	m_OtherPlayer.emplace_back(OtherPlayerInfo{
			0.0f,0.0f,0.0f,
			static_cast<char>(spawnNumber),OtherPlayerController });

}
void UNetworkManager::PossessToController(int32 playerIndex, APawn* Pawn) {

	auto iter = std::find_if(m_OtherPlayer.begin(), m_OtherPlayer.end(), [=](const OtherPlayerInfo& lhs) {
		return lhs.m_id == playerIndex;
		});

	if (iter != m_OtherPlayer.end()) {
		iter->m_otherController->Possess(Pawn);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Sucess"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Found Controller"));
		UE_LOG(LogTemp, Warning, TEXT("Not Found Controller"));
	}
}
APlayerController* UNetworkManager::GetMyController() {

	if (m_playerInfo.m_myController == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("My Player Controller Nullptr"));

	return m_playerInfo.m_myController;
}
int32 UNetworkManager::SetMyController(APlayerController* Controller) {
	if (m_playerInfo.m_myController == nullptr) { m_playerInfo.m_myController = Controller; }

	return m_playerInfo.m_index;
}
APlayerController* UNetworkManager::GetControllerToIndex(int32 PlayerIndex) {

	if (m_playerInfo.m_index == PlayerIndex)
		return m_playerInfo.m_myController;

	else {

		auto iter = std::find_if(m_OtherPlayer.begin(), m_OtherPlayer.end(), [=](const OtherPlayerInfo& lhs) {
			return lhs.m_id == PlayerIndex;
			});
		if (iter != m_OtherPlayer.end()) {
			return iter->m_otherController;
		}

	}

	return nullptr;
}
void UNetworkManager::SetPlayerPosition(FVector& lhs) {
	m_playerInfo.m_x = lhs.X;
	m_playerInfo.m_y = lhs.Y;
	m_playerInfo.m_z = lhs.Z;
}
void UNetworkManager::SetPlayerRotation(FRotator& lhs) {
	m_playerInfo.m_pitch = lhs.Pitch;
	m_playerInfo.m_roll = lhs.Roll;
	m_playerInfo.m_yaw = lhs.Yaw;
}
void UNetworkManager::SetPlayerVelocity(float speed, float direction) {
	m_playerInfo.m_speed = speed;
	m_playerInfo.m_direction = direction;
}
PlayerInfo::PlayerInfo()
	:m_x(0.0f), m_y(0.0f), m_z(0.0f), m_pitch(0.0f), m_yaw(0.0f), m_roll(0.0f),
	m_speed(0.0f), m_direction(0.0f), m_index(0),
	m_action(-1), m_packet_size(0), m_saved_packet_size(0) {
	m_myController = nullptr;
	ZeroMemory(&m_wsabuf, sizeof(m_wsabuf));

}
PlayerInfo::PlayerInfo(float x, float y, float z, char id)
	: m_x(x), m_y(y), m_z(z), m_index(id) {
	m_myController = nullptr;
}
PlayerInfo::PlayerInfo(float x, float y, float z, char id, float yaw, float pitch, float roll)
	: m_x(x), m_y(y), m_z(z), m_pitch(pitch), m_yaw(yaw), m_roll(roll), m_index(id){
	m_myController = nullptr;
}
OtherPlayerInfo::OtherPlayerInfo() : m_x(0.0f), m_y(0.0f), m_z(0.0f), m_pitch(0.0f), m_yaw(0.0f), m_roll(0.0f),
m_speed(0.0f), m_direction(0.0f), m_id(-1) {
	m_otherController = nullptr;

}
OtherPlayerInfo::OtherPlayerInfo(float x, float y, float z, char id, APlayerController* controller) {
	m_otherController = controller;
	m_x = x;
	m_y = y;
	m_z = z;
	m_roll = 0;
	m_yaw = 0;
	m_pitch = 0;
	m_speed = 0;
	m_direction = 0;
	m_id = id;
}