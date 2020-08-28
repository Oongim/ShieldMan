// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../IOCPGameServer/IOCPGameServer/IOCP.h"

#include "Components/ActorComponent.h"
#include "NetworkManager.generated.h"

UENUM(BlueprintType)
enum UE_Player_EVENT_TYPE {
	SM_MOVE, SM_JUMP
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMoveEventDelegate, APlayerController*, movePlayer, FVector, Position, float, speed, float, direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRotatorEventDelegate, APlayerController*, RotatorPlayer, FRotator, Rotator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventPlayerDelegate, APlayerController*, eventPlayer, UE_Player_EVENT_TYPE, type);

//enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT };
//
//struct EXOVER {
//	WSAOVERLAPPED 	over;
//	ENUMOP			op;
//	char			io_buf[MAX_BUFFER];
//	union {
//		WSABUF			wsabuf;
//		SOCKET			c_socket;
//	};
//};

struct OtherPlayerInfo {
public:
	APlayerController* m_otherController;
	float m_x, m_y, m_z;
	float m_camerax, m_cameray, m_cameraz;
	float m_pitch, m_yaw, m_roll;
	float m_speed, m_direction;
	char m_id[MAX_ID_LEN];
	int m_cid;
	bool m_changed;
	char m_name[MAX_ID_LEN];
	PLAYER_POSITION_TYPE m_type;
	UE_Player_EVENT_TYPE m_player_event;
	bool m_connecting;


	float rp = 0, ry = 0, rr = 0;
	float lp = 0, ly = 0, lr = 0;
	float cx = 0, cy = 0;

	OtherPlayerInfo(float x, float y, float z, char m_id[MAX_ID_LEN], APlayerController*, int cid);
	OtherPlayerInfo();
};

struct PlayerInfo {
public:
	float m_x, m_y, m_z;
	float m_camerax, m_cameray, m_cameraz;
	float m_pitch, m_yaw, m_roll;
	float m_speed, m_direction;
	char m_index;
	UE_Player_EVENT_TYPE m_player_event;
	char m_action;
	char m_id[MAX_ID_LEN];
	int m_cid;
	bool m_changed;
	PLAYER_POSITION_TYPE m_type;
	SOCKET m_socket;
	char m_buffer[MAX_BUFFER];
	char m_recv_packet_buf[MAX_BUFFER];
	APlayerController* m_myController;
public:
	WSABUF m_wsabuf;
	WSAOVERLAPPED m_over;
	DWORD m_packet_size = 0;
	int m_saved_packet_size = 0;

public:
	void clearoverlapped();
	PlayerInfo();
	PlayerInfo(float x, float y, float z, char id[MAX_ID_LEN], float yaw, float pitch, float roll, int cid);
	PlayerInfo(float x, float y, float z, char id[MAX_ID_LEN], int cid);

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHIELDMAN_API UNetworkManager : public UActorComponent
{
	GENERATED_BODY()
public:
	WSADATA m_wsa;
	SOCKADDR_IN m_serverAddr;
	BOOL m_connecting = FALSE;
	WSAEVENT m_event[WSA_MAXIMUM_WAIT_EVENTS];
	int nTotalSockets = 0;
	WSANETWORKEVENTS m_netWorkEvents;
	const char* m_ip = nullptr;
	int m_cid = 0;
	int m_recvcid = 0;
	int m_playercnt = 0;
	PlayerInfo m_playerInfo;
	OtherPlayerInfo m_OtherPlayer[3];
	FRotator temp = { 0.f,0.f,0.f };
public:
	// Sets default values for this component's properties
	UNetworkManager();
	void SetPlayerPosition(FVector&);
	void SetPlayerCamera(FVector&);
	void SetPlayerRotation(FRotator&);
	void SetPlayerVelocity(float, float);
	void GetIp(const char*);
	bool PacketProcess(const char*);
	bool Read_Packet();
	void Send_PlayerMove_packet();
	void Send_Packet(const void*);
	//void EventPlayerProcess(APlayerController*, PLAYER_EVENT_TYPE);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "IocpServer")
		FString SERVER_IP;

	UPROPERTY(BlueprintReadOnly, Category = "IocpServer")
		bool isConnect = false;

	UPROPERTY(BlueprintReadWrite, Category = "IocpServer")
		TMap<int32, AActor*>acornMap;
public:

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		bool ConnectServer(FString IP);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_MoveForward_Packet(float axisValue, FVector Position, float Speed, float Direction);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_MoveRight_Packet(float axisValue, FVector Position, float Speed, float Direction);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_MoveStop_Packet(FVector Position, float Speed, float Direction);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_EventPlayer_Packet(UE_Player_EVENT_TYPE type);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_Rotator_Packet(float axisValue, FRotator Rotator);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		bool RecvPacket();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void DisConnect();

public:
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FMoveEventDelegate MovePlayerEvent;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FRotatorEventDelegate RotatorPlayerEvent;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FEventPlayerDelegate EventPlayerEvent;

public:
	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_Enter_Packet();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_Pick_Position();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_Ready_Position();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_InGame(float x, float y, float z, float pitch, float yaw, float roll, float cx, float cy, float cz);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void Send_Connect();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		int GetCID();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void SetType(int pick);

	UFUNCTION(BlueprintCallable)
		void SetPlayerID(FString id);
};
