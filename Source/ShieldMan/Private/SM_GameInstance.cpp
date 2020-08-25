// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_GameInstance.h"
#include "NetworkManager.h"


USM_GameInstance::USM_GameInstance()
{
	Stage1 = 0;
	Stage2 = 0;
	Stage3 = 0;

    networkManager= CreateDefaultSubobject<UNetworkManager>(TEXT("NETWORKMANAGER"));

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

void USM_GameInstance::Host()
{
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Hosting")));

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

   // networkManager->ConnectServer(FString{"127.0.0.1"});
    World->ServerTravel("/Game/1Stage/Stage1?listen");
}

void USM_GameInstance::Join(const FString& Address)
{
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    APlayerController* PlayerController = GetFirstLocalPlayerController();

    if (!ensure(Engine != nullptr)) return;
    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
   // networkManager->ConnectServer(Address);
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

}

UNetworkManager* USM_GameInstance::GetNetworkManager()
{
    return networkManager;
}
