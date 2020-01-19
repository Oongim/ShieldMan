// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShieldManGameMode.h"
#include "ShieldManCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShieldManGameMode::AShieldManGameMode()
{
	DefaultPawnClass = AShieldManCharacter::StaticClass();
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}
