// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_GameInstance.h"
#include "NetworkManager.h"
#include "shellapi.h"

USM_GameInstance::USM_GameInstance()
{
	Stage1 = 0;
	Stage2 = 0;
	Stage3 = 0;

    networkManager= CreateDefaultSubobject<UNetworkManager>(TEXT("NETWORKMANAGER"));
    playercnt = 0;
}



void USM_GameInstance::SetTimeStage1(float time)
{
	Stage1 = time;
}
void USM_GameInstance::SetTimeStage2(float time)
{
	Stage2 = time;
}
void USM_GameInstance::SetTimeStage3(float time)
{
	Stage3 = time;
}

float USM_GameInstance::GetTimeStage1()
{
	return Stage1;
}
float USM_GameInstance::GetTimeStage2()
{
	return Stage2;
}
float USM_GameInstance::GetTimeStage3()
{
	return Stage3;
}

void USM_GameInstance::Host(const FString& Address)
{
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;
    
   

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;


       //Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("make")));
    if (true == networkManager->ConnectServer(Address))
    {
        networkManager->RecvPacket();
    }

    MakeRoom();
    

      // GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("%d"),playercnt));

       //TimerManager->SetTimer(HostTimer, this, &USM_GameInstance::Host, 1.f);
    
     

}

void USM_GameInstance::Join(const FString& Address)
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();

    if (true == networkManager->ConnectServer(Address))
    {
        networkManager->RecvPacket();
    }
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void USM_GameInstance::MakeRoom()
{
    networkManager->Send_Connect();
    networkManager->RecvPacket();
    if (networkManager->m_playercnt == 3)
    {
        UWorld* World = GetWorld();
        if (!ensure(World != nullptr)) return;
        World->ServerTravel("/Game/1Stage/Stage1?listen");
        networkManager->m_playercnt = 0;
    }
    else
    {
        TimerManager->SetTimer(HostTimer, this, &USM_GameInstance::MakeRoom, 1.f);
    }
}

UNetworkManager* USM_GameInstance::GetNetworkManager()
{
    return networkManager;
}
