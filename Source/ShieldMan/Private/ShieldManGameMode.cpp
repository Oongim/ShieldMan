// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShieldManGameMode.h"
#include "ShieldManCharacter.h"
#include "SM_PlayerPawn.h"
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
