// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShieldManGameMode.h"
#include "ShieldManCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShieldManGameMode::AShieldManGameMode()
{
	DefaultPawnClass = AShieldManCharacter::StaticClass();
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<AShieldManCharacter> PlayerPawnBPClass(TEXT("/Game/BP/BP_SMCharacter.BP_SMCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}
