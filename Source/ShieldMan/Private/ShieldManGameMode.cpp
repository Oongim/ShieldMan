// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShieldManGameMode.h"
#include "ShieldManCharacter.h"
#include "SM_PlayerPawn.h"
#include "SM_GameState.h"
#include "SM_PlayerState.h"
#include "UObject/ConstructorHelpers.h"

AShieldManGameMode::AShieldManGameMode()
{
	DefaultPawnClass = AShieldManCharacter::StaticClass();

	static ConstructorHelpers::FClassFinder<AShieldManCharacter>MAINCHARACTERCLASS(TEXT(
		"/Game/BP/BP_SMCharacter.BP_SMCharacter_C"
	));
	MainCharacterClass = MAINCHARACTERCLASS.Class;

	static ConstructorHelpers::FClassFinder<ASM_PlayerPawn>ARMCHARACTERCLASS(TEXT(
		"/Game/MySM_PlayerPawn.MySM_PlayerPawn_C"
	));
	ArmCharacterClass = ARMCHARACTERCLASS.Class;
	GameStateClass = ASM_GameState::StaticClass();
	PlayerStateClass = ASM_PlayerState::StaticClass();

	PlayerNum = 0;
}

void AShieldManGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("InitGame")));
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), MainCharacterClass, FoundActors);

	if (0 == FoundActors.Num())
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("AShieldManCharacter")));
		DefaultPawnClass= MainCharacterClass;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("ArmCharacterClass")));
		DefaultPawnClass= ArmCharacterClass;
	}
	Super::InitGame(MapName, Options, ErrorMessage);
	
}

void AShieldManGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("PostLogin")));
	auto PS = Cast<ASM_PlayerState>(NewPlayer->PlayerState);
	if (nullptr == PS) {
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("ASM_PlayerState Nullptr")));
		return;
	}
	if (0 == PlayerNum) {
		PS->InitPlayerData(GetGameState<ASM_GameState>() ,{ "Body" });
	}
	else if (1 == PlayerNum) {
		PS->InitPlayerData(GetGameState<ASM_GameState>(),{ "RightArm" });
	}
	else if (2 == PlayerNum) {
		PS->InitPlayerData(GetGameState<ASM_GameState>(),{ "LeftArm" });
	}
	PlayerNum++;
	//GetGameState();
}

void AShieldManGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
