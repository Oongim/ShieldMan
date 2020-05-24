// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "../Iocp_Server/Iocp_Server/global.h"
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../Iocp_Server/Iocp_Server/protocol.h"

#include "Components/ActorComponent.h"
#include "NetworkManager.generated.h"

UENUM(BlueprintType)
enum UE_Player_EVENT_TYPE {
	SM_MOVE, SM_JUMP
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FPutEventDelegate, int32, newPlayerindex, FVector, Position, FRotator, Rotator, float, hp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMoveEventDelegate, APlayerController*, movePlayer, FVector, Position, float, speed, float, direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRotatorEventDelegate, APlayerController*, RotatorPlayer, FRotator, Rotator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventPlayerDelegate, APlayerController*, eventPlayer, UE_Player_EVENT_TYPE, type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemovePlayerDelegate, APlayerController*, removePlayer);


struct OtherPlayerInfo {
public:
	APlayerController* m_otherController;
	float m_x, m_y, m_z;
	float m_pitch, m_yaw, m_roll;
	float m_speed, m_direction;
	char m_id;
	UE_Player_EVENT_TYPE m_player_event;
	OtherPlayerInfo(float x, float y, float z, char m_id, APlayerController*);
	OtherPlayerInfo();
};

struct PlayerInfo {
public:
	float m_x, m_y, m_z;
	float m_pitch, m_yaw, m_roll;
	float m_speed, m_direction;
	char m_index;
	UE_Player_EVENT_TYPE m_player_event;
	char m_action;
	FString m_id;
	SOCKET m_socket;
	char m_buffer[MAX_BUFFER];
	char m_recv_packet_buf[MAX_BUFFER];
	APlayerController* m_myController;
public:
	WSABUF m_wsabuf;
	DWORD m_packet_size = 0;
	int m_saved_packet_size = 0;

public:
	PlayerInfo();
	PlayerInfo(float x, float y, float z, char id, float yaw, float pitch, float roll);
	PlayerInfo(float x, float y, float z, char id);

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIELDMAN_API UNetworkManager : public UActorComponent
{
	GENERATED_BODY()
private:
	WSADATA m_wsa;
	SOCKADDR_IN m_serverAddr;
	BOOL m_connecting = FALSE;
	WSAEVENT m_event[WSA_MAXIMUM_WAIT_EVENTS];
	int nTotalSockets = 0;
	WSANETWORKEVENTS m_netWorkEvents;
	const char* m_ip = nullptr;
	PlayerInfo m_playerInfo;
	std::vector<OtherPlayerInfo> m_OtherPlayer;
public:
	// Sets default values for this component's properties
	UNetworkManager();
	void SetPlayerPosition(FVector&);
	void SetPlayerRotation(FRotator&);
	void SetPlayerVelocity(float, float);
	void GetIp(const char*);
	BOOL StartServer();
	void PacketProcess(const char*);
	void ObserveEvents();
	void Read_Packet();
	void Send_PlayerMove_packet();
	void Send_Packet(const void*);
	void EventPlayerProcess(APlayerController*, PLAYER_EVENT_TYPE);
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
		void Send_ChangeLevel_Packet();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void RecvPacket();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void DisConnect();

	/*  Other Player Funciton  */
	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		bool IsOtherPlayer(int32 playerIndex);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		bool IsUsedOtherPlayerController(int32 playerIndex);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void SetOtherPlayerController(APlayerController* OtherPlayerController, int32 spawnNumber);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		void PossessToController(int32 playerIndex, APawn* Pawn);

	/*  Get/Set MyController  */
	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		APlayerController* GetMyController();

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		int32 SetMyController(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, Category = "IocpServer")
		APlayerController* GetControllerToIndex(int32 playerIndex);

public:
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FPutEventDelegate PutNewPlayerEvent;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FMoveEventDelegate MovePlayerEvent;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FRotatorEventDelegate RotatorPlayerEvent;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FEventPlayerDelegate EventPlayerEvent;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FRemovePlayerDelegate RemovePlayerEvent;
};
